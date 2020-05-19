/*
 * Isolabel
 * text position method
 *
 * module to take positions and node data values from isoline,
 * and generate text label positions and strings for annotation of the isoline
 *
 * Author:  John Sheehan, AVS Inc.
 * Date:    21 June 1999
 *
 * Updated  Ian Curington, AVS Inc.
 * Date:    28 June 1999
 * changed for full field input, 2D, 3D support, multiple
 * data types, unlimited number of labels, data value
 * controlled label strings, error processing,
 * IAC project structure guidelines.
 *
 */

#include "xp_comm_proj/isolabel/gen.h"

#include <avs/util.h>
#include <avs/err.h>
#include <avs/db.h>
#include <avs/fld.h>
#include <avs/arr.h>


/* set the upper limit of label size */
#define MAX_LABEL_SIZE 1024

/* define a indentity matrix for use when there is no input matrix */
static float IXFORM[16] = {1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0};

#define ERR_RETURN(MESS) {\
  ERRerror("isolabel",1,ERR_ORIG, MESS);\
  if (pos != NULL) ARRfree((float *)pos);\
  if (out_pos != NULL) ARRfree((float *)out_pos);\
  if (datavals != NULL) ARRfree((char *)datavals);\
  if (format) free(format);\
  return(0);\
}

/*
 * Uncomment the following line to get debug messages 
 */
/*
#define DEBUG 1
 */

#ifdef DEBUG
    #define PRINT(A) {printf("isolabel: %s\n",(A)); fflush(stdout); }
#else
    #define PRINT(A) /* noop */
#endif

int
isolabel_update(OMobj_id isolabel_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  pos_size = 0;
   float *pos = NULL; 
   int  out_pos_size = 0;
   float *out_pos = NULL; 
   int  datavals_size = 0;
   char  *datavals = NULL; 

   unsigned char *datavals_byte = NULL; 
   short *datavals_short = NULL; 
   int *datavals_int = NULL; 
   float *datavals_float = NULL; 
   double *datavals_double = NULL; 
   float val;

   int nnodes;
   int num_labels;
   char *format = NULL;

   int stat;
   float xform[16];

   OMobj_id in, out, string_vals_id;
   char tempstr[MAX_LABEL_SIZE];

   int nspace, ncomp, veclen, data_type;

   int i,k,count;



   /***********************/
   /* Function's Body     */
   /***********************/
   
	PRINT("starting update method");
   
   /***********************/
   /*  Get input values   */
   /***********************/
   /* get pointer to position list, data value list, and parameters */

   /* input field from isoline */
   in = OMfind_subobj(isolabel_id, OMstr_to_name("in"), OM_OBJ_RD);
   if (OMis_null_obj(in))
		ERR_RETURN("failed to get input field");

   /* output field to TextGlyph */
   out = OMfind_subobj(isolabel_id, OMstr_to_name("out"), OM_OBJ_RW);
   if (OMis_null_obj(out))
		ERR_RETURN("failed to get output field");

   /* get num labels */
   if (OMget_name_int_val(isolabel_id, OMstr_to_name("num_labels"), &num_labels) != 1) 
      count = num_labels = nnodes = 0;

   /* get text formatting string */
   if (OMget_name_str_val(isolabel_id, OMstr_to_name("format"), &format, 0) != 1)
      ERR_RETURN("failed to get format string");


	PRINT("got params and input field");

   /************************************************************/
   /*  if isolines are 0-length clean the output and return */
   /************************************************************/
   if (FLDget_nnodes(in, &nnodes) != 1) {
		ERR_RETURN("cannot get nnodes");
   }
	PRINT("got nnodes from input field");

   /*****************************************/
   /* get isolines coordinates and data  */
   /*****************************************/
   if (FLDget_node_data_ncomp(in, &ncomp) != 1) {
		ERR_RETURN("Error getting ncomp");
   }
   if (ncomp == 0) {
		ERR_RETURN("isolabel: no data component in the line input field");
   }
   if (FLDget_node_data_veclen(in, 0, &veclen) !=1) {
		ERR_RETURN("Error getting veclen");
   }
   if ( veclen != 1 ) 
		ERR_RETURN("veclen not 1, input must be scalar");

   if (FLDget_node_data(in, 0, &data_type, (char **)(&datavals),
			 &datavals_size, OM_GET_ARRAY_RD) != 1) {
		ERR_RETURN("cannot get node data");
   }

   /*****************************************/
   /* get data type for data array          */
   /*****************************************/
#ifdef DEBUG
printf(" data_type = %d\n",data_type);
#endif

   /* assign all pointers to the same array */
   datavals_byte =   (char *)   datavals;
   datavals_short =  (short *)  datavals;
   datavals_int =    (int *)    datavals;
   datavals_float =  (float *)  datavals;
   datavals_double = (double *) datavals;

   if (FLDget_nspace(in, &nspace) != 1) {
		ERR_RETURN("cannot get nspace");
   }
   if ( nspace <= 1 )
		ERR_RETURN("nspace must be at least 2D or 3D");

   if (FLDget_coord(in, &pos, &pos_size, OM_GET_ARRAY_RD) != 1) {
		ERR_RETURN("cannot get coordinates");
   }

	PRINT("got ncomp, veclen, node data, nspace, and coords from input field");

   /* This algorithm simply  skips through node list to select
      equally spaced nodes through the list,
      rather than looking for where they are in space, or
      what data values are picked up.
    */

   if (num_labels > nnodes ) num_labels = nnodes;

   if ( num_labels <= 0 )
       count = nnodes = 0;
   else {
       count = nnodes / num_labels;
       if ( count <= 0 ) {
            count = 1;
            num_labels = nnodes;
       }
   }

   if ( nnodes == 0 ) count = num_labels = 0;


	PRINT("calculated loop parameters");

#ifdef DEBUG
   printf("nnodes=%d, count=%d, num_labels=%d\n",
           nnodes, count, num_labels);
#endif

   /* set initial field output parameters */
   if (FLDset_nnodes (out, num_labels) != 1) {
       ERR_RETURN("Error setting nnodes");
   }

	PRINT("set nnodes");

   if (FLDset_nspace (out, nspace) != 1) {
      ERR_RETURN("Error setting nspace");
   }

	PRINT("set nspace");

   /* copy input transform to output */
   stat = FLDget_xform(in, xform);
   if (stat < 0) {
      ERR_RETURN("cannot get xform for input");
   }
   else if (stat == 0) 
      memcpy(xform, IXFORM, 16*sizeof(float));

   if (FLDset_xform (out, xform) != 1) {
      ERR_RETURN("Error setting xform");
   }

	PRINT("set xform");

   /* get output coordinate array */
   if (FLDget_coord(out, &out_pos, &out_pos_size, OM_GET_ARRAY_WR) != 1) {
      ERR_RETURN("Error setting coordinate array");
   }

#ifdef DEBUG
   printf("nnodes=%d, count=%d, num_labels=%d, out_size=%d\n",
           nnodes, count, num_labels, out_pos_size);
#endif

	PRINT("setup output field");


   /* Set string_vals array into output port structure*/ 
   string_vals_id = OMfind_subobj(isolabel_id, OMstr_to_name("string_vals"), OM_OBJ_RW);
   if (OMis_null_obj(string_vals_id))
		ERR_RETURN("failed to get string_vals_id");

   if (OMset_array_size(string_vals_id, 0) != 1) {
		ERR_RETURN("cannot zero size of label text array before processing");
   }
   if (OMset_array_size(string_vals_id, num_labels) != 1) {
		ERR_RETURN("cannot set size of label text array");
   }

	PRINT("fully prepared for label loop");

   /*****************************************/
   /* Main Loop                             */
   /*****************************************/
   /* loop over node list, skipping most, use num_labels to set stride */
   for (i=0, k=0; k<num_labels; i+=count, k++){

#ifdef DEBUG
   printf("i=%d, k=%d, nnodes=%d, count=%d, num_labels=%d\n",
           i,k,nnodes,count,num_labels);
#endif
	if ( data_type == DTYPE_BYTE)          {
		val = datavals_byte[i];
   	} else if ( data_type == DTYPE_SHORT)  {
		val = datavals_short[i];
   	} else if ( data_type == DTYPE_INT)    {
		val = datavals_int[i];
   	} else if ( data_type == DTYPE_FLOAT)  {
		val = datavals_float[i];
   	} else if ( data_type == DTYPE_DOUBLE) {
		val = datavals_double[i];
   	} else
		ERR_RETURN("invalid data type");


       /* convert sampled node data into a formatted text string */
       sprintf(tempstr, format, val);

       OMset_str_array_val(string_vals_id, k, tempstr);
	    PRINT("  label string array set");

       /* put 2D coordinate of string onto list, manage both 2D and 3D input */
       out_pos[k*nspace] = pos[i*nspace];
       out_pos[k*nspace+1] = pos[i*nspace+1];
       if ( nspace >= 3 )
           out_pos[k*nspace+2] = pos[i*nspace+2];

   }
   PRINT("label loop complete");

/********
        if (FLDget_ncell_sets(in, &nsets) != 1) {
		ERR_RETURN("cannot get nsets");
	}
	for (cs=0; cs<nsets; cs++) {
		if (FLDget_cell_set(in, cs, &cell_set) != 1) {
			ERR_RETURN("cannot get cell set");
		}
		if (FLDget_npolys(cell_set, &ncells) != 1) {
			ERR_RETURN("cannot get ncells");
		}
		if (FLDget_poly_connect(cell_set, &node_list, 
					&size, OM_GET_ARRAY_RD) != 1) {
			ERR_RETURN("cannot get cell connectivity");
		}
		for (cell=0; cell<ncells; cell++) {
			for(node=node_list[cell*2]; node<node_list[cell*2+1]; node++) {
				 datavals+node*veclen....;
				 pos[node*nspace]
*********/


   /*************************/
   /*  Free input variables */
   /*************************/
   if (pos != NULL) 
      ARRfree((float *)pos);

   if (out_pos != NULL) 
      ARRfree((float *)out_pos);

   if (datavals != NULL) 
      ARRfree((char *)datavals);

   if (format)
      free(format);

   PRINT("module finished.");

   return(1);
}
/**** end of file ****/


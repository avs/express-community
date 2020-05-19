
#include "xp_comm_proj/con2byt/gen.h"

#define MAX_NAME_SIZE 1024
#define ERROR(MESS) { ERRerror("ConvertToByte",1,ERR_ORIG, MESS); return 0; }

int
ConvertToByte(OMobj_id ConvertToByteNodeData_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/

   /* in object's data values */
   OMobj_id   in_id;
   int        in_ncomp, in_comp_count, in_veclen;
   int        in_data_type, in_ndata;
   char       *in_data;
   char       *in_min;
   char       *in_max;
   char       labels[MAX_NAME_SIZE];

   /*  out object's data values */
   OMobj_id   out_id;
   int        out_veclen;
   int        out_data_type, out_ndata;
   char       *out_data;
 

   /* scaling factor (always a float) */
   float  scale;

   /* index to data array position */
   int index; 


   /* in_data pointers for every data type */
   short *in_data_short;
   int  *in_data_int;
   float *in_data_float;
   double *in_data_double;


   /* Allocate memory for min & max values */
   /* allocate enough space so that any type can be accomodated */
   if (  ( (in_min= (char *)malloc(sizeof(double)*2)) == NULL) ||
         ( (in_max= (char *)malloc(sizeof(double)*2)) == NULL) ) ERROR("Unable to allocate memory for parameters\n"); 

  

   /*********************************/
   /*  Get input values for  field  */
   /*********************************/

   /* Get in field id */
     in_id = OMfind_subobj(ConvertToByteNodeData_id, OMstr_to_name("in"), OM_OBJ_RD);
  
   /* check "in" is an actual object, otherwise exit */
     if (OMis_null_obj(in_id)) ERROR("No input field.\n");

   /* Get out field id */
     out_id = OMfind_subobj(ConvertToByteNodeData_id, OMstr_to_name("out"), OM_OBJ_RW);
  

   /* Get in number of node data components */
     FLDget_node_data_ncomp (in_id, &in_ncomp);

   /* Set number of node data components */
     FLDset_node_data_ncomp (out_id, in_ncomp);


   /*******************************************/
   /* Apply conversion to each data component */
   /*******************************************/
   for (in_comp_count=0; in_comp_count < in_ncomp; in_comp_count++) {


      /**************************************/
      /* Get input values for the component */
      /**************************************/

      /* Get veclen */
         FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);


      /* Get data lable */
         if (FLDget_node_data_label(in_id, in_comp_count, labels, MAX_NAME_SIZE) != 1)
                       strcpy(labels, "");


      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */
         if  ( FLDget_node_data (in_id, in_comp_count, &in_data_type, &in_data,
                        &in_ndata, OM_GET_ARRAY_RD) != 1) ERROR("Unable to get input field data\n");


      /* Get Minimum and Maximum values */
         FLDget_node_data_minmax(in_id, in_comp_count, in_min, in_max);
 


      /*******************************/
      /* Allocate memory of out data */
      /*******************************/

         out_data_type = DTYPE_CHAR;
  
         if ((out_data = (char *)ARRalloc(NULL, out_data_type, in_veclen*in_ndata, NULL)) == NULL ) 
            ERROR(" Can not allocate memory for output data. \n");
       


      /***************************************/
      /* Perform processing                  */
      /* Dependant of datatype of input data */
      /***************************************/

/*  Use macro to perform generic process                                          */
#define GENERIC_CONV(D_TYPE,INPTR)                                                 \
      scale= (  (float)(*(D_TYPE *)in_max) - (float)(*(D_TYPE *)in_min) ) / 255;   \
      INPTR= ((D_TYPE *)in_data);                                                  \
      for (index= 0; index < in_ndata; index++) {                                  \
        out_data[index]= (unsigned char)((float)INPTR[index] / scale);}
/*  End of macro                                                                  */

        switch (in_data_type) {

            case DTYPE_CHAR: /* No conversion needed */
              for (index= 0; index < in_ndata; index++) out_data[index]= in_data[index];
	      break;

       	    case DTYPE_BYTE:
              for (index= 0; index < in_ndata; index++) out_data[index]= in_data[index];
	      break;
          
	    case DTYPE_SHORT:
	      GENERIC_CONV( short, in_data_short)    
	      break;

	    case DTYPE_INT:
	      GENERIC_CONV( int, in_data_int)
	      break;

	    case DTYPE_FLOAT:
	      GENERIC_CONV( float, in_data_float)
	      break; 

	    case DTYPE_DOUBLE:
	      GENERIC_CONV( double, in_data_double)
	      break;

	    default:
	      ERROR("Node data is of an unknown data type.");
        }


      /***************************************/
      /* Set output values of the component  */
      /***************************************/

      /* set new min & max values */
        *in_min= 0;
        *in_max= 255;
        if (FLDset_node_data_minmax (out_id, in_comp_count, in_min, in_max, DTYPE_BYTE) != 1) 
                     ERROR("Could not set output data.\n");

      /* copy label, veclen and units accross if there is one */
        if (FLDset_node_data_comp (out_id, in_comp_count, in_veclen, labels, "") != 1)
                     ERROR("Could not set output data.\n");

      /* Set node data */
        if (FLDset_node_data (out_id, in_comp_count, out_data, out_data_type,
                     in_ndata*in_veclen, OM_SET_ARRAY_FREE) != 1) ERROR("Could not set output data.\n");

      if (in_data)
         ARRfree((char *)in_data);
   }
   

   /* Free space used by min & max values */
   free(in_min);
   free(in_max);


   /* return to Object Manager with success flag */
   return(1);
}

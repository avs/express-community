/*
 */

/* ----------------------------------------------------------------------
 * ThresholdNodeData Module
 * ----------------------------------------------------------------------
 * Copies node data from input to output, applying threshold function.
 * Designed as example of simple data filter module, allocating new
 * output field, and copying data from an input port.
 *
 * Author: Nick Trout, January 15, 1996
 *
 * Revision: 2nd February 1996 - Ian Curington
 *   FLD call restructure
 *
 * Revision: 27th May 1997 - Paul G Lever
 *   Uses generated include files, rather than explicit AVS/Express or
 *   process specific include directives.
 * Revision 1st Oct 2011 - James S Perrin
 *   64bit compatible 
 * ----------------------------------------------------------------------
 * Note:
 *   The gen.h include file is generated by Express when the module is 
 *   compiled. It avoids including "user.h" or "express.h" directly, so 
 *   that the module may be moved across the processes by changing the V
 *   properties in the library.
 * ----------------------------------------------------------------------
 */

#define XP_WIDE_API
#include "xp_comm_proj/thresh/gen.h"

/*
 ***** MACROS *****
 */

/*
 * Error path: use ERRerror instead of printf,
 * since it reports nested context in messages
 */
#define ERROR(MESS) { ERRerror("Threshold",1,ERR_ORIG, MESS); return 0; }
#define SILENT_RTN(MESS) { return 0; }



/*********************************
 * Express Module Entry Point    *
 *********************************/

int
ThresholdNodeData(OMobj_id ThresholdNodeData_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/

   OMobj_id     in_id;
   int          in_ncomp, in_comp_count, in_veclen;
   int          in_data_type;
   xp_long      in_ndata;
   char         *in_data;
   OMobj_id     out_id;
   char         *out_data;
   double       minimum;
   double       maximum;

   /* declare variables for all the different data types used */

   xp_long       i;
   unsigned char *r_byte, *w_byte, min_byte, max_byte;
   char          *r_char, *w_char, min_char, max_char;
   short         *r_short, *w_short, min_short, max_short;
   int           *r_int, *w_int, min_int, max_int;
   xp_long       *r_long, *w_long, min_long, max_long;
   float         *r_float, *w_float, min_float, max_float;
   double        *r_double, *w_double, min_double, max_double;


   /***********************/
   /*  Get input values   */
   /***********************/

   /* Get minimum's value */
   if (OMget_name_real_val(ThresholdNodeData_id,     /* id of whole module */
			   OMstr_to_name("minimum"), /* name of param */
			   &minimum) != 1)           /* return value */
      ERROR("Could not get minimum range value.");

   /* Get maximum's value */
   if (OMget_name_real_val(ThresholdNodeData_id,     /* id of whole module */
			   OMstr_to_name("maximum"), /* name of param */
			   &maximum) != 1)           /* return value */
      ERROR("Could not get maximum range value.");


   /* Get the field id's so we can access the fields */

   /* Get field id */
   in_id= OMfind_subobj(ThresholdNodeData_id, OMstr_to_name("in"), OM_OBJ_RD);

   /* Get field id */
   out_id= OMfind_subobj(ThresholdNodeData_id, OMstr_to_name("out"), OM_OBJ_RW);


   /* Copy the number of data components (nnode_data) */

   /* Get number of node data components */
   FLDget_node_data_ncomp (in_id, &in_ncomp);

   /* Set number of node data components */
   FLDset_node_data_ncomp (out_id, in_ncomp);


   /* For each node data component get veclen, type and data array itself */

   for (in_comp_count=0; in_comp_count < in_ncomp; in_comp_count++) {

      /* Copy the vector length of the node data across */

      /* Get veclen */
      FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);

      /* Set veclen to same as input data */
      FLDset_node_data_veclen (out_id, in_comp_count, in_veclen);


      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

      if (FLDget_node_data (in_id,              /* field object id */
			    in_comp_count,      /* node_data[in_comp_count] */
			    &in_data_type,      /* data type returned */
			    &in_data,           /* pointer to data (char *) */
			    &in_ndata,          /* number of data elements */
			    OM_GET_ARRAY_RD)    /* we are reading the data */
	  != 1)
	ERROR("Could not get field node data.");



      /***
      printf("Type: %d, size: %d", in_data_type, in_ndata);
       ***/

      /*
       * Allocate New Output Array
       *  - same size & type as input array 
       */
      if ((out_data= (char *)ARRalloc(NULL,           /* always null */
				      in_data_type,   /* requested data type */
				      in_ndata,       /* requested size */
				      NULL)) == NULL) /* always null */
	ERROR("Could not allocate memory for thresholded array.");


      /* We will have to deal with each type of data seperately */

      /* Define a generic macro to deal with all of the data types */

#define GENERIC_THRESH(D_TYPE,RPTR,WPTR,MIN,MAX) \
      MIN= (D_TYPE)minimum; \
      MAX= (D_TYPE)maximum; \
      RPTR= (D_TYPE *)in_data; \
      WPTR= (D_TYPE *)out_data; \
      for (i= 0; i < in_ndata; i++) \
        WPTR[i]= ((RPTR[i] < MIN || RPTR[i] > MAX) ? (D_TYPE)0 : RPTR[i]);

      switch (in_data_type) {

        case DTYPE_CHAR:
          GENERIC_THRESH( char, r_char, w_char, min_char, max_char )
	  break;

	case DTYPE_BYTE:
          GENERIC_THRESH( unsigned char, r_byte, w_byte, min_byte, max_byte )
	  break;
          
	case DTYPE_SHORT:
	  GENERIC_THRESH( short, r_short, w_short, min_short, max_short )
	  break;

	case DTYPE_INT:
	  GENERIC_THRESH( int, r_int, w_int, min_int, max_int )
	  break;

	case DTYPE_LONG:
	  GENERIC_THRESH( xp_long, r_long, w_long, min_long, max_long )
	  break;

	case DTYPE_FLOAT:
	  GENERIC_THRESH( float, r_float, w_float, min_float, max_float )
	  break;

	case DTYPE_DOUBLE:
	  GENERIC_THRESH( double, r_double, w_double, min_double, max_double )
	  break;

	default:
	  ERROR("Node data is of an unknown data type.");
      }

      /* Set the output node_data's values. Since we are using ARRAY_FREE mode,
	 the OM will handle the array from now on and there is no need for us
	 to free it (like the in_data array). */

      if (FLDset_node_data (out_id,            /* output field id */
                            in_comp_count,     /* which component */
                            out_data,          /* pointer to output data */
                            in_data_type,      /* data type flag */
			    in_ndata,          /* number of data elements */
                            OM_SET_ARRAY_FREE) != 1)
	ERROR("Could not set thresholded node data.");

      /*  release our dependency on the input data pointer */
      /*  this should be done for each data component */
      ARRfree((char *)in_data);

   }
   

   /*
    * return to Object Manager with success flag
    */
   return(1);
}

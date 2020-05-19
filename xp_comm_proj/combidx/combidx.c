
#include "gen.h"

/* #define DEBUG /**/

int
CombineCoordinateIndex(OMobj_id CombineCoordinateIndex_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  x_index;
   int  y_index;
   int  z_index;
   int  dims_size = 0;
   int *dims = NULL; 
   int  output;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get x_index's value */ 
   if (OMget_name_int_val(CombineCoordinateIndex_id, OMstr_to_name("x_index"), &x_index) != 1) 
      x_index = 0;

   /* Get y_index's value */ 
   if (OMget_name_int_val(CombineCoordinateIndex_id, OMstr_to_name("y_index"), &y_index) != 1) 
      y_index = 0;

   /* Get z_index's value */ 
   if (OMget_name_int_val(CombineCoordinateIndex_id, OMstr_to_name("z_index"), &z_index) != 1) 
      z_index = 0;

   dims = (int *)OMret_name_array_ptr(CombineCoordinateIndex_id, OMstr_to_name("dims"), OM_GET_ARRAY_RD,
			&dims_size, NULL);


   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG   
   ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: CombineCoordinateIndex generated from method: CombineCoordinateIndex.Update\n");
#endif

   if (!dims_size) return;
   
   output = (z_index * dims[1] + y_index) * dims[0] + x_index;
	   	
   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set output's value */  
   OMset_name_int_val(CombineCoordinateIndex_id, OMstr_to_name("output"), output);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (dims != NULL) 
      ARRfree(dims);

   return(1);
}

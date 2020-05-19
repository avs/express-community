
#include "xp_comm_proj/drawcrop/cropdata/gen.h"


int
cropdat1(OMobj_id cropdata_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  dims_in_size = 0;
   int *dims_in = NULL; 
   int  set_coord;
   int  min_axis2_size = 2;
   float *min_axis2 = NULL; 
   int  max_axis2_size = 2;
   float *max_axis2 = NULL; 
   int  min_axis1_size = 2;
   float *min_axis1 = NULL; 
   int  max_axis1_size = 2;
   float *max_axis1 = NULL; 
   int  min_axis0_size = 2;
   float *min_axis0 = NULL; 
   int  max_axis0_size = 2;
   float *max_axis0 = NULL; 

   /***********************/
   /*  Get input values   */
   /***********************/
   dims_in = (int *)OMret_name_array_ptr(cropdata_id, OMstr_to_name("dims_in"), OM_GET_ARRAY_RD,
			&dims_in_size, NULL);

   /* Get set_coord's value */ 
   if (OMget_name_int_val(cropdata_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
      set_coord = 0;


   /***********************/
   /* Function's Body     */
   /***********************/
   
/*************************************************/
/*IF THE FILE HAS CHANGED ************************/
/*************************************************/     
   
   if (set_coord == 5)
       {
    
       

   /***********************/
   /*  Set output values  */
   /***********************/
	 
   /*set number of elements in array with min_axis2_size = ..*/
	 min_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_axis2_size, NULL);
	 
	 /*fill in array min_axis2*/
	 
	 min_axis2[0]=0;
	 min_axis2[1]=0;
	 
   OMset_name_array(cropdata_id, OMstr_to_name("min_axis2"), DTYPE_FLOAT, (void *)min_axis2, 
                    min_axis2_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array max_axis2_size = .. */
   max_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_axis2_size, NULL);
   
   /*fill in array max_axis2*/
   
	 max_axis2[0]=dims_in[0];
	 max_axis2[1]=dims_in[1];   
   
   
   OMset_name_array(cropdata_id, OMstr_to_name("max_axis2"), DTYPE_FLOAT, (void *)max_axis2, 
                    max_axis2_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array  min_axis1_size = .. */
   min_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_axis1_size, NULL);
   
   /*fill in array min_axis1*/
   
	 min_axis1[0]=0;
	 min_axis1[1]=0;
	 
   OMset_name_array(cropdata_id, OMstr_to_name("min_axis1"), DTYPE_FLOAT, (void *)min_axis1, 
                    min_axis1_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array  max_axis1_size = .. */
   max_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_axis1_size, NULL);
   /*fill in array max_axis1*/
   
	 max_axis1[0]=dims_in[0];
	 max_axis1[1]=dims_in[2];
	 
   OMset_name_array(cropdata_id, OMstr_to_name("max_axis1"), DTYPE_FLOAT, (void *)max_axis1, 
                    max_axis1_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array   min_axis0_size = .. */
   min_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_axis0_size, NULL);
   
   /*fill in array min_axis0*/
   
	 min_axis0[0]=0;
	 min_axis0[1]=0;
	 
   OMset_name_array(cropdata_id, OMstr_to_name("min_axis0"), DTYPE_FLOAT, (void *)min_axis0, 
                    min_axis0_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array  max_axis0_size = .. */
   max_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_axis0_size, NULL);
   /*fill in array max_axis0*/
   
	 max_axis0[0]=dims_in[1];
	 max_axis0[1]=dims_in[2];   
   OMset_name_array(cropdata_id, OMstr_to_name("max_axis0"), DTYPE_FLOAT, (void *)max_axis0, 
                    max_axis0_size, OM_SET_ARRAY_FREE);
       }


   /*************************/
   /*  Free input variables */
   /*************************/
   if (dims_in != NULL) 
      ARRfree((char *)dims_in);

   return(1);
}

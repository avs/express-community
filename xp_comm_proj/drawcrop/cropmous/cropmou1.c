
#include "gen.h"


int
crop_mouse1(OMobj_id crop_mouse_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  dims_in_size = 0;
   int *dims_in = NULL; 
   int  set_coord;
   int  min_vec_axis2_size = 2;
   float *min_vec_axis2 = NULL; 
   int  min_vec_axis1_size = 2;
   float *min_vec_axis1 = NULL; 
   int  min_vec_axis0_size = 2;
   float *min_vec_axis0 = NULL; 
   int  max_vec_axis2_size = 2;
   float *max_vec_axis2 = NULL; 
   int  max_vec_axis1_size = 2;
   float *max_vec_axis1 = NULL; 
   int  max_vec_axis0_size = 2;
   float *max_vec_axis0 = NULL; 



 
   /***********************/
   /*  Get input values   */
   /***********************/

   
   /* Get dims_in's value*/
      dims_in = (int *)OMret_name_array_ptr(crop_mouse_id, OMstr_to_name("dims_in"), OM_GET_ARRAY_RD,
			&dims_in_size, NULL);

   /* Get set_coord's value */ 
   if (OMget_name_int_val(crop_mouse_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
      set_coord = 0;


   /***********************/
   /* Function's Body     */
   /***********************/


   
/*************************************************/
/*IF THE FILE NAME HAS CHANGED *******************/
/*************************************************/   

   /*   printf("crop_mouse1 is running\n");*/
   

   if (set_coord == 5)
       {
    
    
	 /* printf("crop_mouse1 is running and set_coord is %d  \n",set_coord);*/
    

	 /***********************/
	 /*  Set output values  */
	 /***********************/
    
    
	 /*set number of elements in array with min_vec_axis2_size = ..*/ 
	 min_vec_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_vec_axis2_size, NULL);
    
    
	 /*fill in array min_vec_axis2*/
    
	 min_vec_axis2[0]=0;
	 min_vec_axis2[1]=0;
    
    
	 OMset_name_array(crop_mouse_id, OMstr_to_name("min_vec_axis2"), DTYPE_FLOAT, (void *)min_vec_axis2, 
                    min_vec_axis2_size, OM_SET_ARRAY_FREE);
	 
   

	 /*set number of elements in array with  min_vec_axis1_size = ...*/ 
	 min_vec_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_vec_axis1_size, NULL);
   
	 /*fill in array min_vec_axis1*/
   
	 min_vec_axis1[0]=0;
	 min_vec_axis1[1]=0;
   
   
	 OMset_name_array(crop_mouse_id, OMstr_to_name("min_vec_axis1"), DTYPE_FLOAT, (void *)min_vec_axis1, 
                    min_vec_axis1_size, OM_SET_ARRAY_FREE);
   
   
   
   
	 /*set number of elements in array with min_vec_axis0_size = ..*/ 
	 min_vec_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_vec_axis0_size, NULL);
   
	 /*fill in array min_vec_axis0*/
   
	 min_vec_axis0[0]=0;
	 min_vec_axis0[1]=0;
   
	 OMset_name_array(crop_mouse_id, OMstr_to_name("min_vec_axis0"), DTYPE_FLOAT, (void *)min_vec_axis0, 
                    min_vec_axis0_size, OM_SET_ARRAY_FREE);
   
   
   
	 /*set number of elements in array with  max_vec_axis2_size = ..*/ 
	 max_vec_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_vec_axis2_size, NULL);
   
	 /*fill in array max_vec_axis2*/
   
	 max_vec_axis2[0]=dims_in[0];
	 max_vec_axis2[1]=dims_in[1];

   
	 OMset_name_array(crop_mouse_id, OMstr_to_name("max_vec_axis2"), DTYPE_FLOAT, (void *)max_vec_axis2, 
                    max_vec_axis2_size, OM_SET_ARRAY_FREE);

	 /*set number of elements in array with  max_vec_axis1_size = ..*/
	 max_vec_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_vec_axis1_size, NULL);
   
	 /*fill in array max_vec_axis1*/
   
	 max_vec_axis1[0]=dims_in[0];
	 max_vec_axis1[1]=dims_in[2];

   
	 OMset_name_array(crop_mouse_id, OMstr_to_name("max_vec_axis1"), DTYPE_FLOAT, (void *)max_vec_axis1, 
                    max_vec_axis1_size, OM_SET_ARRAY_FREE);

	 /*set number of elements in array with max_vec_axis0_size = ..*/ 
	 max_vec_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_vec_axis0_size, NULL);
   
	 /*fill in array max_vec_axis0 */
   
	 max_vec_axis0[0]=dims_in[1];
	 max_vec_axis0[1]=dims_in[2];

   
   
	 OMset_name_array(crop_mouse_id, OMstr_to_name("max_vec_axis0"), DTYPE_FLOAT, (void *)max_vec_axis0, 
                    max_vec_axis0_size, OM_SET_ARRAY_FREE);
       }

   /*************************/
   /*  Free input variables */
   /*************************/


   if (dims_in != NULL) 
      ARRfree((char *)dims_in);

   return(1);
}

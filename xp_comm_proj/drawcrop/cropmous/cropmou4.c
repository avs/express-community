
#include "gen.h"


int
crop_mouse4(OMobj_id crop_mouse_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  set_coord;
   int  min_vec_axis0_cs_size = 2;
   float *min_vec_axis0_cs = NULL; 
   int  max_vec_axis0_cs_size = 2;
   float *max_vec_axis0_cs = NULL; 
   int  min_vec_axis0_size = 2;
   float *min_vec_axis0 = NULL; 
   int  max_vec_axis0_size = 2;
   float *max_vec_axis0 = NULL; 

   /*   ERRverror ("crop_mouse4", ERR_WARNING, "  start."); */
   
   
   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get set_coord's value */ 
   if (OMget_name_int_val(crop_mouse_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
      set_coord = 0;

   min_vec_axis0_cs = (float *)OMret_name_array_ptr(crop_mouse_id, OMstr_to_name("min_vec_axis0_cs"), OM_GET_ARRAY_RD,
			&min_vec_axis0_cs_size, NULL);

   max_vec_axis0_cs = (float *)OMret_name_array_ptr(crop_mouse_id, OMstr_to_name("max_vec_axis0_cs"), OM_GET_ARRAY_RD,
			&max_vec_axis0_cs_size, NULL);


   /***********************/
   /* Function's Body     */
   /***********************/
   
   /*   printf("crop_mouse4 is running\n");*/
  
   
   if (set_coord == 3)
     {
       /* printf("crop_mouse4 is running and set_coord = %d \n",set_coord);*/
       
       
       /***********************/
       /*  Set output values  */
       /***********************/
       /*set number of elements in array with   min_vec_axis0_size = ..*/
       min_vec_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_vec_axis0_size, NULL);

       /*fill in array min_vec_axis0*/

       min_vec_axis0[0]=min_vec_axis0_cs[0];
       min_vec_axis0[1]=min_vec_axis0_cs[1];


       OMset_name_array(crop_mouse_id, OMstr_to_name("min_vec_axis0"), DTYPE_FLOAT, (void *)min_vec_axis0, 
                    min_vec_axis0_size, OM_SET_ARRAY_FREE);


       /*set number of elements in array with max_vec_axis0_size = ..*/  
       max_vec_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_vec_axis0_size, NULL);

       /*fill in array max_vec_axis0*/

       max_vec_axis0[0]=max_vec_axis0_cs[0];
       max_vec_axis0[1]=max_vec_axis0_cs[1];


       OMset_name_array(crop_mouse_id, OMstr_to_name("max_vec_axis0"), DTYPE_FLOAT, (void *)max_vec_axis0, 
                    max_vec_axis0_size, OM_SET_ARRAY_FREE);
     }

   /*************************/
   /*  Free input variables */
   /*************************/
   if (min_vec_axis0_cs != NULL) 
      ARRfree((char *)min_vec_axis0_cs);

   if (max_vec_axis0_cs != NULL) 
     ARRfree((char *)max_vec_axis0_cs);
   
   /*  ERRverror ("crop_mouse4", ERR_WARNING, "  end.");*/
   
   
   return(1);
}

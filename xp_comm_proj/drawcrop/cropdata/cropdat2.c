
#include "xp_comm_proj/drawcrop/cropdata/gen.h"


int
cropdat2(OMobj_id cropdata_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  set_coord;
   int which_view;  
   int  min_in_size = 2;
   float *min_in = NULL; 
   int  max_in_size = 2;
   float *max_in = NULL; 
   int  min_axis2_size = 2;
   float *min_axis2 = NULL; 
   int  max_axis2_size = 2;
   float *max_axis2 = NULL; 


   OMobj_id min_in_id;
   OMobj_id max_in_id;   

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get set_coord's value */ 
   if (OMget_name_int_val(cropdata_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
     set_coord = 0;
   
   /* Get which_view's value */ 
   if (OMget_name_int_val(cropdata_id, OMstr_to_name("which_view"), &which_view) != 1) 
     which_view = 0;
      

   min_in = (float *)OMret_name_array_ptr(cropdata_id, OMstr_to_name("min_in"), OM_GET_ARRAY_RD,
			&min_in_size, NULL);

   max_in = (float *)OMret_name_array_ptr(cropdata_id, OMstr_to_name("max_in"), OM_GET_ARRAY_RD,
					  &max_in_size, NULL);
   
   
   /*   box_in = (int *)OMret_name_array_ptr(cropdata_id, OMstr_to_name("box_in"), OM_GET_ARRAY_RD, 
					&box_in_size, NULL);*/


   /***********************/
   /* Function's Body     */
   /***********************/

   min_in_id=OMfind_subobj(cropdata_id, OMstr_to_name("min_in"),
OM_OBJ_RD); /*This is used to see if the mouse has moved and the value changed*/
   
   max_in_id=OMfind_subobj(cropdata_id, OMstr_to_name("max_in"),
OM_OBJ_RD); /*This is used to see if the mouse has moved and the value changed*/

  
   
 if( (set_coord ==1) && (which_view == 0) && ((OMchanged(min_in_id, seq_num) ==
1) || (OMchanged(max_in_id, seq_num) == 1)))
     {   


   /***********************/
   /*  Set output values  */
   /***********************/
   /*set number of elements in array min_axis2_size = ..  */
   min_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, min_axis2_size, NULL);
   /*fill in array min_axis2*/
   
     min_axis2[0]=min_in[0];
     min_axis2[1]=min_in[1];
   
   /*min_axis2[0]=box_in[0];
   min_axis2[1]=box_in[1];*/         
   
   OMset_name_array(cropdata_id, OMstr_to_name("min_axis2"), DTYPE_FLOAT, (void *)min_axis2, 
                    min_axis2_size, OM_SET_ARRAY_FREE);

   /*set number of elements in array max_axis2_size = ..*/
   max_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, max_axis2_size, NULL);
   /*fill in array max_axis2*/
   
   max_axis2[0]=max_in[0];
   max_axis2[1]=max_in[1];   
   
   /*max_axis2[0]=box_in[2];
   max_axis2[1]=box_in[3];*/

      
   OMset_name_array(cropdata_id, OMstr_to_name("max_axis2"), DTYPE_FLOAT, (void *)max_axis2, 
                    max_axis2_size, OM_SET_ARRAY_FREE);
     }

   /*************************/
   /*  Free input variables */
   /*************************/
   if (min_in != NULL) 
      ARRfree((char *)min_in);

   if (max_in != NULL) 
     ARRfree((char *)max_in);
   
   /*   if (box_in != NULL)
     ARRfree((char *)box_in);*/

   return(1);
}

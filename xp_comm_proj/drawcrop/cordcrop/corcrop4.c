
#include "gen.h"


int
coordcrop4(OMobj_id crop_coordinate_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
  /***********************/
   int  set_coord;  
   int  mins_store_size = 3;
   int *mins_store = NULL; 
   int  maxs_store_size = 3;
   int *maxs_store = NULL; 
   int  mins_size = 3;
   int *mins = NULL; 
   int  maxs_size = 3;
   int *maxs = NULL; 

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get set_coord's value */    
   if (OMget_name_int_val(crop_coordinate_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
     set_coord = 0;
   
   mins_store = (int *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("mins_store"), OM_GET_ARRAY_RD,
			&mins_store_size, NULL);

   maxs_store = (int *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("maxs_store"), OM_GET_ARRAY_RD,
			&maxs_store_size, NULL);


   /***********************/
   /* Function's Body     */
   /***********************/
   
   
   /* ERRverror ("coord4", ERR_WARNING, "  start."); */
   

   /*SET FINAL VALUES*/
   
   
   if (set_coord == 4) 
     {
       
       /***********************/
       /*  Set output values  */
       /***********************/
       
       /*set number of elements in array mins with mins_size = ..*/ 
       mins = (int *)ARRalloc(NULL, DTYPE_INT, mins_size, NULL);
       
       /*fill in array mins*/
       
       
       mins[0]=mins_store[0];
       mins[1]=mins_store[1];
       mins[2]=mins_store[2];       
       

       OMset_name_array(crop_coordinate_id, OMstr_to_name("mins"), DTYPE_INT, (void *)mins, 
			mins_size, OM_SET_ARRAY_FREE);
       
       /*set number of elements in array maxs with maxs_size = ...*/
       maxs = (int *)ARRalloc(NULL, DTYPE_INT, maxs_size, NULL);
       
       /*fill in array maxs*/
       
       
       maxs[0]=maxs_store[0];
       maxs[1]=maxs_store[1];
       maxs[2]=maxs_store[2];       
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("maxs"), DTYPE_INT, (void *)maxs, 
			maxs_size, OM_SET_ARRAY_FREE);
     }

   /*************************/
   /*  Free input variables */
   /*************************/
   if (mins_store != NULL) 
      ARRfree((char *)mins_store);

   if (maxs_store != NULL) 
     ARRfree((char *)maxs_store);
   
   /*ERRverror ("coord4", ERR_WARNING, "  end.");*/

   return(1);
}

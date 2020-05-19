
#include "xp_comm_proj/drawcrop/cropcont/gen.h"


int
crop_control(OMobj_id crop_controls_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  dims_fld_size = 0;
   int *dims_fld = NULL; 
   int  restart;
   int  OK_axis2;
   int  OK_axis1;
   int  OK_axis0;
   int  all_OK;
   int  set_coord;
   
   OMobj_id  dims_fld_id;

   /***********************/
   /*  Get input values   */
   /***********************/
   dims_fld = (int *)OMret_name_array_ptr(crop_controls_id, OMstr_to_name("dims_fld"), OM_GET_ARRAY_RD,
			&dims_fld_size, NULL);

   /* Get restart's value */ 
   if (OMget_name_int_val(crop_controls_id, OMstr_to_name("restart"), &restart) != 1) 
      restart = 0;

   /* Get OK_axis2's value */ 
   if (OMget_name_int_val(crop_controls_id, OMstr_to_name("OK_axis2"), &OK_axis2) != 1) 
      OK_axis2 = 0;

   /* Get OK_axis1's value */ 
   if (OMget_name_int_val(crop_controls_id, OMstr_to_name("OK_axis1"), &OK_axis1) != 1) 
      OK_axis1 = 0;

   /* Get OK_axis0's value */ 
   if (OMget_name_int_val(crop_controls_id, OMstr_to_name("OK_axis0"), &OK_axis0) != 1) 
      OK_axis0 = 0;

   /* Get all_OK's value */ 
   if (OMget_name_int_val(crop_controls_id, OMstr_to_name("all_OK"), &all_OK) != 1) 
      all_OK = 0;


   /***********************/
   /* Function's Body     */
   /***********************/
   /*THIS UPDATE FUNCTION IS TO COORDINATE THE BUTTON PRESSES*/

	
   set_coord=0;  /*This is for initiallizing the value*/
   
   dims_fld_id=OMfind_subobj(crop_controls_id, OMstr_to_name("dims_fld"), OM_OBJ_RD); /*This is used to see if the file has changed*/
   


   /*RESET TOGGLES FOR DISALLOWED STATES*/

   if (OK_axis2 == 1 && OK_axis1 == 1 && OK_axis0 ==1)
     {

       /*reset OK_axis0 to 0*/

       OK_axis0=0;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis0"), OK_axis0);
     }



   if ( (OK_axis2 == 0 && OK_axis1 == 1) || (OK_axis2 == 0 && OK_axis0 == 1) )
     {
       /*reset OK_axis2 to 1 AND OK_axis1 and OK_axis0 to 0*/

       OK_axis2=1;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis2"), OK_axis2);

       OK_axis1=0;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis1"), OK_axis1);

       OK_axis0=0;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis0"), OK_axis0);
     }
   
   if (OK_axis2 == 0 && all_OK == 1)
     {
       /*reset OK_axis2 to start and all_OK to stop crop action*/
       
       OK_axis2=1;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis2"), OK_axis2);
       
       all_OK=0;
       OMset_name_int_val(crop_controls_id, OMstr_to_name("all_OK"), all_OK);
     }
       



   /*SET SET_COORD VALUES*/



   if (OK_axis2 == 1 && OK_axis1 == 0 && OK_axis0 ==0 && all_OK == 0 && restart ==0)
     {set_coord=1;}

   if (OK_axis2 == 1 && OK_axis1 == 1 && OK_axis0 ==0 && all_OK == 0 && restart ==0)
     {set_coord=2;}

   if (OK_axis2 == 1 && OK_axis1 == 0 && OK_axis0 ==1 && all_OK == 0 && restart ==0)
     {set_coord=3;}


   if (all_OK == 1)
     {

     set_coord=4;  

     all_OK=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("all_OK"), all_OK);
     }
   
   


   if ((restart == 1) || (OMchanged(dims_fld_id, seq_num) == 1))
     {set_coord=5;

     restart=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("restart"), restart);

     OK_axis2=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis2"), OK_axis2);

     OK_axis1=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis1"), OK_axis1);

     OK_axis0=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("OK_axis0"), OK_axis0);

     all_OK=0;
     OMset_name_int_val(crop_controls_id, OMstr_to_name("all_OK"), all_OK);
     }

   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set set_coord's value */  
   OMset_name_int_val(crop_controls_id, OMstr_to_name("set_coord"), set_coord);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (dims_fld != NULL) 
      ARRfree((char *)dims_fld);

   return(1);
}




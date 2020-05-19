
#include "gen.h"


int
crop_mouse5(OMobj_id crop_mouse_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  set_coord;
   int  reset_coord;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get set_coord's value */ 
   if (OMget_name_int_val(crop_mouse_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
      set_coord = 0;


   /***********************/
   /* Function's Body     */
   /***********************/

	reset_coord = set_coord;


   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set reset_coord's value */  
   OMset_name_int_val(crop_mouse_id, OMstr_to_name("reset_coord"), reset_coord);
   return(1);
}

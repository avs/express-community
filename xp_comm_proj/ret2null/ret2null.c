
#include "xp_comm_proj/ret2null/gen.h"


int
ret2null_update(OMobj_id Ret2Null_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  trigger;
   OMobj_id fld_id;
   OMobj_id value_id;
   OMobj_id trigger_id;


   /***********************/
   /*  Get input values   */
   /***********************/

   /* Get obj_id for ui_field object */
   fld_id = OMfind_str_subobj(Ret2Null_id, "ui_field", OM_OBJ_RW);
   if OMis_null_obj(fld_id) {
      printf("Cannot find ui_field object\n");
      return 0;
   }

   /* Get obj_id for value sub-object */
   value_id = OMfind_str_subobj(fld_id, "value", OM_OBJ_RW);
   if OMis_null_obj(value_id) {
      printf("Cannot find value sub-object of ui_field object\n");
      return 0;
   }

   /* Get obj_id for trigger object */
   trigger_id = OMfind_str_subobj(Ret2Null_id, "trigger", OM_OBJ_RW);
   if OMis_null_obj(trigger_id) {
      printf("Cannot find trigger object\n");
      return 0;
   }

   /***********************/
   /* Function's Body     */
   /***********************/
   /* ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: ret2null_update generated from method: ReturnToNull.update\n");*/

   /* Check if trigger parameter has changed */
   if (!OMchanged(trigger_id, seq_num)) {
      printf("Trigger parameter has not changed\n");
      return 1;
   }

   /* Check trigger's value & change field value if appropriate */ 
  
   OMget_int_val(trigger_id, &trigger);
      
   if (trigger == 1) {
      OMset_obj_val(value_id, OMnull_obj, 0);
      OMset_int_val(trigger_id, 0);
      printf("It worked!\n");
   }
   else
   {
      printf("Don't change value until button is pushed.\n");
   }   

   return 1;
}


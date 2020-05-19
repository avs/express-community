#include "gen.h"


int
SetCursorExampleMod_update(OMobj_id CursorExampleMod_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  busy_enable;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get busy_enable's value */ 
   if (OMget_name_int_val(CursorExampleMod_id, OMstr_to_name("busy_enable"), &busy_enable) != OM_STAT_SUCCESS)
      busy_enable = 0;


   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: SetCursorExampleMod_update generated from method: SetCursorExampleMod.update\n");*/

   SetApplicationCursor(busy_enable);

   return(1);
}



#include "gen.h"


int
DisplayErrorExampleMod_update(OMobj_id ErrorExampleMod_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *title = NULL;
   char  *message = NULL;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get title's value */
   if (OMget_name_str_val(ErrorExampleMod_id, OMstr_to_name("title"), &title, 0) != OM_STAT_SUCCESS) {
      printf("error: unable to get title string\n");
      return(0);
   }

   /* Get message's value */
   if (OMget_name_str_val(ErrorExampleMod_id, OMstr_to_name("message"), &message, 0) != OM_STAT_SUCCESS) {
      printf("error: unable to get message string\n");
      if (title==NULL) free(title);
      return(0);
   }


   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: DisplayErrorExampleMod_update generated from method: DisplayErrorExampleMod.update\n");*/

   DisplayErrorDialog(title, message);


   /*************************/
   /*  Free input variables */
   /*************************/
   if (title==NULL)
      free(title);

   if (message==NULL)
      free(message);

   return(1);
}


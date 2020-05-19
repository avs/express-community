
#include "gen.h"


int
init_mask(OMobj_id init_mask_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  init,i;
   int  mask_size = 0;
   int  mask_num = 0;
   unsigned char *mask = NULL; 

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get init's value */ 
   if (OMget_name_int_val(init_mask_id, OMstr_to_name("init"), &init) != 1) 
      init = 0;

   if (OMget_name_int_val(init_mask_id, OMstr_to_name("mask_num"), &mask_num) != 1) 
      mask_num = 0;

   mask = (unsigned char *)OMret_name_array_ptr(init_mask_id, OMstr_to_name("mask"), OM_GET_ARRAY_RW,
			&mask_size, NULL);


   /***********************/
   /* Function's Body     */
   /***********************/
  
   if(init)
   {
	 #ifdef DEBUG
	   fprintf(stderr,"init_mask\n");fflush(stderr);
     #endif
     for (i=0;i< mask_size; i++)
       if (mask[i]==mask_num) mask[i]=0;
	 init=0;
	 OMset_name_int_val(init_mask_id, OMstr_to_name("init"), init);
   }

   /*************************/
   /*  Free input variables */
   /*************************/
   if (mask != NULL) 
      ARRfree(mask);

   return(1);
}

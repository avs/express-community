#include "gen.h"

//#define DEBUG 

int
create_outm(OMobj_id create_outm_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  smooth;
   int  do_;
   int  t1;
   OMobj_id  in_id;
   int       mask_array_size = 0;
   unsigned char  *mask_array = NULL; 
   
   int i,k,min,max;

   #ifdef DEBUG
  	 fprintf(stderr,"create_outm\n");fflush(stderr);
   #endif

   in_id   = OMfind_subobj(create_outm_id, 
             OMstr_to_name("in"), OM_OBJ_RD);
   if (!OMchanged(create_outm_id,seq_num))
       return(0);
   mask_array = (unsigned char *)OMret_name_array_ptr(
                                 create_outm_id, 
                                 OMstr_to_name("mask_array"), 
                                 OM_GET_ARRAY_RW,
			                        &mask_array_size, NULL);
   for (i=0;i< mask_array_size; i++)
       mask_array[i]=0;

   
   if (mask_array != NULL) 
      ARRfree(mask_array);

   return(1);
}

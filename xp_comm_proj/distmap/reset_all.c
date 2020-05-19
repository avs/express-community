
#include "gen.h"
//#define DEBUG


int
reset_all(OMobj_id reset_all_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  reset;
   int  init_point0_size = 0;
   int *init_point0 = NULL; 
   int  init_point1_size = 0;
   int *init_point1 = NULL;
   OMobj_id in_buf_id;
   int  i;
   int  mask_size = 0;
   unsigned char *mask = NULL;

   #ifdef DEBUG
	fprintf(stderr,"reset_all called\n");fflush(stderr);
   #endif


   /***********************/
   /*  Get input values   */
   /***********************/


	if (OMget_name_int_val(reset_all_id,
       OMstr_to_name("reset"), &reset) != 1) 
      reset = 0;

   init_point0 = (int *)OMret_name_array_ptr(reset_all_id,
                        OMstr_to_name("init_point0"), OM_GET_ARRAY_RW,
			&init_point0_size, NULL);

   init_point1 = (int *)OMret_name_array_ptr(reset_all_id,
                        OMstr_to_name("init_point1"), OM_GET_ARRAY_RW,
			&init_point1_size, NULL);

   in_buf_id = OMfind_subobj(reset_all_id, 
               OMstr_to_name("in_buf"), OM_OBJ_RD);

   mask = (unsigned char *)OMret_name_array_ptr(reset_all_id, 
                           OMstr_to_name("mask"), OM_GET_ARRAY_RW,
			                  &mask_size, NULL);

   if(reset)
   {
	 #ifdef DEBUG
	   fprintf(stderr,"reset_all init_point0_size %i\n",init_point0_size);fflush(stderr);
     #endif
     for (i=0;i<init_point0_size;i++)
       init_point0[i]=-1.;
     for (i=0;i<init_point1_size;i++)
       init_point1[i]=-1.;
     if (OMchanged(in_buf_id,seq_num))
       for (i=0;i<init_point1_size;i++)
         init_point0[i]=init_point1[i]=-1.;
     for (i=0;i< mask_size; i++)
       if (mask[i]!=0) mask[i]=0;
	 reset=0;
	 OMset_name_int_val(reset_all_id, OMstr_to_name("reset"), reset);
   }
     
   if (init_point0 != NULL) 
      ARRfree((char *)init_point0);

   if (init_point1 != NULL) 
      ARRfree((char *)init_point1);

   if (mask != NULL) 
      ARRfree(mask);

   return(1);
}

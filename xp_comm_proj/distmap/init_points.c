#include "gen.h"


//#define DEBUG 
#ifdef DEBUG
#include <stdio.h>
#endif

int
init_pt(OMobj_id init_pts_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  init_points0;
   int  init_points1;
   int  init_point0_size = 0;
   int *init_point0 = NULL; 
   int  init_point1_size = 0;
   int *init_point1 = NULL;
   OMobj_id in_buf_id;
   int i;

   #ifdef DEBUG
	fprintf(stderr,"init_points called\n");fflush(stderr);
   #endif


   /***********************/
   /*  Get input values   */
   /***********************/

   /* Get init_point's value */ 
   if (OMget_name_int_val(init_pts_id,
       OMstr_to_name("init_inside_points"), &init_points0) != 1) 
      init_points0 = 0;

   if (OMget_name_int_val(init_pts_id,
       OMstr_to_name("init_outside_points"), &init_points1) != 1) 
      init_points1 = 0;

   init_point0 = (int *)OMret_name_array_ptr(init_pts_id,
                        OMstr_to_name("init_point0"), OM_GET_ARRAY_RW,
			&init_point0_size, NULL);

   init_point1 = (int *)OMret_name_array_ptr(init_pts_id,
                        OMstr_to_name("init_point1"), OM_GET_ARRAY_RW,
			&init_point1_size, NULL);

   in_buf_id = OMfind_subobj(init_pts_id, 
               OMstr_to_name("in_buf"), OM_OBJ_RD);

   if (init_points0)
      for (i=0;i<init_point0_size;i++)
        init_point0[i]=-1.;
   if (init_points1)
      for (i=0;i<init_point1_size;i++)
         init_point1[i]=-1.;
   if (OMchanged(in_buf_id,seq_num))
      for (i=0;i<init_point1_size;i++)
         init_point0[i]=init_point1[i]=-1.;
     
   if (init_point0 != NULL) 
      ARRfree((char *)init_point0);

   if (init_point1 != NULL) 
      ARRfree((char *)init_point1);

   return(1);
}

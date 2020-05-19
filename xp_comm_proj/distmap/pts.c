#include "gen.h"

//#define DEBUG
int add_pt(OMobj_id add_point_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  point_size = 0;
   int *point = NULL; 
   int  axis;
   int  plane;
   int  sw;
   int  init_point0_size = 0;
   int *init_point0 = NULL; 
   int  init_point1_size = 0;
   int *init_point1 = NULL;
   int  pt[3], *s_pt, i, j;

#ifdef DEBUG
	fprintf(stderr,"add_pts called\n");fflush(stderr);
#endif


   point = (int *)OMret_name_array_ptr(add_point_id,
                  OMstr_to_name("point"), OM_GET_ARRAY_RD,
			         &point_size, NULL);

   /* Get axis's value */ 
   if (OMget_name_int_val(add_point_id, OMstr_to_name("axis"), &axis) != 1) 
      axis = 0;

   /* Get plane's value */ 
   if (OMget_name_int_val(add_point_id, OMstr_to_name("plane"), &plane) != 1) 
      plane = 0;

   /* Get switch's value */ 
   if (OMget_name_int_val(add_point_id, OMstr_to_name("switch"), &sw) != 1) 
      sw = 0;

   init_point0 = (int *)OMret_name_array_ptr(add_point_id,
         OMstr_to_name("init_point0"), OM_GET_ARRAY_RW,
			&init_point0_size, NULL);

   init_point1 = (int *)OMret_name_array_ptr(add_point_id,
         OMstr_to_name("init_point1"), OM_GET_ARRAY_RW,
			&init_point1_size, NULL);


	if (sw==0) s_pt=init_point0;
	else       s_pt=init_point1;

   if (axis==0)
      {
      pt[0]=plane;
      pt[1]=point[0];
      pt[2]=point[1];
      }
   if (axis==1)
      {
      pt[1]=plane;
      pt[0]=point[0];
      pt[2]=point[1];
      }
   if (axis==2)
      {
      pt[2]=plane;
      pt[0]=point[0];
      pt[1]=point[1];
      }
      
   for (i=0;i<init_point0_size;i+=3)
      if (s_pt[i]<0) break;
   for (j=0;j<3;j++)
      s_pt[i+j]=pt[j];

   /*************************/
   /*  Free input variables */
   /*************************/
   if (point != NULL) 
      ARRfree((char *)point);

   if (init_point0 != NULL) 
      ARRfree((char *)init_point0);

   if (init_point1 != NULL) 
      ARRfree((char *)init_point1);

   return(1);
}


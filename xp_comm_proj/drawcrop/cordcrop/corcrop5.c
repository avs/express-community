
#include "gen.h"


int
coordcrop5(OMobj_id crop_coordinate_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  set_coord;
   int  dims_in_size = 3;
   int *dims_in = NULL; 
   double  z_scale;
   int  stored_size = 4;
   int *stored = NULL;
   int  mins_store_size = 3;
   int *mins_store = NULL; 
   int  maxs_store_size = 3;
   int *maxs_store = NULL; 
   int  xlate1_axis2_size = 8;
   float *xlate1_axis2 = NULL; 
   int  xlate2_axis2_size = 8;
   float *xlate2_axis2 = NULL; 
   int  xlate1_axis1_size = 8;
   float *xlate1_axis1 = NULL; 
   int  xlate2_axis1_size = 8;
   float *xlate2_axis1 = NULL; 
   int  xlate1_axis0_size = 8;
   float *xlate1_axis0 = NULL; 
   int  xlate2_axis0_size = 8;
   float *xlate2_axis0 = NULL; 
   
   
   int i=0;
   int xmin=0,ymin=0,zmin=0,xmax=0,ymax=0,zmax=0;
   
   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get set_coord's value */ 
   if (OMget_name_int_val(crop_coordinate_id, OMstr_to_name("set_coord"), &set_coord) != 1) 
      set_coord = 0;

   dims_in = (int *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("dims_in"), OM_GET_ARRAY_RD,
			&dims_in_size, NULL);

   /* Get z_scale's value */
   if (OMget_name_real_val(crop_coordinate_id, OMstr_to_name("z_scale"), &z_scale) != 1)
      z_scale = 0.0;


   /***********************/
   /* Function's Body     */
   /***********************/
   
   /* ERRverror ("coord5", ERR_WARNING, "  start."); */
   
   
   /*NO SQUARES HAVE BEEN ACCEPTED OR THE RESET IS PRESSED*/
   
   /* printf("coordcrop5 is running \n");*/
   
   
   if (set_coord == 5)
     {
       
       /* printf("coordcrop5 is running and set_coord = %d \n",set_coord);*/
       
       xmin=0;
       ymin=0;
       zmin=0;
       
       xmax=dims_in[0]-1;
       ymax=dims_in[1]-1;
       zmax=dims_in[2]-1;
       
   
       /***********************/
       /*  Set output values  */
       /***********************/
       

       /*FOR AXIS 2*/
             
       /*set number of elements in array stored*/
       stored = (int *)ARRalloc(NULL, DTYPE_INT, stored_size, NULL);
       
       /*fill in array stored*/
       
       stored[0]=xmin;
       stored[1]=ymin;
       stored[2]=xmax;
       stored[3]=ymax;
              
       OMset_name_array(crop_coordinate_id, OMstr_to_name("stored"), DTYPE_INT, (void *)stored, 
			stored_size, OM_SET_ARRAY_FREE);
       
       /*ERRverror ("coord5", ERR_WARNING, "  after stored."); */    
       
       /*set number of elements in array mins_store*/
       mins_store = (int *)ARRalloc(NULL, DTYPE_INT, mins_store_size, NULL);
       
       /*fill in array mins_store*/
       
       mins_store[0]=xmin;
       mins_store[1]=ymin;
       mins_store[2]=zmin;
              
       OMset_name_array(crop_coordinate_id, OMstr_to_name("mins_store"), DTYPE_INT, (void *)mins_store, 
			mins_store_size, OM_SET_ARRAY_FREE);

       /* set number of elements in array maxs_store*/
       maxs_store = (int *)ARRalloc(NULL, DTYPE_INT, maxs_store_size, NULL);
       
       /*fill in array maxs_store*/
       
       maxs_store[0]=xmax;
       maxs_store[1]=ymax;
       maxs_store[2]=zmax;
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("maxs_store"), DTYPE_INT, (void *)maxs_store, 
			maxs_store_size, OM_SET_ARRAY_FREE);
       
       
       

      
       /********************************/       
       /*ZMAX NOW CHANGED FOR Z_SCALING*/
       /********************************/
       
       zmax = zmax * z_scale;
       
       
       
       
       /*FOR AXIS 2*/  
       

       /*set number of elements in array xlate1_axis2*/
       xlate1_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate1_axis2_size, NULL);
       /*fill in array xlate1_axis2*/
       
       /*first line of the cross*/
       xlate1_axis2[0]=0;
       xlate1_axis2[1]=0; /*the origin*/       
       
       xlate1_axis2[2]=xmax;
       xlate1_axis2[3]=0; /*the end of x dim line*/       
       
       /*second line of the cross*/
       xlate1_axis2[4]=0;
       xlate1_axis2[5]=0; /*the origin*/
       
       
       xlate1_axis2[6]=0; 
       xlate1_axis2[7]=ymax; /*end of the y dim line*/


       /*set values in array xlate1_axis2*/ 
              
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate1_axis2"), DTYPE_FLOAT, (void *)xlate1_axis2, 
			xlate1_axis2_size, OM_SET_ARRAY_FREE);

       /* set number of elements in array xlate2_axis2*/
       xlate2_axis2 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate2_axis2_size, NULL);
       /*fill in array xlate2_axis2*/
       
       /*for the first line of the cross*/
       xlate2_axis2[0]=xmax;
       xlate2_axis2[1]=ymax; /*point furthest from the origin*/
       
       xlate2_axis2[2]=xmax;
       xlate2_axis2[3]=0; /*end of x line*/
       
       
       /*for the second line of the cross*/       
       xlate2_axis2[4]=xmax;
       xlate2_axis2[5]=ymax; /*point furthest from the origin*/       
       
       xlate2_axis2[6]=0;
       xlate2_axis2[7]=ymax; /*end of y line*/
              

       /*set values in array xlate2_axis2*/
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate2_axis2"), DTYPE_FLOAT, (void *)xlate2_axis2, 
			xlate2_axis2_size, OM_SET_ARRAY_FREE);

       /*FOR AXIS 1*/
       

       /*set number of elements in array xlate1_axis1*/
       
       xlate1_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate1_axis1_size, NULL);
       /*fill in array xlate1_axis1*/
       
       /*for the first line of the cross*/       
       xlate1_axis1[0]=0;
       xlate1_axis1[1]=0; /*the origin*/
              
       xlate1_axis1[2]=xmax;
       xlate1_axis1[3]=0; /*end of z line*/       
       
       /*for the second line of the cross*/       
       xlate1_axis1[4]=0;
       xlate1_axis1[5]=0; /*the origin*/       
       
       xlate1_axis1[6]=0;
       xlate1_axis1[7]=zmax; /*end of y line*/
              


       /*set values in array xlate1_axis1*/
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate1_axis1"), DTYPE_FLOAT, (void *)xlate1_axis1, 
			xlate1_axis1_size, OM_SET_ARRAY_FREE);
       
       
       /*set number of elements in array xlate2_axis1*/
       xlate2_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate2_axis1_size, NULL);
       /*fill in array xlate2_axis1*/
       
       /*for the first line of the cross*/
       
       xlate2_axis1[0]=xmax;
       xlate2_axis1[1]=zmax; /*point furthest from the origin*/
       
       xlate2_axis1[2]=xmax;
       xlate2_axis1[3]=0; /*end of z line*/
       
       
       /*for the second line of the cross*/      
       xlate2_axis1[4]=xmax;
       xlate2_axis1[5]=zmax; /*point furthest from the origin*/
            
       xlate2_axis1[6]=0;
       xlate2_axis1[7]=zmax; /*end of y line*/       
       
       
       /*set values in array xlate2_axis1*/
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate2_axis1"), DTYPE_FLOAT, (void *)xlate2_axis1, 
			xlate2_axis1_size, OM_SET_ARRAY_FREE);
       
       
       /*FOR AXIS 0*/       

       /*set number of elements in array xlate1_axis0*/        
       xlate1_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate1_axis0_size, NULL);
       /*fill in array xlate1_axis0*/
       
       /*for the first line*/       
       xlate1_axis0[0]=0;
       xlate1_axis0[1]=0; /*the origin*/
       
       xlate1_axis0[2]=ymax;
       xlate1_axis0[3]=0; /*end of the y line*/
             
       /*for the second line of the cross*/       
       xlate1_axis0[4]=0;
       xlate1_axis0[5]=0; /*the origin*/
       
       xlate1_axis0[6]=0; 
       xlate1_axis0[7]=zmax; /*end of the z line*/
       
   
       /*set output values in array  xlate1_axis0*/
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate1_axis0"), DTYPE_FLOAT, (void *)xlate1_axis0, 
			xlate1_axis0_size, OM_SET_ARRAY_FREE);
       
       
       
       /*set number of elements in array xlate2_axis0*/
       xlate2_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate2_axis0_size, NULL);
       
       /*fill in array xlate2_axis0*/
       
       /*for the first line of the cross*/
       xlate2_axis0[0]=ymax;
       xlate2_axis0[1]=zmax; /*point furthest from origin*/
       
       xlate2_axis0[2]=ymax;
       xlate2_axis0[3]=0;  /*end of y line*/
             
       /*for the second line of the cross*/       
       xlate2_axis0[4]=ymax;
       xlate2_axis0[5]=zmax; /*point furthest from origin*/
              
       xlate2_axis0[6]=0; 
       xlate2_axis0[7]=zmax; /*end of z line*/
              

       /*set output values in array  xlate2_axis0*/
       
       
   OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate2_axis0"), DTYPE_FLOAT, (void *)xlate2_axis0, 
                    xlate2_axis0_size, OM_SET_ARRAY_FREE);
   
     }

   /*************************/
   /*  Free input variables */
   /*************************/
   
   if (dims_in != NULL) 
     ARRfree((char *)dims_in);
   
   /*ERRverror ("coord5", ERR_WARNING, "  end."); */

   return(1);
}

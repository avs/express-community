
#include "gen.h"


int
coordcrop2(OMobj_id crop_coordinate_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  set_coord;
   int  dims_in_size = 3;
   int *dims_in = NULL; 
   double  z_scale;
   int  min_vec_axis1_size = 2;
   float *min_vec_axis1 = NULL; 
   int  max_vec_axis1_size = 2;
   float *max_vec_axis1 = NULL; 
   int  stored_size = 4;
   int *stored = NULL;
   int  mins_store_size = 3;
   int *mins_store = NULL; 
   int  maxs_store_size = 3;
   int *maxs_store = NULL; 
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

   min_vec_axis1 = (float *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("min_vec_axis1"), OM_GET_ARRAY_RD,
			&min_vec_axis1_size, NULL);

   max_vec_axis1 = (float *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("max_vec_axis1"), OM_GET_ARRAY_RD,
			&max_vec_axis1_size, NULL);

   stored = (int *)OMret_name_array_ptr(crop_coordinate_id, OMstr_to_name("stored"), OM_GET_ARRAY_RD,
			&stored_size, NULL);



   /***********************/
   /* Function's Body     */
   /***********************/
   
   /****************************************************************/   
   /*THE SECOND SQUARE HAS BEEN ACCEPTED****************************/   
   /*SET TRANSFORMS FOR AXIS 1 AND 0 *******************************/   
   /****************************************************************/
   
   /*   printf("coordcrop2 is running \n");*/
   

   if (set_coord == 2)
     {
       /*printf("coordcrop2 is running and set_coord = %d \n",set_coord);*/
       
       /*GET RELEVENT VALUES*/
       

       xmin=stored[0];
       ymin=stored[1];
       zmin=(int)   (min_vec_axis1[1]);

       xmax=stored[2];
       ymax=stored[3];
       zmax=(int)   (max_vec_axis1[1]);
       
       
       

       if(zmin < 0)
	 {
	   zmin=0;
	 }

       if(zmax > (dims_in[2]-1))
	 {
	   zmax=(dims_in[2]-1);
	 }
       
	 
		    
       /*set number of elements in array mins_store*/
       mins_store = (int *)ARRalloc(NULL, DTYPE_INT, mins_store_size, NULL);
       
       /*fill in array mins_store*/
       
       mins_store[0]=xmin;
       mins_store[1]=ymin;
       mins_store[2]=zmin;
       
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("mins_store"), DTYPE_INT, (void *)mins_store, 
			mins_store_size, OM_SET_ARRAY_FREE);

       /*set number of elements in array maxs_store*/       
       maxs_store = (int *)ARRalloc(NULL, DTYPE_INT, maxs_store_size, NULL);
       
       /*fill in array maxs_store*/
       
       maxs_store[0]=xmax;
       maxs_store[1]=ymax;
       maxs_store[2]=zmax;
       
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("maxs_store"), DTYPE_INT, (void *)maxs_store, 
			maxs_store_size, OM_SET_ARRAY_FREE);
       
       
       /***********************************************/
       /* NOW CHANGE THE Z_SCLAE VALUE SO IT IS SCALED*/
       /***********************************************/
       
       zmin = zmin * z_scale;
       zmax = zmax * z_scale;
       

       /***********************/
       /*  Set output values  */
       /***********************/
       

       
       /*FOR AXIS 1*/
       
       /*set number of elements in array xlate1_axis1*/
       xlate1_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate1_axis1_size, NULL);
       
       /*fill in array xlate1_axis1*/
       
       xlate1_axis1[0]=xmin;
       xlate1_axis1[1]=zmin;
       xlate1_axis1[2]=xmin;
       xlate1_axis1[3]=zmax;     
       
       xlate1_axis1[4]=xmin;
       xlate1_axis1[5]=zmin;
       xlate1_axis1[6]=xmax;
       xlate1_axis1[7]=zmin;

   
       /*set values in array xlate1_axis1*/      
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate1_axis1"), DTYPE_FLOAT, (void *)xlate1_axis1, 
			xlate1_axis1_size, OM_SET_ARRAY_FREE);
       
       
       /*set number of elements in array xlate2_axis1*/
       xlate2_axis1 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate2_axis1_size, NULL);
       
       /*fill in array xlate2_axis1*/
       

       xlate2_axis1[0]=xmax;
       xlate2_axis1[1]=zmax;
       xlate2_axis1[2]=xmax;
       xlate2_axis1[3]=zmin;     
       
       xlate2_axis1[4]=xmax;
       xlate2_axis1[5]=zmax;
       xlate2_axis1[6]=xmin;
       xlate2_axis1[7]=zmax;

       /*set values in array xlate2_axis1*/
       
       
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate2_axis1"), DTYPE_FLOAT, (void *)xlate2_axis1, 
			xlate2_axis1_size, OM_SET_ARRAY_FREE);
       
       /*FOR AXIS 0*/
       
       /*set number of elements in array xlate1_axis0*/ 
       xlate1_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate1_axis0_size, NULL);
       
       /*fill in array xlate1_axis0*/
       
       xlate1_axis0[0]=ymin;
       xlate1_axis0[1]=zmin;
       xlate1_axis0[2]=ymin;
       xlate1_axis0[3]=zmax;     
       
       xlate1_axis0[4]=ymin;
       xlate1_axis0[5]=zmin;
       xlate1_axis0[6]=ymax;
       xlate1_axis0[7]=zmin;


       /*set output values in array  xlate1_axis0*/
              
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate1_axis0"), DTYPE_FLOAT, (void *)xlate1_axis0, 
			xlate1_axis0_size, OM_SET_ARRAY_FREE);
       
       
       /*set number of elements in array xlate2_axis0*/
       xlate2_axis0 = (float *)ARRalloc(NULL, DTYPE_FLOAT, xlate2_axis0_size, NULL);
       /*fill in array xlate2_axis0*/
   

       xlate2_axis0[0]=ymax;
       xlate2_axis0[1]=zmax;
       xlate2_axis0[2]=ymax;
       xlate2_axis0[3]=zmin;     
   
       xlate2_axis0[4]=ymax;
       xlate2_axis0[5]=zmax;
       xlate2_axis0[6]=ymin;
       xlate2_axis0[7]=zmax;
   

       /*set output values in array  xlate2_axis0*/  
   
       OMset_name_array(crop_coordinate_id, OMstr_to_name("xlate2_axis0"), DTYPE_FLOAT, (void *)xlate2_axis0, 
			xlate2_axis0_size, OM_SET_ARRAY_FREE);
     }
   
   
   /*************************/
   /*  Free input variables */
   /*************************/
   if (dims_in != NULL) 
      ARRfree((char *)dims_in);

   if (min_vec_axis1 != NULL) 
      ARRfree((char *)min_vec_axis1);

   if (max_vec_axis1 != NULL) 
      ARRfree((char *)max_vec_axis1);

   if (stored != NULL) 
      ARRfree((char *)stored);



   return(1);
}

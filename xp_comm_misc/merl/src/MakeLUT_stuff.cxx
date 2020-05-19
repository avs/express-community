/*****************************************************************************

                        Copyright (c) 1998 by
                        Advanced Visual Systems Inc.
                        All Rights Reserved

        This software comprises unpublished confidential information of
        Advanced Visual Systems Inc. and may not be used, copied or made
        available to anyone, except in accordance with the license
        under which it is furnished.

******************************************************************************

This file contains a method for converting from an Express datamap to a VLI
look-up table (LUT) for use with the VLIVolume renderer.

******************************************************************************
*****************************************************************************/

#include <math.h>
#include <malloc.h>
#include "MakeLUT_gen.h"

extern "C" {
void COLORhsv_to_rgb(float,float,float,float *,float *,float *);
void COLORrgb_to_hsv(float,float,float,float *,float *,float *);
}

const int XP_MAX_DMAP_SIZE = 256; // XP color and alpha map size
const int VLI_ALPHA_MAX = 4095;  // Alpha goes 0-4095 in VLI
const int VLI_COLOR_MAX = 255;  // R,G,B go 0-255 in VLI
const int NUM_VALS = 4;  // R,G,B and alpha

enum XPcolorModels
{
   XP_HSV = 0,
   XP_RGB
};

enum XPrangeModels
{
   XP_LINEAR = 0,
   XP_CONSTANT
};


int
Utility_macros_MakeLUT::MakeLUT_update(OMevent_mask event_mask, int seq_num)
{
  //LUTsize = OMXint
  //in_dmap = OMXgroup =>input datamap
  
  OMobj_id subobjId,arrId,subArrId;
  OMobj_id DatamapId,DataRangeId;
  float v[NUM_VALS],v2[NUM_VALS],*tmp_v;
  double range_min,range_max,tmp_val;
  int arr_size;
  int numRang,colorModel,colorType,alphaType,range_size;
  int status,i,j,indx;
  
  int alpha_array[XP_MAX_DMAP_SIZE];
  char red_array[XP_MAX_DMAP_SIZE];
  char green_array[XP_MAX_DMAP_SIZE];
  char blue_array[XP_MAX_DMAP_SIZE];
  
  // Check table size
  status = OMget_int_val(LUTsize,&arr_size);
  if (arr_size > XP_MAX_DMAP_SIZE)
  {
    ERRverror("MakeLUT", ERR_ERROR,"LUT size exceeds XP_MAX_DMAP_SIZE \n");
    return(0);  //return 0 for error
  }
  
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // Read number of ranges, control points, range type (linear or
  //	constant), etc from datamap in XP
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  subobjId = OMfind_subobj(in_dmap, OMstr_to_name("currentColorModel"), 
			   OM_OBJ_RD);
  if (!OMis_null_obj(subobjId))
     status=OMget_int_val(subobjId,&colorModel);
  
  subobjId = OMfind_subobj(in_dmap, OMstr_to_name("numRanges"), OM_OBJ_RD);
  if (!OMis_null_obj(subobjId))
     status=OMget_int_val(subobjId,&numRang);
  
  DatamapId = OMfind_subobj(in_dmap, OMstr_to_name("DatamapValue"), 
			    OM_OBJ_RD);
  DataRangeId = OMfind_subobj(in_dmap, OMstr_to_name("DataRange"),
			      OM_OBJ_RD);

  if (!(OMis_null_obj(DatamapId) && OMis_null_obj(DataRangeId)))
  {
    // Loop through ranges 
    for (i = 0; i < numRang; i++)
    {
      double  a_incr = 0.0;
      double  r_incr = 0.0;
      double  g_incr = 0.0; 
      double  b_incr = 0.0;
      double  alpha_min, red_min, green_min, blue_min;
      
      status = OMget_array_val(DataRangeId, i, &arrId, OM_OBJ_RD);
      subArrId = OMfind_subobj(arrId, OMstr_to_name("selectAlphaRange"), 
			       OM_OBJ_RD); 
      if (!OMis_null_obj(subArrId))
	 status=OMget_int_val(subArrId,&alphaType);

      subArrId = OMfind_subobj(arrId, OMstr_to_name("selectColorRange"), 
			       OM_OBJ_RD); 
      if (!OMis_null_obj(subArrId))
	 status=OMget_int_val(subArrId,&colorType);

      subArrId = OMfind_subobj(arrId, OMstr_to_name("minimum"), OM_OBJ_RD); 
      if (!OMis_null_obj(subArrId))
	 status=OMget_real_val(subArrId,&range_min);

      subArrId = OMfind_subobj(arrId, OMstr_to_name("maximum"), OM_OBJ_RD); 
      if (!OMis_null_obj(subArrId))
	 status=OMget_real_val(subArrId,&range_max);

      subArrId = OMfind_subobj(arrId, OMstr_to_name("size"), OM_OBJ_RD); 
      if (!OMis_null_obj(subArrId))
	 status=OMget_int_val(subArrId,&range_size);

      //      printf("Range[%d] %d %d %f %f %d \n", i, alphaType, colorType, 
      //	     range_min, range_max,range_size);
      
      double retVal;
      status = OMget_array_val(DatamapId, i*2, &arrId, OM_OBJ_RD);
      for (j = 0; j < NUM_VALS; j++)
      {
	status = OMget_array_subobj(arrId, j, &subArrId, OM_OBJ_RD);
	if (!OMis_null_obj(subArrId))
	{
	   status=OMget_real_val(subArrId, &retVal);
	   v[j] = retVal;
	}
	//	printf("Datamap[%d] v%d = %f \n",i*2,j,v[j]);
      }
            
      status = OMget_array_val(DatamapId, i*2+1, &arrId, OM_OBJ_RD);
      for (j = 0; j < NUM_VALS; j++)
      {
	status = OMget_array_subobj(arrId, j, &subArrId, OM_OBJ_RD);
	if (!OMis_null_obj(subArrId))
	{
	   status=OMget_real_val(subArrId, &retVal);
           v2[j] = retVal;
	}
	//	printf("Datamap[%d] v%d = %f \n",i*2+1,j,v2[j]);
      }

      if (colorModel == XP_HSV)
      {
        COLORhsv_to_rgb(  v[1],  v[2],  v[3],  &v[1],  &v[2],  &v[3] );
        COLORhsv_to_rgb( v2[1], v2[2], v2[3], &v2[1], &v2[2], &v2[3] );
      }      
            
      alpha_min = ceil(VLI_ALPHA_MAX * v[0]);  
      if (alphaType == XP_LINEAR)
	 a_incr = (VLI_ALPHA_MAX + 1) * (v2[0]-v[0])/range_size;
      
      if (colorType == XP_LINEAR)
      {
	r_incr    = (VLI_COLOR_MAX + 1) * (v2[1]-v[1])/range_size;
	g_incr    = (VLI_COLOR_MAX + 1) * (v2[2]-v[2])/range_size;
	b_incr    = (VLI_COLOR_MAX + 1) * (v2[3]-v[3])/range_size;
	red_min   = ceil(VLI_COLOR_MAX * v[1]);
	green_min = ceil(VLI_COLOR_MAX * v[2]);
	blue_min  = ceil(VLI_COLOR_MAX * v[3]);
      }
      else
      {  //Set constant color value to mean of range's control pts
	red_min   = ceil(VLI_COLOR_MAX * (v[1]+v2[1])/2);
	green_min = ceil(VLI_COLOR_MAX * (v[2]+v2[2])/2);
	blue_min  = ceil(VLI_COLOR_MAX * (v[3]+v2[3])/2);
      }
            
      for (j = 0; j < range_size; j++)
      {
	indx= j + (int)ceil(range_min);
	alpha_array[indx] = ceil(alpha_min + a_incr*j);
	red_array[indx]   = ceil(red_min   + r_incr*j);
	green_array[indx] = ceil(green_min + g_incr*j);
	blue_array[indx]  = ceil(blue_min  + b_incr*j);
      }
            
    }  // for numRang
        
    // Return ARGB arrays to XP
    status=OMset_array(alpha,OM_TYPE_INT,alpha_array,arr_size,
		       OM_SET_ARRAY_COPY);
    status=OMset_array(red,OM_TYPE_BYTE,red_array,arr_size,
		       OM_SET_ARRAY_COPY);
    status=OMset_array(green,OM_TYPE_BYTE,green_array,arr_size,
		       OM_SET_ARRAY_COPY);
    status=OMset_array(blue,OM_TYPE_BYTE,blue_array,arr_size,
		       OM_SET_ARRAY_COPY);
    
    return(1);   // return 1 for success
  }

  else 
  {
    ERRverror("MakeLUT", ERR_ERROR,"Unable to get Express Datamap \n");
    return(0);  //return 0 for error
  }
    
}



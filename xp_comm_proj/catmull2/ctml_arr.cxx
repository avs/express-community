
#include <math.h>
#include "gen.hxx"

#define ERROR_RETURN(MESS) {\
   ERRverror("CatmullSpline_2Core", ERR_ERROR, MESS);\
   if (points_arr!=NULL) ARRfree(points_arr);\
   if (dims_arr!=NULL) ARRfree(dims_arr);\
   if (out_arr!=NULL) ARRfree(out_arr);\
   return 0;\
}

int Catmull_2_Catmull_2Mods_CatmullSpline_2Core::update (OMevent_mask event_mask, 
                                                   int seq_num)
{
	// points (OMXfloat_array read req notify)

	int points_size;
	float *points_arr = NULL;
	
	// dims (OMXint_array read req notify)

	int dims_size;
	int *dims_arr = NULL;
	
	// line_type (OMXenum read req notify)
	// spline_type (OMXenum read req notify)
	// smoothness (OMXfloat read req notify)
	// points_added (OMXint read req notify)
	// out_size (OMXint write)
	// out (OMXfloat_array write)

	int out_arr_size;
	float *out_arr = NULL;
	
	int ret_val;

	//
	//	Create the spline
	//

	CatmullRomSpline crs;
	
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_RD, &points_size);

	if ((points_arr==NULL) || (points_size<=0))
		ERROR_RETURN("Could not get points array");
	
	dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD, &dims_size);

	if ((dims_arr==NULL) || (dims_size!=2))
		ERROR_RETURN("Could not get dimensions array");
	
	//
	//	Compute the points
	//	

	ret_val = crs.ComputePoints (points_arr, dims_arr[0], dims_arr[1],
	                             points_added, line_type, (float)smoothness);

	//
	//	On error report problem
	//
	
	if (ret_val == 0)
		ERROR_RETURN("Could not generate spline points");

	out_size = ret_val;

	//
	//	Copy the output points and exit
	//
	
	out_arr = (float *)out.ret_array_ptr(OM_GET_ARRAY_WR, &out_arr_size);

	if ((out_arr==NULL) || (out_arr_size != (out_size*dims_arr[0])) )
		ERROR_RETURN("Could not get output points array");

	crs.CopyResult(out_arr);

	if (points_arr!=NULL) ARRfree(points_arr);
	if (dims_arr!=NULL) ARRfree(dims_arr);
	if (out_arr!=NULL) ARRfree(out_arr);

	return 1;
}

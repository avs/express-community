
#include "lnrs_gen.h"
#include <math.h>

static void clipper(float *x, float *y, int *dims)
{
	if(*x < 0.) *x = 0.;
	else if(dims && *x > (dims[0]-1)) *x = (float)dims[0]-1.0F;
	if(*y < 0.) *y = 0.;
	else if(dims && *y > (dims[1]-1)) *y = (float)dims[1]-1.0F;
}

int
Draw_DrawMods_LineResamplerCore::Resampler(OMevent_mask event_mask, int seq_num)
{
	// in (OMXfloat_array read req notify)
	int in_size;
	float *in_arr;
	
	// dims (OMXint_array read req notify)
	int *dims_arr = 0;
	
	// type (OMXint read notify)
	// npts (OMXint read notify)
	// npoints (OMXint write)
	// points (OMXfloat_array write)
	int points_size;
	float *points_arr;
	int i, j;
	
	//
	//	Get the input points
	//
	in_arr = (float *)in.ret_array_ptr(OM_GET_ARRAY_RD, &in_size);
   	if(!in_arr || in_size < 4)
	{
		if(in_arr) ARRfree(in_arr);
		npoints	= 0;
		points.set_array(OM_TYPE_FLOAT, NULL, 0, OM_SET_ARRAY_FREE);
		return 1;
	}
	
	//
	//	Compute the number of output points.
	//
	if(npts < in_size/2) npts = in_size/2;
	npoints = npts;
	
	//
	//	Create an array of partial lenghts and compute the total lenght
	//
	int npt = in_size/2;
	float *partial = new float[npt];
	
	float len = partial[0] = 0.0F;
	for(i=0; i < npt-1; i++)
	{
		float dx = in_arr[2*i+0]-in_arr[2*i+2];
		float dy = in_arr[2*i+1]-in_arr[2*i+3];
		len  += (float)sqrt(dx*dx+dy*dy);
		
		partial[i+1] = len;
	}
	
	//
	//	Check if the line is closed.
	//	TOL is the maximum difference between first and last point to consider them coincident
	//
#define TOL 0.001
	
	bool is_closed = ((in_arr[0] - in_arr[in_size-2]) < TOL) && ((in_arr[0] - in_arr[in_size-2]) > -TOL) &&
		((in_arr[1] - in_arr[in_size-1]) < TOL) && ((in_arr[1] - in_arr[in_size-1]) > -TOL);
	float step = is_closed ? len/npts : len/(npts-1);
	
	//
	//	Output the first point
	//
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_WR,&points_size);
	if(!points_arr) return 0;
	points_arr[0] = in_arr[0];
	points_arr[1] = in_arr[1];
	
	//
	//	If requested clip it inside the active area)
	//
	if((int)type)
	{
		dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD);
		clipper(&points_arr[0], &points_arr[1], dims_arr);
	}
	
	//
	//	Now compute equispaced points
	//
	len = step;
	for(i=1; i < npts; i++, len += (float)step)
	{
		//
		//	Find the interval that contains the point
		//
		for(j=1; j < npt; j++) if(len < partial[j]) break;
		if(j == npt) j--;
		j--;
		
		//
		//	Compute the point
		//
		float perc = (len - partial[j]) / (partial[j+1] - partial[j]);
		points_arr[2*i+0] = in_arr[2*j+0] + perc * (in_arr[2*j+2] - in_arr[2*j+0]);
		points_arr[2*i+1] = in_arr[2*j+1] + perc * (in_arr[2*j+3] - in_arr[2*j+1]);
		if((int)type) clipper(&points_arr[2*i+0], &points_arr[2*i+1], dims_arr);
	}
	
	ARRfree(in_arr);
	ARRfree(points_arr);
	delete [] partial;
	if(dims_arr) ARRfree(dims_arr);
	
	// return 1 for success
	return 1;
}


int
Draw_DrawMods_LineResamplerCore::UpdateInput(OMevent_mask event_mask, int seq_num)
{
	// in_npt (OMXint write)
	// in (OMXfloat_array write)
	float *in_arr;
	
	// points (OMXfloat_array read)
	int points_size;
	float *points_arr;
	
	// trigger (OMXint read write req notify)
	if(trigger == 0) return 0;
	trigger = 0;
	
	//
	//	Copies computed points back to the input array
	//
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_RD, &points_size);
	if(!points_arr || points_size < 2) return 0;

	in_npt = points_size/2;
	in_arr = (float *)in.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!in_arr) return 0;
	
	for(int i=0; i < points_size; i++)
	{
		in_arr[i] = points_arr[i];
	}

	ARRfree(in_arr);
	ARRfree(points_arr);
	return 1;
}


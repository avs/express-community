
#include "rgrs_gen.h"
#include <math.h>

//
//	Low level clip and set in the output matrix
//
static void lldraw(int x, int y, int *dims, unsigned char *out)
{
	if(x < 0) x = 0;
	else if(x > (dims[0]-1)) x = dims[0]-1;
	if(y < 0) y = 0;
	else if(y > (dims[1]-1)) y = dims[1]-1;
	
	out[y*dims[0]+x] = 1;
}

//
//	Bresenham line drawing
//
static void Line2D(float *arr, int *dims, unsigned char *out)
{
	// get lo the nearest pixel
	int x0 = (int)floor(arr[0] + 0.5);
	int y0 = (int)floor(arr[1] + 0.5);
	int x1 = (int)floor(arr[2] + 0.5);
	int y1 = (int)floor(arr[3] + 0.5);
	
    // starting point of line
    int x = x0, y = y0;
	
    // direction of line
    int dx = x1-x0, dy = y1-y0;
	
    // increment or decrement depending on direction of line
    int sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
    int sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));
	
    // decision parameters for voxel selection
    if ( dx < 0 ) dx = -dx;
    if ( dy < 0 ) dy = -dy;
    int ax = 2*dx, ay = 2*dy;
    int decx, decy;
	
    // determine largest direction component, single-step related variable
    int max = dx, var = 0;
    if ( dy > max ) { var = 1; }
	
    // traverse Bresenham line
    switch(var)
    {
    case 0:  // single-step in x-direction
        for (decy=ay-dx; /**/; x += sx, decy += ay)
        {
            // process pixel
            lldraw(x, y, dims, out);
			
            // take Bresenham step
            if ( x == x1 ) break;
            if ( decy >= 0 ) { decy -= ax; y += sy; }
        }
        break;
		
    case 1:  // single-step in y-direction
        for (decx=ax-dy; /**/; y += sy, decx += ax)
        {
            // process pixel
            lldraw(x, y, dims, out);
			
            // take Bresenham step
            if ( y == y1 ) break;
            if ( decx >= 0 ) { decx -= ay; x += sx; }
        }
        break;
    }
}


int
Draw_DrawMods_RegionResamplerCore::Resampler(OMevent_mask event_mask, int seq_num)
{
	// in (OMXfloat_array read req notify)
	int in_size;
	float *in_arr;
	
	// dims (OMXint_array read req notify)
	int dims_size;
	int *dims_arr;
	
	// points (OMXbyte_array write)
	int points_size;
	unsigned char *points_arr;
	int i;
	
	//
	//	Initialize the output matrix with all the points unmarked
	//
	dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD, &dims_size);
	if (!dims_arr) return 0;
	
	points_arr = (unsigned char *)points.ret_array_ptr(OM_GET_ARRAY_WR, &points_size);
	if (!points_arr) return 0;
	
	for(i=0; i < points_size; i++) points_arr[i] = 0;
	
	//
	//	Get the input polyline
	//
	in_arr = (float *)in.ret_array_ptr(OM_GET_ARRAY_RD, &in_size);
	if(!in_arr) 
	{
		ARRfree(dims_arr);
		ARRfree(points_arr);
		return 1;
	}
	if(in_size < 4)
	{
		ARRfree(in_arr);
		ARRfree(dims_arr);
		ARRfree(points_arr);
		return 1;
	}
	
	//
	//	For each line segment
	//
	for(i=0; i < in_size/2 - 1; i++)
	{
		Line2D(in_arr+2*i, dims_arr, points_arr);
	}
	
	//
	//	Release resources and return
	//
	ARRfree(in_arr);
	ARRfree(dims_arr);
	ARRfree(points_arr);
	return 1;
}


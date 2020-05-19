/*
**	Interface to Thin Plate Spline interpolation algoritm
**	The source code derive from Dave Eberly an can be found on his
**	home page http://www.cs.unc.edu/~eberly/
**	The real code is under http://www.cs.unc.edu/~eberly/gr_intr.htm
*/

#include <malloc.h>
#include "xp_comm_proj/tpspline/gen.hxx"
#include "tpspline.hxx"



int
ThinPlateSpline_ThinPlateSplineMods_TPS_unif_core::update(OMevent_mask event_mask, int seq_num)
{
	// coord (OMXfloat_array read req notify)
	int coord_size;
	float *coord_arr;
	
	// values (OMXfloat_array read req notify)
	int values_size;
	float *values_arr;
	
	// smooth (OMXfloat read req notify)

	// dims (OMXint_array read req notify)
	int dims_size;
	int *dims_arr;
	
	// out (OMXfloat_array write)
	float *out_arr;
	
	// points (OMXfloat_array write)
	float *points_arr;
	
	int i, j;
	float *x, *y;
	float xmin, xmax, ymin, ymax, xstep, ystep, xc, yc;
	
	/*
	** Get the input coordinate array
	** and bounding box calculation
	*/
	coord_arr = (float *)coord.ret_array_ptr(OM_GET_ARRAY_RD,&coord_size);
	if (!coord_arr)	return 0;

	x = (float *)malloc(coord_size/2*sizeof(float));
	y = (float *)malloc(coord_size/2*sizeof(float));
		
	xmin = xmax = coord_arr[0];
	ymin = ymax = coord_arr[1];
		
	for(i=0; i < coord_size/2; i++)
	{
		x[i] = coord_arr[i*2];
		y[i] = coord_arr[i*2+1];
			
		if(x[i] < xmin) xmin = x[i];
		else if(x[i] > xmax) xmax = x[i];
		if(y[i] < ymin) ymin = y[i];
		else if(y[i] > ymax) ymax = y[i];
	}
		
	ARRfree((char *)coord_arr);
	
	/*
	** Set the min and max coordinates of the resulting output uniform field
	*/
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_WR);
	if (points_arr)
	{
		points_arr[0] = xmin;
		points_arr[1] = ymin;
		points_arr[2] = xmax;
		points_arr[3] = ymax;
		
		ARRfree((char *)points_arr);
	}
	else
	{
		free(x);
		free(y);
 		return 0;
	}
	
	/*
	** Get the point values (ie z value)
	*/
	values_arr = (float *)values.ret_array_ptr(OM_GET_ARRAY_RD,&values_size);
	if(!values_arr)
	{
		free(x);
		free(y);
		return 0;
	}

	/*
	** Get the output grid size
	*/
	dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD,&dims_size);
	if(!dims_arr)
	{
		free(x);
		free(y);
       		ARRfree((char *)values_arr);
		return 0;
	}

	/*
	** Allocate the output array
	*/
	out_arr = (float *)out.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_arr)
	{
		free(x);
		free(y);
       		ARRfree((char *)values_arr);
        	ARRfree((char *)dims_arr);
		return 0;
	}

	/*
	** Compute the thin plate spline
	*/
	mgcThinPlateSpline tps(values_size, x, y, values_arr, smooth);
	
	/*
	** Now compute the spline on all the grid nodes
	*/	
	xstep = (xmax - xmin)/(dims_arr[0]-1);
	ystep = (ymax - ymin)/(dims_arr[1]-1);

	for(j=0,xc=xmin; j < dims_arr[0]; j++,xc+=xstep)
	{
		for(i=0,yc=ymin; i < dims_arr[1]; i++,yc+=ystep)
		{
			out_arr[j+i*dims_arr[0]] = tps(xc, yc);
		}
	}
	
	/*
	** Release temporary arrays and return success
	*/	
	free(x);
	free(y);
	ARRfree((char *)out_arr);
	ARRfree((char *)dims_arr);
	ARRfree((char *)values_arr);

	return 1;
}

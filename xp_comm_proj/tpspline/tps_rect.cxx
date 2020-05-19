/*
**	Interface to Thin Plate Spline interpolation algoritm
**	The source code derive from Dave Eberly an can be found on his
**	home page http://www.cs.unc.edu/~eberly/
**	The real code is under http://www.cs.unc.edu/~eberly/gr_intr.htm
**	This version creates a rectilinear grid that pass through the
**	input points.
*/
#include <stdlib.h>
#include <search.h>
#include <malloc.h>
#include "xp_comm_proj/tpspline/gen.hxx"
#include "tpspline.hxx"

static int compare(const void *arg1, const void *arg2)
{
	if(*(float *)arg1 < *(float *)arg2) return -1;
	else if(*(float *)arg1 > *(float *)arg2) return 1;
	else return 0;
}

int
ThinPlateSpline_ThinPlateSplineMods_TPS_rect_core::update(OMevent_mask event_mask, int seq_num)
{
	// coord (OMXfloat_array read req notify)
	int coord_size;
	float *coord_arr;
	
	// values (OMXfloat_array read req notify)
	int values_size;
	float *values_arr;
	
	// smooth (OMXfloat read req notify)

	// add_points (OMXint read req notify)

	// dims (OMXint_array write)
	int *dims_arr;
	
	// out (OMXfloat_array write)
	float *out_arr;
	
	// points (OMXfloat_array write)
	float *points_arr;
	
	int i, j;
	int nxd, nyd;
	float *x, *y, *xd, *yd;
	float step;
	
	/*
	** Get the input coordinate array
	*/
	coord_arr = (float *)coord.ret_array_ptr(OM_GET_ARRAY_RD,&coord_size);
	if (!coord_arr)	return 0;

	x  = (float *)malloc(coord_size/2*sizeof(float));
	y  = (float *)malloc(coord_size/2*sizeof(float));

	/*
	** Prepare the output coordinate arrays
	*/
	if(add_points < 0) add_points = 0;
	xd = (float *)malloc((coord_size/2+add_points*(coord_size/2-1))*sizeof(float));
	yd = (float *)malloc((coord_size/2+add_points*(coord_size/2-1))*sizeof(float));
			
	for(i=0; i < coord_size/2; i++)
	{
		x[i] = coord_arr[i*2];
		y[i] = coord_arr[i*2+1];
	}
		
	ARRfree((char *)coord_arr);

	/*
	** Find distinct x values and sort them
	*/
	for(i = 0, nxd = 0; i < coord_size/2; i++)
	{
		for(j=0; j < nxd; j++)
		{
			if(x[i] == xd[j]) goto nextx;
		}
		xd[nxd] = x[i];
		nxd++;
		nextx:;
	}
	qsort((void *)xd, nxd, sizeof(float), compare);

	/*
	** Find distinct y values and sort them
	*/
	for(i = 0, nyd = 0; i < coord_size/2; i++)
	{
		for(j=0; j < nyd; j++)
		{
			if(y[i] == yd[j]) goto nexty;
		}
		yd[nyd] = y[i];
		nyd++;
		nexty:;
	}
	qsort((void *)yd, nyd, sizeof(float), compare);
	
	/*
	** Insert the requested number of points between the existing points
	*/
	if(add_points > 0)
	{
		for(i=nxd-1; i > 0; i--)
		{
			xd[i*(add_points+1)] = xd[i];
		}
		for(i=0; i < (nxd-1); i++)
		{
			step = (xd[(i+1)*(add_points+1)] - xd[i*(add_points+1)])/(add_points+1);
			for(j=1; j <= add_points; j++)
			{
				xd[i*(add_points+1)+j] = xd[i*(add_points+1)] + j*step;
			}
		}
		for(i=nyd-1; i > 0; i--)
		{
			yd[i*(add_points+1)] = yd[i];
		}
		for(i=0; i < (nyd-1); i++)
		{
			step = (yd[(i+1)*(add_points+1)] - yd[i*(add_points+1)])/(add_points+1);
			for(j=1; j <= add_points; j++)
			{
				yd[i*(add_points+1)+j] = yd[i*(add_points+1)] + j*step;
			}
		}
		nxd = nxd + add_points*(nxd-1);
		nyd = nyd + add_points*(nyd-1);
	}

	/*
	** Set the output grid size
	*/
	dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!dims_arr)
	{
		free(x);
		free(y);
		free(xd);
		free(yd);
		return 0;
	}
	dims_arr[0] = nxd;
	dims_arr[1] = nyd;
	ARRfree((char *)dims_arr);

	/*
	** Set the points array for the rectilinear output field
	*/
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_WR);
	if (points_arr)
	{
		for(i=0; i < nxd; i++)
		{
			points_arr[i*2] = xd[i];
			points_arr[i*2+1] = yd[0];
		}
		for(i=0; i < nyd; i++)
		{
			points_arr[(i+nxd)*2] = xd[0];
			points_arr[(i+nxd)*2+1] = yd[i];
		}
		
		ARRfree((char *)points_arr);
	}
	else
	{
		free(x);
		free(y);
		free(xd);
		free(yd);
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
		free(xd);
		free(yd);
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
		free(xd);
		free(yd);
       		ARRfree((char *)values_arr);
 		return 0;
	}

	/*
	** Compute the thin plate spline
	*/
	mgcThinPlateSpline tps(values_size, x, y, values_arr, smooth);
	
	/*
	** Now compute the spline on all the grid nodes
	*/	
	for(j=0; j < nxd; j++)
	{
		for(i=0; i < nyd; i++)
		{
			out_arr[j+i*nxd] = tps(xd[j], yd[i]);
		}
	}
	
	/*
	** Release temporary arrays and return success
	*/	
	free(x);
	free(y);
	free(xd);
	free(yd);
	ARRfree((char *)out_arr);
	ARRfree((char *)values_arr);

	return 1;
}


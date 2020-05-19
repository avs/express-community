
#include "scat_to_vol_gen.h"
#include <string.h>

int
STM3_SCAT2VOL_scat_to_vol::upd(OMevent_mask event_mask, int seq_num)
{
	// in (Mesh+Node_Data read req notify)
	// radius (OMXfloat read req notify)
	// resolution (OMXfloat read req notify)
	// out (Field_Unif write)
	int i, j, k;
	float c_min[3], c_max[3];
	
	// get input scatter points coordinates and set a suitable output volume dimensions 
	// the output grid step is "resolution"
	float *in_coordinates = (float *)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!in_coordinates) return 0;

	// for the volume limitis see if there are manual limits
	if((int)use_manual_limits)
	{
  		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
   		if(manual_limits_arr)
		{
			c_min[0] = manual_limits_arr[0];
			c_min[1] = manual_limits_arr[1];
			c_min[2] = manual_limits_arr[2];
			c_max[0] = manual_limits_arr[3];
			c_max[1] = manual_limits_arr[4];
			c_max[2] = manual_limits_arr[5];
      		ARRfree(manual_limits_arr);
		}
		else
		{
			use_manual_limits = 0;
		}
	}
	
	// no manual limits, compute them
	if(!(int)use_manual_limits)
	{
		// compute the output volume dimensions
		float *c_min_arr = (float *)in.coordinates.min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!c_min_arr)
		{
			ARRfree(in_coordinates);
			return 0;
		}
		float *c_max_arr = (float *)in.coordinates.max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!c_max_arr)
		{
			ARRfree(c_min_arr);
			ARRfree(in_coordinates);
			return 0;
		}

		// move the limits to accomodate a full sphere
		c_min[0] = c_min_arr[0] - (float)radius;
		c_min[1] = c_min_arr[1] - (float)radius;
		c_min[2] = c_min_arr[2] - (float)radius;
		c_max[0] = c_max_arr[0] + (float)radius;
		c_max[1] = c_max_arr[1] + (float)radius;
		c_max[2] = c_max_arr[2] + (float)radius;
		ARRfree(c_min_arr);
		ARRfree(c_max_arr);
		
  		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_WR);
   		if(manual_limits_arr)
		{
			manual_limits_arr[0] = c_min[0];
			manual_limits_arr[1] = c_min[1];
			manual_limits_arr[2] = c_min[2];
			manual_limits_arr[3] = c_max[0];
			manual_limits_arr[4] = c_max[1];
			manual_limits_arr[5] = c_max[2];
      		ARRfree(manual_limits_arr);
		}
	}
	
	// prepare the fixed output field parts
	out.nspace = 3;
	out.ndim = 3;
	int *out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_dims)
	{
		ARRfree(in_coordinates);
		return 0;
	}

	out_dims[0] = (int)((c_max[0] - c_min[0])/resolution+0.5);
	out_dims[1] = (int)((c_max[1] - c_min[1])/resolution+0.5);
	out_dims[2] = (int)((c_max[2] - c_min[2])/resolution+0.5);
	
	float *out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(out_points)
	{
		out_points[0] = c_min[0]; 
		out_points[1] = c_min[1]; 
		out_points[2] = c_min[2]; 
		out_points[3] = c_max[0]; 
		out_points[4] = c_max[1]; 
		out_points[5] = c_max[2]; 
		ARRfree(out_points);
	}
	
	// get input values
	float *in_node_data = (float *)in.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);
    if(!in_node_data)
	{
		ARRfree(in_coordinates);
		return 0;
	}

	// input values limits
	float v_min = in.node_data[0].min;
	float v_max = in.node_data[0].max;
	
	// out.nnodes (int)
	out.nnode_data = 1;
	out.node_data[0].veclen = 1;
	unsigned char *out_node_data = (unsigned char *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(!out_node_data)
	{
		ARRfree(out_dims);
		ARRfree(in_coordinates);
		return 0;
	}

	// initialize output
	memset(out_node_data, 0, out.nnodes);
	float sqr_radius = (float)(radius*radius);
	
	// for each input point
	for(int n=0; n < in.nnodes; ++n)
	{
		// find bounding box indexes along x
		int i_min = (int)((in_coordinates[3*n+0] - radius - c_min[0])/resolution+0.5);
		if(i_min < 0) i_min = 0;
		if(i_min >= out_dims[0]) i_min = out_dims[0] - 1;
		int i_max = (int)((in_coordinates[3*n+0] + radius - c_min[0])/resolution+0.5);
		if(i_max < 0) i_max = 0;
		if(i_max >= out_dims[0]) i_max = out_dims[0] - 1;

		// find bounding box indexes along y
		int j_min = (int)((in_coordinates[3*n+1] - radius - c_min[1])/resolution+0.5);
		if(j_min < 0) j_min = 0;
		if(j_min >= out_dims[1]) j_min = out_dims[1] - 1;
		int j_max = (int)((in_coordinates[3*n+1] + radius - c_min[1])/resolution+0.5);
		if(j_max < 0) j_max = 0;
		if(j_max >= out_dims[1]) j_max = out_dims[1] - 1;

		// find bounding box indexes along z
		int k_min = (int)((in_coordinates[3*n+2] - radius - c_min[2])/resolution+0.5);
		if(k_min < 0) k_min = 0;
		if(k_min >= out_dims[2]) k_min = out_dims[2] - 1;
		int k_max = (int)((in_coordinates[3*n+2] + radius - c_min[2])/resolution+0.5);
		if(k_max < 0) k_max = 0;
		if(k_max >= out_dims[2]) k_max = out_dims[2] - 1;
		
		// find pt
		float x0, y0, z0;
		x0 = in_coordinates[3*n+0];
		y0 = in_coordinates[3*n+1];
		z0 = in_coordinates[3*n+2];
		
		unsigned char v = (unsigned char)((in_node_data[n]-v_min)/(v_max-v_min)*255.+0.5);
		
		for(k=k_min; k <= k_max; ++k)
		{
			for(j=j_min; j <= j_max; ++j)
			{
				for(i=i_min; i <= i_max; ++i)
				{
					float x, y, z;
					x = c_min[0] + i*(c_max[0]-c_min[0])/(out_dims[0]-1.F);
					y = c_min[1] + j*(c_max[1]-c_min[1])/(out_dims[1]-1.F);
					z = c_min[2] + k*(c_max[2]-c_min[2])/(out_dims[2]-1.F);
					float d = (x-x0)*(x-x0)+(y-y0)*(y-y0)+(z-z0)*(z-z0);
					if(d <= sqr_radius) out_node_data[k*out_dims[0]*out_dims[1]+j*out_dims[0]+i] = v;
				}
			}
		}
	}

	// release memory and exit
	ARRfree(out_dims);
	ARRfree(out_node_data);
	ARRfree(in_node_data);
	ARRfree(in_coordinates);
	return 1;
}



#include "gen.h"

int
texture_base::ComputeTextureBase(OMevent_mask event_mask, int seq_num)
{
	// dims (OMXint_array read req notify)
	// uv (OMXfloat_array write)

	int dims_size;
	int *dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD, &dims_size);
	if(!dims_arr) return 0;
	if(dims_size != 2)
	{
		ARRfree(dims_arr);
		return 0;
	}

	int d[2];
	d[1] = dims_arr[0]*dims_arr[1];
	d[0] = 2;
	OMset_array_dims(uv.obj_id(OM_OBJ_RW), 2, d);
	
	float *uv_arr = (float *)uv.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!uv_arr)
	{
		ARRfree(dims_arr);
		return 0;
	}

	float maxc = dims_arr[0] - 1.0F;
	float maxr = dims_arr[1] - 1.0F;
	for(int r=0; r < dims_arr[1]; ++r)
	{
		for(int c=0; c < dims_arr[0]; ++c)
		{
			uv_arr[r*dims_arr[0]*2+c*2+0] = (float)c/maxc;
			uv_arr[r*dims_arr[0]*2+c*2+1] = (float)r/maxr;
		}
	}
	
	ARRfree(uv_arr);
	ARRfree(dims_arr);
	return 1;
}

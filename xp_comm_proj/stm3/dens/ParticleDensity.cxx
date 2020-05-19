
#include "ParticleDensity_gen.h"
#include <float.h>
#include "../lib/stm3.h"

#include "ComputeSlot.h"

int
STM3_DENS_ParticleDensity::ComputeMeanDensity(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// use_manual_limits (OMXint read)
	// manual_limits (OMXfloat_array read)
	// accumulator_size (OMXint read notify)
	// smoothing (OMXfloat read notify)
	// out_density (Field_Unif write)
	int i, j, k, a;
	int sides[3];
	float corners[2][3];

	// compute sizes
	float *out_density_points = (float *)out_density.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_density_points) return 0;
	
	if((int)use_manual_limits)
	{
		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!manual_limits_arr)
		{
			ARRfree(out_density_points);
			return 0;
		}
		memcpy(out_density_points, manual_limits_arr, 6*sizeof(float));
		ARRfree(manual_limits_arr);
	}
	else
	{
		float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_coords)
		{
			ARRfree(out_density_points);
			return 0;
		}
		
		out_density_points[0] = out_density_points[1] = out_density_points[2] = FLT_MAX;
		out_density_points[3] = out_density_points[4] = out_density_points[5] = FLT_MIN;
		
		for(i=0; i < in.num_atoms; ++i)
		{
			if(in_coords[3*i+0] < out_density_points[0]) out_density_points[0] = in_coords[3*i+0];
			if(in_coords[3*i+1] < out_density_points[1]) out_density_points[1] = in_coords[3*i+1];
			if(in_coords[3*i+2] < out_density_points[2]) out_density_points[2] = in_coords[3*i+2];

			if(in_coords[3*i+0] > out_density_points[3]) out_density_points[3] = in_coords[3*i+0];
			if(in_coords[3*i+1] > out_density_points[4]) out_density_points[4] = in_coords[3*i+1];
			if(in_coords[3*i+2] > out_density_points[5]) out_density_points[5] = in_coords[3*i+2];
		}
		ARRfree(in_coords);

		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_WR);
		if(manual_limits_arr)
		{
			memcpy(manual_limits_arr, out_density_points, 6*sizeof(float));
			ARRfree(manual_limits_arr);
		}
	}

	corners[0][0] = out_density_points[0];
	corners[0][1] = out_density_points[1];
	corners[0][2] = out_density_points[2];
	corners[1][0] = out_density_points[3];
	corners[1][1] = out_density_points[4];
	corners[1][2] = out_density_points[5];
	ARRfree(out_density_points);

	// compute the number of cell per side
	// cubic slots: the longest volume side is split in side_slots cells.
	// the other sides try to maintain approximately the same cell side lenght
	if((int)use_cubic_slots)
	{
		float lx = corners[1][0]-corners[0][0];
		float ly = corners[1][1]-corners[0][1];
		float lz = corners[1][2]-corners[0][2];

		float longest = lx;
		if(ly > longest) longest = ly;
		if(lz > longest) longest = lz;
		longest /= side_slots;

		sides[0] = (int)(lx/longest+0.5F);
		sides[1] = (int)(ly/longest+0.5F);
		sides[2] = (int)(lz/longest+0.5F);
	}
	else
	{
		sides[0] = sides[1] = sides[2] = side_slots;
	}

	int *out_density_dims = (int *)out_density.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_density_dims) return 0;
	out_density_dims[0] = sides[0]+1;
	out_density_dims[1] = sides[1]+1;
	out_density_dims[2] = sides[2]+1;
	ARRfree(out_density_dims);

	// prepare the output cell data
	FLDset_cell_data_type(out_density.cell_set.obj_id(OM_OBJ_RW), 0, DTYPE_FLOAT);
	float *density_arr = (float *)out_density.cell_set.cell_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!density_arr) return 0;

	float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!in_coords)
	{
		ARRfree(density_arr);
		return 0;
	}

	int *atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!atom_z_arr)
	{
		ARRfree(density_arr);
		ARRfree(in_coords);
		return 0;
	}

	// accumulate particle masses
	int num_slots = sides[0]*sides[1]*sides[2];
	memset(density_arr, 0, sizeof(float)*num_slots);
	ComputeSlot cs(sides, corners);

	if(smoothing > 0.)
	{
		int half_kernel_len = smoothing_kernel/2;
		cs.init_weights((float)smoothing, smoothing_kernel);

		float dx = (corners[1][0] - corners[0][0])/sides[0];
		float dy = (corners[1][1] - corners[0][1])/sides[1];
		float dz = (corners[1][2] - corners[0][2])/sides[2];

		for(a=0; a < in.num_atoms; ++a)
		{
			for(i = -half_kernel_len; i <= half_kernel_len; ++i)
			{
				for(j = -half_kernel_len; j <= half_kernel_len; ++j)
				{
					for(k = -half_kernel_len; k <= half_kernel_len; ++k)
					{
						int slot = cs.slot(in_coords[3*a+0]+dx*i, in_coords[3*a+1]+dy*j, in_coords[3*a+2]+dz*k);
						float weight = cs.weight(i, j, k);
						density_arr[slot] += atom_properties[atom_z_arr[a]].mass * weight;
					}
				}
			}
		}
	}
	else
	{
		for(a=0; a < in.num_atoms; ++a)
		{
			int slot = cs.slot(&in_coords[3*a]);
			density_arr[slot] += atom_properties[atom_z_arr[a]].mass;
		}
	}

	// divide the total mass for the cell volume
	float volume = ((corners[1][0]-corners[0][0]) *
					(corners[1][1]-corners[0][1]) *
					(corners[1][2]-corners[0][2])) / num_slots;
	for(i=0; i < num_slots; ++i) density_arr[i] /= volume;

	// release and exit
	ARRfree(density_arr);
	ARRfree(in_coords);
	ARRfree(atom_z_arr);

	return 1;
}

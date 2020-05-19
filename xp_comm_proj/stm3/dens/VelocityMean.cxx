
#include "VelocityMean_gen.h"
#include <float.h>
#include <string.h>

#include "ComputeSlot.h"

int
STM3_DENS_VelocityMean::ComputeMeanVelocity(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// use_manual_limits (OMXenum read notify)
	// manual_limits (OMXfloat_array read notify)
	// use_cubic_slots (OMXenum read notify)
	// side_slots (OMXint read notify)
	// timestep (OMXfloat read notify)
	// out_velocity (Field_Unif write)
	int i;
	int sides[3];
	float corners[2][3];

	// compute sizes of the output block
	float *out_velocity_points = (float *)out_velocity.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_velocity_points) return 0;
	
	if((int)use_manual_limits)
	{
		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!manual_limits_arr)
		{
			ARRfree(out_velocity_points);
			return 0;
		}
		memcpy(out_velocity_points, manual_limits_arr, 6*sizeof(float));
		ARRfree(manual_limits_arr);
	}
	else
	{
		float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_coords)
		{
			ARRfree(out_velocity_points);
			return 0;
		}
		
		out_velocity_points[0] = out_velocity_points[1] = out_velocity_points[2] = FLT_MAX;
		out_velocity_points[3] = out_velocity_points[4] = out_velocity_points[5] = FLT_MIN;
		
		for(i=0; i < in.num_atoms; ++i)
		{
			if(in_coords[3*i+0] < out_velocity_points[0]) out_velocity_points[0] = in_coords[3*i+0];
			if(in_coords[3*i+1] < out_velocity_points[1]) out_velocity_points[1] = in_coords[3*i+1];
			if(in_coords[3*i+2] < out_velocity_points[2]) out_velocity_points[2] = in_coords[3*i+2];

			if(in_coords[3*i+0] > out_velocity_points[3]) out_velocity_points[3] = in_coords[3*i+0];
			if(in_coords[3*i+1] > out_velocity_points[4]) out_velocity_points[4] = in_coords[3*i+1];
			if(in_coords[3*i+2] > out_velocity_points[5]) out_velocity_points[5] = in_coords[3*i+2];
		}
		ARRfree(in_coords);

		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_WR);
		if(manual_limits_arr)
		{
			memcpy(manual_limits_arr, out_velocity_points, 6*sizeof(float));
			ARRfree(manual_limits_arr);
		}
	}

	corners[0][0] = out_velocity_points[0];
	corners[0][1] = out_velocity_points[1];
	corners[0][2] = out_velocity_points[2];
	corners[1][0] = out_velocity_points[3];
	corners[1][1] = out_velocity_points[4];
	corners[1][2] = out_velocity_points[5];
	ARRfree(out_velocity_points);

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

	int *out_velocity_dims = (int *)out_velocity.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!out_velocity_dims) return 0;
	out_velocity_dims[0] = sides[0]+1;
	out_velocity_dims[1] = sides[1]+1;
	out_velocity_dims[2] = sides[2]+1;
	ARRfree(out_velocity_dims);
	int num_slots = sides[0]*sides[1]*sides[2];

	// check if there is already one set of atoms positions saved
	bool have_past_position = (past_positions.ret_array_size() > 0) && past_positions.ret_array_size() == in.num_atoms*3;
	
	if(!have_past_position)
	{
		// save this position
		past_positions.set_array_size(in.num_atoms*3);
		
		float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_coords) return 0;

		float *past_positions_arr = (float *)past_positions.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!past_positions_arr)
		{
			ARRfree(in_coords);
			return 0;
		}
		
		memcpy(past_positions_arr, in_coords, in.num_atoms*3*sizeof(float));
		ARRfree(in_coords);
		ARRfree(past_positions_arr);
	}

	// prepare the output cell data
	FLDset_cell_data_type(out_velocity.cell_set.obj_id(OM_OBJ_RW), 0, DTYPE_FLOAT);
	float *velocity_arr = (float *)out_velocity.cell_set.cell_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!velocity_arr) return 0;
	
	// reinitialize the velocities
	memset(velocity_arr, 0, num_slots*3*sizeof(float));
	
	if(have_past_position)
	{
		// initialize the converter from coordinates to slot
		ComputeSlot cs(sides, corners);
		
		// count the number of particles for each slot (to compute the mean)
		int *cnt = new int[num_slots];
		memset(cnt, 0, sizeof(int)*num_slots);

		// compute the velocity
		float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_coords) return 0;

		float *past_positions_arr = (float *)past_positions.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!past_positions_arr)
		{
			ARRfree(in_coords);
			return 0;
		}
		
		for(i=0; i < in.num_atoms; ++i)
		{
			// compute the slot
			int slot = cs.slot(&in_coords[3*i]);
			
			// compute particle velocity estimate
			velocity_arr[3*slot+0] += (in_coords[3*i+0] - past_positions_arr[3*i+0])/timestep;
			velocity_arr[3*slot+1] += (in_coords[3*i+1] - past_positions_arr[3*i+1])/timestep;
			velocity_arr[3*slot+2] += (in_coords[3*i+2] - past_positions_arr[3*i+2])/timestep;
			
			// count the number of particles in this cell
			++cnt[slot];
		}
		ARRfree(in_coords);
		ARRfree(past_positions_arr);
		
		// compute the mean
		for(i=0; i < num_slots; ++i)
		{
			if(cnt[i] > 1)
			{
				velocity_arr[3*i+0] /= cnt[i];
				velocity_arr[3*i+1] /= cnt[i];
				velocity_arr[3*i+2] /= cnt[i];
			}
		}
	}
	ARRfree(velocity_arr);

	// update atom positions
	if(have_past_position)
	{
		float *in_coords = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_coords) return 0;

		float *past_positions_arr = (float *)past_positions.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!past_positions_arr)
		{
			ARRfree(in_coords);
			return 0;
		}
		
		memcpy(past_positions_arr, in_coords, in.num_atoms*3*sizeof(float));
		ARRfree(in_coords);
		ARRfree(past_positions_arr);
	}
	
	// return 1 for success
	return 1;
}


int
STM3_DENS_VelocityMean::Clear(OMevent_mask event_mask, int seq_num)
{
	// clear (OMXint read notify)
	// past_positions (OMXfloat_array write)

	past_positions.set_array_size(0);

	float *past_positions_arr = (float *)past_positions.ret_array_ptr(OM_GET_ARRAY_RW);
	if(past_positions_arr) ARRfree(past_positions_arr);
	
	int velocity_size;
	float *velocity_arr = (float *)out_velocity.cell_set.cell_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &velocity_size);
	if(!velocity_arr) return 0;
	memset(velocity_arr, 0, velocity_size*sizeof(float));
	ARRfree(velocity_arr);

	return 1;
}

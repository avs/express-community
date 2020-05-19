
#include "AccumulateTraces_gen.h"
#include "../base/mol_type.h"
#include <stdio.h>
#include <float.h>

static int compute_slot(float x, float y, float z, int side, float *limits)
{
	int i = (int)((x - limits[0])/(limits[3] - limits[0])*(side-1));
	if(i < 0 || i >= side) return -1;
	int j = (int)((y - limits[1])/(limits[4] - limits[1])*(side-1));
	if(j < 0 || j >= side) return -1;
	int k = (int)((z - limits[2])/(limits[5] - limits[2])*(side-1));
	if(k < 0 || k >= side) return -1;
	
	return i + side*j + side*side*k;
}

struct AccumulatorStatus {
	float limits[2][3];
	int num_steps;
	int num_atoms;
	int curr_step;
	int *step_number;
	float **positions;
	int accumulator_size;
	bool playback_initialized;
};

int
STM3_TRACES_AccumulateTraces::Accumulate(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// use_manual_limits (OMXint read)
	// manual_limits (OMXfloat_array read)
	// start (OMXint read)
	// end (OMXint read)
	// increment (OMXint read)
	// step (OMXint read)
	// operation (OMXint read notify)
	// out_trace (Mesh write)
    // out_occupancy (Field_Unif write)
   	// local_ptr (OMXptr read)
	int i, j;

	// if not active return immediately
	if((int)operation == OP_INACTIVE) return 0;

	// access the local status and check for correct initialization.
	struct AccumulatorStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;

	// playback
	if((int)operation == OP_PLAYBACK)
	{
		int last_step;
		
		// update output only if the step number matches
		for(i=0; i <= st->curr_step; ++i)
		{
			if(st->step_number[i] >= step) break;
		}
		if(i > st->curr_step) return 0;
		if(st->step_number[i] > step)
		{
			if(i > 0) --i;
		}
		last_step = i;
	
        unsigned char *out_occupancy_node_data = (unsigned char *)out_occupancy.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RW);
		if(out_occupancy_node_data)
		{
			if(!st->playback_initialized)
			{
				st->playback_initialized = true;

				// update the accumulator limits
 				float *out_occupancy_points = (float *)out_occupancy.points.ret_array_ptr(OM_GET_ARRAY_WR);
				if(out_occupancy_points)
				{
					memcpy(out_occupancy_points, (float *)st->limits, 6 * sizeof(float));
      				ARRfree(out_occupancy_points);
				}

				memset(out_occupancy_node_data, 0, st->accumulator_size*st->accumulator_size*st->accumulator_size*sizeof(unsigned char));
			}
		
			// update the occupancy map
			for(i=0; i < st->num_atoms; ++i)
			{
				for(j=0; j <= last_step; ++j)
				{
					int ijk = compute_slot(	st->positions[j][3*i+0],
											st->positions[j][3*i+1],
											st->positions[j][3*i+2],
											st->accumulator_size,
											(float *)st->limits);

					if(ijk < 0) continue;
					if(out_occupancy_node_data[ijk] < 255) ++out_occupancy_node_data[ijk];				
				}
			}
			ARRfree(out_occupancy_node_data);
		}
		
		// update the trace map
		out_trace.nnodes = (last_step + 1) * st->num_atoms;
		out_trace.cell_set[0].ncells = st->num_atoms;
		out_trace.cell_set[0].npolys = st->num_atoms;
   		float *out_trace_coordinates = (float *)out_trace.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RW);
		if(out_trace_coordinates)
		{
			for(i=0; i < st->num_atoms; ++i)
			{
				for(j=0; j <= last_step; ++j)
				{
					out_trace_coordinates[i * (last_step+1) * 3 + j * 3 + 0] = st->positions[j][3*i+0];
					out_trace_coordinates[i * (last_step+1) * 3 + j * 3 + 1] = st->positions[j][3*i+1];
					out_trace_coordinates[i * (last_step+1) * 3 + j * 3 + 2] = st->positions[j][3*i+2];
				}
			}
			
   			int *out_trace_connect_list = (int *)out_trace.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_trace_connect_list)
			{
				for(i=0; i < st->num_atoms; ++i)
				{
					out_trace_connect_list[2*i+0] = i * (last_step + 1);
					out_trace_connect_list[2*i+1] = i * (last_step + 1) + last_step;
				}
           		ARRfree(out_trace_connect_list);
   			}
   			int *out_trace_values = (int *)out_trace.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RW);
			if(out_trace_values)
			{
				for(i=0; i < st->num_atoms; ++i)
				{
					for(j=0; j <= last_step; ++j)
					{
						out_trace_values[i * (last_step+1) + j] = j;
					}
				}
				ARRfree(out_trace_values);
			}
			
			ARRfree(out_trace_coordinates);
		}

		return 1;
	}
	else
	{
		st->playback_initialized = false;
	}

	// do nothing if no data present
	if(((int)(in.num_atoms) <= 0) || ((int)increment == 0) || ((int)start > (int)end)) return 0;

	if(accumulator_size != st->accumulator_size)
	{
		st->accumulator_size = accumulator_size;
    	unsigned char *out_occupancy_node_data = (unsigned char *)out_occupancy.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(out_occupancy_node_data)
		{
			memset(out_occupancy_node_data, 0, st->accumulator_size*st->accumulator_size*st->accumulator_size*sizeof(unsigned char));
			ARRfree(out_occupancy_node_data);
		}
	}
	
	// check if global data has changed
	if((((end - start)/increment+1) != st->num_steps) || in.num_atoms != st->num_atoms)
	{
		// release the current data
		if(st->step_number) free((void *)st->step_number);
		if(st->positions)
		{
			for(i=0; i < st->num_steps; ++i) if(st->positions[i]) free((void *) st->positions[i]);
			free((void *)st->positions);
		}

		// initialize the memory
		st->num_steps = (end - start)/increment + 1;
		st->num_atoms = in.num_atoms;
		st->curr_step = -1;
		st->step_number = (int *)malloc(st->num_steps * sizeof(int));
		st->positions = (float **)malloc(st->num_steps * sizeof(float *));
		for(i=0; i < st->num_steps; ++i) st->positions[i] = (float *)malloc(st->num_atoms * sizeof(float) * 3);
		st->accumulator_size = accumulator_size;
    	unsigned char *out_occupancy_node_data = (unsigned char *)out_occupancy.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
		if(out_occupancy_node_data)
		{
			memset(out_occupancy_node_data, 0, st->accumulator_size*st->accumulator_size*st->accumulator_size*sizeof(unsigned char));
			ARRfree(out_occupancy_node_data);
		}
		
		if((int)use_manual_limits)
		{
   			float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
   			if(manual_limits_arr)
			{
				memcpy((float *)st->limits, manual_limits_arr, 6 * sizeof(float));
      			ARRfree(manual_limits_arr);
   			}
   			float *out_occupancy_points = (float *)out_occupancy.points.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_occupancy_points)
			{
				memcpy(out_occupancy_points, (float *)st->limits, 6 * sizeof(float));
      			ARRfree(out_occupancy_points);
			}
		}
		else
		{
			st->limits[0][0] = st->limits[0][1] = st->limits[0][2] = FLT_MAX;
			st->limits[1][0] = st->limits[1][1] = st->limits[1][2] = FLT_MIN;
		}
	}

	// save the new step only if itr is not a false step (sometimes happens at the end of a loop)
	++(st->curr_step);
	if(st->curr_step >= st->num_steps) return 0;
	st->step_number[st->curr_step] = step;
	
	// copy atom positions
	float *xyz_lst_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_lst_arr) return 0;
	for(i=0; i < st->num_atoms; ++i)
	{
		st->positions[st->curr_step][3*i+0] = xyz_lst_arr[3*i+0];
		st->positions[st->curr_step][3*i+1] = xyz_lst_arr[3*i+1];
		st->positions[st->curr_step][3*i+2] = xyz_lst_arr[3*i+2];
	}
	ARRfree(xyz_lst_arr);
	
	if((int)operation == OP_ACCUMULATE)
	{
		// compute new limits
		if(!(int)use_manual_limits)
		{
			for(i=0; i < st->num_atoms; ++i)
			{
				for(j=0; j <= st->curr_step; ++j)
				{
					if(st->limits[0][0] > st->positions[j][3*i+0]) st->limits[0][0] = st->positions[j][3*i+0];
					if(st->limits[0][1] > st->positions[j][3*i+1]) st->limits[0][1] = st->positions[j][3*i+1];
					if(st->limits[0][2] > st->positions[j][3*i+2]) st->limits[0][2] = st->positions[j][3*i+2];
					
					if(st->limits[1][0] < st->positions[j][3*i+0]) st->limits[1][0] = st->positions[j][3*i+0];
					if(st->limits[1][1] < st->positions[j][3*i+1]) st->limits[1][1] = st->positions[j][3*i+1];
					if(st->limits[1][2] < st->positions[j][3*i+2]) st->limits[1][2] = st->positions[j][3*i+2];
				}
			}
			
  			float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_WR);
   			if(manual_limits_arr)
			{
				memcpy(manual_limits_arr, (float *)st->limits, 6 * sizeof(float));
      			ARRfree(manual_limits_arr);
   			}
 		}
	}
	
	if((int)operation == OP_IMMEDIATE)
	{
		// compute new limits
		if(!(int)use_manual_limits)
		{
			for(i=0; i < st->num_atoms; ++i)
			{
				for(j=0; j <= st->curr_step; ++j)
				{
					if(st->limits[0][0] > st->positions[j][3*i+0]) st->limits[0][0] = st->positions[j][3*i+0];
					if(st->limits[0][1] > st->positions[j][3*i+1]) st->limits[0][1] = st->positions[j][3*i+1];
					if(st->limits[0][2] > st->positions[j][3*i+2]) st->limits[0][2] = st->positions[j][3*i+2];
					
					if(st->limits[1][0] < st->positions[j][3*i+0]) st->limits[1][0] = st->positions[j][3*i+0];
					if(st->limits[1][1] < st->positions[j][3*i+1]) st->limits[1][1] = st->positions[j][3*i+1];
					if(st->limits[1][2] < st->positions[j][3*i+2]) st->limits[1][2] = st->positions[j][3*i+2];
				}
			}
			
  			float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_WR);
   			if(manual_limits_arr)
			{
				memcpy(manual_limits_arr, (float *)st->limits, 6 * sizeof(float));
      			ARRfree(manual_limits_arr);
   			}
  			float *out_occupancy_points = (float *)out_occupancy.points.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_occupancy_points)
			{
				memcpy(out_occupancy_points, (float *)st->limits, 6 * sizeof(float));
      			ARRfree(out_occupancy_points);
			}
		}
		
		// update the occupancy map
        unsigned char *out_occupancy_node_data = (unsigned char *)out_occupancy.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RW);
		if(out_occupancy_node_data)
		{
			if((int)use_manual_limits)
			{
				for(i=0; i < st->num_atoms; ++i)
				{
					int ijk = compute_slot(	st->positions[st->curr_step][3*i+0],
											st->positions[st->curr_step][3*i+1],
											st->positions[st->curr_step][3*i+2],
											st->accumulator_size,
											(float *)st->limits);

					if(ijk < 0) continue;
					if(out_occupancy_node_data[ijk] < 255) ++out_occupancy_node_data[ijk];				
				}
			}
			else
			{
				memset(out_occupancy_node_data, 0, st->accumulator_size*st->accumulator_size*st->accumulator_size*sizeof(unsigned char));
				for(i=0; i < st->num_atoms; ++i)
				{
					for(j=0; j <= st->curr_step; ++j)
					{
						int ijk = compute_slot(	st->positions[j][3*i+0],
												st->positions[j][3*i+1],
												st->positions[j][3*i+2],
												st->accumulator_size,
												(float *)st->limits);

						if(ijk < 0) continue;
						if(out_occupancy_node_data[ijk] < 255) ++out_occupancy_node_data[ijk];				
					}
				}
			}
			ARRfree(out_occupancy_node_data);
		}
		
		// update the trace map
		out_trace.nnodes = (st->curr_step + 1) * st->num_atoms;
		out_trace.cell_set[0].ncells = st->num_atoms;
		out_trace.cell_set[0].npolys = st->num_atoms;
   		float *out_trace_coordinates = (float *)out_trace.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RW);
		if(out_trace_coordinates)
		{
			for(i=0; i < st->num_atoms; ++i)
			{
				for(j=0; j <= st->curr_step; ++j)
				{
					out_trace_coordinates[i * (st->curr_step+1) * 3 + j * 3 + 0] = st->positions[j][3*i+0];
					out_trace_coordinates[i * (st->curr_step+1) * 3 + j * 3 + 1] = st->positions[j][3*i+1];
					out_trace_coordinates[i * (st->curr_step+1) * 3 + j * 3 + 2] = st->positions[j][3*i+2];
				}
			}
			
   			int *out_trace_connect_list = (int *)out_trace.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_trace_connect_list)
			{
				for(i=0; i < st->num_atoms; ++i)
				{
					out_trace_connect_list[2*i+0] = i * (st->curr_step + 1);
					out_trace_connect_list[2*i+1] = i * (st->curr_step + 1) + st->curr_step;
				}
           		ARRfree(out_trace_connect_list);
   			}
   			int *out_trace_values = (int *)out_trace.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RW);
			if(out_trace_values)
			{
				for(i=0; i < st->num_atoms; ++i)
				{
					for(j=0; j <= st->curr_step; ++j)
					{
						out_trace_values[i * (st->curr_step+1) + j] = j;
					}
				}
				ARRfree(out_trace_values);
			}
			ARRfree(out_trace_coordinates);
		}
	}

   return 1;
}


int
STM3_TRACES_AccumulateTraces::OnInstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)

	struct AccumulatorStatus *st;
	st = (struct AccumulatorStatus *)malloc(sizeof(struct AccumulatorStatus));
	if(!st) return 0;
	OMset_ptr_val((OMobj_id)local_ptr, (void *)st, 0);

	st->num_steps = 0;
	st->num_atoms = 0;
	st->curr_step = -1;
	st->step_number = 0;
	st->positions = 0;
	st->accumulator_size = 64;
	st->playback_initialized = false;

  	float *out_occupancy_points = (float *)out_occupancy.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(out_occupancy_points)
	{
		memcpy(out_occupancy_points, (float *)st->limits, 6 * sizeof(float));
      	out_occupancy_points[0] = out_occupancy_points[1] = out_occupancy_points[2] = 0;
      	out_occupancy_points[3] = out_occupancy_points[4] = out_occupancy_points[5] = accumulator_size - 1.F;
		ARRfree(out_occupancy_points);
	}

	return 1;
}


int
STM3_TRACES_AccumulateTraces::OnDeinstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)

	struct AccumulatorStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;

	if(st->step_number) free((void *)st->step_number);
	if(st->positions)
	{
		for(int i=0; i < st->num_steps; ++i) if(st->positions[i]) free((void *) st->positions[i]);
		free((void *)st->positions);
	}
	free((void *)st);

	OMset_ptr_val((OMobj_id)local_ptr, (void *)0, 0);

	return 1;
}


int
STM3_TRACES_AccumulateTraces::Clear(OMevent_mask event_mask, int seq_num)
{
	// clear (OMXint read notify)
	// local_ptr (OMXptr read)

	struct AccumulatorStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;

	st->curr_step = -1;

    unsigned char *out_occupancy_node_data = (unsigned char *)out_occupancy.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(out_occupancy_node_data)
	{
		memset(out_occupancy_node_data, 0, st->accumulator_size*st->accumulator_size*st->accumulator_size*sizeof(unsigned char));
		ARRfree(out_occupancy_node_data);
	}
	
	out_trace.nnodes = 0;
	out_trace.cell_set[0].ncells = 0;
	out_trace.cell_set[0].npolys = 0;

	if((int)use_manual_limits)
	{
   		float *manual_limits_arr = (float *)manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
   		if(manual_limits_arr)
		{
			memcpy((float *)st->limits, manual_limits_arr, 6 * sizeof(float));
      		ARRfree(manual_limits_arr);
   		}
   		float *out_occupancy_points = (float *)out_occupancy.points.ret_array_ptr(OM_GET_ARRAY_WR);
		if(out_occupancy_points)
		{
			memcpy(out_occupancy_points, (float *)st->limits, 6 * sizeof(float));
      		ARRfree(out_occupancy_points);
		}
	}
	else
	{
		st->limits[0][0] = st->limits[0][1] = st->limits[0][2] = FLT_MAX;
		st->limits[1][0] = st->limits[1][1] = st->limits[1][2] = FLT_MIN;
	}

	return 1;
}

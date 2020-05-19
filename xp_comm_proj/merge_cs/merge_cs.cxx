//
//	Module that merges cell sets with same cell type and same number of cell datas
//	The goal is to improve rendering performance reducing the number of cell sets to be traversed.
//
//	Author:		Mario Valle
//	Date:		14-JAN-2002
//	Version:	1.0
//
//
//	Limitations:
//		It works only with unstructured fields
//		It works only with non polygonal cell sets
//		The properties value of a merged cell sets are equal to the values of the first cell set added
//
//
#include <memory.h>
#include <string.h>
#include "gen.h"

// Size of the buffer user for labels and units copy
#define TXT_SIZE	200

int
MergeCellSets_MergeCellSetsCore::MergeCellSetsCore(OMevent_mask event_mask, int seq_num)
{
	int out_set = -1;
	char txt[TXT_SIZE+1];
	char txt2[TXT_SIZE+1];
	int i, j, size, type, type2, id, nprops;
	// Table of data type size from DTYPE_* constants
	int size_table[] = {
		1, 1, sizeof(short), sizeof(int), sizeof(float), sizeof(double), 0, 0, 0, 0, 0
	};

	//	Prepare the list of active cell sets (ie. cell sets to be examined)
	bool *active = new bool[in.ncell_sets];
	for(i = 0; i < in.ncell_sets; i++) active[i] = true;

	//	Prepare the list of cell sets to be merged
	int *merge_list = new int[in.ncell_sets];
	int  merge_list_size;

	//	Start merging without output cell sets defined
	out.ncell_sets = 0;

	//
	//	Now for each cell set
	//
	for(int in_set=0; in_set < in.ncell_sets; in_set++)
	{
		// Skip inactive cell sets
		if(!active[in_set]) continue;
		active[in_set] = false;

		// Fill the list of cell sets to be merged to the one considered (added to the list)
		merge_list_size	= 1;
		merge_list[0] = in_set;
		for(int in_set2=in_set+1; in_set2 < in.ncell_sets; in_set2++)
		{
			if(!active[in_set2]) continue;

			// Merge cell sets of the same type and with the same number of cell data
			int type, type2;
			FLDget_cell_type(in.cell_set[in_set].obj_id(OM_OBJ_RW), &type);
			FLDget_cell_type(in.cell_set[in_set2].obj_id(OM_OBJ_RW), &type2);
			if(type == type2 && in.cell_set[in_set].ncell_data == in.cell_set[in_set2].ncell_data)
			{
				active[in_set2] = false;

				merge_list[merge_list_size] = in_set2;
				++merge_list_size;
			}
		}

		// If there are cell sets to be merged
		if(merge_list_size > 1)
		{
			// Create the output cell set
			FLDget_cell_set_name(in.cell_set[in_set].obj_id(OM_OBJ_RW), txt, TXT_SIZE);
			FLDadd_cell_set(out.obj_id(OM_OBJ_RW), txt);
			out_set++;

			// Set cell set name
			sprintf(txt, "merged-%d", out_set);
			FLDset_cell_set_user_name(out.cell_set[out_set].obj_id(OM_OBJ_RW), txt);

			//	Compute new cell set size
			int ncells = 0;
			for(i=0; i < merge_list_size; i++)
			{
				ncells += in.cell_set[merge_list[i]].ncells;
			}

			// Set number of cells
			out.cell_set[out_set].ncells = ncells;

			// Set properties
			if(FLDget_cell_nprops(in.cell_set[in_set].obj_id(OM_OBJ_RW), &nprops) == 1)
			{
				FLDset_cell_nprops(out.cell_set[out_set].obj_id(OM_OBJ_RW), nprops);
				if(nprops)
				{
					int size;
					float *in_props, *out_props;
					FLDget_cell_props(in.cell_set[in_set].obj_id(OM_OBJ_RW), &in_props, &size, OM_GET_ARRAY_RD);
					if(in_props)
					{
						FLDget_cell_props(out.cell_set[out_set].obj_id(OM_OBJ_RW), &out_props, &size, OM_GET_ARRAY_WR);
						if(out_props)
						{
							memcpy(out_props, in_props, size * sizeof(float));
							ARRfree(out_props);
						}
						ARRfree(in_props);
					}
				}
			}

			// Add connectivity
			int curr_start = 0;
			int *out_node_connect_list = (int *)out.cell_set[out_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
			if(!out_node_connect_list)
			{
				ERRverror("MergeCellSets",ERR_ERROR,"Cannot create connect list for cell set %d", out_set);
				delete [] active;
				delete [] merge_list;
				return 0;
			}

			// Merge partial connectivity lists
			for(i = 0; i < merge_list_size; i++)
			{
				int *in_node_connect_list = (int *)in.cell_set[merge_list[i]].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD, &size);
				if(!in_node_connect_list)
				{
					ERRverror("MergeCellSets",ERR_ERROR,"Cannot access connect list for cell set %d", merge_list[i]);
					ARRfree(out_node_connect_list);
					delete [] active;
					delete [] merge_list;
					return 0;
				}

				memcpy(out_node_connect_list+curr_start, in_node_connect_list, size*sizeof(int));
				curr_start += size;
				ARRfree(in_node_connect_list);
			}
			ARRfree(out_node_connect_list);

			// Initialize number of cell data
			out.cell_set[out_set].ncell_data = in.cell_set[in_set].ncell_data;

			// Merge cell data
			for(i=0; i < in.cell_set[in_set].ncell_data; i++)
			{
				if(FLDget_cell_data_label(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, txt,  TXT_SIZE) != 1) txt[0]  = '\0';
				if(FLDget_cell_data_units(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, txt2, TXT_SIZE) != 1) txt2[0] = '\0';
				FLDset_cell_data_comp(out.cell_set[out_set].obj_id(OM_OBJ_RW), i,
									  in.cell_set[in_set].cell_data[i].veclen, txt, txt2);
				FLDget_cell_data_type(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, &type);
				FLDset_cell_data_type(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, type);
				if(FLDget_cell_data_id(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, &id) == 1)
					FLDset_cell_data_id(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, id);
				int cell_null_flag;
				double cell_null_value; // large enough to hold any prim
				if(FLDget_cell_null_data(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, &cell_null_flag, (char *)&cell_null_value) == 1)
				{
				   FLDset_cell_null_flag(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, cell_null_flag);
				   if(cell_null_flag)
				   {
					  FLDset_cell_null_data(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, (char *)&cell_null_value, type);
				   }
				}

				char *out_values = (char *)out.cell_set[out_set].cell_data[i].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(!out_values)
				{
					ERRverror("MergeCellSets",ERR_ERROR,"Cannot create cell data component %d for cell set %d", i, out_set);
					delete [] active;
					delete [] merge_list;
					return 0;
				}

				// For each cell set to be merged
				int curr_start = 0;
				for(j=0; j < merge_list_size; j++)
				{
					char *in_values = (char *)in.cell_set[merge_list[j]].cell_data[i].values.ret_array_ptr(OM_GET_ARRAY_RD, &size, &type2);
					if(!in_values)
					{
						ERRverror("MergeCellSets",ERR_ERROR,"Cannot access cell data component %d for cell set %d", i, merge_list[j]);
						ARRfree(out_values);
						delete [] active;
						delete [] merge_list;
						return 0;
					}
					if(type2 != type)
					{
						ERRverror("MergeCellSets",ERR_ERROR,"Cell data component %d for cell set %d has different type %d", i, merge_list[j], type2);
						ARRfree(out_values);
						delete [] active;
						delete [] merge_list;
						return 0;
					}

					memcpy(out_values+curr_start, in_values, size*size_table[type]);
					curr_start += size*size_table[type];
					ARRfree(in_values);
				}

				ARRfree(out_values);
			}
		}
		else // This cell set must not be merged, only copied to output
		{
			// Copy the cell set to output
			FLDget_cell_set_name(in.cell_set[in_set].obj_id(OM_OBJ_RW), txt, TXT_SIZE);
			FLDadd_cell_set(out.obj_id(OM_OBJ_RW), txt);
			out_set++;

			// Set cell set name
			if(FLDget_cell_set_user_name(in.cell_set[in_set].obj_id(OM_OBJ_RW), txt, TXT_SIZE) != 1) txt[0] = '\0';
			FLDset_cell_set_user_name(out.cell_set[out_set].obj_id(OM_OBJ_RW), txt);

			// Set number of cells
			out.cell_set[out_set].ncells = in.cell_set[in_set].ncells;

			// Set properties
			if(FLDget_cell_nprops(in.cell_set[in_set].obj_id(OM_OBJ_RW), &nprops) == 1)
			{
				FLDset_cell_nprops(out.cell_set[out_set].obj_id(OM_OBJ_RW), nprops);
				if(nprops)
				{
					int size;
					float *in_props, *out_props;
					FLDget_cell_props(in.cell_set[in_set].obj_id(OM_OBJ_RW), &in_props, &size, OM_GET_ARRAY_RD);
					if(in_props)
					{
						FLDget_cell_props(out.cell_set[out_set].obj_id(OM_OBJ_RW), &out_props, &size, OM_GET_ARRAY_WR);
						if(out_props)
						{
							memcpy(out_props, in_props, size * sizeof(float));
							ARRfree(out_props);
						}
						ARRfree(in_props);
					}
				}
			}

			// Add connectivity
			int size;
			int *in_node_connect_list = (int *)in.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD, &size);
			if(!in_node_connect_list)
			{
				ERRverror("MergeCellSets",ERR_ERROR,"Cannot access connect list for cell set %d", in_set);
				delete [] active;
				delete [] merge_list;
				return 0;
			}

			int *out_node_connect_list = (int *)out.cell_set[out_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
			if(!out_node_connect_list)
			{
				ERRverror("MergeCellSets",ERR_ERROR,"Cannot create connect list for cell set %d", out_set);
				ARRfree(in_node_connect_list);
				delete [] active;
				delete [] merge_list;
				return 0;
			}

			memcpy(out_node_connect_list, in_node_connect_list, size*sizeof(int));
			ARRfree(out_node_connect_list);
			ARRfree(in_node_connect_list);

			// Initialize number of cell data
			out.cell_set[out_set].ncell_data = in.cell_set[in_set].ncell_data;

			// Copy every cell data
			for(i=0; i < in.cell_set[in_set].ncell_data; i++)
			{
				if(FLDget_cell_data_label(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, txt,  TXT_SIZE) != 1) txt[0]  = '\0';
				if(FLDget_cell_data_units(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, txt2, TXT_SIZE) != 1) txt2[0] = '\0';
				FLDset_cell_data_comp(out.cell_set[out_set].obj_id(OM_OBJ_RW), i,
									  in.cell_set[in_set].cell_data[i].veclen, txt, txt2);
				if(FLDget_cell_data_id(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, &id) == 1)
					FLDset_cell_data_id(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, id);

				char *in_values = (char *)in.cell_set[in_set].cell_data[i].values.ret_array_ptr(OM_GET_ARRAY_RD, &size, &type);
				if(!in_values)
				{
					ERRverror("MergeCellSets",ERR_ERROR,"Cannot access cell data component %d for cell set %d", i, in_set);
					delete [] active;
					delete [] merge_list;
					return 0;
				}

				FLDset_cell_data_type(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, type);
				int cell_null_flag;
				double cell_null_value; // large enough to hold any prim
				if(FLDget_cell_null_data(in.cell_set[in_set].obj_id(OM_OBJ_RW), i, &cell_null_flag, (char *)&cell_null_value) == 1)
				{
				   FLDset_cell_null_flag(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, cell_null_flag);
				   if(cell_null_flag)
				   {
					  FLDset_cell_null_data(out.cell_set[out_set].obj_id(OM_OBJ_RW), i, (char *)&cell_null_value, type);
				   }
				}

				char *out_values = (char *)out.cell_set[out_set].cell_data[i].values.ret_array_ptr(OM_GET_ARRAY_WR);
				if(!out_values)
				{
					ERRverror("MergeCellSets",ERR_ERROR,"Cannot access cell data component %d for cell set %d", i, out_set);
					ARRfree(in_values);
					delete [] active;
					delete [] merge_list;
					return 0;
				}

				memcpy(out_values, in_values, size*size_table[type]);
				ARRfree(out_values);
				ARRfree(in_values);
			}
		}
	}
	
	// Release temporary arrays
	delete [] active;
	delete [] merge_list;
	return 1;
}

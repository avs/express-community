
#include "FindTetrahedra_gen.h"
#include "../base/mol_type.h"


int
STM3_CELLS_FindTetrahedraCore::BuildTetra(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// atom_names (OMXstr_array write)
	// operation (OMXenum read notify)
	// selected_atoms (OMXint_array read notify)
	// last_selected_atoms (OMXint_array read write)
	// tetrahedras (Mesh+Node_Data write)
	int i, j, k, c, na;
	char name[256], *curr_name, *next_name;

	int natoms = in.num_atoms;

	// if input chages recompute the list of atoms
	if(in.changed(seq_num))
	{
		// find candidate atoms (4 bonds)
		int *bonds = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		int *count_bonds = new int[natoms];
		for(i=0; i < natoms; ++i) count_bonds[i] = 0;
		if(bonds)
		{
			for(i=0; i < 2*in.num_bonds; ++i) ++count_bonds[bonds[i]];
			ARRfree(bonds);
		}
		
		for(i=na=0; i < natoms; ++i) if(count_bonds[i] == 4) ++na;
		
		atom_names.set_array_size(na);
		selected_atoms_idx.set_array_size(na);
		
		int *selected_atoms_idx_arr = (int *)selected_atoms_idx.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!selected_atoms_idx_arr)
		{
			delete [] count_bonds;
			return 0;
		}
		
		for(i=j=0; i < natoms; ++i)
		{
			if(count_bonds[i] != 4) continue;
			selected_atoms_idx_arr[j] = i;
			curr_name = in.atom_name.ret_str_array_val(i, name);
			char buf[128];
			sprintf(buf, "%-5s %6d", curr_name, i+1);
			atom_names.set_str_array_val(j++, buf);
		}
		delete [] count_bonds;
		ARRfree(selected_atoms_idx_arr);
		return 1;
	}
	
	// access the list of atoms to be selected
	int selected_atoms_size;
	int *selected_atoms_arr = (int *)selected_atoms.ret_array_ptr(OM_GET_ARRAY_RD, &selected_atoms_size);
	if(!selected_atoms_arr || selected_atoms_size == 0)
	{
		tetrahedras.nnodes = 0;
		tetrahedras.cell_set[0].ncells = 0;
		if(selected_atoms_arr) ARRfree(selected_atoms_arr);
		return 1;
	}

	// convert it into the list of real atom numbers
	int *real_selected_atoms_arr = (int *)selected_atoms_idx.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!real_selected_atoms_arr)
	{
		tetrahedras.nnodes = 0;
		tetrahedras.cell_set[0].ncells = 0;
		ARRfree(selected_atoms_arr);
		return 1;
	}
	
	// prepare the list of atoms to be used.
	bool *ok = new bool[natoms];
	for(i = 0; i < natoms; ++i) ok[i] = false;

	// operation:
	//	0	pass all selected
	//	1	pass all with similar names as selected
	//
	if((int)operation == SELECT_EXACT)
	{
		// simplest case: select only the atoms present in the selection list
		for(j=0; j < selected_atoms_size; ++j)
		{
			i = selected_atoms_arr[j];
			i = real_selected_atoms_arr[i];
			ok[i] = true;
		}
	}
	else
	{
		// create a conversion table
		int *conv = new int[in.num_atoms];
		for(i = 0; i < natoms; ++i)
		{
			conv[i] = -1;
		}
		for(i = 0; i < natoms; ++i)
		{
			if(conv[i] >= 0) continue;

			curr_name = in.atom_name.ret_str_array_val(i, name);
			for(j = i; j < natoms; ++j)
			{
				if(conv[j] >= 0) continue;
				next_name = in.atom_name.ret_str_array_val(j, name);
				if(!strcmp(curr_name, next_name)) conv[j] = i;
			}
		}

		int *conv2 = new int[selected_atoms_size];
		for(j=0; j < selected_atoms_size; ++j)
		{
			int k;
			for(k=0; k < natoms; ++k)
			{
				if(real_selected_atoms_arr[selected_atoms_arr[j]] == k) break;
			}
			conv2[j] = k;
		}

		for(i = 0; i < natoms; ++i)
		{
			for(j=0; j < selected_atoms_size; ++j)
			{
				if(conv[i] == conv[conv2[j]]) {ok[i] = true; break;}
			}
		}

		delete [] conv;
		delete [] conv2;
	}
	ARRfree(selected_atoms_arr);
	ARRfree(real_selected_atoms_arr);

	// Count the output nodes
	for(i=na=0; i < natoms; ++i) if(ok[i]) ++na;
	tetrahedras.nnodes = na*4;
	tetrahedras.cell_set[0].ncells = na;
	
	// Set the connection list
	int *bonds = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!bonds)
	{
		delete [] ok;
		return 0;
	}
	int *tetrahedras_node_connect_list = (int *)tetrahedras.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!tetrahedras_node_connect_list)
	{
		delete [] ok;
		ARRfree(bonds);
		return 0;
	}
	for(i=0; i < na*4; ++i) tetrahedras_node_connect_list[i] = i;
	ARRfree(tetrahedras_node_connect_list);
	
	// Set the coordinates
	float *tetrahedras_coordinates = (float *)tetrahedras.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!tetrahedras_coordinates)
	{
		delete [] ok;
		ARRfree(bonds);
		ARRfree(tetrahedras_node_connect_list);
		return 0;
	}
	float *in_coordinates = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!in_coordinates)
	{
		delete [] ok;
		ARRfree(tetrahedras_coordinates);
		ARRfree(tetrahedras_node_connect_list);
		ARRfree(bonds);
		return 0;
	}
	
	// OK, set the values for the coordinates
	for(i=k=0; i < natoms; ++i)
	{
		if(ok[i])
		{
			int tmp_conn[5];
			
			for(c=j=0; j < in.num_bonds; ++j)
			{
				if(bonds[2*j+0] == i)      tmp_conn[c++] = bonds[2*j+1];
				else if(bonds[2*j+1] == i) tmp_conn[c++] = bonds[2*j+0];
				if(c >= 5) break;
			}
			if(c == 4)
			{
				for(j=0; j < 4; ++j,++k)
				{
					tetrahedras_coordinates[3*k+0] = in_coordinates[3*tmp_conn[j]+0];
					tetrahedras_coordinates[3*k+1] = in_coordinates[3*tmp_conn[j]+1];
					tetrahedras_coordinates[3*k+2] = in_coordinates[3*tmp_conn[j]+2];
				}
			}
		}
	}
	ARRfree(tetrahedras_coordinates);
	ARRfree(in_coordinates);
	ARRfree(bonds);
	delete [] ok;

	// Set the color node data
	float *tetrahedras_node_data = (float *)tetrahedras.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!tetrahedras_node_data) return 0;
	for(i=0; i < 4*na*3; ++i) tetrahedras_node_data[i] = 1.0F;
	ARRfree(tetrahedras_node_data);
	
	// finish
	return 1;
}

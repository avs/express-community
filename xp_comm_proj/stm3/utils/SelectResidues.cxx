
#include "SelectResidues_gen.h"
#include "../base/mol_type.h"
#include <stdio.h>
#include "copy_mol.h"

int
STM3_UTILS_SelectResidues::Select(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// residue_names (OMXstr_array write)
	// selected_residues (OMXint_array read notify)
	// out (MolecularDataType write)
	int selected_residues_size;
	int *selected_residues_arr;
	int i, j;
	char last_residue[256], name[256], *curr_name, *next_name;
	int out_dim;

	// if input chages recompute the list of residues
	if(in.changed(seq_num))
	{
		last_residue[0] = '\0';
		out_dim = 0;
		residue_names.set_array_size(0);
		for(i = 0; i < in.num_atoms; ++i)
		{
			curr_name = in.residue_name.ret_str_array_val(i, name);
			if(strcmp(curr_name, last_residue))
			{			
				strcpy(last_residue, curr_name);
				++out_dim;
				residue_names.set_array_size(out_dim);
				residue_names.set_str_array_val(out_dim - 1, curr_name);
			}
		}	
		return 1;
	}

	// get the residues to be included/excluded
	selected_residues_arr = (int *)selected_residues.ret_array_ptr(OM_GET_ARRAY_RD, &selected_residues_size);
	
	// operation:
	//	0	pass all selected
	//	1	remove selected
	//	2	pass all with similar names as selected
	//	3	remove	all with similar names as selected
	//
	// Now select the operation to perform: ie copy everything, copy nothing, copy same of the residues
	enum {COPY_ALL, COPY_NONE, COPY_SOME} op;
	bool rem_op = ((int)operation == REMOVE_SELECTED || (int)operation == REMOVE_LIKE);
	
	if(!selected_residues_arr)
	{
		if(rem_op) op = COPY_ALL;
		else       op = COPY_NONE;
	}
	else
	{
		if(selected_residues_size == 0)
		{
			if(rem_op) op = COPY_ALL;
			else       op = COPY_NONE;
		}
		else if(selected_residues_size == residue_names.ret_array_size())
		{
			if(rem_op) op = COPY_NONE;
			else       op = COPY_ALL;
		}
		else
		{
			op = COPY_SOME;
		}
	}
	
	// fix the double execution problem. The selection list is notified twice if some entries selected
	if(selected_residues.changed(seq_num))
	{
		int *last_selected_residues_arr;
		
		// check if a preceiding configuration has been saved
		if(last_selected_residues.ret_array_size() == 0 || last_selected_residues.ret_array_size() != selected_residues_size)
		{
			last_selected_residues.set_array_size(selected_residues_size);
			last_selected_residues_arr = (int *)last_selected_residues.ret_array_ptr(OM_GET_ARRAY_WR);
			if(last_selected_residues_arr)
			{
				for(i=0; i < selected_residues_size; ++i) last_selected_residues_arr[i] = selected_residues_arr[i];
				ARRfree(last_selected_residues_arr);
			}
			else
			{
				last_selected_residues.set_array_size(0);
			}
		}
		else
		{
			last_selected_residues_arr = (int *)last_selected_residues.ret_array_ptr(OM_GET_ARRAY_RD);
			i = -1;
			if(last_selected_residues_arr)
			{
				for(i=0; i < selected_residues_size; ++i) if(last_selected_residues_arr[i] != selected_residues_arr[i]) break;
				ARRfree(last_selected_residues_arr);
			}
			last_selected_residues.set_array_size(0); // verify if OK
			if(i == selected_residues_size)
			{
				ARRfree(selected_residues_arr);
				return 0;
			}
		}
	}
	
	// now do the copy of everything (COPY_ALL), nothing (COPY_NONE) or only the requested residues.
	if(op == COPY_ALL)
	{
		copy_full_molecule(in, out);
	}
	else if(op == COPY_NONE)
	{
		empty_molecule(out);
	}
	else
	{
		// prepare the list of atoms to be copied and the list of indices (in the selected_residue_array) corresponding to the residues.
		bool *ok = new bool[in.num_atoms];
		int *idx = new int[in.num_atoms];
		last_residue[0] = '\0';
		out_dim = -1;
		for(i = 0; i < in.num_atoms; ++i)
		{
			curr_name = in.residue_name.ret_str_array_val(i, name);
			if(strcmp(curr_name, last_residue))
			{			
				strcpy(last_residue, curr_name);
				++out_dim;
			}
			idx[i] = out_dim;
			ok[i]  = rem_op;
		}

		// operation:
		//	0	pass all selected
		//	1	remove selected
		//	2	pass all with similar names as selected
		//	3	remove	all with similar names as selected
		//
		if((int)operation == PASS_SELECTED || (int)operation == REMOVE_SELECTED)
		{
			// simplest case: select only the residues present in the selection list
			for(i = 0; i < in.num_atoms; ++i)
			{
				for(j=0; j < selected_residues_size; ++j)
				{
					if(idx[i] == selected_residues_arr[j]) {ok[i] = !ok[i]; break;}
				}
			}
		}
		else
		{
			// create a conversion table
			int *conv = new int[in.num_atoms];
			for(i = 0; i < in.num_atoms; ++i)
			{
				conv[i] = -1;
			}
			for(i = 0; i < in.num_atoms; ++i)
			{
				if(conv[i] >= 0) continue;
				
				curr_name = in.residue_name.ret_str_array_val(i, name);
				for(j = i; j < in.num_atoms; ++j)
				{
					if(conv[j] >= 0) continue;
					next_name = in.residue_name.ret_str_array_val(j, name);
					if(!strncmp(curr_name, next_name, 3)) conv[j] = i;
				}
			}
			 
			int *conv2 = new int[selected_residues_size];
			for(j=0; j < selected_residues_size; ++j)
			{
				int k;
				for(k=0; k < in.num_atoms; ++k)
				{
					if(selected_residues_arr[j] == idx[k]) break;
				}
				conv2[j] = k;
			}
			
			for(i = 0; i < in.num_atoms; ++i)
			{
				for(j=0; j < selected_residues_size; ++j)
				{
					if(idx[conv[i]] == idx[conv[conv2[j]]]) {ok[i] = !ok[i]; break;}
				}
			}
			
			delete [] conv;
			delete [] conv2;
		}
		delete [] idx;
		
		copy_partial_molecule(in, out, ok);
		
		delete [] ok;
	}

	if(selected_residues_arr) ARRfree(selected_residues_arr);
	return 1;
}


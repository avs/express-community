
#include "SelectAtoms_gen.h"
#include "../base/mol_type.h"
#include <stdio.h>
#include "copy_mol.h"

int
STM3_UTILS_SelectAtoms::Select(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// atom_names (OMXstr_array write)
	// selected_atoms (OMXint_array read notify)
	// out (MolecularDataType write)
	int selected_atoms_size;
	int *selected_atoms_arr;
	int i, j;
	char name[256], *curr_name, *next_name;

	// if input chages recompute the list of atoms
	if(in.changed(seq_num))
	{
		atom_names.set_array_size(in.atom_name.ret_array_size());
		for(i = 0; i < in.num_atoms; ++i)
		{
			curr_name = in.atom_name.ret_str_array_val(i, name);
			char buf[128];
			sprintf(buf, "%-5s %6d", curr_name, i+1);
			atom_names.set_str_array_val(i, buf);
		}	
		return 1;
	}

	// access the list of atoms to be selected/deselected
	selected_atoms_arr = (int *)selected_atoms.ret_array_ptr(OM_GET_ARRAY_RD, &selected_atoms_size);
	
	// operation:
	//	0	pass all selected
	//	1	remove selected
	//	2	pass all with similar names as selected
	//	3	remove	all with similar names as selected
	//
	// Now select the operation to perform: ie copy everything, copy nothing, copy same of the atoms
	enum {COPY_ALL, COPY_NONE, COPY_SOME} op;
	bool rem_op = ((int)operation == REMOVE_SELECTED || (int)operation == REMOVE_LIKE);
	
	if(!selected_atoms_arr)
	{
		if(rem_op) op = COPY_ALL;
		else       op = COPY_NONE;
	}
	else
	{
		if(selected_atoms_size == 0)
		{
			if(rem_op) op = COPY_ALL;
			else       op = COPY_NONE;
		}
		else if(selected_atoms_size == atom_names.ret_array_size())
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
	if(selected_atoms.changed(seq_num))
	{
		int *last_selected_atoms_arr;
		
		// check if a preceiding configuration has been saved
		if(last_selected_atoms.ret_array_size() == 0 || last_selected_atoms.ret_array_size() != selected_atoms_size)
		{
			last_selected_atoms.set_array_size(selected_atoms_size);
			last_selected_atoms_arr = (int *)last_selected_atoms.ret_array_ptr(OM_GET_ARRAY_WR);
			if(last_selected_atoms_arr)
			{
				for(i=0; i < selected_atoms_size; ++i) last_selected_atoms_arr[i] = selected_atoms_arr[i];
				ARRfree(last_selected_atoms_arr);
			}
			else
			{
				last_selected_atoms.set_array_size(0);
			}
		}
		else
		{
			last_selected_atoms_arr = (int *)last_selected_atoms.ret_array_ptr(OM_GET_ARRAY_RD);
			i = -1;
			if(last_selected_atoms_arr)
			{
				for(i=0; i < selected_atoms_size; ++i) if(last_selected_atoms_arr[i] != selected_atoms_arr[i]) break;
				ARRfree(last_selected_atoms_arr);
			}
			last_selected_atoms.set_array_size(0); // verify if OK
			if(i == selected_atoms_size)
			{
				ARRfree(selected_atoms_arr);
				return 0;
			}
		}
	}
	
	// now do the copy of everything (COPY_ALL), nothing (COPY_NONE) or only the requested atoms.
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
		// prepare the list of atoms to be copied.
		bool *ok = new bool[in.num_atoms];
		for(i = 0; i < in.num_atoms; ++i)
		{
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
			// simplest case: select only the atoms present in the selection list
			for(j=0; j < selected_atoms_size; ++j)
			{
				i = selected_atoms_arr[j];
				ok[i] = !ok[i];
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
				
				curr_name = in.atom_name.ret_str_array_val(i, name);
				for(j = i; j < in.num_atoms; ++j)
				{
					if(conv[j] >= 0) continue;
					next_name = in.atom_name.ret_str_array_val(j, name);
					//if(!strncmp(curr_name, next_name, 2)) conv[j] = i; // I don't know why selected similar atoms only on the first two letters...
					if(!strcmp(curr_name, next_name)) conv[j] = i;
				}
			}
			 
			int *conv2 = new int[selected_atoms_size];
			for(j=0; j < selected_atoms_size; ++j)
			{
				int k;
				for(k=0; k < in.num_atoms; ++k)
				{
					if(selected_atoms_arr[j] == k) break;
				}
				conv2[j] = k;
			}
			
			for(i = 0; i < in.num_atoms; ++i)
			{
				for(j=0; j < selected_atoms_size; ++j)
				{
					if(conv[i] == conv[conv2[j]]) {ok[i] = !ok[i]; break;}
				}
			}
			
			delete [] conv;
			delete [] conv2;
		}
		
		copy_partial_molecule(in, out, ok);

		delete [] ok;
	}

	if(selected_atoms_arr) ARRfree(selected_atoms_arr);
	return 1;
}


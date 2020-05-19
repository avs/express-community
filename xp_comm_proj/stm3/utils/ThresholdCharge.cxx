
#include "ThresholdCharge_gen.h"
#include "../base/mol_type.h"
#include "copy_mol.h"
#include <float.h>


int
STM3_UTILS_ThresholdChargeCore::Select(OMevent_mask event_mask, int seq_num)
{
	// molecule_in (MolecularDataType read req notify)
	// operation (OMXenum read notify)
	// min_val (OMXfloat read notify)
	// max_val (OMXfloat read notify)
	// molecule_out (MolecularDataType write)
	int i;
	
	// Access the charge value
	int natoms = molecule_in.num_atoms;
	if(natoms < 1) return 0;
	
	float *charge_arr = (float *)molecule_in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!charge_arr) return 0;

	// Insure sane values for limits
	if(min_val > max_val) max_val = min_val;
	
	// Select the atoms
	bool *ok = new bool[natoms];
	int sel = 0;
	for(i=0; i < natoms; ++i)
	{
		// select the atoms to be copied to output
		switch((int)operation)
		{
		default:
		case TH_BELOW:
			ok[i] = (charge_arr[i] < min_val);
			break;
		case TH_BELOW_EQ:
			ok[i] = (charge_arr[i] <= min_val);
			break;
		case TH_ABOVE:
			ok[i] = (charge_arr[i] > max_val);
			break;
		case TH_ABOVE_EQ:
			ok[i] = (charge_arr[i] >= max_val);
			break;
		case TH_BETWEEN:
			ok[i] = (charge_arr[i] > min_val) && (charge_arr[i] < max_val);
			break;
		case TH_BETWEEN_EQ:
			ok[i] = (charge_arr[i] >= min_val) && (charge_arr[i] <= max_val);
			break;
		case TH_EXACT:
			ok[i] = (charge_arr[i] == max_val);
			break;
		};
		
		// Count the selected atoms
		if(ok[i]) ++sel;
	}
	
	// Copy the atoms to output
	if(sel == 0)
	{
		empty_molecule(molecule_out);
	}
	else if(sel == natoms)
	{
		copy_full_molecule(molecule_in, molecule_out);
	}
	else
	{
		copy_partial_molecule(molecule_in, molecule_out, ok);
	}

	delete [] ok;
	ARRfree(charge_arr);
	return 1;
}

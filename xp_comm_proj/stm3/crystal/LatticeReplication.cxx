
#include "LatticeReplication_gen.h"
#include "../utils/copy_mol.h"

// tolerance to check for coincident (after replication) atoms
#define DELTA 1e-4F

int
STM3_CRYSTAL_LatticeReplication::Replicate(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// replications_a (OMXint read notify)
	// replications_b (OMXint read notify)
	// replications_c (OMXint read notify)
	// out (MolecularDataType write)
	int i, j, k;
	
	// sanity check and conversion to local vars
	if((int)replications_a < 1) replications_a = 1;
	int ra = replications_a;
	
	if((int)replications_b < 1) replications_b = 1;
	int rb = replications_b;
	
	if((int)replications_c < 1) replications_c = 1;
	int rc = replications_c;
	
	// special case: no replication at all
	if(ra == 1 && rb == 1 && rc == 1)
	{
		copy_full_molecule(in, out, true);
		return 1;
	}
	
	// get the unit cell basis vectors
	float *basis_arr = (float *)in.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!basis_arr) return 0;
	float basis[9];
	memcpy(basis, basis_arr, 9 * sizeof(float));
	ARRfree(basis_arr);

	// if no unit cell data available, then copy the molecule without replication
	if(basis[0] == 0.0F && basis[1] == 0.0F && basis[2] == 0.0F)
	{
		copy_full_molecule(in, out, true);
		return 1;
	}
	
	// allocate new number of atoms
	out.num_atoms = in.num_atoms * ra * rb *rc;
	
	// allocate new number of bonds
	out.num_bonds = in.num_bonds * ra * rb *rc;
	
	// reverse the direction of the basis vectors if requested
	if((int)reverse_a) {basis[0] = -basis[0]; basis[1] = -basis[1]; basis[2] = -basis[2];}
	if((int)reverse_b) {basis[3] = -basis[3]; basis[4] = -basis[4]; basis[5] = -basis[5];}
	if((int)reverse_c) {basis[6] = -basis[6]; basis[7] = -basis[7]; basis[8] = -basis[8];}
	
	// copy the initial cell (without setting again the output sizes)
	copy_full_molecule(in, out, false);

	// do the replication
	int atoms_start_idx = in.num_atoms;
	int bonds_start_idx = in.num_bonds;
	for(k=0; k < rc; ++k)
	{
		for(j=0; j < rb; ++j)
		{
			for(i=0; i < ra; ++i)
			{
				// initial cell, do not copy
				if(i == 0 && j == 0 && k == 0) continue;
				
				float position_offset[3];
				
				position_offset[0] = basis[0]*i + basis[3]*j + basis[6]*k;
				position_offset[1] = basis[1]*i + basis[4]*j + basis[7]*k;
				position_offset[2] = basis[2]*i + basis[5]*j + basis[8]*k;
				
				replicate_full_molecule(in, out, atoms_start_idx, bonds_start_idx, position_offset);
				
				atoms_start_idx += in.num_atoms;
				bonds_start_idx += in.num_bonds;
			}
		}
	}
	
	return 1;
}

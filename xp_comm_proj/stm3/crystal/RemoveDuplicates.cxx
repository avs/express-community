
#include "RemoveDuplicates_gen.h"
#include "../utils/copy_mol.h"
#include <vector>
using namespace std;

// Maximum coordinate difference to consider duplicated two atoms
#define TOLERANCE	1e-3F

// Check if two points are coincident
static inline bool coincident(float *p1, float *p2, float eps)
{
	float delta = p1[0] - p2[0];
	if(delta < eps && delta > -eps)
	{
		delta = p1[1] - p2[1];
		if(delta < eps && delta > -eps)
		{
			delta = p1[2] - p2[2];
			if(delta < eps && delta > -eps) return true;
		}
	}
	return false;
}


int
STM3_CRYSTAL_RemoveDuplicates::RemoveDuplicates(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// out (MolecularDataType write)
	vector<int> dupl_atoms_vect;
	vector<int>::iterator id;
	int i;
	int *in_connect_lst = 0;
	
	// Access the coordinates
	float *xyz_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_arr) return 0;
	int natoms = in.num_atoms;
	int nbonds = in.num_bonds;

	// Access the connection list to adjust the bonds to duplicated atoms
	if(nbonds)
	{
		in_connect_lst = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!in_connect_lst) nbonds = 0;
	}
		
	// Prepare the list of atoms to be copied
	bool *ok = new bool[natoms];
	for(i = 0; i < natoms; ++i) ok[i]  = true;

	// For each atom try to find the coincident ones
	for(int a1=0; a1 < natoms-1; ++a1)
	{
		if(!ok[a1]) continue;
		
		for(int a2=a1+1; a2 < natoms; ++a2)
		{
			if(coincident(&xyz_arr[3*a1], &xyz_arr[3*a2], TOLERANCE)) dupl_atoms_vect.push_back(a2);
		}
		
		if(dupl_atoms_vect.size() > 0)
		{
			// move the bonds from the removed atoms to the first one
			for(id = dupl_atoms_vect.begin(), i=0; id != dupl_atoms_vect.end(); id++, i++)
			{
				for(i=0; i < nbonds*2; ++i) if(in_connect_lst[i] == *id) in_connect_lst[i] = a1;
				ok[*id] = false;
			}
			
			dupl_atoms_vect.clear();
		}
	}
	ARRfree(xyz_arr);
	if(nbonds) ARRfree(in_connect_lst);
	
	// Copy the remaining part of the molecule
	copy_partial_molecule(in, out, ok);

	delete [] ok;
	return 1;
}

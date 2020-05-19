
#include "CropAtoms_gen.h"
#include "../base/mol_type.h"
#include "copy_mol.h"

int
STM3_UTILS_CropAtoms::Crop(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// operation (OMXenum read notify)
	// shape (OMXenum read notify)
	// radius (OMXfloat read notify)
	// center (OMXfloat_array read notify)
	// sides (OMXfloat_array read notify)
	int sides_size;
	float *sides_arr;

	// outside (MolecularDataType write)
	// inside (MolecularDataType write)
	float r;
	int i;
	float fake_center_arr[3] = {0., 0., 0.};
	
	float *xyz_lst_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_lst_arr) return 0;
	
	int center_size = 0;
	bool has_center = true;
	float *center_arr = (float *)center.ret_array_ptr(OM_GET_ARRAY_RD, &center_size);
	if(!center_arr)
	{
		center_arr = fake_center_arr;
		has_center = false;
	}
	if(center_size < 3)
	{
		center_arr = fake_center_arr;
		has_center = false;
	}
	
	// create the inside index array
	bool *included = new bool[in.num_atoms];
	
	switch((int)shape)
	{
	case CROP_BLOCK:
		sides_arr = (float *)sides.ret_array_ptr(OM_GET_ARRAY_RD, &sides_size);
		if(sides_arr)
		{
			for(i=0; i < in.num_atoms; ++i)
			{
				float x = xyz_lst_arr[i*3+0] - center_arr[0];
				float y = xyz_lst_arr[i*3+1] - center_arr[1];
				float z = xyz_lst_arr[i*3+2] - center_arr[2];
				included[i] = (x <= sides_arr[0]/2. && x >= -sides_arr[0]/2.) &&
							  (y <= sides_arr[1]/2. && y >= -sides_arr[1]/2.) &&
							  (z <= sides_arr[2]/2. && z >= -sides_arr[2]/2.);
			}
			ARRfree(sides_arr);
		}
		break;

	case CROP_CYLINDER_X:
		r = (float)(radius*radius);
		for(i=0; i < in.num_atoms; ++i)
		{
			float z = xyz_lst_arr[i*3+2] - center_arr[2];
			float y = xyz_lst_arr[i*3+1] - center_arr[1];
			included[i] = (z*z+y*y) <= r;
		}
		break;

	case CROP_CYLINDER_Y:
		r = (float)(radius*radius);
		for(i=0; i < in.num_atoms; ++i)
		{
			float x = xyz_lst_arr[i*3+0] - center_arr[0];
			float z = xyz_lst_arr[i*3+2] - center_arr[2];
			included[i] = (x*x+z*z) <= r;
		}
		break;

	case CROP_CYLINDER_Z:
		r = (float)(radius*radius);
		for(i=0; i < in.num_atoms; ++i)
		{
			float x = xyz_lst_arr[i*3+0] - center_arr[0];
			float y = xyz_lst_arr[i*3+1] - center_arr[1];
			included[i] = (x*x+y*y) <= r;
		}
		break;

	case CROP_SPHERE:
		r = (float)(radius*radius);
		for(i=0; i < in.num_atoms; ++i)
		{
			float x = xyz_lst_arr[i*3+0] - center_arr[0];
			float y = xyz_lst_arr[i*3+1] - center_arr[1];
			float z = xyz_lst_arr[i*3+2] - center_arr[2];
			included[i] = (x*x+y*y+z*z) <= r;
		}
		break;

	default:
		ERRverror("CropAtoms", ERR_ERROR, "Invalid crop shape");
		ARRfree(xyz_lst_arr);
		if(has_center) ARRfree(center_arr);
		delete [] included;
		return 0;
	};
	if(has_center) ARRfree(center_arr);
	ARRfree(xyz_lst_arr);
	
	// compute the atoms that go in the other side
	bool *not_included = new bool[in.num_atoms];
	for(i=0; i < in.num_atoms; ++i) not_included[i] = !included[i];

	// if requested add atoms bonded to selected atoms
	if((int)cross_bonds)
	{
		int *connect_lst_arr = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(connect_lst_arr)
		{
			bool *add = new bool[in.num_atoms];
			for(i=0; i < in.num_atoms; ++i) add[i] = false;
			if((int)reverse)
			{
				for(i=0; i < in.num_bonds; ++i)
				{
					if(not_included[connect_lst_arr[2*i+0]])      add[connect_lst_arr[2*i+1]] = true;
					else if(not_included[connect_lst_arr[2*i+1]]) add[connect_lst_arr[2*i+0]] = true;
				}
				for(i=0; i < in.num_atoms; ++i) not_included[i] = not_included[i] || add[i];
			}
			else
			{
				for(i=0; i < in.num_bonds; ++i)
				{
					if(included[connect_lst_arr[2*i+0]])      add[connect_lst_arr[2*i+1]] = true;
					else if(included[connect_lst_arr[2*i+1]]) add[connect_lst_arr[2*i+0]] = true;
				}
				for(i=0; i < in.num_atoms; ++i) included[i] = included[i] || add[i];
			}
			delete [] add;
			ARRfree(connect_lst_arr);
		}
	}
	
	int n = 0;
	for(i=0; i < in.num_atoms; ++i)
	{
		if(included[i]) ++n;
	}

	if((int)reverse)
	{
		if(n == in.num_atoms)
		{
			copy_full_molecule(in, outside);
			empty_molecule(inside);
		}
		else if(n == 0)
		{
			copy_full_molecule(in, inside);
			empty_molecule(outside);
		}
		else
		{
			copy_partial_molecule(in, outside, included);
			copy_partial_molecule(in, inside, not_included);
		}
	}
	else
	{
		if(n == in.num_atoms)
		{
			copy_full_molecule(in, inside);
			empty_molecule(outside);
		}
		else if(n == 0)
		{
			copy_full_molecule(in, outside);
			empty_molecule(inside);
		}
		else
		{
			copy_partial_molecule(in, inside, included);
			copy_partial_molecule(in, outside, not_included);
		}
	}

	// release and exit
	delete [] included;
	delete [] not_included;
	return 1;
}

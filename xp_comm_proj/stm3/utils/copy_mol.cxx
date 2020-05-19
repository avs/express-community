#include "copy_mol.h"

void copy_full_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, bool set_sizes_also)
{
	float *fin, *fout;
	int   *iin, *iout;
	int    i;
	char   s[256];

	if(set_sizes_also)
	{	
		out.num_atoms = in.num_atoms;
		out.num_bonds = in.num_bonds;
	}

	fin = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms*3; ++i) fout[i] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i) fout[i] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.offset_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms*3; ++i) fout[i] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	iin = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds*2; ++i) iout[i] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds; ++i) iout[i] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	for(i=0; i < in.num_atoms; ++i) out.atom_name.set_str_array_val(i, in.atom_name.ret_str_array_val(i, s));
	for(i=0; i < in.num_atoms; ++i) out.residue_name.set_str_array_val(i, in.residue_name.ret_str_array_val(i, s));
}

void empty_molecule(STM3_BASE_MolecularDataType& out)
{
	float *fout;
	int   *iout;

	out.num_atoms = 0;
	out.num_bonds = 0;
	fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(fout) ARRfree(fout);
	fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(fout) ARRfree(fout);
	fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(fout) ARRfree(fout);
	iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_RW);
	if(iout) ARRfree(iout);
	iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_RW);
	if(iout) ARRfree(iout);
	iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(iout) ARRfree(iout);
	iout = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RW);
	if(iout) ARRfree(iout);
	OMparse_buffer(out.obj_id(OM_OBJ_RW), "atom_name=>;", 0);
	OMparse_buffer(out.obj_id(OM_OBJ_RW), "residue_name=>;", 0);
}

void copy_partial_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out, bool *included)
{
	float *fin, *fout;
	int   *iin, *iout;
	int    i, j;
	char   s[256];
	int    natoms = in.num_atoms;
		
	// count the resultant atoms
	int na = 0;
	for(i = 0; i < natoms; ++i) if(included[i]) ++na;
	
	// special cases
	if(na == 0)
	{
		empty_molecule(out);
		return;
	}
	else if(na == natoms)
	{
		copy_full_molecule(in, out, true);
		return;
	}
	
	out.num_atoms = na;

	// now start copy
	fin = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=j=0; i < natoms; ++i) if(included[i]) {fout[j++] = fin[3*i+0]; fout[j++] = fin[3*i+1]; fout[j++] = fin[3*i+2];}
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=j=0; i < natoms; ++i) if(included[i]) fout[j++] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.offset_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=j=0; i < natoms; ++i) if(included[i]) {fout[j++] = fin[3*i+0]; fout[j++] = fin[3*i+1]; fout[j++] = fin[3*i+2];}
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	iin = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=j=0; i < natoms; ++i) if(included[i]) iout[j++] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=j=0; i < natoms; ++i) if(included[i]) iout[j++] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	
	int nbonds = in.num_bonds;
	out.num_bonds = 0;
	iin = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		// prepare another table to convert connectivity info
		int *conv_na = new int[natoms];
		for(i = j = 0; i < natoms; ++i)
		{
			if(included[i]) conv_na[i] = j++;
		}
		for(i=j=0; i < nbonds; ++i)
		{
			if(included[iin[2*i+0]] && included[iin[2*i+1]]) ++j;
		}
		out.num_bonds = j;

		iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=j=0; i < nbonds; ++i)
			{
				if(included[iin[2*i+0]] && included[iin[2*i+1]])
				{
					iout[j++] = conv_na[iin[2*i+0]];
					iout[j++] = conv_na[iin[2*i+1]];
				}
			}
			ARRfree(iout);
		}

		int *iin2 = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(iin2)
		{
			int *iout2 = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
			if(iout2)
			{
				for(i=j=0; i < nbonds; ++i)
				{
					if(included[iin[2*i+0]] && included[iin[2*i+1]])
					{
						iout2[j++] = iin2[i];
					}
				}
				ARRfree(iout2);
			}
			ARRfree(iin2);
		}

		ARRfree(iin);
		delete [] conv_na;
	}

	for(i=j=0; i < natoms; ++i) if(included[i]) out.atom_name.set_str_array_val(j++, in.atom_name.ret_str_array_val(i, s));
	for(i=j=0; i < natoms; ++i) if(included[i]) out.residue_name.set_str_array_val(j++, in.residue_name.ret_str_array_val(i, s));
}

void replicate_full_molecule(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
							 int atoms_start_idx, int bonds_start_idx, float position_offset[3])
{
	float *fin, *fout;
	int   *iin, *iout;
	int    i, j;
	char   s[256];

	fin = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i)
			{
				for(j=0; j < 3; ++j) fout[3*(i+atoms_start_idx)+j] = fin[3*i+j] + position_offset[j];
			}
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i) fout[i+atoms_start_idx] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.offset_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms*3; ++i) fout[i+atoms_start_idx*3] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	iin = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds*2; ++i) iout[i+bonds_start_idx*2] = iin[i]+atoms_start_idx;
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds; ++i) iout[i+bonds_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	for(i=0; i < in.num_atoms; ++i) out.atom_name.set_str_array_val(i+atoms_start_idx, in.atom_name.ret_str_array_val(i, s));
	for(i=0; i < in.num_atoms; ++i) out.residue_name.set_str_array_val(i+atoms_start_idx, in.residue_name.ret_str_array_val(i, s));
}
#if 0
//old version
void replicate_symmetry(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
						int atoms_start_idx, int bonds_start_idx, float rotation[3][3], float translation[3])
{
	float *fin, *fout;
	int   *iin, *iout;
	int    i, j;
	char   s[256];

	fin = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i)
			{
				for(j=0; j < 3; ++j) fout[3*(i+atoms_start_idx)+j] = rotation[j][0]*fin[3*i+0] + rotation[j][1]*fin[3*i+1] + rotation[j][2]*fin[3*i+2] + translation[j];
			}
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i) fout[i+atoms_start_idx] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.offset_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms*3; ++i) fout[i+atoms_start_idx*3] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	iin = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds*2; ++i) iout[i+bonds_start_idx*2] = iin[i]+atoms_start_idx;
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds; ++i) iout[i+bonds_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	for(i=0; i < in.num_atoms; ++i) out.atom_name.set_str_array_val(i+atoms_start_idx, in.atom_name.ret_str_array_val(i, s));
	for(i=0; i < in.num_atoms; ++i) out.residue_name.set_str_array_val(i+atoms_start_idx, in.residue_name.ret_str_array_val(i, s));						   
}
#endif
void replicate_symmetry(STM3_BASE_MolecularDataType& in, STM3_BASE_MolecularDataType& out,
						int atoms_start_idx, int bonds_start_idx,
						float R1[3][3], float T1[3], float R2[3][3], float T2[3])
{
	float *fin, *fout;
	int   *iin, *iout;
	int    i, j;
	char   s[256];
	float  Bfr[3];

	fin = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i)
			{
				for(j=0; j < 3; ++j)
				{
					Bfr[j] = R1[j][0]*fin[3*i+0] + R1[j][1]*fin[3*i+1] + R1[j][2]*fin[3*i+2] + T1[j];
					if(Bfr[j] > 1.0F)      Bfr[j] -= 1.F;
					else if(Bfr[j] < -1.F) Bfr[j] += 2.F;
					else if(Bfr[j] < 0.F)  Bfr[j] += 1.F;

					Bfr[j] -= T2[j];
				}
				for(j=0; j < 3; ++j)
				{
					fout[3*(i+atoms_start_idx)+j] = R2[j][0]*Bfr[0]+R2[j][1]*Bfr[1]+R2[j][2]*Bfr[2];
				}
			}
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms; ++i) fout[i+atoms_start_idx] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	fin = (float *)in.offset_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(fin)
	{
		fout = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(fout)
		{
			for(i=0; i < in.num_atoms*3; ++i) fout[i+atoms_start_idx*3] = fin[i];
			ARRfree(fout);
		}
		ARRfree(fin);
	}
	iin = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_atoms; ++i) iout[i+atoms_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds*2; ++i) iout[i+bonds_start_idx*2] = iin[i]+atoms_start_idx;
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	iin = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(iin)
	{
		iout = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(iout)
		{
			for(i=0; i < in.num_bonds; ++i) iout[i+bonds_start_idx] = iin[i];
			ARRfree(iout);
		}
		ARRfree(iin);
	}
	for(i=0; i < in.num_atoms; ++i) out.atom_name.set_str_array_val(i+atoms_start_idx, in.atom_name.ret_str_array_val(i, s));
	for(i=0; i < in.num_atoms; ++i) out.residue_name.set_str_array_val(i+atoms_start_idx, in.residue_name.ret_str_array_val(i, s));						   
}

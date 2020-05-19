
#include <stdio.h>
#include "ReadMol_gen.h"
#include "mol_type.h"
#include "../lib/stm3.h"

#include <vector>

int
STM3_BASE_ReadMol::ReadMol(OMevent_mask event_mask, int seq_num)
{
	// filename (OMXstr read req notify)
	// format (OMXenum read req)
	// units (OMXenum read write req notify)
	// read_hydrogens (OMXenum read req notify)
	// out (MolecularDataType write)
	vector<Atom> atoms;
	vector<Bond> bonds;
	vector<float> offsets;
	int i;
	UnitCell unit_cell;
	char sgrp[20];
	
	// initialize the variables
	atoms.clear();
	bonds.clear();
	offsets.clear();
	out.num_atoms = 0;
	out.num_bonds = 0;
	sgrp[0] = '\0';

	// open the input file
	FILE *fp = fopen(filename, "r");
	if(fp == NULL)
	{
		ERRverror("ReadMol", ERR_ERROR, "Cannot open file %s", (char *)filename);
		return 0;
	}
	
	// redirect the read to the appropriate reader
	switch((int)format)
	{
	case COORDS:
		out.num_atoms = read_coord(fp, (int)read_hydrogens, atoms, offsets);
		break;
		
	case DCAR:
		out.num_atoms = read_dcar(fp, (int)read_hydrogens, atoms);
		break;
		
	case DMOL_XYZ:
		out.num_atoms = read_dmol_xyz(fp, (int)read_hydrogens, atoms);
		break;
		
	case PDB:
		out.num_atoms = read_pdb(fp, (int)read_hydrogens, atoms, bonds, unit_cell, sgrp);
		break;
		
	case MOPAC_ESP:
		units = BOHR;
		out.num_atoms = read_mopac_esp(fp, (int)read_hydrogens, atoms);
		break;
		
	case CHEM_3D:
		out.num_atoms = read_chem_3d(fp, (int)read_hydrogens, atoms);
		break;
		
	case MOL:
		out.num_atoms = read_mol(fp, (int)read_hydrogens, atoms);
		break;
		
	case MOL2:
		out.num_atoms = read_mol2(fp, (int)read_hydrogens, atoms);
		break;
		
	case MM3:
		out.num_atoms = read_mm3(fp, (int)read_hydrogens, atoms);
		break;
		
	case MOPAC_XYZ:
		out.num_atoms = read_mopac_xyz(fp, (int)read_hydrogens, atoms);
		break;
		
	case CUBE:
		out.num_atoms = read_cube(fp, (int)read_hydrogens, atoms, unit_cell);
		break;
		
	case PGAMESS:
		units = BOHR;
		out.num_atoms = read_gamess(fp, (int)read_hydrogens, atoms);
		break;
		
	case GAMESSCUBE:
		out.num_atoms = read_Cgamess(fp, (int)read_hydrogens, atoms);
		break;
		
	case PLAINCOORD:
		out.num_atoms = read_plain_coord(fp, (int)read_hydrogens, atoms);
		break;
		
	case SHELX:
		out.num_atoms = read_shelx(fp, (int)read_hydrogens, atoms, bonds, unit_cell, sgrp);
		break;
		
	default:
		ERRverror("ReadMol", ERR_ERROR, "Invalid file format %d", (int)format);
		fclose(fp);
		return 0;
	};
	fclose(fp);
	
	// set the unit cell info
	float *f;
	f = (float *)out.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_WR);
	if(f)
	{
		memcpy(f, unit_cell.side_vectors, 9*sizeof(float));
		if((int)units == BOHR) for(i=0; i < 9; ++i) f[i] *= BOHR_TO_ANGSTROM;
		ARRfree(f);
	}
	f = (float *)out.unit_cell.angles.ret_array_ptr(OM_GET_ARRAY_WR);
	if(f)
	{
		memcpy(f, unit_cell.angles, 3*sizeof(float));
		ARRfree(f);
	}
	f = (float *)out.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_WR);
	if(f)
	{
		memcpy(f, unit_cell.origin, 3*sizeof(float));
		if((int)units == BOHR) for(i=0; i < 3; ++i) f[i] *= BOHR_TO_ANGSTROM;
		ARRfree(f);
	}
	
	// symmetries
	out.symmetry.space_group = sgrp;
	
	// copy the output to the Express objects
	if(out.num_atoms <= 0)
	{
		out.num_atoms = 0;
		out.num_bonds = 0;
		// touch the output to set them to zero
	}
	else
	{
		int *atom_z_arr = (int *)out.atom_z.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!atom_z_arr)
		{
			out.num_atoms = 0;
			return 0;
		}
		int *residue_id_arr = (int *)out.residue_id.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!residue_id_arr)
		{
			ARRfree(atom_z_arr);
			out.num_atoms = 0;
			return 0;
		}
		float *xyz_lst_arr = (float *)out.xyz_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!xyz_lst_arr)
		{
			ARRfree(atom_z_arr);
			ARRfree(residue_id_arr);
			out.num_atoms = 0;
			return 0;
		}
		float *charge_lst_arr = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(!charge_lst_arr)
		{
			ARRfree(atom_z_arr);
			ARRfree(residue_id_arr);
			ARRfree(xyz_lst_arr);
			out.num_atoms = 0;
			return 0;
		}

		vector<Atom>::iterator ia;

		if((int)units == BOHR)
		{
			for(ia = atoms.begin(), i=0; ia != atoms.end(); ia++, i++)
			{
				atom_z_arr[i]      = ia->atom_z;
				residue_id_arr[i]  = ia->res_id;
				xyz_lst_arr[3*i+0] = ia->x * BOHR_TO_ANGSTROM;
				xyz_lst_arr[3*i+1] = ia->y * BOHR_TO_ANGSTROM;
				xyz_lst_arr[3*i+2] = ia->z * BOHR_TO_ANGSTROM;
				charge_lst_arr[i]  = ia->c;

				out.atom_name.set_str_array_val(i, ia->name);
				out.residue_name.set_str_array_val(i, ia->res_name);
			}
		}
		else
		{
			for(ia = atoms.begin(), i=0; ia != atoms.end(); ia++, i++)
			{
				atom_z_arr[i]      = ia->atom_z;
				residue_id_arr[i]  = ia->res_id;
				xyz_lst_arr[3*i+0] = ia->x;
				xyz_lst_arr[3*i+1] = ia->y;
				xyz_lst_arr[3*i+2] = ia->z;
				charge_lst_arr[i]  = ia->c;

				out.atom_name.set_str_array_val(i, ia->name);
				out.residue_name.set_str_array_val(i, ia->res_name);
			}
		}
		
		ARRfree(atom_z_arr);
		ARRfree(residue_id_arr);
		ARRfree(xyz_lst_arr);
		ARRfree(charge_lst_arr);
		
		// sets the offsets if provided
		float *offset_lst_arr = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(offset_lst_arr)
		{
			if(offsets.size() > 0)
			{
				if((int)units == BOHR)
				{
					for(i=0; i < out.num_atoms*3; ++i)
					{
						offset_lst_arr[i] = offsets[i] * BOHR_TO_ANGSTROM;
					}
				}
				else
				{
					for(i=0; i < out.num_atoms*3; ++i)
					{
						offset_lst_arr[i] = offsets[i];
					}
				}
			}
			else
			{
				for(i=0; i < out.num_atoms*3; ++i) offset_lst_arr[i] = 0.0;
			}
			
			ARRfree(offset_lst_arr);
		}
		
		// sets the connectivity if provided
		if(bonds.size() > 0)
		{
			out.num_bonds = bonds.size();

			bool all_ok = true;
			int *out_connect_lst = (int *)out.connect_lst.ret_array_ptr(OM_GET_ARRAY_WR);
			if(!out_connect_lst) all_ok = false;
			int *out_bond_type = (int *)out.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_WR);
			if(!out_bond_type) all_ok = false;

			if(all_ok)
			{
				vector<Bond>::iterator ib;
				int i;
				for(ib = bonds.begin(), i=0; ib != bonds.end(); ib++, i++)
				{
					out_connect_lst[2*i+0] = ib->from;
					out_connect_lst[2*i+1] = ib->to;
					out_bond_type[i]       = ib->type;
				}
			}
			
			if(out_bond_type) ARRfree(out_bond_type);
			if(out_connect_lst) ARRfree(out_connect_lst);
			if(!all_ok) out.num_bonds = 0; 
		}
		else
		{
			out.num_bonds = 0;
		}
	}
	
	// return 1 for success
	return 1;
}

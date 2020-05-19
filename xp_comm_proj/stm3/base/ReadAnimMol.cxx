
#include <stdio.h>
#include "ReadAnimMol_gen.h"
#include "mol_type.h"
#include "../lib/stm3.h"

#include <vector>

#define DCD_HDR_SIZE    92

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

//
//	Manage a sequence of files
//	The file names contains a sequence number (zero filled or not). The routine works also if passed a filename without a numeric part.
//	Examples:
//		test0000.bmp ... test0054.bmp
//		Slide1.png ... Slide32.png
//
//	Input:
//		file 		the first filename
//
//	Return value:
//		the number of files in the sequence. It is zero if no file can be opened.
//		
static int FileSequenceLen(char *file)
{
	int i, j, k;
	char tmp[256], format[256];
	int first;
	FILE *fp;

	// find the filename part
	char *p = strrchr(file, SEPARATOR);
	if(!p) p = file;

	// find the first digit
	for(i=0; p[i]; ++i)
	{
		if(isdigit(p[i])) break;
	}
	if(!p[i])
	{
		// no digit found
		strcpy(format, file);
		first = 0;
		fp = fopen(file, "r");
		if(!fp) return 0;
		fclose(fp);
		return 1;
	}
	
	// find number of digits
	for(j=i,k=0; isdigit(p[j]); ++j) tmp[k++] = p[j];
	tmp[k] = '\0';

	// find the first index
	first = atoi(tmp);

	// create the format string
	strncpy(format, file, i+(p-file));
	
	if(p[i] != '0')
	{
		sprintf(format+i+(p-file), "%%d%s", p+j);	
	}	
	else
	{
		sprintf(format+i+(p-file), "%%0%dd%s", j-i, p+j);	
	}
	
	for(i = first,k = -1; ; ++i,++k)
	{
		sprintf(tmp, format, i);
		fp = fopen(tmp, "r");
		if(!fp) break;
		fclose(fp);
	}
	if(k < 0) return 0;
	return k+1;
}

//
//	Open a file in a sequence of files
//	The file names contains a sequence number (zero filled or not). The routine works also if passed a filename without a numeric part.
//	Examples:
//		test0000.bmp ... test0054.bmp
//		Slide1.png ... Slide32.png
//
//	Input:
//		file 		the first filename
//		step		the step to be opened (1 means the first file in the sequence)
//
//	Return value:
//		FILE * referring to the requested file
//		
static FILE *OpenFileSequence(char *file, int step)
{
	int i, j, k;
	char tmp[256], format[256];
	int first;

	// find the filename part
	char *p = strrchr(file, SEPARATOR);
	if(!p) p = file;

	// find the first digit
	for(i=0; p[i]; ++i)
	{
		if(isdigit(p[i])) break;
	}
	if(!p[i])
	{
		// no digit found
		return fopen(file, "r");
	}
	
	// find number of digits
	for(j=i,k=0; isdigit(p[j]); ++j) tmp[k++] = p[j];
	tmp[k] = '\0';

	// find the first index
	first = atoi(tmp);

	// create the format string
	strncpy(format, file, i+(p-file));
	
	if(p[i] != '0')
	{
		sprintf(format+i+(p-file), "%%d%s", p+j);	
	}	
	else
	{
		sprintf(format+i+(p-file), "%%0%dd%s", j-i, p+j);	
	}

	sprintf(tmp, format, step+first-1);
	return fopen(tmp, "r");
}


static inline void REVERSE4BYTES(char *a)
{
        char x;
        x = a[0]; a[0] = a[3]; a[3] = x;
        x = a[1]; a[1] = a[2]; a[2] = x;
}

int
STM3_BASE_ReadAnimMol::ReadAnimMol(OMevent_mask event_mask, int seq_num)
{
	// filename (OMXstr read req notify)
	// format (OMXenum read req)
	// units (OMXenum read write req notify)
	// read_hydrogens (OMXenum read req notify)
	// out (MolecularDataType write)
    // file_offsets (OMXint_array read write)
	vector<Atom> atoms;
	vector<Bond> bonds;
	UnitCell unit_cell;
	int i;
	FILE *fp = NULL, *fp_dcd = NULL;
	float *X = 0, *Y = 0, *Z = 0;
	char sgrp[20];
	sgrp[0] = '\0';

	// if format DCD open the dcd file and, if name changed, recompute offsets
	if((int)format == DCD && (char *)filename_dcd != NULL && *(char *)filename_dcd != '\0')
	{
		// open the file
		fp_dcd = fopen(filename_dcd, "rb");
		if(fp_dcd == NULL)
		{
			ERRverror("ReadAnimMol", ERR_ERROR, "Cannot open DCD file %s", (char *)filename_dcd);
			return 0;
		}

		if(filename_dcd.changed(seq_num))
		{
        	char  dum1[4];
        	int  *ntap, *magic, natoms, nsteps;
			char  hdr[DCD_HDR_SIZE];
        	int   full_header_size;
			long here;

			// check magic number
        	fread(hdr, DCD_HDR_SIZE, 1, fp_dcd);
        	magic = (int *)hdr;
        	if(*magic != 84)
        	{
                REVERSE4BYTES(hdr);
                magic = (int *)hdr;
                if(*magic != 84)
				{
					ERRverror("ReadAnimMol", ERR_ERROR, "Invalid DCD file magic number");
					fclose(fp_dcd);
					return 0;
				}
                big_endian = 1;
        	}
        	else big_endian = 0;

        	// read number of comment lines
        	fread(dum1, sizeof(int), 1, fp_dcd);
        	if(big_endian) REVERSE4BYTES(dum1);
        	ntap = (int *)dum1;

        	// full header size
        	full_header_size = DCD_HDR_SIZE + *ntap + 8;

        	// read n of atoms
        	fseek(fp_dcd, full_header_size + 4, SEEK_SET);
        	fread(dum1, sizeof(int), 1, fp_dcd);
        	if(big_endian) REVERSE4BYTES(dum1);
        	ntap = (int *)dum1;
        	natoms = *ntap;

        	// compute the number of coordinate sets (disregard the number in the header that sometimes does not reflect reality)
        	fseek(fp_dcd, 0L, SEEK_END);
        	nsteps = (ftell(fp_dcd) - (full_header_size + 12))/(12*(2 + natoms));

			// fill the offsets list
			num_steps = nsteps;
			file_offsets.set_array_size(nsteps);
			int *file_offsets_arr = (int *)file_offsets.ret_array_ptr(OM_GET_ARRAY_WR);
			if(file_offsets_arr)
			{
				here = full_header_size + 16;
				for(i=0; i < nsteps; ++i)
				{
					file_offsets_arr[i] = here;
					here += 12*(2 + natoms);
				}
				ARRfree(file_offsets_arr);
			}
			num_atoms = natoms;
		}
	}
	
	// open the input file(s)
	if((char *)filename == NULL || *(char *)filename == '\0') {if(fp_dcd) fclose(fp_dcd); return 0;}
	if((int)format != MULTIFILE_PDB)
	{
		fp = fopen(filename, "rb"); // to make ftell work on PC
		if(fp == NULL)
		{
			ERRverror("ReadAnimMol", ERR_ERROR, "Cannot open file %s", (char *)filename);
			if(fp_dcd) fclose(fp_dcd);
			return 0;
		}
	}

	// if filename change recompute the offsets to the various timesteps
	if(filename.changed(seq_num))
	{
		int nsteps = 0;
		vector<long> offs;
		offs.clear();
		int natoms;
        char  line[130];
		long here;
		
		switch((int)format)
		{
		case KINO:
            fgets(line, 128, fp);
            sscanf(line, "%d", &natoms);
            fgets(line, 128, fp);
			
			here = ftell(fp);
			offs.push_back(here);
            nsteps = 1;
			for(;;)
			{
				char *x = NULL;
				for(i=0; i < natoms+2; ++i) if((x=fgets(line, 128, fp)) == NULL) break;
				if(x == NULL) break;
				here = ftell(fp);
				offs.push_back(here);
            	++nsteps;
			}
			num_atoms = natoms;
			break;

		case MULTI_PDB:
			nsteps = 0;
			for(;;)
			{
				char *x = NULL;
				while((x = fgets(line, 128, fp)) != NULL && (strncmp("HEADER", line, 6)))
				{
				}
				if(x == NULL) break;
				here = ftell(fp);
				offs.push_back(here);
            	++nsteps;
				natoms = 0;
				while((x = fgets(line, 128, fp)) != NULL && (strncmp("END", line, 3)))
				{
					if(!strncmp("ATOM", line, 4) || !strncmp("HETATM", line, 6))
					{
						++natoms;
					}
				}
				if(natoms) num_atoms = natoms;
				if(x == NULL) break;
			}
			if(nsteps == 0)
			{
				here = 0;
				offs.push_back(here);
            	nsteps = 1;
			}
			break;

		case DCD:
			// read the fixed geometry
			atoms.clear();
			bonds.clear();
			out.num_atoms = read_pdb(fp, (int)read_hydrogens, atoms, bonds, unit_cell, sgrp);
			break;

		case MULTIFILE_PDB:
			num_steps = FileSequenceLen((char *)filename);
			break;
		
		default:
			ERRverror("ReadAnimMol", ERR_ERROR, "Invalid file format %d", (int)format);
			fclose(fp);
			return 0;
		};
		
		// fill the offsets list
		if((int)format != DCD && (int)format != MULTIFILE_PDB)
		{
			num_steps = nsteps;
			file_offsets.set_array_size(nsteps);
			int *file_offsets_arr = (int *)file_offsets.ret_array_ptr(OM_GET_ARRAY_WR);
			if(file_offsets_arr)
			{
				for(i=0; i < nsteps; ++i) file_offsets_arr[i] = offs[i];
				ARRfree(file_offsets_arr);
			}
			big_endian = 0;
		}
		else if((int)format == MULTIFILE_PDB)
		{
			file_offsets.set_array_size(num_steps);
		}
		
		// just to be sure
		step = 1;
	}

	if((int)format == MULTIFILE_PDB)
	{
		int tot_steps = file_offsets.ret_array_size();
		if(step > tot_steps) step = tot_steps;
		fp = OpenFileSequence((char *)filename, step);
		if(!fp) return 0;
	}
	else
	{
		// read the file offset from where to start
		int tot_steps;
		int *file_offsets_arr = (int *)file_offsets.ret_array_ptr(OM_GET_ARRAY_RD, &tot_steps);
		if(file_offsets_arr)
		{
			if(step > tot_steps) step = tot_steps;
			if((int)format == DCD) fseek(fp_dcd, file_offsets_arr[step-1], SEEK_SET);
			else                   fseek(fp, file_offsets_arr[step-1], SEEK_SET);
			ARRfree(file_offsets_arr);
		}
		else
		{
			if((int)format == DCD) if(fp_dcd) rewind(fp_dcd);
			else                   rewind(fp);
		}
	}
	
	// preallocate the atoms array
	atoms.reserve((int)num_atoms);
	
	// redirect the reading to the appropriate reader
	switch((int)format)
	{
	case KINO:
		atoms.clear();
		out.num_atoms = read_kino(fp, (int)read_hydrogens, (int)num_atoms, atoms);
		break;
		
	case MULTI_PDB:
		atoms.clear();
		bonds.clear();
		out.num_atoms = read_pdb(fp, (int)read_hydrogens, atoms, bonds, unit_cell, sgrp);
		break;
		
	case DCD:
		if(fp_dcd)
		{
			X = new float[(int)num_atoms];
			Y = new float[(int)num_atoms];
			Z = new float[(int)num_atoms];
			read_dcd(fp_dcd, (int)num_atoms, (int)big_endian, X, Y, Z);
			fclose(fp_dcd);
		}
		break;

	case MULTIFILE_PDB:
		out.num_atoms = read_pdb(fp, (int)read_hydrogens, atoms, bonds, unit_cell, sgrp);
		break;

	default:
		ERRverror("ReadAnimMol", ERR_ERROR, "Invalid file format %d", (int)format);
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

		// if we are reading a DCD file, overwrite the coordinates with
		// the ones contained in the DCD file
		if(X)
		{
			if((int)units == BOHR)
			{
				for(i=0; i < out.num_atoms; ++i)
				{
					xyz_lst_arr[3*i+0] = X[i] * BOHR_TO_ANGSTROM;
					xyz_lst_arr[3*i+1] = Y[i] * BOHR_TO_ANGSTROM;
					xyz_lst_arr[3*i+2] = Z[i] * BOHR_TO_ANGSTROM;
				}
			}
			else
			{
				for(i=0; i < out.num_atoms; ++i)
				{
					xyz_lst_arr[3*i+0] = X[i];
					xyz_lst_arr[3*i+1] = Y[i];
					xyz_lst_arr[3*i+2] = Z[i];
				}
			}

			delete [] X;
			delete [] Y;
			delete [] Z;
		}
		
		ARRfree(atom_z_arr);
		ARRfree(residue_id_arr);
		ARRfree(xyz_lst_arr);
		ARRfree(charge_lst_arr);
		
		// sets the offsets if provided
		float *offset_lst_arr = (float *)out.offset_lst.ret_array_ptr(OM_GET_ARRAY_WR);
		if(offset_lst_arr)
		{
			for(i=0; i < out.num_atoms*3; ++i) offset_lst_arr[i] = 0.0;
			
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


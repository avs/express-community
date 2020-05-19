
#include "WritePDB_gen.h"
#include <stdio.h>
#include <avs/mathutil.h>
#include <avs/f_utils.h>
#include <avs/mat.h>
#include "../base/mol_type.h"

enum {
	PDB_SINGLE,
	PDB_MULTI,
	PDB_MULTIFILE
} PdbAnimType;

int
STM3_UTILS_WritePdbCore::Write(OMevent_mask event_mask, int seq_num)
{
	// molecule (MolecularDataType read req notify)
	// filename (OMXstr read req notify)
	FILE *fp;
	int i, j;
	
	// check if filename available
	if(!filename.valid_obj() || (char *)filename == NULL || *(char *)filename == '\0') return 0;

	// if no animation support ignore molecule changes
	if((int)animation_support == PDB_SINGLE && molecule.changed(seq_num)) return 0;

	// animation support
	char *fn;
	char fn_str[256];
	if((int)animation_support == PDB_MULTIFILE && step.valid_obj())
	{
		char tmp[256];
		strcpy(tmp, filename);
		char *s = strrchr(tmp, '.');
		if(s) *s = '\0';
		sprintf(fn_str, "%s%05d.pdb", tmp, (int)step);
		fn = fn_str;
	}
	else
	{
		fn = filename;
	}

	// create the Pdb file
	const char *mode = ((int)animation_support == PDB_MULTI) ? "a" : "w";
	if((fp = (FILE *)FILEfopen(fn, mode)) == (FILE *)NULL)
	{
		ERRverror("WritePDB", ERR_ERROR, "cannot create file %s", fn);
		return 0;
	}
	
	if((int)animation_support == PDB_MULTI) fprintf(fp, "HEADER\n");
	
	// Write unit cell and symmetry info
	/*
	CRYST1   75.390   22.581   28.600  90.00  90.00  90.00 P 21 21 21    4  1BPI 130
	ORIGX1      1.000000  0.000000  0.000000        0.00000                 1BPI 131
	ORIGX2      0.000000  1.000000  0.000000        0.00000                 1BPI 132
	ORIGX3      0.000000  0.000000  1.000000        0.00000                 1BPI 133
	SCALE1      0.013264  0.000000  0.000000        0.00000                 1BPI 134
	SCALE2      0.000000  0.044285  0.000000        0.00000                 1BPI 135
	SCALE3      0.000000  0.000000  0.034965        0.00000                 1BPI 136
	*/
	float *base = (float *)molecule.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(base)
	{
		float lena = AVS_LENGTH((&base[0]));
		float lenb = AVS_LENGTH((&base[3]));
		float lenc = AVS_LENGTH((&base[6]));

		if(lena)
		{
			float *angles = (float *)molecule.unit_cell.angles.ret_array_ptr(OM_GET_ARRAY_RD);
			if(angles)
			{
				float *origin = (float *)molecule.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_RD);
				if(origin)
				{
					fprintf(fp, "CRYST1%9.3f%9.3f%9.3f%7.2f%7.2f%7.2f %-11.11s   1\n",
							lena, lenb, lenc, angles[0], angles[1], angles[2], (char *)molecule.symmetry.space_group);
							
					// add ORIGXn records to put cell origin
					fprintf(fp, "ORIGX1    %10.6f%10.6f%10.6f     %10.5f\n", 1.0F, 0.0F, 0.0F, 0.0F);
					fprintf(fp, "ORIGX2    %10.6f%10.6f%10.6f     %10.5f\n", 0.0F, 1.0F, 0.0F, 0.0F);
					fprintf(fp, "ORIGX3    %10.6f%10.6f%10.6f     %10.5f\n", 0.0F, 0.0F, 1.0F, 0.0F);
					
					float src[3][3], res[3][3];
					src[0][0] = base[0]; src[1][0] = base[1]; src[2][0] = base[2];
					src[0][1] = base[3]; src[1][1] = base[4]; src[2][1] = base[5];
					src[0][2] = base[6]; src[1][2] = base[7]; src[2][2] = base[8];

					MATmat3_inverse(res, src);					
					
					float o[3];
					o[0] = -(res[0][0]*origin[0]+res[0][1]*origin[1]+res[0][2]*origin[2]);
					o[1] = -(res[1][0]*origin[0]+res[1][1]*origin[1]+res[1][2]*origin[2]);
					o[2] = -(res[2][0]*origin[0]+res[2][1]*origin[1]+res[2][2]*origin[2]);
					fprintf(fp, "SCALE1    %10.6f%10.6f%10.6f     %10.5f\n", res[0][0], res[0][1], res[0][2], o[0]);
					fprintf(fp, "SCALE2    %10.6f%10.6f%10.6f     %10.5f\n", res[1][0], res[1][1], res[1][2], o[1]);
					fprintf(fp, "SCALE3    %10.6f%10.6f%10.6f     %10.5f\n", res[2][0], res[2][1], res[2][2], o[2]);

					ARRfree(origin);
				}
				ARRfree(angles);
			}
		}
		ARRfree(base);
	}
	
	// Write atom data
	/*
COLUMNS        DATA TYPE       FIELD         DEFINITION
---------------------------------------------------------------------------------
 1 -  6        Record name     "ATOM  "

 7 - 11        Integer         serial        Atom serial number.

13 - 16        Atom            name          Atom name.

17             Character       altLoc        Alternate location indicator.

18 - 20        Residue name    resName       Residue name.

22             Character       chainID       Chain identifier.

23 - 26        Integer         resSeq        Residue sequence number.

27             AChar           iCode         Code for insertion of residues.

31 - 38        Real(8.3)       x             Orthogonal coordinates for X in
                                             Angstroms.

39 - 46        Real(8.3)       y             Orthogonal coordinates for Y in
                                             Angstroms.

47 - 54        Real(8.3)       z             Orthogonal coordinates for Z in
                                             Angstroms.

55 - 60        Real(6.2)       occupancy     Occupancy.

61 - 66        Real(6.2)       tempFactor    Temperature factor.

73 - 76        LString(4)      segID         Segment identifier, left-justified.

77 - 78        LString(2)      element       Element symbol, right-justified.

79 - 80        LString(2)      charge        Charge on the atom.
*/
	int *residue_id = (int *)molecule.residue_id.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!residue_id)
	{
		if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

		fclose(fp);
		return 1;
	}
	float *xyz_lst = (float *)molecule.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_lst)
	{
		ARRfree(residue_id);

		if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

		fclose(fp);
		return 1;
	}
	float *charge_lst = (float *)molecule.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!charge_lst)
	{
		ARRfree(residue_id);
		ARRfree(xyz_lst);

		if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

		fclose(fp);
		return 1;
	}
	
	// output atoms here
	for(i=0; i < molecule.num_atoms; ++i)
	{
		char atmp[256], *aname;
		char rtmp[256], *rname;
		
		aname = molecule.atom_name.ret_str_array_val(i, atmp);
		rname = molecule.residue_name.ret_str_array_val(i, rtmp);
		
		fprintf(fp, "ATOM  %5d %-4.4s %-3.3s  %4d    %8.3f%8.3f%8.3f%6.2f%6.2f\n",
				i, aname, rname, residue_id[i], xyz_lst[3*i+0], xyz_lst[3*i+1], xyz_lst[3*i+2], 1.0F, charge_lst[i]);
	}
	
	ARRfree(residue_id);
	ARRfree(xyz_lst);
	ARRfree(charge_lst);
	
	// Now add bonds
/*
COLUMNS         DATA TYPE        FIELD           DEFINITION
---------------------------------------------------------------------------------
 1 -  6         Record name      "CONECT"

 7 - 11         Integer          serial          Atom serial number

12 - 16         Integer          serial          Serial number of bonded atom

17 - 21         Integer          serial          Serial number of bonded atom

22 - 26         Integer          serial          Serial number of bonded atom

27 - 31         Integer          serial          Serial number of bonded atom

32 - 36         Integer          serial          Serial number of hydrogen bonded
                                                 atom

37 - 41         Integer          serial          Serial number of hydrogen bonded
                                                 atom

42 - 46         Integer          serial          Serial number of salt bridged
                                                 atom

47 - 51         Integer          serial          Serial number of hydrogen bonded
                                                 atom

52 - 56         Integer          serial          Serial number of hydrogen bonded
                                                 atom

57 - 61         Integer          serial          Serial number of salt bridged
                                                 atom
*/
	if((int)molecule.num_bonds > 0)
	{	
		int *connect_lst = (int *)molecule.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!connect_lst)
		{
			if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

			fclose(fp);
			return 1;
		}
		int *bond_type_lst = (int *)molecule.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!bond_type_lst)
		{
			if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

			ARRfree(connect_lst);
			fclose(fp);
			return 1;
		}
		
		for(i=0; i < (int)molecule.num_atoms; ++i)
		{
			int nb  = 0;
			int nhb = 0;
			int idxb[10];
			int idxhb[10];
			
			// find all the bonds for the given atom
			for(j=0; j < (int)molecule.num_bonds; ++j)
			{
				if(connect_lst[2*j+0] == i)
				{
					if(bond_type_lst[j] == H_BOND) idxhb[nhb++] = connect_lst[2*j+1];
					else idxb[nb++] = connect_lst[2*j+1];
				}
				else if(connect_lst[2*j+1] == i)
				{
					if(bond_type_lst[j] == H_BOND) idxhb[nhb++] = connect_lst[2*j+0];
					else idxb[nb++] = connect_lst[2*j+0];
				}
			}
			
			if(nb >= 1 || nhb >= 1)
			{
 				fprintf(fp, "CONECT%5d", i);
 				if(nb >= 1)  fprintf(fp, "%5d", idxb[0]);  else fprintf(fp, "     ");
				if(nb >= 2)  fprintf(fp, "%5d", idxb[1]);  else fprintf(fp, "     ");
				if(nb >= 3)  fprintf(fp, "%5d", idxb[2]);  else fprintf(fp, "     ");
				if(nb >= 4)  fprintf(fp, "%5d", idxb[3]);  else fprintf(fp, "     ");
				if(nhb >= 1) fprintf(fp, "%5d", idxhb[0]);
				if(nhb >= 2) fprintf(fp, "%5d", idxhb[1]);
				fprintf(fp, "\n");
			}
			if(nb >= 5 || nhb >= 3)
			{
				fprintf(fp, "CONECT%5d", i);
				if(nb >= 5)  fprintf(fp, "%5d", idxb[4]);  else fprintf(fp, "     ");
				if(nb >= 6)  fprintf(fp, "%5d", idxb[5]);  else fprintf(fp, "     ");
				if(nb >= 7)  fprintf(fp, "%5d", idxb[6]);  else fprintf(fp, "     ");
				if(nb >= 8)  fprintf(fp, "%5d", idxb[7]);  else fprintf(fp, "     ");
				if(nhb >= 3) fprintf(fp, "%5d", idxhb[2]);
				if(nhb >= 4) fprintf(fp, "%5d", idxhb[3]);
				fprintf(fp, "\n");
			}
			if(nhb >= 5)
			{
				fprintf(fp, "CONECT%5d                    ", i);
				if(nhb >= 5) fprintf(fp, "%5d", idxhb[4]);
				if(nhb >= 6) fprintf(fp, "%5d", idxhb[5]);
				fprintf(fp, "\n");
			}
		}
		
		ARRfree(connect_lst);
		ARRfree(bond_type_lst);
	}
	
	if((int)animation_support == PDB_MULTI) fprintf(fp, "END\n");

	fclose(fp);
	return 1;
}

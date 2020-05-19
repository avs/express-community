/*
	The original pdb module has been modified to correctly
assure that hydrogens do not display more than one bond.  Additional
atoms have been added to the known atoms and the structure for atom
coloration has been changed to make adding new atoms easier.  The
table of van der Waals radii have been taken from Emsley.  True
CPK models are now displayed.  The VDW volumes for the CPK can be
scaled, as well as the balls in the ball and stick model.  The
types of files that can now be read are:
		PlainCoord,
		coord,
		G94 Cube,
		pdb,
		Pgamess,
		GAMESS_Cube,
		dcar,
		DMol_xyz,
		MOPAC_ESP,
		MOPAC_XYZ,
		mol2
		mol,
		chem-3d,
		mm3
coordinate files.  One can input coordinates in either bohrs or
Angstroms.  Also, an option to hyde or show hydrogens has been
added.
				Lee Bartolotti  (July 17, 1993).
				Ken Flurchick	(July 17, 1993)

Conversion to AVS/EXPRESS by Ken Flurchick 9/96

	Fget routines in libSTM Modified by
	Theresa Windus  (October 16, 1997)

Update in AVS/XP by Ken Flurchick 2/98

Rebuilt for AVS/Express to run on WINNT 12/99

*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "express.h"

int     atomtype[MAX_ATOMS], atomamin[MAX_ATOMS];

int MolCoords(OMobj_id Mol_c_id, OMevent_mask event_mask, int seq_num)
{
/***********************/
/*  Declare variables  */
/***********************/
	double  Scale, HScale, BScale;
	int     Units = 0;
	int     FType, Rep, trigger;
	int     num_atoms;
	int     a_lst_size = 0;
	float   *a_lst = NULL;
	int     connect_lst_size = 0;
	int		 *connect_lst = NULL;
	int     rgb_lst_size = 0;
	float   *rgb_lst = NULL;
	int     r_lst_size = 0;
	float   *r_lst = NULL;
	int     charge_size = 0;
	float   *charge = NULL;
	int     num_bonds;
	char    **atom_name = NULL;
	OMobj_id atom_name_id;
	int     atom_name_size = 0, atom_name_count;
	char    **residue_name = NULL;
	OMobj_id residue_name_id;
	int     residue_name_size = 0, residue_name_count;
	int     Show_Hydrogens;
	int     Show_Atom_Names;
	int     Show_Residue_Names;
	char    *File_Name = NULL;
	char    Hyd;
	int     rep_type, file_type, units, hydr, names_a, names_r;
	float   sc;
	float   bscale = 1.2, hbscale = 1.0;

	char    type[MAX_ATOMS][4], amin[MAX_ATOMS][6], blank = ' ';
	char    ablank[] = "    \0";
	char    rblank[] = "      \0";
	int             bonds[MAX_BONDS][2];
	float   location[MAX_ATOMS][3];
	float   fcharge[MAX_ATOMS];
	float   bond_colors[MAX_ATOMS][3];
	int     atom_connect[MAX_BONDS];
	float   xx, atom_positions[MAX_BONDS];
	float   bb_lst[4*MAX_BONDS][3], bb_rgb_lst[4*MAX_BONDS][3];

	FILE    *in, *out;
	char    *name, *strrchr();
	int     i, j, atom_count, bond_count, vcolors, a, b;
	float   bondlength, offset[3], ll[3], ur[3], *lcols;
	float   ballscale = STICKSANDSPHERES_RADFACTOR;
	int     kk1, kk2, kk;

/***********************/
/*  Get input values   */
/***********************/
	/* Get Scale's value */
	if (OMget_name_real_val(Mol_c_id, OMstr_to_name("Scale"),
		&Scale) != 1.0) Scale = 0.0;

	/* Get HScale's value */
	if (OMget_name_real_val(Mol_c_id, OMstr_to_name("HScale"),
		&HScale) != 1.0) HScale = 0.0;

	/* Get BScale's value */
	if (OMget_name_real_val(Mol_c_id, OMstr_to_name("BScale"),
		&BScale) != 1.0) BScale = 0.0;

	/* Get Units's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("Units"),
		&Units) != 1) Units = 0;

	/* Get File_Type's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("FType"),
		&FType) != 1) FType = 13;

	/* Get trigger's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("trigger"),
		&trigger) != 1) trigger = 0;

	/* Get Representation's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("Rep"),
		&Rep) != 1) Rep = 0;

	/* Get Show_Atom_Names's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("Show_Atom_Names"),
		&Show_Atom_Names) != 1) Show_Atom_Names = 0;

	/* Get Show_Residue_Names's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("Show_Residue_Names"),
		&Show_Residue_Names) != 1) Show_Residue_Names = 0;

	/* Get Show_Hydrogens's value */
	if (OMget_name_int_val(Mol_c_id, OMstr_to_name("Show_Hydrogens"),
		&Show_Hydrogens) != 1) Show_Hydrogens = 0;

	/* Get File_Name's value */
	if (OMget_name_str_val(Mol_c_id, OMstr_to_name("File_Name"),
		&File_Name, 0) != 1) File_Name = NULL;

/***********************/
/* Function's Body     */
/***********************/
/*
	ERRverror("",ERR_NO_HEADER | ERR_INFO,
		"I'm in function: MolCoords generated from method: ",
		" Mol_c.update\n");
*/
/*
	printf(" Initial Data\n");
	printf("File_Name = %s\n", File_Name);
	printf("File_Type = %d\n", FType);
	printf("Units = %d\n", Units);
	printf("Scale Factor = %f\n", Scale);
	printf("Rep = %d\n", Rep);
	printf("Show_Hydrogens Toggle = %d\n", Show_Hydrogens);
	printf("Show_Atom_Names Toggle = %d\n", Show_Atom_Names);
	printf("Show_Residue_Names Toggle = %d\n", Show_Residue_Names);
*/

	if (File_Name == NULL) return(0);
	switch(Rep){
		case 0: {rep_type = BALL_AND_STICK;}
		break;
		case 1: {rep_type = CPK;}
		break;
		case 2: {rep_type = COLORED_STICK;}
		break;
		case 3: {rep_type = COLORED_PROTEIN;}
		break;
		case 4: {rep_type = STICK;}
		break;
	}

	if(Units == 0) units = BOHR;
	else if (Units == 1) units = ANGSTROM;

	if (Show_Hydrogens == 1) hydr = 0;
	if (Show_Hydrogens == 0) hydr = 1;
	names_a = Show_Atom_Names;
	names_r = Show_Residue_Names;

	switch(FType){
		case 0: {file_type = PLAINCOORD;}
		break;
		case 1: {file_type = COORD;}
		break;
		case 2: {
			units = 1;
			file_type = CUBE;
			}
		break;
		case 3: {file_type = PDB;}
		break;
		case 4: {
			units = BOHR;
			file_type = PGAMESS;
			}
		break;
		case 5: {
			units = BOHR;
			file_type = GAMESSCUBE;
			}
		break;
		case 6: {file_type = DCAR;}
		break;
		case 7: {file_type = DMOL_XYZ;}
		break;
		case 8: {
			units = BOHR;
			file_type = ESP;
			}
		break;
		case 9: {file_type = MOPAC;}
		break;
		case 10: {file_type = MOL2;}
		break;
		case 11: {file_type = MOL;}
		break;
		case 12: {file_type = CHEM_3D;}
		break;
		case 13: {file_type = MM3;}
		break;
	}
	if(Scale == 0.0) Scale = 1.0;
	if (rep_type == CPK) {
		ballscale = 1.0;
	}
	ballscale = ballscale * Scale;
	hbscale = (float)HScale;
	bscale = bscale * BScale;

	if(units == BOHR) ballscale = ballscale/0.52914;
	bondlength = BONDLENGTH;
	if(units == BOHR) bondlength = BONDLENGTH/0.52914;
/*
	printf("processed data\n");
	printf("file type = %d\n", file_type);
	printf("ballscale = %f, bondlength = %f, rep_type = %d\n",
		ballscale, bondlength, rep_type);

	printf("Trigger = %d\n", trigger);
*/
	if (trigger == 1){
		if((in = fopen(File_Name,"r")) != NULL){
			printf("opened file File_Name = %s\n",
				File_Name);
		}
		else {
			printf("did not open file\n");
			return(0);
		}
	}
	else if (trigger == 0){
		return(0);
	}

	if(file_type == 1){
		/* printf("Calling fgetdcar\n"); */
		num_atoms = fgetdcar(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 2){
		/* printf("Calling fgetdmol_xyz\n"); */
		num_atoms = fgetdmol_xyz(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 4){
		/* printf("Calling fgetespcoord\n"); */
		num_atoms = fgetespcoord(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 9){
		/* printf("Calling fgetmopac\n"); */
		num_atoms = fgetmopac(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 7){
		/* printf("Calling fgetmol2\n"); */
		num_atoms = fgetmol2(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 6){
		/* printf("Calling fgetmol\n"); */
		num_atoms = fgetmol(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 5){
		/* printf("Calling fgetchem\n"); */
		num_atoms = fgetchem_3d(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 3){
		/* printf("Calling fgetpdb\n"); */
		num_atoms = fgetpdb(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 8){
		/* printf("Calling fgetmm3\n"); */
		num_atoms = fgetmm3(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 10){
		/* printf("Calling fgetcube\n"); */
		num_atoms = fgetcube(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 11){
		/* printf("Calling fgetgamess\n"); */
		num_atoms = fgetgamess(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 12){
		/* printf("Calling fgetCgamess\n"); */
		num_atoms = fgetCgamess(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 13){
		/* printf("Calling fgetPlainCoord\n"); */
		num_atoms = fgetPlainCoord(in, MAX_ATOMS, location,
			type, amin, fcharge, hydr, atomtype);
	}
	else if (file_type == 0){
		/* printf("Calling file_type == 0\n"); */
		num_atoms = fgetcoord(in, MAX_ATOMS, location,
		type, amin, fcharge, hydr, atomtype);
	}
	fclose(in);

	num_bonds = makebonds(location, type, num_atoms,
		MAX_BONDS, bonds, units, bscale, hbscale, atomtype);
/*
	printf("Lastly, num_atoms = %d, num_bonds = %d\n",
			num_atoms, num_bonds);
*/
	if (rep_type == COLORED_PROTEIN)
		makeatomamin(amin, num_atoms, atomamin);

	if (rep_type == COLORED_STICK) {
		for (i = 0; i < num_atoms; i++) {
			bond_colors[i][0] = atomcolors[atomtype[i]][0];
			bond_colors[i][1] = atomcolors[atomtype[i]][1];
			bond_colors[i][2] = atomcolors[atomtype[i]][2];
		}
		vcolors = 1;
	}
	else if (rep_type == COLORED_PROTEIN) {
		for (i = 0; i < num_atoms; i++) {
			bond_colors[i][0] = amincolors[atomamin[i]][0];
			bond_colors[i][1] = amincolors[atomamin[i]][1];
			bond_colors[i][2] = amincolors[atomamin[i]][2];
		}
		vcolors = 1;
	}
	else vcolors = 0;

	bond_count = 0;
	for(i = 0; i < num_bonds; i++) {
		a = bonds[i][0];
		b = bonds[i][1];

		bb_lst[2*bond_count][0] = location[a][0];
		bb_lst[2*bond_count][1] = location[a][1];
		bb_lst[2*bond_count][2] = location[a][2];

		if (vcolors) {
			bb_rgb_lst[2*bond_count][0] = bond_colors[a][0];
			bb_rgb_lst[2*bond_count][1] = bond_colors[a][1];
			bb_rgb_lst[2*bond_count][2] = bond_colors[a][2];

			bb_rgb_lst[2*bond_count+1][0] = bond_colors[a][0];
			bb_rgb_lst[2*bond_count+1][1] = bond_colors[a][1];
			bb_rgb_lst[2*bond_count+1][2] = bond_colors[a][2];
		}

		if (vcolors && (bond_colors[a][0] != bond_colors[b][0] ||
			bond_colors[a][1] != bond_colors[b][1] ||
			bond_colors[a][2] != bond_colors[b][2])) {

			bb_lst[2*bond_count+2][0] = bb_lst[2*bond_count+1][0] =
				(location[a][0] + location[b][0])/2.;
			bb_lst[2*bond_count+2][1] = bb_lst[2*bond_count+1][1] =
				(location[a][1] + location[b][1])/2.;
			bb_lst[2*bond_count+2][2] = bb_lst[2*bond_count+1][2] =
				(location[a][2] + location[b][2])/2.;

			bb_rgb_lst[2*bond_count+2][0] = bond_colors[b][0];
			bb_rgb_lst[2*bond_count+2][1] = bond_colors[b][1];
			bb_rgb_lst[2*bond_count+2][2] = bond_colors[b][2];

			bb_rgb_lst[2*bond_count+3][0] = bond_colors[b][0];
			bb_rgb_lst[2*bond_count+3][1] = bond_colors[b][1];
			bb_rgb_lst[2*bond_count+3][2] = bond_colors[b][2];

			bond_count++;
		}

		bb_lst[2*bond_count+1][0] = location[b][0];
		bb_lst[2*bond_count+1][1] = location[b][1];
		bb_lst[2*bond_count+1][2] = location[b][2];
		atom_connect[bond_count+1] = a;
		atom_connect[2*bond_count+1] = b;
		bond_count++;
	}
/***********************/
/*  Set output values  */
/***********************/

/* Set num_atoms's value */
	OMset_name_int_val(Mol_c_id, OMstr_to_name("num_atoms"),
		num_atoms);

/* The atomic positions ---- a_lst */
	a_lst_size = num_atoms * 3;
	a_lst = (float *)OMret_name_array_ptr(Mol_c_id,
		OMstr_to_name("a_lst"), OM_GET_ARRAY_RW, &a_lst_size, NULL);

/* The atom colors ---- rgb_lst */
	rgb_lst_size = num_atoms * 3;
	rgb_lst = (float *)OMret_name_array_ptr(Mol_c_id,
		OMstr_to_name("rgb_lst"), OM_GET_ARRAY_RW, &rgb_lst_size,
		NULL);

/* The atomic radii ---- r_lst */
	r_lst_size = num_atoms;
	r_lst = (float *)OMret_name_array_ptr(Mol_c_id,
		OMstr_to_name("r_lst"), OM_GET_ARRAY_RW,
		&r_lst_size, NULL);

/* The charge array (if charges are present) ---- charge */
	charge_size = num_atoms;
	charge = (float *)OMret_name_array_ptr(Mol_c_id,
		OMstr_to_name("charge"), OM_GET_ARRAY_RW,
		&charge_size, NULL);

/* Set num_bonds's value */
	OMset_name_int_val(Mol_c_id, OMstr_to_name("num_bonds"),
		num_bonds);

/* Set atom_name array */
	atom_name_id = OMfind_subobj(Mol_c_id, OMstr_to_name("atom_name"),
			OM_OBJ_RW);
	atom_name_size = num_atoms;
	OMset_array_size(atom_name_id, atom_name_size);

/* Set residue_name array */
	residue_name_id = OMfind_subobj(Mol_c_id,
			OMstr_to_name("residue_name"), OM_OBJ_RW);
	residue_name_size = num_atoms;
	OMset_array_size(residue_name_id, residue_name_size);

/* The atom connect list ---- connect_lst */
	connect_lst_size = num_bonds * 2;
	connect_lst = (int *)OMret_name_array_ptr(Mol_c_id,
		OMstr_to_name("connect_lst"), OM_GET_ARRAY_RW,
		&connect_lst_size, NULL);
/*
	printf("Finished the set up for the Express objects\n");
*/
/*
	Fill the arrays for Express

	if BALL and BALL_AND_STICK
		fill:
			Atom Positions
			Atom Colors
	if BALL_AND_STICK, COLORED_PROTEIN, COLORED_STICK and STICK
		fill:
			Atom Connection list
*/
	if(rep_type == BALL_AND_STICK){
		/* printf("in BALL_AND_STICK\n"); */
		for(i = 0; i < num_atoms; i++) {
			*a_lst = location[i][0];
			a_lst++;
			*a_lst = location[i][1];
			a_lst++;
			*a_lst = location[i][2];
			a_lst++;
			*rgb_lst = atomcolors[atomtype[i]][0];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][1];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][2];
			rgb_lst++;
			*r_lst = radius[atomtype[i]] * ballscale;
			r_lst++;
		}
		for(i = 0; i < num_bonds; i++) {
			*connect_lst = bonds[i][0];
			connect_lst++;
			*connect_lst = bonds[i][1];
			connect_lst++;
		}
		for (atom_name_count = 0; atom_name_count < atom_name_size;
					atom_name_count++){
			if(names_a == 1) OMset_str_array_val(atom_name_id,
				atom_name_count, &type[atom_name_count][0]);
			else if(names_a == 0) OMset_str_array_val(atom_name_id,
				atom_name_count,
				&blank);
		}
		for (residue_name_count = 0;
				residue_name_count < residue_name_size;
				residue_name_count++){
			if(names_r == 1) OMset_str_array_val(residue_name_id,
				residue_name_count,     &amin[residue_name_count][0]);
			else if(names_r == 0) OMset_str_array_val(residue_name_id,
				residue_name_count, &blank);
		}
	}
	else if(rep_type == STICK){
		/* printf("in STICK\n"); */
		for(i = 0; i < num_atoms; i++) {
			*a_lst   = location[i][0];
			a_lst++;
			*a_lst   = location[i][1];
			a_lst++;
			*a_lst   = location[i][2];
			a_lst++;
			*rgb_lst = 0.5;
			rgb_lst++;
			*rgb_lst = 1.0;
			rgb_lst++;
			*rgb_lst = 0.5;
			rgb_lst++;
			*r_lst = radius[atomtype[i]] * 0.0;
			r_lst++;
	}
		for(i = 0; i < num_bonds; i++) {
			*connect_lst = bonds[i][0];
			connect_lst++;
			*connect_lst = bonds[i][1];
			connect_lst++;
		}
		for (atom_name_count = 0; atom_name_count < atom_name_size;
							atom_name_count++){
			if(names_a == 1) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&type[atom_name_count][0]);
			else if(names_a == 0) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&blank);
		}
		for (residue_name_count = 0;
				residue_name_count < residue_name_size;
				residue_name_count++) {
			if(names_r == 1) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&amin[residue_name_count][0]);
			else if(names_r == 0) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&blank);
		}
	}
	else if(rep_type == COLORED_STICK){
		/* printf("in COLORED_STICK\n"); */
		for(i = 0; i < num_atoms; i++) {
			*a_lst   = location[i][0];
			a_lst++;
			*a_lst   = location[i][1];
			a_lst++;
			*a_lst   = location[i][2];
			a_lst++;
			*rgb_lst = atomcolors[atomtype[i]][0];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][1];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][2];
			rgb_lst++;
			*r_lst = radius[atomtype[i]] * 0.0;
			r_lst++;
	}
		for(i = 0; i < num_bonds; i++) {
			*connect_lst = bonds[i][0];
			connect_lst++;
			*connect_lst = bonds[i][1];
			connect_lst++;
		}
		for (atom_name_count = 0; atom_name_count < atom_name_size;
							atom_name_count++){
			if(names_a == 1) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&type[atom_name_count][0]);
			else if(names_a == 0) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&blank);
		}
		for (residue_name_count = 0;
				residue_name_count < residue_name_size;
				residue_name_count++) {
			if(names_r == 1) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&amin[residue_name_count][0]);
			else if(names_r == 0) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&blank);
		}
	}
	else if(rep_type == CPK){
		/* printf("in CPK\n"); */
		for(i = 0; i < num_atoms; i++) {
			*a_lst = location[i][0];
			a_lst++;
			*a_lst = location[i][1];
			a_lst++;
			*a_lst = location[i][2];
			a_lst++;
			*rgb_lst = atomcolors[atomtype[i]][0];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][1];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][2];
			rgb_lst++;
			*r_lst = radius[atomtype[i]] * ballscale;
			r_lst++;
	}
		for(i = 0; i < num_bonds; i++) {
			*connect_lst = 0;
			connect_lst++;
			*connect_lst = 0;
			connect_lst++;
		}
		for (atom_name_count = 0; atom_name_count < atom_name_size;
							atom_name_count++){
			if(names_a == 1) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&type[atom_name_count][0]);
			else if(names_a == 0) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&blank);
		}
		for (residue_name_count = 0;
				residue_name_count < residue_name_size;
				residue_name_count++){
			if(names_r == 1) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&amin[residue_name_count][0]);
			else if(names_r == 0) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&blank);
		}
	}
	else if(rep_type == COLORED_RESIDUE) {
		/* printf("in COLORED_RESIDUE\n"); */
		for(i = 0; i < num_atoms; i++) {
			*a_lst   = location[i][0];
			a_lst++;
			*a_lst   = location[i][1];
			a_lst++;
			*a_lst   = location[i][2];
			a_lst++;
			*rgb_lst = atomcolors[atomtype[i]][0];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][1];
			rgb_lst++;
			*rgb_lst = atomcolors[atomtype[i]][2];
			rgb_lst++;
			*r_lst = radius[atomtype[i]] * 0.0;
			r_lst++;
	}
		for(i = 0; i < num_bonds; i++) {
			*connect_lst = bonds[i][0];
			connect_lst++;
			*connect_lst = bonds[i][1];
			connect_lst++;
		}
		for (atom_name_count = 0; atom_name_count < atom_name_size;
							atom_name_count++){
			if(names_a == 1) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&type[atom_name_count][0]);
			else if(names_a == 0) OMset_str_array_val(atom_name_id,
						atom_name_count,
						&blank);
		}
		for (residue_name_count = 0;
				residue_name_count < residue_name_size;
				residue_name_count++) {
			if(names_r == 1) OMset_str_array_val(residue_name_id,
						residue_name_count,
						&amin[residue_name_count][0]);
			else if(names_r == 0) OMset_str_array_val(
						residue_name_id,
						residue_name_count,
						&blank);
		}
	}
/*************************/
/*  Free input variables */
/*************************/

	a_lst = a_lst - a_lst_size;
    r_lst = r_lst - r_lst_size;
    rgb_lst = rgb_lst - rgb_lst_size;
    connect_lst = connect_lst - connect_lst_size;

	if (File_Name) free(File_Name);
	if (a_lst != NULL) ARRfree(a_lst);
	if (r_lst != NULL) ARRfree(r_lst);
	if (rgb_lst != NULL) ARRfree(rgb_lst);
	if (connect_lst != NULL) ARRfree(connect_lst);

	trigger = 0;
	printf("Mol_c returning\n");
	return(1);
}


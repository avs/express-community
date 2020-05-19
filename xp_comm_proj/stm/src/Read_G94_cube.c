/*
        Reads a Gaussian 94 Cube file and ouputs arrays for a
rectilinear mesh.
        Written by Lee Bartolotti from a DMol reader written by
Ken Flurchick and Lee Bartolotti.  Modified by Ken Flurchick for
AVS/Express.
*/

#include "express.h"
#define MAX_STRING    256       /* Maximun string buffer length */
#define STORAGE    750000

int
Read_G94_cube(OMobj_id Read_G94_cube_id, OMevent_mask event_mask, int seq_num)
{
/***********************/
/*  Declare variables  */
/***********************/
	char  	*File_Name = NULL;
	int  	total_data;
	int  	total_coords;
	int  	Orbital_Data_size = 0;
	float 	*Orbital_Data = NULL;
	int  	geometry_size = 0;
	float 	*geometry = NULL;
	int  	dims_size = 0;
	int 	*dims = NULL;

	FILE 	*in;
	int 	i = 0, j = 0, k = 0, kount = 0;
	float 	x0, y0, z0, cord, tmp;
	float 	Tmp[STORAGE];
	int 	n_atom, nt_atom, n1, n2, n3;
	int 	nmo, nmo_num;
	float 	dx1, dy1, dz1;
	float 	dx2, dy2, dz2;
	float 	dx3, dy3, dz3;
	float 	x, y, z;
	float 	chg, min=0.0, max=0.0;
	int 	iatom;
	int 	n_atoms = 0;
	int 	total = 0;
	char 	Title1[MAX_STRING], Title2[MAX_STRING];
	char 	*T1, *T2;

	T1 = &Title1[0];
	T2 = &Title2[0];
/***********************/
/*  Get input values   */
/***********************/
	/* Get File_Name's value */
	if (OMget_name_str_val(Read_G94_cube_id,
		OMstr_to_name("File_Name"),
		&File_Name, 0) != 1) File_Name = NULL;
/***********************/
/* Function's Body     */
/***********************/
/*
	ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: Read_G94_cube generated from method: Read_G94_cube.Update\n");
*/
/*
	Open the file
*/
	in = fopen(File_Name, "r");
	if (in == NULL) return(0);
/*
	Read in two title lines
*/
	fgets(T1, MAX_STRING, in);
	fgets(T2, MAX_STRING, in);
/*
	Read in number of atoms, x-origin, y-origin z-origin
*/
	fscanf(in, "%d %f %f %f", &nt_atom, &x0, &y0, &z0);
	if(nt_atom < 0 )  n_atom = -nt_atom;
	else n_atom = nt_atom;
	n_atoms = n_atom;

	printf("n_atom = %d\n", n_atom);
	printf("x0, y0, z0 = %f  %f  %f \n", x0, y0, z0);

/*
        Read in number of increments in slowest running direction
to fastest running direction
*/
	fscanf(in, "%d %f %f %f", &n1, &dx1, &dy1, &dz1);
	fscanf(in, "%d %f %f %f", &n2, &dx2, &dy2, &dz2);
	fscanf(in, "%d %f %f %f", &n3, &dx3, &dy3, &dz3);
/*
	Read in atomic number, charge and coordinates of atom i:
not used, just dummied
*/
	for(k = 0; k < n_atom; k++) {
		fscanf(in, "%d %f %f %f %f", &iatom, &chg, &x, &y, &z);
	}
	if(nt_atom < 0){
		fscanf(in,"%d %d", &nmo, &nmo_num);
		if(nmo != 1) {
			printf("Not set up for reading multiple orbitals\n");
			return(5);
		}
	}
/*
	Read and store the value data
*/
	total = 0;
	for(k = 0; k < n3; k++){
		for(j = 0; j < n2; j++){
			for(i = 0; i < n1; i++){
				fscanf(in,"%g", &tmp);
				if(tmp < min) min = tmp;
				if(tmp > max) max = tmp;
				Tmp[total] = tmp;
				total++;
			}
		}
	}
	total_data = n1 * n2 * n3;
	total_coords = (n1 * n2 * n3) * 3;
	printf("Read the data\n");
	fclose(in);
/***********************/
/*  Set output values  */
/***********************/
	/* Set total_data's value */
	OMset_name_int_val(Read_G94_cube_id,
		OMstr_to_name("total_data"), total_data);

	/* Set total_coords's value */
	OMset_name_int_val(Read_G94_cube_id,
		OMstr_to_name("total_coords"), total_coords);

/* set number of elements in array:  Orbital_Data */
	Orbital_Data_size = total_data;
	Orbital_Data = (float *)OMret_name_array_ptr(Read_G94_cube_id,
				OMstr_to_name("Orbital_Data"), OM_GET_ARRAY_RW,
				&Orbital_Data_size, NULL);

/* set number of elements in array: geometry */
	geometry_size = total_coords;
	geometry = (float *)OMret_name_array_ptr(Read_G94_cube_id,
				OMstr_to_name("geometry"), OM_GET_ARRAY_RW,
				&geometry_size, NULL);

/* set number of elements in array: dims */
	dims_size = 3;
	dims = (int *)OMret_name_array_ptr(Read_G94_cube_id,
	       	   	OMstr_to_name("dims"), OM_GET_ARRAY_RW,
    			&dims_size, NULL);
/*
	Fill the arrays for Express

	Orbital_Data is the data array for the mesh
	geometry is the points array for the mesh
			 is the dimensions for the problem
*/
	total = 0;
	for(k = 0; k < n1; k++){
		for(j = 0; j < n2; j++){
			for(i = 0; i < n3; i++){
				*Orbital_Data = Tmp[total];
				Orbital_Data++;
				total++;
			}
		}
	}
	kount = 0;
	for (i = 1; i <= n1; i++) {
		for (j = 1; j <= n2; j++) {
			for (k = 1; k <= n3; k++) {
				*geometry = z0 + (k-1)*dz3;
				geometry++;
				*geometry = y0 + (j-1)*dy2;
				geometry++;
				*geometry = x0 + (i-1)*dx1;
				geometry++;
				kount++;
			}
		}
	}
	*dims = n3;
	dims++;
	*dims = n2;
	dims++;
	*dims = n1;
	dims++;
	/* printf("Filled the Express Objects\n"); */
/*************************/
/*  Free input variables */
/*************************/
	Orbital_Data = Orbital_Data - Orbital_Data_size;
	geometry = geometry - geometry_size;
	dims = dims - dims_size;

	if (File_Name) free(File_Name);
	if (Orbital_Data != NULL) ARRfree(Orbital_Data);
	if (geometry != NULL) ARRfree(geometry);
	if (dims != NULL) ARRfree(dims);

	return(1);
}

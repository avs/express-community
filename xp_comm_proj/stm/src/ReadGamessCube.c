/*
        Reads a GAMESS CubePlot file file and ouputs arrays for a
structuredr mesh.
        Written by by Ken Flurchick for AVS/Express.
	GANESS plot file code written bt Theresa Windus.

  	Read a plot file from GAMESS
  	Here is some sample output:

 	#of_atoms      #of_vectors
 	#of_x_pts      #of_y_pts       #of_z_pts
 	xmin xmax      ymin ymax       zmin zmax
 	coordinates in GAMESS_format
 	vec1 vec2 vec3 ... vec_max_num
 	...
 	...

*/
#include "express.h"
#define MAX_STRING    256	/* Maximun string buffer length */
#define STORAGE     50000
#define MAX_VECLEN     64	/* Maximum number of elements to read */

int
readcgamess(OMobj_id ReadGamessCube_id, OMevent_mask event_mask, int seq_num)
{
/***********************/
/*  Declare variables  */
/***********************/
	char  	*FileName = NULL;
	int  	TotalSize;
	int  	dims_size = 0;
	int 	*dims = NULL;
	int  	Coords_size = 0;
	float 	*Coords = NULL;
	int  	Data_size = 0;
	float 	*Data = NULL;

	FILE 	*in;
	char 	dum1[255];
	int 	i = 0, j = 0, k = 0, l = 0, kount = 0;
	int 	nx = 0, ny = 0, nz = 0;
	int 	num_atoms = 0, num_vec = 0, veclen = 1;
	int 	total = 0, trigger = 0;
	float 	xmin, xmax, ymin, ymax, zmin, zmax;
	float 	dx, dy, dz, rx, ry, rz;
	float 	catom;
	float 	Tmp[STORAGE][MAX_VECLEN], tmp;
/***********************/
/*  Get input values   */
/***********************/
   	/* Get FileName's value */
   	if (OMget_name_str_val(ReadGamessCube_id, OMstr_to_name("FileName"),
		&FileName, 0) != 1) FileName = NULL;

	/* Get trigger's value */
	if (OMget_name_int_val(ReadGamessCube_id, OMstr_to_name("trigger"),
		&trigger) != 1) trigger = 0;
/***********************/
/* Function's Body     */
/***********************/
/*
   	ERRverror("",ERR_NO_HEADER | ERR_INFO,
		"I'm in function: ReadGamessCube.ReadCGamess\n");
*/
/*  Open the file */
	if (trigger != 1){
		printf("Reading file %s\n", FileName);
		in = fopen(FileName,"r");
/*
	Read in the
 	#of_atoms      #of_vectors
*/
		fscanf(in, "%d %d\n", &num_atoms, &num_vec);
		/*
		printf("In ReadCGamess, num_atoms = %d num_vec = %d\n",
			num_atoms, num_vec);
		*/
/*
	Read in geometry of the box

	#of_x_pts      #of_y_pts       #of_z_pts
	xmin xmax      ymin ymax       zmin zmax
	coordinates in GAMESS_format
*/
		fscanf(in, "%d %d %d\n", &nx, &ny, &nz);
		fscanf(in, "%f %f %f\n", &xmin, &xmax, &ymin);
		fscanf(in, "%f %f %f\n", &ymax, &zmin, &zmax);

		rx = (xmax - xmin)/nx;
		ry = (ymax - ymin)/ny;
		rz = (zmax - zmin)/nz;
		/*
		printf("Max/Min %f %f %f %f %f %f\n",
			xmax, xmin, ymax, ymin, zmax, zmin);
		printf("Step sizes %f %f %f\n", rx, ry, rz);
		*/
		k = 0;
		while(k < num_atoms){
			fscanf(in, "%s %f %f %f %f",
				&dum1, &catom, &dx, &dy, &dz);
			/*
			printf("In ReadCGamess, %d %s %f %f %f %f\n",
				k, dum1, catom, dx, dy, dz);
			*/
	        k++;
		}
/*
	Read in the data
 	vec1 vec2 vec3 ... vec_max_num
*/
		total = 0;
		for (k = 0; k < nx; k++){
			for (j = 0; j < ny; j++){
				for (i = 0; i < nz; i++){
					for (l = 0; l < num_vec; l++){
						fscanf(in, "%g", &tmp);
						Tmp[total][l] = tmp;
					}
					total++;
				}
			}
		}
/***********************/
/*  Set output values  */
/***********************/
		veclen = num_vec; /* veclen = 1 */

		/* Set TotalSize's value */
		OMset_name_int_val(ReadGamessCube_id,
			OMstr_to_name("TotalSize"), veclen);

		/* set number of elements in array: dims: */
		dims_size = 3;
		dims = (int *)OMret_name_array_ptr(ReadGamessCube_id,
			OMstr_to_name("dims"), OM_GET_ARRAY_RW,
			&dims_size, NULL);
		*dims = nx;
		dims++;
		*dims = ny;
		dims++;
		*dims = nz;
		dims++;

		/* set number of elements in array: Coords: */
		Coords_size = total * 3;
		Coords = (float *)OMret_name_array_ptr(ReadGamessCube_id,
			OMstr_to_name("Coords"), OM_GET_ARRAY_RW,
			&Coords_size, NULL);
		kount = 0;
		for (i = 1; i <= nx; i++) {
			for (j = 1; j <= ny; j++) {
				for (k = 1; k <= nz; k++) {
					*Coords = xmin + (i-1)*rx;
					Coords++;
					kount++;
					*Coords = ymin + (j-1)*ry;
					Coords++;
					kount++;
					*Coords = zmin + (k-1)*rz;
					Coords++;
					kount++;
				}
			}
		}

		/* set number of elements in array: Data: */
		Data_size = total;
		Data = (float *)OMret_name_array_ptr(ReadGamessCube_id,
			OMstr_to_name("Data"), OM_GET_ARRAY_RW,
			&Data_size, NULL);

		total = 0;
		for (k = 0; k < nx; k++){
			for (j = 0; j < ny; j++){
				for (i = 0; i < nz; i++){
					for (l = 0; l < num_vec; l++){
						*Data = Tmp[total][l];
						Data++;
					}
					total++;
				}
			}
		}
	} else if (trigger == 0){
		return(0);
	}
/*************************/
/*  Free input variables */
/*************************/
	Data = Data - Data_size;
	Coords = Coords - Coords_size;
	dims = dims - dims_size;

	if (FileName) free(FileName);
	if (Data != NULL) ARRfree(Data);
	if (Coords != NULL) ARRfree(Coords);
	if (dims != NULL) ARRfree(dims);
	return(1);
}



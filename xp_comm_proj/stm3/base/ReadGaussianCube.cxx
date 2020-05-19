/* Original code from:

Reads a Gaussian 94 Cube file and ouputs arrays for a rectilinear mesh.
Written by Lee Bartolotti from a DMol reader written by
Ken Flurchick and Lee Bartolotti.  Modified by Ken Flurchick for AVS/Express.
*/

/*
Updated code by Jean M. Favre, Swiss Center for Scientific Computing
Last update: Tue Nov 27, 2001

Creates a uniform mesh and applies the translation and rotation matrices
required to map from the world coordinates to the [0,0,0]-based orthonormal
coordinates system. This eliminates the explicit storage of coordinates.

Eliminates the temporary storage of 750000 and lets AVS/Express manage the
exact memory required for the Node_Data value.

Assigns a name to the Node_Data based on the title

Reference for the format are in:

http://www.gaussian.com/00000430.htm
*/

#include <math.h>

#define MAX_STRING    256       /* Maximun string buffer length */

#include "ReadGaussianCube_gen.h"
#include "mol_type.h"
#include "../lib/stm3.h"
extern void  GDxform_set_xlate (OMobj_id, float [3]);


int
STM3_BASE_ReadGaussianCube::ReadGaussianCube(OMevent_mask event_mask, int seq_num)
{
	// filename (OMXstr read req notify)
	// units (OMXint read req notify)
	// out (Field_Unif write)

	char     Title[MAX_STRING];
	char     data_name[MAX_STRING];
	int      i, j, k, nmo;
	int      iatom, n_atom, nt_atom, n1, n2, n3, N1N2, JN1;
	float    tmp, xlate[3], *bounds;
	float    dx1, dy1, dz1;
	float    dx2, dy2, dz2;
	float    dx3, dy3, dz3;
	float    x, y, z, chg;
	FILE     *in;
	float    v1, v2;
	char     *p;

	// open the cube file
	in = fopen(filename, "r");
	if(in == NULL) return 0;
	
	/* Read in two title lines */
	strcpy(data_name, "");
	fgets(Title, MAX_STRING, in);
	if((p=strtok(Title, ":")) != NULL)
	{
		strcpy(data_name, p);
	}
	fgets(Title, MAX_STRING, in);

	// Read in number of atoms, x-origin, y-origin z-origin
	if(fscanf(in, "%d %f %f %f", &nt_atom, &xlate[0], &xlate[1], &xlate[2]) < 4) return 0;
	if(nt_atom == 0) return 0;
	if(nt_atom < 0)
		n_atom = -nt_atom;
	else
		n_atom = nt_atom;

	if(units == BOHR)
	{
		xlate[0] *= BOHR_TO_ANGSTROM;
		xlate[1] *= BOHR_TO_ANGSTROM;
		xlate[2] *= BOHR_TO_ANGSTROM;
	}
	
	float *out_xlate = (float *)out.xform.xlate.ret_array_ptr(OM_GET_ARRAY_WR);
	if(out_xlate)
	{
		out_xlate[0] = xlate[0];
		out_xlate[1] = xlate[1];
		out_xlate[2] = xlate[2];
		ARRfree(out_xlate);
	}

	// Read in number of increments in slowest running direction to fastest running direction
	fscanf(in, "%d %f %f %f", &n1, &dx1, &dy1, &dz1);
	fscanf(in, "%d %f %f %f", &n2, &dx2, &dy2, &dz2);
	fscanf(in, "%d %f %f %f", &n3, &dx3, &dy3, &dz3);

	if(units == BOHR)
	{
		dx1 *= BOHR_TO_ANGSTROM;
		dx2 *= BOHR_TO_ANGSTROM;
		dx3 *= BOHR_TO_ANGSTROM;
		dy1 *= BOHR_TO_ANGSTROM;
		dy2 *= BOHR_TO_ANGSTROM;
		dy3 *= BOHR_TO_ANGSTROM;
		dz1 *= BOHR_TO_ANGSTROM;
		dz2 *= BOHR_TO_ANGSTROM;
		dz3 *= BOHR_TO_ANGSTROM;
	}

	float *out_xform = (float *)out.xform.mat.ret_array_ptr(OM_GET_ARRAY_WR);
  	if(out_xform)
	{
		out_xform[0]  = dx1;
		out_xform[1]  = dy1;
		out_xform[2]  = dz1;
		out_xform[3]  = 0.0;

		out_xform[4]  = dx2;
		out_xform[5]  = dy2;
		out_xform[6]  = dz2;
		out_xform[7]  = 0.0;

		out_xform[8]  = dx3;
		out_xform[9]  = dy3;
		out_xform[10] = dz3;
		out_xform[11] = 0.0;

		out_xform[12] = 0.0;
		out_xform[13] = 0.0;
		out_xform[14] = 0.0;
		out_xform[15] = 1.;

		ARRfree(out_xform);
	}

	out.ndim = 3;
	out.nspace = 3;

	int *out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_WR);
	if(out_dims)
	{
		out_dims[0] = n1;
		out_dims[1] = n2;
		out_dims[2] = n3;
		ARRfree(out_dims);
	}

	/* Read in atomic number, charge and coordinates of atom i: not used */

	for(k = 0; k < n_atom; k++)
	{
		fscanf(in, "%d %f %f %f %f", &iatom, &chg, &x, &y, &z);
	}

	/* check if the line with the number of orbitals is present */
	fscanf(in, "%g %g", &v1, &v2);
	nmo = (int)v1;
	if(nmo > 1)
	{
		printf("\nNot set up for reading multiple orbitals\n");
		return 0;
	}
	if(v1 >= 1.-0.000001) fscanf(in, "%g %g", &v1, &v2);

	/* set  Cube node data */
	/* Set number of node data components */
	out.nnode_data = 1;
	out.node_data[0].veclen = 1;
	out.node_data[0].labels = data_name;
	float *Cube_data = (float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(Cube_data)
    {
		N1N2 = n1*n2;
		for(i = 0; i < n1; i++)
		{
			JN1 = 0;
			for(j = 0; j < n2; j++)
			{
				for(k = 0; k < n3; k++)
				{
					if(i==0 && j==0 && k==0)      tmp = v1;
			    	else if(i==0 && j==0 && k==1) tmp = v2;
                	else                          if(fscanf(in, "%g", &tmp) < 1) tmp = 0.;

                	Cube_data[k*N1N2 + JN1 + i] = tmp;
				}
				JN1 += n1;
			}
		}
		ARRfree((char *) Cube_data);
	}

	fclose(in);

	bounds = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_WR);
	if(bounds)
	{
		bounds[0] = 0.0;
		bounds[1] = 0.0;
		bounds[2] = 0.0;
		bounds[3] = (n1 - 1.0);
		bounds[4] = (n2 - 1.0);
		bounds[5] = (n3 - 1.0);	

		ARRfree(bounds);
	}

  return 1;
}


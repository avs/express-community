/*
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetcube.
*/
/*
Updated code by Jean M. Favre, Swiss Center for Scientific Computing
Last update: Tue Nov 27, 2001

*/
#include "stm3.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279
#endif
/*
        Option: cube
        Read a CUBE file from Gaussian, putting the coords into apos
        and the char string names into atype. Return number of atoms.
        Stop after coordinate read.
        Here is some sample input:

Gaussian Test Job 318: Potential cube
SCF Total Density
    3   -9.393328   -9.393328   -8.985941
   10    2.087406    0.000000    0.000000
   10    0.000000    2.087406    0.000000
   10    0.000000    0.000000    2.087406
    8    8.000000    0.000000    0.000000    0.215580
    1    1.000000    0.000000    1.435626   -0.862319
    1    1.000000    0.000000   -1.435626   -0.862319
  1.88175E-03  1.79419E-03  1.51013E-03  9.93986E-04  2.86401E-04 -4.81750E-04
 -1.14728E-03 -1.60245E-03 -1.83108E-03 -1.87998E-03
*/

int read_cube(FILE *fp, int read_hydrogens, vector<Atom> &atoms, UnitCell &unit_cell)
{
	char linebuf[200];
	int k;  /* counter of atoms read */
	int kk; /* counter of atoms */
	float dx, dy, dz;
	float chg;

	int natom, iatom;
	int n1, n2, n3;
	float dx0, dy0, dz0;
	float dx1, dy1, dz1, dx2, dy2, dz2, dx3, dy3, dz3;

	/* read in two title lines */
	fgets(linebuf, sizeof(linebuf), fp);
	fgets(linebuf, sizeof(linebuf), fp);

	// read the number of atoms and the unit cell basis vectors and subdivisions
	if(fscanf(fp, "%d %f %f %f\n", &natom, &dx0, &dy0, &dz0) < 4) return 0;
	fscanf(fp,    "%d %f %f %f\n", &n1, &dx1, &dy1, &dz1);
	fscanf(fp,    "%d %f %f %f\n", &n2, &dx2, &dy2, &dz2);
	fscanf(fp,    "%d %f %f %f\n", &n3, &dx3, &dy3, &dz3);

    if(natom < 0) natom = -natom;
	
	// preallocate storage
	atoms.reserve(natom);
	
    k = kk = 0;
    while(fgets(linebuf, sizeof(linebuf), fp) != NULL && (kk < natom))
	{
		sscanf(&linebuf[0], " %d %f %f %f %f", &iatom, &chg, &dx, &dy, &dz);

		if((read_hydrogens == 1) || (iatom != 1 ))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strcpy(a.name, atom_properties[iatom].atom_name);
			a.c = chg;

			a.atom_z = iatom;

			// no residue name provided
			a.res_id = 0;
			a.res_name[0] = '\0';

			atoms.push_back(a);

			k++;
		}
		kk++;
	}

	// fill the unit cell info
	unit_cell.side_vectors[0][0] = dx1*(n1-1); unit_cell.side_vectors[0][1] = dy1*(n1-1); unit_cell.side_vectors[0][2] = dz1*(n1-1);
	unit_cell.side_vectors[1][0] = dx2*(n2-1); unit_cell.side_vectors[1][1] = dy2*(n2-1); unit_cell.side_vectors[1][2] = dz2*(n2-1);
	unit_cell.side_vectors[2][0] = dx3*(n3-1); unit_cell.side_vectors[2][1] = dy3*(n3-1); unit_cell.side_vectors[2][2] = dz3*(n3-1);

	unit_cell.origin[0] = dx0;
	unit_cell.origin[1] = dy0;
	unit_cell.origin[2] = dz0;
	
	unit_cell.angles[0] = (float)(acos((dx3*dx2+dy3*dy2+dz3*dz2)/sqrt((dx3*dx3+dy3*dy3+dz3*dz3)*(dx2*dx2+dy2*dy2+dz2*dz2)))*180./M_PI);
	unit_cell.angles[1] = (float)(acos((dx3*dx1+dy3*dy1+dz3*dz1)/sqrt((dx3*dx3+dy3*dy3+dz3*dz3)*(dx1*dx1+dy1*dy1+dz1*dz1)))*180./M_PI);
	unit_cell.angles[2] = (float)(acos((dx1*dx2+dy1*dy2+dz1*dz2)/sqrt((dx1*dx1+dy1*dy1+dz1*dz1)*(dx2*dx2+dy2*dy2+dz2*dz2)))*180./M_PI);

	return k;
}

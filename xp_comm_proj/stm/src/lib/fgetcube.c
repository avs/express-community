/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetcube.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

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

int fgetcube(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
FILE *f;
int maxnatoms;
float apos[][3];
char atype[][4];
char aamin[][6];
float charge[];
int atomtype[];
int hydr;
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	float chg;

	int natom;
	int iatom;

	k=1;
	while(k <= 3 ){
		fgets(linebuf, sizeof linebuf, f);
		k++;
	}

	sscanf(&linebuf[0]," %d", &natom);

	k=1;
	while(k <= 3 ){
		fgets(linebuf, sizeof linebuf, f);
		k++;
	}

	if(natom < 0) natom=-natom;
	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < natom) {
		sscanf(&linebuf[0]," %d %f %f %f %f",
			&iatom, &chg, &dz, &dy, &dx) ;

		if(hydr == 0){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],atom_names[iatom-1]);
			atomtype[k] = iatom-1;
			charge[k] = chg;
			k++;
		}else if(iatom != 1 ){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],atom_names[iatom-1]);
			atomtype[k] = iatom-1;
			charge[k] = chg;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}

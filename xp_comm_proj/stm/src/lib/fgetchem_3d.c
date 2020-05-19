/*
	Created October 16, 1997 by Theresa Windus and Ken Flurchick.
	Taken from coord_to_geom.c to create a reusable library.
	The original code was written by Lee Bartolotti and Ken Flurchick.

	Basic library source for fgetchem_3d.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: chem_3d
        Read a CHEM-3D file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

540
C    1   -6.345535   -0.819672    8.125168    1    2    6  111
C    2   -5.099655   -1.005203    8.798187    1    1    3  173
C    3   -4.514252   -2.309235    8.871323    1    2    4  216
C    4   -5.176743   -3.422500    8.273132    1    3    5  221
C    5   -6.482925   -3.259781    7.752930    1    4    6    7
*/

int fgetchem_3d(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
char dum1[255];

	fgets(linebuf, sizeof linebuf, f);

	k=0;
	while(fgets(linebuf, sizeof linebuf, f)!= NULL
		&& k < maxnatoms ){
		sscanf(&linebuf[2]," %2s",dum1);
		sscanf(&linebuf[9]," %f %f %f", &dx, &dy, &dz) ;

		if(hydr == 0) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
	 		charge[k] = 0.00;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h')) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
	 		charge[k] = 0.00;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}


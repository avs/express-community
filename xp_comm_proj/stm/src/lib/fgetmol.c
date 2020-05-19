/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmol.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: mol
        Read a MOL file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

                                         1.000   1.000   1.000
                       90.000  90.000  90.000
  61

   1 O      -1.99050  -3.82015  -0.25595   21  41   0   0   0   0
   2 O      -3.53075   0.44747   0.16308   22  42   0   0   0   0
   3 O       0.82619  -3.51817   0.62851   23  43   0   0   0   0
   4 O      -2.98560   1.99548  -2.14861   24  44   0   0   0   0
   5 O      -0.54806   2.46364  -0.81459   25  45   0   0   0   0
   6 O       1.64986   3.31839  -2.11407   26  46   0   0   0   0
*/

int fgetmol(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	fgets(linebuf, sizeof linebuf, f);
	fgets(linebuf, sizeof linebuf, f);
	fgets(linebuf, sizeof linebuf, f);

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms ){
		sscanf(&linebuf[5]," %2s", dum1);
		sscanf(&linebuf[9]," %f %f %f",
			&dx, &dy, &dz) ;
		if(hydr == 0) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
			charge[k] = 0.00;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h') ) {
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

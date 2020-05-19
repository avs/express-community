/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetespcoord.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: mopac_esp
        Read a CONVEX mopac ESP file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

  protonated complex structure 1
  backbone anchored
    PM3 XYZ MMOK SETUP=KEYS AVSWAVE 1SCF
    GNORM=0.001 SCFCRT=1.0D-10 NOLOG PRECISE NOANCI NODIIS
  130  -23.124657  -22.388311  -21.155438
  322    0.000000    0.000000    0.755905
   -1    0.000000    0.755905    0.000000
  322    0.755905    0.000000    0.000000
          6    0.6000000E+01   -0.7892052E+01   -0.1296199E+02    0.2722263E+01
          1    0.1000000E+01   -0.5812176E+01   -0.1296199E+02    0.2722263E+01
          6    0.6000000E+01   -0.8921321E+01   -0.1031720E+02    0.2722263E+01
*/

int fgetespcoord(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	float dummy;
	float dx;
	float dy;
	float dz;

	int natom;

	k=1;
	while(k <= 8 ) {
		fgets(linebuf, sizeof linebuf, f);
		k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms
			&& 0!=strncmp("AL", linebuf,2)) {
		sscanf(&linebuf[0]," %d %f %f %f %f", &natom,
			&dummy, &dx, &dy, &dz) ;
		if(hydr == 0) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],atom_names[natom-1]);
			charge[k] = 0.00;
			atomtype[k] = natom-1;
			k++;
		} else if(natom != 1 ) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],atom_names[natom-1]);
			charge[k] = 0.00;
			atomtype[k] = natom-1;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}

/*
        Read a set of coordinates for structure
        and charge field.

        Written by Ken Flurchick        Date:05/17/99

        Basic library source for fgetSurface.
*/
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

int fgetSurface(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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

	char dum1[4];
	int natom;
	int iatom;
	int i;

	k = 0;
	while(k <= 4 ) {
		fgets(linebuf, sizeof linebuf, f);
		k++;
	}
/*
	Read the input coordinate file
*/
	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < 1000 && 0!=strncmp(" --", linebuf,3)){
		sscanf(&linebuf[0],"%s %d %f %f %f\n",
			&dum1, &chg, &dx, &dy, &dz);
		if(hydr == 0) {
			apos[k][0] = dx;
			apos[k][1] = dy;
			apos[k][2] = dz;
			strcpy(atype[k],dum1);
			charge[k] = chg;
			k++;
		} else if(!(dum1[0]=='H' || dum1[0]=='h')) {
			apos[k][0] = dx;
			apos[k][1] = dy;
			apos[k][2] = dz;
			strcpy(atype[k],dum1);
			charge[k] = chg;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}

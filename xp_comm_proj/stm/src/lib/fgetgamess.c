/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetgamess.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
        Option: pgamess
        Read in GAMESS xyz format from the PERL extraction, putting the
        coords into apos, the char string names into atype, and the
        charges in charge.
        Here is some sample input:

  1  C           6.0     0.0000000000        0.0000000000       -0.1699098334
  2  H           1.0    -1.6872904675        0.0000000000        1.0115436711
  3  H           1.0     1.6872904675        0.0000000000        1.0115436711
*/

int fgetgamess(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	float catom;
	int dummy;

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms){
		sscanf(&linebuf[0]," %d %s %f %f %f %f", &dummy,
			dum1, &catom, &dx, &dy, &dz) ;
		if(hydr == 0){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
			charge[k] = catom;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h') ){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
			charge[k] = catom;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}


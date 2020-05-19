/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmopac.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: mopac_xyz
        Read a MOPAC xyz coordinte file, putting the coords into apos and
        the char string names into atype. Return number of atoms.
        Stop reading on End-of-file,  blank line or blank atom.
        Here is some sample input:

AM1 XYZ T=02.00D GNORM=0.01 MODE=1 SCFCRT=1.D-7 LET
/users/natalia/chris12.dat
chris12
   O    0.0000000000000  1    0.0000000000000  1    0.0000000000000  1
   N    1.1593000000000  1    0.0000000000000  1    0.0000000000000  1
   C    1.8677000000000  1    1.2712000000000  1    0.0000000000000  1
   C    3.2845000000000  1    1.2280000000000  1    0.0040000000000  1
*/

int fgetmopac(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	int dum;
	float dx;
	float dy;
	float dz;
	float tcharge;
	char dum1[255];

	k=1;
	while(fgets(linebuf, sizeof linebuf, f)!=NULL
			&& k <= 2){
		if(strrchr(linebuf,'+') == NULL ) k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms ){
		if(linebuf[3] == '0' || linebuf[0]  == '\n'
			|| linebuf[3] == ' ') break;

		sscanf(&linebuf[4]," %f %d %f %d %f",
			&dx, &dum, &dy, &dum, &dz) ;
		sscanf(&linebuf[0]," %s", dum1);

		if(hydr == 0) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strncpy(atype[k],dum1,2);
			charge[k] = 0.00;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h') ) {
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strncpy(atype[k],dum1,2);
			charge[k] = 0.00;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}


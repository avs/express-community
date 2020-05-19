/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetdcar.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"


/*
	Option: dcar
        Read a dmol DCAR file putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line.
        Here is some sample input:

!BIOSYM archive 3
PBC=OFF
Dialog output of CAR file
!DATE Thu Jan 29 23:17:53 1998
C1       2.173573433    0.961526260   -1.297810015 dmol 1      ???     C   0.056
N2       0.949602589    1.044543878   -1.877666392 dmol 1      ???     N  -0.039
C3       0.834991771    1.051056677   -3.212643498 dmol 1      ???     C   0.036
C4       1.943454333    0.959837557   -4.033515517 dmol 1      ???     C  -0.020
C5       3.204077978    0.876387343   -3.458677357 dmol 1      ???     C  -0.011
C6       3.317632617    0.880345325   -2.079999151 dmol 1      ???     C  -0.029
H7      -0.203321155    1.111136731   -3.577315730 dmol 1      ???     H   0.053
H8       1.809054380    0.959477872   -5.120608244 dmol 1      ???     H   0.089
H9       4.101509922    0.803186132   -4.084409124 dmol 1      ???     H   0.086
H10      4.302412308    0.814418233   -1.604968613 dmol 1      ???     H   0.075
.
.
end
end
*/

int fgetdcar(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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

	k=0;
	fgets(linebuf, sizeof linebuf, f);
	fgets(linebuf, sizeof linebuf, f);
	fgets(linebuf, sizeof linebuf, f);
	fgets(linebuf, sizeof linebuf, f);
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms
			&& 0!=strncmp("END", linebuf,3)
			&& 0!=strncmp("end", linebuf,3)) {
		sscanf(&linebuf[0]," %*s %f %f %f %*s %*s %*s %s",
			&dx, &dy, &dz, dum1) ;
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

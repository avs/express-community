/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetcoord.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: coord
        Read a general xyz format, putting the coords into apos, the char
        string names into atype, and the charges in charge.
        Here is some sample input:

    14.0     2.9639742677       -2.9639742677       -2.9639742677
    14.0    -2.9639742677        2.9639742677       -2.9639742677
    14.0    -2.9639742677       -2.9639742677        2.9639742677
    14.0    -2.9639742677        2.9639742677        2.9639742677
    14.0     2.9639742677        2.9639742677       -2.9639742677
    14.0    -2.9639742677       -2.9639742677       -2.9639742677
    14.0     2.9639742677       -2.9639742677        2.9639742677
    14.0     2.9639742677        2.9639742677        2.9639742677
     8.0      .0000000000       -3.5306894327       -3.5306894327
     8.0      .0000000000        3.5306894327       -3.5306894327
     8.0      .0000000000       -3.5306894327        3.5306894327
     8.0     3.5306894327         .0000000000        3.5306894327
     8.0    -3.5306894327         .0000000000       -3.5306894327
     8.0    -3.5306894327         .0000000000        3.5306894327
     8.0     3.5306894327         .0000000000       -3.5306894327
     8.0     3.5306894327        3.5306894327         .0000000000
     8.0    -3.5306894327       -3.5306894327         .0000000000
     8.0     3.5306894327       -3.5306894327         .0000000000
     8.0    -3.5306894327        3.5306894327         .0000000000
     8.0      .0000000000        3.5306894327        3.5306894327
     1.0     4.5627046338       -4.5627046338       -4.5627046338
     1.0    -4.5627046338        4.5627046338       -4.5627046338
     1.0    -4.5627046338       -4.5627046338        4.5627046338
     1.0    -4.5627046338        4.5627046338        4.5627046338
     1.0     4.5627046338        4.5627046338       -4.5627046338
     1.0    -4.5627046338       -4.5627046338       -4.5627046338
     1.0     4.5627046338       -4.5627046338        4.5627046338
     1.0     4.5627046338        4.5627046338        4.5627046338
*/
int fgetPlainCoord(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
FILE *f;
int maxnatoms;
float apos[][3];
char aamin[][6];
float charge[];
int hydr;
int atomtype[];
char atype[][4];
{
	char linebuf[200];
	register int k; /* counter of atoms */

	float dx;
	float dy;
	float dz;

	int iatom;
	float catom;
	int natom;

	k=0;
	printf("In fgetPlainCoord.c\n\n") ;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms ){
		sscanf(&linebuf[0],"%d %f %f %f",
			&iatom, &dx, &dy, &dz) ;
		if(hydr == 0){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			atomtype[k] = iatom - 1;
			strcpy(atype[k],atom_names[iatom-1]);
			k++;
		} else if( !(iatom==1) ){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			atomtype[k] = iatom - 1;
			strcpy(atype[k],atom_names[iatom-1]);
			k++;
		}
	}
	return k;
}

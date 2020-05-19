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

 SILICON    14.0     2.9639742677       -2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677        2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677       -2.9639742677        2.9639742677
 SILICON    14.0    -2.9639742677        2.9639742677        2.9639742677
 SILICON    14.0     2.9639742677        2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677       -2.9639742677       -2.9639742677
 SILICON    14.0     2.9639742677       -2.9639742677        2.9639742677
 SILICON    14.0     2.9639742677        2.9639742677        2.9639742677
 OXYGEN      8.0      .0000000000       -3.5306894327       -3.5306894327
 OXYGEN      8.0      .0000000000        3.5306894327       -3.5306894327
 OXYGEN      8.0      .0000000000       -3.5306894327        3.5306894327
 OXYGEN      8.0     3.5306894327         .0000000000        3.5306894327
 OXYGEN      8.0    -3.5306894327         .0000000000       -3.5306894327
 OXYGEN      8.0    -3.5306894327         .0000000000        3.5306894327
 OXYGEN      8.0     3.5306894327         .0000000000       -3.5306894327
 OXYGEN      8.0     3.5306894327        3.5306894327         .0000000000
 OXYGEN      8.0    -3.5306894327       -3.5306894327         .0000000000
 OXYGEN      8.0     3.5306894327       -3.5306894327         .0000000000
 OXYGEN      8.0    -3.5306894327        3.5306894327         .0000000000
 OXYGEN      8.0      .0000000000        3.5306894327        3.5306894327
 HYDROGEN    1.0     4.5627046338       -4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338        4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338       -4.5627046338        4.5627046338
 HYDROGEN    1.0    -4.5627046338        4.5627046338        4.5627046338
 HYDROGEN    1.0     4.5627046338        4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338       -4.5627046338       -4.5627046338
 HYDROGEN    1.0     4.5627046338       -4.5627046338        4.5627046338
 HYDROGEN    1.0     4.5627046338        4.5627046338        4.5627046338
*/
int fgetcoord(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	int natom;

	k=0;
	printf("In fgetcoord.c\n\n") ;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms ){
		sscanf(&linebuf[0],"%s %f %f %f %f", &dum1,
			&catom, &dx, &dy, &dz) ;
		if(hydr == 0){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k], atom_names[(int)catom-1]);
			charge[k] = catom;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h')){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],atom_names[(int)catom-1]);
			charge[k] = catom;
			k++;
		}
	}
	makeatomtype(atype, k, atomtype);
	return k;
}

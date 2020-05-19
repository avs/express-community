/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetpdb.


        The original pdb module has been modified to correctly
        assure that hydrogens do not display more than one bond.
	Modification by Lee Bartolotti.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
	Option: pdb
        Read a Brookhaven file putting the coords into apos and the char
        string names into atype. Return number of atoms.  M Pique
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

ATOM     19  HN  THR     2      15.386  10.901   4.600
ATOM     20  HOG THR     2      14.161   9.481   4.420
ATOM     21  N   CYS     3      13.507  11.238   8.398
ATOM     22  CA  CYS     3      13.659  10.715   9.763
ATOM     23  C   CYS     3      12.265  10.443  10.307
HETATM   99  FE  XXX     4      33.265  10.443  10.307
ATOM         OT  OME    52     -16.009  30.871 -13.037                -0.543
HETATM       CU+2CU    152     -20.577   2.601 -14.587                 2.000
HETATM       ZN+2ZN    152     -17.911   1.372 -19.974                 2.000
END

These charge values
are non-standard
and are not genuine
PDB. They are optional.
*/

int fgetpdb(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
	float tcharge;
	float dx;
	float dy;
	float dz;
	char dum1[255];
	char dum2[255];

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms
			&& 0!=strcmp(".", linebuf)
			&& 0!=strncmp("END", linebuf,3)
			&& 0!=strncmp("end", linebuf,3)) {
		if((0==strncmp("ATOM",linebuf,4)||
			0==strncmp("atom",linebuf,4) ||
			0==strncmp("HETATM",linebuf,6) ||
			0==strncmp("hetatm",linebuf,6) )
			&& 1==sscanf(&linebuf[13]," %4s", dum1)
			&& 1==sscanf(&linebuf[17],"%3s", dum2)
			&& 3==sscanf(&linebuf[30],"%f %f %f",
			&dx, &dy, &dz)) {
			if(hydr == 0) {
				apos[k][0] = dx ;
				apos[k][1] = dy ;
				apos[k][2] = dz ;
				strncpy(atype[k],dum1,4);
				strncpy(aamin[k],dum2,3);
				if(1 == sscanf(&linebuf[70],"%f", &tcharge))
						charge[k] = tcharge;
	 			else charge[k] = 0.00;
				k++;
			} else if( !(dum1[0]=='H' || dum1[0]=='h') ) {
				apos[k][0] = dx ;
				apos[k][1] = dy ;
				apos[k][2] = dz ;
				strncpy(atype[k],dum1,4);
				strncpy(aamin[k],dum2,3);
				if(1 == sscanf(&linebuf[70],"%f", &tcharge))
		 				charge[k] = tcharge;
	 			else charge[k] = 0.00;
				k++;
			}
		}
	}
	makeatomtype_prot(atype, k, atomtype);
	return k;
}


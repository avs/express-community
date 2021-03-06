/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetdmol_xyz.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
        Option: dmol_xyz
        Read a DMOL COORD file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

    6      0.023686931    0.012495107   -0.007829269
    6      2.843215911    0.013994655    0.001164353
    6      2.827832482    2.478733396    0.013675318
    6      0.051153183    2.485086901   -0.037928336
    6      0.725787474    1.250353341   -0.001539732
    6      0.739853552   -1.167748028   -0.020982015
    6      2.142159249   -1.166479489   -0.026115009
    6      2.150693385    1.241655457    0.012056303
    6      2.145387402    3.671603031   -0.007541485
    6      0.739696008    3.675868114   -0.039536914
    7     -1.363420688    0.016179865   -0.062974898
    1      3.940376021    0.028288071    0.006272796
    1      3.926297501    2.464521877    0.018642964
    1     -1.044818066    2.489473673   -0.107059570
    1      0.187845382   -2.118852568   -0.024323365
    1      2.674422682   -2.126356814   -0.043517514
    1      2.694544726    4.624088114   -0.017596128
    1      0.193117454    4.628430336   -0.084691499
    1     -1.800003163    0.717067320    0.547694977
    1     -1.769329947   -0.907255040    0.125147620
*/

int fgetdmol_xyz(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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

	int natom;

	k=0;
	while(fgets(linebuf, sizeof linebuf, f) != NULL
			&& k < maxnatoms){
		sscanf(&linebuf[0]," %d %f %f %f", &natom,
			&dx, &dy, &dz) ;
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

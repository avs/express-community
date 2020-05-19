/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetgamessC the GAMESS CUBE option.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/*
        Option: cgamess
        Read in GAMESS xyz format from the F33 file output from the new
		code incorporating a CUBE like option for GAMESS, putting the
        coords into apos, the char string names into atype, and the
        charges in charge.
        Here is some sample file:
           18          64
           21          21          21
   -7.108052604910962       7.108052604910965      -4.730225315175534
    4.730225315175533      -9.884127734009613       9.884127734009610
     N    7.0  -1.2734569   0.1851574  -2.9104685
     N    7.0  -1.0003999   0.4082293  -0.3712765
     C    6.0   0.2845821  -0.2118040  -4.8820604
     N    7.0  -1.0215597  -0.4149750  -6.8842239
     N    7.0  -3.4517689  -0.1577011  -6.1484679
     N    7.0  -3.6157759   0.1944229  -3.8306807
     N    7.0   1.0117295  -0.3582334   0.3663083
     N    7.0   1.2799096  -0.1360275   2.9077008
     C    6.0  -0.2891865   0.2103004   4.8794772
     N    7.0   1.0103832   0.4367422   6.8840316
     N    7.0   3.4458780   0.2464911   6.1491131
     N    7.0   3.6201898  -0.0904430   3.8289971
     N    7.0   3.0177616  -0.1332769  -4.8246523
     N    7.0  -3.0186930   0.0558846   4.8188180
     O    8.0   4.0841137  -1.7999399  -5.8563814
     O    8.0   3.9002899   1.6549841  -3.7856064
     O    8.0  -4.1319915   1.6605107   5.8976052
     O    8.0  -3.8509178  -1.7266012   3.7285570

	The first line are the number of atoms and the number of data
		values (MO's and properties) ate each grid point.
	The second line are the grid dimensions.
	The third line are
	The forth line are
	The next set of lines contain the atomic positions and types.
*/

int fgetCgamess(f, maxnatoms, apos, atype, aamin, charge, hydr, atomtype)
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
int dummy, dummy1, dummy2, dummy3;
float Dummy1, Dummy2, Dummy3, Dummy4;
int filenum;

/*
	Skip the first four lines
*/
	/*printf("Entering fgetCgamess\n");*/
	fscanf(f, "%d %d\n", &filenum, &dummy2);
	fscanf(f, "%d %d %d\n", &dummy1, &dummy2, &dummy3);
	fscanf(f, "%f %f %f\n", &Dummy1, &Dummy2, &Dummy3);
	fscanf(f, "%f %f %f\n", &Dummy1, &Dummy2, &Dummy3);
	/*printf("Read the first 4 lines, filenum = %d\n", filenum);*/
/*
	Begin read, k counts the number of atoms
*/
	k=0;
	while(k < filenum){
		fscanf(f, "%s %f %f %f %f", &dum1, &catom, &dx, &dy, &dz);
		/*
		printf("In fgetCgamess, line %d read with values %s %f %f %f %f\n",
			k, dum1, catom, dx, dy, dz);
		*/
		if(hydr == 0){
			apos[k][0] = dx ;
			apos[k][1] = dy ;
			apos[k][2] = dz ;
			strcpy(atype[k],dum1);
			charge[k] = catom;
			k++;
		} else if( !(dum1[0]=='H' || dum1[0]=='h')){
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

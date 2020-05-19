/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmm3.
*/

#include "stm3.h"

/*
  Option: mm3
        Read a MM3 coordinte file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

ETHANOL                                                     0   9 0  0 0  0 10.00   1      0.0000000         6    0         0    0    0    0    1         1    0    1    2    3
    1    4    1    5    1    6    2    7    2    8    3    9
   1.27819  -0.24461   0.00000 C  1(  1)
  -0.01083   0.57083   0.00000 C  1(  2)
  -1.12978  -0.32457   0.00000 O  6(  3)
   2.17404   0.41427   0.00000 H  5(  4)
   1.34833  -0.89848   0.89680 H  5(  5)
   1.34833  -0.89848  -0.89680 H  5(  6)
  -0.05313   1.22856   0.90370 H  5(  7)
  -0.05313   1.22856  -0.90370 H  5(  8)
  -1.92470   0.19229   0.00000 H 21(  9)
*/

int read_mm3(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	char dum1[255];

	k=1;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL && k <= 3 )
	{
		k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		if(sscanf(&linebuf[0]," %f %f %f", &dx, &dy, &dz) != 3) continue;
		sscanf(&linebuf[30]," %s", dum1);
		if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strncpy(a.name, dum1, 2);
			a.c = 0.0;
			
			a.atom_z = atom_to_z(a.name);
			 
			// no residue name provided
			a.res_id = 0;
			a.res_name[0] = '\0';
			
			atoms.push_back(a);
			k++;
		}
	}
	return k;
}

/*
  Created October 16, 1997 by Theresa Windus and Ken Flurchick.
  Taken from coord_to_geom.c to create a reusable library.
  The original code was written by Lee Bartolotti and Ken Flurchick.

  Basic library source for fgetchem_3d.
*/

#include "stm3.h"

/*
  Option: chem_3d
        Read a CHEM-3D file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

540
C    1   -6.345535   -0.819672    8.125168    1    2    6  111
C    2   -5.099655   -1.005203    8.798187    1    1    3  173
C    3   -4.514252   -2.309235    8.871323    1    2    4  216
C    4   -5.176743   -3.422500    8.273132    1    3    5  221
C    5   -6.482925   -3.259781    7.752930    1    4    6    7
*/

int read_chem_3d(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	char dum1[255];
	int di;

	fgets(linebuf, sizeof linebuf, fp);

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp)!= NULL)
	{
		/* sanity check */
		if(sscanf(&linebuf[5], "%d", &di) != 1) continue;
	
		sscanf(&linebuf[2]," %2s",dum1);
		if(sscanf(&linebuf[9], " %f %f %f", &dx, &dy, &dz) != 3) continue;

		if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strcpy(a.name, dum1);
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

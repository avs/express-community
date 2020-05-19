/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmol.
*/

#include "stm3.h"

/*
  Option: mol
        Read a MOL file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

                                         1.000   1.000   1.000
                       90.000  90.000  90.000
  61

   1 O      -1.99050  -3.82015  -0.25595   21  41   0   0   0   0
   2 O      -3.53075   0.44747   0.16308   22  42   0   0   0   0
   3 O       0.82619  -3.51817   0.62851   23  43   0   0   0   0
   4 O      -2.98560   1.99548  -2.14861   24  44   0   0   0   0
   5 O      -0.54806   2.46364  -0.81459   25  45   0   0   0   0
   6 O       1.64986   3.31839  -2.11407   26  46   0   0   0   0
*/

int read_mol(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	char dum1[255];
	int di;

	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		/* sanity check */
		if(sscanf(&linebuf[0], " %d", &di) != 1) continue;

		sscanf(&linebuf[5]," %2s", dum1);
		if(sscanf(&linebuf[9]," %f %f %f", &dx, &dy, &dz) != 3) continue;
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

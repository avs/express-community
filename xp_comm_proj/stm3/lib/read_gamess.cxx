/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetgamess.
*/

#include "stm3.h"

/*
        Option: pgamess
        Read in GAMESS xyz format from the PERL extraction, putting the
        coords into apos, the char string names into atype, and the
        charges in charge.
        Here is some sample input:

  1  C           6.0     0.0000000000        0.0000000000       -0.1699098334
  2  H           1.0    -1.6872904675        0.0000000000        1.0115436711
  3  H           1.0     1.6872904675        0.0000000000        1.0115436711
*/

int read_gamess(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */

	float dx;
	float dy;
	float dz;
	char dum1[255];
	float catom;
	int dummy;

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		if(sscanf(&linebuf[0], " %d %s %f %f %f %f", &dummy, dum1, &catom, &dx, &dy, &dz) != 6) continue;
		if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strcpy(a.name, dum1);
			a.c = catom;
			
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

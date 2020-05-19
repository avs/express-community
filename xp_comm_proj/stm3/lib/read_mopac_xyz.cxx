/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmopac.
*/

#include "stm3.h"

/*
  Option: mopac_xyz
        Read a MOPAC xyz coordinte file, putting the coords into apos and
        the char string names into atype. Return number of atoms.
        Stop reading on End-of-file,  blank line or blank atom.
        Here is some sample input:

AM1 XYZ T=02.00D GNORM=0.01 MODE=1 SCFCRT=1.D-7 LET
/users/natalia/chris12.dat
chris12
   O    0.0000000000000  1    0.0000000000000  1    0.0000000000000  1
   N    1.1593000000000  1    0.0000000000000  1    0.0000000000000  1
   C    1.8677000000000  1    1.2712000000000  1    0.0000000000000  1
   C    3.2845000000000  1    1.2280000000000  1    0.0040000000000  1
*/

int read_mopac_xyz(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	int dum;
	float dx;
	float dy;
	float dz;
	char dum1[255];

	k=1;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL && k <= 2)
	{
		if(strrchr(linebuf, '+') == NULL ) k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		if(linebuf[3] == '0' || linebuf[0]  == '\n' || linebuf[3] == ' ') break;

		if(sscanf(&linebuf[4], " %f %d %f %d %f", &dx, &dum, &dy, &dum, &dz) != 5) continue;
		sscanf(&linebuf[0], " %s", dum1);

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

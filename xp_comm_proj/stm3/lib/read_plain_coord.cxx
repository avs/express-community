/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetcoord.
*/

#include "stm3.h"

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
int read_plain_coord(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */

	float dx;
	float dy;
	float dz;

	int iatom;

	k = 0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		if(sscanf(&linebuf[0], "%d %f %f %f", &iatom, &dx, &dy, &dz) != 4) continue;
		if(read_hydrogens == 1 || !(iatom == 1))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strcpy(a.name, atom_properties[iatom].atom_name);
			a.c = 0.0;
			
			a.atom_z = iatom;
			 
			// no residue name provided
			a.res_id = 0;
			a.res_name[0] = '\0';
			
			atoms.push_back(a);
			k++;
		}
	}
	
	return k;
}

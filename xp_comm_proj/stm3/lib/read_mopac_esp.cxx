/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetespcoord.
*/

#include "stm3.h"

/*
  Option: mopac_esp
        Read a CONVEX mopac ESP file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

  protonated complex structure 1
  backbone anchored
    PM3 XYZ MMOK SETUP=KEYS AVSWAVE 1SCF
    GNORM=0.001 SCFCRT=1.0D-10 NOLOG PRECISE NOANCI NODIIS
  130  -23.124657  -22.388311  -21.155438
  322    0.000000    0.000000    0.755905
   -1    0.000000    0.755905    0.000000
  322    0.755905    0.000000    0.000000
          6    0.6000000E+01   -0.7892052E+01   -0.1296199E+02    0.2722263E+01
          1    0.1000000E+01   -0.5812176E+01   -0.1296199E+02    0.2722263E+01
          6    0.6000000E+01   -0.8921321E+01   -0.1031720E+02    0.2722263E+01
*/

int read_mopac_esp(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dummy;
	float dx;
	float dy;
	float dz;

	int natom;

	k=1;
	while(k <= 8)
	{
		fgets(linebuf, sizeof linebuf, fp);
		k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL
	  		&& 0!=strncmp("AL", linebuf, 2))
	{
		if(sscanf(&linebuf[0]," %d %f %f %f %f", &natom, &dummy, &dx, &dy, &dz) != 5) continue;
		if(read_hydrogens == 1 || (natom != 1 ))
		{
        	Atom a;

        	a.x = dx;
        	a.y = dy;
        	a.z = dz;
			strcpy(a.name, atom_properties[natom].atom_name);
			a.c = 0.0;
			
			a.atom_z = natom;
			 
			// no residue name provided
			a.res_id = 0;
			a.res_name[0] = '\0';
			
			atoms.push_back(a);
			k++;
		}
	}

	return k;
}

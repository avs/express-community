/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetdcar.
*/

#include "stm3.h"


/*
  Option: dcar
        Read a dmol DCAR file putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line.
        Here is some sample input:

!BIOSYM archive 3
PBC=OFF
Dialog output of CAR file
!DATE Thu Jan 29 23:17:53 1998
C1       2.173573433    0.961526260   -1.297810015 dmol 1      ???     C   0.056
N2       0.949602589    1.044543878   -1.877666392 dmol 1      ???     N  -0.039
C3       0.834991771    1.051056677   -3.212643498 dmol 1      ???     C   0.036
C4       1.943454333    0.959837557   -4.033515517 dmol 1      ???     C  -0.020
C5       3.204077978    0.876387343   -3.458677357 dmol 1      ???     C  -0.011
C6       3.317632617    0.880345325   -2.079999151 dmol 1      ???     C  -0.029
H7      -0.203321155    1.111136731   -3.577315730 dmol 1      ???     H   0.053
H8       1.809054380    0.959477872   -5.120608244 dmol 1      ???     H   0.089
H9       4.101509922    0.803186132   -4.084409124 dmol 1      ???     H   0.086
H10      4.302412308    0.814418233   -1.604968613 dmol 1      ???     H   0.075
.
.
end
end

THE FORMAT DOES NOT CORRESPOND TO THE DISTRIBUTED FILE DCAR.DAT !!!	
*/

int read_dcar(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	char dum1[255];

	k=0;
	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);
	fgets(linebuf, sizeof linebuf, fp);
	
	while(fgets(linebuf, sizeof linebuf, fp) != NULL
			&& 0!=strncmp("END", linebuf,3)
			&& 0!=strncmp("end", linebuf,3))
	{
		/* if(sscanf(&linebuf[0]," %*s %f %f %f %*s %*s %*s %s", &dx, &dy, &dz, dum1) != 4) continue; (SEE ABOVE) */
		if(sscanf(&linebuf[0]," %*s %f %f %f %*s %*s %s", &dx, &dy, &dz, dum1) != 4) continue;

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

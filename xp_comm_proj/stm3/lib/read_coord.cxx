

#include "stm3.h"


int read_coord(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<float> &offsets)
{
	char linebuf[200];
	register int k; /* counter of atoms */

	float dx, dy, dz, offx, offy, offz;
	int i, j, nbvals;
	char dum1[32];

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		nbvals = sscanf(&linebuf[0],"%s %f %f %f %f %f %f", dum1, &dx, &dy, &dz, &offx, &offy, &offz);
		if((nbvals == 4) || (nbvals == 7))
		{
			if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
			{
        		Atom a;

        		a.x = dx;
        		a.y = dy;
        		a.z = dz;
				
				j=0; i=strspn(dum1, " ");
				while(dum1[i] != ' ')
					a.name[j++] = dum1[i++];
				a.name[j++] = '\0';
				
				a.c = 0.0;
			
				a.atom_z = atom_to_z(a.name);
			 
				// no residue name provided
				a.res_id = 0;
				a.res_name[0] = '\0';
				
				atoms.push_back(a);
			
				if(nbvals == 7)
				{
					offsets.push_back(offx);
					offsets.push_back(offy);
					offsets.push_back(offz);
				}
				
				k++;
			}
		}
	}

	return k;
}

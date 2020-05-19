/*
        Created Mon Oct 01 09:46:37 2001

*/

#include "stm3.h"

/*
           26

C         -3.0946     -0.6861      0.5896
C         -2.9141      0.6983      0.3306
C         -0.2263      0.7632      0.4228
C         -0.6015     -1.6904      0.7081
C          0.0077     -0.4073      1.4622
C         -0.2035     -1.4270     -0.7728
C         -0.1671      0.0808     -0.9033
H         -3.0936     -1.2197      1.6119
H         -3.5981     -1.2588     -0.3719
H         -3.1991      1.1007     -0.7895
H         -2.9907      1.6390      1.0327
H         -0.2736      2.0163      0.5987
H         -0.0719     -2.6944      0.8875
H          1.1579     -0.6427      1.7127
H         -0.3635     -0.1238      2.4947
H         -0.6166     -1.9986     -1.6644
H         -0.4282      0.5643     -1.9471


  The first line are the number of atoms 
  The second line is blank (MV: it is ignored)
  The next set of lines contain the atomic positions and types.
  For Kino files the file pointer is positioned at the start of the first atom line
*/
//
// MV 09-apr-2003
//	added a fifth column read that carries a parameter (not necessarely atom charge)
//

int read_kino(FILE *fp, int read_hydrogens, int num_atoms, vector<Atom> &atoms)
{
	register int k; /* counter of atoms */

	double dx, dy, dz, c;
	char dum1[32];
	char linebuf[130];
	int t, nval;

	t=0;
	for(k = 0; k < num_atoms; ++k)
	{
	    if(fgets(linebuf, 128, fp) == NULL) return 0; 
		if((nval=sscanf(linebuf, "%s %lf %lf %lf %lf", dum1, &dx, &dy, &dz, &c)) < 4) return 0;
		if(nval < 5) c = 0.0;
		if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
		{
			Atom a;

			a.x = dx;
			a.y = dy;
			a.z = dz;
			strcpy(a.name, dum1);
			a.c = c;

			a.atom_z = atom_to_z(a.name);

			// no residue name provided
			a.res_id = 0;
			a.res_name[0] = '\0';

			atoms.push_back(a);
			t++;
		}
	}
	return t;
}

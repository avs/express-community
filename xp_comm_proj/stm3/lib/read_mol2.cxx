/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for fgetmol2.
*/

#include "stm3.h"

/*
  Option: mol2
        Read a TRIPOS MOL2 file, putting the coords into apos and the char
        string names into atype. Return number of atoms.
        Stop reading on End-of-file, or 'END' line, or line with just a ".".
        Here is some sample input:

#       Name:                   ACETYLCHOLINESTERASE-EDROPHONIUM COMPLEX (ACHE-EDR)
#       Creating user name:     maria
#       Creation time:          Wed Dec  1 12:50:27 1993

#       Modifying user name:    maria
#       Modification time:      Wed Dec  1 12:50:27 1993

@<TRIPOS>MOLECULE
ACETYLCHOLINESTERASE-EDROPHONIUM COMPLEX (ACHE-EDR)
 7652  8211   529     0     9
PROTEIN
NO_CHARGES


@<TRIPOS>DICT
PROTEIN PROTEIN
@<TRIPOS>ATOM
      1 N         -12.6074  -23.1989  -24.7946 N.am      1 SER4       0.0000 BACKBONE|DICT|DIRECT
      2 CA        -11.6823  -23.4022  -25.9155 C.3       1 SER4       0.0000 BACKBONE|DICT|DIRECT
      3 CB        -12.3635  -23.2206  -27.2850 C.3       1 SER4       0.0000 DICT
      4 OG        -13.4899  -24.1115  -27.1791 O.3       1 SER4       0.0000 DICT
      5 C         -10.5679  -22.4829  -25.5506 C.2       1 SER4       0.0000 BACKBONE|DICT|DIRECT
      6 O         -10.8245  -21.7067  -24.6525 O.2       1 SER4       0.0000 BACKBONE|DICT|DIRECT
      7 N          -9.4054  -22.5321  -26.1952 N.am      2 GLU5       0.0000 BACKBONE|DICT|DIRECT
      8 CA         -8.3110  -21.6834  -25.7109 C.3       2 GLU5       0.0000 BACKBONE|DICT|DIRECT
      9 CB         -7.2229  -21.5776  -26.7863 C.3       2 GLU5       0.0000 DICT
     10 CG         -5.8910  -20.9530  -26.3251 C.3       2 GLU5       0.0000 DICT
     11 CD         -4.9174  -21.9241  -25.6819 C.2       2 GLU5       0.0000 DICT
     12 OE1        -5.1290  -22.2838  -24.5283 O.co2     2 GLU5       0.0000 DICT
.
.
.
@<TRIPOS>BOND
     1    2   13 1    BACKBONE|DICT
     2    2    3 1    DICT
     3    2    1 1    BACKBONE|DICT
.
.
.
@<TRIPOS>SUBSTRUCTURE
     1 TRP84       2 RESIDUE           1 A     TRP     0 ROOT
     2 SER200     16 RESIDUE           1 A     SER     0 ROOT
     3 PHE330     22 RESIDUE           1 A     PHE     0 ROOT
     4 HIS440     33 RESIDUE           1 A     HIS     0 ROOT
     5 THA999     42 GROUP             0 A     ****    0 ROOT
@<TRIPOS>SET
UNK_ATOMS       STATIC     ATOMS    AMSOM    ****
15 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56
ATOM$BY_TYPE    STATIC     ATOMS    COLORGROUP SYSTEM
15 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56
ATOM$MAGENTA    STATIC     ATOMS    COLORGROUP SYSTEM
41 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 \
 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 \
 38 39 40 41

*/

int read_mol2(FILE *fp, int read_hydrogens, vector<Atom> &atoms)
{
	char linebuf[200];
	register int k; /* counter of atoms */
	float dx;
	float dy;
	float dz;
	float tcharge;
	char dum1[255];
	char dum2[255];
	int rid;

	/*  while(fgets(linebuf, sizeof linebuf, fp)!=NULL &&
	0!=strcmp("<TRIPOS>ATOM", linebuf) ) { */

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		if(!strncmp(linebuf, "@<TRIPOS>ATOM", 13)) break;
		k++;
	}

	k=0;
	while(fgets(linebuf, sizeof linebuf, fp) != NULL)
	{
		sscanf(&linebuf[47]," %s", dum1);
		sscanf(&linebuf[14]," %f %f %f", &dx, &dy, &dz);
		sscanf(&linebuf[55]," %d", &rid);
		sscanf(&linebuf[59]," %s", dum2);

		if(!strncmp(linebuf, "@<TRIPOS>", 9)) break;

		if(read_hydrogens == 1 || !(dum1[0]=='H' || dum1[0]=='h'))
		{
            Atom a;

            a.x = dx;
            a.y = dy;
            a.z = dz;
			strncpy(a.name, dum1, 2);
     		a.c = (1==sscanf(&linebuf[70],"%f", &tcharge)) ? tcharge : 0.0;
			
			a.atom_z = atom_to_z(a.name);
			 
			// no residue id provided
			a.res_id = rid;
      		strncpy(a.res_name, dum2, 3);
			
			atoms.push_back(a);
			k++;
		}
	}

	return k;
}

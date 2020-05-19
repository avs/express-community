/*
	Coord To Geom for AVS/EXPRESS

        A periodic table of the elements has been added that
        includes atom names, atomic number, van der Waals radius
        and color.  The table of van der Waals radii have been
        taken from Emsley and Huheey.

        The types of files that can now be read are: pdb, DMol,
        dcar, Gaussian cube, chem-3d, dmol_xyz, mol, tripos mol2,
        mm3, pgamess, and coord file types.
*/


/*	dcar
	Read a dmol DCAR file putting the coords into apos and the char
	string names into atype. Return number of atoms.
	Stop reading on End-of-file, or 'END' line.
 	Here is some sample input:

!BIOSYM archive 2
PBC=OFF
DMol output .car file; Hirshfeld partial charges
!date
C1       0.023686931    0.012495107   -0.007829269 dmol    1    C    0.0354    1
C2       2.843215911    0.013994655    0.001164353 dmol    1    C   -0.0723    2
C3       2.827832482    2.478733396    0.013675318 dmol    1    C   -0.0518    3
C4       0.051153183    2.485086901   -0.037928336 dmol    1    C   -0.0582    4
C5       0.725787474    1.250353341   -0.001539732 dmol    1    C   -0.0162    5
C6       0.739853552   -1.167748028   -0.020982015 dmol    1    C   -0.0806    6
C7       2.142159249   -1.166479489   -0.026115009 dmol    1    C   -0.0591    7
C8       2.150693385    1.241655457    0.012056303 dmol    1    C   -0.0055    8
C9       2.145387402    3.671603031   -0.007541485 dmol    1    C   -0.0574    9
C10      0.739696008    3.675868114   -0.039536914 dmol    1    C   -0.0587   10
N1      -1.363420688    0.016179865   -0.062974898 dmol    1    N   -0.1832   11
H1       3.940376021    0.028288071    0.006272796 dmol    1    H    0.0493   12
.
.
.
end
end
*/


/*	pdb
 	Read a Brookhaven file putting the coords into apos and the char
 	string names into atype. Return number of atoms.  M Pique
 	Stop reading on End-of-file, or 'END' line, or line with just a ".".
  	Here is some sample input:

ATOM     19  HN  THR     2      15.386  10.901   4.600
ATOM     20  HOG THR     2      14.161   9.481   4.420
ATOM     21  N   CYS     3      13.507  11.238   8.398
ATOM     22  CA  CYS     3      13.659  10.715   9.763
ATOM     23  C   CYS     3      12.265  10.443  10.307
HETATM   99  FE  XXX     4      33.265  10.443  10.307
ATOM         OT  OME    52     -16.009  30.871 -13.037                -0.543
HETATM       CU+2CU    152     -20.577   2.601 -14.587                 2.000
HETATM       ZN+2ZN    152     -17.911   1.372 -19.974                 2.000
END

These charge values
are non-standard
and are not genuine
PDB. They are optional.
*/


/*	dmol_xyz
	Read a DMOL COORD file, putting the coords into apos and the char
 	string names into atype. Return number of atoms.
 	Stop reading on End-of-file, or 'END' line, or line with just a ".".
 	Here is some sample input:

    6      0.023686931    0.012495107   -0.007829269
    6      2.843215911    0.013994655    0.001164353
    6      2.827832482    2.478733396    0.013675318
    6      0.051153183    2.485086901   -0.037928336
    6      0.725787474    1.250353341   -0.001539732
    6      0.739853552   -1.167748028   -0.020982015
    6      2.142159249   -1.166479489   -0.026115009
    6      2.150693385    1.241655457    0.012056303
    6      2.145387402    3.671603031   -0.007541485
    6      0.739696008    3.675868114   -0.039536914
    7     -1.363420688    0.016179865   -0.062974898
    1      3.940376021    0.028288071    0.006272796
    1      3.926297501    2.464521877    0.018642964
    1     -1.044818066    2.489473673   -0.107059570
    1      0.187845382   -2.118852568   -0.024323365
    1      2.674422682   -2.126356814   -0.043517514
    1      2.694544726    4.624088114   -0.017596128
    1      0.193117454    4.628430336   -0.084691499
    1     -1.800003163    0.717067320    0.547694977
    1     -1.769329947   -0.907255040    0.125147620
*/


/*	coord
	Read a general xyz format, putting the coords into apos, the char
	string names into atype, and the charges in charge.
	Here is some sample input:

 SILICON    14.0     2.9639742677       -2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677        2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677       -2.9639742677        2.9639742677
 SILICON    14.0    -2.9639742677        2.9639742677        2.9639742677
 SILICON    14.0     2.9639742677        2.9639742677       -2.9639742677
 SILICON    14.0    -2.9639742677       -2.9639742677       -2.9639742677
 SILICON    14.0     2.9639742677       -2.9639742677        2.9639742677
 SILICON    14.0     2.9639742677        2.9639742677        2.9639742677
 OXYGEN      8.0      .0000000000       -3.5306894327       -3.5306894327
 OXYGEN      8.0      .0000000000        3.5306894327       -3.5306894327
 OXYGEN      8.0      .0000000000       -3.5306894327        3.5306894327
 OXYGEN      8.0     3.5306894327         .0000000000        3.5306894327
 OXYGEN      8.0    -3.5306894327         .0000000000       -3.5306894327
 OXYGEN      8.0    -3.5306894327         .0000000000        3.5306894327
 OXYGEN      8.0     3.5306894327         .0000000000       -3.5306894327
 OXYGEN      8.0     3.5306894327        3.5306894327         .0000000000
 OXYGEN      8.0    -3.5306894327       -3.5306894327         .0000000000
 OXYGEN      8.0     3.5306894327       -3.5306894327         .0000000000
 OXYGEN      8.0    -3.5306894327        3.5306894327         .0000000000
 OXYGEN      8.0      .0000000000        3.5306894327        3.5306894327
 HYDROGEN    1.0     4.5627046338       -4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338        4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338       -4.5627046338        4.5627046338
 HYDROGEN    1.0    -4.5627046338        4.5627046338        4.5627046338
 HYDROGEN    1.0     4.5627046338        4.5627046338       -4.5627046338
 HYDROGEN    1.0    -4.5627046338       -4.5627046338       -4.5627046338
 HYDROGEN    1.0     4.5627046338       -4.5627046338        4.5627046338
 HYDROGEN    1.0     4.5627046338        4.5627046338        4.5627046338
*/


/*	pgamess
        Read in GAMESS xyz format from the PERL extraction, putting the
	coords into apos, the char string names into atype, and the
	charges in charge.
        Here is some sample input:

  1  C           6.0     0.0000000000        0.0000000000       -0.1699098334
  2  H           1.0    -1.6872904675        0.0000000000        1.0115436711
  3  H           1.0     1.6872904675        0.0000000000        1.0115436711
*/


/*	mopac_esp
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


/*	chem_3d
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


/*	mol
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

/*	mol2
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

/*	mm3
  	Read a MM3 coordinte file, putting the coords into apos and the char
  	string names into atype. Return number of atoms.
  	Stop reading on End-of-file, or 'END' line, or line with just a ".".
  	Here is some sample input:

ETHANOL                                                     0   9 0  0 0  0 10.0
0   1      0.0000000         6    0         0    0    0    0    1         1    0
    1    2    3
    1    4    1    5    1    6    2    7    2    8    3    9
   1.27819  -0.24461   0.00000 C  1(  1)
  -0.01083   0.57083   0.00000 C  1(  2)
  -1.12978  -0.32457   0.00000 O  6(  3)
   2.17404   0.41427   0.00000 H  5(  4)
   1.34833  -0.89848   0.89680 H  5(  5)
   1.34833  -0.89848  -0.89680 H  5(  6)
  -0.05313   1.22856   0.90370 H  5(  7)
  -0.05313   1.22856  -0.90370 H  5(  8)
  -1.92470   0.19229   0.00000 H 21(  9)
*/


/*	mopac_xyz
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

/*	cube
	Read a CUBE file from Gaussian, putting the coords into apos
	and the char string names into atype. Return number of atoms.
	Stop after coordinate read.
	Here is some sample input:

Gaussian Test Job 318: Potential cube
SCF Total Density
    3   -9.393328   -9.393328   -8.985941
   10    2.087406    0.000000    0.000000
   10    0.000000    2.087406    0.000000
   10    0.000000    0.000000    2.087406
    8    8.000000    0.000000    0.000000    0.215580
    1    1.000000    0.000000    1.435626   -0.862319
    1    1.000000    0.000000   -1.435626   -0.862319
  1.88175E-03  1.79419E-03  1.51013E-03  9.93986E-04  2.86401E-04 -4.81750E-04
 -1.14728E-03 -1.60245E-03 -1.83108E-03 -1.87998E-03
*/

/*
  Read a CUBE like file from GAMESS, putting the coords into apos
  and the char string names into atype. Return number of atoms.
  Stop after coordinate read.
  Here is some sample output:

 #of_atoms	#of_vectors
 #of_x_pts	#of_y_pts	#of_z_pts
 xmin xmax	ymin ymax	zmin zmax
 coordinates in GAMESS_format
 vec1 vec2 vec3 ... vec_max_num
 ...
 ...
*/

#define BALL            0
#define CPK             0
#define BALL_AND_STICK  1
#define STICK           2
#define COLORED_STICK   3
#define COLORED_PROTEIN 4
#define COLORED_RESIDUE 4

#define COORD           0
#define DCAR            1
#define DMOL_XYZ        2
#define PDB             3
#define ESP             4
#define CHEM_3D         5
#define MOL             6
#define MOL2            7
#define MM3             8
#define MOPAC           9
#define CUBE           10
#define PGAMESS        11
#define GAMESSCUBE     12
#define PLAINCOORD     13

#define ANGSTROM        0
#define BOHR            1

#define SHOW            0
#define HIDE            1

#define NATOM_TYPE      104

#define MAX_ATOMS       10000
#define MAX_BONDS       10000
#define BONDLENGTH      1.85

#define STICKSANDSPHERES_RADFACTOR   0.2   /* spheres for ball-and-stick */

/*
	table of Van der Waals radii per each of (NATOM_TYPE) at types
	Note: the order is keyed to the values set in the host "makeatomtype"
*/

static float radius[] = {
	1.20, 1.22, 1.75,  /* Be and Sc are a guestimates. */
	1.50, 2.08, 1.85,
	1.54, 1.40, 1.35,
	1.60, 2.31, 1.70,
	2.05, 2.00, 1.90,
	1.85, 1.81, 1.91,
	2.31, 1.74, 1.80,
	1.60, 1.50, 1.40,  /* Ti-Cu and Ge are guestimates. */
	1.40, 1.40, 1.40,
	1.60, 1.40, 1.40,
	1.90, 1.80, 2.00,
	2.00, 1.95, 1.98,
	2.44, 2.40, 2.10,  /* Sr-Rh and Ba and La are guestimates. */
	2.00, 1.80, 1.80,
	1.80, 1.80, 1.80,
	1.60, 1.70, 1.60,
	1.90, 2.20, 2.20,
	2.20, 2.15, 2.20,
	2.62, 2.30, 2.30,
	2.30, 2.30, 2.30,  /* All of these are guestimates. */
	2.30, 2.30, 2.40,
	2.30, 2.30, 2.30,
	2.30, 2.30, 2.30,
	2.40, 2.50, 2.30,
	2.30, 2.30, 2.30,  /* All but Pt and Bi are guestimates. */
	2.30, 2.30, 2.40,
	2.30, 2.40, 2.50,
	2.50, 2.40, 2.40,
	2.40, 2.40, 2.90,
	2.60, 2.30, 2.30,   /* These are all guestimates. */
	2.30, 2.30, 2.30,
	2.30, 2.30, 2.30,
	2.30, 2.30, 2.30,
	2.30, 2.30, 2.30,
	2.30, 1.50
      			} ;

static float atomcolors[][3] = {
	{1.0, 1.0, 1.0}, {0.5, 0.0, 0.5}, {1.0, 0.0, 1.0},
	{0.5, 0.5, 0.5}, {0.5, 0.0, 0.5}, {0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 1.0},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.7, 0.6, 0.4},
	{0.5, 0.5, 0.5}, {0.2, 0.5, 0.9}, {1.0, 0.5, 0.5},
	{1.0, 1.0, 0.0}, {1.0, 0.5, 0.5}, {1.0, 1.0, 0.5},
	{0.5, 0.5, 0.5}, {0.2, 0.8, 0.8}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.8, 0.0, 1.0}, {1.0, 0.0, 1.0},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.9, 0.4, 0.2}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 1.0, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.4, 0.2, 0.8}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.2, 0.5, 0.2}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}
        			};

static char *aminnames[] = { "THR", "CYS", "PRO", "SER",
                             "ILE", "VAL", "ALA", "ARG",
                             "ASN", "PHE", "LEU", "GLY",
                             "TYR", "ASP", "MET", "GLN",
                             "GLU", "HOH", "TRP", "LYS",
                             "HIS", "SO4", "BEN", "AGLN",
                             "BGLN","AGLU","BGLU","AVAL",
                             "BVAL","AARG", "BARG","ALYS",
                             "BLYS", "ZN1", "ZN2", "MPD",
                             "NONE"};
static float amincolors[][3] = { {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,0.0,1.0},
                                 {1.0,0.0,0.0}, {1.0,1.0,0.5}, {1.0,1.0,0.5},
                                 {1.0,1.0,0.5}, {0.0,1.0,1.0}, {1.0,0.5,.75},
                                 {0.5,1.0,0.5}, {1.0,1.0,0.5}, {0.0,0.0,1.0},
                                 {0.5,1.0,0.5}, {1.0,0.5,.75}, {1.0,1.0,0.0},
                                 {1.0,0.5,.75}, {1.0,0.5,.75}, {1.0,1.0,1.0},
                                 {0.5,1.0,0.5}, {0.0,1.0,1.0}, {0.0,1.0,1.0},
                                 {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                 {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                 {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                 {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                 {1.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,1.0},
                                 {1.0,1.0,1.0}, {1.0,1.0,1.0}};

static   char  *atom_names[104] = { "H ","HE","LI","BE","B ","C ","N ","O ",
                                    "F ","NE","NA", "MG","AL","SI","P ","S ",
                                    "CL","AR","K ","CA","SC","TI","V ", "CR",
                                    "MN","FE","CO","NI","CU","ZN","GA","GE",
                                    "AS","SE","BR", "KR","RB","SR","Y ","ZR",
                                    "NB","MO","TC","RU","RH","PD","AG", "CD",
                                    "IN","SN","SB","TE","I ","XE","CS","BA",
                                    "LA","CE","PR", "ND","PM","SM","EU","GD",
                                    "TB","DY","HO","ER","TM","YB","LU", "HF",
                                    "TA","W ","RE","OS","IR","PT","AU","HG",
                                    "TL","PB","BI", "PO","AT","RN","FR","RA",
                                    "AC","TH","PA","U ","NP","PU","AM", "CM",
                                    "BK","CF","ES","FM","MD","NO","LR","  "
                                  } ;

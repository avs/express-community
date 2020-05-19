#include <stdio.h>
#include <vector>
using namespace std;

// element info
typedef struct {
	char  *atom_name;		// two letters (if one, padded with blank) element symbol
	float  coval;			// covalent radius
	int    nbonds;			// maximum number of bonds
	float  rvdw;			// Van der Waals radius
	float  color[3];		// color to be used to display this element
	float  mass;			// element atomic mass
} AtomProperties;

extern AtomProperties atom_properties[];	// element table

#define MAX_ATOM_NAME_LEN	20
#define MAX_RES_NAME_LEN	20
#define BOHR_TO_ANGSTROM	0.529177F

typedef struct {
	float x;
	float y;
	float z;
	float c;
	int atom_z;
	int res_id;
	char name[MAX_ATOM_NAME_LEN];
	char res_name[MAX_RES_NAME_LEN];
} Atom;

typedef struct {
	int from;
	int to;
	int type;
} Bond;

class UnitCell {
public:
	float side_vectors[3][3];
	float angles[3];
	float origin[3];
	
	UnitCell() {
		side_vectors[0][0] = side_vectors[1][1] = side_vectors[2][2] = 0.0F; // changed from 1 to signal unit cell data not available
		side_vectors[0][1] = side_vectors[0][2] = 0.0F;
		side_vectors[1][0] = side_vectors[1][2] = 0.0F;
		side_vectors[2][0] = side_vectors[2][1] = 0.0F;
		angles[0] = angles[1] = angles[2] = 90.0F;
		origin[0] = origin[1] = origin[2] =  0.0F;
	};
};

//
//	Support routines
//
extern int  atom_to_z(char *name, int forced_to = 1);
extern void res_to_color(char *residue, float *colors);
extern int res_number(char *residue);
extern char *res_name(int res_idx);

//
//	Readers
//
extern int  read_pdb(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<Bond> &bonds, UnitCell &unit_cell, char *space_group);
extern int  read_Cgamess(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_chem_3d(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_coord(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<float> &offsets);
extern int  read_cube(FILE *fp, int read_hydrogens, vector<Atom> &atoms, UnitCell &unit_cell);
extern int  read_dcar(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_dmol_xyz(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_mopac_esp(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_gamess(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_mm3(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_mol2(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_mol(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_plain_coord(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_mopac_xyz(FILE *fp, int read_hydrogens, vector<Atom> &atoms);
extern int  read_kino(FILE *fp, int read_hydrogens, int num_atoms, vector<Atom> &atoms);
extern void read_dcd(FILE *fp, int num_atoms, int big_endian, float *X, float *Y, float *Z);
extern int  read_shelx(FILE *fp, int read_hydrogens, vector<Atom> &atoms, vector<Bond> &bonds, UnitCell &unit_cell, char *space_group);

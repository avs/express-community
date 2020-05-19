/*
**	This file contains an enum for every enum present in the STM3 express interfaces
*/


extern enum {
	NO_BOND,
	SINGLE_BOND,
	H_BOND,
	H_NORMAL_BOND,
	SPECIAL_BOND
} BondTypeType;


extern enum {
	PLAINCOORD,
	COORDS,
	CUBE,
	PDB,
	PGAMESS,
	GAMESSCUBE,
	DCAR,
	DMOL_XYZ,
	MOPAC_ESP,
	MOPAC_XYZ,
	MOL2,
	MOL,
	CHEM_3D,
	MM3,
	SHELX
} FileFormatType;


extern enum {
	KINO,
	MULTI_PDB,
	DCD,
	MULTIFILE_PDB
} AnimFormatType;


extern enum{
	BALL_AND_STICK,
	CPK,
	COLORED_STICK,
	COLORED_RESIDUE,
	STICK,
	LICORICE,
	LICORICE_STICK,
	BALL_AND_STICK_CHARGE,
	CPK_CHARGE,
	LICORICE_CHARGE
} RepresentationType;


extern enum {
	BOHR,
	ANGSTROM
} UnitsType;


extern enum {
	NOTHING,
	REPLICATE,
	CLIP_CYL,
	CLIP_SPHERE
} ClippingType;


extern enum {
	LINEFLAG_LINES,
	LINEFLAG_TUBE,
	LINEFLAG_ALL_H,
	LINEFLAG_H_BONDS
} LineflagType;


extern enum {
	PASS_SELECTED,
	REMOVE_SELECTED,
	PASS_LIKE,
	REMOVE_LIKE
} SelectionType;


/* operation values for Accumulate Traces */
extern enum {
	OP_INACTIVE,
	OP_IMMEDIATE,
	OP_ACCUMULATE,
	OP_PLAYBACK
} OperationType;


extern enum {
	NO_COLOR,
	ATOM_TYPE,
	ATOM_CHARGE,
	ATOM_Z
} MSMSType;


extern enum {
	CROP_BLOCK,
	CROP_CYLINDER_X,
	CROP_CYLINDER_Y,
	CROP_CYLINDER_Z,
	CROP_SPHERE
} CropShapeType;


extern enum {
	TH_BELOW,
	TH_BELOW_EQ,
	TH_BETWEEN,
	TH_BETWEEN_EQ,
	TH_ABOVE,
	TH_ABOVE_EQ,
	TH_EXACT
} ThresholdType;


extern enum {
	SELECT_EXACT,
	SELECT_LIKE
} SelectType;


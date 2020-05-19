/*************************************************************************
*                                                                        *
* Copyright (c) 1993, Scientific Visualization Associates, Inc.,         *
*                                                                        *
* All rights reserved.                                                   *
* This is licensed software of Scientific Visualization Associates, Inc. *
* This material contains trade secret information of Scientific          *
* Visualization Associates, Inc.  Use, repreoduction, or disclosure is   *
* prohibited, except under a written license by Scientific Visualization *
* Associates, Inc.  Use by the Federal Government is governed by         *
* FAR 52.227-19(c), Commercial Computer Software, or for Department of   *
* Defense Users, by DFAR 252.227-7013.                                   *
*                                                                        *
*************************************************************************/

/************************************************************************/
/* Defines								*/
/************************************************************************/
#define		MAX_LINE			256

#define		FIDAP_EDGE		0
#define		FIDAP_QUAD		1
#define		FIDAP_TRI		2
#define		FIDAP_BRICK		3
#define		FIDAP_WEDGE		4
#define		FIDAP_TETRA		5

/************************************************************************/
/* Structures to use in this program.					*/
/************************************************************************/
typedef struct {
   /* Record 2 from FDNEUT File.					*/
   char		title[81];	/* Title of problem.			*/

   /* Record 3 from FDNEUT File.					*/
   float	revision_level;	/* Revision (should be 7.0)		*/
   int		int_version;

   /* Record 4 from FDNEUT File.					*/
   /* No need to keep the date and time.				*/

   /* Record 5 & 6 From FDNEUT File.					*/
   int		numnp;		/* Number of Nodes.			*/
   int		nelem;		/* Number of Elements.			*/
   int		ngrps;		/* Number of Groups.			*/
   int		ndfcd;		/* Number of Coordinate Dir.		*/
   int		ndfvl;		/* Number of Velocity Components	*/

   /* Record 7 & 8 From FDNEUT File.					*/
   int		itim;		/* 0 = Steady         1 = Transient	*/
   int		iturb;		/* 0 = Laminar        2 = K-E Turbulence*/
   int		ifree;		/* 0 = Fixed          1 = Free Surface	*/
   int		icompr;		/* 0 = Incompressible 1 = Compressible	*/

   /* Still need temp - species flags and pressure flags 		*/

   /* Record 11 & 12 From FDNEUT File.					*/
   int		idcts;		/* 0 = Continuous 1 = Discontinuous	*/
   int		ipeny;		/* 0 = Penalty    1 = Mixed		*/
   int		mpdf;		/* Max Num Pres DOF / Element.		*/
} Neutral_Header;

typedef struct {
   int		*node_id;
   float	*x;
   float	*y;
   float	*z;
} Nodal_Coordinates;

typedef struct {
   int		ne;		/* Global Element Number.		*/
   int		*node;		/* List of nodes for defining element.	*/
				/* length of node is ndp.		*/
} Element;

typedef struct {
   int		ngp;		/* Element Group Number.		*/
   int		nelgp;		/* Number Elements in Group.		*/
   int		ndp;		/* Number nodes Per Element in Group.	*/
   int		igeom;		/* Element Geometry Type.		*/
   int		nftyp;		/* Fidap Element Type.			*/
   char		elmmat[21];	/* Entity Name				*/
   Element	*elem_ptr;	/* Points to block of Element structs.	*/
				/* length of elem_ptr is nelgp.		*/
   int		boundary_flag;	/* 1 = boundary elements 0 = non-bound	*/
} Element_Group;

typedef struct {
   int				type;
   unsigned int			fp_loc[3];
   int				species_num;
} Data_Item_Info;

/* Max Data Items is the max data items you could have in one time	*/
/* step.  We could have velocity, temp, free surf, turbul, dissip,	*/
/* density, pres,  and up to 15 species (maybe more species) so 30 is 	*/
/* safe.								*/ 
#define				MAX_DATA_ITEMS		30

/* These are the types for assignment to the "type" entry in 		*/
/* Data_Item_Info.							*/
#define				FIDAP_VELOCITY		0
#define				FIDAP_TEMPERATURE	1
#define				FIDAP_FREE_SURFACE	2
#define				FIDAP_TURBULENT_KE	3
#define				FIDAP_DISSIPATION	4
#define				FIDAP_SPECIES		5
#define				FIDAP_DENSITY		6
#define				FIDAP_PRESSURE		7

struct dataset_entry {
   struct dataset_entry 	*next;
   int				num_blocks;
   float			time_increment;
   float			time_value;
   int				int_seq;
   Data_Item_Info		data_item_info[MAX_DATA_ITEMS];
};
typedef struct dataset_entry Dataset_Entry;

/************************************************************************/
/* Functions in this reader.                                            */
/************************************************************************/
#ifdef _NO_PROTO
int             process_command_line();
void            print_usage();
FILE            *open_neutral_file();
FILE            *open_meta_file();
int             read_neutral_header();
int             read_line();
int             read_string();
int             read_string_no_err();
void            read_past_newline();
int             read_variable_float();
int             read_int();
int             read_float();
/*
int             read_nodal_coordinates();
int             read_element_groups();
*/
void            remove_trailing_blanks();
int             write_meta_file();
Dataset_Entry   *scan_for_datasets();
int		handle_xelem_section();
int		handle_xdata_section();
int		handle_xdblk_section();
float		*read_scalar_data();
float		*read_coord_data();
float		*read_vel_data();
#else
int             process_command_line(int, char **, char *, int *);
void            print_usage(void);
FILE            *open_neutral_file(char *);
FILE            *open_meta_file(char *);
int             read_neutral_header(FILE *, Neutral_Header *);
int             read_string_no_err(FILE *, char *, int);
void            read_past_newline(FILE *);
int             read_variable_float(FILE *, float *);
int             read_int(FILE *, int *, int);
int             read_float(FILE *, float *, int);
/*
int             read_nodal_coordinates(FILE *, Neutral_Header, Nodal_Coordinates *);
int             read_element_groups(FILE *, Neutral_Header, Element_Group *);
*/
void            remove_trailing_blanks(char *);
int             write_meta_file(FILE *, Neutral_Header, Nodal_Coordinates, 
                   Element_Group *, Dataset_Entry *, FILE *, int);
Dataset_Entry   *scan_for_datasets(FILE *, Neutral_Header, int *);
int             handle_xelem_section(FILE *, Neutral_Header, Element_Group *,
                   int, int);
int             handle_xdata_section(FILE *, Dataset_Entry *);
int             handle_xdblk_section(FILE *, Dataset_Entry *, Neutral_Header, FILE *, Nodal_Coordinates);
float           *read_scalar_data(FILE *, unsigned int, int, float *, float *);
float           *read_coord_data(FILE *, unsigned int *, int, int, float *, float *);
float           *read_vel_data(FILE *, unsigned int, int, int, float *, float *);


int             read_line(FILE *, char *);
int             read_string(FILE *, char *, int);



#endif

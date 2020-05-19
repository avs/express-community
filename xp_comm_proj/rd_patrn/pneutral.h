/*
 *
 *  PATRAN neutral constants for different packet types
 *
 * (taken from file by David Laidlaw, 1991 Stardent Computer Inc.
 */

#ifndef _pneutral_h_
#define _pneutral_h_


/* packet types */
/* initial */
#define P_TITLE			25
#define P_SUMMARY		26   
/* FEM packets */
#define P_NODE_DATA		1
#define P_ELEMENT_DATA		2
#define P_MATERIAL_PROP		3
#define P_ELEMENT_PROP		4  	 
#define P_COORD_FRAME		5
#define P_DIST_LOAD		6
#define P_NODE_FORCE		7
#define P_NODE_DISP		8
#define P_BAR_IDISP		9
#define P_NODE_TEMP		10
#define P_ELEMENT_TEMP		11
#define P_DOF_LIST		12
#define P_MECH_ENT		13
#define P_MPC_DATA		14
#define P_NODAL_HEAT		15
#define P_ELEMENT_HEAT		16
#define P_CONVECTION		17
#define P_RADIATION		18
#define P_VIEW_FACTOR		19
#define P_NAME_COMP		21
/* geom model packets */
#define P_GRID_DATA		31
#define P_LINE_DATA		32
#define P_PATCH_DATA		33
#define P_HYPER_DATA		34
#define P_DATA_LINE_DATA	36
#define P_DATA_PATCH_DATA	37
#define P_DATA_HYPER_DATA	38
#define P_FIELD_DATA		39
#define P_FIELD_DATA_PCL	48
#define P_LIST_CARD		40
#define P_DATA_CARD		41
/* GFEG/CFEG */
#define P_GFEG_LINE4		42
#define P_GFEG_PATCH		43
#define P_GFEG_HYPER		44
#define P_CFEG			45
/* trimmed surface solid model */
#define P_PRIM			46
#define P_PRIM_FACE		47
#define P_EOF			99

/* element types */
#define PE_BAR		2
#define PE_TRI		3
#define PE_QUAD		4
#define PE_TET		5
#define PE_WEDGE	6
#define PE_HEX		8
   

#endif

/*
 */

/*
 * load_neutral class definition
 */

#ifndef _load_neutral_h_
#define _load_neutral_h_

#define PN_MAX_BUF_LEN 501  // maximum length of line
#define PN_TITLE_LENGTH 81 // length of title
#define PN_MAX_ELEM_DATA 5 // maximum number of data values per element
#define PN_MAX_ELEM_NODES 10 // maximum number of nodes per element
#define PN_NODEDATA_BUFLEN 10000 // number of node data packets that can be buffered
#define PN_ELEMENTDATA_BUFLEN 31000 // number of element data packets that can be buffered
#define PN_GRIDDATA_BUFLEN 50 // number of grid data packets that can be buffered
#define PN_LINEDATA_BUFLEN 50 // number of line data packets that can be buffered
#define PN_PATCHDATA_BUFLEN 50 // number of patch data packets that can be buffered


/*
 * STATUS FLAGS for return from card reading functions
 */
#define pnBUFFER_FULL 2 // card read OK but buffer is full
#define pnOK 1 // card read OK
#define pnEOF 0 // end of file instead of new card
#define pnFAIL -1 // failed to read card
#define pnCATASTROPHE -2 // unrecoverable error

extern "C" {

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "pneutral.h"
#include "rnum.h"

};

typedef struct str_pnReport {
	int possible;
	int count;
	char text[50];
} str_pnReport;

typedef struct str_pnHeader {
	// contents of the packet header
	int it; // packet type
	int id; // identifier (0 = not applicable)
	int iv; // additional identifier (0 = not applicable)
	int kc; // number of cards
	int n1;
	int n2;
	int n3;
	int n4;
	int n5;
} str_pnHeader;

typedef struct str_pnSummary {
	// Summary data
	char title[PN_TITLE_LENGTH];
	char title2[PN_TITLE_LENGTH];
	int summary_n_nodes;  // count of nodes accroding to summary card
	int summary_n_elements;
	int summary_n_materials;
	int summary_n_elem_props;
	int summary_n_coord_fram;
} str_pnSummary;

// information on array sizes
typedef struct str_buffer_arrays {
	int NodeData;
	int ElementData;
	int GridData;
	int LineData;
	int PatchData;
} str_buffer_arrays;

typedef struct str_pnNodeData {
	// buffer for node data cards

	int id; // node id

	double x; // coordinates
	double y;
	double z;

	int icf; // condensation flag (NOT USED)
	char gtype; // node type
	int ndf;  // number of degrees of freedom (NOT USED)
	int config; // node configuration

	int cid; // coordinate frame

	int pspc1; // single point constraint flags (NOT USED)
	int pspc2;
	int pspc3;
	int pspc4;
	int pspc5;
	int pspc6;
} str_pnNodeData;

typedef struct str_pnElementData {
	int id; // element id
	int type; // from header.iv: 2=line;3=tri;4=quad;5=tet;6=prism;7=hex

	// ELEMENT DATA
	int n_data; // number of data values 
	            // (We will handle up to 5 data values; i.e. the first card.)
	double elem_data[PN_MAX_ELEM_DATA];

	int id_node_in_xy; // id of node in x-y plane (bar only)

	int n_nodes;
	int nodes[PN_MAX_ELEM_NODES]; // 10 values are allowed (9th and 10th
	                         // support pin flags).
	int connectivity[PN_MAX_ELEM_NODES];
	  // node list based upion order rather than node-id
	int nodes_do_exist; // nodes in connectivity list are found

	int config;
	int property_id;  // 0 = missing  } both of these come
	int material_id;  // 0 = missing  } from PID field
	int ceid;
	double theta1,theta2,theta3;
} str_pnElementData;

typedef struct str_pnLineData 
{
} str_pnLineData;


// added by S Larkin (2/2/98)

typedef struct str_pnGridData
{
	// from the file
	int id;
	double x;
	double y;
	double z;
} str_pnGridData;

typedef struct str_pnPatchData
{
	// from the file
	int id;
	int corners[4]; // grid id's
	double coefficients[16][3];

	// resolution flag:
	// 0 = use corners only
	// 1 = use coefficients as vertices (i.e. 3x3)
	// 2 = double number of quads (i.e. 6x6)
	// 3 = triple the number of quads (i.e. 9x9)
	// (formula is (3*resolution)**2)

	int resolution; // might change this as input to tessellation alg for patch

	// the following are calculated from number of node_datas and the
	// resolution of grids.
	// number of vertices.

	int n_verts;
	int verts_per_side;
	int n_quads;

	// index of first vertex.

	int first_vertex_index;
	int grid_vertex_index; // used to store start of grids in node array

} str_pnPatchData;


class load_neutral {

public:
 
   //////////////////////
   // PUBLIC FUNCTIONS //
   //////////////////////
   load_neutral();
   ~load_neutral();

   // read a new neutral file
   int read_neutral_file(char *filename);

   // access functions
   const char *title1();  // title string
   const char *title2();  // secondary title string from summary card

   int n_nodes(); // number of nodes

   // simple elements
   int n_elements(int type, int material, int property); // number of elements
   int connectivity (int type, int material, int property, int* conn); // load connectivity list
   int nodes_per_cell(int type);

   // griddata

   int n_grid_data();
   
   // patches
   int n_patch_quads(); // number of quads used to define all patches
   int patch_connectivity(int *conn); // quad connectivity for patches

   void print_report(); // print the report after the file was read

   // coordinates returned as double.  the array space ptr
   // is managed by the calling program
   void double_coordinates(double *ptr); 

   ////////////////////
   // PUBLIC MEMBERS //
   ////////////////////

private:

	///////////////////////
    // PRIVATE FUNCTIONS //
	///////////////////////

   // read the next packet's header into member Header; return 1 (0)
   // for success (failure).
   int load_packet_header();
   int getFile(char *filename);
   void reset_buffer(void);
   int load_packet();
   void collate_connectivity(); // generate a connectivity list based
                            // on node ordering rather than node-id
   void init_report(); // initialise the report
   void init_report_element(int i, char *s); // initialise a report element

   // FUNCTIONS TO READ CARDS (see status flags for return codes)
   int rdTitle(); // title card
   int rdSummary(); // summary card
   int rdNodeData(); // node data card
   int rdElementData(); // element data card
   int rdGridData();// grid data card
   int rdLineData(); // Line Data
   int rdPatchData(); // Patch Data

   /////////////////////
   // PRIVATE MEMBERS //
   /////////////////////
   FILE* fp;  // pointer to the neutral file
   char line_buffer[PN_MAX_BUF_LEN]; // buffer to contain current line

   // CONTROL OVER SIZE OF ARRAYS OF ELEMENTS
   str_buffer_arrays BufferCount; // count of nodes etc currently stored
                                // within buffers
   str_buffer_arrays BufferMax; // maximum size of buffers for nodes etc.
   str_buffer_arrays BlockSize; // block size increments for node data buffers etc.

   // Summary of all packets read
   // initialised by init_report();
   // maintained by load_packet_header();
   // output by print_report();
   str_pnReport Report[100];

   // CONTENTS OF CURRENT PACKET HEADER
   str_pnHeader Header;

   // SUMMARY DATA
   str_pnSummary Summary; // summary data
   
   // NODE DATA
   str_pnNodeData *NodeData; // array buffer of node data.
   str_pnNodeData *pNodeData; // pointer into buffer of node data.
   
   // ELEMENT DATA
   str_pnElementData *ElementData; // array buffer of element data.
   str_pnElementData *pElementData; // pointer into buffer of element data.

   // GRID DATA
   str_pnGridData *GridData;  // array buffer of grid data
   str_pnGridData *pGridData; // pointer to array of grid data;

   // LINE DATA
   str_pnLineData *LineData; // array buffer of line data
   str_pnLineData *pLineData; // pointer to array buffer of line data

   // PATCH DATA
   str_pnPatchData *PatchData; // array buffer of patch data
   str_pnPatchData *pPatchData; // pointer to array buffer of patch data

};




#endif


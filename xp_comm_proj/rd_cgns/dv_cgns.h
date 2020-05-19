#include <string.h>

#include <avs/f_utils.h>

#include "gen.h"
#include "cgnslib.h"

#define CGNS_NAME_MAX_LEN 33 /* maximal length of strings in CGNS (32+termin) */

#define NGON_MAX NGON_n+21   /* first NGON not supported - support for up to 20 vertices */

/* AVS IDs for cell_sets */

#define Point	1
#define Line	2
#define Line2	12
#define Tri		4
#define Tri2	14
#define Quad	5
#define Quad2	15
#define Tet		6
#define Tet2	16
#define Pyr		9
#define Pyr2	19
#define Prism	8
#define Prism2	18
#define Hex		7
#define Hex2	17
#define PolyH	20 /* actually it's 19 in AVS, but it would conflict with Pyr2 */

void reset_output_FLDs(OMobj_id DVread_cgns_id);

int add_structured_coords(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int index_dim, int phys_dim, int *vertex_size,
	int *rind, int *XYZ);

int add_structured_cell_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int *rind, int solution);

int add_structured_vrtx_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int *rind, int solution);



int add_unstructured_coords(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int phys_dim, int *vertex_size,
	int *XYZ);

int add_unstructured_sections(OMobj_id DVread_cgns_id, OMobj_id zone_id,
	char *file_name, int base, int zone);

int add_unstructured_vrtx_data(OMobj_id DVread_cgns_id, char *file_name,
	int base, int zone, int fld_el_indx, int index_dim, int *vertex_size,
	int solution);

int add_unstructured_cell_data(OMobj_id DVread_cgns_id, OMobj_id zone_id,
	char *file_name, int base, int zone, int fld_el_indx, int *vertex_size,
	int solution);


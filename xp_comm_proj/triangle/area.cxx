
#include "gen.h"
#include <math.h>

// compute the area and minimum angle of each triangle in the
// list. useful for determining parameters for further refinement
// of the mesh

int
Triangle_TriangleMods_compute_triangle_areas::update(OMevent_mask event_mask, int seq_num)
{
	// ntris (OMXint write)
	// connect (OMXint_array read req notify)
	int connect_size;
	int *connect_arr;

	// coords (OMXfloat_array read req notify)
	int coords_size;
	float *coords_arr;

	// areas (OMXfloat_array write)
	int areas_size;
	float *areas_arr;

	connect_arr = (int *)connect.ret_array_ptr(OM_GET_ARRAY_RD,&connect_size);
	coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_RD,&coords_size);
	
	ntris = connect_size/3;
	areas_arr = (float *)areas.ret_array_ptr(OM_GET_ARRAY_WR,&areas_size);

	int i1,i2,i3,i;
	float x1,x2,x3,y1,y2,y3;

	for (i=0; i<ntris; i++) {

		i1 = connect_arr[i*3+0];
		i2 = connect_arr[i*3+1];
		i3 = connect_arr[i*3+2];

		x1 = coords_arr[i1*2+0];
		y1 = coords_arr[i1*2+1];
		x2 = coords_arr[i2*2+0];
		y2 = coords_arr[i2*2+1];
		x3 = coords_arr[i3*2+0];
		y3 = coords_arr[i3*2+1];

		// compute area
		areas_arr[i] = 0.5 * ((x1*y2-y1*x2)+(x2*y3-y2*x3)+(x3*y1-y3*x1));

		// compute minimum angle
		// TODO
	}

	if (connect_arr) ARRfree((char *)connect_arr);
	if (coords_arr) ARRfree((char *)coords_arr);
	if (areas_arr) ARRfree((char *)areas_arr);


	// return 1 for success
	return(1);
}
// end of file


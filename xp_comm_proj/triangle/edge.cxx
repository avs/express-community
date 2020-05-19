
#include "gen.h"

// fix voronoi edges: remove infinite edges in the voronoi diagram
// (flagged by a -1 index) by setting both endpoints to be the same
int
Triangle_TriangleMods_fix_voronoi_edges::update(OMevent_mask event_mask, int seq_num)
{
	// in_edges (OMXint_array read req notify)
	int in_edges_size;
	int *in_edges_arr;

	// out_edges (OMXint_array write)
	int out_edges_size;
	int *out_edges_arr;

	in_edges_arr = (int *)in_edges.ret_array_ptr(OM_GET_ARRAY_RD,&in_edges_size);
	OMset_array_size(out_edges.obj_id(),in_edges_size);
	out_edges_arr = (int*)out_edges.ret_array_ptr(OM_GET_ARRAY_WR,&out_edges_size);

	int n = in_edges_size/2;
	int i;

	for (i=0; i<n; i++) {
		out_edges_arr[i*2+0] = in_edges_arr[i*2+0];
		if (in_edges_arr[i*2+1] == -1) out_edges_arr[i*2+1] = in_edges_arr[i*2+0];
		else out_edges_arr[i*2+1] = in_edges_arr[i*2+1];
	}

	if (in_edges_arr) ARRfree((char *)in_edges_arr);
	if (out_edges_arr) ARRfree((char *)out_edges_arr);

	// return 1 for success
	return(1);
}
// end of file


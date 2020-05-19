
#include "gen.h"
#include <string.h>

#define MIN(A,B) ((A)<(B) ? (A) : (B))
#define MAX(A,B) ((A)>(B) ? (A) : (B))


int
Triangle_TriangleMods_fault_region::update(OMevent_mask event_mask, int seq_num)
{
	// mesh_connect (OMXint_array read notify)
	int mesh_connect_size;
	int *mesh_connect_arr = NULL;
	
	// mesh_coords (OMXfloat_array read notify)
	int mesh_coords_size;
	float *mesh_coords_arr = NULL;

	// poly_coords (OMXfloat_array read notify)
	int poly_coords_size;
	float *poly_coords_arr = NULL;

	// npts (OMXint write)
	// pts (OMXfloat_array write)
	int pts_size;
	float *pts_arr = NULL;

	// nsegs (OMXint write)
	// segments (OMXint_array write)
	int segments_size;
	int *segments_arr = NULL;

	// ncells (OMXint write)
	// celldat (OMXint_array write)
	int celldat_size;
	int *celldat_arr = NULL;


	/***********************/
	/* Function's Body     */
	/***********************/
	poly_coords_arr = (float *)poly_coords.ret_array_ptr(OM_GET_ARRAY_RD,&poly_coords_size);
	if (!poly_coords_arr) {
	   return(0);
	}
	mesh_connect_arr = (int *)mesh_connect.ret_array_ptr(OM_GET_ARRAY_RD,&mesh_connect_size);
	if (!mesh_connect_arr) { 
		ARRfree((char*)poly_coords_arr);
		return(0);
	}
	mesh_coords_arr = (float *)mesh_coords.ret_array_ptr(OM_GET_ARRAY_RD,&mesh_coords_size);
	if (!mesh_coords_arr) {
		ARRfree((char*)poly_coords_arr);
		ARRfree((char*)mesh_connect_arr);
		return(0);
   }

	int np = poly_coords_size/2;
	int nmp = mesh_coords_size/2;
	int nc = mesh_connect_size/4;

	ncells = nc;
	celldat_arr = (int *)celldat.ret_array_ptr(OM_GET_ARRAY_WR,&celldat_size);

	int i, j, k;
	float x, y, cx[2], cy[2];
	float min_x, max_x, min_y, max_y;
	int found, nfound, corner[4];

	// reset celldat array
	for (i=0; i<nc; i++) celldat_arr[i] = 0;

	// go through points array
	nfound = 0;
	for (i=0; i<np; i++) {

		x = poly_coords_arr[i*2+0];
		y = poly_coords_arr[i*2+1];

		// go through mesh array until we find the cell
		// which contains the node
		found = 0;
		for (j=0; j<nc && !found; j++) {

			// get corner nodes
			corner[0] = mesh_connect_arr[j*4+0];
			corner[1] = mesh_connect_arr[j*4+1];
			corner[2] = mesh_connect_arr[j*4+2];
			corner[3] = mesh_connect_arr[j*4+3];

			// get corner coords
			cx[0] = mesh_coords_arr[corner[0]*2+0];
			cx[1] = mesh_coords_arr[corner[2]*2+0];

			cy[0] = mesh_coords_arr[corner[0]*2+1];
			cy[1] = mesh_coords_arr[corner[2]*2+1];

			min_x = MIN(cx[0],cx[1]);
			max_x = MAX(cx[0],cx[1]);
			min_y = MIN(cy[0],cy[1]);
			max_y = MAX(cy[0],cy[1]);

			// determine if point lies inside cell
			if (x >= min_x && x <= max_x && y >= min_y && y <= max_y) {
				found = 1;
				if (!celldat_arr[j]) nfound++;
				celldat_arr[j] = 1;
			}
		}
	}

	if (nfound) {

		// temporary storage for cell edges
		int *edges = new int[8*nfound];
		int *ext = new int[8*nfound];

		j = 0;
		for (i=0; i<nc; i++) {
			if (celldat_arr[i]) {

				// add edges: 01 12 32 03
				edges[j*8+0] = mesh_connect_arr[i*4+0];
				edges[j*8+1] = mesh_connect_arr[i*4+1];
				edges[j*8+2] = mesh_connect_arr[i*4+1];
				edges[j*8+3] = mesh_connect_arr[i*4+2];
				edges[j*8+4] = mesh_connect_arr[i*4+3];
				edges[j*8+5] = mesh_connect_arr[i*4+2];
				edges[j*8+6] = mesh_connect_arr[i*4+0];
				edges[j*8+7] = mesh_connect_arr[i*4+3];
				j++;
			}
		};

		k = 0;

		// remove duplicate edges and copy to ext array
		for (i=0; i<4*nfound; i++) {

			found = 0;
			for (j=0; j<4*nfound && !found; j++) {
				if (i==j) continue;
				if ((edges[i*2+0] == edges[j*2+0]) && (edges[i*2+1] == edges[j*2+1])) found = 1;
			}
			if (!found) {
				ext[k*2+0] = edges[i*2+0];
				ext[k*2+1] = edges[i*2+1];
				k++;
			}
		}

		nsegs = k;
		delete edges;

		// now reorder the edges from zero
		int *ptr = new int[nmp];
		int *nd = new int[2*nsegs];
		k = 0;
		for (i=0; i<nmp; i++) ptr[i] = -1;
		for (i=0; i<2*(int)nsegs; i++) {
			j = ext[i];
			if (ptr[j] == -1) {
				ptr[j] = k;
				ext[i] = k;
				nd[k] = j;
				k++;
			} else ext[i] = ptr[j];
		}

		// set the segments array
		segments_arr = (int *)segments.ret_array_ptr(OM_GET_ARRAY_WR,&segments_size);
		if (segments_arr) {
			memcpy(segments_arr, ext, 2*(int)nsegs*sizeof(int));
			ARRfree((char *)segments_arr);
		}
		delete ext;

		// total number of new points
		npts = k+np;	
		pts_arr = (float *)pts.ret_array_ptr(OM_GET_ARRAY_WR,&pts_size);
		if (pts_arr) {
			for (i=0; i<k; i++) {
				pts_arr[i*2+0] = mesh_coords_arr[nd[i]*2+0];
				pts_arr[i*2+1] = mesh_coords_arr[nd[i]*2+1];
			}
			// add polyline points
			for (i=k; i<np+k; i++) {
				pts_arr[i*2+0] = poly_coords_arr[(i-k)*2+0];
				pts_arr[i*2+1] = poly_coords_arr[(i-k)*2+1];
			}
			ARRfree((char *)pts_arr);
		}
		delete ptr;
		delete nd;
	}


	if (mesh_coords_arr) ARRfree((char *)mesh_coords_arr);
	if (mesh_connect_arr) ARRfree((char *)mesh_connect_arr);
	if (poly_coords_arr) ARRfree((char *)poly_coords_arr);
	if (celldat_arr) ARRfree((char *)celldat_arr);

	// return 1 for success
	return(1);
}
// end of file


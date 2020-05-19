
#include "gen.h"

extern "C" {
#include "tridefs.h"
}

#define FREE_ALL_ARRAYS { \
	if (point_arr!=NULL)      ARRfree((char *)point_arr); \
	if (connect_arr!=NULL)    ARRfree((char *)connect_arr); \
	if (neighbours_arr!=NULL) ARRfree((char *)neighbours_arr); \
	if (coords_arr!=NULL)     ARRfree((char *)coords_arr); \
	if (index_arr!=NULL)      ARRfree((char *)index_arr); \
	if (count_arr!=NULL)      ARRfree((char *)count_arr); \
}


int
Triangle_TriangleMods_point_in_triangle::update(OMevent_mask event_mask, int seq_num)
{
	// mode (OMXint read req notify)
	// connect (OMXint_array read req notify)
	int connect_size;
	int *connect_arr = NULL;

	// neighbours (OMXint_array read notify)
	int neighbours_size;
	int *neighbours_arr = NULL;

	// coords (OMXfloat_array read req notify)
	int coords_size;
	float *coords_arr = NULL;

	// point (OMXfloat_array read req notify)
	int point_size;
	float *point_arr = NULL;

	// index (OMXint_array write)
	int index_size;
	int *index_arr = NULL;

	// found (OMint_array write)
	int count_size;
	int *count_arr = NULL;

	int nc = (int)ncoords;
	int nt = (int)ntris;
	int np = (int)npoints;
	int local_mode = (int)mode;

	if ((nc<=0) || (nt<=0) || (np<=0)) {
		return(1);
	}

	point_arr = (float *)point.ret_array_ptr(OM_GET_ARRAY_RD,&point_size);
    if (point_arr==NULL) {
		FREE_ALL_ARRAYS;
		return(0);
	} else if (point_size!=(2*np)) {
		FREE_ALL_ARRAYS;
		return(0);
	}

   connect_arr = (int *)connect.ret_array_ptr(OM_GET_ARRAY_RD,&connect_size);
   if (connect_arr==NULL) {
		FREE_ALL_ARRAYS;
		return(0);
	} else if (connect_size!=(3*nt)) {
		FREE_ALL_ARRAYS;
		return(0);
	}

   if (local_mode) {
   	neighbours_arr = (int *)neighbours.ret_array_ptr(OM_GET_ARRAY_RD,&neighbours_size);
      if ((neighbours_arr==NULL) || (neighbours_size!=(3*nt))) {
         local_mode = 0;
   	}
   }

	coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_RD,&coords_size);
   if (coords_arr==NULL) {
		FREE_ALL_ARRAYS;
		return(0);
	} else if (coords_size!=(2*nc)) {
		FREE_ALL_ARRAYS;
		return(0);
	}

	index_arr = (int *)index.ret_array_ptr(OM_GET_ARRAY_WR,&index_size);
   if (index_arr==NULL) {
		FREE_ALL_ARRAYS;
		return(0);
	} else if (index_size!=np) {
		FREE_ALL_ARRAYS;
		return(0);
	}

	count_arr = (int *)count.ret_array_ptr(OM_GET_ARRAY_WR,&count_size);
   if (count_arr==NULL) {
		FREE_ALL_ARRAYS;
		return(0);
	} else if (count_size!=nt) {
		FREE_ALL_ARRAYS;
		return(0);
	}

	int i, j = 0, k;
	int i0, i1, i2;
	float p[2], p0[2], p1[2], p2[2];

	// reset found array
	for (i=0; i<nt; i++)
	   count_arr[i] = 0;

	// go through points
	for (k=0; k<np; k++) {

		p[0] = point_arr[k*2+0];
		p[1] = point_arr[k*2+1];
		index_arr[k] = -1;

		// use directed search
		if (local_mode) {

			// start at triangle zero
			i = 0;
			while (j != -1 && i != -1) {

				i0 = connect_arr[i*3+0];
				i1 = connect_arr[i*3+1];
				i2 = connect_arr[i*3+2];

				p0[0] = coords_arr[i0*2+0];
				p0[1] = coords_arr[i0*2+1];
				p1[0] = coords_arr[i1*2+0];
				p1[1] = coords_arr[i1*2+1];
				p2[0] = coords_arr[i2*2+0];
				p2[1] = coords_arr[i2*2+1];

				j = barycentric(p, p0, p1, p2);
				if (j != -1) i = neighbours_arr[i*3+j];
			}

			if (j == -1 && i > -1) {
				index_arr[k] = i;
				count_arr[i]++;
			}
		
		} else {
			// use exhaustive search
			for (i=0; i<ntris && j > -1; i++) {

				i0 = connect_arr[i*3+0];
				i1 = connect_arr[i*3+1];
				i2 = connect_arr[i*3+2];

				p0[0] = coords_arr[i0*2+0];
				p0[1] = coords_arr[i0*2+1];
				p1[0] = coords_arr[i1*2+0];
				p1[1] = coords_arr[i1*2+1];
				p2[0] = coords_arr[i2*2+0];
				p2[1] = coords_arr[i2*2+1];

				j = barycentric(point_arr, p0, p1, p2);
				if (j == -1) {
					index_arr[k] = i;
					count_arr[i]++;
				}
			}
		}
	}

	FREE_ALL_ARRAYS;

	// return 1 for success
	return(1);
}
// end of file


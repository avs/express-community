#include "gen.h"

// create a seismic 'wiggle' display from a 2d structured mesh. Output
// coordinates and connectivity lists for conversion to fields

#include <math.h>

// #define DEBUG

#define SIGN(a) (((a)==0.0) ? 0 : (((a)<0.0) ? -1 : 1))


int
Wiggle_WiggleMods_WiggleCore::update(OMevent_mask event_mask, int seq_num)
{

	// in_fld (Mesh_Struct+Node_Data read req notify)
	if (!in_fld.valid_obj()) return (0);

	// in_fld.ndim (int) 
	// in_fld.dims (int []) 
	int *dims = (int *)in_fld.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!dims) return(0); // check dims OK

	// in_fld.nspace (int) 
	// in_fld.nnodes (int)
	// in_fld.coordinates.values (float [])
	float *in_fld_coordinates = (float *)in_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);

	// direction (OMXint read req notify)
	// scale (OMXfloat read req notify)
	// mode (OMXint read req notify)
	// component (OMXint read req notify)
	// offset (OMXfloat read req notify)
	// line_pts (OMXfloat_array write)
	int line_pts_size;
	float *line_pts_arr;

	// line_connect (OMXint_array write)
	int line_connect_size;
	int *line_connect_arr;

	// tri_pts (OMXfloat_array write)
	int tri_pts_size;
	float *tri_pts_arr;

	// tri_connect (OMXint_array write)
	int tri_connect_size;
	int *tri_connect_arr;


	/***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Wiggle_WiggleMods_wiggle::update\n");
#endif

	// get array pointers
	line_pts_arr = (float *)line_pts.ret_array_ptr(OM_GET_ARRAY_WR,&line_pts_size);
	line_connect_arr = (int *)line_connect.ret_array_ptr(OM_GET_ARRAY_WR,&line_connect_size);
	tri_pts_arr = (float *)tri_pts.ret_array_ptr(OM_GET_ARRAY_WR,&tri_pts_size);
	tri_connect_arr = (int *)tri_connect.ret_array_ptr(OM_GET_ARRAY_WR,&tri_connect_size);
	float *in_data_arr = (float*)in_data.ret_array_ptr(OM_GET_ARRAY_RD);

	float x1,y1,x2,y2,xs,ys,xt,yt,d1,d2,r;
	int i,j,k1,k2,l=0,m=0,ni,nj,ii,jj, interrupt,msave,nt=0;

	// switch dims according to direction
	if (direction == 0) {
		ni = dims[0];
		nj = dims[1];
	} else {
		ni = dims[1];
		nj = dims[0];
	}

	// loop over nodes
	for (j=0; j<nj; j++) {

		msave = m;

		for (i=0; i<ni; i++) {

			if (direction == 0) {
				ii = i; jj = j;
			} else {
				ii = j; jj = i;
			}

			k1 = jj*dims[0]+ii;
			if (direction==0) k2 = k1+1;
			else if (i!=(ni-1)) k2 = k1+dims[0];

			x1 = in_fld_coordinates[k1*2+0];
			y1 = in_fld_coordinates[k1*2+1];
			d1 = in_data_arr[k1]-offset;

			x2 = in_fld_coordinates[k2*2+0];
			y2 = in_fld_coordinates[k2*2+1];
			d2 = in_data_arr[k2]-offset;

			// amount to offset
			xs = direction*scale*d1;
			ys = !direction*scale*d1;

			// line points
			line_pts_arr[l*2+0] = x1+xs;
			line_pts_arr[l*2+1] = y1+ys;

			// clamp points according to mode
			if (mode == 1) {
				if (ys < 0.0) ys = 0.0;
				if (xs < 0.0) xs = 0.0;
			} else
			if (mode == 2) {
				if (ys > 0.0) ys = 0.0;
				if (xs > 0.0) xs = 0.0;
			}

			// calculate triangle point intersection
			xt = yt = 0.0;
			if (direction == 0) {
				r = -d1*(x2-x1)/(d2-d1);
				if (((x1+r) >= x1 && (x1+r) <= x2)) xt = r;

			} else {
				r = -d1*(y2-y1)/(d2-d1);
				if (((y1+r) >= y1 && (y1+r) <= y2)) yt = r;
			}

			// fill triangle point array
			tri_pts_arr[m*2+0] = x1;
			tri_pts_arr[m*2+1] = y1;
			m++;
			tri_pts_arr[m*2+0] = x1+xs;
			tri_pts_arr[m*2+1] = y1+ys;
			m++;
			
			// add extra points if line crosses base line
			if ((i != (ni-1)) && ((d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0))) {
					
					tri_pts_arr[m*2+0] = x1+xt;
					tri_pts_arr[m*2+1] = y1+yt;
					m++;
					tri_pts_arr[m*2+0] = x1+xt;
					tri_pts_arr[m*2+1] = y1+yt;
					m++;
			}
			l++;
		}

		// fill connect lists
		line_connect_arr[j*2+0] = j*ni;
		line_connect_arr[j*2+1] = j*ni+ni-1;

		// fill triangle connect list
		tri_connect_arr[j*2+0] = msave;
		tri_connect_arr[j*2+1] = m-1;

		OMstatus_check(100*j/(float)nj, "creating wiggle", &interrupt);
	}

	// free array pointers
	if (dims) ARRfree((char *)dims);
	if (in_fld_coordinates) ARRfree((char *)in_fld_coordinates);
	if (line_pts_arr) ARRfree((char *)line_pts_arr);
	if (line_connect_arr) ARRfree((char *)line_connect_arr);
	if (tri_pts_arr) ARRfree((char *)tri_pts_arr);
	if (tri_connect_arr) ARRfree((char *)tri_connect_arr);
	if (in_data_arr) ARRfree((char *)in_data_arr);

	// return 1 for success
	return(1);
}




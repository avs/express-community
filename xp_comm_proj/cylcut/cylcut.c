/*
        Cylinder Distance module code
        Adapted from Cut Plane, DVplane_dist
        Ian Curington, AVS 11 Feb, 99


			Copyright (c) 1999 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

*/


#include "xp_comm_proj/cylcut/gen.h"

#include <avs/util.h>
#include <avs/err.h>
#include <avs/db.h>
#include <avs/fld.h>
#include <avs/math.h>
#include <avs/arr.h>
#include <avs/om_att.h>

#define ERR_RETURN(A) ERRerror("cylinder_cut_dist",1,ERR_ORIG, A); return(0);
#define MAX_NAME_SIZE 1024
#define EPS  0.0001

/* uncomment the next line to get all debug messages */
/*
     #define DEBUG 1
 */

static float IXFORM[16] = {1.0, 0.0, 0.0, 0.0,
			   0.0, 1.0, 0.0, 0.0,
			   0.0, 0.0, 1.0, 0.0,
			   0.0, 0.0, 0.0, 1.0};

int cylinder_cut_update(elem_id)
    OMobj_id elem_id;
{
	OMobj_id in, out, plane;
	float dist;

	if (OMis_null_obj(elem_id)) {
		ERR_RETURN("module id is 0");
	}

	in = OMfind_subobj(elem_id, OMstr_to_name("in"), OM_OBJ_RD);
	if (OMis_null_obj(in)) {
		ERR_RETURN("in id is 0");
	}
	out = OMfind_subobj(elem_id, OMstr_to_name("out"), OM_OBJ_RW);
	if (OMis_null_obj(out)) {
		ERR_RETURN("out id is 0");
	}
	plane = OMfind_subobj(elem_id, OMstr_to_name("plane"), OM_OBJ_RD);
	if (OMis_null_obj(plane)) {
		ERR_RETURN("plane id is 0");
	}
	if (!FUNCcylinder_dist(in, plane, out, &dist)) {
		return(0);
	}

	if (OMset_name_real_val (elem_id, OMstr_to_name("dist"), (double)dist) != 1) {
		ERR_RETURN("cannot set dist value");
	}
	return(1);
}

int FUNCcylinder_dist (in, plane, out, dist)
    OMobj_id  in, plane, out;
    float      *dist;
{
	int   nnodes, type, size, xsize, xlsize;
	int   nspace, plane_nspace, npts, stat, i;
	float  *coord, *pnts;
	float  bounds[3], norm[3], norm_xfm[3], *norm_x;
	float  xfm[4][4], *in_xfm, in_xform[4][4], xfm_inv[4][4];
	float  cross[3], l;
	float  *xlate;
	float  *node_dist, plane_dist;
	int    in_seq, out_seq;
	OMobj_id pln_id, xform_id, xlate_id;

	/*-----------------*/
	/*   GET PLANE     */
	/*-----------------*/
	if (FLDget_nspace (plane, &plane_nspace) != 1) {
		ERR_RETURN("Error getting nspace");
	}
	if (FLDget_points (plane, &pnts, &size, OM_GET_ARRAY_RD) != 1) {
		ERR_RETURN("Error getting points");
	}
	stat = FLDget_xform(plane, (float *)xfm);
	if (stat < 0) {
		ERR_RETURN("cannot get xform for probe");
	}

	/*------------------*/
	/* TRANSFORM PLANE  */
	/*------------------*/
	stat = FLDget_xform(in, (float *)in_xform);
	if (stat < 0) {
		ERR_RETURN("cannot get xform for field");
	}
	else if (stat == 0) {
		in_xfm = (float *)0;
	}
	else if (MATmat_is_identity((float *)in_xform, 4))
		in_xfm = (float *)0;
	else
		in_xfm = (float *)in_xform;

	for (i=0; i<plane_nspace; i++)
		bounds[i] = pnts[i];

	ARRfree(pnts);

	for (; i<3; i++)
		bounds[i] = 0.0;
	norm[0] = 0.0; norm[1] = 0.0; norm[2] = 1.0;

	MATvec3_mat4_multiply(bounds, xfm);

	MATxform_vecs(1, (Matr3 *)norm, xfm, (Matr3 *)norm_xfm);

	l = VEC_LEN(norm_xfm);
	VEC_NORMALIZE(norm_xfm,l);

	plane_dist = VEC_DOT(bounds, norm_xfm);

	*dist = plane_dist;

	xform_id = OMfind_subobj(plane, OMstr_to_name("xform"), OM_OBJ_RD);
	if OMis_null_obj(xform_id)
		printf("Error searching for xform_id\n");

	xlate_id = OMfind_subobj(xform_id, OMstr_to_name("xlate"), OM_OBJ_RD);
	if OMis_null_obj(xlate_id)
		printf("Error searching for xlate_id\n");

	stat = FLDget_array_float (xform_id, "xlate", &xlate, &xlsize, OM_GET_ARRAY_RD);

#ifdef DEBUG
	printf("\nthe translate is [%f %f %f]\n", xlate[0], xlate[1], xlate[2]);
#endif
	if (stat <1) {
		printf("Error searching for xlate array\n");
	}


	stat = FLDget_array_float (out, "plane_norm", &norm_x, &size, OM_GET_ARRAY_RD);
	if (stat < 1) {
		if (FLDadd_float_array(out, "plane_norm", 3) != 1) {
			ERR_RETURN("cannot create normal array");
		}
		/*
		 * Prevent this array from being saved.
		 * Eventually we might be able to determine this
		 * automatically but for now, it must be set by hand.
		 */
		pln_id = OMfind_subobj(out, OMstr_to_name("plane_norm"),
					OM_OBJ_RW);
		OMset_obj_atts(pln_id, OM_atts_nosave);
		norm_x = NULL;
	}

	if (FLDget_nnodes(in, &nnodes) != 1) {
		ERR_RETURN("cannot get nnodes");
	}
	if (FLDget_nspace(in, &nspace) != 1) {
		ERR_RETURN("cannot get nnodes");
	}
	if (FLDget_coord(in, &coord, &size, OM_GET_ARRAY_RD) != 1) {
		ERR_RETURN("cannot get coordinates");
	}

	if (FLDset_nnodes (out, nnodes) != 1) {
		ERR_RETURN("Error setting nnodes");
	}
	type = OM_TYPE_FLOAT;
	if (FLDget_node_data(out, 0, &type, (char **)&node_dist,
				      &size, OM_GET_ARRAY_WR) != 1) {
			ERR_RETURN("cannot get node data");
	}

	cross[0]=cross[1]=cross[2]=0.0; /* pre-initialize */

	/*
	 * take inverse, to map projection from plane to mesh
	 * in the correct relationship
	 */
        MATmat4_inverse( xfm_inv, xfm );

	/*
	 * Loop over all nodes of field, assigning new node data
	 */
	for (i=0; i<nnodes; i++) {
		/*
		 * get a local copy of each coordinate of input mesh
		 */
		memcpy(cross, coord+nspace*i, nspace*sizeof(float));

		/*
		 * Transform the point against the Field xform
		 */
		if (in_xfm)
			MATvec3_mat4_multiply(cross, (Matr4 *)in_xfm);

		/*
		 * apply plane xform onto point also
		 */
		MATvec3_mat4_multiply(cross, (Matr4 *)xfm_inv);

#ifdef DEBUG
		if (i<20)
		printf("\nthe cross %d is [%f %f %f]\n",i,cross[0], cross[1], cross[2]);
#endif
		/*
		 * compute distance to Z axis, 2D circular
		 * distance is same as projection onto line
		 */
	 	node_dist[i] = sqrt( cross[0]*cross[0] + cross[1]*cross[1] );

	}
	if (FLDset_array_float (out, "plane_norm",
		3, norm_xfm, OM_SET_ARRAY_COPY) != 1) {
		ERR_RETURN("cannot set normal array");
	}

	ARRfree((char *)coord);
	ARRfree((char *)node_dist);
	ARRfree (xlate);
	if (norm_x)
    		ARRfree((char *)norm_x);

	return(1);

} /* end of module */


/*
 * Custom AVS/Express Project Version - Plane Projection
 *
 * Author: Ian Curington
 * 24 November 97
 * derived from modules/pln_dist.c
 *
 */
/*
            Copyright (c) 1994 by
            Advanced Visual Systems Inc.
            All Rights Reserved
    
    This software comprises unpublished confidential information of
    Advanced Visual Systems Inc. and may not be used, copied or made
    available to anyone, except in accordance with the license
    under which it is furnished.
    
    This file is under sccs control at AVS in:
    $Source: /cvs/repository/express/modules/pln_dist.c,v $
*/

#include <avs/om.h>
#include <avs/util.h>
#include <avs/err.h>
#include <avs/fld.h>
#include <avs/math.h>
#include <avs/arr.h>
#include <avs/om_att.h>
#include <avs/dv_util.h>

#define ERR_RETURN(A) {ERRerror("plane_proj",1,ERR_ORIG, A); return(0);}
#define EPS  0.0001	


/* ========================================
 * Module Entry Point, Resolve OM Paramters
 */
int plane_proj_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
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
    if (!plane_proj_process(in, plane, out, &dist)) {
        return(0);
    }

    if (OMset_name_real_val (elem_id, OMstr_to_name("dist"), (double)dist) != 1) {
        ERR_RETURN("cannot set dist value");
    }
    return(1);
}


/*
 * Process algorithm for module
 */
int plane_proj_process (in, plane, out, dist)
    OMobj_id  in, plane, out;
    float      *dist;
{
    int    nnodes, type, size, recomp;
    int    nspace, plane_nspace, npts, stat, i;
    float  *coord=NULL, *pnts=NULL;
    float  bounds[3], norm[3], norm_xfm[3], *norm_x;
    float  xfm[4][4], *in_xfm, in_xform[4][4], xfm_inv[4][4];
    float  norm_xfm_tx[3], norm_xfm_ty[3];
    float  cross[3], l;
    float  *node_dist=NULL, plane_dist;
    int    in_seq, out_seq;
    OMobj_id pln_id;

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
    /* based on IN mesh */
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
    
    for (i=0; i<plane_nspace; i++) /* make local copy */
        bounds[i] = pnts[i];
    
    if (pnts!=NULL)
        ARRfree(pnts); /* release OM original */
    
    /*------------------*/
    /* Resolve actual   */
    /* Plane eq normal  */
    /* with respect to  */
    /* transformed mesh */
    /*------------------*/


    for (; i<3; i++)  /* if 2D, zero pad Z */
        bounds[i] = 0.0;

        /* define Z direction vector */
    norm[0] = 0.0; norm[1] = 0.0; norm[2] = 1.0;

        /* move plane coord point by the plane equation transform */
    MATvec3_mat4_multiply(bounds, xfm);

        /* transform normal vector by in plane rotation matrix */
    MATxform_vecs(1, (Matr3 *)norm, xfm, (Matr3 *)norm_xfm);

        /* normalize length of vector with scalar float length l */
    l = VEC_LEN(norm_xfm);
    VEC_NORMALIZE(norm_xfm,l);
    
        /* compute d=distance scalar for plane equation */
    plane_dist = VEC_DOT(bounds, norm_xfm);

    *dist = plane_dist; /* output var result */


    MATmat4_inverse( xfm_inv, xfm );

    /*-------------------*/
    /* Decide to Compute */
    /* or not!           */
    /*-------------------*/
        /* 
         * create a local temporary storage area for the plane normal coeff. 
         * by dynamically creating the array and saving the normal into it.
         */
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
        pln_id = OMfind_subobj(out, OMstr_to_name("plane_norm"), OM_OBJ_RW);
        OMset_obj_atts(pln_id, OM_atts_nosave);
        norm_x = NULL;
    }


    /*--------------------------------------------------*/
    /* Get Input Mesh and output node sequence numbers  */
    /* Check which one changed last                     */
    /*--------------------------------------------------*/
    out_seq = FLDget_subelem_seq(out, "Node_Data");
    in_seq = FLDget_subelem_seq(in, "Mesh");
    if (in_seq > out_seq) 
        recomp = 1;  /* new mesh has arrived */
    else {
        if (stat < 1) 
            recomp = 1; /* old mesh, but 1st time, no history */
        else {
                        /*
                         * The plane equation has moved, most likely,
                         * so check if the new angle is different enough
                         * to recompute, if very close to old, then ignore
                         * cross product is = cosine of angle between vectors
                         */
            VEC_CROSS(cross, norm_xfm, norm_x);
            l = VEC_LEN(cross);
            if (l < EPS)
                recomp = 0; /* similar to last time, ignore */
            else
                recomp = 1; /* ok, big enough move, go ahead */
        }
    }


    /*------------------*/
    /* Project 3D dist  */
    /* to new node data */
    /*------------------*/
    if (recomp) {
        if (FLDget_nnodes(in, &nnodes) != 1) {      /* get length of input list */
            ERR_RETURN("cannot get nnodes");
        }
        if (FLDget_nspace(in, &nspace) != 1) {      /* 2D or 3D? */
            ERR_RETURN("cannot get nnodes");
        }
        if (FLDget_coord(in, &coord, &size, OM_GET_ARRAY_RD) != 1) {  /* get COORDS */
            ERR_RETURN("cannot get coordinates");
        }

        if (FLDset_nnodes (out, nnodes) != 1) {     /* set OUTPUT nnodes */
            ERR_RETURN("Error setting nnodes");
        }
        type = OM_TYPE_FLOAT;
        if (FLDget_node_data(out, 0, &type, (char **)&node_dist, &size, OM_GET_ARRAY_WR) != 1) {
            ERR_RETURN("cannot get node data");
        }

            /*-------------------------*/
            /* Compute x-y-Z Rot func */
            /* of each point on mesh   */
            /* to a transformed axis   */
            /* assume veclen=3         */
            /*-------------------------*/
        cross[0]=cross[1]=cross[2]=0.0;
        for (i=0; i<nnodes; i++) {
            memcpy(cross, coord+nspace*i, nspace*sizeof(float));
            if (in_xfm)
                MATvec3_mat4_multiply(cross, (Matr4 *)in_xfm);

            MATvec3_mat4_multiply(cross, (Matr4 *)xfm_inv);

            node_dist[i*3+0] = cross[0];
            node_dist[i*3+1] = cross[1];
            node_dist[i*3+2] = cross[2];
        }

                /*
                 * load plane normal back into local cache for next time
                 */
        if (FLDset_array_float (out, "plane_norm", 3, norm_xfm, OM_SET_ARRAY_COPY) != 1) {
            ERR_RETURN("cannot set normal array");
        }

        if (coord!=NULL)
            ARRfree((char *)coord);
        if (node_dist!=NULL)
            ARRfree((char *)node_dist);
    }


    if (norm_x!=NULL)
        ARRfree((char *)norm_x);

    return(1);
}


/*
   Xedge Module - external edges special version for silhouette edge conditions

			Copyright (c) 1994 / 1999 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
	This file is under sccs control at AVS in:
	$Source: /net/build/cvs/repository/express/modules/ext_edge.c,v $

        This source file contains calls to other internal AVS systems.
        The exposure of these here does not imply support, documentation,
        or user accessability to these systems.

*/
#include <stdio.h>
#include <avs/util.h>
#include <avs/err.h>
#include <avs/db.h>
#include <avs/fld.h>
#include <avs/arr.h>
#include <avs/port.h>
#include <avs/dtype.h>
#include <avs/math.h>

#define STATUS_REPORT   256

#define ERR_RETURN(A) {ERRerror("xedge",1,ERR_ORIG, A); return(0);}

#define MAX_CELL_SETS 256

#define ALLOC_BLOCK_SIZE 1000
	
#define XSIGN(a) ((a) >= 0.0 ? 1 : -1)

static int corner_nnodes[]={0,1,2,3,4,0,3,0,4};

typedef struct _FLD_edge_table {
	int   *h_table;
	int   *link;
	int   *max_n;
	float *norm;
	int    *free_locs;
	int    free_size;
	int    free_count;
	int    edge_count;
	int    h_size;
	int    h_loc;
	int    h_block;
} FLD_edge_table;

static   FLD_edge_table *create_edge_table PTARGS((int nnodes, int block));
static int delete_edge_table PTARGS((FLD_edge_table *h));

/*
 * Module Entry Point - V wrappers link to this via "omethod"
 *   updated from the old "method" single argument style.
 */

int xedge_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
{
 
	OMobj_id in, out, angle_id, xtest_id, ytest_id, ztest_id;
	double   angle;
        int xtest, ytest, ztest;

	in = OMfind_subobj(elem_id, OMstr_to_name("in"), OM_OBJ_RD);
	out = OMfind_subobj(elem_id, OMstr_to_name("out_field"), OM_OBJ_RW);

	angle_id = OMfind_subobj(elem_id, OMstr_to_name("angle"), OM_OBJ_RD);
	OMget_real_val(angle_id, &angle);

	xtest_id = OMfind_subobj(elem_id, OMstr_to_name("xtest"), OM_OBJ_RD);
	OMget_int_val(xtest_id, &xtest);

	ytest_id = OMfind_subobj(elem_id, OMstr_to_name("ytest"), OM_OBJ_RD);
	OMget_int_val(ytest_id, &ytest);

	ztest_id = OMfind_subobj(elem_id, OMstr_to_name("ztest"), OM_OBJ_RD);
	OMget_int_val(ztest_id, &ztest);

	if (FUNCxedge(in, (float)angle, xtest, ytest, ztest, out)) {
		return(1);
	}
	else return(0);
}

/*
 * arguments resolved, now process contents of arguments
 */

int FUNCxedge (in, angle, xtest, ytest, ztest, out)
    OMobj_id  in, out;
    float      angle;
    int xtest, ytest, ztest;
{
	int   cs, os, ns, nsets, ncells, nnodes, cell_nnodes, cell_corner_nnodes, stat;
	int   nspace, ndim, size, out_edges;
	int   *node_list;
	int   nfaces, *face_nnodes, *face_corner_nnodes, *face_node_list, *out_list;
	int   out_nsets, out_ncells[MAX_CELL_SETS], out_cell_nnodes[MAX_CELL_SETS], 
              *out_cells[MAX_CELL_SETS],*out_lists[MAX_CELL_SETS], out_list_sizes[MAX_CELL_SETS];
	float *xyz;

	OMobj_id   cell_set;
	/* 
	 * clean the output
	 */
	if (FLDset_nnodes(out, 0) != 1) {
		ERR_RETURN("cannot get nnodes");
	}
	if (FLDset_ncell_sets (out, 0) != 1) {
		ERR_RETURN("Error setting ncell_sets");
	}
	FLDset_node_data_ncomp (out, 0);

	if (FLDget_nnodes(in, &nnodes) != 1 || nnodes == 0) {
		/* 
		 * this could be a case when module disconnected 
		 * clean the output and return
		 */
		return(0);
	}
	if (FLDget_nspace(in, &nspace) != 1) {
		ERR_RETURN("cannot get nnodes");
	}
	if (FLDget_coord(in, &xyz, &size, OM_GET_ARRAY_RD) != 1) {
		ERR_RETURN("cannot get coordinates");
	}

	/*--------------------------------------*/
	/*  Make out to be the same as in       */
	/*--------------------------------------*/
	if (FLDcopy_subelem_ref(out, in) != 1) {
		ERR_RETURN("Error setting field reference");
	}

	/*----------------------------------------*/
	/*   REMOVE ALL 3D and 2D cells from out  */
	/*----------------------------------------*/
	if (FLDget_ncell_sets(out, &nsets) != 1) {
		ERR_RETURN("cannot get nsets");
	}
	for (cs=nsets-1; cs>=0; cs--) {
		if (FLDget_cell_set(out, cs, &cell_set) != 1) {
			ERR_RETURN("cannot get cell set");
		}
		if (FLDget_cell_ndim(cell_set, &ndim) != 1) {
			ERR_RETURN("cannot get ndim");
		}

		if (ndim > 1) {
			if (FLDdel_cell_set(out, cell_set) != 1) {
				ERR_RETURN("cannot delete cell");
			}
		}
	}
	/*-------------------------------------*/
	/*  PROCESS all 3D and 2D cells in in  */
	/*-------------------------------------*/
	if (FLDget_ncell_sets(in, &nsets) != 1) {
		ERR_RETURN("cannot get nsets");
	}
	if (FLDget_ncell_sets(out, &ns) != 1) {
		ERR_RETURN("cannot get nsets");
	}
	for (cs=0; cs<nsets; cs++) {
		if (FLDget_cell_set(in, cs, &cell_set) != 1) {
			ERR_RETURN("cannot get cell set");
		}
		if (FLDget_cell_ndim(cell_set, &ndim) != 1) {
			ERR_RETURN("cannot get ndim");
		}
		if (ndim < 2)
			continue;

		if (FLDget_ncells(cell_set, &ncells) != 1) {
			ERR_RETURN("cannot get ncells");
		}
		
		if (FLDget_cell_set_nnodes(cell_set,  &cell_nnodes) != 1) {
			ERR_RETURN("cannot get cell nnodes");
		}
		if (FLDget_cell_corner_nnodes(cell_set,  &cell_corner_nnodes) != 1) {
			ERR_RETURN("cannot get cell nnodes");
		}
		
		if (FLDget_node_connect(cell_set, &node_list, 
					    &size, OM_GET_ARRAY_RD) != 1) {
			ERR_RETURN("cannot get cell connectivity");
		}
		if (ndim < 3) {
			OMpush_status_range((int)(cs*100/nsets),
					    (int)((cs+1)*100/nsets));

			stat = UTILxedges (nnodes, nspace, xyz, ncells, 
						   cell_nnodes, cell_corner_nnodes,
						   node_list, 
						   &out_edges, &out_list, angle,
							xtest, ytest, ztest);
			
			OMpop_status_range();
			if (stat != 1) {
				ARRfree((char *)node_list);
				if (FLDset_ncell_sets (out, 0) != 1) {
					ERR_RETURN("Error setting ncell_sets");
				}
				ERR_RETURN("edges are not extracted");
			}

			if (out_edges == 0) 
				continue;

			if (FLDadd_cell_set(out, "Line") != 1) {
				ERR_RETURN("Error setting cell type");
			}
			if (FLDget_cell_set(out, ns, &cell_set) != 1) {
				ERR_RETURN("Error getting cell set");
			}
		
			if (FLDset_ncells(cell_set, out_edges) != 1) {
				ERR_RETURN("Error setting ncells");
			}
			
			if (FLDset_node_connect(cell_set, out_list, 2*out_edges, 
						    OM_SET_ARRAY_FREE) != 1) {
				ERR_RETURN("Error setting cell connect list");
			}
			ns++;
			ARRfree(node_list);
			continue;
		}
		
		/*------------------------------------------*/
		/*   3D case: GET EXTERNAL faces            */
		/*------------------------------------------*/
		OMpush_status_range((int)(cs*100/nsets),
				    (int)((cs+1)*100/nsets));

		OMpush_status_range(0, 50);

		if (FLDget_cell_faces(cell_set, &nfaces, &face_nnodes, 
				      &face_corner_nnodes, &face_node_list, &size) != 1) {
			ERR_RETURN("cannot get cell nfaces");
		}
		
		stat = UTILexternal_faces(nnodes, ncells, cell_nnodes, node_list, 
					  nfaces, face_nnodes, face_corner_nnodes, 
					  face_node_list,
					  MAX_CELL_SETS, &out_nsets, out_ncells, 
					  out_cell_nnodes, out_cells,
					  out_lists, out_list_sizes);
		OMpop_status_range();

		if (stat != 1) {
			OMpop_status_range();
			ARRfree((char *)node_list);
			ARRfree((char *)face_nnodes);
			ARRfree((char *)face_corner_nnodes);
			ARRfree((char *)face_node_list);
			if (FLDset_ncell_sets (out, 0) != 1) {
				ERR_RETURN("Error setting ncell_sets");
			}
			ERR_RETURN("faces are not extracted");
		}
		/*-----------------------------------*/
		/*  EXTRACT EDGES                    */
		/*-----------------------------------*/
		OMpush_status_range(50, 100);

		for (os=0; os<out_nsets; os++) {
			if (out_cells[os]) {
				ARRfree((char *)out_cells[os]);
			}
			if (out_list_sizes[os] == 0 || out_ncells[os] == 0) {
				if (out_lists[os]) {
					ARRfree((char *)out_lists[os]);
				}
				continue;
			}
			
			OMpush_status_range((int)(os*100/out_nsets),
				    (int)((os+1)*100/out_nsets));

			stat = UTILxedges (nnodes, nspace, xyz, 
						   out_ncells[os], out_cell_nnodes[os],
						   corner_nnodes[out_cell_nnodes[os]],
						   out_lists[os],
						   &out_edges, &out_list, angle,
							xtest, ytest, ztest);
			OMpop_status_range();
			if (stat != 1) {
				OMpop_status_range();
				OMpop_status_range();
				ARRfree((char *)node_list);
				ARRfree((char *)face_nnodes);
				ARRfree((char *)face_corner_nnodes);
				ARRfree((char *)face_node_list);
				for (cs=os; cs<out_nsets; cs++) {
					if (out_cells[cs]) {
						ARRfree((char *)out_cells[cs]);
					}
					if (out_lists[cs]) {
						ARRfree((char *)out_lists[cs]);
					}
					
					if (FLDset_ncell_sets (out, 0) != 1) {
						ERR_RETURN("Error setting ncell_sets");
					}
				}
				ERR_RETURN("edges are not extracted");
			}
			if (out_lists[os]) {
				ARRfree((char *)out_lists[os]);
			}

			if (out_edges == 0) 
				continue;

			if (FLDadd_cell_set(out, "Line") != 1) {
				ERR_RETURN("Error setting cell type");
			}
			if (FLDget_cell_set(out, ns++, &cell_set) != 1) {
				ERR_RETURN("Error getting cell set");
			}

			if (FLDset_ncells(cell_set, out_edges) != 1) {
				ERR_RETURN("Error setting ncells");
			}
			
			if (FLDset_node_connect(cell_set, out_list, 2*out_edges, OM_SET_ARRAY_FREE) != 1) {
				ERR_RETURN("Error setting cell connect list");
			}
		}
		OMpop_status_range();
		OMpop_status_range();
		ARRfree((char *)node_list);
		ARRfree((char *)face_nnodes);
		ARRfree((char *)face_corner_nnodes);
		ARRfree((char *)face_node_list);
	}
	ARRfree((char *)xyz);
	return(1);
}


/*
 * Low level functional routines to extract edge lines
 */

int UTILxedges (nnodes, nspace, xyz, nfaces, face_nnodes, face_corner_nnodes, node_list, 
			out_edges, out_list, angle, xtest, ytest, ztest)
    int     nspace, nnodes, nfaces, face_nnodes, face_corner_nnodes;
    int     *node_list, *out_edges, **out_list;
    float   *xyz, angle;  
    int     xtest, ytest, ztest;
{
	FLD_edge_table *h;

	int   *Hh_table;
	int   *Hh_link;
	int   *Hmax_n;
	int    *Hfree_locs;
	float  *Hnorm;
	int  per_count, interrupt;
	float per;

	
	int    face, ind0, ind1, ind2, v1, v2, *flist, i, n, found, min_v, max_v, 
	       edge, ptr, prev_ptr, block;
	float  norm[3], vec1[3], vec2[3], alf;
	
	block = ALLOC_BLOCK_SIZE;
	if (nfaces > 10000)
		block = 2*ALLOC_BLOCK_SIZE;
	if (nfaces > 100000)
		block = 4*ALLOC_BLOCK_SIZE;

	h = create_edge_table(nnodes, block);
	if (!h)
		return(0);

	Hh_table = h->h_table;
	Hh_link = h->link;
	Hmax_n = h->max_n;
	Hnorm = h->norm;
	Hfree_locs = h->free_locs;

	MATbuild_sqrt_table();
	/*--------------------------------------------*/
	/*  decompose each face into a set of edges   */
	/*   and put them into the edge_table.        */
	/*--------------------------------------------*/
	
	for (i=0; i<3; i++) {
		vec1[i] = 0.0;
		vec2[i] = 0.0;
	}

	per_count =0;
	per = 100.0/nfaces;

	for (flist=node_list, face = 0; face < nfaces; face++) {
		if (per_count == STATUS_REPORT) {
                        OMstatus_check((int)(face*per),NULL,&interrupt);
                        if (interrupt) {
				delete_edge_table(h);
				return(0);
                        }
                        per_count =0;
                 }
                 else
                        per_count++;

		/*---------------------------------*/
		/*  Calculate normal to a face     */
		/*---------------------------------*/
		ind0 = flist[0]*nspace;
		ind1 = flist[1]*nspace;
		ind2 = flist[face_corner_nnodes-1]*nspace;
		for (i=0; i<nspace; i++) {
			vec1[i] = xyz[ind1+i]-xyz[ind0+i];
			vec2[i] = xyz[ind2+i]-xyz[ind0+i];
		}
		
		VEC_CROSS(norm, vec1, vec2);
		VEC_UNITIZE(norm, 10.e-30);

		/*------------------------------*/
		/*  break the face into edges.  */
		/*------------------------------*/
		for (edge = 0; edge < face_corner_nnodes; edge++) {
			if (edge < (face_corner_nnodes -1)) {
				v1 = flist[edge];
				v2 = flist[edge+1];
			}
			else {
				v1 = flist[edge];
				v2 = flist[0];
			}
			if (v1 < v2) {
				min_v = v1;
				max_v = v2;
			}
			else {
				min_v = v2;
				max_v = v1;
			}			
			ptr = prev_ptr = Hh_table[min_v];
			found = 0;
			/*--------------------------------------------------------*/
			/*  if there are no entries at the hash location or there */
			/*  is no match between the current edges left and right  */
		        /*  vertices with one of the entries then add the edge.   */
			/*--------------------------------------------------------*/
			while (ptr != 0) {
				if (Hmax_n[ptr] == max_v) {  /*  match found.  */
					/*
					 * alf = angle between faces, by
					 * taking dot product of the two face normals
					 * which gets you cosine of the angle.
					 * finally, acos to retrieve actual angle
					 */
					alf = Hnorm[ptr * 3] * norm[0] +
					      Hnorm[ptr * 3 + 1] * norm[1] +
					      Hnorm[ptr * 3 + 2] * norm[2];
					if (alf > 1.0) alf = 1.0;
					if (alf < -1.0) alf = -1.0;
					alf = acos(alf);
					found = 1;

					/*
					 * special mode to check if curvature crosses
					 * an axis, if so, keep the line anyway
					 * - I. Curington, December 1999
					 */
					if (xtest &&(XSIGN(Hnorm[ptr*3]) != XSIGN(norm[0])))
						alf = angle + 1.0; /* force condition */
					else if (ytest &&(XSIGN(Hnorm[ptr*3+1]) != XSIGN(norm[1])))
						alf = angle + 1.0; /* force condition */
					else if (ztest &&(XSIGN(Hnorm[ptr*3+2]) != XSIGN(norm[2])))
						alf = angle + 1.0; /* force condition */

					/*
					 * if the angle is less than parameter threshold, then
					 * remove the edge from the table
					 */
					if (fabs(alf) < angle) {
						if (h->free_count >= h->free_size) {
							h->free_size += ALLOC_BLOCK_SIZE ;
							h->free_locs = (int *)realloc(h->free_locs, 
							     sizeof(int) * (h->free_size));
							if (!h->free_locs) {
								delete_edge_table(h);
								return(0);
							}
							Hfree_locs = h->free_locs;
						}
						Hfree_locs[h->free_count++] = ptr;
						if (ptr == prev_ptr)
							Hh_table[min_v] = Hh_link[ptr]; 
						else
							Hh_link[prev_ptr] = Hh_link[ptr];
						h->edge_count--;
					}
					ptr = 0;
				} 
				else {
					prev_ptr = ptr;
					ptr = Hh_link[ptr];
				}
			}
			
			/*  if there was no match found then get a new ptr 
			    either from the free list or by incrementing.   */
			if (!found) {
				if (h->free_count) 
					ptr = Hfree_locs[--(h->free_count)];
				else
					ptr = h->h_loc++; 
				
				/*  grow those arrays.  */
				if (ptr >= h->h_size) {
					int prev_size = ptr;
					h->h_size = ptr + ALLOC_BLOCK_SIZE;
					
					h->link = (int *)realloc(h->link, sizeof(int) * (h->h_size));
					h->max_n = (int *)realloc(h->max_n, sizeof(int) * (h->h_size));
					h->norm = (float *)realloc(h->norm,3*sizeof(float) * (h->h_size));
					if (!h->link || !h->max_n || !h->norm) {
						delete_edge_table(h);
						return(0);
					}
					memset (h->link+prev_size, 0, sizeof(int) * ALLOC_BLOCK_SIZE);
					Hh_link = h->link;
					Hmax_n = h->max_n;
					Hnorm = h->norm;
				}
				/*  store the new edge.  */
				Hh_link[ptr] = Hh_table[min_v];
				Hmax_n[ptr] = max_v;
				Hh_table[min_v] = ptr;
				Hnorm[ptr * 3] = norm[0];
				Hnorm[ptr * 3 + 1] = norm[1];
				Hnorm[ptr * 3 + 2] = norm[2];
				h->edge_count++;
			}
		}
		flist += face_nnodes;
	}

	/*--------------------------------------------*/
	/* Create exterior line list                  */
	/*--------------------------------------------*/
	*out_edges =  h->edge_count;
	if ((*out_edges)) {
		*out_list = (int *)ARRalloc(NULL, DTYPE_INT, 2 * (*out_edges), NULL);
		for (n=0,i = 0; i < nnodes; i++) {
			ptr = Hh_table[i];
			while (ptr != 0) {
				(*out_list)[n++] = i;
				(*out_list)[n++] = Hmax_n[ptr];
				ptr = Hh_link[ptr];
			}      
		}
	}
	delete_edge_table(h);
	return(1);
}

static   FLD_edge_table *create_edge_table(nnodes, block)
    int nnodes, block;
{
	FLD_edge_table *h;

	h = (FLD_edge_table *)malloc(sizeof(FLD_edge_table));

	h->h_table = (int *)malloc(sizeof(int) * (nnodes + 1));
	h->link = (int *)malloc(sizeof(int) * (block));
	h->free_locs = (int *)malloc(sizeof(int) * (block));
	h->max_n = (int *)malloc(sizeof(int) * (block));
	h->norm = (float *)malloc(3*sizeof(float) * (block));
	if (!h->h_table || !h->link || !h->free_locs || !h->max_n || !h->norm) {
		delete_edge_table(h);
		return((FLD_edge_table *)0);
	}
	h->edge_count = 0;
	h->free_count = 0;
	h->free_size = block;
	h->h_loc = 1;
	h->h_size = block;
	h->h_block = block;
	memset (h->link, 0, sizeof(int) * (h->h_block));
	memset (h->h_table, 0, sizeof(int) * (nnodes+1));
	return((FLD_edge_table *)h);
}

static int delete_edge_table(h)
    FLD_edge_table *h;
{
	if (h->h_table)
		free (h->h_table); 
	if (h->link)
		free (h->link);
	if (h->free_locs)
		free (h->free_locs); 
	if (h->max_n)
		free (h->max_n);
	if (h->norm)
		free (h->norm);
	free(h);
	return(1);
}
/* end of file */

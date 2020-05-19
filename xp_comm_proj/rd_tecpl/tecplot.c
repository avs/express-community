/*******************************************************************
**	Read Tecplot-Data-Files
**
**	Copyright (c) 1997 DLR
********************************************************************
**	Created:	01/97, 08/97, 11/97
**	by:		I.Hallstein
**
**	DLR, WT-DV
**	Linder Hoehe
**	D-51147 Cologne
**	Germany
*******************************************************************/

#define XP_WIDE_API

#include "gen.h"
#include <avs/avs.h>
#include <avs/om.h>
#include <avs/fld.h>
#include "rdtecpl.h"


/*******************************************************************
**
**	read Tecplot-data (acsii- and binary-file-format)
**
*******************************************************************/
int
ReadTecplotData (OMobj_id Read_Tecplot_id, OMevent_mask event_mask, int seq_num)
{
    char  	*filename = NULL;
    OMobj_id	mb, elm, *fields;
    int         nspace, ndim, ncomp, veclen;
    int		dtype, stat;
    xp_long     nnodes, size, inode, dims[3];
    float	*coord, *node_data;
    float	v, min, max, rmin[20], rmax[20];
    xp_long	i, j, k, n;
    int		nblocks;
    TecBlock	*block;

    /*
    **	get input values (filename's value)
    */
    if (OMget_name_str_val (Read_Tecplot_id, OMstr_to_name ("filename"),
			    &filename, 0) != 1)
	filename = NULL;

    if (filename == NULL || strlen (filename) == 0)
	return 1;

    /*
    **	read data
    */
#ifdef DEBUG
	printf ("*** read Tecplot Datafile '%s' ***\n", filename);
#endif

    if (!ReadTecplot (Read_Tecplot_id, filename)) {
	ERRverror("",ERR_NO_HEADER | ERR_INFO,"In ReadTecplotData: Read-Error\n");
	return 0;
    }

    /*
    **	set data
    */
#ifdef DEBUG
	printf ("*** save Tecplot-data in multi-block-structure 'mb'***\n");
#endif
    mb	= OMfind_subobj (Read_Tecplot_id, OMstr_to_name ("mb"), OM_OBJ_RW);
    elm	= OMfind_subobj (mb, OMstr_to_name ("nblocks"), OM_OBJ_RW);
    nblocks	= tecMb.nBlocks;
    if (nblocks == 0)
	return 0;
    OMset_int_val (elm, nblocks);

    fields	= (OMobj_id*) malloc (nblocks * sizeof (OMobj_id));
    elm	= OMfind_subobj (mb, OMstr_to_name ("fields"), OM_OBJ_RW);
    for (i = 0; i < nblocks; i++) /* stat evtl. weglassen */
	if ((stat = OMget_array_val (elm, i, fields+i, OM_OBJ_RD)) != 1)
	    return (stat);

    nnodes	= 0;
    nspace	= 3;
    ndim	= 3;
    block	= tecMb.blocks;
    dims[0] = block->dim[0]; dims[1] = block->dim[1]; dims[2] = block->dim[2]; 
    for (i = 0; i < nblocks && block != NULL; i++) {

#ifdef DEBUG
	    printf ("--- set block %d ---\n", i);
#endif

	nnodes	= nnodes + dims[0] * dims[1] * dims[2];
	if (FLDset_nspace (fields[i], nspace) != 1) {
	    ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error setting nspace");
	    return 0;
	}
	if (FLDset_ndim (fields[i], ndim) != 1) {
	    ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error setting ndim");
	    return 0;
	}
	if (FLDset_dims (fields[i], dims) != 1) {
	    ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error setting dims");
	    return 0;
	}

	/*
	**	coordinates
	*/
	if (FLDget_coord (fields[i], (float**)&coord, &size, OM_GET_ARRAY_WR) != 1) {
	    ERRverror ("",ERR_NO_HEADER | ERR_INFO,
		       "In ReadTecplotData: Error setting coordinate array\n");
	    return 0;
	}
	n	= dims[0] * dims[1] * dims[2];
	if (block->nVar >= 3) {
	    for (j = 0; j < n; j++) {
		coord[j*nspace]		= block->val[0][j];
		coord[j*nspace+1]	= block->val[1][j];
		coord[j*nspace+2]	= block->val[2][j];
	    }
	}
	/* if nVar < 3 */
	else if (block->nVar == 2) {
	    for (j = 0; j < n; j++) {
		coord[j*nspace]		= block->val[0][j];
		coord[j*nspace+1]	= block->val[1][j];
		coord[j*nspace+2]	= 0;
	    }
	}
	else if (block->nVar == 1) {
	    for (j = 0; j < n; j++) {
		coord[j*nspace]		= block->val[0][j];
		coord[j*nspace+1]	= 0;
		coord[j*nspace+2]	= 0;
	    }
	}
	ARRfree ((char*)coord);

	/*
	**	values, minmax
	*/
	if (tecMb.nVar > 3) {
	    ncomp	= tecMb.nVar - 3;
	    veclen	= 1;
	    dtype	= DTYPE_FLOAT;

	    if (FLDset_node_data_ncomp (fields[i], ncomp) != 1) {
		ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error setting nnode_data");
		return 0;
	    }

	    for (j = 0; j < ncomp; j++) {
		if (FLDset_node_data_comp (fields[i], j, veclen,
					   tecMb.varNames[j+3], "") != 1) {
		    ERRverror ("",ERR_NO_HEADER | ERR_INFO,
			       "Error setting node data component");
		    return 0;
		}
		if (FLDget_node_data (fields[i], j, &dtype, (char**)&node_data, &size,
				      OM_GET_ARRAY_WR) != 1) {
		    ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error getting node data");
		    return 0;
		}

		inode	= 0;

		min = max = block->val[j+3][0];
		
		for (k = 0; k < block->nVal; k++) {
		  v = block->val[j+3][k];
		  node_data[inode++] = v;
		  /* compute are own min/max as the v code min array
		     gets overwritten for subsequent loads */
		  if(v<min) min = v;
		  else if(v>max) max = v;    
		}

		ARRfree ((char*) node_data);

		if (i == 0) {
		    rmin[j]	= min;
		    rmax[j]	= max;
		}
		else {
		    if (min < rmin[j])
			rmin[j]	= min;
		    if (max > rmax[j])
			rmax[j]	= max;
		}
	    }

	    if (FLDget_node_data_ncomp (fields[i], &ncomp) != 1) {
		ERRverror ("",ERR_NO_HEADER | ERR_INFO, "Error setting nnode_data");
		return 0;
	    }
	    for (j = 0; j < ncomp; j++) {
		if (FLDset_node_data_minmax (fields[i], j, (char*) &(rmin[j]),
					     (char*) &(rmax[j]),
					     DTYPE_FLOAT) != 1) {
		    ERRverror ("",ERR_NO_HEADER | ERR_INFO, 
			       "Error setting minmax data");
		    return 0;
		}
	    }
	}
	block	= block->next;
    }

    /* free input variables */
    if (filename)
	free (filename);

    /* free tecMb-structure */
    CleanTecMultiBlock (&tecMb);

    return (1);
}

/*
			Copyright (c) 2004 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

	This file is under Perforce control
	$Id: //depot/express/trunk/segm_edtr/segreg.cxx#1 $
*/

#include <avs/om.h>

#include "seggrow.h"
#include "gen_grow.h"

//#define DEBUG

int SegmentationEditor_SegmentationEditorMods_GrowRegion::update(OMevent_mask event_mask, int seq_num)
{
	// in (Node_Data read req notify)
	// dims (OMXint_array read req notify)
	// use_diagonals (OMXint read req notify)
	// and_precedence (OMXint read req notify)
	// incremental (OMXint_array read req notify)
	// delta (OMXdouble_array read req notify)
	// seed (OMXint_array read req notify)
	// combine_and_or (OMXint_array read req notify)
	// out_val (OMXint read req notify)
	// trigger (OMXint read req notify)
	// out_label (OMXstr read req notify)
	// out (Node_Data write)

	int delta_size;
	double *delta_arr;
	int seed_size;
	int grow_out_value;
	int grow_null_value=0;
	int *seed_arr;
	int pt[3]={0,0,0};
	int i,j;
	int comp=0;
	int *vecpos;	// 0 ... scalar ; 1 .. n  position in the vector
	int ndata,veclen=0;
	double *in_data;
	int grow_dims[3]={0,0,0};
	double *grow_delta;
	double *grow_increment;
	segment_grow_region * grow;
	short* s_in_node_data=NULL;
	int*  i_in_node_data=NULL;
	float* f_in_node_data=NULL;
	double* d_in_node_data=NULL;
	int	 in_data_comp;
	int	 in_data_size, in_data_type;
	int dims_size;
	int *dims_arr;
	int *and_or;
	int combine_and_or_size;
	int *combine_and_or_arr;
	int incremental_size;
	int *incremental_arr;
	int out_data_size;

	if (!(int)trigger) return 0;

	#ifdef DEBUG
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm starting method: SegmentationEditor_GrowRegion::update\n");
		fprintf(stderr, "SegmentationEditor_GrowRegion::update\n");
	#endif

	combine_and_or_arr = (int *)combine_and_or.ret_array_ptr(OM_GET_ARRAY_RD,&combine_and_or_size);
	if (combine_and_or_arr) {
		j=0;
		for (i=0; i<combine_and_or_size;i++) {
			if (combine_and_or_arr[i]) {
				j=1;
				break;
			}
		}
		if (!j) {
			ARRfree(combine_and_or_arr);
			return 1;
		}
	} else {
		return 1;
	}
	delta_arr = (double *)delta.ret_array_ptr(OM_GET_ARRAY_RD,&delta_size);
	if (!delta_arr) {
		ARRfree(combine_and_or_arr);
		return 1;
	}
	incremental_arr = (int *)incremental.ret_array_ptr(OM_GET_ARRAY_RD,&incremental_size);
	if (!incremental_arr) {
		ARRfree(combine_and_or_arr);
		ARRfree(delta_arr);
		return 1;
	}

	dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD,&dims_size);
	if (dims_arr) {
		/* Your code to set dims_arr goes here */
		seed_arr = (int *)seed.ret_array_ptr(OM_GET_ARRAY_RD,&seed_size);
		if (seed_arr) {
			if (seed_size != dims_size)
			{
				// seed point has different dimensions from Field
				ARRfree(dims_arr);
				ARRfree(seed_arr);
				ARRfree(combine_and_or_arr);
				ARRfree(delta_arr);
				ARRfree(incremental_arr);
				return (0);
			}
			for (i=0; i<dims_size;i++)
			{
				grow_dims[i]=dims_arr[i];
				pt[i]=seed_arr[i];
			}
			ARRfree(seed_arr);
		}
		ARRfree(dims_arr);
	}
	#ifdef DEBUG
		fprintf(stderr,"read dims_arr\n");
		fprintf(stderr,"pt: {%i,%i,%i}\n",pt[0],pt[1],pt[2]);
		fprintf(stderr,"dims: {%i,%i,%i}\n",grow_dims[0],grow_dims[1],grow_dims[2]);
	#endif

	ndata = 0;
	for (in_data_comp = 0; in_data_comp < in.nnode_data; in_data_comp++)
		if (combine_and_or_arr[in_data_comp])
			ndata += (int) in.node_data[in_data_comp].veclen;

	#ifdef DEBUG
		fprintf(stderr,"ndata: %i\n",ndata);
	#endif

	vecpos = new int[ndata];
	and_or = new int[ndata];
	grow_delta = new double[ndata];
	grow_increment = new double[ndata];
	in_data = new double[ndata*(int)in.nnodes];
	out.nnodes=(int)in.nnodes;

	for (in_data_comp = 0; in_data_comp < (int)in.nnode_data; in_data_comp++) {
		// transfer only components which are used for the region growing
		// combine_and_or_arr ... 0 unused
		// combine_and_or_arr ... 1 and
		// combine_and_or_arr ... 2 or
		if (!combine_and_or_arr[in_data_comp]) continue;

		// transpose and + or to 0 and 1
		// and_or[i] ... 0 and
		// and_or[i] ... 1 or
		and_or[comp] = (int)combine_and_or_arr[in_data_comp]-1;
		grow_delta[comp] = ((int)incremental_arr[in_data_comp])?0:(double)delta_arr[in_data_comp];
		grow_increment[comp] = ((int)incremental_arr[in_data_comp])?(double)delta_arr[in_data_comp]:0;

		char *in_node_data = (char *)in.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
		veclen = (int)in.node_data[in_data_comp].veclen;
		#ifdef DEBUG
			fprintf(stderr,"comp: %i\n",comp);
			fprintf(stderr,"grow_delta[%i]: %f\n",comp,grow_delta[comp]);
			fprintf(stderr,"in[%i].veclen: %i\n",in_data_comp,veclen);
			fprintf(stderr,"in_data_type: %i\n",in_data_type);
			fprintf(stderr,"and_or[%i]: %i\n",comp,and_or[comp]);
		#endif
		for (i = 0; i < veclen; i++)
		{
			vecpos[comp] = (veclen>1)?(i+1):0;
			switch (in_data_type)
			{
				case DTYPE_CHAR:
				case DTYPE_BYTE:
				{
					for (j=0;j<(in_data_size/veclen);j++)
						in_data[j*ndata+comp] = (double) in_node_data [j*veclen+i];
					break;
				}
				case DTYPE_SHORT:
				{
					s_in_node_data = (short*) in_node_data;
					for (j=0;j<(in_data_size/veclen);j++)
						in_data[j*ndata+comp] = (double) s_in_node_data [j*veclen+i];
					break;
				}
				case DTYPE_INT:
				{
					i_in_node_data = (int*) in_node_data;
					for (j=0;j<(in_data_size/veclen);j++)
						in_data[j*ndata+comp] = (double) s_in_node_data [j*veclen+i];
					break;
				}
				case DTYPE_FLOAT:
				{
					f_in_node_data = (float*) in_node_data;
					for (j=0;j<(in_data_size/veclen);j++)
						in_data[j*ndata+comp] = (double) f_in_node_data [j*veclen+i];
					break;
				}
				case DTYPE_DOUBLE:
				{
					d_in_node_data = (double*) in_node_data;
					for (j=0;j<(in_data_size/veclen);j++)
						in_data[j*ndata+comp] = (double) d_in_node_data [j*veclen+i];
					break;
				}
			}
			comp++;
		}
		#ifdef DEBUG
			fprintf(stderr,"read in %i components\n",comp);
		#endif
		if (in_node_data) {
			ARRfree(in_node_data);
		}
	}
	if (delta_arr) {
		ARRfree(delta_arr);
	}
	if (incremental_arr) {
		ARRfree(incremental_arr);
	}
	if (combine_and_or_arr) {
		ARRfree(combine_and_or_arr);
	}

	// at this point the node_data in in_data is double, whatever it was before

	if ((int)invert_region) {
		grow_null_value=(int)out_val;
		grow_out_value=(grow_null_value)?0:-1;
	}
	else {
		grow_out_value=(int)out_val;
		grow_null_value=(grow_out_value)?0:-1;
	}

	grow = new segment_grow_region(comp,grow_dims,in_data);
	grow->set_delta(grow_delta);
	grow->set_increment(grow_increment);
	grow->set_and_or(and_or);
	grow->set_out_value(grow_out_value,grow_null_value);
	grow->set_diagonals((int)use_diagonals);
	grow->set_and_precedence((int)and_precedence);
	grow->set_min_max(pt[0],pt[1],pt[2]);

	grow->grow(pt[0],pt[1],pt[2]);

	#ifdef DEBUG
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor_GrowRegion::update\n");
	#endif

	out.nnode_data=1;
	out.node_data[0].veclen=1;
	out.node_data[0].labels=out_label;
	out.node_data[0].null_value = ((int)invert_region)?grow_out_value:grow_null_value;

	if ((int) set_null_value)
		out.node_data[0].null_flag = 1;
	else
		out.node_data[0].null_flag = 0;

	unsigned char* out_node_data = (unsigned char*)out.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_WR, DTYPE_BYTE,&out_data_size);

	grow->get_out_data(&out_node_data);

	delete grow;
	delete[] vecpos;
	delete[] and_or;
	delete[] grow_delta;
	delete[] grow_increment;
	delete[] in_data;

	if (out_node_data)
		ARRfree(out_node_data);

#ifdef DEBUG
		fprintf(stderr,"exit SegmentationEditor_GrowRegion::update\n");
#endif

	// return 1 for success
	return(1);
}


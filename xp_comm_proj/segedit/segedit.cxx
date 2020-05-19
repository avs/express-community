/*
			Copyright (c) 2004 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

	This file is under Perforce control
	$Id: //depot/express/trunk/segm_edtr/segedit.cxx#1 $
*/

#include <limits.h>

#include <avs/om.h>

#include "gen_seg.h"
#include "seggrow.h"

//#define DEBUG

#define CMD_ADD	  0
#define CMD_APPLY 1

#define REG_ADD	 0
#define REG_REPL 1

#define CMD_CLEAR_VOLUME  0
#define CMD_CLEAR_MASK_3D 1
#define CMD_CLEAR_MASK_2D 2

#define CLEAR_VOLUME_DATA(T,T2,T3) \
{ \
	T2 *out_node_data = (T2 *)out_comp.values.ret_typed_array_ptr(OM_GET_ARRAY_RW, T3, &out_data_size); \
	if (!out_node_data) \
	{ \
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"SegmentationEditorCore::OnInstance\nCan't get out.node_data[%i]\n",data_comp); \
		if (in_field_node_data) \
			{ ARRfree(in_field_node_data); in_field_node_data=NULL;} \
		return 0; \
	} \
	out_comp.null_value=null_value; \
	out_comp.null_flag=1; \
	for (i=0; i<out_data_size; i++) \
		out_node_data[i]=null_value; \
	if (out_node_data) \
		{ ARRfree(out_node_data); out_node_data=NULL;} \
}

#define APPLY_MASK_DATA(T,T2) \
{ \
	T * idata = (T *) in_field_node_data; \
	T * odata = (T *) out_node_data; \
	T2 in_null, out_null=0; \
	if (in_null_flag) \
		in_null=(T2) in_field.node_data[data_comp].null_value; \
	if (out_null_flag) \
		out_null=(T2) out.node_data[data_comp].null_value; \
	mask_node_data=(unsigned char*)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size); \
	if (update3D) { \
		for (mask_comp=0; mask_comp<mask3D.nnode_data; mask_comp++) { \
			if (!(int)mask3D.node_data[mask_comp].min) continue; \
			mask3D_node_data=(unsigned char*)mask3D.node_data[mask_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size); \
			if (!mask3D_node_data) continue; \
			for (i=0;i<out_dims[0];i++) \
				for (j=0;j<out_dims[1];j++)	 \
					for (k=0;k<out_dims[2];k++) { \
						idx3d=((k * out_dims[1])+j)* out_dims[0]+i; \
						if (!mask_comp) \
							odata[idx3d]=(T)out_null;\
						odata[idx3d]=(mask3D_node_data[idx3d])?idata[idx3d]:(T)odata[idx3d]; \
						switch (axis) { \
						case 0:	   \
						{ \
							if (i==slice) { \
								idx2d=k*mask_dims[0]+j; \
								if (mask_node_data[idx2d]!=value) { \
									mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d]; \
								} \
							} \
						} break; \
						case 1:	   \
						{ \
							if (j==slice) { \
								idx2d=k*mask_dims[0]+i; \
								if (mask_node_data[idx2d]!=value) {\
									mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
								}\
							} \
						} break; \
						case 2:	   \
						{ \
							if (k==slice) { \
								idx2d=j*mask_dims[0]+i; \
								if (mask_node_data[idx2d]!=value) {\
									mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
								}\
							} \
						} \
						} \
					} \
			if (mask3D_node_data) \
				{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;} \
		} \
	} else { \
		for (mask_comp=0;mask_comp<mask3D.nnode_data;mask_comp++) { \
			if (!(int)mask3D.node_data[mask_comp].min) continue; \
			mask3D_node_data=(unsigned char*)mask3D.node_data[mask_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size); \
			if (!mask3D_node_data) continue; \
			switch (axis) { \
			case 0:	   \
			{ \
				for (j=0;j<out_dims[1];j++) {  \
					for (k=0;k<out_dims[2];k++) { \
						idx2d=k*mask_dims[0]+j; \
						idx3d=((k * out_dims[1])+j)* out_dims[0]+slice; \
						if(!mask_comp) \
							odata[idx3d]=(T)out_null; \
						odata[idx3d]=(mask3D_node_data[idx3d])?idata[idx3d]:odata[idx3d]; \
						if (mask_node_data[idx2d]!=value) {\
							mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
						}\
					} \
				} \
			} break; \
			case 1: \
			{ \
				for (i=0;i<out_dims[0];i++) { \
					for (k=0;k<out_dims[2];k++) { \
						idx2d=k*mask_dims[0]+i; \
						idx3d=((k * out_dims[1])+slice)* out_dims[0]+i; \
						if(!mask_comp) \
							odata[idx3d]=(T)out_null; \
						odata[idx3d]=(mask3D_node_data[idx3d])?idata[idx3d]:odata[idx3d]; \
						if (mask_node_data[idx2d]!=value) {\
							mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
						}\
					} \
				} \
			} break; \
			case 2: \
			{ \
				for (i=0;i<out_dims[0];i++) { \
					for (j=0;j<out_dims[1];j++) { \
						idx2d=j*mask_dims[0]+i; \
						idx3d=((slice * out_dims[1])+j)* out_dims[0]+i; \
						if(!mask_comp) \
							odata[idx3d]=(T)out_null; \
						odata[idx3d]=(mask3D_node_data[idx3d])?idata[idx3d]:odata[idx3d]; \
						if (mask_node_data[idx2d]!=value) {\
							mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
						}\
					} \
				} \
			} \
			} \
			if (mask3D_node_data) \
				{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;} \
		} \
	} \
	if (mask_node_data) \
		{ ARRfree(mask_node_data); mask_node_data=NULL;} \
}


int
SegmentationEditor_SegmentationEditorMods_SegmentationEditorCore::OnInstance(OMevent_mask event_mask, int seq_num)
{
	// clear (OMXint read write notify)
	// clearMask (OMXint read write notify)
	// clearMask3D (OMXint read write notify)
	// in_field (Mesh_Unif+Node_Data read notify)
	// mask (Mesh_Unif+Node_Data read write)
	// mask3D (Mesh_Unif+Node_Data read write)
	// axis (OMXint read)
	// out (Mesh_Unif+Node_Data read write)

	int i;
	int invalid = 0;
	int field_unchanged = (int)in_field.changed(seq_num);
	int clearcmd = CMD_CLEAR_VOLUME;
	char *in_field_node_data;
	int *in_field_dims;
	float *in_field_points;
	int *mask3D_dims;
	float *mask3D_points;
	int *mask_dims;
	int *out_dims;
	float *out_points;
	int data_comp, veclen;
	int out_data_size;
	int in_field_data_size, in_field_data_type;

	int	 mask_data_size, mask3D_data_size;
	unsigned char *mask_node_data;
	unsigned char *mask3D_node_data;

	if( in_field.ndim.valid_obj()	== 0 ||
		in_field.nspace.valid_obj() == 0 ) {
		//
		// Probably input field has been disconnected.
		//
		if( out.ndim.valid_obj() )
			out.ndim.set_obj_val( OMnull_obj );
		if( out.dims.valid_obj() )
			out.dims.set_obj_val( OMnull_obj );
		if( out.nspace.valid_obj() )
			out.nspace.set_obj_val( OMnull_obj );
		return 1;
	}

	if ((int)in_field.ndim != (int)out.ndim) {
		field_unchanged = 0;
		out.ndim = in_field.ndim;
	}

	if ((int)in_field.ndim != (int)mask3D.ndim) {
		field_unchanged = 0;
		mask3D.ndim = in_field.ndim;
	}

	in_field_dims = (int *)in_field.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	mask3D_dims = (int *)mask3D.dims.ret_array_ptr(OM_GET_ARRAY_RW);
	mask_dims = (int *)mask.dims.ret_array_ptr(OM_GET_ARRAY_RW);
	out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_RW);

	if (!in_field_dims || !mask3D_dims || !mask_dims || !out_dims)
		invalid=1;
	if (!invalid &&(((int)axis < 0) || ((int)axis >= (int)out.ndim)) )
	{
		axis=0;
	}
	if (!invalid)
	{
		switch (axis)
		{
			case 0:
			{
				if (mask_dims[0]!=in_field_dims[1]) {
					mask_dims[0]=in_field_dims[1];
					field_unchanged = 0;
				}
				if (mask_dims[1]!=in_field_dims[2]) {
					mask_dims[1]=in_field_dims[2];
					field_unchanged = 0;
				}
			} break;
			case 1:
			{
				if (mask_dims[0]!=in_field_dims[0]) {
					mask_dims[0]=in_field_dims[0];
					field_unchanged = 0;
				}
				if (mask_dims[1]!=in_field_dims[2]) {
					mask_dims[1]=in_field_dims[2];
					field_unchanged = 0;
				}
			} break;
			case 2:
			{
				if (mask_dims[0]!=in_field_dims[0]) {
					mask_dims[0]=in_field_dims[0];
					field_unchanged = 0;
				}
				if (mask_dims[1]!=in_field_dims[1]) {
					mask_dims[1]=in_field_dims[1];
					field_unchanged = 0;
				}
			} break;
		}

		for (i=0;i<(int)in_field.ndim;i++) {
			if (mask3D_dims[i] != in_field_dims[i]) {
				mask3D_dims[i] = in_field_dims[i];
				field_unchanged = 0;
			}
			if (out_dims[i] != in_field_dims[i]) {
				out_dims[i] = in_field_dims[i];
				field_unchanged = 0;
			}
		}
	}
	if (mask_dims)
		{ ARRfree(mask_dims); mask_dims=NULL;}

	if (mask3D_dims)
		{ ARRfree(mask3D_dims); mask3D_dims=NULL;}

	if (in_field_dims)
		{ ARRfree(in_field_dims); in_field_dims=NULL;}
	if (out_dims)
		{ ARRfree(out_dims); out_dims=NULL;}

	if (field_unchanged || invalid)
		return(0);

	if ((int)clearMask) {
		clearcmd = CMD_CLEAR_MASK_2D;
		field_unchanged=0;
		clearMask=0;
	}
	if ((int)clearMask3D) {
		clearcmd = CMD_CLEAR_MASK_3D;
		field_unchanged=0;
		clearMask3D=0;
	}
	if ((int)clear) {
		clearcmd = CMD_CLEAR_VOLUME;
		field_unchanged=0;
		clear=0;
	}

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditorCore::OnInstance\n");
	fprintf(stderr, "SegEditCore::OnInstance\n");
#endif

	in_field_points = (float *)in_field.points.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!in_field_points) {
		ERRverror("",ERR_NO_HEADER | ERR_PRINT, "SegmentationEditorCore::OnInstance\nCan't get in_field.points\n");
		return 0;
	}
	mask3D.nspace = out.nspace = (int) in_field.nspace;
	mask3D.npoints = out.npoints = (int) in_field.npoints;

	out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_RW);
	if (!out_points) {
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"SegmentationEditorCore::OnInstance\nCan't get out.points\n");
		if (in_field_points)
			ARRfree(in_field_points);
		return 0;
	}
	memcpy(out_points,in_field_points,(int)out.nspace*(int)out.npoints*sizeof(float));
	ARRfree(out_points); out_points=NULL;

	mask3D_points = (float *)mask3D.points.ret_array_ptr(OM_GET_ARRAY_RW);
	if (!mask3D_points)
	{
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"SegmentationEditorCore::OnInstance\nCan't get mask3D.points\n");
		if (in_field_points)
			ARRfree(in_field_points);
		return 0;
	}
	memcpy(mask3D_points,in_field_points,(int)mask3D.nspace*(int)mask3D.npoints*sizeof(float));
	ARRfree(mask3D_points); mask3D_points=NULL;

	if (in_field_points)
		{ ARRfree(in_field_points); in_field_points=NULL;}

	switch(clearcmd)
	{
	case CMD_CLEAR_VOLUME:
	{
		out.nnode_data = in_field.nnode_data;
		for (data_comp = 0; data_comp < out.nnode_data; data_comp++) {

			FLD_Data_Array &in_comp	 = in_field.node_data[data_comp];
			FLD_Data_Array &out_comp = out.node_data[data_comp];

			veclen = out_comp.veclen = in_comp.veclen;
			in_field_node_data = (char *)in_comp.values.ret_array_ptr(OM_GET_ARRAY_RW, &in_field_data_size, &in_field_data_type);
			if (!in_field_node_data) {
				ERRverror("",ERR_NO_HEADER | ERR_PRINT,"SegmentationEditorCore::OnInstance\nCan't get in_field.node_data[%i]\n",data_comp);
				return 0;
			}
			switch (in_field_data_type)
			{
				case OM_TYPE_BYTE:
				{
					int null_value;
					if ((int)in_comp.null_flag)
						null_value=(int)in_comp.null_value;
					else {
						if((int)in_comp.min > 0)
							null_value = 0;
						else {
							if ((int)in_comp.max < UCHAR_MAX)
								null_value = UCHAR_MAX;
							else {
								null_value = -1;
								CLEAR_VOLUME_DATA(unsigned char,short,OM_TYPE_SHORT);
								break;
							}
						}
					}
					CLEAR_VOLUME_DATA(unsigned char,unsigned char,OM_TYPE_BYTE);
				} break;
				case OM_TYPE_CHAR:
				{
					int null_value;
					if ((int)in_comp.null_flag)
						null_value=(int)in_comp.null_value;
					else {
						if((int)in_comp.min > SCHAR_MIN)
							null_value = SCHAR_MIN;
						else {
							if ((int)in_comp.max < SCHAR_MAX)
								null_value = SCHAR_MAX;
							else {
								null_value = SHRT_MIN;
								CLEAR_VOLUME_DATA(char,short,OM_TYPE_SHORT);
								break;
							}
						}
					}
					CLEAR_VOLUME_DATA(char,char,OM_TYPE_CHAR);
				} break;
				case OM_TYPE_SHORT:
				{
					int null_value;
					if ((int)in_comp.null_flag)
						null_value=(int)in_comp.null_value;
					else {
						if((int)in_comp.min > SHRT_MIN)
							null_value = SHRT_MIN;
						else {
							if ((int)in_comp.max < SHRT_MAX)
								null_value = SHRT_MAX;
							else {
								null_value = INT_MIN;
								CLEAR_VOLUME_DATA(short,int,OM_TYPE_INT);
								break;
							}
						}
					}
					CLEAR_VOLUME_DATA(short,short,OM_TYPE_SHORT);
				} break;
				case OM_TYPE_INT:
				{
					int null_value;
					if ((int)in_comp.null_flag)
						null_value=(int)in_comp.null_value;
					else {
						if((int)in_comp.min > INT_MIN)
							null_value = INT_MIN;
						else {
							if ((int)in_comp.max < INT_MAX)
								null_value = INT_MAX;
							else {
								// look for unused value
								null_value = INT_MIN;
								CLEAR_VOLUME_DATA(int,int,OM_TYPE_INT);
								break;
							}
						}
					}
					CLEAR_VOLUME_DATA(int,int,OM_TYPE_INT);
				} break;
				case OM_TYPE_FLOAT:
				{
					float null_value;
					if ((int)in_comp.null_flag)
						null_value = (float)in_comp.null_value;
					else
						null_value = (float)in_comp.min - 1;
					CLEAR_VOLUME_DATA(float,float,OM_TYPE_FLOAT);
				} break;
				case OM_TYPE_DOUBLE:
				{
					double null_value;
					if ((int)in_comp.null_flag)
						null_value = (double)in_comp.null_value;
					else
						null_value = (double)in_comp.min - 1;
					CLEAR_VOLUME_DATA(double,double,OM_TYPE_DOUBLE);
				} break;
			}
			if (in_field_node_data)
				ARRfree(in_field_node_data);
		}
	} // FALLTHROUGH
	case CMD_CLEAR_MASK_3D:
	{
		mask3D.nnode_data = 1;
		mask3D.node_data[0].veclen = 1;
		mask3D.node_data[0].null_value = 0;
		mask3D.node_data[0].null_flag = 1;
		mask3D_node_data = (unsigned char*)mask3D.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
		if (mask3D_node_data) {
			memset(mask3D_node_data, 0, mask3D_data_size*sizeof(unsigned char));
			ARRfree(mask3D_node_data);
		}
	} // FALLTHROUGH
	case CMD_CLEAR_MASK_2D:
	{
		mask.nnode_data=1;
		mask.node_data[0].veclen = 1;
		mask.node_data[0].null_value = 0;
		mask.node_data[0].null_flag = 1;
		mask_node_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
		if (mask_node_data) {
			memset(mask_node_data, 0, mask_data_size*sizeof(unsigned char));
			ARRfree(mask_node_data);
		}
	}
	} // switch cmd
	// return 1 for success
	return(1);
}


int
SegmentationEditor_SegmentationEditorMods_SegmentationEditorCore::Mask_2D_from_3D()
{
	int* mask3D_dims=NULL;
	unsigned char* mask_node_data;
	int mask_data_size;
	unsigned char* mask3D_node_data;
	int mask3D_data_size;
	int mask_comp;	  
	int idx2d,idx3d;
	int i,j,k,use;

	mask3D_dims = (int *)mask3D.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!mask3D_dims)
		return 0;

	// Initialize 2D mask with the relevant slice from the 3D mask.
	mask_node_data = (unsigned char*)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
	if (!mask_node_data) {
		if (mask3D_dims)
			ARRfree(mask3D_dims);
		return 0;
	}
	memset(mask_node_data,0,mask_data_size*sizeof(unsigned char));
	switch ((int) command)
	{
		case CMD_APPLY:
		{
			for (mask_comp=0;mask_comp<mask3D.nnode_data;mask_comp++) {
				if (!(int)mask3D.node_data[mask_comp].min) continue;
				mask3D_node_data=(unsigned char*)mask3D.node_data[mask_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_BYTE, &mask3D_data_size);
				if (!mask3D_node_data) continue;
				switch (axis) {
				case 0:
				{
					for (j=0;j<mask3D_dims[1];j++)
						for (k=0;k<mask3D_dims[2];k++) {
							idx2d=k*mask3D_dims[1]+j;
							idx3d=((k * mask3D_dims[1])+j)* mask3D_dims[0]+slice;
							if (mask_node_data[idx2d]!=value)
							{
								mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];\
							}
						}
				} break;
				case 1:
				{
					for (i=0;i<mask3D_dims[0];i++)
						for (k=0;k<mask3D_dims[2];k++) {
							idx2d=k*mask3D_dims[0]+i;
							idx3d=((k * mask3D_dims[1])+slice)* mask3D_dims[0]+i;
							if (mask_node_data[idx2d]!=value)
							{
								mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];
							}
						}
				} break;
				case 2:
				{
					for (i=0;i<mask3D_dims[0];i++)
						for (j=0;j<mask3D_dims[1];j++) {
							idx2d=j*mask3D_dims[0]+i;
							idx3d=((slice * mask3D_dims[1])+j)* mask3D_dims[0]+i;
							if (mask_node_data[idx2d]!=value)
							{
								mask_node_data[idx2d]=(mask_node_data[idx2d]<mask3D_node_data[idx3d])?mask3D_node_data[idx3d]:mask_node_data[idx2d];
							}
						}
				} // case
				} // switch axis
				if (mask3D_node_data)
					{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;}
			} // for mask3D node data comps
		} break;
		case CMD_ADD:
		{
			use=-1;
			for (i=0; i<mask3D.nnode_data; i++)
				if ((int)mask3D.node_data[i].min==(int)value) {
					use=i;
					break;
				}
			if (use!=-1)
			{
				mask3D_node_data = (unsigned char*)mask3D.node_data[use].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_BYTE, &mask3D_data_size);
				if (mask3D_node_data) {
					switch (axis) {
					case 0:
					{
						for (j=0;j<mask3D_dims[1];j++ ) {
							for (k=0;k<mask3D_dims[2];k++) {
								idx2d=k*mask3D_dims[1]+j;
								idx3d=((k * mask3D_dims[1])+j)* mask3D_dims[0]+slice;
								mask_node_data[idx2d]=mask3D_node_data[idx3d];
							}
						}
					} break;
					case 1:
					{
						for (i=0;i<mask3D_dims[0];i++) {
							for (k=0;k<mask3D_dims[2];k++) {
								idx2d=k*mask3D_dims[0]+i;
								idx3d=((k * mask3D_dims[1])+slice)* mask3D_dims[0]+i;
								mask_node_data[idx2d]=mask3D_node_data[idx3d];
							}
						}
					} break;
					case 2:
					{
						for (i=0;i<mask3D_dims[0];i++) {
							for (j=0;j<mask3D_dims[1];j++) {
								idx2d=j*mask3D_dims[0]+i;
								idx3d=((slice * mask3D_dims[1])+j)* mask3D_dims[0]+i;
								mask_node_data[idx2d]=mask3D_node_data[idx3d];
							}
						}
					} // case
					} // switch axis
					{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;}
				}
			}
		} break;
	} // switch
	if (mask3D_dims)
		ARRfree(mask3D_dims);
	if (mask_node_data)
		ARRfree(mask_node_data);
	return 1;
}



int
SegmentationEditor_SegmentationEditorMods_SegmentationEditorCore::OnChange(OMevent_mask event_mask, int seq_num)
{
	// axis (OMXint read req notify)
	// slice (OMXint read req notify)
	// value (OMXint read notify)
	// value_nd (OMXint write)
	// use_value (OMXint read notify)
	// command (OMXenum read req notify)
	// regcmd (OMXenum read req notify)
	// in_field (Mesh_Unif+Node_Data read write)
	// mask (Mesh_Unif+Node_Data read write req )
	// mask3D (Mesh_Unif+Node_Data read write req )
	// region2D (Node_Data read notify)
	// region3D (Node_Data read notify)
	// draw (OMXint read write notify)
	// out (Mesh_Unif+Node_Data read write)
	// label (OMXstr read write notify)

	int	 mask3D_data_size;
	unsigned char *mask3D_node_data=NULL;
	int mask_data_size;
	unsigned char *mask_node_data;
	int *region2D_node_data;
	int region2D_data_size, region3D_data_size;
	int *region3D_node_data;
	int	 masknnodes;
	int region2D_null_flag;
	int region2D_null=0;
	int region3D_null_flag;
	int region3D_null=0;
	int update3D=0;
	unsigned char null_repl;
	//int mask_changed, region2D_changed, region3D_changed, axis_changed;
	int mask_changed;
	int data_comp,mask_comp;
	int valid = 1;
	int i,j,k,use,idx2d,idx3d;
	char * enlabel;
	char nlabel[255];
	int	 in_field_data_size;
	int	 in_null_flag, out_null_flag;
	int	 out_data_type;
	char *out_node_data;
	char *in_field_node_data;
	int *mask3D_dims;
	int *in_field_dims;
	int *out_dims;
	int *mask_dims;
	segment_grow_region * grow;
	double* reg_data=NULL;	
	int* seed_arr;
	int seed_size;	

	#ifdef DEBUG
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditorCore::OnChange\n");
		fprintf(stderr, "SegEditCore::OnChange comand:%d\n",
				(int)command );
	#endif

	mask_changed = mask.changed(seq_num);
	//region2D_changed = region2D.changed(seq_num);
	//region3D_changed = region3D.changed(seq_num);
	//axis_changed = axis.changed(seq_num);

	if (value.changed(seq_num))
	{
		use=-1;
		for (i=0; i<mask3D.nnode_data; i++) {
			if ((int)mask3D.node_data[i].min==(int)value) {
				label=mask3D.node_data[i].labels;
				value_nd=i;
				use=i;
				break;
			}
		}
		if (use==-1) {
			sprintf(nlabel,"Segment %i",(int)value);
			label.set_str_val(nlabel);
		}
	}
	label.get_str_val(&enlabel);
	if (!enlabel) {
		sprintf(nlabel,"Segment %i",(int)value);
		label.set_str_val(nlabel);
	}
	#ifdef DEBUG
		fprintf(stderr,"label %s\n",enlabel);
	#endif

/*	  if (label)
		sprintf(label,"Segment %i",(int)value);
*/

	mask3D_dims = (int *)mask3D.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	in_field_dims = (int *)in_field.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_RD);

	if (!mask3D_dims || !in_field_dims || !out_dims)
		valid = 0;

	//sanity check
	if (valid && ((int) mask3D.ndim != (int) in_field.ndim))
		valid = 0;

	if (valid && ((int) out.ndim != (int) in_field.ndim))
		valid = 0;

	if( valid && (((int)axis < 0) || ((int)axis >= (int)in_field.ndim)) )
		valid = 0;

	if( valid && (((int)slice < 0) || ((int)slice >= in_field_dims[axis])) )
		valid = 0;

	if (valid) {
		for (i=0;i<(int)in_field.ndim;i++) {
			if ((mask3D_dims[i] != in_field_dims[i])||(out_dims[i] != in_field_dims[i]))
			{
				valid = 0;
				break;
			}
		}
	}

	mask_dims = (int *)mask.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!mask_dims)
		valid=0;

	if (!valid) {
		if (in_field_dims) ARRfree(in_field_dims);
		if (mask3D_dims)   ARRfree(mask3D_dims);
		if (out_dims)	   ARRfree(out_dims);
		return 0;
	}

#ifdef DEBUG
	fprintf(stderr, "SegEditCore::OnChange changed: "
			"axis:%d, slice:%d, region2D:%d, region3D:%d, mask3D:%d\n",
			axis.changed(seq_num), slice.changed(seq_num),
			region2D.changed(seq_num), region3D.changed(seq_num),
			mask3D.changed(seq_num) );

#endif

	if (axis.changed(seq_num))
	{
		ARRfree(mask_dims);
		// reget in RW instead of RD
		mask_dims = (int *)mask.dims.ret_array_ptr(OM_GET_ARRAY_RW);
		switch (axis)
		{
		case 0:
		{
			mask_dims[0]=in_field_dims[1];
			mask_dims[1]=in_field_dims[2];
		} break;
		case 1:
		{
			mask_dims[0]=in_field_dims[0];
			mask_dims[1]=in_field_dims[2];
		} break;
		case 2:
		{
			mask_dims[0]=in_field_dims[0];
			mask_dims[1]=in_field_dims[1];
		} break;
		}
	}

	if (mask3D.changed(seq_num))
	  update3D=1;

	if (axis.changed(seq_num)||slice.changed(seq_num)||mask3D.changed(seq_num))
	{
		// Initialize 2D mask with the relevant slice from the 3D mask.
		if (!Mask_2D_from_3D()) {
			if (in_field_dims)
				ARRfree(in_field_dims);
			if (mask3D_dims)
				ARRfree(mask3D_dims);
			if (out_dims)
				ARRfree(out_dims);
			if (mask_dims)
				ARRfree(mask_dims);
			return 1;
		}

		if (!mask3D.changed(seq_num)) {
			// If the mask3D has changed, we got to redo the out field
			// so don't exit here.
//			  if (mask_node_data) {
//				  ARRfree(mask_node_data);
//			  }
			if (in_field_dims) {
				ARRfree(in_field_dims);
			}
			if (mask3D_dims) {
				ARRfree(mask3D_dims);
			}
			if (out_dims) {
				ARRfree(out_dims);
			}
			if (mask_dims) {
				ARRfree(mask_dims);
			}
			// not an error condition, so return 1 for success
			return 1;
		}
	} // if axis or slice or mask3d changed initialize mask2d

	while (value && ((int)grow_region_3D || mask.changed(seq_num)))
	{
		// insert 2D mask in 3D_mask which uses the value
		use = mask3D.nnode_data;
		#ifdef DEBUG
			fprintf(stderr,"mask3D.nnode_data: %i / %i\n",use,(int)mask3D.nnode_data);
		#endif
		for (i=0; i<mask3D.nnode_data; i++)
			if ((!(int)mask3D.node_data[i].min)||((int)mask3D.node_data[i].min==(int)value)) {
				mask3D.node_data[i].labels=label;
				use=i;
				break;
			}
		// if there is no 3D mask with the given value create a new one
		value_nd = use;
		if (use != mask3D.nnode_data)
		{
			mask3D_node_data = (unsigned char*)mask3D.node_data[use].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
		} else {
			++(mask3D.nnode_data);
			mask3D.node_data[use].veclen = 1;
			mask3D.node_data[use].null_value = 0;
			mask3D.node_data[use].null_flag = 1;
			mask3D.node_data[use].labels=label;
			mask3D_node_data = (unsigned char*)mask3D.node_data[use].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
			if (mask3D_node_data) {
				memset(mask3D_node_data,0,mask3D_data_size*sizeof(unsigned char));
			}
		}
		if (!mask3D_node_data) {
			if (in_field_dims)
				ARRfree(in_field_dims);
			if (mask3D_dims)
				ARRfree(mask3D_dims);
			if (out_dims)
				ARRfree(out_dims);
			if (mask_dims)
				ARRfree(mask_dims);
			return 0;
		}
		if ((int)grow_region_3D)
		{
			seed_arr = (int *)seed.ret_array_ptr(OM_GET_ARRAY_RD,&seed_size);
			if (seed_arr && (seed_size==3)) {
				reg_data = (double*)in_field.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_DOUBLE,&in_field_data_size);
				if (reg_data) {
					grow = new segment_grow_region(mask3D_dims,reg_data,&mask3D_node_data);
					grow->set_params(seed_arr[0], seed_arr[1], seed_arr[2], (double) reg_delta, (int) reg_incr);
					grow->set_out_value((unsigned char)value,0,(int)regcmd);
					grow->grow(seed_arr[0], seed_arr[1], seed_arr[2]);
					delete grow;
					ARRfree(reg_data); 
					reg_data=NULL;
				}
				ARRfree(seed_arr); 
				seed_arr=NULL;
				update3D=1;			   
			}
			else {
				if (mask3D_node_data)
					ARRfree(mask3D_node_data);
				if (in_field_dims)
					ARRfree(in_field_dims);
				if (mask3D_dims)
					ARRfree(mask3D_dims);
				if (out_dims)
					ARRfree(out_dims);
				if (mask_dims)
					ARRfree(mask_dims);
				return 0;
			}
		}
		if (mask.changed(seq_num)) {
			masknnodes =  (axis!=0)?mask3D_dims[0]:1;
			masknnodes *= (axis!=1)?mask3D_dims[1]:1;
			masknnodes *= (axis!=2)?mask3D_dims[2]:1;
			if ((!mask.nnode_data)||((int)mask.nnodes != masknnodes))
			{
				if (mask3D_node_data)
					ARRfree(mask3D_node_data);
				if (in_field_dims)
					ARRfree(in_field_dims);
				if (mask3D_dims)
					ARRfree(mask3D_dims);
				if (out_dims)
					ARRfree(out_dims);
				if (mask_dims)
					ARRfree(mask_dims);
				return 0;
			}
			mask_node_data = (unsigned char*)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_BYTE,&mask_data_size);
			if (!mask_node_data) {
				if (mask3D_node_data)
					ARRfree(mask3D_node_data);
				if (in_field_dims)
					ARRfree(in_field_dims);
				if (mask3D_dims)
					ARRfree(mask3D_dims);
				if (out_dims)
					ARRfree(out_dims);
				if (mask_dims)
					ARRfree(mask_dims);
				return 0;
			}
			switch (axis) {
			case 0:
			{
				for (j=0;j<mask3D_dims[1];j++) {
					for (k=0;k<mask3D_dims[2];k++) {
						idx2d=k*mask3D_dims[1]+j;
						idx3d=((k * mask3D_dims[1])+j)* mask3D_dims[0]+slice;
						mask3D_node_data[idx3d]=(mask_node_data[idx2d]!=(int)value)?0:(unsigned char)value;
					}
				}
			} break;
			case 1:
			{
				for (i=0;i<mask3D_dims[0];i++) {
					for (k=0;k<mask3D_dims[2];k++) {
						idx2d=k*mask3D_dims[0]+i;
						idx3d=((k * mask3D_dims[1])+slice)* mask3D_dims[0]+i;
						mask3D_node_data[idx3d]=(mask_node_data[idx2d]!=(int)value)?0:(unsigned char)value;
					}
				}
			} break;
			case 2:
			{
				for (i=0;i<mask3D_dims[0];i++) {
					for (j=0;j<mask3D_dims[1];j++) {
						idx2d=j*mask3D_dims[0]+i;
						idx3d=((slice * mask3D_dims[1])+j)* mask3D_dims[0]+i;
						mask3D_node_data[idx3d]=(mask_node_data[idx2d]!=(int)value)?0:(unsigned char)value;
					}
				}
			} // case
			} // axis
			if (mask_node_data)
				{ ARRfree(mask_node_data); mask_node_data=NULL;}
		}
		break;
	} // while
	if (mask3D_node_data)
		{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;}

	if (command.changed(seq_num))
		update3D=1;

#ifdef DEBUG
	fprintf(stderr,"before switch command\n");
#endif
	switch ((int) command)
	{
	case CMD_APPLY:
	{
		for (data_comp = 0; data_comp < in_field.nnode_data; data_comp++) {
			in_null_flag=in_field.node_data[data_comp].null_flag;
			out_null_flag=out.node_data[data_comp].null_flag;
			out.node_data[data_comp].labels=in_field.node_data[data_comp].labels;
			out_node_data = (char *)out.node_data[data_comp].values.ret_array_ptr(OM_GET_ARRAY_RW, &in_field_data_size, &out_data_type);
			in_field_node_data = (char *)in_field.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, out_data_type, &in_field_data_size);
			switch (out_data_type)
			{
			case OM_TYPE_BYTE:
				APPLY_MASK_DATA(unsigned char,int);
			break;
			case OM_TYPE_CHAR:
				APPLY_MASK_DATA(char,int);
			break;
			case OM_TYPE_SHORT:
				APPLY_MASK_DATA(short,int);
			break;
			case OM_TYPE_INT:
				APPLY_MASK_DATA(int,int);
			break;
			case OM_TYPE_FLOAT:
				APPLY_MASK_DATA(float,float);
			break;
			case OM_TYPE_DOUBLE:
				APPLY_MASK_DATA(double,double);
			} // switch out_data_type
			if (in_field_node_data)
				{ ARRfree(in_field_node_data); in_field_node_data=NULL;}
			if (out_node_data)
				{ ARRfree(out_node_data); out_node_data=NULL;}
		}
	} break;
	case CMD_ADD:
	{
		mask_node_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
		if (!mask_node_data) {
			if (in_field_dims)
				ARRfree(in_field_dims);
			if (mask3D_dims)
				ARRfree(mask3D_dims);
			if (out_dims)
				ARRfree(out_dims);
			if (mask_dims)
				ARRfree(mask_dims);
			return 0;
		}
		use=-1;
		for (i=0; i<mask3D.nnode_data; i++)
			if ((int)mask3D.node_data[i].min==(int)value) {
				use=i;
				break;
			}

		if (use==-1)
		{
			memset(mask_node_data,0,mask_data_size*sizeof(unsigned char));
		} else {
			mask3D_node_data = (unsigned char*)mask3D.node_data[use].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
			if (!mask3D_node_data) {
				memset(mask_node_data,0,mask_data_size*sizeof(unsigned char));
			} else {
				switch (axis) {
				case 0:
				{
					for (j=0;j<mask3D_dims[1];j++ ) {
						for (k=0;k<mask3D_dims[2];k++) {
							idx2d=k*mask3D_dims[1]+j;
							idx3d=((k * mask3D_dims[1])+j)* mask3D_dims[0]+slice;
							mask_node_data[idx2d]=mask3D_node_data[idx3d];
						}
					}
				} break;
				case 1:
				{
					for (i=0;i<mask3D_dims[0];i++) {
						for (k=0;k<mask3D_dims[2];k++) {
							idx2d=k*mask3D_dims[0]+i;
							idx3d=((k * mask3D_dims[1])+slice)* mask3D_dims[0]+i;
							mask_node_data[idx2d]=mask3D_node_data[idx3d];
						}
					}
				} break;
				case 2:
				{
					for (i=0;i<mask3D_dims[0];i++) {
						for (j=0;j<mask3D_dims[1];j++) {
							idx2d=j*mask3D_dims[0]+i;
							idx3d=((slice * mask3D_dims[1])+j)* mask3D_dims[0]+i;
							mask_node_data[idx2d]=mask3D_node_data[idx3d];
						}
					}
				} // case
				} // switch axis
			}
			if (mask3D_node_data)
				{ ARRfree(mask3D_node_data); mask3D_node_data=NULL;}
		}
		if (mask_node_data)
			{ ARRfree(mask_node_data); mask_node_data=NULL;}
	} // case
	} // switch command

	Mask_2D_from_3D();
	
	if (mask_node_data)
		ARRfree(mask_node_data);
	if (in_field_dims)
		ARRfree(in_field_dims);
	if (mask_dims)
		ARRfree(mask_dims);
	if (mask3D_dims)
		ARRfree(mask3D_dims);
	if (out_dims)
		ARRfree(out_dims);

#ifdef DEBUG
	fprintf(stderr,"exit segedit\n");
#endif
	// return 1 for success
	return(1);
}

int
SegmentationEditor_SegmentationEditorMods_SegmentationEditorCore::OnClear(OMevent_mask event_mask, int seq_num)
{
	// axis (OMXint read)
	// slice (OMXint read)
	// clearMask (OMXint read write notify)
	// clearMask3D (OMXint read write notify)
	// clearEntity (OMXint read write notify) 
	// combineEntity (OMXint read write notify)
	// entityList (OMXint_array read write)
	// command (OMXenum read req)
	// mask (Mesh_Unif+Node_Data write)
	// mask3D (Mesh_Unif+Node_Data read write req)
	// out (Mesh_Unif+Node_Data read write req)


	int entityList_size;
	int *entityList_arr;

	int outEntities_size;
	int *outEntities_arr;

	int clear_mask=0;
	int clear_mask3D=0;
	int clear_entity=0;
	int combine_entity=0;

	int need_entity=0;
	int i,j,k;
	int mask3D_data_size,mask3D_data_comp;
	unsigned char *mask3D_node_data;
	unsigned char *mask3D_node_data2;
/*
	unsigned char *mask_node_data;
	int mask_data_size;
	int *in_field_dims;
	int *out_dims;
	int *mask_dims;
	int mask_comp;
	int use,idx2d,idx3d;
	int *mask3D_dims;
*/
	
	if ((int)clearEntity)
	{
		clear_entity=1;
		need_entity=1;
		clearEntity=0;
	}
	if ((int)combineEntity)
	{
		combine_entity=1;
		need_entity=2;
		combineEntity=0;
	}  
/*	  if ((int)clearMask)
	{
		clear_mask=1;
		clearMask=0;
	}
*/	  if ((int)clearMask3D)
	{
		 clear_mask3D=1;
		clearMask3D=0;
	}


	entityList_arr = (int *)entityList.ret_array_ptr(OM_GET_ARRAY_RD,&entityList_size);
	if (need_entity > entityList_size){
		if (entityList_arr) 
			ARRfree(entityList_arr);
	}
	else {
		if (entityList_arr) {
			int nentities = (int)mask3D.nnode_data;
			nentities -= entityList_size;
			// move;
			int entity_count = need_entity-1;
			if (combine_entity){
				nentities++;
			}
			if (!nentities)
				clear_mask3D=1;
			else {
				j=(int)mask3D.nnode_data;
				// OMobj_id* move=(OMobj_id*)malloc(j*sizeof(OMobj_id));
				int* move= (int*) malloc(j*sizeof(int));
				j--;
				if ((int)combine_entity){
					mask3D_node_data = (unsigned char *)mask3D.node_data[entityList_arr[0]].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
					unsigned char value = (int)mask3D.node_data[entityList_arr[0]].min;
					if (mask3D_node_data) {
						for (k = 1; k < entityList_size; k++) {
							mask3D_node_data2 = (unsigned char *)mask3D.node_data[entityList_arr[k]].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_BYTE, &mask3D_data_size);
							if (mask3D_node_data2){
								for (i=0;i<mask3D_data_size;i++)
									if (mask3D_node_data2[i])
										mask3D_node_data[i] = value;
								ARRfree(mask3D_node_data2);
							}
						}				  
						if (mask3D_node_data)
							ARRfree(mask3D_node_data);
					}
				}
				i=0;
				for (mask3D_data_comp = 0; mask3D_data_comp < (int)mask3D.nnode_data; mask3D_data_comp++) { 
					if (entity_count<entityList_size) {
						if (mask3D_data_comp==entityList_arr[entity_count]) {
							move[j]=(int)mask3D_data_comp;
							entity_count++;
							j--;
							continue;
						}
					}
					move[i]=(int)mask3D_data_comp;
					i++;
				}
				for (mask3D_data_comp = 0; mask3D_data_comp < nentities; mask3D_data_comp++)
				{
					mask3D_node_data = (unsigned char *)mask3D.node_data[mask3D_data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
					if (mask3D_node_data){
						mask3D_node_data2 = (unsigned char *)mask3D.node_data[move[mask3D_data_comp]].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_BYTE, &mask3D_data_size);
						if (mask3D_node_data2){
							memcpy(mask3D_node_data,mask3D_node_data2,mask3D_data_size*sizeof(unsigned char));
							mask3D.node_data[mask3D_data_comp].labels=mask3D.node_data[move[mask3D_data_comp]].labels;
							ARRfree(mask3D_node_data2);
						}
						ARRfree(mask3D_node_data);
					}
				}
				mask3D.nnode_data = nentities;
				outEntities_arr = (int *)outEntities.ret_array_ptr(OM_GET_ARRAY_RW,&outEntities_size);
				if (outEntities_arr) // if not there is no problem
				{
					k=0;
					for (i=0;i<outEntities_size;i++) { 
						move[k]=outEntities_arr[i];
						for (j=0;j<entityList_size;j++) {
							if(entityList_arr[j]==outEntities_arr[i])
							{
								k--;
								break;
							}
							if(entityList_arr[j]<outEntities_arr[i])
								move[k]--;
						}
						k++;
					}
					for(i=0;i<k;i++)
						outEntities_arr[i]=move[i];
					for(i=k;i<outEntities_size;i++)
						outEntities_arr[i]=move[0];
					ARRfree(outEntities_arr);		   
				}	 
				free(move);
			} 
			/* Your code to set entityList_arr goes here */
			ARRfree(entityList_arr);
		}
	}
	value=(int)mask3D.node_data[0].min;
	value_nd=0;
	label=mask3D.node_data[0].labels;
	if (clear_mask3D) {
		mask3D.nnode_data=1;
		mask3D.node_data[0].veclen = 1;
		mask3D_node_data = (unsigned char *)mask3D.node_data[entityList_arr[0]].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask3D_data_size);
		if (mask3D_node_data) {
			memset(mask3D_node_data,0,mask3D_data_size*sizeof(unsigned char));
			ARRfree(mask3D_node_data);
		}
		value=1;
		label="";
		outEntities.set_obj_ref(OMnull_obj);
//		  outEntities.set_obj_ref(OMnull_obj);

/*		  outEntities_arr = (int *)outEntities.ret_array_ptr(OM_GET_ARRAY_RW,&outEntities_size);
		if (outEntities_arr) // if not there is no problem
		{
			for (i=0;i<outEntities_size;i++) 
				outEntities_arr[i]=0;
			ARRfree(outEntities_arr);		   
		}
*/	  
	}
	Mask_2D_from_3D();

/*	  // mask.ndim (int) 
	// mask.dims (int []) 
*/
/*

	// mask.nnodes (int)
	// mask.nnode_data (int)
	int	 mask_data_comp;
	int	 mask_data_size, mask_data_type;
	for (mask_data_comp = 0; mask_data_comp < mask.nnode_data; mask_data_comp++) { 
		// mask.node_data[mask_data_comp].veclen (int) 
		// mask.node_data[mask_data_comp].values (char [])
		char *mask_node_data = (char *)mask.node_data[mask_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RW, &mask_data_size, &mask_data_type);
		if (mask_node_data)
			ARRfree(mask_node_data);
	}

*/
/*
	// out.ndim (int) 
	// out.dims (int []) 
	int *out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_RW);
	// out.nspace (int) 
	// out.npoints (int)
	// out.points (float [])
	float *out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_RW);


	// out.nnodes (int)
	// out.nnode_data (int)
	int	 out_data_comp;
	int	 out_data_size, out_data_type;
	for (out_data_comp = 0; out_data_comp < out.nnode_data; out_data_comp++) { 
		// out.node_data[out_data_comp].veclen (int) 
		// out.node_data[out_data_comp].values (char [])
		char *out_node_data = (char *)out.node_data[out_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RW, &out_data_size, &out_data_type);
		if (out_node_data)
			ARRfree(out_node_data);
	}
*/

	/***********************/
	/* Function's Body	   */
	/***********************/
#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor::OnClear\n");
	fprintf(stderr,"I'm in method: SegmentationEditor::OnClear\n");
#endif
/*
	if (mask_dims)
		ARRfree(mask_dims);

	if (mask3D_dims)
		ARRfree(mask3D_dims);
*/
/*	  if (out_dims)
		ARRfree(out_dims);
*/

	// return 1 for success
	return(1);
}

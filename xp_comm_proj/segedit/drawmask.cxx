/*
			Copyright (c) 2004 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

	This file is under Perforce control
	$Id: //depot/express/trunk/segm_edtr/drawmask.cxx#3 $
*/

#include <math.h>
#include <stdlib.h>

#include <avs/om.h>
#include <avs/gd.h>


#include "gen_mask.h"
#include "seggrow.h"
#include "drawmask.h"

#ifndef M_PI_2
#define M_PI_2	1.57079632679489661923	  /* pi/2 */
#endif

#define CMD_POINT		0
#define CMD_LINE		1
#define CMD_CLOSE		2
#define CMD_FILL		3
#define CMD_NEW			4
#define CMD_INVERT		5
#define CMD_CLEAR		6
#define CMD_REGION_2D	7
#define CMD_REGION_3D	8
#define CMD_THRESH		9
#define CMD_SAVE_MASK  10
#define CMD_LOAD_MASK  11
#define CMD_CLEAR_OUT  12
#define CMD_MAX		   CMD_CLEAR_OUT

#define REG_ADD	 0
#define REG_REPL 1

#define PEN_DRAW		0
#define PEN_ERASE		1

//#define DEBUG

#define THRESH_MASK_DATA(T) \
{ \
	T *idata = (T *)slice_data; \
	unsigned char tmp; \
	for(i=0; i<mask_data_size; i++) { \
		tmp=mask_data[i]; \
		mask_data[i]=reg_value; \
		if ((idata[i]<(T)thr_min) || (idata[i]>(T)thr_max)) { \
			mask_data[i]=((int)regcmd)?0:tmp; \
		} \
	} \
}

/* Low level clip and set in the output matrix */
void maskinspt (float a, float b, int *dims, unsigned char *out, unsigned char value)
{
	/* round a and b (assuming they are positive) */
	int x = (int)floor(a+0.49999);
	int y = (int)floor(b+0.49999);

	if(x < 0) x = 0;
	else {
		if(x > (dims[0]-1)) x = dims[0]-1;
	}
	if(y < 0) y = 0;
	else {
		if (y > (dims[1]-1)) y = dims[1]-1;
	}
	out[y*dims[0]+x] = value;
}

void masklldraw(float x, float y, int *dims, unsigned char *out, int width,unsigned char value)
{
	float r;
	int i;
	float xa;
	float ya;
	int phi;

	r = (float)(width/2.0);
	maskinspt (x,y,dims,out,value);
	for (i=0; i<r; i++) {
		for (phi=0; phi<10; phi++) {
			xa=i*sin(phi*M_PI_2/10.0);
			ya=i*cos(phi*M_PI_2/10.0);
			maskinspt (x+xa,y+ya, dims, out, value);
			maskinspt (x+xa,y-ya, dims, out, value);
			maskinspt (x-xa,y+ya, dims, out, value);
			maskinspt (x-xa,y-ya, dims, out, value);
		}
	}
}


/* Bresenham line drawing */

void maskDrawBresLine2D(float *arr, int *dims, unsigned char *out, int width, unsigned char value)
{
	int x0,y0,x1,y1,x,y,dx,dy,sx,sy,ax,ay,decx,decy,max,var;

	/* get lo the nearest pixel */
	x0 = (int)floor(arr[0] + 0.5);
	y0 = (int)floor(arr[1] + 0.5);
	x1 = (int)floor(arr[2] + 0.5);
	y1 = (int)floor(arr[3] + 0.5);

	/* starting point of line */
	x = x0;
	y = y0;

	/* direction of line */
	dx = x1-x0;
	dy = y1-y0;

	/* increment or decrement depending on direction of line */
	sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
	sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

	/* decision parameters for voxel selection */
	if ( dx < 0 ) dx = -dx;
	if ( dy < 0 ) dy = -dy;
	ax = 2*dx;
	ay = 2*dy;

	/* determine largest direction component, single-step related variable */
	max = dx;
	var = 0;
	if ( dy > max ) { var = 1; }
	/* traverse Bresenham line */
	switch(var)
	{
		case 0:		/* single-step in x-direction */
			for (decy=ay-dx; /**/; x += sx, decy += ay)
			{
				/* process pixel */
				masklldraw(x, y, dims, out, width, value);

				/* take Bresenham step */
				if ( x == x1 ) break;
				if ( decy >= 0 ) { decy -= ax; y += sy; }
			}
		break;
		case 1:		/* single-step in y-direction */
			for (decx=ax-dy; /**/; y += sy, decx += ax)
			{
				/* process pixel */
				masklldraw(x, y, dims, out, width, value);

				/* take Bresenham step */
				if ( y == y1 ) break;
				if ( decx >= 0 ) { decx -= ay; x += sx; }
			}
		break;
	}
}


void maskflood(float a, float b, int *dims, unsigned char *out, unsigned char value)
{
	int x = (int)floor(a);
	int y = (int)floor(b);

	if(x < 0) return;
	if(x > (dims[0]-1)) return;
	if(y < 0) return;
	if (y > (dims[1]-1)) return;
	if (out[y*dims[0]+x] == value) return;

	out[y*dims[0]+x] = value;
	maskflood (x-1,y,dims,out,value);
	maskflood (x+1,y,dims,out,value);
	maskflood (x,y-1,dims,out,value);
	maskflood (x,y+1,dims,out,value);
}

int
SegmentationEditor_SegmentationEditorMods_DrawMask::OnInstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)
	DrawMaskStruct *dmstr;

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: DrawMask::OnInstance\n");
	fprintf( stderr, "DrawMask::OnInstance\n");
#endif

	dmstr = (DrawMaskStruct *) malloc (sizeof(DrawMaskStruct));

	if (!dmstr) return 0;

	dmstr->prev_x = -1;
	dmstr->prev_y = -1;
	dmstr->prev_x_mask = -1;
	dmstr->prev_y_mask = -1;
	dmstr->prev_state = -1;
	dmstr->npoints = 0;
	dmstr->prev_idx = -1;
	dmstr->precmd = -1;
	dmstr->preprecmd = -1;

	OMset_ptr_val((OMobj_id)local_ptr, (void *)dmstr, 0);

	// return 1 for success
	return(1);
}


int
SegmentationEditor_SegmentationEditorMods_DrawMask::OnDeinstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)
	DrawMaskStruct *dmstr;

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor_DrawMask::OnDeinstance\n");
#endif

	OMget_ptr_val((OMobj_id)local_ptr, (void **)&dmstr, 0);

	if (dmstr)
		free (dmstr);

	OMset_ptr_val((OMobj_id)local_ptr, (void *)0, 0);
	// return 1 for success
	return(1);
}

int
SegmentationEditor_SegmentationEditorMods_DrawMask::setup_environment(DrawMaskStruct *st)
{
	OMobj_id view_id, cam_id, obj_id, tmp_id;

#ifdef DEBUG
	fprintf( stderr, "DrawMask::setup_environment\n");
#endif

	//
	//	  Get the view structure pointer
	//
	view_id = OMfind_subobj(*this, OMstr_to_name("view"), OM_OBJ_RD);
	if (OMis_null_obj(view_id)) {
		ERRerror("DrawMask::setup_environment", 0, ERR_ORIG, "Can't get view");
		return 1;
	}

	st->view = (GDview *)GDget_local(view_id, (OMpfi)GDview_init);
	if (!st->view) {
		ERRerror("DrawMask::setup_environment", 0, ERR_ORIG, "Can't get view information");
		return 1;
	}
	if (!st->view->Winfo || !st->view->State) {
		//ERRerror("DrawMask::setup_environment", 0, ERR_ORIG, "Winfo or State not initialized in view");
		return 1;
	}

	//
	//	  Get the camera
	//
	tmp_id = OMfind_subobj(*this, OMstr_to_name("camera"), OM_OBJ_RD);
	if (OMis_null_obj(tmp_id))
		return 1;
	if (OMget_obj_val(tmp_id, &cam_id) != OM_STAT_SUCCESS) {
		ERRerror("DrawMask::setup_environment", 0, ERR_ORIG, "Can't create output without camera");
		return 1;
	}
	st->camera = (GDcamera *)GDget_local(cam_id, (OMpfi)GDcamera_create);
	if (!st->camera) {
		ERRerror("DrawMask::setup_environment", 0, ERR_ORIG, "Can't create output without camera");
		return 1;
	}

	//
	//	  The object is optional. If we don't have one the points
	//	  are mapped back to the camera's space.
	//
	tmp_id = OMfind_subobj(*this, OMstr_to_name("obj"), OM_OBJ_RD);
	if (OMis_null_obj(tmp_id))
		st->object = NULL;
	else if (OMget_obj_val(tmp_id, &obj_id) != OM_STAT_SUCCESS)
		st->object = NULL;
	else
		st->object = (GDobject *)GDget_local(obj_id, (OMpfi)GDobject_create);

	return 0;
}

int
SegmentationEditor_SegmentationEditorMods_DrawMask::init_environment(int command)
{
	DrawMaskStruct *dmstr;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&dmstr, 0);

#ifdef DEBUG
	fprintf( stderr, "DrawMask::init_environment\n");
#endif

	dmstr->prev_x = -1;
	dmstr->prev_y = -1;
	dmstr->prev_x_mask = -1;
	dmstr->prev_y_mask = -1;
	dmstr->prev_state = -1;
	dmstr->npoints = 0;

	dmstr->prev_idx = -1;

	dmstr->precmd = -1;
	dmstr->preprecmd = -1;

	OMset_ptr_val((OMobj_id)local_ptr, (void *)dmstr, 0);
	return 1;
}

int
SegmentationEditor_SegmentationEditorMods_DrawMask::OnDraw(OMevent_mask event_mask, int seq_num)
{
	// command (OMXenum read write req notify)
	// colors (OMXfloat_array read)
	// value (OMXint read)
	// width (OMXint read)
	// x (OMXint read)
	// y (OMXint read)
	// state (OMXint read req notify)
	// obj (GDobject_templ read req notify)
	// view (GDview_templ read req)
	// camera (GDcamera_templ read)
	// reg_delta (OMXdouble read notify)
	// reg_incr (OMXint read notify)
	// reg_trig (OMXint notify)
	// thr_min (OMXdouble read)
	// thr_max (OMXdouble read)
	// thr_trig (OMXint read write notify)
	// thr_comp (OMXint read)
	// local_ptr (OMXptr read)
	// mask (Mesh_Unif+Node_Data read write)
	// npoints (OMXint write)
	// points (OMXfloat_array read write)
	// in_slice (Node_Data read)
	// regcmd (OMXenum read notify)
	
	
	int pts;
	int cmd = (int)command;
	int i, first;
	int add_pt=0;
	int lx,ly,lwidth,lstate;
	short pt[2];
	float opt[2];
	float lpt[4];
	int seed[3];
	int grow_dims[3];
	float  mat[4][4], invmat[4][4];
	unsigned char reg_value=0;
	char *slice_data=NULL;
	int slice_data_size;
	int slice_data_type;
	int run_region = 0;
	segment_grow_region * grow;
	double* reg_data=NULL;	  

	DrawMaskStruct *dmstr;

	OMget_ptr_val((OMobj_id)local_ptr, (void **)&dmstr, 0);
	lwidth = (int) width;
	lx = (int) x;
	ly = (int) y;
	lstate = (int) state;

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor_DrawMask::OnDraw\n");
	fprintf( stderr, "SegmentationEditor_DrawMask::OnDraw cmd:%d, state:%d\n", cmd, lstate );
#endif

	if (CMD_THRESH < cmd && cmd <= CMD_MAX )
		// Commands not handled in this file, but not an error.
		// Returning an error code is not something to do causally,
		// because all the parameters marked +write are marked invalid.
		return 1;
	if (cmd > CMD_MAX)
		return 0;

	switch ((int) pen) {
		case PEN_DRAW:
			reg_value = (unsigned char) value;
			break;
		case PEN_ERASE:
			reg_value = 0;
			break;
	}

	switch (lstate) {
		case 1:
		{
			/* start draw */
			add_pt = 1;
		#ifdef DEBUG
			fprintf(stderr,"\tStart Draw x: %d\t y: %d\n",lx,ly);
		#endif

		} break;
		case 2:
		{
			/* move draw */
			add_pt = 1;
		#ifdef DEBUG
			fprintf(stderr,"\tMove Draw x: %d\t y: %d\n",lx,ly);
		#endif

		} break;
		case 3:
		{
			/* end draw */
			if (dmstr->prev_state != 3 && dmstr->prev_state != -1 )
			{
				add_pt = 1;
		#ifdef DEBUG
				fprintf(stderr,"\tEnd Draw x: %d\t y: %d\n",lx,ly);
		#endif
			}
		}
	}
	dmstr->prev_state = lstate;

	if ((!add_pt)&&((cmd == CMD_POINT)||(cmd == CMD_LINE)))
		return (1);

	setup_environment(dmstr);
	first	= !dmstr->npoints;
	npoints = dmstr->npoints;

	int *mask_dims = (int *)mask.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!mask_dims)
		return 0;

	float *mask_points = (float *)mask.points.ret_array_ptr(OM_GET_ARRAY_RD);
	if (!mask_points) {
		if(mask_dims)
			ARRfree(mask_dims);
		return 0;
	}

	int	 mask_data_size=1;
	for (i=0; i<(int)mask.ndim; i++) {
		mask_data_size *= mask_dims[i];
	}
#ifdef DEBUG
	fprintf(stderr,"mask.nnodes %i\n",(int)mask.nnodes);
	fprintf(stderr,"mask.nnode_data %i\n",(int)mask.nnode_data);
	fprintf(stderr,"mask_data_size %i\n",mask_data_size);
#endif

	if ((mask.node_data[0].veclen.valid_obj() == 0) ||
		((int)mask.node_data[0].veclen) != 1 )
		mask.node_data[0].veclen = 1;

	if ((mask.node_data[0].null_flag.valid_obj() == 0) ||
		((int)mask.node_data[0].null_flag != 1) )
		mask.node_data[0].null_flag	 = 1;

	if ((mask.node_data[0].null_value.valid_obj() == 0) ||
		((int)mask.node_data[0].null_value != 0) )
		mask.node_data[0].null_value = 0;

	GDmap_get_matrix(dmstr->view, dmstr->camera, dmstr->object, &mat[0][0], 1);

	/*	  Take the inverse of the matrix and map the points */

	MATmat4_inverse(invmat, mat);
	pt[0]=lx;
	pt[1]=ly;
	GDmap_ss_to_2dworld(dmstr->camera, &invmat[0][0], 1, pt, opt);

	// Need to map from coordinate space to array dimensions
	// Assuming the mask is a 2D slice.
	float x_span = mask_points[2] - mask_points[0];
	float y_span = mask_points[3] - mask_points[1];

	opt[0] = ((opt[0] - mask_points[0])/x_span) * (mask_dims[0] - 1);
	opt[1] = ((opt[1] - mask_points[1])/y_span) * (mask_dims[1] - 1);

	//unsigned char *mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
	unsigned char *mask_data = NULL;
	int points_size;
	float *out_points=NULL;

	// User just clicked on the region2d button, don't
	// accidently skip the next click in the viewer.
	if ((cmd==CMD_REGION_2D)&&(dmstr->precmd!=CMD_REGION_2D)) {
		dmstr->prev_x = -1;
		dmstr->prev_y = -1;
	}
	// User just clicked on the region3d button, don't
	// accidently skip the next click in the viewer.
	if ((cmd==CMD_REGION_3D)&&(dmstr->precmd!=CMD_REGION_3D)) {
		dmstr->prev_x = -1;
		dmstr->prev_y = -1;
	}

	if (((cmd==CMD_REGION_2D)&&(dmstr->precmd==CMD_REGION_2D)) ||
		((cmd==CMD_REGION_3D)&&(dmstr->precmd==CMD_REGION_3D))) {
		// Don't do a repeat region grow because it can be expensive
//		  if ((dmstr->prev_x != lx) || (dmstr->prev_y != ly)) {
			dmstr->prev_x = lx;
			dmstr->prev_y = ly;
			 if (cmd==CMD_REGION_2D) {
				seed[0]=(int)floor(opt[0]);
				seed[1]=(int)floor(opt[1]);
				seed[2]=0;
				grow_dims[0]=mask_dims[0];
				grow_dims[1]=mask_dims[1];
				grow_dims[2]=0;
				run_region=1;
			}
			else {
				int seed_point_size;
				int* seed_point_arr = (int *)seed_point.ret_array_ptr(OM_GET_ARRAY_WR, &seed_point_size);
				if (seed_point_arr) {
//					  draw=1;
//					  mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE);
					switch ((int)axis) {
						case 0:{
							seed_point_arr[0]=(int)slice;
							seed_point_arr[1]=(int)floor(opt[0]);
							seed_point_arr[2]=(int)floor(opt[1]);
						} break;
						case 1:{
							seed_point_arr[0]=(int)floor(opt[0]);
							seed_point_arr[1]=(int)slice;
							seed_point_arr[2]=(int)floor(opt[1]);
						} break;
						case 2:{
							seed_point_arr[0]=(int)floor(opt[0]);
							seed_point_arr[1]=(int)floor(opt[1]);
							seed_point_arr[2]=(int)slice;
						}
					} // switch axis
				} // if
//				  maskinspt (opt[0], opt[1], mask_dims, mask_data, reg_value);
				ARRfree(seed_point_arr);
				seed_point_arr = NULL;
			}
//		  } // if
	} // if

	switch (cmd) {
		case CMD_LINE:
		case CMD_POINT:
		{
			if ((dmstr->prev_x != lx) || (dmstr->prev_y != ly))
			{
				++npoints;
				mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
				out_points = (float *)points.ret_array_ptr(OM_GET_ARRAY_RW,&points_size);
				#ifdef DEBUG
					fprintf(stderr,"add_pt:End Draw x: %d\t y: %d\n",lx,ly);
				#endif
				if (out_points) {
					out_points[2*npoints-2] = opt[0];
					out_points[2*npoints-1] = opt[1];
					{ ARRfree(out_points); out_points=NULL;}
				}
				#ifdef DEBUG
					fprintf(stderr,"prev_x_mask: %f, prev_y_mask: %f, opt[0]: %f, opt[1]: %f\n",dmstr->prev_x_mask,dmstr->prev_y_mask,opt[0],opt[1]);
				#endif
				if (!first)
				{
					lpt[0] = dmstr->prev_x_mask;
					lpt[1] = dmstr->prev_y_mask;
					lpt[2] = opt[0];
					lpt[3] = opt[1];
					#ifdef DEBUG
						fprintf(stderr,"lpt[0]: %f, lpt[1]: %f, lpt[2]: %f, lpt[3]: %f\n",lpt[0],lpt[1],lpt[2],lpt[3]);
					#endif
					if (cmd == CMD_LINE)
						maskDrawBresLine2D(lpt, mask_dims, mask_data, lwidth, reg_value);
					if (cmd == CMD_POINT)
						masklldraw(opt[0],opt[1], mask_dims, mask_data, lwidth, reg_value);
				}
				else {
					masklldraw(opt[0],opt[1], mask_dims, mask_data, lwidth, reg_value);
				}
				dmstr->npoints=npoints;
				dmstr->prev_x = lx;
				dmstr->prev_y = ly;
				dmstr->prev_x_mask = opt[0];
				dmstr->prev_y_mask = opt[1];

				draw=1;	   // trigger SegEditorCore
			}
		} break;
		case CMD_CLOSE:
		{
			pts = dmstr->npoints;
			mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
			out_points = (float *)points.ret_array_ptr(OM_GET_ARRAY_RD,&points_size);
			if (out_points)
			{
				lpt[0]=out_points[2*pts-2];
				lpt[1]=out_points[2*pts-1];
				for(i=0;i<pts;i++){
					lpt[2] = out_points[2*i];
					lpt[3] = out_points[2*i+1];
					maskDrawBresLine2D(lpt, mask_dims, mask_data, lwidth, reg_value);
					lpt[0] = lpt[2];
					lpt[1] = lpt[3];
				}
				if (dmstr->precmd!=-1) {
					command = dmstr->precmd;
					cmd = dmstr->precmd;
				}
				ARRfree(out_points);
				out_points=NULL;
				draw=1;
			}
		} break;
		case CMD_NEW:
		{
			init_environment(cmd);
			npoints=0;
			OMset_obj_val(points, OMnull_obj, 0);
		} //FALLTHROUGH
		case CMD_CLEAR:
		{
			if (mask_data==NULL) mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
			for (i=0;i<mask_data_size;i++)
				mask_data[i]=0;
			if (dmstr->precmd!=-1) {
				command = dmstr->precmd;
				cmd = dmstr->precmd;
			}
			draw=1;
		} break;
		case CMD_INVERT:
		{
			mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
			for (i=0;i<mask_data_size;i++) {
				mask_data[i]=(mask_data[i])?0:(unsigned char)value;
			}
			if (dmstr->precmd !=- 1) {
				command = dmstr->precmd;
				cmd = dmstr->precmd;
			}
			draw=1;
		} break;
		case CMD_FILL:
		{
			if (dmstr->precmd==CMD_FILL) {
				mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
				if (dmstr->precmd!=-1){
					command = dmstr->precmd;
					cmd = dmstr->preprecmd;
				}
				maskflood (opt[0], opt[1], mask_dims, mask_data, reg_value);
			}
			draw=1;
		} break;
		case CMD_REGION_2D:
		{
			if (run_region) {
				reg_data = (double*)in_slice.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RD,OM_TYPE_DOUBLE,&slice_data_size);
				mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
				grow = new segment_grow_region(grow_dims,reg_data,&mask_data);
				grow->set_params(seed[0], seed[1], seed[2], (double) reg_delta, (int) reg_incr);
				grow->set_out_value(reg_value,0,(int)regcmd);
				grow->grow(seed[0], seed[1], seed[2]);
				delete grow;
				if (reg_data)
					{ ARRfree(reg_data); reg_data=NULL;}
				draw=1;
			}
		} break;
		case CMD_THRESH:
		{
			if (thr_trig) {
				thr_trig = 0;
				if (thr_comp<in_slice.nnode_data) {
					mask_data = (unsigned char *)mask.node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW, OM_TYPE_BYTE, &mask_data_size);
					slice_data = (char*)in_slice.node_data[thr_comp].values.ret_array_ptr(OM_GET_ARRAY_RD,&slice_data_size, &slice_data_type);
					switch (slice_data_type)
					{
						case OM_TYPE_BYTE:
							THRESH_MASK_DATA(unsigned char);
						break;
						case OM_TYPE_CHAR:
							THRESH_MASK_DATA(char);
						break;
						case OM_TYPE_SHORT:
							THRESH_MASK_DATA(short);
						break;
						case OM_TYPE_INT:
							THRESH_MASK_DATA(int);
						break;
						case OM_TYPE_FLOAT:
							THRESH_MASK_DATA(float);
						break;
						case OM_TYPE_DOUBLE:
							THRESH_MASK_DATA(double);
						break;
					}
					if (slice_data)
						{ ARRfree(slice_data); slice_data=NULL;}
					draw=1;
				}
			}
		} // case
	} // switch cmd

	dmstr->preprecmd = dmstr->precmd;
	dmstr->precmd = cmd;

	if (mask_dims)
		ARRfree(mask_dims);
	if (mask_points)
		ARRfree(mask_points);
	if (mask_data)
		ARRfree(mask_data);

	// return 1 for success
	return(1);
}

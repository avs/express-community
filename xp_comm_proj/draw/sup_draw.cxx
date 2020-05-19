//
//	Super polyline editor for AVS/Express
//
//	Provides more control and speed compared to Click Sketch.
//
//	command:
//		0		no op
//		1		add point at the end of the polyline
//		2		add point between
//		3		move point
//		4		delete point
//		5		delete all points
//		6		translate the whole line
//
//
//	Author: Mario Valle
//	Date:	20-may-2000
//

#include <avs/util.h>
#include <avs/om.h>
#include <avs/om_type.h>
#include <avs/gd.h>
#include <malloc.h>
#include "sd_gen.h"

//
//	Adapted interface to draw routines (the official definition lives in <gd/sw/swx.h>)
//
extern "C" void GD2d_draw_lines(GDstate *, int, short[], float[], float[]);
extern "C" void	GD2d_draw_polyline(GDstate *, int, short[], float[], float[], int);

//
//	Point list initial size (multiplied by INCREMENT every time overflows)
//
#define	INITIAL_SIZE	50
#define INCREMENT		 2

//
//	Commands
//
#define	CMD_INACTIVE	0
#define	CMD_APPEND		1
#define	CMD_INSERT		2
#define	CMD_MOVE		3
#define	CMD_DELETE		4
#define	CMD_RESET		5
#define	CMD_TRANSLATE	6

//
//	Line types
//
#define LINE_OPEN		0
#define LINE_CLOSED		1

//
//	Mouse sensitivity
//
#define TRESHOLD 2

//
//	Distance measurement functions (defined in Distance.cxx)
//
extern float point_segment_squared_dist(short px, short py, short segstartx, short segstarty, short segendx, short segendy);
extern int find_nearest_segment(short *pt, int npt, short x, short y);
extern int find_nearest_point(short *pt, int npt, short x, short y);

//
//	Enlarge the points list if needed
//
static void check_size(struct SuperDrawStatus *st)
{
	if(st->npt >= st->len-1)
	{
		st->len *= INCREMENT;
		st->pt = (short *)realloc(st->pt, st->len*sizeof(short)*2);
	}
}

//
//	Reads camera, view and object. Return 1 on error
//
int Draw_DrawMods_SuperDrawCore::setup_environment(struct SuperDrawStatus *st)
{
	OMobj_id view_id, cam_id, obj_id, tmp_id;
	
	//
	//	Get the view structure pointer
	//
	view_id = OMfind_subobj(*this, OMstr_to_name("view"), OM_OBJ_RD);
	if (OMis_null_obj(view_id))
	{
		ERRerror("SuperDraw::setup_environment", 1, ERR_ORIG, "Can't get view\n");
		return 1;
	}
	st->view = (GDview *)GDget_local(view_id, (OMpfi)GDview_init);
	if (!st->view)
	{
		ERRerror("SuperDraw::setup_environment", 1, ERR_ORIG, "Can't get view information\n");
		return 1;
	}
	if (!st->view->Winfo || !st->view->State)
	{
		//ERRerror("SuperDraw::setup_environment", 1, ERR_ORIG, "Winfo or State not initialized in view\n");
		return 1;
	}

	//
	//	Get the camera
	//
	tmp_id = OMfind_subobj(*this, OMstr_to_name("camera"), OM_OBJ_RD);
	if (OMis_null_obj(tmp_id))
		return 1;
	if (OMget_obj_val(tmp_id, &cam_id) != OM_STAT_SUCCESS) 
	{
		ERRerror("SuperDraw::setup_environment", 1, ERR_ORIG, "Can't create output without camera\n");
		return 1;
	}
	st->camera = (GDcamera *)GDget_local(cam_id, (OMpfi)GDcamera_create);
	if (!st->camera)
	{
		ERRerror("SuperDraw::setup_environment", 1, ERR_ORIG, "Can't create output without camera\n");
		return 1;
	}
	
	//
	//	The object is optional. If we don't have one the points
	//	are mapped back to the camera's space.
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

//
//	Output world coordinates points
//
void Draw_DrawMods_SuperDrawCore::output_points(struct SuperDrawStatus *st)
{
	float  mat[4][4], invmat[4][4];
	int    size;
	float *points_arr;
	
	//
	//	Open the line if the user ask for an open line and the line was closed
	//	and the number of points was >=4
	//
	if(st->is_closed && (!st->want_line_closed || st->npt < 4))
	{
		st->is_closed = false;
		st->npt--;
	}

	//
	//	Close the line if the user so requested and the line was open
	//	and there is a sufficient number of points
	//
	else if(st->want_line_closed && !st->is_closed && st->npt > 2)
	{
		st->is_closed = true;
		st->pt[st->npt*2+0] = st->pt[0];
		st->pt[st->npt*2+1] = st->pt[1];
		st->npt++;
		check_size(st);
	}

	npoints = st->npt;
	if(!st->npt)
	{
		points.set_array(OM_TYPE_FLOAT, NULL, 0, OM_SET_ARRAY_FREE);
		npoints = 0;
	}
	else
	{
		points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_WR, &size);
		if(points_arr)
		{
			//
			//	Get the full matrix associated with the object. This
			//	includes the camera matrix and all object matricies 
			//	including the object's.
			//
			GDmap_get_matrix(st->view, st->camera, st->object, &mat[0][0], 1);

			//
			//	Take the inverse of the matrix and map the points
			//
			MATmat4_inverse(invmat, mat);
			GDmap_ss_to_2dworld(st->camera, &invmat[0][0], st->npt, st->pt, points_arr);
			ARRfree(points_arr);
		}
	}
}

//
//	Map world coordinates points back to screen coordinates. Also syncronize the internal list of points
//
void Draw_DrawMods_SuperDrawCore::setup_pt(struct SuperDrawStatus *st)
{
	float  mat[4][4];
	int    size;
	float *points_arr;
	
	points_arr = (float *)points.ret_array_ptr(OM_GET_ARRAY_RD, &size);
	if(points_arr)
	{
		if(size/2 >= st->len)
		{
			free((void *)st->pt);
			st->len = size/2;
			st->pt = (short *)malloc(sizeof(short)*2*size/2);
		}
		st->npt = size/2;
		if(size)
		{
			float *points;

			ALLOCN(points, float, st->npt*2, "can't alloc line buffer");

			//
			//	Get the full matrix associated with the object. This
			//	includes the camera matrix and all object matricies 
			//	including the object's.
			//
			GDmap_get_matrix(st->view, st->camera, st->object, &mat[0][0], 1);
			
			//
			//	Multiply the coordinates for the transformation matrix
			//
			MATxform_verts2(st->npt, (Matr2*)points_arr, (Matr4*)mat, (Matr2*)points);
			
			//
			//	Then correct the position back to screen coordinates.
			//	The first step is to correct the y positions which 
			//	are from the windowing system which has 0,0 in the
			//	upper left to world space where 0,0 is in the center
			//	and xmax, ymax is in the upper left.
			//	While doing this we also transform the points from the -1,1 range
			//	back to their screen space values.  
			//
			for(int i=0; i<st->npt; i++) 
			{
				double xs = (points[2*i+0] + 1.0)*(st->camera->vp_xmax - st->camera->vp_xmin)/2. + st->camera->vp_xmin;
				double ys = -((points[2*i+1] + 1.0)*(st->camera->vp_ymax - st->camera->vp_ymin)/2. - st->camera->vp_ymax + st->camera->vp_ymin);
				st->pt[2*i+0] = (short)(xs + 0.5);
				st->pt[2*i+1] = (short)(ys + 0.5);
			}
			
		   // Free the temporary points array.
		   FREE(points);
		}
		ARRfree(points_arr);
	}
	else
	{
		st->npt = 0;
	}
}

int
Draw_DrawMods_SuperDrawCore::OnInstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)
	
	struct SuperDrawStatus *st;
	st = (struct SuperDrawStatus *)malloc(sizeof(struct SuperDrawStatus));
	if(!st) return 0;
	OMset_ptr_val((OMobj_id)local_ptr, (void *)st, 0);

	st->prev_state = 3;
	st->npt = 0;
	st->len = INITIAL_SIZE;
	st->pt = (short *)malloc(sizeof(short)*2*INITIAL_SIZE);
	st->is_closed = false;
	st->want_line_closed = false;
	st->must_update = false;
	st->first_point = false;

	// return 1 for success
	return 1;
}

int
Draw_DrawMods_SuperDrawCore::OnDeinstance(OMevent_mask event_mask, int seq_num)
{
	// local_ptr (OMXptr read write)
	
	struct SuperDrawStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st) return 0;
	free((void *)st->pt);
	free((void *)st);

	OMset_ptr_val((OMobj_id)local_ptr, (void *)0, 0);

	// return 1 for success
	return 1;
}

int
Draw_DrawMods_SuperDrawCore::OnDraw(OMevent_mask event_mask, int seq_num)
{
	// x (OMXfloat read req)
	// y (OMXfloat read req)
	// state (OMXint read req notify)
	// camera (GDcamera_templ read)
	// obj (GDobject_templ read req notify)
	// view (GDview_templ read req notify)
	// command (OMXint read req notify)
	// npoints (OMXint write)
	// local_ptr (OMXptr read)

	short lines[16];
	int i, size, idx;

	//
	//	Access the local status and check for correct initialization.
	//
	struct SuperDrawStatus *st;
	OMget_ptr_val((OMobj_id)local_ptr, (void **)&st, 0);
	if(!st || !st->pt) return 0;

	//
	//	Get Camera, View and Object
	//
	if(setup_environment(st))
		return 0;

	//
	//	Record the line type requested by the user.
	//	The real setup is in output_points()
	//
	st->want_line_closed = ((int)line_type == LINE_CLOSED);
	if(line_type.changed(seq_num))
	{
		output_points(st);
	}

	//
	//	Do nothing
	//
	if((int)command == CMD_INACTIVE) return 0;

	//
	//	Read screen coordinates on button down
	//
	if((int)command != CMD_RESET && state == 1) setup_pt(st);

	//
	//	Push start of rendering marker onto the stack so we can
	//	restore original attributes when we are done.
	//
	GDstack_push(st->view);
	
	//
	//	Set the color to be used for drawing
	//
	float white[3];
	float *colors_arr = (float *)colors.ret_array_ptr(OM_GET_ARRAY_RD, &size);
	if(!colors_arr || size != 3)
	{
		white[0] = white[1] = white[2] = 1.0;
		colors_arr = white;
	}
	GDstate_set_color(st->view, colors_arr);
	
	//
	//	Tell GD to use the front buffer (ie. window) as the
	//	drawble. This allows our drawing to be visible even if
	//	we are in double buffer mode.
	//	Also set draw mode to XOR.
	//
	GDstate_set_drawable(st->view, 1);
	GDstate_set_draw_mode(st->view, 1);
	GDstate_set_line_width(st->view, ((int)width < 0) ? 0 : (int)width);

	//
	//	Now execute the command
	//
	switch(command)
	{
	case CMD_INACTIVE:
		//	No-op
		break;

	case CMD_APPEND:
		//	Append point (only to an open line)
		if(!st->is_closed)
		{
			switch(state)
			{
			case 1:
				if(st->npt == 0)
				{
					st->npt = 1;
					st->pt[0] = x;
					st->pt[1] = y;
					st->first_point = true;
					st->must_update = true;
				}
				else
				{
					lines[0] = st->pt[st->npt*2-2];
					lines[1] = st->pt[st->npt*2-1];
					lines[2] = x;
					lines[3] = y;

					GD2d_draw_lines(st->view->State, 2, lines, NULL, NULL);
				}
				break;

			case 2:
				lines[0] = st->pt[st->npt*2-2];
				lines[1] = st->pt[st->npt*2-1];
				lines[2] = st->prev_x;
				lines[3] = st->prev_y;
				lines[4] = st->pt[st->npt*2-2];
				lines[5] = st->pt[st->npt*2-1];
				lines[6] = x;
				lines[7] = y;

				GD2d_draw_lines(st->view->State, 4, lines, NULL, NULL);
				if(!(((st->prev_x - x) < TRESHOLD) && ((st->prev_x - x) > -TRESHOLD) &&
					 ((st->prev_y - y) < TRESHOLD) && ((st->prev_y - y) > -TRESHOLD)))
					st->first_point = false;
				break;

			case 3:
				if(st->prev_state == 3) break;

				if(st->first_point)
				{
					st->first_point = false;
				}
				else
				{
					lines[0] = st->pt[st->npt*2-2];
					lines[1] = st->pt[st->npt*2-1];
					lines[2] = st->prev_x;
					lines[3] = st->prev_y;

					GD2d_draw_lines(st->view->State, 2, lines, NULL, NULL);

					st->pt[st->npt*2+0] = x;
					st->pt[st->npt*2+1] = y;
					st->npt++;
					check_size(st);
					st->must_update = true;
				}
			}
			break;
		}
		// FALLTHROUGH

	case CMD_INSERT:
		//	Add point between
		if(st->npt < 2) break;
		switch(state)
		{
		case 1:
			st->prev_idx = find_nearest_segment(st->pt, st->npt, x, y);
			st->npt++;
			check_size(st);
			for(i=st->npt-2; i > st->prev_idx; i--)
			{
				st->pt[2*i+2] = st->pt[2*i+0];
				st->pt[2*i+3] = st->pt[2*i+1];
			}
			st->pt[2*st->prev_idx+2] = x;
			st->pt[2*st->prev_idx+3] = y;
			GD2d_draw_polyline(st->view->State, 3, &st->pt[2*st->prev_idx], NULL, NULL, 0);
			break;
		case 2:
			GD2d_draw_polyline(st->view->State, 3, &st->pt[2*st->prev_idx], NULL, NULL, 0);
			st->pt[2*st->prev_idx+2] = x;
			st->pt[2*st->prev_idx+3] = y;
			GD2d_draw_polyline(st->view->State, 3, &st->pt[2*st->prev_idx], NULL, NULL, 0);
			break;
		case 3:
			if(st->prev_state == 3) break;
			GD2d_draw_polyline(st->view->State, 3, &st->pt[2*st->prev_idx], NULL, NULL, 0);
			st->pt[2*st->prev_idx+2] = x;
			st->pt[2*st->prev_idx+3] = y;
			st->must_update = true;
		}
		break;

	case CMD_MOVE:
		//	Move point
		if(st->npt == 0) break;
		switch(state)
		{
		case 1:
			st->prev_idx = find_nearest_point(st->pt, st->npt, x, y);

			if(st->is_closed)
			{
				st->llen = 3;
				if(st->prev_idx == 0 || st->prev_idx == st->npt-1)
				{
					st->l[0] = st->pt[2*st->npt-4];
					st->l[1] = st->pt[2*st->npt-3];
					st->l[2] = x;
					st->l[3] = y;
					st->l[4] = st->pt[2];
					st->l[5] = st->pt[3];
				}
				else if(st->prev_idx == st->npt-2)
				{
					st->l[0] = st->pt[2*st->prev_idx-2];
					st->l[1] = st->pt[2*st->prev_idx-1];
					st->l[2] = x;
					st->l[3] = y;
					st->l[4] = st->pt[0];
					st->l[5] = st->pt[1];
				}
				else
				{
					st->l[0] = st->pt[2*st->prev_idx-2];
					st->l[1] = st->pt[2*st->prev_idx-1];
					st->l[2] = x;
					st->l[3] = y;
					st->l[4] = st->pt[2*st->prev_idx+2];
					st->l[5] = st->pt[2*st->prev_idx+3];
				}
			}
			else
			{
				if(st->prev_idx == 0)
				{
					st->llen = 2;
					st->l[0] = st->pt[2];
					st->l[1] = st->pt[3];
					st->l[2] = x;
					st->l[3] = y;
				}
				else if(st->prev_idx == st->npt-1)
				{
					st->llen = 2;
					st->l[0] = st->pt[2*st->npt-4];
					st->l[1] = st->pt[2*st->npt-3];
					st->l[2] = x;
					st->l[3] = y;
				}
				else
				{
					st->llen = 3;
					st->l[0] = st->pt[2*st->prev_idx-2];
					st->l[1] = st->pt[2*st->prev_idx-1];
					st->l[2] = x;
					st->l[3] = y;
					st->l[4] = st->pt[2*st->prev_idx+2];
					st->l[5] = st->pt[2*st->prev_idx+3];
				}
			}
			GD2d_draw_polyline(st->view->State, st->llen, st->l, NULL, NULL, 0);
			break;

		case 2:
			GD2d_draw_polyline(st->view->State, st->llen, st->l, NULL, NULL, 0);
			st->l[2] = x;
			st->l[3] = y;
			GD2d_draw_polyline(st->view->State, st->llen, st->l, NULL, NULL, 0);
			break;

		case 3:
			if(st->prev_state == 3) break;
			GD2d_draw_polyline(st->view->State, st->llen, st->l, NULL, NULL, 0);

			st->pt[2*st->prev_idx+0] = x;
			st->pt[2*st->prev_idx+1] = y;
			if(st->is_closed)
			{
				if(st->prev_idx == 0)
				{
					st->pt[2*st->npt-2] = x;
					st->pt[2*st->npt-1] = y;
				}
				else if(st->prev_idx == st->npt-1)
				{
					st->pt[0] = x;
					st->pt[1] = y;
				}
			}
			st->must_update = true;
			break;
		}
		break;

	case CMD_DELETE:
		//	Delete point
		if(st->prev_state != 3 && state == 3 && st->npt > 0) 
		{
			idx = find_nearest_point(st->pt, st->npt, x, y);
			if(st->is_closed)
			{
				if(idx == 0 || idx == st->npt-1)
				{
					for(i=0; i < st->npt-1; i++)
					{
						st->pt[2*i+0] = st->pt[2*i+2];
						st->pt[2*i+1] = st->pt[2*i+3];
					}
					st->pt[2*st->npt-4] = st->pt[0];
					st->pt[2*st->npt-3] = st->pt[1];
				}
				else
				{
					for(i=idx; i < st->npt-1; i++)
					{
						st->pt[2*i+0] = st->pt[2*i+2];
						st->pt[2*i+1] = st->pt[2*i+3];
					}
				}
			}
			else
			{
				for(i=idx; i < st->npt-1; i++)
				{
					st->pt[2*i+0] = st->pt[2*i+2];
					st->pt[2*i+1] = st->pt[2*i+3];
				}
			}

			// now make the change effective
			st->npt--;
			st->must_update = true;
		}
		break;

	case CMD_RESET:
		// delete all points (after asking confirmation)
		if(!ERRsync_yesno_dialog(1, "Confirmation request", "Do you confirm the whole line deletion?")) break;

		st->npt = 0;
		command = CMD_INACTIVE;
		st->is_closed = false;
		st->must_update = true;
		break;

	case CMD_TRANSLATE:
		// translate the whole line
		switch(state)
		{
		case 1:
			GD2d_draw_polyline(st->view->State, st->npt, st->pt, NULL, NULL, 0);
			break;

		case 2:
			GD2d_draw_polyline(st->view->State, st->npt, st->pt, NULL, NULL, 0);
			for(i=0; i < st->npt; i++)
			{
				st->pt[2*i+0] += (x - st->prev_x);
				st->pt[2*i+1] += (y - st->prev_y);
			}
			GD2d_draw_polyline(st->view->State, st->npt, st->pt, NULL, NULL, 0);
			break;

		case 3:
			if(st->prev_state == 3) break;
			GD2d_draw_polyline(st->view->State, st->npt, st->pt, NULL, NULL, 0);
			st->must_update = true;
			break;
		}
		break;

	default:
		ERRverror("SuperDraw::OnDraw", ERR_ERROR, "Invalid command %d\n", (int)command);
		break;
	};

	//
	//	Save current xy as prev_xy for erasure on other events
	//
	st->prev_x = x;
	st->prev_y = y;
	st->prev_state = state;

	GDstack_pop(st->view);

	//
	//	Update the output array
	//
	if(st->must_update)
	{
		output_points(st);
		st->must_update = false;
	}

	// return 1 for success
	return 1;
}


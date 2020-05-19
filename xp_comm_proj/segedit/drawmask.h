/*
			Copyright (c) 2004 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

	This file is under Perforce control
	$Id: //depot/express/trunk/segm_edtr/drawmask.h#1 $
*/

#ifndef _SEGEDIT_DRAWMASK_H_
#define _SEGEDIT_DRAWMASK_H_

typedef struct _DrawMaskStruct_ {

	/* Previous mouse event */

	int prev_x;
	int prev_y;
	float prev_x_mask;
	float prev_y_mask;
	int prev_state;
	int npoints;

	/* Index to the nearest point/segment */

	int prev_idx;
	int precmd;
	int preprecmd;

	/* Graphic data */

	GDview	 *view;
	GDcamera *camera;
	GDobject *object;

} DrawMaskStruct;

#endif

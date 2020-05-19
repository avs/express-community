
struct SuperDrawStatus {
	//
	//	Previous mouse event
	//
	int prev_x;
	int prev_y;
	int prev_state;

	//
	//	Index to the nearest point/segment
	//
	int prev_idx;

	//
	//	Scratch area
	//
	short l[6];
	short llen;

	//
	//	Array of points in screen coordinates.
	//	Len is the currently allocated points array lenght
	//
	int    len;
	int    npt;
	short *pt;

	//
	//	Flags
	//
	bool want_line_closed;		//	the user request a closed line
	bool is_closed;				//	the line is really closed
	bool must_update;			//	the output must update after resetting GD status
	bool first_point;			//	this is the first clicked point

	//
	//	Graphic data
	//
	GDview   *view;
	GDcamera *camera;
	GDobject *object;
};

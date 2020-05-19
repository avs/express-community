//
// profile.cxx - build a profile image from
// mouse event in window
//
// I. Curington, July 97


#include "express.h"


int
Profile_ProfileMods_profile::update(OMevent_mask event_mask, int seq_num)
{
	// nx (OMXint read req notify)
	// ny (OMXint read req notify)
	// x (OMXint read req notify)
	// y (OMXint read req notify)
	// state (OMXint read req notify)
	// mode (OMXint read req notify)
	// px (OMXint read write)
	// py (OMXint read write)
	// out_nx (OMXint write)
	// out_ny (OMXint write)
	// map (OMXbyte_array read write)
	// line (OMXint_array read write)

	int map_size;
	unsigned char *map_arr;
	int line_size;
	int *line_arr;
	int local_nx;
	int local_ny;
	int local_x;
	int local_y;
	int local_state;
	int local_mode;
	int local_px;
	int local_py;
	int i,j,k,dx,dy,iy;
	float rate;
	
	/***********************/
	/* Function's Body	  */
	/***********************/
	// ERRerror("lathe profile update",1,ERR_ORIG, "entry"); 
	
	//
	// get local copies for efficiency
	//
	local_nx = nx;
	local_ny = ny;
	local_x = x;
	local_y = y;
	local_state = state;
	local_mode = mode;

	//
	// clip to mouse position to window
	//
	if ( local_x < 0 ) local_x = 0;
	if ( local_y < 0 ) local_y = 0;
	if ( local_x >= local_nx ) local_x = local_nx-1;
	if ( local_y >= local_ny ) local_y = local_ny-1;

	// initialize previous position if first mouse down
	if ( local_state == 1 ) {
		  px = local_x;
		  py = local_y;
		  out_nx = local_nx;
		  out_ny = local_ny;
	}
	local_px = px;
	local_py = py;

	//
	// work with array pointer
	//

	// only operate on mouse down and drag, ignore up
	if (local_state == 1 || local_state == 2 )
	{
		// only operate on mouse down and drag
		line_arr = (int *)line.ret_array_ptr(OM_GET_ARRAY_RW,&line_size);
		if (line_arr == 0) {
			 ERRerror("lathe profile update",1,ERR_ORIG, "could not get line array ptr"); 
			 return(0);
		};

		map_arr = (unsigned char *)map.ret_array_ptr(OM_GET_ARRAY_RW,&map_size);
		if (map_arr) {
			 /* set pixel */
			 // ERRerror("lathe profile update",1,ERR_ORIG, "array set"); 

			 // swap if reverse direction
			 if ( local_py > local_y ) {
					  j = local_px;
					  local_px = local_x;
					  local_x = j;
					  j = local_py;
					  local_py = local_y;
					  local_y = j;
			 }

			 dx = local_x - local_px;
			 dy = local_y - local_py;
			 if ( dy != 0 )
				  rate = (float)dx / (float)dy;
			 else
				  rate = 0;

			 for (i=0; i < dy; i++) { 

				  //
				  // update image under mouse
				  //
				  j  = (int)((float)local_px + ( rate * (float)i  ));
				  if ( j < 0 ) j = 0;
				  if ( local_nx <= j ) j = local_nx -1;

				  iy = local_ny - (local_py + i) -1;
				  if ( iy < 0 ) iy = 0;
				  if ( local_ny <= iy ) iy = local_ny -1;

				  if (local_mode == 0)
				  {
						// values below line
						for (k=0; k<j; k++)
							 map_arr[ (iy) * local_nx + (k) ] = 0;
						// values at line
							 map_arr[ (iy) * local_nx + (j) ] = 255;
						// values above line
						for (k=j+1; k<local_nx; k++)
							 map_arr[ (iy) * local_nx + (k) ] = 30;
				  }
				  else if (local_mode == 1)
				  {
						// values below line
						for (k=0; k<j; k++)
							 map_arr[ (iy) * local_nx + (k) ] = j;
						// values at line
							 map_arr[ (iy) * local_nx + (j) ] = local_nx;
						// values above line
						for (k=j+1; k<local_nx; k++)
							 map_arr[ (iy) * local_nx + (k) ] = 0;
				  }
				  else if (local_mode == 2)
				  {
						// values below line
						for (k=0; k<j; k++)
							 map_arr[ (iy) * local_nx + (k) ] = 0;
						// values at line
							 map_arr[ (iy) * local_nx + (j) ] = local_nx;
						// values above line
						for (k=j+1; k<local_nx; k++)
							 map_arr[ (iy) * local_nx + (k) ] = j;
				  }

				  //
				  // update line plot value
				  //
				  k = local_py + i;
				  if ( k < 0 ) k = 0;
				  if ( local_ny <= k ) k = local_ny -1;
				  line_arr[ k ] = j;
			 }


			 ARRfree((char *)map_arr);
			 ARRfree((char *)line_arr);
		}
		else
		{
			 ERRerror("lathe profile update",1,ERR_ORIG, "could not get map array ptr"); 
		}
		// update history for next time around on drag (non-swapped)
		px = x;
		py = y;
	}

	// return 1 for success
	return(1);
}



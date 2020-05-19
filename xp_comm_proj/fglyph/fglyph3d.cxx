//
// Fast Mesh Glyph 3D module
//
// Mark Lambert, August 1999
// Advanced Visual Systems, Inc.
//
//

#include "xp_comm_proj/fglyph/gen.h"
#include <math.h>

#define PI 3.14159265359

		//free arrays		
#define FGLYPH_ARR_FREE {\
	if (glyph_fld_coordinates_arr) ARRfree(glyph_fld_coordinates_arr);\
	if (coordOut_arr) ARRfree(coordOut_arr);\
	if (coords_arr)	ARRfree(coords_arr);\
	if (glyph_fld_connect_list_arr)	ARRfree(glyph_fld_connect_list_arr);\
	if (node_connect_list_arr) ARRfree(node_connect_list_arr);\
}

  //clear coordinates, display error message and return
#define ERR_RETURN(MESS) {\
	ncoordOut = 0;\
	cell_nnodes = 0;\
  ERRerror("FastGlyph3DCore.update",1,ERR_ORIG, MESS);\
  return(0);\
}




int
FastGlyph_FastGlyphMods_FastGlyph3DCore::update(OMevent_mask event_mask, int seq_num)
{
	// coords (OMXfloat_array read notify)
	int coords_size;
	float *coords_arr = NULL;
	
	// ncoords (OMXint read)
	
	// scale (OMXfloat read)
	// glyph_fld (Mesh read req)
	// glyph_fld.nnodes (int)
	// glyph_fld.coordinates.values (float [])
	int glyph_fld_coordinates_size;
	float *glyph_fld_coordinates_arr = NULL;
	
	// node_connect_list (OMXint_array write)
	int glyph_fld_connect_list_size;
	int *glyph_fld_connect_list_arr = NULL;


	// ncoordOut (OMXint write)
	// coordOut (OMXfloat_array write)
	int coordOut_size;
	float *coordOut_arr = NULL;
	
	int node_connect_list_size;
	int *node_connect_list_arr = NULL;
	
	/***********************/
	/* Function's Body     */
	/***********************/
	int i, j, nnodes, nspace, o_ncoords;
	float scaleVal,newx,newy,newz;
	
		
	// if empty input set output dimensions to zero
	if ( (!ncoords.valid_obj()) || (ncoords == 0) || ((ncoords%3)!=0) || (glyph_fld.nspace<2) ) {
		ncoordOut = 0;
		cell_nnodes = 0;
	}
	else {
  	o_ncoords = ncoords / 3;

		coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_RD,&coords_size);
    if ((coords_arr==NULL) || (coords_size!=ncoords)) {
      FGLYPH_ARR_FREE;
      ERR_RETURN("Unable to get input coordinates");
    }

		//if scale valid use scale otherwise use default of 1
		if (scale.valid_obj())
			scaleVal = scale;
		else
			scaleVal = 1;


		//set number of output coordinates
		ncoordOut = o_ncoords * glyph_fld.nnodes;
		coordOut_arr = (float *)coordOut.ret_array_ptr(OM_GET_ARRAY_WR,&coordOut_size);
    if ((coordOut_arr==NULL) || (coordOut_size!=(ncoordOut*3))) {
      FGLYPH_ARR_FREE;
      ERR_RETURN("Unable to allocate output coordinate array");
    }


		//set output cell information - presume that only one cell_set exists
		ncells = glyph_fld.cell_set[0].ncells * o_ncoords;
		cell_ndim = glyph_fld.cell_set[0].cell_ndim;
		cell_nnodes = glyph_fld.cell_set[0].cell_nnodes;

	  glyph_fld_coordinates_arr = (float *)glyph_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD,&glyph_fld_coordinates_size);
    if (glyph_fld_coordinates_arr==NULL) {
      FGLYPH_ARR_FREE;
      ERR_RETURN("Unable to get glyph coordinates array");
    }

		glyph_fld_connect_list_arr = (int *)glyph_fld.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD,&glyph_fld_connect_list_size);
    if (glyph_fld_connect_list_arr==NULL) {
      FGLYPH_ARR_FREE;
      ERR_RETURN("Unable to get glyph connectivity array");
    }


    //get output node connectivity array
		node_connect_list_arr = (int *)node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR,&node_connect_list_size);
    if ( (node_connect_list_arr==NULL) || (node_connect_list_size!=(ncells*cell_nnodes)) ) {
      FGLYPH_ARR_FREE;
      ERR_RETURN("Unable to allocate output connectivity array");
    }
	
		nspace = glyph_fld.nspace;
		nnodes = glyph_fld.nnodes;
		
    // Indicate on status bar that module is computing
    // Save time by NOT updating status during computation
    OMpush_status_range(0, 100);
    OMstatus_check(1, "Generating Glyph Data", NULL);

		//calculate the output coordinates and connectivity
		for (i = 0; i < o_ncoords; i++){

			//calculate coordinates
			for (j = 0; j < nnodes; j++){
        // TODO: Further performance could be gained from precalculating a scaled up
			  //       glyph and using that instead.  Compiler should optimise loop.

				// store x and y coordinates of glyph
				newx = glyph_fld_coordinates_arr[(j*nspace)];
				newy = glyph_fld_coordinates_arr[(j*nspace)+1];

				//translate each glyph to the points in the coords array
				coordOut_arr[(i*nnodes*3)+(j*3)]   = (newx * scaleVal) + coords_arr[(i*3)];
				coordOut_arr[(i*nnodes*3)+(j*3)+1] = (newy * scaleVal) + coords_arr[(i*3)+1];

        // Deal with optional z coordinate of glyph
        if (nspace==3) {
  				newz = glyph_fld_coordinates_arr[(j*nspace)+2];
  				coordOut_arr[(i*nnodes*3)+(j*3)+2] = (newz * scaleVal) + coords_arr[(i*3)+2];
        } else {
  				coordOut_arr[(i*nnodes*3)+(j*3)+2] = coords_arr[(i*3)+2];
        }
			}
			
			//create connectivity
			for (j = 0; j < glyph_fld_connect_list_size; j++){
				node_connect_list_arr[(i*glyph_fld_connect_list_size)+j] = glyph_fld_connect_list_arr[j] + (i*nnodes);
			}

		}
				
    // Indicate on status bar that module has finished computing
    OMstatus_check(100, "Glyph Data Generated", NULL);
    OMpop_status_range();

    // Free all allocated arrays
    FGLYPH_ARR_FREE;
	}

	// return 1 for success
	return(1);
}


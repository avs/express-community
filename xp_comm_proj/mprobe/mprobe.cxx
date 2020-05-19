/* xp_comm_proj/mprobe/mprobe.cxx
 * ----------------------------------------------------------------------
 * MultiProbe IAC Project for AVS/Express
 *
 * Description:
 * 
 *   The MultiProbeMerge module inputs an array of Grid objects, and
 *   concatenates all of their coordinates into a single grid, and also
 *   generates a point mesh, so that they may be rendered.
 *   This allows multiple probes (glyphs) to be merged to form a larger
 *   probe for use with modules such as streamlines and advect.
 * 
 * Author:
 * 
 *   Paul G. Lever
 *   International AVS Centre
 *   Manchester Visualization Centre
 *   Manchester Computing
 *   University of Manchester
 *   Oxford Road
 *   Manchester
 *   United Kingdom
 *   M13 9PL
 *
 *   Tel: +44 161 275 6252/6095
 *   Fax: +44 161 275 6800/6040
 *   Email: paul.lever@mcc.ac.uk
 *          avs@iavsc.org
 *
 * Notes:
 *   Version 1.0 - 25th February 1999
 * ----------------------------------------------------------------------
 */

#include <avs/mat.h>
#include "xp_comm_proj/mprobe/gen.h"

int
MultiProbe_MultiProbeMerge::update(OMevent_mask event_mask, int seq_num)
{
  // in (Grid_array read req notify)
  // ngrids (OMXint read req)

  // out (Mesh write)
  // out.nspace (int) 
  // out.nnodes (int)
  // out.coordinates.values (float [])

  // out.cell_set[0].ncells (int)
  // out.cell_set[0].cell_nnodes (int)
  // out.cell_set[0].cell_ndim (int)
  // out.cell_set[0].cell_order (int)
  // out.cell_set[0].cell_name (char *)
  // out.cell_set[0].node_connect_list (int *)

  float *in_coords;
  float *out_coords;

  int grid;
  int total;
  int in_index;
  int out_index;
  int coord;
  int identity;
  int stat;
  
  float in_xform[4][4];

  /* count total number of coords	 */

  total = 0;
  for( grid=0; grid<(int)ngrids; grid++ ) {
    total += in[grid].nnodes;
  }

  /* set number of coords in output	 */

  out.nnodes = total;

  /* retrieve pointer to output coords array	 */
  
  out_coords = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  /* cycle through input grids, extract coords, and apply xform	 */
  /* then append to output coords	 */

  out_index = 0;
  for( grid=0; grid<(int)ngrids; grid++ ) {

    in_coords =
      (float *)in[grid].coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
    
    /* retrieve input xform	 */

    stat = FLDget_xform( in[grid].obj_id(), (float *)in_xform );

    if( stat < 0 )  {
      printf( "ERROR: multi_probe cannot find xform\n" );
      fflush(stdout);
      return 1;
    }

    if( !stat ) {
      identity = 1;
    } else {
      identity = MATmat_is_identity( (float *)in_xform, 4 );
    }
    
    in_index = 0;
    for( coord=0; coord<(int)in[grid].nnodes; coord++ ) {

      /* copy point to output	 */

      if( (int)in[grid].nspace == 2 ) {
	out_coords[out_index] = in_coords[in_index++];
	out_coords[out_index+1] = in_coords[in_index++];
	out_coords[out_index+2] = 0.0;
      } else {
	out_coords[out_index] = in_coords[in_index++];
	out_coords[out_index+1] = in_coords[in_index++];
	out_coords[out_index+2] = in_coords[in_index++];
      }

      if( !identity )
	MATvec3_mat4_multiply( out_coords+out_index, in_xform );

      /* step to next point	 */
      
      out_index += 3;
    }

    if( in_coords )
      ARRfree( (char *)in_coords );
  }
  
  /* finished with output	 */

  if( out_coords )
    ARRfree( (char *)out_coords );
  
  /* return sucess	 */
  
  return(1);
}

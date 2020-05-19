
/* ----------------------------------------------------------------------
 * xp_comm_proj/cprobe/cprobe.cxx
 * ----------------------------------------------------------------------
 * ClusterProbe IAC Project for AVS/Express
 *
 * Description:
 * 
 *   The ClusterProbeCreate module inputs two Grid+Xform objects (glyphs)
 *   and replicates the second glyph at each of the coordinates in the
 *   first grid, using that point as the origin of the second.
 *   The xform of the second glyph is applied to each coordinate of the
 *   second glyph, so that it can be transformed (usually scaled)
 *   relative to the base (first) glyph.
 *   For example this allows a box glyph to replicated at each point on
 *   a plane object, producing an array of boxes, or a sphere glyph
 *   could be repeated along a multi-segment line object. Such clustered
 *   glyphs can be used as probes for use with modules such as
 *   streamlines and advect. The module also generates a Point mesh, so
 *   that each point may be rendered.
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
 *   Version 1.0 - 5th March 1999
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/cprobe/gen.h"

#include "avs/mat.h"

int
ClusterProbe_ClusterProbeCreate::update(OMevent_mask event_mask, int seq_num)
{
  // in_base (Grid+Xform read req notify)
  // in_glyph (Grid+Xform read req notify)
  // out (Mesh write)
  
  // out.nspace (int) 
  // out.nnodes (int)
  // out.coordinates.values (float [])
  // out.cell_set[0].ncells (int)
  // out.cell_set[0]. cell_nnodes (int)
  // out.cell_set[0]. cell_ndim (int)
  // out.cell_set[0]. cell_order (int)
  // out.cell_set[0]. cell_name (char *)
  // out.cell_set[0].node_connect_list (int *)

  float in_xform[4][4];

  /* base, glyph and temporary coord arrays	 */
  
  float *g_coords;
  float *b_coords;
  float *t_coords;

  /* output coord arrays 	 */
  
  float *o_coords;

  int stat;
  int identity;
  int coord, gcoord;

  int b_index, g_index, t_index, o_index;

  /* get input glyph coords	 */

  g_coords = (float *) in_glyph.coordinates.values.
    ret_array_ptr( OM_GET_ARRAY_RD );

  if( !g_coords ) {
    return 1;
  };
  
  /* get input glyph xform	 */

  stat = FLDget_xform( in_glyph.obj_id(), (float *)in_xform );

  if( stat < 0 )  {
    printf( "ERROR: cluster_probe cannot find xform\n" );
    fflush(stdout);
    return 1;
  }

  if( !stat ) {
    identity = 1;
  } else {
    identity = MATmat_is_identity( (float *)in_xform, 4 );
  }

  /* if not identity, copy input coords and apply xform	 */

  if( !identity ) {

    t_coords = (float *)malloc( sizeof(float) * 3 * (int)in_glyph.nnodes );

    if( !t_coords ) {
      printf( "ERROR: cluster_probe cannot allocate temporary array\n" );
      fflush(stdout);
      return 1;
    };
  
    g_index = 0;
    t_index = 0;

    for( coord=0; coord<(int)in_glyph.nnodes; coord++ ) {
      t_coords[t_index] = g_coords[g_index++];
      t_coords[t_index+1] = g_coords[g_index++];

      if( (int)in_glyph.nspace == 2 )
	t_coords[t_index+2] = 0.0;
      else
	t_coords[t_index+2] = g_coords[g_index++];
      
      /* apply xform	 */

      MATvec3_mat4_multiply( t_coords+t_index, in_xform );
      t_index += 3;

    }
  }

  /* get input base coords	 */

  b_coords = (float *)in_base.coordinates.values.
    ret_array_ptr( OM_GET_ARRAY_RD );

  if( !b_coords ) {

    /* tidy up and return	 */

    if( !identity ) {
      if( t_coords )
	ARRfree( (char *)t_coords );
    }

    if( g_coords )
      ARRfree( (char *)g_coords );

    return 1;
  };
  
  /* allocate output 	 */

  out.nnodes = (int)in_base.nnodes * (int)in_glyph.nnodes;

  /* get output coords	 */

  o_coords = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

  if( !o_coords ) {

    /* tidy up and return	 */

    if( !identity ) {
      if( t_coords )
	ARRfree( (char *)t_coords );
    }

    if( g_coords )
      ARRfree( (char *)g_coords );

    if( b_coords )
      ARRfree( (char *)b_coords );

    return 1;
  };
  
  /* replicate input coords at each base coord, using it as origin	 */

  o_index = 0;
  b_index = 0;
  
  for( coord=0; coord<(int)in_base.nnodes; coord++ ) {

    g_index = 0;
    
    for( gcoord=0; gcoord<(int)in_glyph.nnodes; gcoord++ ) {

      if( identity ) {

	o_coords[o_index++] = g_coords[g_index++] + b_coords[b_index];
	o_coords[o_index++] = g_coords[g_index++] + b_coords[b_index+1];

	if( (int)in_glyph.nspace == 2 ) {
	  if( (int)in_base.nspace == 2 ) {
	    o_coords[o_index++] = 0.0;
	  } else {
	    o_coords[o_index++] = b_coords[b_index+2];
	  }
	} else {
	  if( (int)in_base.nspace == 2 ) {
	    o_coords[o_index++] = g_coords[g_index++];
	  } else {
	    o_coords[o_index++] = g_coords[g_index++] + b_coords[b_index+2];
	  }
	}
      } else {

	o_coords[o_index++] = t_coords[g_index++] + b_coords[b_index];
	o_coords[o_index++] = t_coords[g_index++] + b_coords[b_index+1];

	if( (int)in_base.nspace == 2 ) {
	  o_coords[o_index++] = t_coords[g_index++];
	} else {
	  o_coords[o_index++] = t_coords[g_index++] + b_coords[b_index+2];
	}

      }
    }

    b_index += (int)in_base.nspace;

  }

  /* free temporary array if required	 */
  
  if( !identity )
    free( t_coords );

  /* free input and output coords	 */

  if( g_coords )
    ARRfree( (char *)g_coords );

  if( b_coords )
    ARRfree( (char *)b_coords );
  
  if( o_coords )
    ARRfree( (char *)o_coords );

  /* success	 */

  return(1);
}

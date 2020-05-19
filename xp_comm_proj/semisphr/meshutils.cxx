/****************************************************************************/

#include "meshutils.h"
#include "XP_OM_CALL.h"
#include "avs/fld.h"
#include <math.h>

/****************************************************************************/

int 
MU_init_field( OMobj_id  field_id )
{
  /* Reset the number of nodes */
  FLDset_nnodes( field_id, 0 );
  
  /* Reset the number of polys */
  OMobj_id polytri_id;
  FLDget_cell_set( field_id, 0, &polytri_id );
  FLDset_npolys( polytri_id, 0 );
  
  OMobj_id polyline_id;
  FLDget_cell_set( field_id, 1, &polyline_id );
  FLDset_npolys( polyline_id, 0 );
  
  /* Set the normals node data */
  float normals[3] = {0,0,0};
  FLDset_node_data( field_id, 0, (char *)&normals[0], 
		    DTYPE_FLOAT, 0, OM_SET_ARRAY_COPY ); 

  /* Successful return */
  return( XP_SUCCESS );
}

/****************************************************************************/

void
MU_free_poly_data( poly_data_t  *data )
{
  if( data->r_outside != NULL )
  {
    ARRfree( data->r_outside );
    data->r_outside = NULL;
  }
  if( data->z_outside != NULL )
  {
    ARRfree( data->z_outside );
    data->z_outside = NULL;
  }
  if( data->r_inside != NULL )
  {
    ARRfree( data->r_inside );
    data->r_inside = NULL;
  }
  if( data->z_inside != NULL )
  {
    ARRfree( data->z_inside );
    data->z_inside = NULL;
  }
}

/****************************************************************************/

int 
MU_gen_body_of_rev( OMobj_id      field_id,
		    poly_data_t  *data )
{
  int   status;
  int   i;
  float r_in;
  float z_in;

  /* Allocate the coordinate array */
  float *coords = (float *)malloc( (data->num_thetas + 1) * 2 * 
				   3 * sizeof(float) );
  if( coords == NULL ) 
  {
    printf( "MU_gen_body_of_rev: Could not allocate coord array\n");
    return( XP_FAILURE );
  }

  /* Create the outside surface */
  for( i = 0; i < data->num_points_outside - 1; i++ )
  { 
    MU_gen_cyl_coords( data->num_thetas, 
		       data->r_outside[i],   data->z_outside[i],
		       data->r_outside[i+1], data->z_outside[i+1], 
		       coords );

    FUNCCALLFR( "MU_gen_body_of_rev: Could not generate outside surface field",
		MU_gen_field( field_id, coords, data->num_thetas + 1, 2, 1 ),
		free( coords ) );
  }

  /* Create the inside surface */
  if( data->body_type == kHOLLOW )
  {
    for( i = 0; i < data->num_points_inside - 1; i++ )
    { 
      MU_gen_cyl_coords( data->num_thetas, 
			 data->r_inside[i],   data->z_inside[i],
			 data->r_inside[i+1], data->z_inside[i+1], 
			 coords );
      
      FUNCCALLFR( "MU_gen_body_of_rev: Could not generate inside surf field",
		  MU_gen_field( field_id, coords, data->num_thetas + 1, 2, 0 ),
		  free( coords ) );
    }
  }

  /* Create the first end plate */
  if( data->gen_first_end_cap == 1 )
  {
    if( data->body_type == kSOLID )
    {
      r_in = ZERO_TOL;
      z_in = data->z_outside[0];
    }
    else
    {
      r_in = data->r_inside[0];
      z_in = data->z_inside[0];
    }
    
    MU_gen_cyl_coords( data->num_thetas, 
		       data->r_outside[0], data->z_outside[0],
		       r_in, z_in, 
		       coords );
    
    FUNCCALLFR( "MU_gen_body_of_rev: Could not generate first end cap field",
		MU_gen_field( field_id, coords, data->num_thetas + 1, 2, 0 ),
		free( coords ) );
  }
  
  /* Create the second end plate */
  if( data->gen_last_end_cap == 1 )
  {
    if( data->body_type == kSOLID )
    {
      r_in = ZERO_TOL;
      z_in = data->z_outside[data->num_points_outside-1];
    }
    else
    {
      r_in = data->r_inside[data->num_points_inside-1];
      z_in = data->z_inside[data->num_points_inside-1];
    }
    
    MU_gen_cyl_coords( data->num_thetas, 
		       data->r_outside[data->num_points_outside-1], 
		       data->z_outside[data->num_points_outside-1], 
		       r_in, z_in, coords );
    
    FUNCCALLFR( "MU_gen_body_of_rev: Could not generate last end cap field",
		MU_gen_field( field_id, coords, data->num_thetas + 1, 2, 1 ),
		free( coords ) );
  }

  /* Clean up */
  free( coords );

  return( XP_SUCCESS );
}

/****************************************************************************/

int 
MU_gen_semi_sphere( OMobj_id      field_id,
		    semi_data_t  *data )
{
  int   status;
  int   i;
  float half_angle_inside;

  /* Allocate the coordinate array */
  float *coords = (float *)malloc( (data->num_thetas + 1) * 
				   (data->num_phis + 1) *
				   3 * sizeof(float) );
  if( coords == NULL ) 
  {
    printf( "MU_gen_semi_sphere: Could not allocate coord array\n");
    return( XP_FAILURE );
  }
 
  /* Check for half_angle that generates a sphere */
  if( data->half_angle > PI - ZERO_TOL )
  {
    data->half_angle = PI - ZERO_TOL;
  }

  /* Create the outside surface */
  float  offset = -(data->r_outside * cos(data->half_angle - ZERO_TOL)) + 
                    data->z_offset;
  float  dphi   = (data->half_angle - ZERO_TOL) / data->num_phis;
  float  phi    = ZERO_TOL;
  float *cur_coords = coords;

  for( i = 0; i < data->num_phis + 1; i++ )
  { 
    MU_gen_circle_coords( data->num_thetas, 
			  data->r_outside * sin(phi), 
			  data->r_outside * cos(phi) + offset,
			  cur_coords );
    phi += dphi;
    cur_coords += (data->num_thetas + 1) * 3;
  }
  FUNCCALLFR( "MU_gen_semi_sphere: Could not generate outside surface field",
	      MU_gen_field( field_id, coords, data->num_thetas + 1, 
			    data->num_phis + 1, 0 ), free( coords ) );
  
  /* Calculate half angle for the inside surface so that the end is
     parallel to the xy plane not the radius */
  if( data->body_type == kHOLLOW )
  {
    if( abs( data->r_outside * cos(data->half_angle) ) < data->r_inside )
    {
      half_angle_inside = acos( ( data->r_outside * cos(data->half_angle) ) / 
				data->r_inside); 
      
    }
    else
    {
      if( cos(data->half_angle) < 0 )
      {
	half_angle_inside = PI - ZERO_TOL;
      }
      else
      {
	data->body_type = kSOLID;
      }    
    }
  };

  /* Create the inside surface */
  if( data->body_type == kHOLLOW )
  {
    dphi = (half_angle_inside - ZERO_TOL) / data->num_phis;
    phi = ZERO_TOL;
    cur_coords = coords;

    for( i = 0; i < data->num_phis + 1; i++ )
    { 
      MU_gen_circle_coords( data->num_thetas, 
			    data->r_inside * sin(phi), 
			    data->r_inside * cos(phi) + offset,
			    cur_coords );
      phi += dphi;
      cur_coords += (data->num_thetas + 1) * 3;
    }
    FUNCCALLFR( "MU_gen_semi_sphere: Could not generate outside surface field",
		MU_gen_field( field_id, coords, data->num_thetas + 1, 
			      data->num_phis + 1, 1 ), free( coords ) );
  }

  /* Create the end plate */
  if( data->gen_end_cap == 1 )
  {
    if( data->body_type == kSOLID )
    {
      if( data->half_angle < PI - ZERO_TOL ) 
      {
	MU_gen_cyl_coords( data->num_thetas, 
			   data->r_outside * sin(data->half_angle), 
			   data->r_outside * cos(data->half_angle) + offset,
			   ZERO_TOL, 
			   data->r_outside * cos(data->half_angle) + offset, 
			   coords );
	FUNCCALLFR( "MU_gen_body_of_rev: Could not generate sphere end plate",
		    MU_gen_field( field_id, coords, 
				  data->num_thetas + 1, 2, 0 ),
		    free( coords ) );
      }
    }
    else if( data->body_type == kHOLLOW )
    {
      if( half_angle_inside < PI - 2*ZERO_TOL )
      {
	MU_gen_cyl_coords( data->num_thetas, 
			   data->r_outside * sin(data->half_angle), 
			   data->r_outside * cos(data->half_angle) + offset,
			   data->r_inside  * sin(half_angle_inside), 
			   data->r_inside  * cos(half_angle_inside) + offset, 
			   coords );
	FUNCCALLFR( "MU_gen_body_of_rev: Could not generate sphere end plate",
		    MU_gen_field( field_id, coords, 
				  data->num_thetas + 1, 2, 0 ),
		    free( coords ) );
      }
      else if( data->half_angle < PI - 2*ZERO_TOL )
      {
	MU_gen_cyl_coords( data->num_thetas, 
			   data->r_outside * sin(data->half_angle), 
			   data->r_outside * cos(data->half_angle) + offset,
			   ZERO_TOL, 
			   data->r_outside * cos(data->half_angle) + offset, 
			   coords );
	FUNCCALLFR( "MU_gen_body_of_rev: Could not generate sphere end plate",
		    MU_gen_field( field_id, coords, 
				  data->num_thetas + 1, 2, 0 ),
		    free( coords ) );
      }
    }    
  }
  
  /* Clean up */
  free( coords );

  return( XP_SUCCESS );
}

/****************************************************************************/

void 
MU_gen_cyl_coords( int     num_thetas,
		   float   r_value1,
		   float   z_value1,
		   float   r_value2,
		   float   z_value2,
		   float  *coords )
{
  int   ia;
  int   ib;
  float dtheta = 2 * PI / num_thetas;
  float theta;

  ia = 0;
  ib = (num_thetas + 1) * 3;
  theta = 0.0;
  
  for( int i = 0; i < num_thetas + 1; i++ )
  {
    coords[ia]   = r_value1 * sin( theta );
    coords[ia+1] = r_value1 * cos( theta );
    coords[ia+2] = z_value1; 
    ia += 3;
    coords[ib]   = r_value2 * sin( theta );
    coords[ib+1] = r_value2 * cos( theta );
    coords[ib+2] = z_value2; 
    ib += 3;
    
    theta += dtheta; 
  }
}

/****************************************************************************/

void 
MU_gen_circle_coords( int     num_thetas,
		      float   r_value,
		      float   z_value,
		      float  *coords )
{
  int   ia;
  float dtheta = 2 * PI / num_thetas;
  float theta;

  ia = 0;
  theta = 0.0;
  
  for( int i = 0; i < num_thetas + 1; i++ )
  {
    coords[ia]   = r_value * sin( theta );
    coords[ia+1] = r_value * cos( theta );
    coords[ia+2] = z_value; 
    ia += 3;
    
    theta += dtheta; 
  }
}

/****************************************************************************/

int
MU_mass_props_body_of_rev( poly_data_t   *data, 
			   mass_props_t  *mass_props ) 
{
  int    i;
  int    status;

  float  r0;
  float  r1;
  float  z0;
  float  z1;

  float  massi;
  float  zcgi;
  float  izzi;

  float  total_mass = 0.0;
  float  total_zcg  = 0.0;
  float  total_izz  = 0.0;
  
  /* Calculate the values for outside surface */
  for( i = 0; i < data->num_points_outside - 1; i++ )
  { 
    r0 = data->r_outside[i];
    r1 = data->r_outside[i+1];
    z0 = data->z_outside[i]; 
    z1 = data->z_outside[i+1]; 

    FUNCCALLR( "Calculate the mass properties for the frustum of a right cone",
		MU_mass_props_frustum( r0, r1, z0, z1, mass_props->density, 
				       &massi, &zcgi, &izzi ) );

    total_zcg   = (total_zcg*total_mass + zcgi*massi) / (total_mass + massi);
    total_izz  += izzi;
    total_mass += massi;
  }

  /* Calculate the values for inside surface */
  if( data->body_type == kHOLLOW )
  {
    for( i = 0; i < data->num_points_inside - 1; i++ )
    { 
      r0 = data->r_inside[i];
      r1 = data->r_inside[i+1];
      z0 = data->z_inside[i]; 
      z1 = data->z_inside[i+1]; 
      
      FUNCCALLR( "Calculate the mass properties for the frustum of a cone",
		 MU_mass_props_frustum( r0, r1, z0, z1, mass_props->density,
					&massi, &zcgi, &izzi ) );
      
      total_zcg   = (total_zcg*total_mass - zcgi*massi) / (total_mass - massi);
      total_izz  -= izzi;
      total_mass -= massi;
    }
  }

  mass_props->mass = total_mass;
  mass_props->zcg  = total_zcg;
  mass_props->izz  = total_izz;

  return( XP_SUCCESS );
}

/****************************************************************************/

int
MU_mass_props_semi_sphere( semi_data_t   *data, 
			   mass_props_t  *mass_props ) 
{
  int    i;
  int    status;
  
  float  massi;
  float  zcgi;
  float  izzi;

  float  total_mass = 0.0;
  float  total_zcg  = 0.0;
  float  total_izz  = 0.0;
  
  int    calc_inside = 0;
  float  half_angle_inside;

  /* Calculate the values for outside surface */
  FUNCCALLR( "Calculate the mass properties for the outside of semi-sphere",
	     MU_mass_props_sphere( data->r_outside, 
				   data->r_outside * cos(data->half_angle),
				   data->z_offset - 
				   data->r_outside * cos(data->half_angle), 
				   mass_props->density, 
				   &massi, &zcgi, &izzi ) );

  total_mass = massi;
  total_zcg  = zcgi;
  total_izz  = izzi;

  /* Calculate the values for inside surface */
  if( data->body_type == kHOLLOW ) 
  {
    if( abs( data->r_outside * cos(data->half_angle) ) < data->r_inside )
    {
      half_angle_inside = acos( ( data->r_outside * cos(data->half_angle) ) / 
				data->r_inside); 
      calc_inside = 1;
    }
    else
    {
      if( cos(data->half_angle) < 0 )
      {
	half_angle_inside = PI;
	calc_inside = 1;
      }
      else 
      {	  
	calc_inside = 0;
      }
    }
      
    if( calc_inside == 1 )
    {
      FUNCCALLR( "Calculate the mass properties for the inside of semi-sphere",
		 MU_mass_props_sphere( data->r_inside, 
				       data->r_inside * cos(half_angle_inside),
				       data->z_offset +
				       (data->r_outside - data->r_inside) - 
				       data->r_inside * cos(half_angle_inside),
				       mass_props->density, 
				       &massi, &zcgi, &izzi ) );
      
      total_zcg   = (total_zcg*total_mass - zcgi*massi) / (total_mass - massi);
      total_izz  -= izzi;
      total_mass -= massi;
    }
  }

  mass_props->mass = total_mass;
  mass_props->zcg  = total_zcg;
  mass_props->izz  = total_izz;

  return( XP_SUCCESS );
}

/****************************************************************************/

int
MU_mass_props_frustum( float   r0, 
		       float   r1, 
		       float   z0,
		       float   z1,
		       float   rho,
		       float  *mass, 
		       float  *zcg, 
		       float  *izz ) 
{
  float h = z1 - z0; 
  float r0sqrd = r0 * r0;
  float r1sqrd = r1 * r1;
  float r0r1   = r0 * r1;
  float factor = (r1sqrd + r0r1 + r0sqrd);

  *mass = rho * PI * h * factor / 3. / 1000.;  /* 1000 => gm to kg */
  *zcg = z0 + (h * ((3. * r1sqrd + 2. * r0r1 + r0sqrd) / factor) / 4.);
  *izz = 3. * (*mass) * (r1sqrd + r0sqrd - ((r0r1*r0r1) / factor)) / 10.;

  return( XP_SUCCESS );
}

/****************************************************************************/

int
MU_mass_props_sphere( float   r0, 
		      float   x0,
		      float   offset,
		      float   rho,
		      float  *mass, 
		      float  *zcg, 
		      float  *izz ) 
{
   float  r0sqrd  = r0 * r0;
   float  r0cubed = r0sqrd * r0;   
   float  x0sqrd  = x0 * x0;
   float  x0cubed = x0sqrd * x0;   
   float  factor = (2.*r0cubed - 3.*r0sqrd*x0 + x0cubed);

  *mass = rho * PI * factor / 3. / 1000.;  
  *zcg =  offset + 3. * (r0sqrd - x0sqrd) * (r0sqrd - x0sqrd) / factor / 4.;
  *izz =  rho * PI * ( 4.*r0sqrd*r0cubed/15. - r0sqrd*r0sqrd*x0/2. +
		       r0sqrd*x0cubed/3. - x0sqrd*x0cubed/10.) / 1000.;

  return( XP_SUCCESS );
}

/****************************************************************************/

int 
MU_gen_field( OMobj_id  field_id,
	      float    *vertex, 
	      int       ncols, 
	      int       nrows, 
	      int       flip )
{
  int i,j;
  
  /* Get the previous number of nodes */
  int prev_nnodes;
  FLDget_nnodes( field_id, &prev_nnodes );
  
  /* Add the new number of nodes to the previous number */
  int num_nnodes = prev_nnodes + 
                   2*ncols + 2*(nrows-1) + 4*ncols*(nrows-1) - 1; 
  FLDset_nnodes( field_id, num_nnodes );
  
  /* Get the coordinate array */
  int size;
  float *coords;
  FLDget_coord( field_id, &coords, &size, OM_GET_ARRAY_RW );
  if( coords == NULL ) 
  {
    printf( "Coords pointer is NULL\n");
    return( XP_FAILURE );
  }
  
  /* Get the normals data array */
  int type;
  float *normals;
  FLDget_node_data( field_id, 0, &type, (char **)&normals, &size, 
		    OM_GET_ARRAY_RW );
  if( normals == NULL ) 
  {
    printf( "Normals pointer is NULL\n");
    ARRfree( coords );
    return( XP_FAILURE );
  }
   
  /* Allocate an array for the mesh normals */
  float *mesh_normals = (float *)malloc( ncols*nrows*3 * sizeof(float) );
  if( mesh_normals == NULL ) 
  {
    printf( "Normals pointer or mesh is NULL\n");
    ARRfree( coords );
    ARRfree( normals );
    return( XP_FAILURE );
  }

  /* Generate normals for the mesh coordinates */
  if( MU_gen_mesh_normals( vertex, mesh_normals, ncols, nrows, flip ) == 0 )
  {
    printf( "Error generating normals for mesh\n" );
    ARRfree( coords );
    ARRfree( normals );
    free( mesh_normals );
    return( XP_FAILURE );
  };

  /*
     Set the coordinate values for the polytri points 
  */
  int iv1 = 0;
  int ic = prev_nnodes * 3;
  
  /* For each "strip" */
  for( j = 0; j < nrows - 1; j++)
  {
    int iv2 = (j+1)*ncols*3;
    
    /* Set first point on each successive strip */
    if( j > 0 )
    {
      coords[ic]   = vertex[iv1];
      coords[ic+1] = vertex[iv1+1];
      coords[ic+2] = vertex[iv1+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv1], sizeof(float)*3 );
      ic+=3;
    }
    
    /* For each set of points along a "strip" */
    for( i = 0; i < ncols; i++)
    {        
      coords[ic]   = vertex[iv1];
      coords[ic+1] = vertex[iv1+1];
      coords[ic+2] = vertex[iv1+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv1], sizeof(float)*3 );
      ic+=3;      

      coords[ic]   = vertex[iv2];
      coords[ic+1] = vertex[iv2+1];
      coords[ic+2] = vertex[iv2+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv2], sizeof(float)*3 );
      ic+=3;
      
      iv1+=3; 
      iv2+=3; 
    }
    
    /* Repeat last point on strip */
    coords[ic]   = vertex[iv2-3];
    coords[ic+1] = vertex[iv2-2];
    coords[ic+2] = vertex[iv2-1];
    memcpy( (void *)&normals[ic],
	    (void *)&mesh_normals[iv2-3], sizeof(float)*3 );
    ic+=3;
  }      
  
  /*
     Set the coordinate values for the "horizontal" polyline points
  */

  /* For each row */
  iv1 = 0;
  for( j = 0; j < nrows; j++)
  {
    /* For each set of points along a row */
    for( i = 0; i < ncols; i++)
    {        
      coords[ic]   = vertex[iv1];
      coords[ic+1] = vertex[iv1+1];
      coords[ic+2] = vertex[iv1+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv1], sizeof(float)*3 );
      ic+=3;      
      iv1+=3; 
    }
  }      
  
  /*
     Set the coordinate values for the "vertical" polyline points
  */

  /* For each column */
  for( j = 0; j < ncols; j++)
  {
    /* For each set of points along a column */
    for( i = 0; i < nrows; i++)
    {        
      iv1 = j*3 + i*ncols*3;
      
      coords[ic]   = vertex[iv1];
      coords[ic+1] = vertex[iv1+1];
      coords[ic+2] = vertex[iv1+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv1], sizeof(float)*3 );
      ic+=3;      
    }
  }      
  ARRfree( coords );
  ARRfree( normals );
  free( mesh_normals );
  
  /*
     Set up the cell set connectivity information
  */
 
  /* Get the polytri cell_set */
  OMobj_id polytri_id;
  FLDget_cell_set( field_id, 0, &polytri_id );
  
  /* Increment the number of polytri polys */
  int polytri_npolys;
  FLDget_npolys( polytri_id, &polytri_npolys );
  FLDset_npolys( polytri_id, polytri_npolys + 1 );
   
  /* Set the poly connect list for the polytri cell_set */
  int *polytri_poly_conn_array;
  FLDget_poly_connect( polytri_id, &polytri_poly_conn_array, &size, 
		       OM_GET_ARRAY_RW );
  int polytri_end_index = prev_nnodes + (nrows-1)*(ncols*2+2) - 3;
  polytri_poly_conn_array[polytri_npolys*2]   = prev_nnodes;
  polytri_poly_conn_array[polytri_npolys*2+1] = polytri_end_index;
  ARRfree( polytri_poly_conn_array );
  
  /* Set the polyline cell_set */
  OMobj_id polyline_id;
  FLDget_cell_set( field_id, 1, &polyline_id );
  
  /* Increment the number of polyline polys */
  int polyline_npolys;
  FLDget_npolys( polyline_id, &polyline_npolys );
  FLDset_npolys( polyline_id, (polyline_npolys + ncols + nrows) );
  
  /* Set the poly connect list for the polyline cell_set */
  int *polyline_poly_conn_array;
  FLDget_poly_connect( polyline_id, &polyline_poly_conn_array, &size, 
		       OM_GET_ARRAY_RW );
  
  ic = polyline_npolys * 2;
  int polyline_start_index = polytri_end_index + 2;
  for( i = 0; i < nrows; i++ )
  {
    polyline_poly_conn_array[ic]   = polyline_start_index + i*ncols;
    polyline_poly_conn_array[ic+1] = polyline_start_index + (i+1)*ncols - 1;
    ic += 2;
  }
  polyline_start_index += nrows*ncols; 
  for( i = 0; i < ncols; i++ )
  {
    polyline_poly_conn_array[ic]   = polyline_start_index + i*nrows;
    polyline_poly_conn_array[ic+1] = polyline_start_index + (i+1)*nrows - 1;
    ic += 2;
  }
  ARRfree( polyline_poly_conn_array );
  
  /* Successful return */
  return( XP_SUCCESS );
}

/****************************************************************************/

int
MU_gen_mesh_normals( float *verts,
		     float *normals,
		     int    ncols, 
		     int    nrows,
		     int    flip )
{
  /* Declare some local variables */
  register int i,j;
  register int k = 0;
  float tmp1[3], tmp2[3];

  /* Do the first point on the first row  */
  /* Modified to provide continous surface for body of revolution */
  /* MU_sub_vec( tmp1, &verts[0], &verts[3] ); */
  MU_sub_vec( tmp1, &verts[(ncols-2)*3], &verts[3] );
  MU_sub_vec( tmp2, &verts[0], &verts[ncols*3] );
  MU_cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  /* Do the "middle" points in the first row  */
  for( i = 1; i < ncols-1; i++ )
  { 
    MU_sub_vec( tmp1, &verts[(i-1)*3], &verts[(i+1)*3] );
    MU_sub_vec( tmp2, &verts[i*3], &verts[(ncols+i)*3] );
    MU_cross_product( &normals[k], tmp1, tmp2 );
    k += 3; 
  }

  /* Do the last point on the first row  */
  MU_sub_vec( tmp1, &verts[(ncols-1)*3], &verts[(2*ncols-1)*3] );
  /* Modified to provide continous surface for body of revolution */
  /* MU_sub_vec( tmp2, &verts[(ncols-1)*3], &verts[(ncols-2)*3] ); */
  MU_sub_vec( tmp2, &verts[3], &verts[(ncols-2)*3] );
  MU_cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  /* Loop though the "middle" rows */
  for( j = 1; j < nrows-1; j++ ) 
  { 
    /* Do the first point on each row */
    MU_sub_vec( tmp1, &verts[((j+1)*ncols)*3], &verts[((j-1)*ncols)*3] );
    /* Modified to provide continous surface for body of revolution */
    /* MU_sub_vec( tmp2, &verts[(j*ncols)*3], &verts[(j*ncols+1)*3] ); */
    MU_sub_vec( tmp2, &verts[(j*ncols+ncols-2)*3], &verts[(j*ncols+1)*3] );
    MU_cross_product( &normals[k], tmp1, tmp2 );
    k += 3;

    /* Do the "middle" points in each row */
    for( i = 1; i < ncols-1; i++ ) 
    { 
      MU_sub_vec( tmp1, &verts[(j*ncols+(i-1))*3], &verts[(j*ncols+(i+1))*3] );
      MU_sub_vec( tmp2, &verts[((j-1)*ncols+i)*3], &verts[((j+1)*ncols+i)*3] );
      MU_cross_product( &normals[k], tmp1, tmp2 );
      k += 3;
    }

    /* Do the last point on each row */ 
    /* Modified to provide continous surface for body of revolution */
    /* MU_sub_vec( tmp1, 
	  	   &verts[(j*ncols+ncols-2)*3], 
		   &verts[(j*ncols+ncols-1)*3] ); */
    MU_sub_vec( tmp1, 
		&verts[(j*ncols+ncols-2)*3], 
		&verts[(j*ncols+1)*3] );
    MU_sub_vec( tmp2, 
		&verts[((j-1)*ncols+(ncols-1))*3], 
		&verts[((j+1)*ncols+(ncols-1))*3] );
    MU_cross_product( &normals[k], tmp1, tmp2 );
    k += 3;
  }

  /* Do the first point on the last row */ 
  /* Modified to provide continous surface for body of revolution */
  /* MU_sub_vec( tmp1, 
                 &verts[(nrows-1)*ncols*3], 
		 &verts[((nrows-1)*ncols+1)*3] ); */
  MU_sub_vec( tmp1, 
	      &verts[((nrows-1)*ncols+(ncols-2))*3], 
	      &verts[((nrows-1)*ncols+1)*3] );
  MU_sub_vec( tmp2, 
	      &verts[(nrows-2)*ncols*3], 
	      &verts[(nrows-1)*ncols*3] );
  MU_cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  /* Do the "middle" points on the last row  */
  for (i = 1; i < ncols-1; i++) {
    MU_sub_vec( tmp1, 
		&verts[((nrows-1)*ncols+(i-1))*3], 
		&verts[((nrows-1)*ncols+(i+1))*3] );
    MU_sub_vec( tmp2, 
		&verts[((nrows-2)*ncols+i)*3], 
		&verts[((nrows-1)*ncols+i)*3] );
    MU_cross_product( &normals[k],tmp1, tmp2 );
    k += 3;
  }
  
  /* Do the last point on the last row  */
  /* Modified to provide continous surface for body of revolution */
  /* MU_sub_vec( tmp1,  
    	         &verts[((nrows-1)*ncols+(ncols-2))*3], 
  	         &verts[((nrows-1)*ncols+(ncols-1))*3] ); */
  MU_sub_vec( tmp1, 
	      &verts[((nrows-1)*ncols+(ncols-2))*3],
	      &verts[((nrows-1)*ncols+1)*3] );
  MU_sub_vec( tmp2,
	      &verts[((nrows-2)*ncols+(ncols-1))*3],
	      &verts[((nrows-1)*ncols+(ncols-1))*3] );
  MU_cross_product( &normals[k], tmp1, tmp2 );

  /* Normalize and flip normals */
  MU_normalize_verts( normals, nrows*ncols );
  if( flip == 1 )
  {
    MU_flip_verts( normals, nrows*ncols );
  }

  /* Successful return */
  return(1);
}

/****************************************************************************/

void
MU_sub_vec( float res[3], float a[3], float b[3] )
{
   res[0] = a[0] - b[0];
   res[1] = a[1] - b[1];
   res[2] = a[2] - b[2];
}

/****************************************************************************/

void
MU_cross_product( float *res, float *vec1, float *vec2 ) 
{
   res[0] = vec1[1]*vec2[2] - vec2[1]*vec1[2];
   res[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
   res[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

/****************************************************************************/

void
MU_normalize_verts( float *v, int n )
{
   register float len;
   register int i;

   for( i = 0; i < n; i++, v += 3) 
   {
     float sum;
     sum = *v * *v + *(v+1) * *(v+1) + *(v+2) * *(v+2);
     if (sum != 0.0) 
     {
       len = sqrt(sum);
       *v /= len; *(v+1) /= len; *(v+2) /= len;
     }
   }
}

/****************************************************************************/

void
MU_flip_verts( float *v, int n )
{
   register int i;

   for(i = 0; i < n; i++, v += 3 ) 
   {
     *v *= -1.0; *(v+1) *= -1.0; *(v+2) *= -1.0;
   }
}

/****************************************************************************/


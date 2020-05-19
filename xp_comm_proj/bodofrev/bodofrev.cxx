/*
 */

/* ----------------------------------------------------------------------
 * modBodyOfRevolution Module
 * ----------------------------------------------------------------------
 * Description:
 *   
 *   BodyOfRevolution rotates "inside" and "outside" polylines around the
 *   "z" axis to form a "solid" body of revolution.  The routine creates
 *   surfaces and the appropriate normals for the inside, outside, and
 *   end caps (if required).  The outside polyline line is required.  The
 *   routine also calculates the mass properties for the body. The body
 *   is generated for 360 degrees of revolution only.
 *
 * Authors: 
 *   Brian Selle, Leon Thrane, Advanced Visual Systems Inc.
 *   Documentation written by Ian Curington
 *
 * Revision: 10th February 2000 - Paul G. Lever, IAC
 *   Converted to IAC format.
 *
 * ----------------------------------------------------------------------
 * Note:
 *   The gen.h include file is generated by Express when the module is 
 *   compiled. It avoids including "user.h" or "express.h" directly, so 
 *   that the module may be moved across the processes by changing the V
 *   properties in the library.
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/bodofrev/gen.h"
#include "meshUtils.h"
#include "XP_OM_CALL.h"
#include <string.h>
#include <math.h>

// #define DEBUG

int
BodyOfRevolution_BodyOfRevolutionCore::update(OMevent_mask , int seq_num)
{
  int  status;         // Function return status 

  // Check for valid active status
  if( (int)active == 0 )
  {
    return( XP_SUCCESS );
  }

  if ((int)num_thetas <1)
  {
      printf( "modBodyOfRevolution: num_thetas must be greater 0\n" );
      return( XP_FAILURE );
  }

  // Store the array sizes
  int num_points_outside = r_outside.ret_array_size();
  int num_points_inside  = r_inside.ret_array_size();

  // Check for some data 
  if( num_points_outside < 2 && num_thetas > 2 )
  {
    return( XP_SUCCESS );
  }

  // Initialize the mass property structure
  mass_props_t mass_props;
  memset( &mass_props, 0, sizeof(mass_props_t));
  mass_props_t mass_props_semi;
  memset( &mass_props_semi, 0, sizeof(mass_props_t));

  // Define and setup the poly data structure   
  poly_data_t  data;
  data.body_type          = kSOLID;
  data.num_thetas         = (int)num_thetas;
  data.num_points_outside = num_points_outside;
  data.num_points_inside  = num_points_inside;
  data.r_outside          = NULL;
  data.z_outside          = NULL;
  data.r_inside           = NULL;
  data.z_inside           = NULL;
  data.gen_first_end_cap  = 1;
  if( (int)nose_cap_on > 0 )
  {
    data.gen_last_end_cap = 0;
  }
  else
  {
    data.gen_last_end_cap = 1;
  }

  // Check for valid input array sizes and set body type
  if( num_points_outside != z_outside.ret_array_size() )
  {
    printf( "modBodyOfRevolution: Inconsistent outside array sizes\n" );
    return( XP_FAILURE );
  } 

  if( num_points_inside > 1 )
  {
    if( num_points_inside != z_inside.ret_array_size() )
    {
      printf( "modBodyOfRevolution: Inconsistent inside array sizes\n" );
      return( XP_FAILURE );
    } 
    data.body_type = kHOLLOW;
  }    

  // Initialize the field
  OMobj_id  field_id = (OMobj_id)out.obj_id();
  FUNCCALLR( "Error initializing field", MU_init_field( field_id ) ); 

  // Get the defining outside polyline arrays from Express   
  OMCALLFR_NONNULLPTR( "Get r_outside array pointer",     
		       data.r_outside, (float *)r_outside.
		       ret_array_ptr( OM_GET_ARRAY_RD ),
		       MU_free_poly_data( &data ) ); 
  
  OMCALLFR_NONNULLPTR( "Get z_outside array pointer",     
		       data.z_outside, (float *)z_outside.
		       ret_array_ptr( OM_GET_ARRAY_RD ),  
		       MU_free_poly_data( &data ) ); 

  // Insure that the z values are always increasing 
  for( int i = 0; i < num_points_outside - 1; i++ )
  {
    if( data.z_outside[i] > data.z_outside[i+1] )
    {
      printf( "modBodyOfRevolution: z_outside values must by increasing\n" );
      return( XP_FAILURE );
    }
  }
  
  // Get the defining inside polyline arrays from Express   
  if( data.body_type == kHOLLOW )
  {
    OMCALLFR_NONNULLPTR( "Get r_inside array pointer",     
			 data.r_inside, (float *)r_inside.
			 ret_array_ptr( OM_GET_ARRAY_RD ),
			 MU_free_poly_data( &data ) ); 
    
    OMCALLFR_NONNULLPTR( "Get z_inside array pointer",     
			 data.z_inside, (float *)z_inside.
			 ret_array_ptr( OM_GET_ARRAY_RD ),
			 MU_free_poly_data( &data ) ); 

    // Insure that the z values are always increasing 
    for( int i = 0; i < num_points_inside - 1; i++ )
    {
      if( data.z_inside[i] > data.z_inside[i+1] )
      {
	printf( "modBodyOfRevolution: z_inside values must by increasing\n" );
	return( XP_FAILURE );
      }
    }
  }
  
  // Create the surfaces
  FUNCCALLFR( "Create body of revolution",
	      MU_gen_body_of_rev( field_id, &data ), 
	      MU_free_poly_data( &data ) ); 
  
  // Set up the mass properties structure
  mass_props.density = (float)density;
  mass_props_semi.density = mass_props.density;

  // Calculate the mass properties
  FUNCCALLFR( "Calculate the mass properties",
	      MU_mass_props_body_of_rev( &data, &mass_props ), 
	      MU_free_poly_data( &data ) ); 

  // Check if we need to add a nose cap
  if( nose_cap_on > 0 ) 
  {
    // Define and setup the semi data structure   
    semi_data_t  semi_data;
    semi_data.body_type = data.body_type;
    semi_data.gen_end_cap = 0;
    
    float theta = atan( (data.r_outside[data.num_points_outside-2] - 
			 data.r_outside[data.num_points_outside-1]) /
			(data.z_outside[data.num_points_outside-1] - 
			 data.z_outside[data.num_points_outside-2]) );
    semi_data.r_outside = (data.r_outside[data.num_points_outside-1] / 
			   cos(theta) );
    semi_data.half_angle = (PI / 2.0) - theta;
    semi_data.z_offset   = data.z_outside[data.num_points_outside-1];

    if( data.body_type == kHOLLOW )
    {
      float delta = semi_data.r_outside * sin(theta); 
      semi_data.r_inside = sqrt( delta*delta + 
				 data.r_inside[data.num_points_outside-1]*
				 data.r_inside[data.num_points_outside-1] );
    }
    semi_data.num_thetas = data.num_thetas;
    semi_data.num_phis   = (semi_data.num_thetas * semi_data.half_angle / 
			    (2 * PI));

	semi_data.num_phis=semi_data.num_phis?semi_data.num_phis:1;	
#ifdef DEBUG
fprintf (stderr,"semi_data.num_thetas: %i\n",semi_data.num_thetas);
fprintf (stderr,"semi_data.num_phis: %i\n",semi_data.num_phis);
fflush(stderr);
#endif
        
    // Create the surfaces
    FUNCCALLFR( "Create body of revolution",
		MU_gen_semi_sphere( field_id, &semi_data ),
		MU_free_poly_data( &data ) ); 
		    
    // Calculate the mass properties
    FUNCCALLFR( "Calculate the mass properties",
		MU_mass_props_semi_sphere( &semi_data, &mass_props_semi ), 
		MU_free_poly_data( &data ) ); 
  }

  // Set the mass properties in Express  
  mass = mass_props.mass + mass_props_semi.mass;
  zcg  = ( ( mass_props.zcg      * mass_props.mass + 
	     mass_props_semi.zcg * mass_props_semi.mass ) / 
	   ( mass_props.mass + mass_props_semi.mass) );
  izz  = mass_props.izz + mass_props_semi.izz;

  // Clean up
  MU_free_poly_data( &data );

  // Successful return
  return( XP_SUCCESS );
}

/* end of file */
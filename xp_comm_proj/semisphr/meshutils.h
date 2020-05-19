/****************************************************************************/

#ifndef __LIB_UTILITY_MESH_UTILS__
#define __LIB_UTILITY_MESH_UTILS__

/****************************************************************************/

#include "avs/om.h"

/****************************************************************************/

#define PI 3.14159265359
#define ZERO_TOL 0.0001

/****************************************************************************/

typedef enum 
{
  kSOLID,
  kHOLLOW
} 
body_type_e;

/* Data for polyline based bodies of revolution */
typedef struct _poly_data
{
  body_type_e  body_type;
  int          num_thetas;
  int          num_points_outside;
  int          num_points_inside;
  float       *r_outside;
  float       *z_outside;
  float       *r_inside;
  float       *z_inside;
  int          gen_first_end_cap;
  int          gen_last_end_cap;
} 
poly_data_t;

/* Data for semi-spheres */
typedef struct _semi_data
{
  body_type_e  body_type;
  int          num_thetas;
  int          num_phis;
  float        half_angle;
  float        r_outside;
  float        r_inside;
  float        z_offset;
  int          gen_end_cap;
} 
semi_data_t;

typedef struct _mass_props
{
  float  density;
  float  mass;
  float  zcg;
  float  izz;
} 
mass_props_t;

/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/

int 
MU_init_field( OMobj_id  field_id );

void 
MU_free_poly_data( poly_data_t  *data );

int 
MU_gen_body_of_rev( OMobj_id      field_id,
		    poly_data_t  *data );

int 
MU_gen_semi_sphere( OMobj_id      field_id,
		    semi_data_t  *data );

void 
MU_gen_cyl_coords( int     num_thetas,
		   float   r_value1,
		   float   z_value1,
		   float   r_value2,
		   float   z_value2,
		   float  *coords );

void 
MU_gen_circle_coords( int     num_thetas,
		      float   r_value,
		      float   z_value,
		      float  *coords );

int
MU_mass_props_body_of_rev( poly_data_t   *data, 
			   mass_props_t  *mass_props ); 

int
MU_mass_props_semi_sphere( semi_data_t   *data, 
			   mass_props_t  *mass_props ); 

int
MU_mass_props_frustum( float   r0, 
		       float   r1, 
		       float   z0,
		       float   z1, 
		       float   rho,
		       float  *mass, 
		       float  *zcg, 
		       float  *izz ); 

int
MU_mass_props_sphere( float   r0, 
		      float   x0, 
		      float   offset,
		      float   rho,
		      float  *mass, 
		      float  *zcg, 
		      float  *izz ); 

int 
MU_gen_field( OMobj_id  field_id,
	      float    *vertex, 
	      int       ncols, 
	      int       nrows,
	      int       flip );

int
MU_gen_mesh_normals( float *verts,
		     float *normals,
		     int    ncols, 
		     int    nrows,
		     int    flip );

void
MU_sub_vec(float res[3], float a[3], float b[3]);

void
MU_cross_product(float *res, float *vec1, float *vec2);

void
MU_normalize_verts( float *v, int n );

void
MU_flip_verts( float *v, int n );

/****************************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/****************************************************************************/

#endif  /* __LIB_UTILITY_MESH_UTILS__ */

/****************************************************************************/

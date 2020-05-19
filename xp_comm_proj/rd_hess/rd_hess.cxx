// 
// Read Hess Module
//

// Include files
#include "xp_comm_proj/rd_hess/gen.h"
#include <math.h>

// Local defines
#define MAX_VERT 2000    /* MAXIMUM NUMBER of VERTS allowed per network */
typedef float FLOAT3[3];

// Static prototypes
static int 
init_geom_field( OMobj_id  field_id );

static int 
parse_hess_file( OMobj_id  field_id,
		 FILE     *hess_fp );

static int 
gen_field( OMobj_id  field_id,
	   float    *vertex, 
	   int       n, 
	   int       m );

int
gen_mesh_normals( float *verts,
                  float *normals,
		  int ncols, 
		  int nrows );

static void
sub_vec(float res[3], float a[3], float b[3]);

static void
cross_product(float *res, float *vec1, float *vec2);

static void
normalize_verts( float *v, int n );

static void
flip_verts( float *v, int n );

// Read Hess function


int
ReadHess_ReadHessMods_Read_Hess_File::update(OMevent_mask event_mask, int seq_num)
{

   FILE     *hess_fp;

   // Initialize geometry field
   if( init_geom_field( (OMobj_id)out_fld.obj_id() ) == 0) 
   {
      printf( "Error initializing field\n" );
      return(0);
   } 

   // Open Hess file 
   fprintf(stderr,"Read_Hess: Opening: %s \n", (char *)hess_filename); 
   if ((hess_fp = fopen( (char *)hess_filename, "r")) == NULL) {
      printf( "Error opening Hess file = %s\n", (char *)hess_filename );
      return(0);
   }

   // Read data from Hess geometry file and generate field
   if( parse_hess_file( (OMobj_id)out_fld.obj_id(), hess_fp ) == 0) 
   {
      printf( "Error parsing Hess file = %s\n", (char *)hess_filename );
      return(0);
   } 

   // Close Hess file 
   fclose(hess_fp);

   // Successful return
   return(1);
}

static int 
init_geom_field( OMobj_id  field_id )
{
  // Reset the number of nodes
  FLDset_nnodes( field_id, 0 );

  // Reset the number of polys
  OMobj_id polytri_id;
  FLDget_cell_set( field_id, 0, &polytri_id );
  FLDset_npolys( polytri_id, 0 );
  
  OMobj_id polyline_id;
  FLDget_cell_set( field_id, 1, &polyline_id );
  FLDset_npolys( polyline_id, 0 );

  // Set the normals node data
  float normals[3] = {0,0,0};
  FLDset_node_data( field_id, 0, (char *)&normals[0], 
		    DTYPE_FLOAT, 0, OM_SET_ARRAY_COPY ); 

  // Successful return
  return(1);
}

static int 
parse_hess_file( OMobj_id  field_id,
		 FILE     *hess_fp )
{
   char type[100];
   int i, j, n, m, k;
   FLOAT3 vertex[MAX_VERT]; 
   int num_verts = 0;
   int draw_mesh = 0;

   // Variables for parsing 
   char string[81];
   int pt_counter = 0;
   int line_counter = 0;
   float x_coord, y_coord, z_coord;
   int net_value;
   int lift_value;
   char x_char[10];
   char y_char[10];
   char z_char[10];
   char net_name[15];
   char dummy30[30];
   char dummy31[31];
   char dummy65[65];

   // Read in each line from the ile
   while( fgets( string,81,hess_fp ) != NULL ) 
   { 
     // Parse out flags
     sscanf( string, "%10c%10c%10c", x_char, y_char, z_char );
     sscanf( string, "%30c%1d", dummy30, &net_value );
     sscanf( string, "%31c%1d", dummy31, &lift_value );
     
     // Parse out coordinates
     sscanf( x_char, "%f", &x_coord );
     sscanf( y_char, "%f", &y_coord );
     sscanf( z_char, "%f", &z_coord );
     
     // Starting a new network
     if( net_value == 2 ) 
     { 
       // If a network is loaded, generate the field data and connectivity
       if( draw_mesh ) 
       {	 
         if( gen_field( field_id, &vertex[0][0], pt_counter, 
			line_counter ) == 0 )
	 {
	   printf( "Error generating field for current network\n");
	   return(0);
	 }
       }

       // Reset counters 
       draw_mesh = 1;
       line_counter = 1;
       pt_counter = 1;
       num_verts  = 0;
     }

     // Starting a new row
     else if( net_value == 1 ) 
     {
       line_counter ++;
       pt_counter = 1;
       num_verts ++;
       if( num_verts == MAX_VERT )
       {
          printf( "Maximum number of verts exceeded\n" );
          return(0);
       }
     }
     
     // Adding another point on a row
     else if( net_value == 0 ) 
     {
       pt_counter ++;
       num_verts ++;
       if( num_verts == MAX_VERT )
       {
          printf( "Maximum number of verts exceeded\n" );
          return(0);
       }
     } 
     
     // Bad net flag
     else 
     {
       printf( "Hess: Error reading net_value: %d\n", net_value );
       return(0);
     }
     
     // Save coordinate in temp array
     vertex[num_verts][0] = x_coord;
     vertex[num_verts][1] = y_coord;
     vertex[num_verts][2] = z_coord;
     
   } 
   
   // Generate the field data and connectivity for the last network
   if( gen_field( field_id, &vertex[0][0], pt_counter, line_counter ) == 0 )
   {
     printf( "Error generating field for current network\n");
     return(0);
   }

   // Successful return   
   return(1);
}

static int 
gen_field( OMobj_id  field_id,
	   float    *vertex, 
	   int       ncols, 
	   int       nrows )
{
  int i,j;
  
  // Get the previous number of nodes
  int prev_nnodes;
  FLDget_nnodes( field_id, &prev_nnodes );
  
  // Add the new number of nodes to the previous number
  int num_nnodes = prev_nnodes + 
                   2*ncols + 2*(nrows-1) + 4*ncols*(nrows-1) - 1; 
  FLDset_nnodes( field_id, num_nnodes );
  
  // Get the coordinate array
  int size;
  float *coords;
  FLDget_coord( field_id, &coords, &size, OM_GET_ARRAY_RW );
  if( coords == NULL ) 
  {
    printf( "Coords pointer is NULL\n");
    return(0);
  }
  
  // Get the normals data array
  int type;
  float *normals;
  FLDget_node_data( field_id, 0, &type, (char **)&normals, &size, 
		    OM_GET_ARRAY_RW );
  if( normals == NULL ) 
  {
    printf( "Normals pointer is NULL\n");
    ARRfree( coords );
    return(0);
  }
   
  // Allocate an array for the mesh normals
  float *mesh_normals = (float *)malloc( ncols*nrows*3 * sizeof(float) );
  if( mesh_normals == NULL ) 
  {
    printf( "Normals pointer or mesh is NULL\n");
    ARRfree( coords );
    ARRfree( normals );
    return(0);
  }

  // Generate normals for the mesh coordinates
  if( gen_mesh_normals( vertex, mesh_normals, ncols, nrows ) == 0 )
  {
    printf( "Error generating normals for mesh\n" );
    ARRfree( coords );
    ARRfree( normals );
    free( mesh_normals );
    return(0);
  };

  //
  // Set the coordinate values for the polytri points
  //
  int iv1 = 0;
  int ic = prev_nnodes * 3;
  
  // For each "strip"
  for( j = 0; j < nrows - 1; j++)
  {
    int iv2 = (j+1)*ncols*3;
    
    // Set first point on each successive strip
    if( j > 0 )
    {
      coords[ic]   = vertex[iv1];
      coords[ic+1] = vertex[iv1+1];
      coords[ic+2] = vertex[iv1+2];
      memcpy( (void *)&normals[ic],
	      (void *)&mesh_normals[iv1], sizeof(float)*3 );
      ic+=3;
    }
    
    // For each set of points along a "strip"
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
    
    // Repeat last point on strip
    coords[ic]   = vertex[iv2-3];
    coords[ic+1] = vertex[iv2-2];
    coords[ic+2] = vertex[iv2-1];
    memcpy( (void *)&normals[ic],
	    (void *)&mesh_normals[iv2-3], sizeof(float)*3 );
    ic+=3;
  }      
  
  //
  // Set the coordinate values for the "horizontal" polyline points
  //
  // For each row
  iv1 = 0;
  for( j = 0; j < nrows; j++)
  {
    // For each set of points along a row
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
  
  //
  // Set the coordinate values for the "vertical" polyline points
  //
  // For each column
  for( j = 0; j < ncols; j++)
  {
    // For each set of points along a column
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
  
  //
  // Set up the cell set connectivity information
  // 
  // Get the polytri cell_set
  OMobj_id polytri_id;
  FLDget_cell_set( field_id, 0, &polytri_id );
  
  // Increment the number of polytri polys
  int polytri_npolys;
  FLDget_npolys( polytri_id, &polytri_npolys );
  FLDset_npolys( polytri_id, polytri_npolys + 1 );
   
  // Set the poly connect list for the polytri cell_set
  int *polytri_poly_conn_array;
  FLDget_poly_connect( polytri_id, &polytri_poly_conn_array, &size, 
		       OM_GET_ARRAY_RW );
  int polytri_end_index = prev_nnodes + (nrows-1)*(ncols*2+2) - 3;
  polytri_poly_conn_array[polytri_npolys*2]   = prev_nnodes;
  polytri_poly_conn_array[polytri_npolys*2+1] = polytri_end_index;
  ARRfree( polytri_poly_conn_array );
  
  // Set the polyline cell_set
  OMobj_id polyline_id;
  FLDget_cell_set( field_id, 1, &polyline_id );
  
  // Increment the number of polyline polys
  int polyline_npolys;
  FLDget_npolys( polyline_id, &polyline_npolys );
  FLDset_npolys( polyline_id, (polyline_npolys + ncols + nrows) );
  
  // Set the poly connect list for the polyline cell_set
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
  
  // Successful return
  return(1);
}

int
gen_mesh_normals( float *verts,
                  float *normals,
		  int ncols, 
		  int nrows )
{
  // Declare some local variables
  register int i,j;
  register int k = 0;
  float tmp1[3], tmp2[3];

  // Do the first point on the first row 
  sub_vec( tmp1, &verts[0], &verts[3] );
  sub_vec( tmp2, &verts[0], &verts[ncols*3] );
  cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  // Do the "middle" points in the first row 
  for( i = 1; i < ncols-1; i++ )
  { 
    sub_vec( tmp1, &verts[(i-1)*3], &verts[(i+1)*3] );
    sub_vec( tmp2, &verts[i*3], &verts[(ncols+i)*3] );
    cross_product( &normals[k], tmp1, tmp2 );
    k += 3; 
  }

  // Do the last point on the first row 
  sub_vec( tmp1, &verts[(ncols-1)*3], &verts[(2*ncols-1)*3] );
  sub_vec( tmp2, &verts[(ncols-1)*3], &verts[(ncols-2)*3] );
  cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  // Loop though the "middle" rows
  for( j = 1; j < nrows-1; j++ ) 
  { 
    // Do the first point on each row 
    sub_vec( tmp1, &verts[((j+1)*ncols)*3], &verts[((j-1)*ncols)*3] );
    sub_vec( tmp2, &verts[(j*ncols)*3], &verts[(j*ncols+1)*3] );
    cross_product( &normals[k], tmp1, tmp2 );
    k += 3;

    // Do the "middle" points in each row 
    for( i = 1; i < ncols-1; i++ ) 
    { 
      sub_vec( tmp1, &verts[(j*ncols+(i-1))*3], &verts[(j*ncols+(i+1))*3] );
      sub_vec( tmp2, &verts[((j-1)*ncols+i)*3], &verts[((j+1)*ncols+i)*3] );
      cross_product( &normals[k], tmp1, tmp2 );
      k += 3;
    }

    // Do the last point on each row 
    sub_vec( tmp1, 
	     &verts[(j*ncols+ncols-2)*3], 
	     &verts[(j*ncols+ncols-1)*3] );
    sub_vec( tmp2, 
	     &verts[((j-1)*ncols+(ncols-1))*3], 
	     &verts[((j+1)*ncols+(ncols-1))*3] );
    cross_product( &normals[k], tmp1, tmp2 );
    k += 3;
  }

  // Do the first point on the last row 
  sub_vec( tmp1, &verts[(nrows-1)*ncols*3], &verts[((nrows-1)*ncols+1)*3] );
  sub_vec( tmp2, &verts[(nrows-2)*ncols*3], &verts[(nrows-1)*ncols*3] );
  cross_product( &normals[k], tmp1, tmp2 );
  k += 3;

  // Do the "middle" points on the last row 
  for (i = 1; i < ncols-1; i++) {
    sub_vec( tmp1, 
	     &verts[((nrows-1)*ncols+(i-1))*3], 
	     &verts[((nrows-1)*ncols+(i+1))*3] );
    sub_vec( tmp2, 
	     &verts[((nrows-2)*ncols+i)*3], 
	     &verts[((nrows-1)*ncols+i)*3] );
    cross_product( &normals[k],tmp1, tmp2 );
    k += 3;
  }
  
  // Do the last point on the last row 
  sub_vec( tmp1, 
	   &verts[((nrows-1)*ncols+(ncols-2))*3],
	   &verts[((nrows-1)*ncols+(ncols-1))*3] );
  sub_vec( tmp2,
	   &verts[((nrows-2)*ncols+(ncols-1))*3],
	   &verts[((nrows-1)*ncols+(ncols-1))*3] );
  cross_product( &normals[k], tmp1, tmp2 );

  // Normalize and flip normals
  normalize_verts( normals, nrows*ncols );
  flip_verts( normals, nrows*ncols );

  // Successful return
  return(1);
}

static void
sub_vec( float res[3], float a[3], float b[3] )
{
   res[0] = a[0] - b[0];
   res[1] = a[1] - b[1];
   res[2] = a[2] - b[2];
}

static void
cross_product( float *res, float *vec1, float *vec2 ) 
{
   res[0] = vec1[1]*vec2[2] - vec2[1]*vec1[2];
   res[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
   res[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

static void
normalize_verts( float *v, int n )
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

static void
flip_verts( float *v, int n )
{
   register int i;

   for(i = 0; i < n; i++, v += 3 ) 
   {
     *v *= -1.0; *(v+1) *= -1.0; *(v+2) *= -1.0;
   }
}

// end of file


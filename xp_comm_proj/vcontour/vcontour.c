
#include "gen.h"


#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif

#ifndef DTYPE_LONG
#define DTYPE_LONG DTYPE_INT
#endif

/* #define DEBUG */

#ifndef OM_TYPE_LONG
extern int FUNCisoline( OMobj_id, int, int, float*, int, OMobj_id );
static int elemsize[DNTYPES] = { sizeof(char), sizeof(char), sizeof(short), sizeof(int), 
                                 sizeof(float), sizeof(double), 0, 0, 0, 0, 0 };
#else
extern int FUNCisoline( OMobj_id, int, int, float*, int, int, OMobj_id );
static int elemsize[DNTYPES] = { sizeof(char), sizeof(xp_byte), sizeof(short), sizeof(int), 
                                 sizeof(float), sizeof(double), sizeof(xp_long), 0, 0, 0, 0, 0 };
#endif

extern int FUNCorthoslice_unif( OMobj_id, int, int, OMobj_id );



int GenContours(OMobj_id VolumeToContour_id, OMevent_mask event_mask, int seq_num)
{
  int tmp_nsets, tmp_cell_nnodes; 
  xp_long tmp_ncells, tmp_nnodes;
  int tmp_nspace;
  xp_long *tmp_node_connect, tmp_size;
  float *tmp_coord;

  xp_long *out_node_connect;
  xp_long *dst_node_connect;
  OMobj_id dst_cell_set;
  int dst_cell_nnodes;
  float *dst_coords;
  xp_long out_ncells;

  char *out_data, *dst_data, *tmp_data;
  int out_veclen, tmp_veclen;
  xp_long out_ndata, tmp_ndata;
  int tmp_data_type;
  int out_data_type;
  int tmp_ncomp;

  OMobj_id tmp_fld, tmp_contour;
  OMobj_id src_id, dst_id;
  OMobj_id tmp_cell_set;
  /* OMobj_id level_id; */
  int axis, spacing;
  xp_long *dims, size;
  int first_slice;
  int last_slice;
  float *level;
  int plane;
  int color;
  int nlev;
  int type;

  float *out_coords;
  xp_long out_ncoords;

  /* Get axis's value 
   */ 
  if (OMget_name_int_val(VolumeToContour_id, OMstr_to_name("axis"), &axis) != 1) 
    axis = 0;
  
  /* Get color's value 
   */ 
  if (OMget_name_int_val(VolumeToContour_id, OMstr_to_name("color"), &color) != 1) 
    color = 0;

#ifdef DEBUG
fprintf(stderr,"Color: %i\n",color);
#endif
  
  /* Get spacing's value 
   */ 
  if (OMget_name_int_val(VolumeToContour_id, OMstr_to_name("slice_spacing"), &spacing) != 1) 
    spacing = 1;
  
  if( spacing <= 0 )
    {
      fprintf( stderr, "Invalid spacing: %d\n", spacing );
      return( 0 );
    }

  /* Get level array
   */
  level = (float *)OMret_name_array_ptr(VolumeToContour_id,
                   OMstr_to_name("level"),OM_GET_ARRAY_RD,&nlev,&type);

  src_id = OMfind_subobj(VolumeToContour_id,OMstr_to_name("Input_Field"),OM_OBJ_RD);
  tmp_fld = OMfind_subobj(VolumeToContour_id,OMstr_to_name("Temp_Field"),OM_OBJ_RW); 
  tmp_contour = OMfind_subobj(VolumeToContour_id,OMstr_to_name("Temp_Contour"),OM_OBJ_RW); 
  dst_id = OMfind_subobj(VolumeToContour_id,OMstr_to_name("Contours"),OM_OBJ_RW); 

  FLDget_dims( src_id, &dims, &size );

  if( size != 3 )
    {
      fprintf( stderr, "Invalid input field, size: %d\n", size );
      return( 0 );
    }

  if( (axis < 0) || (axis > size) )
    {
      fprintf( stderr, "Invalid axis: %d\n", axis );
      return( 0 );
    }

  /* Get first slice
   */ 
  if (OMget_name_int_val(VolumeToContour_id, OMstr_to_name("first_slice"), &first_slice) != 1) 
    first_slice = 0;
  
  /* Get last slice
   */ 
  if (OMget_name_int_val(VolumeToContour_id, OMstr_to_name("last_slice"), &last_slice) != 1)
    last_slice = dims[axis] - 1;

  if( (first_slice < 0) || (first_slice >= dims[axis]) || (first_slice > last_slice) )
    {
      fprintf( stderr, "Invalid first slice %d\n", first_slice );
      return( 0 );
    }

  if( (last_slice < 0) || (last_slice >= dims[axis]) || (last_slice < first_slice) )
    {
      fprintf( stderr, "Invalid last slice %d\n", last_slice );
      return( 0 );
    }

  ARRfree( dims );

  out_veclen = -1;
  out_ndata = 0;
  out_ncells = 0;
  out_ncoords = 0;
  out_data = NULL;
  out_coords = NULL;
  out_node_connect = NULL;
  for( plane = first_slice; plane <= last_slice; plane += spacing )
    {
      xp_long index, offset, i, j;

      if( FUNCorthoslice_unif( src_id, plane, axis, tmp_fld ) == 0 )
        return( 0 );

#ifdef OM_TYPE_LONG
      if( FUNCisoline( tmp_fld, 0, nlev, level, color,	1, tmp_contour ) == 0 )
#else 
      if( FUNCisoline( tmp_fld, 0, nlev, level, color, tmp_contour ) == 0 )
#endif	  
        return( 0 );

      /* Read Coordinates from Temporary Contour
       */
      FLDget_nnodes( tmp_contour, &tmp_nnodes );
      FLDget_nspace( tmp_contour, &tmp_nspace );
      FLDget_coord( tmp_contour, &tmp_coord, &tmp_size, OM_GET_ARRAY_RD );

#ifdef DEBUG
      fprintf(stderr,"plane %d, nnodes = %d, nspace = %d, ncoords = %d\n", 
                 plane, tmp_nnodes, tmp_nspace, tmp_size ); 
#endif

      if( tmp_size > 0 )
        {
          offset = out_ncoords;
          out_ncoords += tmp_nnodes;
          out_coords = realloc( out_coords, 3 * out_ncoords * sizeof(float) );

          index = 3 * offset;
          for( i = 0; i < tmp_nnodes; i++ )
            {
              switch( axis )
                {
                case 0:
                  out_coords[index++] = (float)plane;
                  out_coords[index++] = tmp_coord[(2*i)];
                  out_coords[index++] = tmp_coord[(2*i)+1];
                  break;
                case 1:
                  out_coords[index++] = tmp_coord[(2*i)];
                  out_coords[index++] = (float)plane;
                  out_coords[index++] = tmp_coord[(2*i)+1];
                  break;
                case 2:
                  out_coords[index++] = tmp_coord[(2*i)];
                  out_coords[index++] = tmp_coord[(2*i)+1];
                  out_coords[index++] = (float)plane;
                  break;
                }
            }
          
          /* Read cell data from Temporary Contour
           */
          FLDget_ncell_sets( tmp_contour, &tmp_nsets );
          for( i = 0; i < tmp_nsets; i++ )
            {
              FLDget_cell_set( tmp_contour, i, &tmp_cell_set );
              FLDget_ncells( tmp_cell_set, &tmp_ncells );
              FLDget_cell_set_nnodes( tmp_cell_set, &tmp_cell_nnodes );
              FLDget_node_connect( tmp_cell_set, &tmp_node_connect, &tmp_size, OM_GET_ARRAY_RD );

#ifdef DEBUG               
              fprintf(stderr,"cell set %d, ncells = %d, cell_nnodes = %d, size = %d\n", 
                      i, tmp_ncells, tmp_cell_nnodes, tmp_size );
#endif              

              index = out_ncells * 2;
              out_ncells += tmp_ncells;
              out_node_connect = realloc( out_node_connect, 2 * out_ncells * sizeof(xp_long) );
              
              for( j = 0; j < tmp_ncells; j++ )
                {
                  out_node_connect[index++] = tmp_node_connect[(2*j)] + offset;
                  out_node_connect[index++] = tmp_node_connect[(2*j)+1] + offset;
                }

              if( tmp_node_connect )
                ARRfree( (char *)tmp_node_connect );
            }

          /* Node data
           */
          FLDget_node_data_ncomp( tmp_contour, &tmp_ncomp );
          if( tmp_ncomp )
            {
              if( tmp_ncomp > 1 )
                fprintf( stderr, "Don't know how to handle multiple components\n" );

              FLDget_node_data_veclen( tmp_contour, 0, &tmp_veclen );
              FLDget_node_data( tmp_contour, 0, &tmp_data_type, &tmp_data, &tmp_ndata, OM_GET_ARRAY_RD );

              if( out_veclen == -1 )
                {
                  out_veclen = tmp_veclen;
                  out_data_type = tmp_data_type;
                }
              else
                {
                  if( out_veclen != tmp_veclen )
                    {
                      fprintf( stderr, "Veclen mismatch in plane %d, previous = %d, current = %d...\n", 
                               plane, out_veclen, tmp_veclen );
                      
                      return( 0 );
                    }

                  if( out_data_type != tmp_data_type )
                    {
                      fprintf( stderr, "Data type mismatch in plane %d, previous = %d, current = %d...\n", 
                               plane, out_data_type, tmp_data_type );
                      
                      return( 0 );
                    }
                }

              index = out_ndata;
              out_ndata += tmp_ndata;
              out_data = realloc( out_data, out_ndata * elemsize[out_data_type] );
              memcpy( out_data+index*elemsize[out_data_type], tmp_data, tmp_ndata * elemsize[out_data_type] );

              ARRfree( (char *)tmp_data );
            }
        }
    }
  
  /* Output Contours mesh 
   */
  if( (out_ncoords > 0) && (out_ncells > 0) )
  {
      FLDset_nnodes( dst_id, out_ncoords );
      FLDset_nspace( dst_id, 3 );
   
      /* Coordinates
       */
      dst_coords = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3 * out_ncoords, NULL );
      memcpy( dst_coords, out_coords, 3 * out_ncoords * sizeof(float) );
      FLDset_coord( dst_id, dst_coords, 3 * out_ncoords, OM_SET_ARRAY_FREE );

      /* Cell data
       */
      FLDset_ncell_sets( dst_id, 0 );
      FLDadd_cell_set( dst_id, "Line" );

      FLDget_cell_set( dst_id, 0, &dst_cell_set );
      FLDset_ncells( dst_cell_set, out_ncells );
      FLDget_cell_set_nnodes( dst_cell_set,  &dst_cell_nnodes );
      dst_node_connect = (xp_long *)ARRalloc(NULL, DTYPE_LONG, out_ncells * dst_cell_nnodes, NULL );
      memcpy( dst_node_connect, out_node_connect, out_ncells * dst_cell_nnodes * sizeof(xp_long) );
      FLDset_node_connect( dst_cell_set, dst_node_connect, out_ncells * dst_cell_nnodes, OM_SET_ARRAY_FREE);

      if( out_ndata > 0 )
      {
	      FLDset_node_data_ncomp( dst_id, 1 );
		  FLDset_node_data_veclen( dst_id, 0, out_veclen );
          dst_data = (char *)ARRalloc( NULL, out_data_type, out_veclen * out_ndata, NULL );
       	  memcpy( dst_data, out_data, out_ndata * elemsize[out_data_type] );
       	  FLDset_node_data( dst_id, 0, dst_data, out_data_type, out_ndata * out_veclen, OM_SET_ARRAY_FREE );
      }else{
	      FLDset_node_data_ncomp( dst_id, 0 );
	  }
  }

  if( out_data )
    free( out_data );

  if( out_coords )
    free( out_coords );
  
  if( out_node_connect )
    free( out_node_connect );

  return( 1 );
}

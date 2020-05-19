
#include "express.h"

int CvtToPolyTri(OMobj_id ConvertToTriStrip_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id in_id;
  int in_nspace, in_nnodes, in_size;
  int  in_nsets, in_ncells;
  int  in_cell_nnodes, *in_node_connect;

  OMobj_id in_cell_set;
  float *in_coord;
  int in_ncomp, in_comp_count, in_veclen;
  int    in_data_type, in_ndata;
  char  *in_data;

  OMobj_id output_id;
  OMobj_id output_cell_set;
  float *output_coord;
  char *output_data;

  int *poly_array;
  int num_polys;

  /* Get input and output object id's
   */
  in_id = OMfind_subobj(ConvertToTriStrip_id, OMstr_to_name("in"), OM_OBJ_RD);
  output_id = OMfind_subobj(ConvertToTriStrip_id, OMstr_to_name("output"), OM_OBJ_RW);
  
  /* Get mesh number of nodes, nspace, and number of cell sets, and coordinate array
   */
  FLDget_nnodes( in_id, &in_nnodes );
  FLDget_nspace( in_id, &in_nspace );

  /* We currently only handle one cell set
   */
  FLDget_ncell_sets( in_id, &in_nsets );
  if( in_nsets != 1 )
    {
      fprintf( stderr, "Cvt2Tri: Can only handle 1 cell set per field\n" );
      return( 0 );
    }

  /* Get input coordinates
   */
  FLDget_coord( in_id, &in_coord, &in_size, OM_GET_ARRAY_RD );

  /* Get cell set id 
   */
  FLDget_cell_set( in_id, 0, &in_cell_set );
      
  /* Get number of cells 
   */
  FLDget_ncells( in_cell_set, &in_ncells );
      
  /* Get number of nodes in each cell 
   */
  FLDget_cell_set_nnodes( in_cell_set, &in_cell_nnodes );
  if( (in_cell_nnodes != 3) && (in_cell_nnodes != 4) )
    {
      fprintf( stderr, "Cvt2Tri: Can only handle quads or tris\n" );
      return( 0 );
    }

  /* Get node connectivity list 
   */
  FLDget_node_connect( in_cell_set, &in_node_connect, &in_size, OM_GET_ARRAY_RD );

  /* Assuming we got a connect list, 
   *      convert it to tristrips, 
   *      gather and output vertices,
   *      for each node array
   *         gather and output node data
   */
  if( in_node_connect )
    {
      int strips[2];

      /* printf( "Cvt2Tri: nnodes = %d, ncells = %d\n", in_cell_nnodes, in_ncells ); */
      GDgraph_cvt_thorough(in_cell_nnodes, in_ncells, in_node_connect, &poly_array, NULL);
      
      /* Set mesh nnodes and nspace
       */
      num_polys = poly_array[0];
      FLDset_nnodes(output_id, num_polys);
      FLDset_nspace(output_id, in_nspace);
      
      output_coord = (float *)ARRalloc(NULL, DTYPE_FLOAT, in_nspace * num_polys, NULL);

      GDgraph_gather(NULL, NULL, (char *)output_coord, poly_array, 
                     (char *)in_coord, OM_TYPE_FLOAT, in_nspace, in_nspace, 0);

      FLDset_coord(output_id, output_coord, in_nspace * num_polys, OM_SET_ARRAY_FREE);

      FLDset_ncell_sets( output_id, 0 );
      FLDadd_cell_set( output_id, "Polytri" );
      FLDget_cell_set( output_id, 0, &output_cell_set );

      FLDset_npolys( output_cell_set, 1 );
      strips[0] = 0; strips[1] = num_polys - 1;
      FLDset_poly_connect(output_cell_set, strips, 2, OM_SET_ARRAY_COPY);

      ARRfree((char *)in_node_connect);
      
      /* Get number of node data components 
       */
      FLDget_node_data_ncomp( in_id, &in_ncomp );
      FLDset_node_data_ncomp( output_id, in_ncomp );
      
      /* For each node data component get veclen, type and data arry itself 
       */
      for( in_comp_count = 0; in_comp_count < in_ncomp; in_comp_count++ )
        {
          /* Get veclen 
           */
          FLDget_node_data_veclen( in_id, in_comp_count, &in_veclen );
          FLDset_node_data_veclen( output_id, in_comp_count, in_veclen );
          
          /* Get data array and data_type
           */
          FLDget_node_data( in_id, in_comp_count, &in_data_type, &in_data,
                            &in_ndata, OM_GET_ARRAY_RD );
          
          if( in_data )
            {
              /* printf( "Gathering %d polys, veclen = %d\n", num_polys, in_veclen ); */
              
              output_data = (char *)ARRalloc(NULL, in_data_type, in_veclen * num_polys, NULL);
              
              GDgraph_gather(NULL, NULL, output_data, poly_array,
                             (char *)in_data, in_data_type,
                             in_veclen, in_veclen,
                             0);
              
              FLDset_node_data(output_id, in_comp_count, output_data, in_data_type,
                               in_veclen * num_polys, OM_SET_ARRAY_FREE);
              
              ARRfree((char *)in_data);
            }
        }
    }

  return( 1 );
}

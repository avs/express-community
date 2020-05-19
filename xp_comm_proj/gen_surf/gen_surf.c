
#include "gen.h"

/*
#define DEBUG
*/

int GenerateSurface(OMobj_id GenerateSurface_id, OMevent_mask event_mask, int seq_num)
{
   OMobj_id input_id;

   int input_ndim, *input_dims, input_nspace, input_npoints;
   float *input_points;
   int input_ncomp, input_comp_count, input_veclen;
   int    input_data_type, input_ndata;
   char  *input_data;

   OMobj_id output_id;
   int output_nspace;
   OMobj_id output_cell_set;
   int *output_polys;
   int *polyptr;

   int max_coords;
   float *coords;
   int ncoords;

   float *n0ptr;
   float *n1ptr;
   float *cptr;

   double threshold;
   int reduction;
   int npolys;
   int i, j;

   if( OMget_name_real_val( GenerateSurface_id, OMstr_to_name("threshold"), &threshold ) != OM_STAT_SUCCESS )
   {
       threshold=0.0;
   }

#ifdef DEBUG
   sprintf(stderr,"Threshold + %g\n", threshold );
#endif

   /* Get input and output id 
    */
   input_id = OMfind_subobj(GenerateSurface_id, OMstr_to_name("input"), OM_OBJ_RD);
   output_id = OMfind_subobj(GenerateSurface_id, OMstr_to_name("output"), OM_OBJ_RW);

   /* Get mesh dims, space, and node data 
    */
   FLDget_dims( input_id, &input_dims, &input_ndim );
   FLDget_nspace( input_id, &input_nspace );
   FLDget_points( input_id, &input_points, &input_npoints, OM_GET_ARRAY_RD );

   if( (input_nspace != 2) || (input_ndim != 2) )
   {
       fprintf( stderr, "GenerateSurface: nspace and ndims must be 2\n" );
       return( 0 );
   }

   FLDget_node_data_ncomp( input_id, &input_ncomp );
   if( input_ncomp != 1 )
   {
       fprintf( stderr, "GenerateSurface: Only works for 1 component\n" );
       return( 0 );
   }

   /* Set output mesh size
    */
   FLDget_node_data_veclen( input_id, 0, &input_veclen );
   if( input_veclen != 1 )
   {
       fprintf( stderr, "GenerateSurface: input veclen must be 1\n" );
       return( 0 );
   }

   input_data_type = OM_TYPE_FLOAT;
   FLDget_typed_node_data( input_id, 0, &input_data_type, 
                     &input_data, &input_ndata, OM_GET_ARRAY_RD );

   if( !input_data )
   {
       fprintf( stderr, "GenerateSurface: Unable to read node data\n" );
       return( 0 );
   }

   /* Setup connection list
    */
   output_nspace = 3;
   npolys = input_dims[1] - 1;
   max_coords = npolys * (input_dims[0] * 2);

   FLDset_ncell_sets( output_id, 0 );
   FLDadd_cell_set( output_id, "Polytri" );
   FLDget_cell_set( output_id, 0, &output_cell_set );
   
   FLDset_npolys( output_cell_set, npolys );
   output_polys = (int *)ARRalloc( NULL, DTYPE_INT, npolys * 2, NULL );

   /* Setup Coordinate array
    */
   coords = (float *)malloc( output_nspace * max_coords * sizeof(float) );

   ncoords = 0;
   cptr = coords;
   polyptr = output_polys;
   for( i = 0; i < npolys; i++ )
   {
       reduction = 0;
       *polyptr++ = ncoords;

       n0ptr = (float *)input_data + (i * input_dims[0]);
       n1ptr = (float *)input_data + ((i + 1) * input_dims[0]);
       for( j = 0; j < input_dims[0]; j++ )
       {
           *cptr++ = (float)j;
           *cptr++ = (float)i;
           *cptr++ = *n0ptr++;
           ncoords += 1;
           
           *cptr++ = (float)j;
           *cptr++ = (float)(i+1);
           *cptr++ = *n1ptr++;
           ncoords += 1;

           if( abs((*n0ptr - *(n0ptr-1))) < threshold )
           {
               j += 1;
               n0ptr += 1;
               n1ptr += 1;
               reduction += 1;
           }
       }

       *polyptr++ = ncoords - 1;

#ifdef DEBUG
       fprintf(stderr, "Row %d, reduction = %d\n", i, reduction );
#endif
   }

   FLDset_nnodes( output_id, ncoords );
   FLDset_nspace( output_id, output_nspace );

   FLDset_coord( output_id, coords, ncoords * output_nspace, OM_SET_ARRAY_COPY );
   FLDset_poly_connect( output_cell_set, output_polys, npolys * 2, OM_SET_ARRAY_FREE );

   if (input_data)
     ARRfree((char *)input_data);

   if (input_dims)
      ARRfree((char *)input_dims);

   if (input_points)
      ARRfree((char *)input_points);

   return( 1 );
}

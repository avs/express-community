
#include "gen.h"

void CheckVoxel( int *seed, int *dims, float min, float max, 
                unsigned char *input, unsigned char *output, char *processed )
{
   int arr_offset = (seed[2] * dims[2] * dims[1]) + (seed[1] * dims[1]) + seed[0];
   unsigned char *optr = output + arr_offset;
   unsigned char *iptr = input + arr_offset;
   char *procptr = processed + arr_offset;
   
   /* If this voxel is already processed, return  */

  
   if( *procptr )
   {
      return;
	}
   
   /* Set processed flag here before recursion */
   
   *procptr = 1;
   
   /* printf( "seed = %d, %d, %d\n", seed[0], seed[1], seed[2] ); */
   
   /* Check input value to see if it is connected */
   
   if( (*iptr > min) && (*iptr < max) )
   {
      int newseed[3];
      int x, y, z;
      
      /* Set output for this voxel */
	  
      *optr = *iptr;
      
      /* Process neighbors */
	  
      for( x = -1; x <= 1; x++ )
      {
         for( y = -1; y <= 1; y++ )
         {
            for( z = -1; z <= 1; z++ )
            {
               newseed[0] = seed[0] + x;
               newseed[1] = seed[1] + y;
               newseed[2] = seed[2] + z;
               
               if( (newseed[0] < 0) || (newseed[1] < 0) || (newseed[2] < 0) )
                  continue;
               
               if( (newseed[0] >= dims[0]) || (newseed[1] >= dims[1]) || (newseed[2] >= dims[2]) )
                  continue;
               CheckVoxel( newseed, dims, min, max, input, output, processed );
            }
         }
      }
   }
}

int CalcThreshold(OMobj_id SeededThreshold_id, OMevent_mask event_mask, int seq_num)
{
   OMobj_id Input_Field_id;
   int Input_Field_ndim, *Input_Field_dims, Input_Field_nspace, Input_Field_npoints;
   float *Input_Field_points;
   int Input_Field_ncomp, Input_Field_comp_count, Input_Field_veclen;
   int    Input_Field_data_type, Input_Field_ndata;
   char  *Input_Field_data;
   
   OMobj_id Output_Field_id;
   int *Output_Field_dims;
   int Output_Field_npoints;
   float *Output_Field_points;
   int Output_Field_data_type;
   /* int Output_Field_ndata; */
   char  *Output_Field_data;
   
   char *processed;
   
   int *seed = NULL; 
   int seed_size = 0;
   
   double min;
   double max;
   
   int i;
   
   /* Get min's value */
   
   if (OMget_name_real_val(SeededThreshold_id, OMstr_to_name("min"), &min) != 1)
      min = 0.0;
   
   /* Get max's value */
   
   if (OMget_name_real_val(SeededThreshold_id, OMstr_to_name("max"), &max) != 1)
      max = 255.0;
   
   seed = (int *)OMret_name_array_ptr(SeededThreshold_id, 
      OMstr_to_name("seed"), OM_GET_ARRAY_RD, &seed_size, NULL);
   
   if( seed_size != 3 )
   {
      fprintf( stderr, "CalcThreshold: seed value must have 3 values\n" );
      return( 0 );
   }
   
   /* Get input and output field ids */
   
   Input_Field_id = OMfind_subobj(SeededThreshold_id, OMstr_to_name("Input_Field"), OM_OBJ_RD);
   Output_Field_id = OMfind_subobj(SeededThreshold_id, OMstr_to_name("Output_Field"), OM_OBJ_RW);
   
   /* Get input dimensions, space, and extents */
   
   FLDget_dims (Input_Field_id, &Input_Field_dims, &Input_Field_ndim);
   FLDget_nspace (Input_Field_id, &Input_Field_nspace);
   FLDget_points (Input_Field_id, &Input_Field_points, &Input_Field_npoints, OM_GET_ARRAY_RD);
   
   /* Make sure input is compatible */
   
   if( Input_Field_ndim != 3 )
   {
      fprintf( stderr, "CalcThreshold: Only process 3d fields\n" );
      return( 0 );
   }
   
   if( (Input_Field_dims[0] <= 0) || (Input_Field_dims[1] <= 0) || (Input_Field_dims[2] <= 0) )
   {
      fprintf( stderr, "CalcThreshold: No data to process\n" );
      return( 0 );
   }
   
   /* Set output field dimensions to match input */
   
   FLDset_ndim (Output_Field_id, Input_Field_ndim);
   Output_Field_dims = (int *)ARRalloc(NULL, DTYPE_INT, Input_Field_ndim, NULL);
   if( !Output_Field_dims )
   {
      fprintf( stderr, "CalcThreshold: Unable to allocate array for Output_Field_dims\n" );
      return( 0 );
   }
   
   Output_Field_dims[0] = Input_Field_dims[0];
   Output_Field_dims[1] = Input_Field_dims[1];
   Output_Field_dims[2] = Input_Field_dims[2];
   FLDset_dims (Output_Field_id, Output_Field_dims);
   ARRfree((char *)Output_Field_dims);
   
   /* Set mesh nspace, Output_Field_nspace can be 1,2 or 3 */
   
   FLDset_nspace( Output_Field_id, Input_Field_nspace );
   
   /* Set mesh extents */
   
   Output_Field_npoints = 2 * Input_Field_nspace;
   Output_Field_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, Output_Field_npoints, NULL);
   for( i = 0; i < Output_Field_npoints; i++ ) Output_Field_points[i] = Input_Field_points[i];
   FLDset_points(Output_Field_id, Output_Field_points, Output_Field_npoints, OM_SET_ARRAY_FREE);
   
   /* Get Input data, calculate threshold and output result */
   
   FLDget_node_data_ncomp (Input_Field_id, &Input_Field_ncomp);
   FLDset_node_data_ncomp (Output_Field_id, Input_Field_ncomp);
   for (Input_Field_comp_count=0; Input_Field_comp_count < Input_Field_ncomp; Input_Field_comp_count++) 
   {
      /* Get veclen */
	  
      FLDget_node_data_veclen (Input_Field_id, Input_Field_comp_count, &Input_Field_veclen);
      FLDset_node_data_veclen (Output_Field_id, Input_Field_comp_count, Input_Field_veclen);
      
      FLDget_node_data (Input_Field_id, Input_Field_comp_count, &Input_Field_data_type, 
         &Input_Field_data, &Input_Field_ndata, OM_GET_ARRAY_RD);
      
      if( Input_Field_veclen != 1 )
      {
         fprintf( stderr, "CalcThreshold: veclen must be 1\n" );
         return( 0 );
      }

      if( Input_Field_data_type != DTYPE_BYTE )
      {
         fprintf( stderr, "CalcThreshold: Can only process byte data\n" );
         return( 0 );
      }
      
      /* Allocate and initialize array to tell which voxels have been processed */
	  
      processed = (char *)malloc( Input_Field_veclen * Input_Field_ndata * sizeof(char) );
      memset( processed, 0, Input_Field_veclen * Input_Field_ndata * sizeof(char) );
      
      /* Allocate and initialize output array */
	  
      Output_Field_data_type = DTYPE_BYTE;
      Output_Field_data = (char *)ARRalloc(NULL, Output_Field_data_type, 
         Input_Field_veclen * Input_Field_ndata, NULL);
      
      memset( Output_Field_data, 0, Input_Field_veclen * Input_Field_ndata * sizeof(char) );
      
      /* Process data, starting with seed point */

      CheckVoxel( seed, Input_Field_dims, (float)min, (float)max, (unsigned char *)Input_Field_data, 
         (unsigned char *)Output_Field_data, processed );

      /* Set output data */
	  
      FLDset_node_data (Output_Field_id, Input_Field_comp_count, Output_Field_data, 
         Output_Field_data_type, Input_Field_ndata * Input_Field_veclen, OM_SET_ARRAY_FREE);
      
      if (Input_Field_data)
         ARRfree((char *)Input_Field_data);
   }
   
   /* Clean up and return */
   
   if (seed != NULL) 
      ARRfree((char *)seed);
   
   if (Input_Field_dims)
      ARRfree((char *)Input_Field_dims);
   
   if (Input_Field_points)
      ARRfree((char *)Input_Field_points);
   
   return(1);
}

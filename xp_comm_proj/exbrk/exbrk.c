#include "gen.h"

typedef struct {
int lastX;
int lastY;
int lastZ;
} ExBrkData;

/* #define DEBUG */

#define CHECKLIMIT(val,limit,flag) ((flag) ? (val > limit) : (val < limit))

/* routine: ExcavateBrickCreate
 * purpose: Constructor for Excavate Brick module
 *          Allocates internal data structure and puts pointer into module
 */
int ExcavateBrickCreate(OMobj_id ExcavateBrick_id, OMevent_mask event_mask, int seq_num)
{
   ExBrkData *exBrkData;

   exBrkData = (ExBrkData *)malloc(sizeof(ExBrkData));
   if( exBrkData == NULL )
     {
       fprintf( stderr, "ExBrkCreate: Unable to allocate memory for local data storage\n" );
       return( 0 );
     }

   if( OMset_name_ptr_val( ExcavateBrick_id, OMstr_to_name("localptr"), exBrkData, 0 ) != OM_STAT_SUCCESS )
     {
       fprintf( stderr, "ExBrkCreate: Unable to save local ptr to object\n" );
       return( 0 );
     }

   return( 1 );
}

/* routine: ExcavateBrickUpdFld
 * purpose: Perform Excavate Brick
 */
int ExcavateBrickUpdFld(OMobj_id ExcavateBrick_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   OMobj_id Input_Field_id;
   int Input_Field_ndim, *Input_Field_dims, Input_Field_nspace, Input_Field_npoints;
   float *Input_Field_points;
   int Input_Field_ncomp, Input_Field_comp_count, Input_Field_veclen;
   int    Input_Field_data_type, Input_Field_ndata;
   char  *Input_Field_data;

   OMobj_id Output_Field_id;
   int Output_Field_ndim, *Output_Field_dims, Output_Field_nspace, Output_Field_npoints;
   float *Output_Field_points;
   int Output_Field_ncomp, Output_Field_veclen;
   int    Output_Field_data_type, Output_Field_ndata;
   char  *Output_Field_data;

   int  X;
   int  Y;
   int  Z;

   int belowX;
   int belowY;
   int belowZ;

   int ok_z;
   int ok_y;

   unsigned char *outptr;
   unsigned char *inptr;
   int image_size;
   int row_size;
   int i, j, k;

   ExBrkData *exBrkData;

#ifdef DEBUG
   printf( "ExcavateBrickUpdFld: Entry\n" );
#endif

   /* Get Object Id for input and output mesh
    */
   Input_Field_id = OMfind_subobj(ExcavateBrick_id, OMstr_to_name("Input_Field"), OM_OBJ_RD);
   Output_Field_id = OMfind_subobj(ExcavateBrick_id, OMstr_to_name("Output_Field"), OM_OBJ_RW);

   /* Get size and extents of input dataset and make sure we can handle it
    */
   FLDget_dims (Input_Field_id, &Input_Field_dims, &Input_Field_ndim);
   FLDget_nspace (Input_Field_id, &Input_Field_nspace);
   FLDget_points (Input_Field_id, &Input_Field_points, &Input_Field_npoints, OM_GET_ARRAY_RD);

   if( Input_Field_nspace != 3 )
     {
       fprintf( stderr, "ExcavateBrickUpdField: Unable to process data for nspace != 3\n" );
       return( 0 );
     }

   if( Input_Field_ndim != 3 )
     {
       fprintf( stderr, "ExcavateBrickUpdField: Unable to process data for ndim != 3\n" );
       return( 0 );
     }

   /* Get X, Y, and Z's value, defaulting based on input dataset
    */ 
   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("X"), &X) != 1) 
      X = Input_Field_dims[0] - 1;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("Y"), &Y) != 1) 
      Y = Input_Field_dims[1] - 1;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("Z"), &Z) != 1) 
      Z = Input_Field_dims[2] - 1;

   /* Get flags for above/below limit
    */
   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowX"), &belowX) != 1) 
      belowX = 0;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowY"), &belowY) != 1) 
      belowY = 0;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowZ"), &belowZ) != 1) 
      belowZ = 0;

   /* Set size of output array, (same as input)
    */
   Output_Field_ndim = Input_Field_ndim;
   FLDset_ndim (Output_Field_id, Output_Field_ndim);

   Output_Field_dims = (int *)ARRalloc(NULL, DTYPE_INT, Output_Field_ndim, NULL);
   Output_Field_dims[0] = Input_Field_dims[0];
   Output_Field_dims[1] = Input_Field_dims[1];
   Output_Field_dims[2] = Input_Field_dims[2];
   FLDset_dims (Output_Field_id, Output_Field_dims);
   if (Output_Field_dims)
     ARRfree((char *)Output_Field_dims);

   Output_Field_nspace = Input_Field_nspace;
   FLDset_nspace(Output_Field_id, Output_Field_nspace);

   /*added by MA YingLiang 10/07/2001*/
   if(Input_Field_points==NULL)
	   return 0;
   /*added by MA YingLiang 10/07/2001*/

   Output_Field_npoints = 2*Output_Field_nspace;
   Output_Field_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, Output_Field_npoints, NULL);
   
   Output_Field_points[0] = Input_Field_points[0];
   Output_Field_points[1] = Input_Field_points[1];
   Output_Field_points[2] = Input_Field_points[2];
   Output_Field_points[3] = Input_Field_points[3];
   Output_Field_points[4] = Input_Field_points[4];
   Output_Field_points[5] = Input_Field_points[5];

   FLDset_points (Output_Field_id, Output_Field_points, Output_Field_npoints, OM_SET_ARRAY_FREE);

   /* Get number of components 
    */
   FLDget_node_data_ncomp (Input_Field_id, &Input_Field_ncomp);

   /* Set number of node data components 
    */
   Output_Field_ncomp = Input_Field_ncomp;
   FLDset_node_data_ncomp (Output_Field_id, Output_Field_ncomp);

   /* For each node data component get veclen, type and data arry itself 
    */
   for (Input_Field_comp_count=0; Input_Field_comp_count < Input_Field_ncomp; Input_Field_comp_count++) 
     {
      FLDget_node_data_veclen (Input_Field_id, Input_Field_comp_count, &Input_Field_veclen);

      Output_Field_veclen = Input_Field_veclen;
      FLDset_node_data_veclen(Output_Field_id, Input_Field_comp_count, Output_Field_veclen);

      FLDget_node_data (Input_Field_id, Input_Field_comp_count, &Input_Field_data_type, 
			&Input_Field_data, &Input_Field_ndata, OM_GET_ARRAY_RD);

      if( Input_Field_data_type != DTYPE_BYTE )
	{
	  fprintf( stderr, "ExcavateBrickUpdField: Unable to process non byte data\n" );
	  return( 0 );
	}

      Output_Field_data_type = DTYPE_BYTE;
      Output_Field_ndata = Input_Field_ndata;
      Output_Field_data = (char *)ARRalloc(NULL, Output_Field_data_type, 
                                           Output_Field_veclen * Output_Field_ndata, NULL);

      memset( Output_Field_data, 0, Output_Field_veclen * Output_Field_ndata * sizeof(char) );

      row_size = Input_Field_dims[0];
      image_size = Input_Field_dims[0] * Input_Field_dims[1];
      for( k = 0; k < Input_Field_dims[2]; k++ )
	{
	  ok_z = CHECKLIMIT( k, Z, belowZ );
	  for( j = 0; j < Input_Field_dims[1]; j++ )
	    {
	      ok_y = CHECKLIMIT( j, Y, belowY );
	      inptr = (unsigned char *)Input_Field_data + (k * image_size) + (j * row_size);
	      outptr = (unsigned char *)Output_Field_data + (k * image_size) + (j * row_size);
	      for( i = 0; i < Input_Field_dims[0]; i++, outptr++, inptr++ )
		{
		  if( ok_z || ok_y || CHECKLIMIT( i, X, belowX ) )
		    *outptr = *inptr;
		}
	    }
	}

      FLDset_node_data(Output_Field_id, Input_Field_comp_count, Output_Field_data, Output_Field_data_type,
		       Output_Field_ndata * Output_Field_veclen, OM_SET_ARRAY_FREE);

      if (Input_Field_data)
         ARRfree((char *)Input_Field_data);
   }

   if (Input_Field_dims)
      ARRfree((char *)Input_Field_dims);

   if (Input_Field_points)
      ARRfree((char *)Input_Field_points);

   if( OMget_name_ptr_val( ExcavateBrick_id, OMstr_to_name("localptr"), (OMptr *)&exBrkData, 0 ) == OM_STAT_SUCCESS )
     {
       exBrkData->lastX = X;
       exBrkData->lastY = Y;
       exBrkData->lastZ = Z;
     }
   else
     fprintf( stderr, "ExcavateBrickUpdateXYZ: Unable to get pointer to local data\n" );

   return(1);
}

/* routine: ExcavateBrickUpdXYZ
 * purpose: Update output of Excavate Brick module based on new x, y, or z
 */
int ExcavateBrickUpdXYZ(OMobj_id ExcavateBrick_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   OMobj_id Input_Field_id;
   int Input_Field_ndim, *Input_Field_dims, Input_Field_nspace, Input_Field_npoints;
   float *Input_Field_points;
   int Input_Field_ncomp, Input_Field_comp_count, Input_Field_veclen;
   int    Input_Field_data_type, Input_Field_ndata;
   char  *Input_Field_data;

   OMobj_id Output_Field_id;
   int Output_Field_ndim, *Output_Field_dims, Output_Field_nspace, Output_Field_npoints;
   float *Output_Field_points;
   int Output_Field_ncomp, Output_Field_veclen;
   int    Output_Field_data_type, Output_Field_ndata;
   char  *Output_Field_data;

   int  X;
   int  Y;
   int  Z;

   int belowX;
   int belowY;
   int belowZ;

   int ok_z;
   int ok_y;

   unsigned char *outptr;
   unsigned char *inptr;
   int image_size;
   int row_size;
   int i, j, k;

   ExBrkData *exBrkData;

#ifdef DEBUG
   printf( "ExcavateBrickUpdXYZ: Entry\n" );
#endif

   /* Get Object Id for input and output mesh
    */
   Input_Field_id = OMfind_subobj(ExcavateBrick_id, OMstr_to_name("Input_Field"), OM_OBJ_RD);
   Output_Field_id = OMfind_subobj(ExcavateBrick_id, OMstr_to_name("Output_Field"), OM_OBJ_RW);

   /* Get size and extents of input dataset and make sure we can handle it
    */
   FLDget_dims (Input_Field_id, &Input_Field_dims, &Input_Field_ndim);
   FLDget_nspace (Input_Field_id, &Input_Field_nspace);
   FLDget_points (Input_Field_id, &Input_Field_points, &Input_Field_npoints, OM_GET_ARRAY_RD);

   if( Input_Field_nspace != 3 )
     {
       fprintf( stderr, "ExcavateBrickUpdField: Unable to process data for nspace != 3\n" );
       return( 0 );
     }

   if( Input_Field_ndim != 3 )
     {
       fprintf( stderr, "ExcavateBrickUpdField: Unable to process data for ndim != 3\n" );
       return( 0 );
     }

   /* Get X, Y, and Z's value, defaulting based on input dataset
    */ 
   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("X"), &X) != 1) 
      X = Input_Field_dims[0] - 1;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("Y"), &Y) != 1) 
      Y = Input_Field_dims[1] - 1;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("Z"), &Z) != 1) 
      Z = Input_Field_dims[2] - 1;

   /* Get flags for above/below limit
    */
   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowX"), &belowX) != 1) 
      belowX = 0;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowY"), &belowY) != 1) 
      belowY = 0;

   if (OMget_name_int_val(ExcavateBrick_id, OMstr_to_name("belowZ"), &belowZ) != 1) 
      belowZ = 0;

   /* Set size of output array, (same as input)
    */
   Output_Field_ndim = Input_Field_ndim;
   FLDset_ndim (Output_Field_id, Output_Field_ndim);

   Output_Field_dims = (int *)ARRalloc(NULL, DTYPE_INT, Output_Field_ndim, NULL);
   Output_Field_dims[0] = Input_Field_dims[0];
   Output_Field_dims[1] = Input_Field_dims[1];
   Output_Field_dims[2] = Input_Field_dims[2];
   FLDset_dims (Output_Field_id, Output_Field_dims);
   if (Output_Field_dims)
     ARRfree((char *)Output_Field_dims);

   Output_Field_nspace = Input_Field_nspace;
   FLDset_nspace(Output_Field_id, Output_Field_nspace);

   Output_Field_npoints = 2*Output_Field_nspace;
   Output_Field_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, Output_Field_npoints, NULL);
   
   /*added by MA YingLiang 10/07/2001*/
   if(Input_Field_points==NULL)
	   return 0;
   /*added by MA YingLiang 10/07/2001*/

   Output_Field_points[0] = Input_Field_points[0];
   Output_Field_points[1] = Input_Field_points[1];
   Output_Field_points[2] = Input_Field_points[2];
   Output_Field_points[3] = Input_Field_points[3];
   Output_Field_points[4] = Input_Field_points[4];
   Output_Field_points[5] = Input_Field_points[5];

   FLDset_points (Output_Field_id, Output_Field_points, Output_Field_npoints, OM_SET_ARRAY_FREE);

   /* Get number of components 
    */
   FLDget_node_data_ncomp (Input_Field_id, &Input_Field_ncomp);

   /* Set number of node data components 
    */
   Output_Field_ncomp = Input_Field_ncomp;
   FLDset_node_data_ncomp (Output_Field_id, Output_Field_ncomp);

   /* For each node data component get veclen, type and data arry itself 
    */
   for (Input_Field_comp_count=0; Input_Field_comp_count < Input_Field_ncomp; Input_Field_comp_count++) 
     {
      FLDget_node_data_veclen (Input_Field_id, Input_Field_comp_count, &Input_Field_veclen);

      Output_Field_veclen = Input_Field_veclen;
      FLDset_node_data_veclen(Output_Field_id, Input_Field_comp_count, Output_Field_veclen);

      FLDget_node_data (Input_Field_id, Input_Field_comp_count, &Input_Field_data_type, 
			&Input_Field_data, &Input_Field_ndata, OM_GET_ARRAY_RD);

      if( Input_Field_data_type != DTYPE_BYTE )
	{
	  fprintf( stderr, "ExcavateBrickUpdField: Unable to process non byte data\n" );
	  return( 0 );
	}

      Output_Field_data_type = DTYPE_BYTE;
      Output_Field_ndata = Input_Field_ndata;
      Output_Field_data = (char *)ARRalloc(NULL, Output_Field_data_type, 
                                 Output_Field_veclen * Output_Field_ndata, NULL);

      memset( Output_Field_data, 0, Output_Field_veclen * Output_Field_ndata * sizeof(char) );

      row_size = Input_Field_dims[0];
      image_size = Input_Field_dims[0] * Input_Field_dims[1];
      for( k = 0; k < Input_Field_dims[2]; k++ )
	{
	  ok_z = CHECKLIMIT( k, Z, belowZ );
	  for( j = 0; j < Input_Field_dims[1]; j++ )
	    {
	      ok_y = CHECKLIMIT( j, Y, belowY );
	      inptr = (unsigned char *)Input_Field_data + (k * image_size) + (j * row_size);
	      outptr = (unsigned char *)Output_Field_data + (k * image_size) + (j * row_size);
	      for( i = 0; i < Input_Field_dims[0]; i++, outptr++, inptr++ )
		{
		  if( ok_z || ok_y || CHECKLIMIT( i, X, belowX ) )
		    *outptr = *inptr;
		}
	    }
	}

      FLDset_node_data(Output_Field_id, Input_Field_comp_count, Output_Field_data, Output_Field_data_type,
		       Output_Field_ndata * Output_Field_veclen, OM_SET_ARRAY_FREE);

      if (Input_Field_data)
         ARRfree((char *)Input_Field_data);
   }

   if (Input_Field_dims)
      ARRfree((char *)Input_Field_dims);

   if (Input_Field_points)
      ARRfree((char *)Input_Field_points);

   if( OMget_name_ptr_val( ExcavateBrick_id, OMstr_to_name("localptr"), (OMptr *)&exBrkData, 0 ) == OM_STAT_SUCCESS )
     {
       exBrkData->lastX = X;
       exBrkData->lastY = Y;
       exBrkData->lastZ = Z;
     }
   else
     fprintf( stderr, "ExcavateBrickUpdateXYZ: Unable to get pointer to local data\n" );

   return(1);
}

/* routine: ExcavateBrickDelete
 * purpose: Destructor for Excavate Brick module
 *          Frees internal structure allocated in ExcavateBrickCreate
 */
int ExcavateBrickDelete(OMobj_id ExcavateBrick_id, OMevent_mask event_mask, int seq_num)
{
   ExBrkData *exBrkData;

#ifdef DEBUG
   printf( "ExcavateBrickDelete: Entry\n" );
#endif

   if( OMget_name_ptr_val( ExcavateBrick_id, OMstr_to_name("localptr"), (OMptr *)&exBrkData, 0 ) == OM_STAT_SUCCESS )
     {
       if( exBrkData )
	 free( exBrkData );
     }
   else
     fprintf( stderr, "ExcavateBrickDelete: Unable to get pointer to local data\n" );

   return( 0 );
}




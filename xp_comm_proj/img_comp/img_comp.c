#include "gen.h"
#include "avs/math.h"
#include "avs/gd.h"

#define ONE_OVER_255 .00392156862745098039

static void copy(unsigned char *dst,
                 unsigned int *a,
                 unsigned int *b,
                 int left,
                 int right,
                 int w,
                 int y
                 )
{
   int y_offset;
   
   y_offset = y * w;
   
   if(w > 0 && (left<0 || right >= w))
   {
      memcpy (dst+y_offset*4, b+y_offset, 4*w);
   }
   else
   {
      if (left > 0)
         memcpy (dst+y_offset*4, a+y_offset, 4*left);
      if (right-left >= 0)
         memcpy (dst+(y_offset+left)*4, b+(y_offset+left),4*(right-left+1));
      if (w-(right+1) > 0)
         memcpy (dst+(y_offset+right+1)*4, a+(y_offset+right+1),4*(w-(right+1)));
   }
}

static void block_copy(
                       unsigned char *dst,
                       unsigned int *src,
                       int start_y,
                       int end_y,
                       int w
                       )
{ 
   int y;
   
   if( w <= 0 )
      return;
   
   for( y = start_y; y < end_y; y++ )
      memcpy( dst+(y*w)*4, src+(y*w), 4*w );
}

static void circle(unsigned char *dst, 
                   unsigned int *a, 
                   unsigned int *b,
                   int ox,
                   int oy,
                   int r,
                   int w,
                   int h
                   )
{
   int x, y, err, prev_x, prev_y;
   
   y = r;
   err = r/2;
   
   prev_x = prev_y = -1;
   
   if(oy-r >= 0)
      block_copy( dst, a, 0, oy-r, w );
   
   if(oy+r < h)
      block_copy( dst, a, oy+r, h, w );

   for( x = 0;x <= y; x++ )
   {   
      if(prev_y != y)
      {
         if(oy-y >= 0)
            copy(dst,a,b,ox-x,ox+x,w,oy-y);
         if(oy+y < h)
            copy(dst,a,b,ox-x,ox+x,w,oy+y);
      }

      if(prev_x != x)
      {
         if(oy-x >= 0)
            copy(dst,a,b,ox-y,ox+y,w,oy-x);
         if(oy+x < h)
            copy(dst,a,b,ox-y,ox+y,w,oy+x);
      }

      prev_x = x;
      prev_y = y;
      if( (err-=x) < 0)
         err += y--;
   }
}

int Image_Compare(OMobj_id Image_Compare_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   unsigned char *in1_data, *in2_data;
   int in1_ndata, in2_ndata;
   int *in1_dims, *in2_dims;
   int in1_ndim, in2_ndim;
   OMobj_id in_id;
   int in_veclen;
   int in_dtype;

   double Value;
   int Switch;
   int Mode;

   int out_dtype, out_ndata;
   unsigned char *out_data;
   OMobj_id out_id;
   
   unsigned int *in_rgb1 = NULL;
   unsigned int *in_rgb2 = NULL;
   OMobj_id Datamap_1_id;
   OMobj_id Datamap_2_id;

   unsigned int *i1,*i2, *src;
   int i, j, nx, ny, np, x, y;
   float a, dx, dy, r;

   unsigned int *tmp, *tmpptr;
   unsigned char *inptr;

   /* Read and validate Image 1
    */
   in_id = OMfind_subobj( Image_Compare_id, OMstr_to_name("In_Data_1"), OM_OBJ_RD );

   FLDget_dims( in_id, &in1_dims, &in1_ndim );
   FLDget_node_data_veclen( in_id, 0, &in_veclen );
   FLDget_node_data( in_id, 0, &in_dtype, &in1_data, &in1_ndata, OM_GET_ARRAY_RD );
   
   if( in1_ndata == 0 )
      return 0;
   
   if( in_dtype != DTYPE_BYTE )
   {
      fprintf( stderr, "Image_Compare only supports byte data...\n" );
      return 0;
   }

   if( in_veclen != 1 )
   {
      fprintf( stderr, "Image_Compare only supports scalar data...\n" );
      return 0;
   }

   if( in1_ndim != 2 )
   {
      fprintf( stderr, "Image_Compare only supports 2D input...\n" );
      return 0;
   }

   /* Read and validate Image 2
    */
   in_id = OMfind_subobj( Image_Compare_id, OMstr_to_name("In_Data_2"), OM_OBJ_RD );

   FLDget_dims( in_id, &in2_dims, &in2_ndim );
   FLDget_node_data_veclen( in_id, 0, &in_veclen );
   FLDget_node_data( in_id, 0, &in_dtype, &in2_data, &in2_ndata, OM_GET_ARRAY_RD );
   
   if( in2_ndata == 0 )
      return 0;

   if( in_dtype != DTYPE_BYTE )
   {
      fprintf( stderr, "Image_Compare only supports byte data...\n" );
      return 0;
   }

   if( in_veclen != 1 )
   {
      fprintf( stderr, "Image_Compare only supports scalar data...\n" );
      return 0;
   }
   
   if( in2_ndim != 2 )
   {
      fprintf( stderr, "Image_Compare only supports 2D input...\n" );
      return 0;
   }

   if( (in2_dims[0] != in1_dims[0]) || (in2_dims[1] != in1_dims[1]) )
   {
      fprintf( stderr, "Image_Compare input images must be the same size...\n" );
      return 0;
   }

   /* Get misc input
    */ 
   if (OMget_name_int_val(Image_Compare_id, OMstr_to_name("Mode"), &Mode) != 1) 
      Mode = 0;
   
   if (OMget_name_real_val(Image_Compare_id, OMstr_to_name("Value"), &Value) != 1) 
      Value = 0;

   if( (Value < 0.0) || (Value > 1.0) )
   {
      fprintf( stderr, "Value should be between 0 and 1 inclusive\n" );
      Value = (Value < 0.0) ? 0.0 : 1.0;
   }
   
   if (OMget_name_int_val(Image_Compare_id, OMstr_to_name("Switch"), &Switch) != 1) 
      Switch = 0;
   
   /* Get datamap ids
    */
   Datamap_1_id = OMfind_subobj( Image_Compare_id, OMstr_to_name("Datamap_1"), OM_OBJ_RD );
   Datamap_2_id = OMfind_subobj( Image_Compare_id, OMstr_to_name("Datamap_2"), OM_OBJ_RD );

   /* Get output data array, (note: most of output will be setup by OM due to V)
    */
   out_id = OMfind_subobj( Image_Compare_id, OMstr_to_name("Out_Data"), OM_OBJ_RW );

   out_dtype = DTYPE_BYTE;
   FLDset_node_data_ncomp( out_id, 1 );
   FLDset_node_data_veclen( out_id, 0, 4 );
   FLDset_node_data_id( out_id, 0, GD_RGB_DATA_ID);
   FLDget_node_data( out_id, 0, &out_dtype, &out_data, &out_ndata, OM_GET_ARRAY_WR );
   
   nx = in1_dims[0];
   ny = in1_dims[1];

   in_rgb1 = (unsigned int *)malloc( nx * ny * sizeof(unsigned int) );
   in_rgb2 = (unsigned int *)malloc( nx * ny * sizeof(unsigned int) ); 
   
   tmp = (unsigned int *)malloc( nx * ny * sizeof(unsigned int) );

   for( i = 0, tmpptr = tmp, inptr = in1_data; i < nx * ny; i++ ) 
      *tmpptr++ = (unsigned int)*inptr++;

   GDgen_argbs( Datamap_1_id, (unsigned char *)tmp, DTYPE_INT, in_rgb1, nx, ny, nx, nx, NULL );

   for( i = 0, tmpptr = tmp, inptr = in2_data; i < nx * ny; i++ ) 
      *tmpptr++ = (unsigned int)*inptr++;

   GDgen_argbs( Datamap_2_id, (unsigned char *)tmp, DTYPE_INT, in_rgb2, nx, ny, nx, nx, NULL );

   free( (char *)tmp );

   /* Switch input datasets if requested
    */
   if( Switch != 0 )
   {
      i1 = in_rgb1;
      i2 = in_rgb2;
   }
   else
   {
      i1 = in_rgb2;
      i2 = in_rgb1;
   }
   
   /* Merge images
    */
   switch( Mode )
   {
   case 0:         /* Vertical slice */
      np = (int)(Value * nx);
      if( np > nx ) np = nx;

      for( i = 0; i < ny; i++ ) 
      {
         memcpy( &(out_data[i * nx * 4]), &(i1[i * nx]), 4 * np );
         if( np < nx )
            memcpy( &(out_data[(i*nx+np) * 4]), &(i2[i * nx + np]), 4 * (nx - np) );
      }
      break;

   case 1: /* Horizontal slice */
      np = (int)(Value * ny);
      if( np > ny ) np = ny;

      memcpy( out_data, i1, (int)(4 * nx * np ) );
      if( np < ny )
         memcpy( &(out_data[nx * np * 4]), &(i2[nx * np]), 4 * nx * (ny-np) );
      break;

   case 2:   /* diagonal */
      if( (Value <= 0.0) || (Value >= 0.999) ) 
      {
         memcpy( out_data, i1, 4 * sizeof(char) * nx * ny );
      }
      else 
      {
         a = (float)tan(M_PI * Value / 2.0);
         for( i = 0; i < ny; i++ ) 
         {
            np = (ny == 0) ? i : (int)((float)i*a*nx/(float)ny);
            if (np > nx) np = nx;

            memcpy( &(out_data[i*nx*4]), &(i1[i*nx]), 4 * np );
            memcpy( &(out_data[(i*nx+np)*4]), &(i2[(i*nx+np)]), 4 * (nx-np) );
         }
      }
      break;

   case 3:  /* solid */
      memcpy( out_data, i1, 4 * nx * ny );
      break;

   case 4:  /* circle */
      dx = (float)(nx / 2.0);
      dy = (float)(ny / 2.0);
      r = (float)(sqrt(dx*dx+dy*dy) * Value);
      circle( out_data, i1, i2, (int)dx, (int)dy, (int)r, nx, ny );
      break;

   case 5:  /* checker */
      x = (int)(nx * Value);
      if ( x <= 1 ) x = 1;

      y = nx / x;
      for(i=0; i<ny; i++)
      {
         src = (((i/x)%2) == 0) ? i1 : i2;
         for(j=0; j<y; j++)
         {
            memcpy(&(out_data[((i*nx) + (j*x)) * 4]),&(src[((i*nx) + (j*x))]), x * 4 );
            src = (src == i1) ? i2 : i1;
         }
         if (nx > (j*x))
            memcpy(&(out_data[((i*nx) + (j*x)) * 4]),&(src[((i*nx) + (j*x))]), (nx-(j*x)) * 4 );
      }
      break;

   default:
      fprintf( stderr, "Image_Compare: Unsupported mode %d\n", Mode );
      break;
   }

   free( in_rgb1 );
   free( in_rgb2 );
   
   if( in1_dims )
      ARRfree( (char *)in1_dims );
   
   if( in1_data )
      ARRfree( (char *)in1_data );
   
      if( in2_dims )
      ARRfree( (char *)in2_dims );
   
   if( in2_data )
      ARRfree( (char *)in2_data );
   
   if( out_data )
      ARRfree( (char *)out_data );
   
   return(1);
}

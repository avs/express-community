//
// Dmap2Image Module
//
//  generates a 2D image based on colors based on Datamap
//
//  mode = 0:  fill simple 2D image with constant scan lines
//             showing datamap, ARGB.
//
//  mode = 1;  more complex image structured for legend
//
//  Author: Ian Curington, Advanced Visual Systems, Inc.
//  Date:   15 December 2000

#include "gen.hxx"

#include <avs/gd.h>
#include <avs/dmap.h>
#include <avs/pal.h>

   /***********************/
   /* Utility Functions   */
   /***********************/

#define CHECKERBOARD_WIDTH 32

/****
 Process Scan Line 
***/

void scanline_loop (
        unsigned char lut[],       // initial input scan line array
        unsigned char scanline[],  // output scan line array
        int line,                  // which line are we on
        int nx,                    // how long is the line
        int ny,                    // how many lines in image
        int mode )                 // mode control, controls different content
{
	int j;
        float alpha;
        unsigned char checkerboard, r, g, b;

        if ( mode == 1 && line < ny/2 ) // first half of image full alpha
        {
		for (j=0; j<nx*4; j+=4)
		{
		  checkerboard = (unsigned char)(((j/CHECKERBOARD_WIDTH) & 0x01)*255);
		  alpha = (lut[j + AVS_ALPHA_BYTE])/255.0;
		  scanline[j + AVS_ALPHA_BYTE] = lut[j + AVS_ALPHA_BYTE];
		  r = lut[j + AVS_RED_BYTE];
		  g = lut[j + AVS_GREEN_BYTE];
		  b = lut[j + AVS_BLUE_BYTE];
		  scanline[j + AVS_RED_BYTE]   = r * alpha + (1.0-alpha) * checkerboard;
		  scanline[j + AVS_GREEN_BYTE] = g * alpha + (1.0-alpha) * checkerboard;
		  scanline[j + AVS_BLUE_BYTE]  = b * alpha + (1.0-alpha) * checkerboard;
		}
	}
        else               // copy across transfer function
	{
		for (j=0; j<nx*4; j+=4)
		{
		  scanline[j + AVS_ALPHA_BYTE] = lut[j + AVS_ALPHA_BYTE];
		  scanline[j + AVS_RED_BYTE]   = lut[j + AVS_RED_BYTE];
		  scanline[j + AVS_GREEN_BYTE] = lut[j + AVS_GREEN_BYTE];
		  scanline[j + AVS_BLUE_BYTE]  = lut[j + AVS_BLUE_BYTE];
		}
	}

} // end of function "scanline_loop".

// utility to resolve dmap
// variation on internal utility in GD kit to resolve datamaps, modified here

void Dmap2Colors(OMobj_id id,          // Dmap
                int size,             // size of color array
                unsigned int *colors) // color array
{
    OMobj_id ptr_id;
    Dmap *map;
    int i;
    double minimum, maximum;
    float delta, val, *values;

    ptr_id = OMfind_subobj(id, OMstr_to_name("localPtr"), OM_OBJ_RW);
    if (!OMis_null_obj(ptr_id)) {
       if (OMget_ptr_val(ptr_id, (void **)&map, 0)) {
	  map->get_minmax(map, &minimum, &maximum);
	  delta = (maximum - minimum) / size;
	  /* Position the value right in the middle of the bin. */
	  val = minimum + (delta / 2);

          values = (float *) malloc (sizeof(float) * size);
	  for (i=0; i<size; i++) {
	     values[i] = val;
	     val += delta;
	  }
          map->get_argbs(map, (char *)values, colors, OM_TYPE_FLOAT,
	  	size, 1, size, size, NULL, 0);

          free(values);
       }
    }
    return;
} // end of Dmap2Colors


   /******************
    *  ModuleCode    *
    ******************/

int
Dmap2Image_Dmap2ImageMods_Dmap2ImageCore::update(OMevent_mask event_mask, int seq_num)
{
  
   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr = NULL;

   // dmap (DatamapTempl read notify)

   // output_dims (OMXint_array write)
   int output_dims_size;
   int *output_dims_arr = NULL;

   int nx, ny, mode;
   int dims[2], i, j, k, NX, NY;

   unsigned char *lut = NULL;
   int status;

   /******************
    *  Start Code    *
    ******************/

   nx = X_Resolution;
   ny = Y_Resolution;
   mode = Mode;
 
   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: update\n");

   // use parameters to set output image size
   dims[0] = NX = nx;
   dims[1] = NY = ny;

   // must set output dims first to dimension data arrays
   output_dims_arr = (int *)output_dims.ret_array_ptr(OM_GET_ARRAY_WR,&output_dims_size);
   if ((output_dims_arr==NULL) || (output_dims_size!=2)) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get output_dims_arr.\n");

      ARRfree(output_dims_arr);
      return(0);
   }

   // establish output dimensions
   output_dims_arr[0] = dims[0];
   output_dims_arr[1] = dims[1];

   // freeing this array triggers Express to allocate memory for image
   ARRfree(output_dims_arr);

   // set output dims arr
   // then get a pointer to the output array

   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if ((output_arr==NULL) || (output_size!=(dims[0]*dims[1]*4)) ) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get output_arr.\n");

      ARRfree(output_arr);
      return (0);
   }

   // pre-calculate first scan line look-up-table to save time

   lut = (unsigned char *) malloc (sizeof(unsigned char) * nx*4);

   Dmap2Colors( dmap.obj_id(), nx, (unsigned int *)lut);

   // loop over each scan-line of output image
   for ( i=0; i<NY; i++)
   {
           scanline_loop(lut, &(output_arr[i*nx*4]), i, NX, NY, mode);
   }

   // close and release the output image for further processing
   ARRfree(output_arr);
   free (lut);

   // return 1 for success
   return(1);
}
// end of file


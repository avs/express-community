//
// Sabreen's Antialias Module
//   Reduces an image by 1/2 in each direction, and performs a
//   3x3 lowpass filter to construct the new image.
//   Assumes ARGB 4-byte images on input and output
//

#include "xp_comm_proj/a_alias/gen.h"

//
// uncomment the following line to get diagnostic info
//
// #define TRACE_IT 1

#define PRINT(a)  ERRverror("",ERR_NO_HEADER | ERR_PRINT,(a));

int antialias_routine (unsigned char *ip, int iw, int ih, unsigned char *op, int ow, int oh);


int
AntiAlias_AntiAliasMods_AntiAliasCore::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXbyte_array read req notify)
   int input_size;
   unsigned char *input_arr;

   // input_dims (OMXbyte_array read req notify)
   int input_dims_size;
   unsigned int *input_dims_arr;

   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr;

   int out_dims[2];
   

   /***********************/
   /* Function's Body     */
   /***********************/

   input_arr = (unsigned char *)input.ret_array_ptr(OM_GET_ARRAY_RD,&input_size);
   if (!input_arr) 
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Couldn't get input_arr\n");
      ARRfree(input_arr);
      return(0);
   } // endif.

   input_dims_arr = (unsigned int *)input_dims.ret_array_ptr(OM_GET_ARRAY_RD,&input_dims_size);
   if (!input_dims_arr) 
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Couldn't get input_dims_arr\n");
      ARRfree(input_dims_arr);
      ARRfree(input_arr);
      return(0);
   } // endif.

   // compute output dimensions for new image, 50% of input
   out_dims[0] = input_dims_arr[0] / 2;
   out_dims[1] = input_dims_arr[1] / 2;

   outx = out_dims[0]; // assign global size to V array (allocates mem)
   outy = out_dims[1];

#ifdef TRACE_IT
        printf (" input size = %d %d\n",input_dims_arr[0],input_dims_arr[1]);
        printf (" output size = %d %d\n",out_dims[0],out_dims[1]);
#endif

   //
   // get handle for output image storage area
   //
   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (!output_arr) 
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Couldn't get output_arr\n");
      ARRfree(output_arr);
      ARRfree(input_dims_arr);
      ARRfree(input_arr);
      return(0);
   } // endif.


   // if input image is has an even dimension, copy it into a
   //  padded-out temporarary area. If odd, it's ok, we can just
   //  reference it as-is.

           if ((input_dims_arr[0] & 1) == 0 || (input_dims_arr[1] & 1) == 0)
           {
                   unsigned char *temp, *from, *to;
                   int old_w, old_h, temp_w, temp_h;
                   int y;

#ifdef TRACE_IT
        printf (" using temp storage\n" );
#endif
                   old_w = input_dims_arr[0];
                   temp_w = old_w | 1;        // or-in the LSB to make it odd

                   old_h = input_dims_arr[1];
                   temp_h = old_h | 1;        // or-in the LSB to make it odd

                   // allocate the padded storage area
                   temp = (unsigned char*) malloc (temp_w * temp_h * 4 * sizeof(unsigned char));

                   if (temp_w != old_w)
                   {
                           for (y=0;y<old_h;y++)
                           {
                                   from = input_arr + y * old_w * 4; 
                                   to = temp + y * temp_w * 4; 
                                   memcpy (to,from,old_w * 4);
                                   from += (old_w - 1) * 4;
                                   to += (temp_w - 1) * 4;
                                   memcpy (to,from,4);
                           }
                   }
                   else
                   {
                           memcpy (temp,input_arr,old_w * old_h * 4);
                   } // end else.
                   if (temp_h != old_h)
                   {
                           from = temp + (temp_h - 2) * temp_w * 4;
                           to = temp + (temp_h - 1) * temp_w * 4;
                           memcpy (to,from,temp_w * 4);
                   } // end if.

#ifdef TRACE_IT
                   printf ("Using padded copy of input image.\n");
#endif

                   antialias_routine( temp,
                                      temp_w,
                                      temp_h,
                                      output_arr,
                                      out_dims[0],
                                      out_dims[1] );

                   free (temp);
           }
           else
           {
//#ifdef TRACE_IT
                   printf ("Using original input image.\n");
//#endif
                   antialias_routine( input_arr,
                                      input_dims_arr[0],
                                      input_dims_arr[1],
                                      output_arr,
                                      out_dims[0],
                                      out_dims[1] );
           }

       // return the arrays to the OM
       ARRfree(input_arr);
       ARRfree(input_dims_arr);
       ARRfree(output_arr);

       return(1);

}



//
// Operational Routines
//

// define local data structures

   typedef struct
   {
           int r;
           int g;
           int b;
           int a;
   }
   COLORSEP;

   typedef unsigned long COLOR;
   
#define IR(c) (((c)>>16)&0xff)
#define IG(c) (((c)>>8)&0xff)
#define IB(c) (((c)>>0)&0xff)
#define RRAS(IP, X, Y, SX) ((IP + (SX * Y) + X))
#define IRGB(r,g,b) (COLOR)(((r & 0xff)<<16)|((g & 0xff)<<8)|(b & 0xff))
#define ALPHA 0
#define RED 1
#define GREEN 2
#define BLUE 3

/*------------------------------------------------------------------------
 *
 * antialias_routine(ip, iw, ih, op, ow, oh)
 * unsigned char *ip;
 * int iw, ih;
 * unsigned char *op;
 * int ow, oh;
 *
 * Produce a new image approximately half the size of the original
 * by averaging groups of adjacent pixels.
 *
 * Inputs:	ip	- Input image, 4 bytes per pixel in usual ARGB order.
 *		iw, ih	- Input width and height.
 *		ow, oh	- Output width and height.
 *
 * Outputs:	op	- New image is stored here.
 *
 * Description:	Each output pixel is obtained by averaging a 3x3 group of 
 *		pixels with the following weights:
 *
 *			1/16	2/16	1/16
 *			2/16	4/16	2/16
 *			1/16	2/16	1/16
 *
 *		These 3x3 groups of input pixels overlap by one.  i.e.
 *
 *		Input column:	0  1  2  3  4  5  6  7  8
 *				 \ | / \ | / \ | / \ | /
 *				  \|/   \|/   \|/   \|/
 *		Output column:	   0     1     2     3
 *
 *		For an input width of N, the output width is (N-1)/2.
 *
 *------------------------------------------------------------------------*/

int
antialias_routine (unsigned char *ip, int iw, int ih, unsigned char *op, int ow, int oh)
{
    unsigned char *y0, *y1, *y2;
    COLORSEP   sum0, sum1, sum2;
    int        x, y;

    for (y=0; y < oh; y++) {   // scan line loop

         /* Set pointers to pixels on left edge of 3x3 group. */
         y0 = ip + (2 * y * iw) * 4;
         y1 = y0 + iw * 4;
         y2 = y1 + iw * 4;

         /* Compute first column - 1,2,1 weighting */
         sum2.a = y0[AVS_ALPHA_BYTE] + 
                  y1[AVS_ALPHA_BYTE] * 2 + 
                  y2[AVS_ALPHA_BYTE];
         sum2.r = y0[AVS_RED_BYTE] + 
                  y1[AVS_RED_BYTE] * 2 + 
                  y2[AVS_RED_BYTE];
         sum2.g = y0[AVS_GREEN_BYTE] + 
                  y1[AVS_GREEN_BYTE] * 2 + 
                  y2[AVS_GREEN_BYTE];
         sum2.b = y0[AVS_BLUE_BYTE] + 
                  y1[AVS_BLUE_BYTE] * 2 + 
                  y2[AVS_BLUE_BYTE];
         y0 += 4;
         y1 += 4;
         y2 += 4;

         /* Process blocks of 3x3 pixels, loop over each pixel of output line */
         for (x = 0; x < ow; x++) {

               /* Column 0 - same as last column 2 */
               sum0 = sum2;

               /* Column 1 - 2,4,2 weighting */
               sum1.a = y0[AVS_ALPHA_BYTE] * 2 + 
                        y1[AVS_ALPHA_BYTE] * 4 + 
                        y2[AVS_ALPHA_BYTE] * 2;
               sum1.r = y0[AVS_RED_BYTE] * 2 + 
                        y1[AVS_RED_BYTE] * 4 + 
                        y2[AVS_RED_BYTE] * 2;
               sum1.g = y0[AVS_GREEN_BYTE] * 2 + 
                        y1[AVS_GREEN_BYTE] * 4 + 
                        y2[AVS_GREEN_BYTE] * 2;
               sum1.b = y0[AVS_BLUE_BYTE] * 2 + 
                        y1[AVS_BLUE_BYTE] * 4 + 
                        y2[AVS_BLUE_BYTE] * 2;
               y0 += 4;
               y1 += 4;
               y2 += 4;

               /* Column 2 - 1,2,1 weighting */
               sum2.r = y0[AVS_RED_BYTE] + 
                        y1[AVS_RED_BYTE] * 2 + 
                        y2[AVS_RED_BYTE];
               sum2.g = y0[AVS_GREEN_BYTE] + 
                        y1[AVS_GREEN_BYTE] * 2 + 
                        y2[AVS_GREEN_BYTE];
               sum2.b = y0[AVS_BLUE_BYTE] + 
                        y1[AVS_BLUE_BYTE] * 2 + 
                        y2[AVS_BLUE_BYTE];
               y0 += 4;
               y1 += 4;
               y2 += 4;

               /* Compute the weighted average */

               op[AVS_ALPHA_BYTE] = (sum0.a + sum1.a + sum2.a) >> 4;
               op[AVS_RED_BYTE] = (sum0.r + sum1.r + sum2.r) >> 4;
               op[AVS_GREEN_BYTE] = (sum0.g + sum1.g + sum2.g) >> 4;
               op[AVS_BLUE_BYTE] = (sum0.b + sum1.b + sum2.b) >> 4;

               op += 4;
         }
    }

    return(1);

}


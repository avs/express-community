//
// Sabreen's Composite Module
//

#define XP_WIDE_API
#include "xp_comm_proj/comp/gen.h"

/* Backward compatibility definitions */
#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif

#define FREE_ARRAYS \
   if (input_foreground_arr != NULL) ARRfree(input_foreground_arr);\
   if (input_background_arr != NULL) ARRfree(input_background_arr);\
   if (output_arr != NULL) ARRfree(output_arr);


#define ONE_OVER_255 .00392156862745098039


int
Composite_CompositeMods_CompositeCore::update(OMevent_mask event_mask, int seq_num)
{
   // input_foreground (OMXbyte_array read req notify)
   xp_long input_foreground_size;
   xp_byte *input_foreground_arr = NULL;

   // input_background (OMXbyte_array read req notify)
   xp_long input_background_size;
   xp_byte *input_background_arr = NULL;

   // output (OMXbyte_array write)
   xp_long output_size;
   xp_byte *output_arr = NULL;

   xp_long i;
   float alpha, minus_alpha;

   /***********************/
   /* Function's Body     */
   /***********************/

   // Get and Check Foreground Image array
   //
   input_foreground_arr = (xp_byte *)input_foreground.ret_array_ptr(OM_GET_ARRAY_RD, &input_foreground_size);
   if (input_foreground_arr == NULL) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                "Can't get input_foreground_arr");
      FREE_ARRAYS;
      return (0);
   }

   if (input_foreground_size <= 0) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                "input_foreground_arr is of zero size.");
      FREE_ARRAYS;
      return (0);
   }

   // Get and Check Background Image array
   //
   input_background_arr = (xp_byte *)input_background.ret_array_ptr(OM_GET_ARRAY_RD, &input_background_size);
   if (input_background_arr == NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "Can't get input_background_arr");
      FREE_ARRAYS;
      return (0);
   }

   if (input_background_size <= 0) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "input_background_arr is of zero size.");
      FREE_ARRAYS;
      return (0);
   }

   // Ensure foreground and background arrays have same dimensions
   if (input_foreground_size != input_background_size) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                "Images have different dimensions - they must be the same");
      FREE_ARRAYS;
      return (0);
   }


   // set the output size to be the same as the primary input
   // The output array is defined as output[out_size][4] in V:
   out_size = input_foreground_size/4;

   // Get and Check Output array
   //
   output_arr = (xp_byte *)output.ret_array_ptr(OM_GET_ARRAY_WR, &output_size);
   if (output_arr== NULL) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT, "Can't get output_arr");
      FREE_ARRAYS;
      return (0);
   }

   if (output_size != input_foreground_size) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                "Output array is of invalid size.");
      FREE_ARRAYS;
      return (0);
   }


    // Composite images together.
    // use simple blending scheme i.e. out_col = fore_col*a + back_col*(1-a)
    //
   for (i=0; i<input_foreground_size; i+=4) {
      alpha = (float)input_foreground_arr[i+AVS_ALPHA_BYTE] * ONE_OVER_255;
      minus_alpha = 1.0 - alpha;
      output_arr[i+AVS_ALPHA_BYTE] = input_foreground_arr[i+AVS_ALPHA_BYTE];

      output_arr[i+AVS_RED_BYTE]   =
        (xp_byte)( ((float)input_foreground_arr[i+AVS_RED_BYTE] * alpha) + 
                   ((float)input_background_arr[i+AVS_RED_BYTE] * minus_alpha) );

      output_arr[i+AVS_GREEN_BYTE] =
        (xp_byte)( ((float)input_foreground_arr[i+AVS_GREEN_BYTE] * alpha) + 
                   ((float)input_background_arr[i+AVS_GREEN_BYTE] * minus_alpha) );

      output_arr[i+AVS_BLUE_BYTE]  =
        (xp_byte)( ((float)input_foreground_arr[i+AVS_BLUE_BYTE] * alpha) + 
                   ((float)input_background_arr[i+AVS_BLUE_BYTE] * minus_alpha));
   }

   FREE_ARRAYS;

   // return 1 for success
   return(1);
}


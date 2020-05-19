//
// Sabreen's Replace Alpha Module
//
//    Input RGB image        Input scalar Alpha image
//           |                |
//           ----          ----
//               |         |
//             Replace_Alpha
//                   |
//                new image

#include "xp_comm_proj/repalpha/gen.h"

#define FREE_ARRAYS \
   if (input_image_arr!=NULL) ARRfree(input_image_arr);\
   if (input_alpha_arr!=NULL) ARRfree(input_alpha_arr);\
   if (output_arr!=NULL) ARRfree(output_arr);



int
ReplaceAlpha_ReplaceAlphaMods_ReplaceAlphaCore::update(OMevent_mask event_mask,int seq_num)
{
   // input_image (OMXbyte_array read req notify)
   int input_image_size;
   unsigned char *input_image_arr = NULL;

   // input_alpha (OMXbyte_array read req notify)
   int input_alpha_size;
   unsigned char *input_alpha_arr = NULL;

   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr = NULL;

   int i;


   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,
   //   "I'm in method: ReplaceAlphaCore::update\n");


   // Get and Check Image array
   //==========================
   input_image_arr =
         (unsigned char *)input_image.ret_array_ptr(OM_GET_ARRAY_RD,&input_image_size);

   if (input_image_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get input_image_arr.\n");
      FREE_ARRAYS;
      return (0);
   }

   if (input_image_size <= 0) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Input Image array is of zero size.\n");
      FREE_ARRAYS;
      return (0);
   }

   
   // Get and Check Alpha array
   //==========================
   input_alpha_arr =
        (unsigned char *)input_alpha.ret_array_ptr(OM_GET_ARRAY_RD,&input_alpha_size);

   if (input_alpha_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get input_alpha_arr.\n");
      FREE_ARRAYS;
      return (0);
   }

   if (input_alpha_size <= 0) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Input Alpha array is of zero size.\n");
      FREE_ARRAYS;
      return (0);
   }

   
   // Ensure image and alpha arrays have same dimensions
   if (input_image_size != input_alpha_size * 4){
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Images are different dimensions - they must be the same");
      FREE_ARRAYS;
      return (0);
   }




   // set the output size to be the same as the primary input
   // The output array is defined as output[out_size][4] in V:
   //out_size = input_image_size / 4;
   out_size = input_alpha_size;

   // Get and Check Output array
   //===========================
   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (output_arr==NULL) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get output_arr.\n");
      FREE_ARRAYS;
      return (0);
   }

   if (output_size != input_image_size) {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Output array is of invalid size.\n");
      FREE_ARRAYS;
      return (0);
   }


   // Pixels are arrays of 4 bytes, not integers.
   // However, on 32 bit machines we can speed this up
   // by casting pixels to unsigned ints.
   //=================================================
   if (sizeof(int) == 4) {
      unsigned int* dst = (unsigned int*)output_arr;
      unsigned int* rgb = (unsigned int*)input_image_arr;
      unsigned int mask;

      mask = AVS_RED_MASK | AVS_GREEN_MASK | AVS_BLUE_MASK;

      for (i=0; i<input_alpha_size; i++) {
         dst[i] = (rgb[i] & mask) | (input_alpha_arr[i]<<AVS_ALPHA_SHIFT);
      }
   }
   else
   {
      for (i=0; i<input_alpha_size; i++) {
         output_arr[i*4+AVS_ALPHA_BYTE] = input_alpha_arr[i]; 
         output_arr[i*4+AVS_RED_BYTE]   = input_image_arr[i*4+AVS_RED_BYTE]; 
         output_arr[i*4+AVS_GREEN_BYTE] = input_image_arr[i*4+AVS_GREEN_BYTE]; 
         output_arr[i*4+AVS_BLUE_BYTE]  = input_image_arr[i*4+AVS_BLUE_BYTE]; 
      }
   }

   FREE_ARRAYS;

   // return 1 for success
   return(1);
}


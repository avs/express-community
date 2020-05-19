//
// Sabreen's Luminance Module
//
//    Computes the scalar luminance from a full-colour image (ARGB)
//    14th February 2000
//

#include "xp_comm_proj/lumi/gen.hxx"


/***********************/
/* Module Entry Point  */
/***********************/

int
Luminance_LuminanceMods_LuminanceCore::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXbyte_array read req notify)
   int input_size;
   unsigned char *input_arr;

   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr;

   int i, j;

   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,
   //     "I'm in method: Luminance_LuminanceMods_LuminanceCore::update\n");

   input_arr = (unsigned char *)input.ret_array_ptr(OM_GET_ARRAY_RD,&input_size);
   if (!input_arr) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Couldn't get input_arr\n");

      ARRfree(input_arr);
      return (0);
   }

   //
   // dimension and allocate the output array (global)
   //
   out_size = input_size / 4;

   //
   // grab a pointer to the new output array
   //
   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (!output_arr) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Couldn't get output_arr\n");

      ARRfree(output_arr);
      ARRfree(input_arr);
      return (0);
   }

   
   for (i=0, j=0; j<out_size; i+=4, j++) {
	   output_arr[j] = (unsigned char)
	    (((float)input_arr[i+AVS_RED_BYTE]*0.299) +
	     ((float)input_arr[i+AVS_GREEN_BYTE]*0.587) +
	     ((float)input_arr[i+AVS_BLUE_BYTE]*0.114));
   }

   ARRfree(output_arr);
   ARRfree(input_arr);

   // return 1 for success
   return(1);
}


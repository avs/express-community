//
// ImageFilter Module
//
//    Peforms simple mathematics functions on full-colour images (ARGB)
//

#define XP_WIDE_API

#include "xp_comm_proj/i_filter/gen.hxx"

#define FILTER_OP_ADD 0
#define FILTER_OP_SUBTRACT 1
#define FILTER_OP_MULTIPLY 2
#define FILTER_OP_DIVIDE 3


#define CLAMP_VAL(VAL) {\
   if (tempVal < 0) VAL=0;\
   else if (tempVal > 255) VAL=255;\
   else VAL=(unsigned char)tempVal;\
}

/***********************/
/* Module Entry Point  */
/***********************/

int
ImageFilter_ImageFilterMods_ImageFilterCore::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXbyte_array read req notify)
   xp_long input_size;
   unsigned char *input_arr;

   // output (OMXbyte_array write)
   xp_long output_size;
   unsigned char *output_arr;

   // Local copies of filtering variables
   float alphaL, redL, greenL, blueL;

   xp_long i, j;
   int tempVal;


   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,
   //     "I'm in method: ImageFilter_ImageFilterMods_ImageFilterCore::update\n");

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

   alphaL = params.alphaConst;
   redL = params.redConst;
   greenL = params.greenConst;
   blueL = params.blueConst;

   switch (params.filterOp)
   {
      case FILTER_OP_ADD:
         for (i=0; i<input_size; i+=4) {
            tempVal = (int)((float)input_arr[i+AVS_ALPHA_BYTE] + alphaL);
            CLAMP_VAL(output_arr[i+AVS_ALPHA_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_RED_BYTE] + redL);
            CLAMP_VAL(output_arr[i+AVS_RED_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_GREEN_BYTE] + greenL);
            CLAMP_VAL(output_arr[i+AVS_GREEN_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_BLUE_BYTE] + blueL);
            CLAMP_VAL(output_arr[i+AVS_BLUE_BYTE]);
         }
         break;


      case FILTER_OP_SUBTRACT:
         for (i=0; i<input_size; i+=4) {
            tempVal = (int)((float)input_arr[i+AVS_ALPHA_BYTE] - alphaL);
            CLAMP_VAL(output_arr[i+AVS_ALPHA_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_RED_BYTE] - redL);
            CLAMP_VAL(output_arr[i+AVS_RED_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_GREEN_BYTE] - greenL);
            CLAMP_VAL(output_arr[i+AVS_GREEN_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_BLUE_BYTE] - blueL);
            CLAMP_VAL(output_arr[i+AVS_BLUE_BYTE]);
         }
         break;


      case FILTER_OP_MULTIPLY:
         for (i=0; i<input_size; i+=4) {
            tempVal = (int)((float)input_arr[i+AVS_ALPHA_BYTE] * alphaL);
            CLAMP_VAL(output_arr[i+AVS_ALPHA_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_RED_BYTE] * redL);
            CLAMP_VAL(output_arr[i+AVS_RED_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_GREEN_BYTE] * greenL);
            CLAMP_VAL(output_arr[i+AVS_GREEN_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_BLUE_BYTE] * blueL);
            CLAMP_VAL(output_arr[i+AVS_BLUE_BYTE]);
         }
         break;


      case FILTER_OP_DIVIDE:
         if ((alphaL==0) || (redL==0) || (greenL==0) || (blueL==0)) {
      	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Cannot divide by zero\n");
      	   alphaL = 1.0; redL = 1.0; greenL = 1.0; blueL=1.0;
         }

         for (i=0; i<input_size; i+=4) {
            tempVal = (int)((float)input_arr[i+AVS_ALPHA_BYTE] / alphaL);
            CLAMP_VAL(output_arr[i+AVS_ALPHA_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_RED_BYTE] / redL);
            CLAMP_VAL(output_arr[i+AVS_RED_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_GREEN_BYTE] / greenL);
            CLAMP_VAL(output_arr[i+AVS_GREEN_BYTE]);
            tempVal = (int)((float)input_arr[i+AVS_BLUE_BYTE] / blueL);
            CLAMP_VAL(output_arr[i+AVS_BLUE_BYTE]);
         }
         break;

      default:
   	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Filter opeartion is not defined\n");
         for (i=0; i<input_size; i+=4) {
            output_arr[i+AVS_ALPHA_BYTE] = input_arr[i+AVS_ALPHA_BYTE];
            output_arr[i+AVS_RED_BYTE]   = input_arr[i+AVS_RED_BYTE];
            output_arr[i+AVS_GREEN_BYTE] = input_arr[i+AVS_GREEN_BYTE];
            output_arr[i+AVS_BLUE_BYTE]  = input_arr[i+AVS_BLUE_BYTE];
         }

   }


   ARRfree(output_arr);
   ARRfree(input_arr);

   // return 1 for success
   return(1);
}


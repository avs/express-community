//
// Texture Wave Module
//   Ian Curington
//   18 February 2000
//

#include "gen.hxx"
#include <math.h>


//#define MAX(a,b) (((a)>(b))?(a):(b))
//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define ONE_OVER_255 0.003921568


/***********************/
/* Module Entry Point  */
/***********************/

int
TextureWave_TextureWaveMods_TextureWaveCore::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXbyte_array read req notify)
   int input_size;
   unsigned char *input_arr = NULL;

   int output_size;
   unsigned char *output_arr = NULL;

   int i, j;
   float freq_val;
   float phase_val;
   float minamp_val;
   float maxamp_val;
   float contrast_val;
   int   wave_mode_val;
   int temp_size;
   int offset,nx,ny;
   unsigned char *ftable, *contable;
   float x,aaa,t,range,u;


   /***********************/
   /* Function's Body     */
   /***********************/

   if ((wave_mode<0) || (wave_mode>4)) 
   {
        ERRverror("", ERR_NO_HEADER | ERR_PRINT, "wave_mode out of range 0 to 5\n");
        out_x = 0;
        out_y = 0;
        return(0);
   }

   input_arr = (unsigned char *)input.ret_array_ptr(OM_GET_ARRAY_RD,&input_size);
   if (input_arr==NULL) 
   {
        ERRverror("", ERR_NO_HEADER | ERR_PRINT,"Couldn't get input_arr\n");
        ARRfree(input_arr);
        out_x = 0;
        out_y = 0;
        return(0);
   }


   // check input sizes in case of errors
   temp_size = in_x * in_y * 4;
   if ( temp_size != input_size )
   {
        ERRverror("", ERR_NO_HEADER | ERR_PRINT,"dims mismatch on input. Check in_x and in_y.\n");
        ARRfree(input_arr);
        out_x = 0;
        out_y = 0;
        return(0);
   }


   // dimension (allocate) the output array
   out_x = in_x;
   out_y = in_y;


   // grab a handle to the new output array
   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (output_arr==NULL) 
   {
        ERRverror("", ERR_NO_HEADER | ERR_PRINT,"Couldn't get output_arr\n");
        ARRfree(input_arr);
        ARRfree(output_arr);
        out_x = 0;
        out_y = 0;
        return(0);
   }


   // check the allocation worked!
   if ( temp_size != output_size )
   {
        ERRverror("", ERR_NO_HEADER | ERR_PRINT,"dims missmatch on output. Check out_x and out_y.\n");
        ARRfree(input_arr);
        ARRfree(output_arr);
        out_x = 0;
        out_y = 0;
        return(0);
   }

   
   // assign local copies of parameters for performance
   freq_val = (float)freq_value;
   phase_val = (float)phase_value;

   // take fraction only
   phase_val = phase_val - (float)((int)phase_val);

   wave_mode_val = wave_mode;

   minamp_val = (float)min_amp;
   maxamp_val = (float)max_amp;

   // If values are outside range only take fractional portion so values are in range 0-1
   if ((min_amp<0.0) || (min_amp>1.0))
      minamp_val = minamp_val - (float)((int)minamp_val);
   if ((max_amp<0.0) || (max_amp>1.0))
      maxamp_val = maxamp_val - (float)((int)maxamp_val);

   contrast_val = (float)contrast;
   nx = in_x;
   ny = in_y;


   //
   // build the alpha function and contrast tables
   //
   ftable =   (unsigned char *) malloc(sizeof(unsigned char)*ny);
   contable = (unsigned char *) malloc(sizeof(unsigned char)*256);


   // build simple contrast control, centered on mid-grey value
   //   placed in lookup table, for fast pixel processing.
   //
   for ( i=0; i<256; i++)
   {
        u = ( (((float)i/255.0) - 0.5)*contrast_val + 0.5) * 255.0;
        if (u < 0.0)   x = 0.0;
        if (u > 255.0) x = 255.0;
        contable[i] = (unsigned char)u;
   }

   //
   // now process the alpha channel function lookup table
   //
   range = maxamp_val - minamp_val;
   if ( range < 0.0 ) {
      range = range * -1;
      minamp_val = maxamp_val;
   }

   if ( wave_mode_val == 0 )  // sine wave
   {
       for ( j=0; j<ny; j++)
       {
          x   = (float)j / (float)ny;                 // 0-1 over vertical
          aaa = sin((x*freq_val-phase_val)*6.28318);  // normalized function
          t = ((aaa+1.0)*0.5*range)+minamp_val;       // remap -1,+1 to 0-1
          ftable[j]=(unsigned char)(t*255.0);         // convert & store in byte format
       }
   }
   else if ( wave_mode_val == 1 ) // cosine squared
   {
       for ( j=0; j<ny; j++)
       {
          x   = (float)j / (float)ny;                 // 0-1 over vertical
          aaa = cos((x*freq_val-phase_val)*3.14159);  // normalized function
          t = aaa*aaa*range+minamp_val;               // square operator
          ftable[j]=(unsigned char)(t*255.0);         // convert & store in byte format
       }
   }
   else if ( wave_mode_val == 2 ) // cosine medium pulse
   {
       for ( j=0; j<ny; j++)
       {
          x   = (float)j / (float)ny;                 // 0-1 over vertical
          t = cos((x*freq_val-phase_val)*3.14159);    // normalized function
          aaa = pow(t, 8.0);                          // raise to 8th power
          u = aaa * range + minamp_val;
          ftable[j]=(unsigned char)((u)*255.0);       // convert & store in byte format
       }
   }
   else if ( wave_mode_val == 3 ) // cosine short pulse
   {
       for ( j=0; j<ny; j++)
       {
          x   = (float)j / (float)ny;                 // 0-1 over vertical
          t = cos((x*freq_val-phase_val)*3.14159);    // normalized function
          aaa = pow(t, 20.0);                         // raise to 20th power
          u = aaa * range + minamp_val;
          ftable[j]=(unsigned char)((u)*255.0);       // convert & store in byte format
       }
   }
   else if ( wave_mode_val == 4 ) // square wave
   {
       for ( j=0; j<ny; j++)
       {
          x   = (float)j / (float)ny;                 // 0-1 over vertical
          t = cos((x*freq_val-phase_val)*6.28318);    // normalized function
          if ( t > 0.0 ) aaa=1.0;
          else           aaa=0.0;                     // sharp threshold
          u = aaa * range + minamp_val;
          ftable[j]=(unsigned char)((u)*255.0);       // convert & store in byte format
       }
   }



   //
   // now for the big loop over each ARGB pixel!
   //
   for( j=0; j<ny; j++)
   {
      for( i=0; i<nx; i++)
      {
        offset = (j*nx+i)*4;
        output_arr[offset + AVS_ALPHA_BYTE] = ftable[j]; // same value for whole line

/***
  original, non contrast code
        output_arr[offset + AVS_RED_BYTE]   = input_arr[offset + AVS_RED_BYTE];
        output_arr[offset + AVS_GREEN_BYTE] = input_arr[offset + AVS_GREEN_BYTE];
        output_arr[offset + AVS_BLUE_BYTE]  = input_arr[offset + AVS_BLUE_BYTE];
 ***/

        // Apply contrast adjustment to RGB components
        output_arr[offset + AVS_RED_BYTE]   = contable[(int)input_arr[offset + AVS_RED_BYTE]];
        output_arr[offset + AVS_GREEN_BYTE] = contable[(int)input_arr[offset + AVS_GREEN_BYTE]];
        output_arr[offset + AVS_BLUE_BYTE]  = contable[(int)input_arr[offset + AVS_BLUE_BYTE]];

      } // end for pixel loop
   } // end for line loop


   // cleanup
   free (ftable);
   free (contable);
   ARRfree(input_arr);
   ARRfree(output_arr);

   return(1);
}


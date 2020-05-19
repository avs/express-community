
//
// ChromaDepth processing Module
//
//    Computes a color to depth transfer function based on
//    a 3D view framebuffer and z-buffer, generating a 
//    full-colour image (ARGB) output image suitable for
//    ChromaDepth stereo 3D glasses.
//    See Chromatek Inc. web site at www.chromatek.com
//
//    19th October 2000
//

#include "xp_comm_proj/chromad/gen.hxx"

#include <math.h>

/***********************/
/* Module Entry Point  */
/***********************/


int
ChromaDepth_ChromaDepthMods_ChromaDepthCore::update(OMevent_mask event_mask, int seq_num)
{
   // zbuffer (OMXfloat_array read req notify)
   int zbuffer_size;
   float *zbuffer_arr;

   // framebuffer (OMXbyte_array read req notify)
   int framebuffer_size;
   unsigned char *framebuffer_arr;

   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr;

   // out_size (OMXint write)
   // mix (OMXfloat read)

   int i, out_size_local;
   float lumi, zval, zmin, zmax, z, h, f, r, g, b;
   float mix_factor_local, background;

   /***********************/
   /* Function's Body     */
   /***********************/
   zbuffer_arr = (float *)zbuffer.ret_array_ptr(OM_GET_ARRAY_RD,&zbuffer_size);
   if (!zbuffer_arr) {
      ARRfree(zbuffer_arr);
      out_size = 0; // kill off output array
      return (0);
   }

   framebuffer_arr = (unsigned char *)framebuffer.ret_array_ptr(OM_GET_ARRAY_RD,&framebuffer_size);
   if (!framebuffer_arr) {
      ARRfree(zbuffer_arr);
      ARRfree(framebuffer_arr);
      out_size = 0; // kill off output array
      return (0);
   }

   //
   // dimension and allocate the output array (global), then get the pointer
   //
   if ( framebuffer_size != zbuffer_size*4 ) {
      ARRfree(zbuffer_arr);
      ARRfree(framebuffer_arr);
      out_size = 0; // kill off output array
      return (0);
   } else {
       out_size = zbuffer_size;
       out_size_local = out_size;
   }
 
   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (!output_arr) {
      ARRfree(zbuffer_arr);
      ARRfree(framebuffer_arr);
      ARRfree(output_arr);
      out_size = 0; // kill off output array
      return (0);
   }

   //
   // pre-scan zbuffer for range
   //   exclude the background value, as this is the back in scene
   //
   // first find the real background value
   zmin = 1e30;
   for (i=0; i<out_size_local; i++)
   {
        z = zbuffer_arr[i];
        if ( z < zmin ) zmin = z; 
   }

   // now find the farthest non-background value
   background = zmin;
   zmin = 1e30;
   zmax = -1e30;
   for (i=0; i<out_size_local; i++)
   {
        z = zbuffer_arr[i];
        if ( z > zmax ) zmax = z;
        if ( z > background && z < zmin ) zmin = z; 
   }
   if ( zmin == zmax ) {
      ARRfree(zbuffer_arr);
      ARRfree(framebuffer_arr);
      ARRfree(output_arr);
      out_size = 0; // kill off output array
      return (0);
   }

   // printf (" background=%f zmin=%f zmax=%f\n", background, zmin, zmax);

   //
   // set control for luminance mix-in with chroma depth
   //  mix=0 for all depth saturated colors, ignore luminance
   //  mix=1 for blended contribution of chroma depth
   //        and shading luminance from original image
   //
   mix_factor_local = mix;
   if ( mix_factor_local < 0.0 ) mix_factor_local = 0.0;
   if ( mix_factor_local > 1.0 ) mix_factor_local = 1.0;

   //
   // loop over all pixels in framebuffer and zbuffer
   //
   for (i=0; i<out_size_local*4; i+=4)
   {
         // check for background, set to black as special case
         if ( zbuffer_arr[i/4] == background ) {
            r = g = b = 0;
         }
         else
         {
            // compute luminance (black & white tone value)
            lumi =((framebuffer_arr[i+AVS_RED_BYTE]*0.299) +
                 	 (framebuffer_arr[i+AVS_GREEN_BYTE]*0.587) +
                   (framebuffer_arr[i+AVS_BLUE_BYTE]*0.114)) / 255.0;
            lumi = 0.8 * lumi + 0.2; // lower contrast, add floor for ambient

            // get normalized depth
            z = (zbuffer_arr[i/4] - zmin)/(zmax - zmin);

            // compute distance to color function, based on 4 regions
            // of distance over scene, with linear interp inside each
            // region. Red=closest, Blue=furthest away
            h = 4.0 * (1.0 - z);
            f = h - floor(h);
            if (h < 0.0 )     { r = 1;     g = 0;     b = 0; } // red
            else if (h < 1.0) { r = 1;     g = f;     b = 0; } // yellow
            else if (h < 2.0) { r = 1 - f; g = 1;     b = 0; } // green
            else if (h < 3.0) { r = 0;     g = 1;     b = f; } // cyan
            else if (h < 4.0) { r = 0;     g = 1 - f; b = 1; }
            else              { r = 0;     g = 0;     b = 1; } // blue

            // mix luminance back in for some shading effects
            r = r * ( mix_factor_local*lumi + (1.0 - mix_factor_local));
            g = g * ( mix_factor_local*lumi + (1.0 - mix_factor_local));
            b = b * ( mix_factor_local*lumi + (1.0 - mix_factor_local));

            // clamp to prevent wrap-around from numerical error
            if ( r > 1.0 ) r = 1.0;
            if ( g > 1.0 ) g = 1.0;
            if ( b > 1.0 ) b = 1.0;
            if ( r < 0.0 ) r = 0.0;
            if ( g < 0.0 ) g = 0.0;
            if ( b < 0.0 ) b = 0.0;
         }

         // store resulting full colour byte pixel
         output_arr[i+AVS_ALPHA_BYTE] = 255; // fixed alpha channel
         output_arr[i+AVS_RED_BYTE]   = (unsigned char) (r * 255);
         output_arr[i+AVS_GREEN_BYTE] = (unsigned char) (g * 255); 
         output_arr[i+AVS_BLUE_BYTE]  = (unsigned char) (b * 255);

   } // end of pixel loop

   // return 1 for success, after releasing memory
   ARRfree(zbuffer_arr);
   ARRfree(framebuffer_arr);
   ARRfree(output_arr);

   return(1);
}


//
// Background Module
//
//  generates a 2D image based on colors at the
//  corners
//
//  Converted from AVS5 - Sabreen Vig, AVS Inc., May 2000
//

#include "xp_comm_proj/backgrnd/gen.hxx"
#include <math.h>


typedef struct {
   float Red;
	float Green;
	float Blue;
} T_pixel;


void fast_inner_loop (unsigned char scanline[], T_pixel fval, T_pixel dx, int nx);
void slow_inner_loop (unsigned char scanline[], T_pixel fval, T_pixel dx, int nx, int i);
void FILTERhsv_to_rgb(float *pr, float *pg, float *pb, float ph, float ps, float pv);


int
Background_BackgroundMods_BackgroundCore::update(OMevent_mask event_mask, int seq_num)
{
  
   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr = NULL;

   // input_dims (OMXint_array read notify)
   int input_dims_size;
   int *input_dims_arr = NULL;

   // output_dims (OMXint_array write)
   int output_dims_size;
   int *output_dims_arr = NULL;

 // ulh = Upper_Left_Hue;
 // uls = Upper_Left_Sat;
 // ulv = Upper_Left_Value;
 // urh = Upper_Right_Hue;
 // urs = Upper_Right_Sat;
 // urv = Upper_Right_Value;
 // llh = Lower_Left_Hue;
 // lls = Lower_Left_Sat;
 // llv = Lower_Left_Value;
 // lrh = Lower_Right_Hue;
 // lrs = Lower_Right_Sat;
 // lrv = Lower_Right_Value;

 // nx = X_Resolution;
 // ny = Y_Resolution;

   /******************
    *  Start Code    *
    ******************/
   
   T_pixel dx, dyr, dyl, ul, ur, ll, lr;

   float ulh, uls, ulv; 
   float urh, urs, urv; 
   float llh, lls, llv; 
   float lrh, lrs, lrv; 

   int nx, ny, dither;

   int dims[2], i, k, NX, NY;

   ulh = (float)Upper_Left_Hue;
   uls = (float)Upper_Left_Sat;
   ulv = (float)Upper_Left_Value;
   urh = (float)Upper_Right_Hue;
   urs = (float)Upper_Right_Sat;
   urv = (float)Upper_Right_Value;
   llh = (float)Lower_Left_Hue;
   lls = (float)Lower_Left_Sat;
   llv = (float)Lower_Left_Value;
   lrh = (float)Lower_Right_Hue;
   lrs = (float)Lower_Right_Sat;
   lrv = (float)Lower_Right_Value;

   nx = X_Resolution;
   ny = Y_Resolution;
   dither = Dither;
 
 
   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: update\n");

   // get input image size (optional connection)
   input_dims_arr = (int *)input_dims.ret_array_ptr(OM_GET_ARRAY_RD,&input_dims_size);
   if ((input_dims_arr==NULL) || (input_dims_size!=2)) {
      // if no input, then use the UI values to set size
      dims[0] = NX = nx;
      dims[1] = NY = ny;
   }
   else
   {
      // if dims input, then ignore the UI values to set size, use
      // input image directly.
      dims[0] = NX = input_dims_arr[0];
      dims[1] = NY = input_dims_arr[1];

      ARRfree(input_dims_arr); 
   }

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

   // Convert parameter values into RGB color space
   FILTERhsv_to_rgb(&(ul.Red), &(ul.Green), &(ul.Blue), ulh, uls, ulv); 
   FILTERhsv_to_rgb(&(ur.Red), &(ur.Green), &(ur.Blue), urh, urs, urv); 
   FILTERhsv_to_rgb(&(ll.Red), &(ll.Green), &(ll.Blue), llh, lls, llv); 
   FILTERhsv_to_rgb(&(lr.Red), &(lr.Green), &(lr.Blue), lrh, lrs, lrv); 

   // first time initialization
   if (NY == 0)
   {
	   dyr.Red = lr.Red;
	   dyl.Red = ll.Red;
	   dyr.Green = lr.Green;
	   dyl.Green = ll.Green;
	   dyr.Blue = lr.Blue;
	   dyl.Blue = ll.Blue;
   }
   else
   {
	   dyr.Red = (lr.Red - ur.Red) / (float)NY;
	   dyl.Red = (ll.Red - ul.Red) / (float)NY;
	   dyr.Green = (lr.Green - ur.Green) / (float)NY;
	   dyl.Green = (ll.Green - ul.Green) / (float)NY;
	   dyr.Blue = (lr.Blue - ur.Blue) / (float)NY;
	   dyl.Blue = (ll.Blue - ul.Blue) / (float)NY;
   }

   // loop over each scan-line of output image
   for ( i=0; i<NY; i++)
   {
	   if (NX == 0)
	   {
		   dx.Red = ur.Red;
		   dx.Green = ur.Green;
		   dx.Blue = ur.Blue;
	   }
	   else
	   {
		   dx.Red = (ur.Red - ul.Red) / (float)NX;
		   dx.Green = (ur.Green - ul.Green) / (float)NX;
		   dx.Blue = (ur.Blue - ul.Blue) / (float)NX;
	   }

      k = (NY-i-1)*NX*4; // reverse top to bottom scan line order

	   if (dither)
		   slow_inner_loop(&(output_arr[k]), ul, dx, NX, i);
	   else
		   fast_inner_loop(&(output_arr[k]), ul, dx, NX);

	   ul.Red += dyl.Red;
	   ur.Red += dyr.Red;
	   ul.Green += dyl.Green;
	   ur.Green += dyr.Green;
	   ul.Blue += dyl.Blue;
	   ur.Blue += dyr.Blue;
   }

   
   // close and release the output image for further processing
   ARRfree(output_arr);

   // return 1 for success
   return(1);
}

   /***********************/
   /* Utility Functions   */
   /***********************/

void fast_inner_loop (unsigned char scanline[], T_pixel fval, T_pixel dx, int nx)
{
	int j;

	for (j=0; j<nx*4; j+=4)
	{
		fval.Red   += dx.Red;
		fval.Green += dx.Green;
		fval.Blue  += dx.Blue;
		scanline[j + AVS_ALPHA_BYTE] = 255;
		scanline[j + AVS_RED_BYTE]   = (unsigned char)(fval.Red*255.0);
		scanline[j + AVS_GREEN_BYTE] = (unsigned char)(fval.Green*255.0);
		scanline[j + AVS_BLUE_BYTE]  = (unsigned char)(fval.Blue*255.0);
	}

} // end of function "fast_inner_loop".




int dither[4][4] = 
{
	0, 8, 2, 10,
	12, 4, 14, 6,
	3, 11, 1, 9,
	15, 7, 13, 5
};

void slow_inner_loop (unsigned char scanline[], T_pixel fval, T_pixel dx, int nx, int i)
{
	int j, value, integer;
	float fraction, colour;

	for (j=0; j<nx*4; j+=4)
	{
		fval.Red += dx.Red;
		fval.Green += dx.Green;
		fval.Blue += dx.Blue;

		value = dither[i%4][(j/4)%4];

		colour = fval.Red*255;
		integer = (int)colour;
		fraction = colour - integer;
		if (fraction*16 > value)
			colour++;
		scanline[j + AVS_RED_BYTE] = (unsigned char)(colour);

		colour = fval.Green*255;
		integer = (int)colour;
		fraction = colour - integer;
		if (fraction*16 > value)
			colour++;
		scanline[j + AVS_GREEN_BYTE] = (unsigned char)(colour);

		colour = fval.Blue*255;
		integer = (int)colour;
		fraction = colour - integer;
		if (fraction*16 > value)
			colour++;
		scanline[j + AVS_BLUE_BYTE] = (unsigned char)(colour);

		scanline[j + AVS_ALPHA_BYTE] = (unsigned char)(255);
	}

} // end of function "slow_inner_loop".



#define UNDEFINED_HUE -666.0

void FILTERhsv_to_rgb(float *pr, float *pg, float *pb, float ph, float ps, float pv)
{
	float r=0, g=0, b=0;
	float ai, f, p, q, t, h, s, v;

   h = ph * 360;
   s = ps;
   v = pv;

	if ((v < 0) || (v > 1))
	{
		fprintf(stderr, "Error: Value out of range in hsv_to_rgb.\n");
	}
	else if ((s < 0) || (s > 1))
	{
		if (h == UNDEFINED_HUE)
		{
			r = v;
			g = v;
			b = v;
		}
		else
		{
			fprintf(stderr, "Error: Saturation out of range in hsv_to_rgb.\n");
		}
	}
	else
	{
		if (h == 360.)
			h = 0.;

		h = h/60;
		ai = (float)floor(h);
		f = h - ai;
		p = v * (1-s);
		q = v * (1-(s*f));
		t = v * (1-(s*(1-f)));

		switch ((int)(ai))
		{
   		case 0:
   			r=v;
   			g=t;
   			b=p;
   			break;
   		case 1:
   			r=q;
   			g=v;
   			b=p;
   			break;
   		case 2:
   			r=p;
   			g=v;
   			b=t;
   			break;
   		case 3:
   			r=p;
   			g=q;
   			b=v;
   			break;
   		case 4:
   			r=t;
   			g=p;
   			b=v;
   			break;
   		case 5:
   			r=v;
   			g=p;
   			b=q;
   			break;
   	   default:
   	      fprintf(stderr, "Error: Hue out of range in hsv_to_rgb.\n");
		}
	}
	
	*pr = r;
	*pg = g;
	*pb = b;

} // end of function hsv_to_rgb.


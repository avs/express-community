/*
 */

//
// Gamma Correct Module
//

#include "xp_comm_proj/gamma/gen.hxx"
#include <math.h>

// Uncomment the following line to enable the display of debugging information
//#define DEBUG

#ifdef DEBUG
#define PRINT(A) { printf(A); }
#else
#define PRINT(A) { }
#endif

void build_char_gamma_table(double g, unsigned char *t, int n);
void build_double_gamma_table(double g, double *t, int n);
void achromatic_gamma_correct_pixel(unsigned char *inp, unsigned char *outp, unsigned char *table, int table_length);
void chromatic_gamma_correct_pixel(unsigned char *inp, unsigned char *outp, double *table, int table_length);
void hsv_to_rgb(double *hsv, double *rgb);
void rgb_to_hsv(double *rgb, double *hsv);

#define TABLE_LENGTH 256



int
GammaCorrect_GammaCorrectMods_GammaCorrectCore::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXbyte_array read req notify)
   int input_size;
   unsigned char *input_arr = NULL;

   // gamma_value (OMXfloat read req notify)
   // method (OMXint read req notify)
   // out_size (OMXint write)
   // output (OMXbyte_array write)
   int output_size;
   unsigned char *output_arr = NULL;

   int i, num_pixels;
   unsigned char *input_pixel, *output_pixel;
   int method_val;
   double gamma_val;


   /***********************/
   /* Function's Body     */
   /***********************/
   //   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: GammaCorrect_GammaCorrectMods_GammaCorrectCore::update\n");

   PRINT("Getting input array\n");

   input_arr = (unsigned char *)input.ret_array_ptr(OM_GET_ARRAY_RD, &input_size);
   if (input_arr==NULL)
   {
	   ERRverror("", ERR_NO_HEADER | ERR_PRINT,"Couldn't get input_arr\n");
		ARRfree(input_arr);
		return(0);
   }

   PRINT("Dimensioning output array\n");

   //
   // dimension and allocate the output array (global)
   //
   num_pixels = input_size / 4;
   out_size = num_pixels;


   PRINT("Getting output array\n");

   output_arr = (unsigned char *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (output_arr==NULL)
   {
	   ERRverror("", ERR_NO_HEADER | ERR_PRINT,"Couldn't get output_arr\n");
	   ARRfree(input_arr);
      ARRfree(output_arr);
      return(0);
   }

   

   input_pixel = input_arr;
   output_pixel = output_arr;

 
   gamma_val = gamma_value;
   method_val = method;

   if (method_val == 0) {
      double table[TABLE_LENGTH];

      PRINT("Building double gamma table\n");

      build_double_gamma_table(gamma_val,table,TABLE_LENGTH);

      PRINT("Changing output pixels\n");

      for(i=0; i<num_pixels; i++,input_pixel+=4,output_pixel+=4)
      {
   		chromatic_gamma_correct_pixel(input_pixel, output_pixel, table, TABLE_LENGTH);
      }
   }
   else {
      unsigned char table[TABLE_LENGTH];

      PRINT("Building char gamma table\n");

      build_char_gamma_table(gamma_val,table,TABLE_LENGTH);

      PRINT("Changing output pixels\n");

      for(i=0; i<num_pixels; i++,input_pixel+=4,output_pixel+=4)
      {
   		achromatic_gamma_correct_pixel(input_pixel, output_pixel, table, TABLE_LENGTH);
      }
   }

   PRINT("Freeing arrays\n");

   ARRfree(input_arr);
   ARRfree(output_arr);

   PRINT("Finished gamma correcting\n");

   // return 1 for success
   return(1);
}


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))


void build_char_gamma_table(double gval,unsigned char *t,int n)
{
	int i;
	double temp;
	double scale = 1./(double)(n-1);

	gval = 1./gval;

	for(i=0; i<n; i++)
	{
		temp = (double)i * scale;
		*(t+i) = (unsigned char)( MIN( MAX(0, pow(temp, gval)), 1) * 255);
		// printf("table %i = %d\n", i, *(t+i));
	}
}

void build_double_gamma_table(double gval,double *t,int n)
{
	int i;
	double temp;
	double scale = 1./(double)(n-1);

	gval = 1./gval;

	for(i=0; i<n; i++)
	{
		temp = (double)i * scale;
		*(t+i) = MIN( MAX(0, pow(temp, gval)), 1);
		// printf("table %i = %g\n", i, *(t+i));
	}
}


#define ONE_OVER_255 0.003921568

void achromatic_gamma_correct_pixel(unsigned char *inp, unsigned char *outp, unsigned char *table, int table_length)
{
	int index;

   *(outp + AVS_ALPHA_BYTE) = *(inp + AVS_ALPHA_BYTE);

   index = *(inp + AVS_RED_BYTE);
	*(outp + AVS_RED_BYTE) = table[index];
	index = *(inp + AVS_GREEN_BYTE);
	*(outp + AVS_GREEN_BYTE) = table[index];
	index = *(inp + AVS_BLUE_BYTE);
	*(outp + AVS_BLUE_BYTE) = table[index];

} // end of function achromatic_gamma_correct_pixel.


void chromatic_gamma_correct_pixel(unsigned char *inp, unsigned char *outp, double *table, int table_length)
{
	double rgb[3];
   double hsv[3];
	int index;

	//convert rgb from range 0...255 to range 0...1.
 	rgb[0] = *(inp + AVS_RED_BYTE) * ONE_OVER_255;
  	rgb[1] = *(inp + AVS_GREEN_BYTE) * ONE_OVER_255;
  	rgb[2] = *(inp + AVS_BLUE_BYTE) * ONE_OVER_255;

	rgb_to_hsv(rgb,hsv);

	index = (int)(hsv[2] * 255.0);
	hsv[2] = (double)table[index];

	hsv_to_rgb(hsv,rgb);

	//convert rgb from range 0...1 to range 0...255.
	rgb[0] *= 255.;
	rgb[1] *= 255.;
	rgb[2] *= 255.;

   *(outp + AVS_ALPHA_BYTE) = *(inp + AVS_ALPHA_BYTE);
	*(outp + AVS_RED_BYTE) = (unsigned char)(rgb[0]);
	*(outp + AVS_GREEN_BYTE) = (unsigned char)(rgb[1]);
	*(outp + AVS_BLUE_BYTE)= (unsigned char)(rgb[2]);

} // end of function chromatic_gamma_correct_pixel.




#define UNDEFINED_HUE -666.0

void hsv_to_rgb(double *hsv, double *rgb)
{
	double h,s,v,f,p,q,t,r,g,b,ai;
	
	h = hsv[0];
	s = hsv[1];
	v = hsv[2];

	if (s == 0.)
	{
		if (h == UNDEFINED_HUE)
		{
			r = v;
			g = v;
			b = v;
		}
		else {
			fprintf(stderr,"Error condition in hsv_to_rgb.\n");
		}
	}
	else
	{
		if (h == 360.)
			h = 0.;
		h = h/60.;
		ai = floor(h);
		f = h - ai;
		p = v * (1.-s);
		q = v * (1.-(s*f));
		t = v * (1.-(s*(1.-f)));

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
		}
	}

	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
} // end of function hsv_to_rgb.

void rgb_to_hsv(double *rgb, double *hsv)
{
	double min,max,v,s,h,rc,gc,bc,r,g,b;

	r = rgb[0];
	g = rgb[1];
	b = rgb[2];

	max = MAX(r,g);
	max = MAX(max,b);

	min = MIN(r,g);
	min = MIN(min,b);

	v = max;

	if (max != 0.)
		s = (max - min)/max;
	else
		s = 0.;

	if (s ==0)
		h = UNDEFINED_HUE;
	else
	{
		rc = (max - r)/(max - min);
		gc = (max - g)/(max - min);
		bc = (max - b)/(max - min);
		if (r == max)
			h = bc - gc;
		else if (g == max)
			h = 2 + rc - bc;
		else if (b == max)
			h = 4 + gc - rc;
		h = h * 60;
		if (h < 0.)
			h = h + 360.;
	}
	
	hsv[0] = h;
	hsv[1] = s;
	hsv[2] = v;
} // end of function rgb_to_hsv.


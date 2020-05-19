
#include <math.h>

#include <avs/gd_def.h>
#include "xp_comm_proj/datmp_fn/gen.hxx"

#ifndef M_PI
#define M_PI    3.141592653589793238462643383279
#endif

#define MIN(A,B) ((A)<(B) ? (A) : (B))
#define MAX(A,B) ((A)>(B) ? (A) : (B))

#define NORMALIZE_VALUE ((val - min) / width)

#define UNDEFINED_HUE -666.0

#define NUM_FUNCS 7

static void convertHSVtoRGB(float *hsv, float *rgb);
static void convertRGBtoHSV(float *hsv, float *rgb);



/*=====================================================================*/
/*             Place user defined function here                        */

static inline void
UserDefinedFunc(float val, float min, float width, float *rgb)
{
   float norm_val, temp;
   float hsv[3];

   norm_val = 0.6667 * NORMALIZE_VALUE;

   hsv[0] = 0.3333 + norm_val;
   hsv[1] = 1.0;
   hsv[2] = 1.0;

   convertHSVtoRGB(hsv, rgb);
}

/*             End of user defined function                            */
/*=====================================================================*/



static inline void
StandardDatamapFunc(float val, float min, float width, float *rgb)
{
   float hsv[3];

   hsv[0] = 0.6667 * (1.0 - NORMALIZE_VALUE);
   hsv[1] = 1.0;
   hsv[2] = 1.0;

   convertHSVtoRGB(hsv, rgb);
}

static inline void
GreyscaleFunc(float val, float min, float width, float *rgb)
{
   float norm_val;

   norm_val = NORMALIZE_VALUE;
   norm_val = MAX(0.0, norm_val);
   norm_val = MIN(norm_val, 1.0);

   rgb[0] = norm_val;
   rgb[1] = norm_val;
   rgb[2] = norm_val;
}

static inline void
HotIronFunc(float val, float min, float width, float *rgb)
{
   float norm_val, temp;

   norm_val = NORMALIZE_VALUE;
   norm_val = 0.9 * norm_val + 0.1;

   temp = MAX(0.0, 3*norm_val);
   rgb[0] = MIN(temp, 1.0);
   temp = MAX(0.0, 3*(norm_val - 0.3333));
   rgb[1] = MIN(temp, 1.0);
   temp = MAX(0.0, 3*(norm_val - 0.6667));
   rgb[2] = MIN(temp, 1.0);
}

static inline void
CoolBlueFunc(float val, float min, float width, float *rgb)
{
   float norm_val, temp;

   norm_val = NORMALIZE_VALUE;
   norm_val = 0.9 * norm_val + 0.1;

   temp = MAX(0.0, 3*(norm_val - 0.6667));
   rgb[0] = MIN(temp, 1.0);
   temp = MAX(0.0, 3*(norm_val - 0.3333));
   rgb[1] = MIN(temp, 1.0);
   temp = MAX(0.0, 3*norm_val);
   rgb[2] = MIN(temp, 1.0);
}

static inline void
BlackWhiteBandsFunc(float val, float min, float width, float *rgb)
{
   int int_val;

   int_val = ((int)(NORMALIZE_VALUE * 10)) % 2;

   rgb[0] = int_val;
   rgb[1] = int_val;
   rgb[2] = int_val;
}

static inline void
ColourBandsFunc(float val, float min, float width, float *rgb)
{
   float norm_val;
   float hsv[3];
   int int_val;

   norm_val = NORMALIZE_VALUE * 10;
   int_val = (int)norm_val;

   hsv[0] = 0.66 * (1.0 - (float)int_val/10.0);
   hsv[1] = 1.0;
   hsv[2] = 0.5 + 0.5 * (norm_val - int_val);

   convertHSVtoRGB(hsv, rgb);
}




int
DatamapFunction_DatamapFunctionMods_DatamapFunctionCore::update(OMevent_mask event_mask, int seq_num)
{

   // in (Node_Data read req notify)
   // in.nnodes (int)
   // in.nnode_data (int)
   // in.node_data[].veclen (int) 
   // in.node_data[].values (char [])
   float *in_node_data = NULL;
   int in_size;
   float in_min, in_max, in_width;

   // out (Node_Data write)
   // out.nnodes (int)
   // out.nnode_data (int)
   // out.node_data[].veclen (int) 
   // out.node_data[].values (char [])
   float *out_node_data = NULL;
   int out_size, out_type;

   int i, dmap_func;

   //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: DatamapFunction_DatamapFunctionMods_DatamapFunctionCore::update\n");


   out.nnodes = 0;
   out.nnode_data = 0;

   if ((!in.nnodes.valid_obj()) || (in.nnodes <= 0)) {
      return(0);
   }
   out.nnodes = in.nnodes;

   if ((!in.nnode_data.valid_obj()) || (in.nnode_data <= 0)) {
      ERRverror("DatamapFunctionCore", ERR_ERROR, "No node data components.\n");
   }
   else {
      int in_scalar_nd = -1;

      for (int i=0; i<in.nnode_data; i++) {
         if (in.node_data[i].veclen == 1) {
            in_scalar_nd = i;
            break;
         }
      }

      if (in_scalar_nd == -1) {
         ERRverror("DatamapFunctionCore", ERR_ERROR, "Input node data must be scalar.\n");
      }
      else {
         in_node_data = (float *)in.node_data[in_scalar_nd].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &in_size);
         if (in_node_data == NULL) {
            ERRverror("DatamapFunctionCore", ERR_ERROR, "Could not get input node data array.\n");
         }
         if (in_size != in.nnodes) {
            ERRverror("DatamapFunctionCore", ERR_ERROR, "Input node data array is incorrect size.\n");
         }

         in_min = (float)in.node_data[in_scalar_nd].min;
         in_max = (float)in.node_data[in_scalar_nd].max;
         in_width = in_max - in_min;
      }
   }



   out.nnode_data = 1;
   out.node_data[0].veclen = 3;
   out.node_data[0].id = GD_COLOR_DATA_ID;
   if (FLDset_node_data_type( (OMobj_id)out, 0, OM_TYPE_FLOAT) != OM_STAT_SUCCESS) {
      ERRverror("DatamapFunctionCore", ERR_ERROR, "Could not set output node data array to float type.\n");
      return(0);
   }

   out_node_data = (float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_size, &out_type);
   if (out_node_data == NULL) {
      ERRverror("DatamapFunctionCore", ERR_ERROR, "Could not get output node data array.\n");
      return(0);
   }
   if (out_size != (out.nnodes*3)) {
      ERRverror("DatamapFunctionCore", ERR_ERROR, "Output node data array is incorrect size.\n");
      return(0);
   }
   if (out_type != OM_TYPE_FLOAT) {
      ERRverror("DatamapFunctionCore", ERR_ERROR, "Output node data array is incorrect type.\n");
      return(0);
   }

   if ((!datamap_function.valid_obj()) || (datamap_function < 0) || (datamap_function >= NUM_FUNCS)) {
      dmap_func = 0;
   }
   else {
      dmap_func = datamap_function;
   }



   /***********************/
   /* Function's Body     */
   /***********************/

   if ((in_node_data==NULL) || (in_width == 0.0)) {
      for (i=0; i < out_size; i++) {
         out_node_data[i] = 1.0;
      }
   }
   else {
      switch (dmap_func)
      {
         case 0:
            for (i=0; i < in_size; i++) {
               UserDefinedFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 1:
            for (i=0; i < in_size; i++) {
               StandardDatamapFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 2:
            for (i=0; i < in_size; i++) {
               GreyscaleFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 3:
            for (i=0; i < in_size; i++) {
               HotIronFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 4:
            for (i=0; i < in_size; i++) {
               CoolBlueFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 5:
            for (i=0; i < in_size; i++) {
               BlackWhiteBandsFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         case 6:
            for (i=0; i < in_size; i++) {
               ColourBandsFunc(in_node_data[i], in_min, in_width, &out_node_data[i*3]);
            }
            break;

         default:
            for (i=0; i < out_size; i++) {
               out_node_data[i] = 1.0;
            }
      }
   }

   if (in_node_data!=NULL)  ARRfree( (char*)in_node_data );
   if (out_node_data!=NULL) ARRfree( (char*)out_node_data );
   

   // return 1 for success
   return(1);
}


/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

/*           RGB-HSV conversion functions.                                          */

/*----------------------------------------------------------------------------------*/




static void convertHSVtoRGB(float *hsv, float *rgb)
{
   float h,s,v,r,g,b;

   h = hsv[0];
   s = hsv[1];
   v = hsv[2];

   if (v <= 0.0) {
      r = g = b = 0.0;
   }
   else if (s <= 0.0) {
      r = g = b = v;
   }
   else
   {
      int ai;
      float f,p,q,t;

      h *= 6.0;
      if (h >= 6.0)
         h = 0.0;

      ai = (int)h;
      f = h - ai;
      p = v * (1.0 - s);
      q = v * (1.0 - (s * f));
      t = v * (1.0 - (s * (1.0-f)));

      switch (ai)
      {
         case 0:
            r=v; g=t; b=p;	break;

         case 1:
            r=q; g=v; b=p;	break;

         case 2:
            r=p; g=v; b=t;	break;

         case 3:
            r=p; g=q; b=v;	break;

         case 4:
            r=t; g=p; b=v;	break;

         case 5:
            r=v; g=p; b=q;	break;
      }
   }


   // Clamp values to 0..1 range to ensure that rounding errors don't
   // cause problems.

   if (r<0.0) rgb[0] = 0.0;
   else if (r>1.0) rgb[0] = 1.0;
   else rgb[0] = r;

   if (g<0.0) rgb[1] = 0.0;
   else if (g>1.0) rgb[1] = 1.0;
   else rgb[1] = g;

   if (b<0.0) rgb[2] = 0.0;
   else if (b>1.0) rgb[2] = 1.0;
   else rgb[2] = b;
}



static void convertRGBtoHSV(float *rgb, float *hsv)
{
   float r,g,b,h,s,v,min,max,width;

   r = rgb[0];
   g = rgb[1];
   b = rgb[2];

   max = MAX(r,g);
   max = MAX(max,b);

   min = MIN(r,g);
   min = MIN(min,b);

   width = max - min;

   v = max;

   if (max != 0.0)
      s = width/max;
   else
      s = 0.0;

   if (s == 0.0) {
      h = UNDEFINED_HUE;
   }
   else
   {
      if (r == max)
         h = (g - b)/width;
      else if (g == max)
         h = 2 + (b - r)/width;
      else if (b == max)
         h = 4 + (r - g)/width;

      if (h < 0.0)
         h += 1.0;
   }

   hsv[0] = h;
   hsv[1] = s;
   hsv[2] = v;
}


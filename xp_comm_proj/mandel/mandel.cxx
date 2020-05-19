
#include "xp_comm_proj/mandel/gen.h"


int
Mandelbrot_MandelbrotMods_MandelbrotGen::update(OMevent_mask event_mask, int seq_num)
{

   int  out_data_size, out_data_type;
   char *out_node_data = (char *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);
   
   int i, j, k, index;
   double dx, dy;
   double cx, cy;
   double zx, zy;
   double tmp;
   int max_k;
   
   index=0;

   /***********************/
   /* Function's Body     */
   /***********************/

   if (max_iter<256) {
     max_k = max_iter;
   } else {
     max_k = 255;
   }

   dx = (rightx - leftx) / (double)(resx-1);
   dy = (topy - bottomy) / (double)(resy-1);
   
   for( j=0; j<resy; j++ ) {
     cy = bottomy + (double)(dy * j);
     for( i=0; i<resx; i++ ) {
       cx = leftx + (double)(dx * i);

       zx = zy = 0.0;
       for( k=0; k<max_k; k++ ) {
      	 tmp = (zx*zx) - (zy*zy) + cx;
	       zy = 2 * zx * zy + cy;
	       zx = tmp;
	       if( ((zx*zx) + (zy*zy)) > max_value )
	         break;
       }
       
       out_node_data[index++] = k;
     }
   }
   
   if (out_node_data)
      ARRfree((char *)out_node_data);

   // return 1 for success
   return(1);
}

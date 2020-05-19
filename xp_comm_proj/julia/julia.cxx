
#include "../xp_comm_proj/julia/gen.h"


int
Julia_JuliaMods_JuliaGen::update(OMevent_mask event_mask, int seq_num)
{

   int  out_data_size, out_data_type;
   char *out_node_data = (char *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);
   
   int i, j, k, index;
   double dx, dy;
   double zx, zy;
   double tmp;
   double posy;   // Used to unroll y pos. calculation in main function loop
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
     posy = bottomy + (double)(dy * j);
     for( i=0; i<resx; i++ ) {

        zy = posy;
        zx = leftx + (double)(dx * i);

        for( k=0; k<max_k; k++ ) {
          tmp = (zx*zx) - (zy*zy) + mandelx;
	        zy = 2 * zx * zy + mandely;
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

//
// reducer.cxx - Ian Curington, July 97
//
// similar to DVdownsize, but works on primitive 1D float arrays,
// and averages rather than skips values
//
//
#include <math.h>
#include "gen.h"

int
Reducer_ReducerMods_ReducerCore::downsizer(OMevent_mask event_mask, int seq_num)
{
   int in_size, out_size;
   float *in_arr, *out_arr;
   int loc_factor, i,j,k;
   float t;
   
   loc_factor = factor;

   if (loc_factor < 2)
   {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,"factor must be at least 2!\n");
       return(0);
   }

   in_arr = (float *) in.ret_array_ptr(OM_GET_ARRAY_RD, &in_size);
   if ( !in_arr || in_size <= 1 ) {
       nout = 0;
       return(1);
   }
   nout = in_size / loc_factor;
   out_arr = (float *) out.ret_array_ptr(OM_GET_ARRAY_RW, &out_size);
   if ( out_size <= 1 ) 
   {
       ARRfree((char *) in_arr);
       return(1);
   }

   // simple box filter, average intermediate values
   for (i=0; i < out_size; i++)
   {
        t = 0.0;
        for (j=0; j < loc_factor; j++)
        {
          k = j + i*loc_factor;
          if ( k >= in_size ) k = in_size -1;
          t += in_arr[ k ];
        }
        out_arr[i] = t / (float)loc_factor;
   }

   ARRfree((char *) in_arr);
   ARRfree((char *) out_arr);

   // return 1 for success
   return(1);
}

//
// agxint.cxx 
// 

#include "gen.h"
#include <agx/agx.h>
#include "agxint.h"

int
Krig_KrigMods_agxint::update(OMevent_mask event_mask, int seq_num)
{
   // x_array (OMXfloat_array read req)
   int x_array_size;
   float *x_array_arr;

   // y_array (OMXfloat_array read req)
   int y_array_size;
   float *y_array_arr;

   // values (OMXfloat_array read req)
   int values_size;
   float *values_arr;

   // num_points (OMXint read req)
   // grid_out (OMXfloat_array write)
   int grid_out_size;
   float *grid_out_arr;

   // num_x (OMXint read req)
   // num_y (OMXint read req)

   /***********************/
   /* Function's Body     */
   /***********************/
//   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Krig_KrigMods_agxint::update\n");

   x_array_arr = (float *)x_array.ret_array_ptr(OM_GET_ARRAY_RD,&x_array_size);

   y_array_arr = (float *)y_array.ret_array_ptr(OM_GET_ARRAY_RD,&y_array_size);

   values_arr = (float *)values.ret_array_ptr(OM_GET_ARRAY_RD,&values_size);

   grid_out_arr = (float *)grid_out.ret_array_ptr(OM_GET_ARRAY_WR,&grid_out_size);

   if (x_array_arr && y_array_arr && values_arr && grid_out_arr) {

    //set up uniras environment

    XuMessageTypes(XuOFF,XuOFF);
    XuDeviceSelect("sel ldummy;exit");
    XuOpen(XuCREATE);  

    XuGriddingLocalFitCalc(x_array_arr, y_array_arr,
                           values_arr, num_points, grid_out_arr, num_y, num_x); 
    

    XuClose();

   }


   //Free Arrays
   if (x_array_arr) {
      ARRfree(x_array_arr);
   }

   if (y_array_arr) {
      ARRfree(y_array_arr);
   }

   if (values_arr) {
      ARRfree(values_arr);
   }

   if (grid_out_arr) {
      ARRfree(grid_out_arr);
   }

   // return 1 for success
   return(1);
}
// end of file


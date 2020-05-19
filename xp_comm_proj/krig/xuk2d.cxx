
//
// Krigging 2D module
//  

#include "gen.h"

#include <agx/agx.h>
#include "agxkri.h"
#include "gam.h"

 
int
Krig_KrigMods_XuKrigging2D::update(OMevent_mask event_mask, int seq_num)
{
   // xarr (OMXfloat_array read req)
   int xarr_size;
   float *xarr_arr;

   // yarr (OMXfloat_array read req)
   int yarr_size;
   float *yarr_arr;

   // values (OMXfloat_array read req)
   int values_size;
   float *values_arr;

   // radius (OMXdouble read req)
   // ny (OMXint read req)
   // poly_x (OMXfloat_array read)
   int poly_x_size;
   float *poly_x_arr;

   // poly_y (OMXfloat_array read)
   int poly_y_size;
   float *poly_y_arr;

   // npoly_pts (OMXint read)
   // zest (OMXfloat_array write)
   int zest_size;
   float *zest_arr;

   // zstd (OMXfloat_array write)
   int zstd_size;
   float *zstd_arr;

   // trigger (OMXint read notify)

   /***********************/
   /* Function's Body     */
   /***********************/
// ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Krig_KrigMods_XuKrigging2D::update\n");

   xarr_arr = (float *)xarr.ret_array_ptr(OM_GET_ARRAY_RD,&xarr_size);

   yarr_arr = (float *)yarr.ret_array_ptr(OM_GET_ARRAY_RD,&yarr_size);

   values_arr = (float *)values.ret_array_ptr(OM_GET_ARRAY_RD,&values_size);

   poly_x_arr = (float *)poly_x.ret_array_ptr(OM_GET_ARRAY_RD,&poly_x_size);

   poly_y_arr = (float *)poly_y.ret_array_ptr(OM_GET_ARRAY_RD,&poly_y_size);

   zest_arr = (float *)zest.ret_array_ptr(OM_GET_ARRAY_WR,&zest_size);

   zstd_arr = (float *)zstd.ret_array_ptr(OM_GET_ARRAY_WR,&zstd_size);

   if (xarr_arr && yarr_arr && values_arr && zest_arr && zstd_arr) {

    //set up uniras environment
    XuMessageTypes(XuOFF,XuOFF);
    XuKrigingOpen();
    XuDeviceSelect("sel ldummy;exit");
    XuOpen(XuCREATE);  

    //do region clip
    if (poly_x_arr && poly_y_arr) {
      XuKrigingRegionPolygon(poly_x_arr, poly_y_arr, npoly_pts); 
    }

    KrigExp = (float) KrigExponent;
    KrigFac = (float) KrigFactor;
    

    XuKriging2DCalc(xarr_arr, yarr_arr, values_arr,
                    np, radius, ny, nx, zest_arr, zstd_arr, lgam);

    XuClose();
  }


   //Free Arrays
   if (xarr_arr) {
      ARRfree(xarr_arr);
   }

   if (yarr_arr) {
      ARRfree(yarr_arr);
   }

   if (values_arr) {
      ARRfree(values_arr);
   }

   if (poly_x_arr) {
      ARRfree(poly_x_arr);
   }

   if (poly_y_arr) {
      ARRfree(poly_y_arr);
   }

   if (zest_arr) {
      ARRfree(zest_arr);
   }

   if (zstd_arr) {
      ARRfree(zstd_arr);
   }

   // return 1 for success
   return(1);
}
// end of file



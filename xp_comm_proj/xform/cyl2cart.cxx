/*
   This module takes a series of (r,theta,z) coordinates and converts them to cartesian
   (X,Y,Z) positions.
   Inputs:
      cyl : OMXfloat[][3]: cylindrical coords in the format (r,t,z,r,t,z,r,t,z,...,r,t,z)
   Outputs:
      cart: OMXfloat[][3]: the cartesian (X,Y,Z) coordinates, with the points in the same
                           order as the input cyl.
      n   : OMXint       : the length of the cyl array, and used to set the length of
                           the cart array.

   Versions:
      15/09/97 - written by: J. Maccelari, visualn@iafrica.com
*/

#include "xp_comm_proj/xform/gen.hxx"
#include <math.h>

int
Xform_XformMods_cylindrical_to_cartesian::update(OMevent_mask event_mask, int seq_num)
{
   // cyl (OMXfloat_array read req notify)
   // n (OMXint write)
   // cart (OMXfloat_array write)
   #define PI (float)3.14159265
   #define VISMINVAL 1.0e-16
   float *cyl_arr, *cart_arr;
   int cart_size, cyl_size, i;
   float x, y, z, r, theta;

   /***********************/
   /* Function's Body     */
   /***********************/

   cyl_arr = (float*)cyl.ret_array_ptr(OM_GET_ARRAY_RD,&cyl_size);
   if (!cyl_arr)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cylindrical_to_cartesian: cannot get cyl\n");
      n = 0;
      cart.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   if ((cyl_size % 3) != 0)
   {
      if (cyl_arr) ARRfree((char*)cyl_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cylindrical_to_cartesian: cyl array is not [][3]\n");
      n = 0;
      cart.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   // set cart array size [n/3][3]
   n = cyl_size;
   cart_arr = (float *)cart.ret_array_ptr(OM_GET_ARRAY_WR,&cart_size);
   if (!cart_arr)
   {
      if (cyl_arr) ARRfree((char*)cyl_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cylindrical_to_cartesian: cannot get cart\n");
      n = 0;
      return(1);
   }

   for (i = 0; i < (cyl_size/3); i++)
   {
      r     = cyl_arr[i*3+0];
      theta = cyl_arr[i*3+1];
      z     = cyl_arr[i*3+2];
      theta *= PI/(float)180.; 
      x = r*cos(theta);
      y = r*sin(theta);
      cart_arr[i*3+0] = x;
      cart_arr[i*3+1] = y;
      cart_arr[i*3+2] = z;
   }

   // free memory
   if (cyl_arr) ARRfree((char*)cyl_arr);
   if (cart_arr) ARRfree((char*)cart_arr);

   // return 1 for success
   return(1);
}


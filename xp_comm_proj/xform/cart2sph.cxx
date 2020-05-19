/*
   This module takes a series of (X,Y,Z) coordinates and converts them to spherical
   (r,theta,phi) positions.
   Inputs:
      cart   : OMXfloat[][3]: cartesian coords in the format (X,Y,Z,X,Y,Z,X,Y,Z,...,X,Y,Z)
   Outputs:
      sph    : OMXfloat[][3]: the spherical (r,theta,phi) coordinates, with the points in the
                              same order as the input cart.
      n      : OMXint       : the length of the cart array; used to set the length of
                              the sph array.
   NOTE: theta is the angle in the X-Y plane, phi is the angle from the Z axis. Some spherical
         coordinate systems reverse these two!

   Versions:
      15/09/97 - written by: J. Maccelari, visualn@iafrica.com
*/

#include "xp_comm_proj/xform/gen.hxx"
#include <math.h>

int
Xform_XformMods_cartesian_to_spherical::update(OMevent_mask event_mask, int seq_num)
{
   // cart (OMXfloat_array read req notify)
   // n (OMXint write)
   // sph (OMXfloat_array write)
   #define PI (float)3.14159265
   #define VISMINVAL 1.0E-16
   int cart_size, i, sph_size;
   float *cart_arr, *sph_arr;
   float phi, r, sign, theta, x, y, z;


   /***********************/
   /* Function's Body     */
   /***********************/

   cart_arr = (float *)cart.ret_array_ptr(OM_GET_ARRAY_RD,&cart_size);
   if (!cart_arr)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cartesian_to_spherical: problem getting cart.");
      n = 0;
      sph.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   if ((cart_size % 3) != 0)
   {
      if (cart_arr) free((char*)cart_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cartesian_to_spherical: cart is not [][3].");
      n = 0;
      sph.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }
   
   // set cart size
   n = cart_size;
   sph_arr = (float *)sph.ret_array_ptr(OM_GET_ARRAY_WR,&sph_size);
   if (!sph_arr)
   {
      if (cart_arr) ARRfree((char*)cart_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cart_to_spherical: cannot get sph.");
      n = 0;
      return(1);
   }

   // do transformation
   for (i = 0; i < (cart_size/3); i++)
   {
      x = cart_arr[i*3+0];
      y = cart_arr[i*3+1];
      z = cart_arr[i*3+2];
      r = sqrt(x*x+y*y+z*z);
      if (fabs(x) < VISMINVAL)
      {
         if (y < 0.)
            sign = (float)-1.;
         else
            sign = (float)1.;
         theta = PI/2.*sign;
      }
      else
         theta = atan(y/x);
      if (r > 1.0e-8)
         phi = acos(z/r);
      else
         phi = (float)0.;
      theta *= (float)180./PI;
      phi *= (float)180./PI;
      sph_arr[i*3+0] = r;
      sph_arr[i*3+1] = theta;
      sph_arr[i*3+2] = phi;
   }

   // free memory
   if (cart_arr) ARRfree((char*)cart_arr);
   if (sph_arr) ARRfree((char*)sph_arr);

   // return 1 for success
   return(1);
}


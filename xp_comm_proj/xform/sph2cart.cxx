/*
   This module takes a series of (r,theta,phi) coordinates and converts them to cartesian
   (X,Y,Z) positions.
   Inputs:
      sph    : OMXfloat[][3]: spherical coords in the format (r,t,p,r,t,p,r,t,p,...,r,t,p)
   Outputs:
      cart   : OMXfloat[][3]: the cartesian (X,Y,Z) coordinates, with the points in the same
                              order as the input sph array.
      n      : OMXint       : the length of the sph array; used to set the length of
                              the cart array.
   NOTE: theta is the angle in the X-Y plane, phi is the angle from the Z axis. Some spherical
         coordinate systems reverse these two!

   Versions:
      15/09/97 - written by: J. Maccelari, visualn@iafrica.com
*/

#include "xp_comm_proj/xform/gen.hxx"
#include <math.h>

int
Xform_XformMods_spherical_to_cartesian::update(OMevent_mask event_mask, int seq_num)
{
   // sph (OMXfloat_array read req notify)
   // n (OMXint write)
   // cart (OMXfloat_array write)
   #define PI (float)3.14159265
   #define VISMINVAL 1.0E-16
   int cart_size, i, sph_size;
   float *cart_arr, *sph_arr;
   float phi, r, theta, x, y, z;

   /***********************/
   /* Function's Body     */
   /***********************/

   sph_arr = (float *)sph.ret_array_ptr(OM_GET_ARRAY_RD,&sph_size);
   if (!sph_arr)
   {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"spherical_to_cartesian: problem getting sph.");
      n = 0;
      cart.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   if ((sph_size % 3) != 0)
   {
      if (sph_arr) ARRfree((char *)sph_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"spherical_to_cartesian: sph is not [][3].");
      n = 0;
      cart.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }


   // set cart size
   n = sph_size;
   cart_arr = (float *)cart.ret_array_ptr(OM_GET_ARRAY_WR,&cart_size);
   if (!cart_arr)
   {
      if (sph_arr) ARRfree((char *)sph_arr);
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"spherical_to_cartesian: problem getting cart.");
      n = 0;
      return(1);
   }

   for (i = 0; i < (sph_size/3); i++)
   {
      r     = sph_arr[i*3+0];
      theta = sph_arr[i*3+1];
      phi   = sph_arr[i*3+2];
      theta *= PI/(float)180.;
      phi   *= PI/(float)180.;
      x = r*sin(phi)*cos(theta);
      y = r*sin(phi)*sin(theta);
      z = r*cos(phi);
      cart_arr[i*3+0] = x;
      cart_arr[i*3+1] = y;
      cart_arr[i*3+2] = z;
   }

   // free memory
   if (sph_arr) ARRfree((char *)sph_arr);
   if (cart_arr) ARRfree((char *)cart_arr);

   // return 1 for success
   return(1);
}


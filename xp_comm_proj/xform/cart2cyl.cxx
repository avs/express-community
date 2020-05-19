/*
   This module takes a series of (X,Y,Z) coordinates and converts them to cylindrical
   (r,theta,z) positions.
   Inputs:
      cart: OMXfloat[][3]: cartesian coords in the format (X,Y,Z,X,Y,Z,X,Y,Z,...,X,Y,Z)
   Outputs:
      cyl : OMXfloat[][3]: the cylindrical (r,theta,z) coordinates, with the points in the same
                           order as the input cart.
      n   : OMXint       : the length of the cart array, and used to set the length of
                           the cyl array.

   Versions:
      15/09/97 - written by: J. Maccelari, visualn@iafrica.com
*/

#include "xp_comm_proj/xform/gen.hxx"
#include <math.h>

int
Xform_XformMods_cartesian_to_cylindrical::update(OMevent_mask event_mask, int seq_num)
{
   // cart (OMXfloat_array read req notify)
   // n (OMXint write)
   // cyl (OMXfloat_array write)
   #define PI 3.14159265
   #define VISMINVAL 1.0e-16
   int cart_size, cyl_size, i;
   float *cart_arr, *cyl_arr;
   float r, theta, x, y, z;

   /***********************/
   /* Function's Body     */
   /***********************/

   cart_arr = (float *)cart.ret_array_ptr(OM_GET_ARRAY_RD,&cart_size);
   if (!cart_arr)
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"cartesian_to_cylindrical: problem getting cart\n");
      n = 0;
      cyl.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   if ((cart_size % 3) != 0)
   {
      if (cart_arr) ARRfree((char *)cart_arr);
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"cartesian_to_cylindrical: cart array size != [][3]\n");
      n = 0;
      cyl.set_array(OM_TYPE_FLOAT,(char*)NULL,0,OM_SET_ARRAY_COPY);
      return(1);
   }

   // set cyl array size = [n/3][3]
   n = cart_size;
   cyl_arr = (float *)cyl.ret_array_ptr(OM_GET_ARRAY_WR,&cyl_size);
   if (!cyl_arr)
   {
      if (cart_arr) ARRfree((char *)cart_arr);
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"cartesian_to_cylindrical: problem getting cyl\n");
      n = 0;
      return(1);
   }

   for (i = 0; i < (cart_size/3); i++)
   {
      x = cart_arr[i*3+0];
      y = cart_arr[i*3+1];
      z = cart_arr[i*3+2];
      r = sqrt(x*x+y*y);
      if (fabs(x) < VISMINVAL)
      {
         if (y < 0)
            theta = -PI/(float)2.;
         else
            theta = PI/(float)2.;
      }
      else
         theta = atan(y/x);
      // get correct quadrant and map onto 0->2*PI
      if (x < 0)
         theta += PI;
      else
         if (theta < 0)
            theta += 2.*PI;
      theta *= (float)180./PI;
      cyl_arr[i*3+0] = r;
      cyl_arr[i*3+1] = theta;
      cyl_arr[i*3+2] = z;
   }

   // free memory
   if (cart_arr) ARRfree((char *)cart_arr);
   if (cyl_arr) ARRfree((char *)cyl_arr);

   // return 1 for success
   return(1);
}


/*****************************************************************************

                        Copyright (c) 1998 by
                        Advanced Visual Systems Inc.
                        All Rights Reserved

        This software comprises unpublished confidential information of
        Advanced Visual Systems Inc. and may not be used, copied or made
        available to anyone, except in accordance with the license
        under which it is furnished.

******************************************************************************

This file contains a method to create a default VLI
look-up table (LUT) for use with the VLIVolume renderer.

******************************************************************************
*****************************************************************************/

#include "express.h"

int
Utility_macros_MakeDefaultLUT::update(OMevent_mask event_mask, int seq_num)
{
   switch( LUTsize )
   {
   case 256:
      break;

   case 512:
      break;

   case 1024:
      break;

   case 2048:
      break;

   case 4096:
      break;

   default:
      printf( "Invalid LUT size.  Must be 256, 512, 1024, 2048, 4096\n" );
      return(0);
   }

   int   array_size;
   byte *red_arr   = (byte*)red.ret_array_ptr(OM_GET_ARRAY_WR, &array_size);
   byte *green_arr = (byte*)green.ret_array_ptr(OM_GET_ARRAY_WR, &array_size);
   byte *blue_arr  = (byte*)blue.ret_array_ptr(OM_GET_ARRAY_WR, &array_size);
   int  *alpha_arr = (int *)alpha.ret_array_ptr(OM_GET_ARRAY_WR, &array_size);

   if( red_arr && green_arr && blue_arr && alpha_arr )
   {
     int value = -1;   
     int multiple = LUTsize / 256;
     for( int i = 0; i < LUTsize; i++ ) 
     {
       if( i % multiple == 0 )
       {
	 value++;
       }
       red_arr[i]   = value;
       green_arr[i] = value;
       blue_arr[i]  = value;
       alpha_arr[i] = i;
     }

     ARRfree( red_arr );
     ARRfree( green_arr );
     ARRfree( blue_arr );
     ARRfree( alpha_arr );
   }

   return(1);   // return 1 for success
}



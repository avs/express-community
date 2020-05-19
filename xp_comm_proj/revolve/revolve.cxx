#include <math.h>

#define XP_WIDE_API
#include "gen.h"

/* Backward compatibility definitions */
#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif

// #define DEBUG

const float pi = 3.1415927;

int Revolve_RevolveMods_revolve::generate(OMevent_mask event_mask, int seq_num)
{
   // resolution (OMXint read req notify)
   // profile (OMXfloat_array read req notify)
   // surface (class FLD_Mesh+FLD_Quad write)
   xp_long profile_size, coord_size, normals_size;
   float *profile_arr, *coord_arr;
   xp_long *connect_arr;
   xp_long connect_size, nnodes;
   int r, band;
   int cell_set;
   int coord_idx;
   int nspace, ncell_sets;

   /***********************/
   /* Function's Body     */
   /***********************/
   if (resolution < 3) {
       ERRverror("", ERR_NO_HEADER | ERR_PRINT, 
                 "resolution must be at least 3!\n");
       return(0);
   }

   profile_arr = (float *) profile.ret_array_ptr(OM_GET_ARRAY_RD,
                                                 &profile_size);

   if (!profile_arr) {
       ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                 "revolve: couldn't get profile array!");
       return(0);
   }
   if (profile_size % 2) {
       ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                 "revolve: profile size must be even!");
       return(0);
   }


   nspace = 3;
   surface.nspace = nspace;

   ncell_sets = profile_size / 2 - 1;
   surface.ncell_sets = ncell_sets;
   if (smooth)
       nnodes = (ncell_sets + 1) * resolution;
   else
       nnodes = ncell_sets * 2 * resolution;
   surface.nnodes = nnodes;

   coord_arr =
       (float *) surface.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR,
                                                          &coord_size);
   if (!coord_arr) {
       ERRverror("", ERR_NO_HEADER | ERR_PRINT,
                 "revolve: couldn't get coordinate array!");
       ARRfree(profile_arr);
       return(0);
   }

   if (smooth)
   {
       for (band=0; band<ncell_sets+1; band++)
       {
           for (r=0; r<resolution; r++)
           {
               coord_arr[nspace * band * resolution + nspace * r + 0] =
                   profile_arr[2 * band + 0];
               coord_arr[nspace * band * resolution + nspace * r + 1] =
                   profile_arr[2 * band + 1] * sin(2 * pi * r / resolution);
               coord_arr[nspace * band * resolution + nspace * r + 2] =
                   profile_arr[2 * band + 1] * cos(2 * pi * r / resolution);
           }
       }
   }
   else
   {
       for (band=0; band<ncell_sets; band++)
       {
           for (r=0; r<resolution; r++)
           {
               coord_arr[nspace * band * 2 * resolution + nspace * r + 0] =
                   profile_arr[2 * band + 0];
               coord_arr[nspace * band * 2 * resolution + nspace * r + 1] =
                   profile_arr[2 * band + 1] * sin(2 * pi * r / resolution);
               coord_arr[nspace * band * 2 * resolution + nspace * r + 2] =
                   profile_arr[2 * band + 1] * cos(2 * pi * r / resolution);

               coord_arr[nspace * (band * 2 + 1) * resolution + nspace * r + 0] =
                   profile_arr[2 * (band + 1) + 0];
               coord_arr[nspace * (band * 2 + 1) * resolution + nspace * r + 1] =
                   profile_arr[2 * (band + 1) + 1] * sin(2 * pi * r / resolution);
               coord_arr[nspace * (band * 2 + 1) * resolution + nspace * r + 2] =
                   profile_arr[2 * (band + 1) + 1] * cos(2 * pi * r / resolution);
           }
       }
   }

   ARRfree(profile_arr);
   ARRfree(coord_arr);

   for (cell_set=0; cell_set<ncell_sets; ++cell_set)
   {
       surface.cell_set[cell_set].cell_nnodes = 4;
       surface.cell_set[cell_set].cell_ndim = 2;
       surface.cell_set[cell_set].cell_order = 1;
       surface.cell_set[cell_set].poly_flag = 0;

       surface.cell_set[cell_set].ncells = resolution;

       connect_arr =
           (xp_long *) surface.cell_set[cell_set].
           node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR, &connect_size);

       if (connect_arr)
       {
           if (smooth)
               coord_idx = cell_set;
           else
               coord_idx = cell_set * 2;

           for (r=0; r<(resolution - 1); r++)
           {
               connect_arr[4 * r + 0] = coord_idx * resolution + r;
               connect_arr[4 * r + 1] = (coord_idx + 1) * resolution + r;
               connect_arr[4 * r + 2] = (coord_idx + 1) * resolution + (r + 1);
               connect_arr[4 * r + 3] = coord_idx * resolution + (r + 1);
           }

           // Now wrap it around to the first.
           connect_arr[4 * r + 0] = coord_idx * resolution + resolution - 1;
           connect_arr[4 * r + 1] = (coord_idx + 1) * resolution + resolution - 1;
           connect_arr[4 * r + 2] = (coord_idx + 1) * resolution + 0;
           connect_arr[4 * r + 3] = coord_idx * resolution + 0;

           ARRfree(connect_arr);
       }
   }

   // return 1 for success
   return(1);
}

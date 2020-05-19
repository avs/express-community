//
//  Volume Integral Framework
//
//     Basic volume integral and surface area calculation module
//
//  Author: I. Curington, AVS Inc.
//  Revision:  31 Jan 00 - Original AVS/Express version
//

/****************************************************************
 * Derived From:                                                *
 * AVS 5 Data Output Module - computevol.c                      *
 *          Andy R. Haas                                        *
 *          R.O.W. Sciences                                     *
 *          National Center for Toxicological Research          *
 *          November 1994                                       *
 *                                                              *
 ****************************************************************/



#include "voli_gen.h"

#include "avs/gd_def.h"
#include "avs/f_utils.h"
#include "avs/math.h"
#include "avs/port.h"
#include <stdlib.h>
#include <stdio.h>

/* Backward compatibility definitions */
#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif

/************************/
/* Local Macros & Utils */
/************************/


#define DOT(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
#define MAG(a) (sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]))


double crossProd (double *a, double *b, double *r)     
{
  r[0] = a[1] * b[2] - a[2] * b[1];
  r[1] = a[2] * b[0] - a[0] * b[2];
  r[2] = a[0] * b[1] - a[1] * b[0];
  
  return MAG(r);
}


/************************/
/* Module Method Entry  */
/************************/

int
VolumeIntegral_VolumeIntegralMods_VolumeIntegralCore::update(OMevent_mask event_mask, int seq_num)
{
   // PARAMETERS
   // in (Mesh+Node_Data read req notify)
   // in.nspace (int) 
   // in.nnodes (int)
   // in.coordinates.values (float [])
   // in.ncell_sets (int)
   // in.nnodes (int)
   // in.nnode_data (int)
   // in.node_data[in_data_comp].veclen (int) 
   // in.node_data[in_data_comp].values (char [])
   // conversion (OMXfloat read req notify)
   // area (OMXdouble write)
   // volume (OMXdouble write)

   float *in_coordinates;
   int  in_data_comp;
   int  in_data_size, in_data_type;
   int  in_set;
   xp_long in_ncells;
   int  in_nspace;
   xp_long *in_node_connect_list;
   char *in_node_data;
   int in_ncell_sets;
   int in_cell_nnodes;
   int in_cell_order;
   double local_area, local_volume;
   float *xfm, *xlate;
   float raw[20];
   xp_long i, j, ncl;
   double pa[3],b[3],c[3],r[3],mag;

   /***********************/
   /* Function's Body     */
   /***********************/
   /*****
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
           "I'm in method: Volintegral_VolintegralMods_volint_core::update\n");
    *****/


   local_area = local_volume = 0.0;

   in_coordinates = (float *)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);

   in_ncell_sets = in.ncell_sets;

   in_nspace = in.nspace;

   // get the transformation for the field
   xfm   = (float*)in.xform.mat.ret_array_ptr(OM_GET_ARRAY_RD);
   xlate = (float*)in.xform.xlate.ret_array_ptr(OM_GET_ARRAY_RD);

   // error conditioning
   if ( in_nspace != 3 ) {
           ERRverror("",ERR_NO_HEADER | ERR_PRINT,
           "nspace must be 3\n");
           return(0);
   }

   for (in_set=0; in_set < in.ncell_sets; in_set++) {
   	// in.cell_set[in_set].ncells (int)
   	// in.cell_set[in_set].cell_nnodes (int)
   	// in.cell_set[in_set].cell_ndim (int)
   	// in.cell_set[in_set].cell_order (int)
   	// in.cell_set[in_set].cell_name (char *)
   	// in.cell_set[in_set].node_connect_list (int *)

   	in_node_connect_list =
            (xp_long *)in.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);

        // Process each Cell Set
   	in_ncells = in.cell_set[in_set].ncells;
   	in_cell_nnodes = in.cell_set[in_set].cell_nnodes;
   	in_cell_order = in.cell_set[in_set].cell_order;

        // error conditioning
        if ( in_ncells <= 0 ) {
           ERRverror("",ERR_NO_HEADER | ERR_PRINT,
           "no cells to process\n");
           return(0);
        }
        if ( in_cell_nnodes < 3 || in_cell_nnodes > 4 || in_cell_order != 1 ) {
           ERRverror("",ERR_NO_HEADER | ERR_PRINT,
           "some type of cell other than linear triangle or quad cell set found.");
           return(0);
        }
 
        if ( in_cell_nnodes == 3 ) // Triangle case
        {
          for (i = 0; i < in_ncells; i++) // loop over triangles
      	  {
                // first point
		ncl = (in_node_connect_list[i*3]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     pa[j] = (double)raw[j];
                }

                // second point
		ncl = (in_node_connect_list[i*3+1]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     b[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // third point
		ncl = (in_node_connect_list[i*3+2]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     c[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // compute mag depending on sign of normal
                mag = crossProd (b, c, r);

                // integrate volume
                local_volume += DOT(pa,r) / 6.0;

                // integrate area
                local_area += mag / 2.0;
          }
        }
        else // Quad case
        {
             for (i = 0; i < in_ncells; i++) // loop over quads, processing each site
             {
               // ======== FIRST triangle side of quad - 0,1,2

                // first point
		ncl = (in_node_connect_list[i*4]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     pa[j] = (double)raw[j];
                }

                // second point
		ncl = (in_node_connect_list[i*4+1]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     b[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // third point
		ncl = (in_node_connect_list[i*4+2]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     c[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // compute mag depending on sign of normal
                mag = crossProd (b, c, r);

                // integrate volume
                local_volume += DOT(pa,r) / 6.0;

                // integrate area
                local_area += mag / 2.0;

               // ======== SECOND triangle side of quad, 0,2,3

                // first point
		ncl = (in_node_connect_list[i*4+0]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     pa[j] = (double)raw[j];
                }

                // second point
		ncl = (in_node_connect_list[i*4+2]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     b[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // third point
		ncl = (in_node_connect_list[i*4+3]); //  connectivity

                for (j=0; j < 3; j++) {
                     raw[j] = in_coordinates[ncl*3+j];
                }
		MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
		VEC_ADD(raw, raw, xlate);

                for (j=0; j < 3; j++) {
                     c[j] = (double)raw[j]  - pa[j]; // compute relative vector from edge
                }

                // compute mag depending on sign of normal
                mag = crossProd (b, c, r);

                // integrate volume
                local_volume += DOT(pa,r) / 6.0;

                // integrate area
                local_area += mag / 2.0;

          }
        }

        // cleanup from list access
	if (in_node_connect_list)
           ARRfree(in_node_connect_list);
   }

   for (in_data_comp = 0; in_data_comp < in.nnode_data; in_data_comp++) { 
   	in_node_data =
                (char *)in.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD,
                         &in_data_size, &in_data_type);

        // Process each Node Data Component


        if (in_node_data)
           ARRfree(in_node_data);
   }

   // apply correction factor on volume
   local_volume *= pow((double)conversion, 3.0);
  
   // apply correction factor on area
   local_area *= pow((double)conversion, 2.0);

   //
   // write out final results to output ports
   //
   area = local_area;
   volume = local_volume;


   //
   // cleanup temp areas
   //
   if (in_coordinates)
      ARRfree(in_coordinates);

   // return 1 for success
   return(1);
}

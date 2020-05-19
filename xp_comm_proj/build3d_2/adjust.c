
#include "gen.h"


#define ERR_RETURN(A) {\
  ERRerror("AdjustSliceSpacingCore", 0, ERR_ORIG, A);\
  if (in_dims!=NULL) ARRfree(in_dims);\
  if (in_points!=NULL) ARRfree(in_points);\
  if (slice_position != NULL) ARRfree(slice_position);\
  if (out_dims!=NULL) ARRfree(out_dims);\
  if (out_points!=NULL) ARRfree(out_points);\
  return(0);\
}


int
AdjustSliceSpacingCore_update(OMobj_id AdjustSliceSpacingCore_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   OMobj_id in_id;
   int in_ndim, *in_dims=NULL, in_nspace, in_npoints;
   float *in_points=NULL;
   int  spacing_axis = 0;
   int  slice_position_size = 0;
   float *slice_position = NULL; 
   OMobj_id out_id;
   int out_ndim, *out_dims=NULL, out_nspace, out_npoints;
   float *out_points=NULL, *curr_point;
   int calc_npoints;
   int i;


   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get in uniform mesh */

   /* Get mesh id */
   in_id = OMfind_subobj(AdjustSliceSpacingCore_id, OMstr_to_name("in"), OM_OBJ_RD);

   /* Get mesh dims array */
   if (FLDget_dims (in_id, &in_dims, &in_ndim)!=OM_STAT_SUCCESS)
      ERR_RETURN("Could not get input dims array");

   if (in_ndim!=3)
      ERR_RETURN("dims array must be 3D");

   /* Get mesh nspace */
   if (FLDget_nspace (in_id, &in_nspace)!=OM_STAT_SUCCESS)
      ERR_RETURN("Could not get input nspace");

   if (in_nspace!=3)
      ERR_RETURN("nspace must be 3D");

   /* Get mesh extents */
   if (FLDget_points (in_id, &in_points, &in_npoints, OM_GET_ARRAY_RD)!=OM_STAT_SUCCESS)
      ERR_RETURN("Could not get input points array");

   if (in_npoints!=6)
      ERR_RETURN("points array must be 6 elements long");

   if (OMget_name_int_val(AdjustSliceSpacingCore_id, OMstr_to_name("spacing_axis"), &spacing_axis) != OM_STAT_SUCCESS)
      ERR_RETURN("Spacing Axis is not valid.");

   if ((spacing_axis < 0) || (spacing_axis>2))
      ERR_RETURN("Spacing Axis must be 0, 1 or 2.");

   slice_position = (float *)OMret_name_array_ptr(AdjustSliceSpacingCore_id, OMstr_to_name("slice_position"), OM_GET_ARRAY_RD, &slice_position_size, NULL);
   if (slice_position==NULL)
      ERR_RETURN("Could not get slice position array");

   if (slice_position_size != in_dims[spacing_axis])
      ERR_RETURN("Slice position array must have a value for each slice");


   /***********************/
   /* Function's Body     */
   /***********************/
   /*ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: AdjustSliceSpacingCore_update generated from method: AdjustSliceSpacingCore.update\n");*/


   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set out rectilinear mesh */

   /* Get mesh id */
   out_id = OMfind_subobj(AdjustSliceSpacingCore_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set mesh dimensionality, out_ndim can be 1,2 or 3 */
   out_ndim = 3;
   FLDset_ndim (out_id, out_ndim);

   /* Set mesh dims array */
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, out_ndim, NULL);

   out_dims[0] = in_dims[0];  out_dims[1] = in_dims[1];  out_dims[2] = in_dims[2];
   FLDset_dims (out_id, out_dims);

   /* Set mesh nspace out_nspace can be 1,2 or 3 */
   out_nspace = 3;
   FLDset_nspace (out_id, out_nspace);

   /* Set mesh extents */
   /* Now, that nspace and dims are set, just get pointer to points array */
   if (FLDget_points (out_id, &out_points, &out_npoints, OM_GET_ARRAY_RW)!=OM_STAT_SUCCESS)
      ERR_RETURN("Could not get output points array");

   calc_npoints = (out_dims[0] + out_dims[1] + out_dims[2])*out_nspace;

   if (out_npoints != calc_npoints)
      ERR_RETURN("Output points array is incorrect size");

   curr_point = out_points;


   /* Generate rectilinear points for X axis */
   if (spacing_axis==0)
   {
      /* Set X axis coordinates. Use values from slice_position array. */
      for (i=0; i<out_dims[0]; i++) {
         *(curr_point++) = slice_position[i];
         *(curr_point++) = 0;
         *(curr_point++) = 0;
      }

      /* Set Y axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[1]; i++) {
         *(curr_point++) = slice_position[0];
         *(curr_point++) = i;
         *(curr_point++) = 0;
      }

      /* Set Z axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[2]; i++) {
         *(curr_point++) = slice_position[0];
         *(curr_point++) = 0;
         *(curr_point++) = i;
      }
   }
   /* Generate rectilinear points for Y axis */
   else if (spacing_axis==1)
   {
      /* Set X axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[0]; i++) {
         *(curr_point++) = i;
         *(curr_point++) = slice_position[0];
         *(curr_point++) = 0;
      }

      /* Set Y axis coordinates. Use values from slice_position array. */
      for (i=0; i<out_dims[1]; i++) {
         *(curr_point++) = 0;
         *(curr_point++) = slice_position[i];
         *(curr_point++) = 0;
      }

      /* Set Z axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[2]; i++) {
         *(curr_point++) = 0;
         *(curr_point++) = slice_position[0];
         *(curr_point++) = i;
      }
   }
   /* Generate rectilinear points for Z axis */
   else if (spacing_axis==2)
   {
      /* Set X axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[0]; i++) {
         *(curr_point++) = i;
         *(curr_point++) = 0;
         *(curr_point++) = slice_position[0];
      }

      /* Set Y axis coordinates. Just use uniform spacing. */
      for (i=0; i<out_dims[1]; i++) {
         *(curr_point++) = 0;
         *(curr_point++) = i;
         *(curr_point++) = slice_position[0];
      }

      /* Set Z axis coordinates. Use values from slice_position array. */
      for (i=0; i<out_dims[2]; i++) {
         *(curr_point++) = 0;
         *(curr_point++) = 0;
         *(curr_point++) = slice_position[i];
      }
   }


   /*************************/
   /*  Free input variables */
   /*************************/
   if (in_dims!=NULL)
      ARRfree(in_dims);
   if (in_points!=NULL)
      ARRfree(in_points);

   if (slice_position != NULL) 
      ARRfree(slice_position);


   /**************************/
   /*  Free output variables */
   /**************************/
   if (out_dims!=NULL)
      ARRfree(out_dims);
   if (out_points!=NULL)
      ARRfree(out_points);

   return(1);
}


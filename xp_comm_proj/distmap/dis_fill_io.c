#include "gen.h"
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

int
dis_fill(OMobj_id dis_fill_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  init_mask;
   int  mask_num;
   int  tolerance;
   int  mask_size = 0;
   unsigned char *mask = NULL; 
   OMobj_id in_id;
   int in_ndim, *in_dims, in_nspace, in_npoints;
   float *in_points;
   int in_ncomp, in_comp_count, in_veclen;
   int    in_data_type, in_ndata;
   char  *in_data;
   int  init_point0_size = 0;
   int *init_point0 = NULL; 
   int  weight_size = 0;
   int *weight = NULL; 
   OMobj_id out_id;
   int out_ndim, *out_dims, out_nspace, out_npoints;
   float *out_points;
   int out_ncomp, out_comp_count, out_veclen;
   int    out_data_type, out_ndata;
   char  *out_data;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get init_mask's value */ 
   #ifdef DEBUG
  	 fprintf(stderr,"dis_fill_io called\n");fflush(stderr);
   #endif


   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("init_mask"), &init_mask) != 1) 
      init_mask = 0;

   /* Get mask_num's value */ 
   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("mask_num"), &mask_num) != 1) 
      mask_num = 0;

   /* Get tolerance's value */ 
   if (OMget_name_int_val(dis_fill_id, OMstr_to_name("tolerance"), &tolerance) != 1) 
      tolerance = 0;

   mask = (unsigned char *)OMret_name_array_ptr(dis_fill_id, OMstr_to_name("mask"), OM_GET_ARRAY_RD,
			&mask_size, NULL);

   /* Get in uniform mesh */

   /* Get mesh id */
   in_id = OMfind_subobj(dis_fill_id, OMstr_to_name("in"), OM_OBJ_RD);

   /* Get mesh dims array */
   FLDget_dims (in_id, &in_dims, &in_ndim);

   /* Get mesh nspace */
   FLDget_nspace (in_id, &in_nspace);

   /* Get mesh extents */
   FLDget_points (in_id, &in_points, &in_npoints, 
                  OM_GET_ARRAY_RD);

   /* Get in's node data */

   /* Get field id */
   in_id = OMfind_subobj(dis_fill_id, OMstr_to_name("in"), OM_OBJ_RD);

   /* Get number of node data components */
   FLDget_node_data_ncomp (in_id, &in_ncomp);

   /* For each node data component get veclen, type and data arry itself */
   for (in_comp_count=0; in_comp_count < in_ncomp; in_comp_count++) {

      /* Get veclen */
      FLDget_node_data_veclen (in_id, in_comp_count, &in_veclen);

      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

      FLDget_node_data (in_id, in_comp_count, &in_data_type, &in_data,
                        &in_ndata, OM_GET_ARRAY_RD);

      /*  Other useful calls:

          FLDget_node_data_id()
          FLDget_node_null_data()
          FLDget_node_data_minmax()
          FLDget_node_data_label()
      */
      if (in_data)
         ARRfree(in_data);
   }
   

   init_point0 = (int *)OMret_name_array_ptr(dis_fill_id, OMstr_to_name("init_point0"), OM_GET_ARRAY_RD,
			&init_point0_size, NULL);

   weight = (int *)OMret_name_array_ptr(dis_fill_id, OMstr_to_name("weight"), OM_GET_ARRAY_RD,
			&weight_size, NULL);

   /* Get out uniform mesh */

   /* Get mesh id */
   out_id = OMfind_subobj(dis_fill_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Get mesh dims array */
   FLDget_dims (out_id, &out_dims, &out_ndim);

   /* Get mesh nspace */
   FLDget_nspace (out_id, &out_nspace);

   /* Get mesh extents */
   FLDget_points (out_id, &out_points, &out_npoints, 
                  OM_GET_ARRAY_RD);

   /* Get out's node data */

   /* Get field id */
   out_id = OMfind_subobj(dis_fill_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Get number of node data components */
   FLDget_node_data_ncomp (out_id, &out_ncomp);

   /* For each node data component get veclen, type and data arry itself */
   for (out_comp_count=0; out_comp_count < out_ncomp; out_comp_count++) {

      /* Get veclen */
      FLDget_node_data_veclen (out_id, out_comp_count, &out_veclen);

      /* Get data array and data_type which is one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE */

      FLDget_node_data (out_id, out_comp_count, &out_data_type, &out_data,
                        &out_ndata, OM_GET_ARRAY_RD);

      /*  Other useful calls:

          FLDget_node_data_id()
          FLDget_node_null_data()
          FLDget_node_data_minmax()
          FLDget_node_data_label()
      */
      if (out_data)
         ARRfree(out_data);
   }
   


   /***********************/
   /* Function's Body     */
   /***********************/
   ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: dis_fill generated from method: dis_fill.dis_fill\n");


   /***********************/
   /*  Set output values  */
   /***********************/
   /* Set init_mask's value */  
   OMset_name_int_val(dis_fill_id, OMstr_to_name("init_mask"), init_mask);
    /* Set out uniform mesh */

   /*  Get mesh id */
   out_id = OMfind_subobj(dis_fill_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set mesh dimensionality, out_ndim can be 1,2 or 3 */
   FLDset_ndim (out_id, out_ndim);

   /* Set mesh dims array */
   out_dims = (int *)ARRalloc(NULL, DTYPE_INT, 
                           out_ndim, NULL);

   /*** fill in dims array with your values ***/

   FLDset_dims (out_id, out_dims);
   if (out_dims)
      ARRfree(out_dims);

   /* Set mesh nspace, out_nspace can be 1,2 or 3 */
   FLDset_nspace (out_id, out_nspace);

   /* Set mesh extents */
   out_npoints = 2*out_nspace;
   out_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 
                               out_npoints, NULL);

   /*** fill in points array with values for 2 points: low left and high right corners of the mesh ***/

   FLDset_points (out_id, out_points, out_npoints, 
                  OM_SET_ARRAY_FREE);
   /* set  out node data */

   /* Get field id */
   out_id = OMfind_subobj(dis_fill_id, OMstr_to_name("out"), OM_OBJ_RW);

   /* Set number of node data components */
   FLDset_node_data_ncomp (out_id, out_ncomp);

   /* For each node data component set veclen, type and data arry itself */
   for (out_comp_count=0; out_comp_count < out_ncomp; out_comp_count++) {

      /* Set veclen, assign out_veclen before next call */
      FLDset_node_data_veclen (out_id,out_comp_count,out_veclen);

      /* Set data array */
      /* data_type should be set to one of the following: 
         DTYPE_BYTE, DTYPE_CHAR, DTYPE_SHORT, 
         DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE) */

      out_data_type = DTYPE_CHAR;

      /* allocate out_data array first */
      /* assume float array and out_ndata is set to number of nodes */

      out_data = (char *)ARRalloc(NULL, out_data_type, 
                                 out_veclen*out_ndata, NULL);

      FLDset_node_data (out_id, out_comp_count, out_data, out_data_type,
                        out_ndata*out_veclen, OM_SET_ARRAY_FREE);

      /*  Other useful calls:

          FLDset_node_data_id()
          FLDset_node_null_data()
          FLDset_node_data_label()
      */
   }
   

   /*************************/
   /*  Free input variables */
   /*************************/
   if (mask != NULL) 
      ARRfree(mask);

   if (in_dims)
      ARRfree(in_dims);
   if (in_points)
      ARRfree(in_points);

   if (init_point0 != NULL) 
      ARRfree(init_point0);

   if (weight != NULL) 
      ARRfree(weight);

   if (out_dims)
      ARRfree(out_dims);
   if (out_points)
      ARRfree(out_points);

   return(1);
}

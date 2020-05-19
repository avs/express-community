//
// Dmap2Color Module
//
//  generates arrays of RGB based on colors based on Datamap
//
//  Author: Ian Curington, Advanced Visual Systems, Inc.
//  Date:   21 January 2001 original


#include "d2c_gen.hxx"

#include <avs/gd.h>
#include <avs/dmap.h>
#include <avs/pal.h>
#include <avs/om.h>




   /***********************/
   /* Utility Functions   */
   /***********************/

// utility to resolve dmap
// variation on internal utility in GD kit to resolve datamaps, modified here

void ResolveDmap2Colors(OMobj_id id,  // Dmap
                float *values,        // data values to sample
                int size,             // size of color array
                float *colors) // color array
{
    OMobj_id ptr_id;
    Dmap *map;

    ptr_id = OMfind_subobj(id, OMstr_to_name("localPtr"), OM_OBJ_RW);
    if (!OMis_null_obj(ptr_id)) {
       if (OMget_ptr_val(ptr_id, (void **)&map, 0)) {
          map->get_colors(map, (char*)values, colors, OM_TYPE_FLOAT,size);
       }
    }
    return;
} // end of Dmap2Colors


   /******************
    *  ModuleCode    *
    ******************/

int
Isonest_IsonestMods_Dmap2Color::update(OMevent_mask event_mask, int seq_num)
{

   // input (OMXfloat_array read)
   int values_size;
   float *values_arr = NULL;

   // output (OMXfloat_array write)
   int output_size;
   float *output_arr = NULL;

   // dmap (DatamapTempl read notify)

   // output_dims (OMXint_array write)
   int output_dims_size;
   int *output_dims_arr = NULL;

   int i, x_res;

   unsigned int *lut = NULL;

   /******************
    *  Start Code    *
    ******************/

   x_res = X_Resolution;

   /***********************/
   /* Function's Body     */
   /***********************/
   // ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: update\n");

   // must set output dims first to dimension data arrays
   output_dims_arr = (int *)output_dims.ret_array_ptr(OM_GET_ARRAY_WR,&output_dims_size);
   if ((output_dims_arr==NULL) || (output_dims_size!=1)) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't get output_dims_arr.\n");

      ARRfree(output_dims_arr);
      return(0);
   }

   // establish output dimensions 1D array
   output_dims_arr[0] = x_res;

   // freeing this array triggers Express to allocate memory for image
   ARRfree(output_dims_arr);

   // set output dims arr
   // then get a pointer to the output array

   output_arr = (float *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if ((output_arr==NULL) || (output_size!=(3*x_res)) ) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get output_arr.\n");

      ARRfree(output_arr);
      return (0);
   }

   // get input array of float values for where to sample DataMap

   values_arr = (float *)values.ret_array_ptr(OM_GET_ARRAY_RD,&values_size);
   if ((values_arr==NULL) || (values_size==0))
   {
	   ARRfree(output_arr);
	   return (0);
   }
   else if ((values_arr==NULL) || (values_size!=x_res) ) {
	   ERRverror("",ERR_NO_HEADER | ERR_PRINT, "Can't get values_arr.\n");

      ARRfree(output_arr);
      ARRfree(values_arr);
      return (0);
   }

   // calculate look-up-table  from datamap
   // (not really a full lookup table, only contains values sampled
   //  as requested)

   lut = (unsigned int *) malloc (sizeof(unsigned int) * x_res);

   OMobj_id id;
   id = OMfind_subobj(this->obj_id(),OMstr_to_name("dmap"),OM_OBJ_RD);

   ResolveDmap2Colors(id, values_arr, x_res, output_arr);

   // close and release the output image for further processing
   ARRfree(output_arr);
   ARRfree(values_arr);
   free (lut);

   // return 1 for success
   return(1);
}
// end of file


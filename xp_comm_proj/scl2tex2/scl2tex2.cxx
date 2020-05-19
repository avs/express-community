
#include <math.h>

#include <avs/gd_def.h>
#include "gen.hxx"


int
Scalar2Texture2_Scalar2Texture2Mods_Scalar2Texture2Core::update(OMevent_mask event_mask, int seq_num)
{

   // in (Node_Data read req notify)
   // in.nnodes (int)
   // in.nnode_data (int)
   // in.node_data[].veclen (int) 
   // in.node_data[].values (char [])
   float *in_node_data = NULL;
   int in_size;
   float in_min, in_max, in_width;

   // out (Node_Data write)
   // out.nnodes (int)
   // out.nnode_data (int)
   // out.node_data[].veclen (int) 
   // out.node_data[].values (char [])
   float *out_node_data = NULL;
   int out_size, out_type;

   int i;
   float norm_val;

   //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Scalar2Texture_Scl2TexMods_Scalar2TextureCore::update\n");


   out.nnodes = 0;
   out.nnode_data = 0;

   if ((!in.nnodes.valid_obj()) || (in.nnodes <= 0)) {
      return(0);
   }
   out.nnodes = in.nnodes;

   if ((!in.nnode_data.valid_obj()) || (in.nnode_data <= 0)) {
      ERRverror("Scalar2TextureCore", ERR_ERROR, "No node data components.\n");
   }
   else {
      int in_scalar_nd = -1;

      for (int i=0; i<in.nnode_data; i++) {
         if (in.node_data[i].veclen == 1) {
            in_scalar_nd = i;
            break;
         }
      }

      if (in_scalar_nd == -1) {
         ERRverror("Scalar2Texture2Core", ERR_ERROR, "Input node data must be scalar.\n");
      }
      else {
         in_node_data = (float *)in.node_data[in_scalar_nd].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_FLOAT, &in_size);
         if (in_node_data == NULL) {
            ERRverror("Scalar2Texture2Core", ERR_ERROR, "Could not get input node data array.\n");
         }
         if (in_size != in.nnodes) {
            ERRverror("Scalar2Texture2Core", ERR_ERROR, "Input node data array is incorrect size.\n");
         }

         in_min = (float)in.node_data[in_scalar_nd].min;
         in_max = (float)in.node_data[in_scalar_nd].max;
         in_width = in_max - in_min;
      }
   }



   out.nnode_data = 1;
   out.node_data[0].id = GD_UV_DATA_ID;
   if (FLDset_node_data_comp( (OMobj_id)out, 0, 2, "texture coords", "unit range") != OM_STAT_SUCCESS) {
      ERRverror("Scalar2Texture2Core", ERR_ERROR, "Could not set output node data properties.\n");
      return(0);
   }
   if (FLDset_node_data_type( (OMobj_id)out, 0, OM_TYPE_FLOAT) != OM_STAT_SUCCESS) {
      ERRverror("Scalar2Texture2Core", ERR_ERROR, "Could not set output node data array to float type.\n");
      return(0);
   }

   out_node_data = (float *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_size, &out_type);
   if (out_node_data == NULL) {
      ERRverror("Scalar2Texture2Core", ERR_ERROR, "Could not get output node data array.\n");
      return(0);
   }
   if (out_size != (out.nnodes*2)) {
      ERRverror("Scalar2Texture2Core", ERR_ERROR, "Output node data array is incorrect size.\n");
      return(0);
   }
   if (out_type != OM_TYPE_FLOAT) {
      ERRverror("Scalar2Texture2Core", ERR_ERROR, "Output node data array is incorrect type.\n");
      return(0);
   }


   /***********************/
   /* Function's Body     */
   /***********************/

   if ((in_node_data==NULL) || (in_width == 0.0)) {
      for (i=0; i < out_size; i++) {
         out_node_data[i] = 1.0;
      }
   }
   else {
      for (i=0; i < in_size; i++) {
         norm_val = (in_node_data[i] - in_min) / in_width;
         out_node_data[i*2 + 0] = norm_val;
         out_node_data[i*2 + 1] = norm_val;
      }
   }

   if (in_node_data!=NULL)  ARRfree( (char*)in_node_data );
   if (out_node_data!=NULL) ARRfree( (char*)out_node_data );
   

   // return 1 for success
   return(1);
}


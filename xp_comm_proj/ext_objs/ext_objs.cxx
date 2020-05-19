
#include "xp_comm_proj/ext_objs/gen.hxx"


int
ExtractObjects_ExtractObjectsMods_ExtractObjectsCore::update(OMevent_mask event_mask, int seq_num)
{
   // in_objs (Link_array read notify)
   int in_objs_size;
   
   // inc_objs (OMXint_array read notify)
   int inc_objs_size;
   int *inc_objs_arr = NULL;

   // out_objs (Link_array write)

   /***********************/
   /* Function's Body     */
   /***********************/
   //ERRverror("", ERR_NO_HEADER | ERR_PRINT,"I'm in method: ExtractObjects_ExtractObjectsMods_ExtractObjectsCore::update\n");

   // Reset output links
   if ( OMset_obj_ref(out_objs, OMnull_obj, 0) != OM_STAT_SUCCESS) {
      ERRverror("ExtractObjectsCore", ERR_PRINT, "Could not reset out_objs array.\n");
      return 0;
   }


   inc_objs_arr = (int *)inc_objs.ret_array_ptr(OM_GET_ARRAY_RD, &inc_objs_size);

   // Check that we've got inc_objs array
   // If not just return silently.
   if ((inc_objs_arr==NULL) || (inc_objs_size==0)) {
      //ERRverror("ExtractObjectsCore", ERR_PRINT, "Could not get inc_objs array.\n");
      if (inc_objs_arr!=NULL) ARRfree(inc_objs_arr);
      return 1;
   }

   // Get in_objs size
   OMget_array_size(in_objs, &in_objs_size);

   // Check that input array is not zero sized
   if (in_objs_size==0) {
      ERRverror("ExtractObjectsCore", ERR_PRINT, "in_objs array should not be zero sized.\n");
      ARRfree(inc_objs_arr);
      return 0;
   }


   // We've got our list of fields, now build out_links array

   ERRsquash_start();

   for (int i=0; i<inc_objs_size; i++) {
      OMobj_id curr_in_obj;
      int curr_inc = inc_objs_arr[i];
      int status;

      if ((curr_inc>=0) && (curr_inc<in_objs_size)) {
         if (OMget_array_val(in_objs, curr_inc, &curr_in_obj, OM_OBJ_RD) != OM_STAT_SUCCESS) {
            ERRsquash_end();
            ERRverror("ExtractObjectsCore", ERR_PRINT, "Could not get in_objs[%d] value. Is object of primitive type?\n", curr_inc);
            ARRfree(inc_objs_arr);
            return 0;
         }
         if (OMadd_obj_ref(out_objs, curr_in_obj, 0) != OM_STAT_SUCCESS) {
            ERRsquash_end();
            ERRverror("ExtractObjectsCore", ERR_PRINT, "Could not add reference to out_objs.\n");
            ARRfree(inc_objs_arr);
            return 0;
         }
      }
   }

   ERRsquash_end();

   // Free arrays
   ARRfree(inc_objs_arr);

   // return 1 for success
   return 1;
}


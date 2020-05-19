
/* swap group order
  given an array of groups, make a new list in reverse order*/


#include "gswap_gen.h"

/*******************
 * Module Entry    *
 *******************/

int
group_swap_order(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  int  in_ints_size, dims[1];
  int i, j;
  OMobj_id   in_group_id, referencing_object, referenced_object;
  int status;

  /*
   * handle to input list
   */
  in_group_id = OMfind_subobj(obj_id,
                              OMstr_to_name("in_groups"),
                              OM_OBJ_RD);
  if (OMis_null_obj(in_group_id)) {
      ERRverror("",ERR_NO_HEADER | ERR_INFO,"Error searching for in_groups\n");
      return 0;
  }

  /*
   * get array length
   */
  OMget_array_dims(in_group_id, &in_ints_size, dims);
  j = dims[0];

  if ( j <= 0 ) {
      ERRverror("",ERR_NO_HEADER | ERR_INFO,"input array of groups is zero length\n");
      return(0);
  }

  /*
   * handle to output list
   */
  referencing_object = OMfind_subobj(obj_id,
                                     OMstr_to_name("out_groups"),
                                     OM_OBJ_RW);
  if (OMis_null_obj(referencing_object)) {
      ERRverror("",ERR_NO_HEADER | ERR_INFO,"Error searching for referencing object\n");
      return 0;
  }

  /*
   * reset, clean out the output array
   */
  OMset_obj_ref(referencing_object, OMnull_obj, 0);


  /*
   * loop over values, set output values in opposite order
   */
  for(i=j-1; i >= 0; i--){
    /*
     * get input reference starting at the end
     */
    OMget_array_val(in_group_id, i, &referenced_object, OM_OBJ_RD);

    /*
     * set output, adding in forward order
     */
    OMadd_obj_ref(referencing_object, referenced_object, 0);
  }

   return(1);
}
/* end of file */

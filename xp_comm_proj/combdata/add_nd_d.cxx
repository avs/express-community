/*
** Module: DVadd_node_data
** Author: Mark Lambert
** Date: 3/11/99
** Description: 
**    Module used add a node data collection to an existing collection.
*/


#include "xp_comm_proj/combdata/gen.hxx"


int
CombineData_CombineDataMods_DVadd_node_data::update(OMevent_mask event_mask, int seq_num)
{
   // in_fld (Mesh read req)
   // in_nd (Node_Data read req notify)

   // new_nd (Node_Data write)

   /***********************/
   /* Function's Body     */
   /***********************/
   int i;
   // empty current node data collection
   new_nd.node_data.set_obj_ref(OMnull_obj);

   // add original node data to collection
   for (i= 0; i< in_fld_nnodedata; i++){
	   new_nd.node_data.add_obj_ref(in_fld.node_data[i].obj_id());
   }

   // add second node data elements to collection
   for (i= 0; i< in_nd_nnodedata; i++){
	   new_nd.node_data.add_obj_ref(in_nd.node_data[i].obj_id());
   }

   // return 1 for success
   return(1);
}


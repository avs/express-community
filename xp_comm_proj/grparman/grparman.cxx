// by James S Perrin  Time-stamp: <Friday 31/08/01 16:55:51 zzcgujp>
// grparman.cxx
// add, replace, delete, swap control for an array referencing a set of groups

#include "xp_comm_proj/grparman/gam_gen.h"
#include <time.h>

int GroupArrMan_group_array_manager::add_group(OMevent_mask event_mask, int seq_num)
{
  // in (Mesh read req)
  // farm (group)
  // nGroups (OMXint read write)
  // out (Mesh_array write)
  char newGroupName[256];
  OMobj_id inGroupID, newGroupID, farmID;
  
  OMget_obj_val(in.obj_id(), &inGroupID);
  OMget_obj_val(farm.obj_id(), &farmID);
  // copy_obj seems to causes a crash id parent is within module ?
  sprintf(newGroupName, "Group%d_%d", (long)time(NULL), (int)nGroups);
  newGroupID = OMcopy_obj(inGroupID, OMstr_to_name(newGroupName), farmID,OMobj_proc(this->obj_id()), 0);
  OMset_obj_val(newGroupID, inGroupID, 0);

  if(!nGroups.valid_obj()) nGroups = 0;
  out.add_obj_ref(newGroupID, 0);
  return 1;
}


int GroupArrMan_group_array_manager::replace_group(OMevent_mask event_mask, int seq_num)
{
  // in (Mesh read req)

  // selected (OMXint read req)
  // nGroups (OMXint read write)
  // out (group_array write)
  char newGroupName[256];
  OMobj_id inGroupID, newGroupID, oldGroupID, farmID;
  
  // if group array is empty call add method
  if((int) nGroups == 0)
  {
    add_group(event_mask, seq_num);
    return 1;
  }
  
  OMget_obj_val(in.obj_id(), &inGroupID);
  OMget_obj_val(farm.obj_id(), &farmID);
  sprintf(newGroupName, "Group%d_%d", (long)time(NULL), (int)nGroups);
  newGroupID = OMcopy_obj(inGroupID, OMstr_to_name(newGroupName), farmID, OMobj_proc(this->obj_id()), 0);
  OMset_obj_val(newGroupID, inGroupID, 0);

  OMget_obj_val(out[selected].obj_id(), &oldGroupID);
  out[selected].set_obj_ref(newGroupID, 0);
  // delete replaced group
  OMdestroy_obj(oldGroupID, 0);
  
  return 1;
}


int GroupArrMan_group_array_manager::swap_group(OMevent_mask event_mask, int seq_num)
{
  // selected (OMXint read req)
  // other (OMXint read req)
  // nGroups (OMXint read write)
  // out (group_array read write)
  OMobj_id selGroupID, othGroupID;

  // need at least 2 groups to swap
  if((int) nGroups < 2) return 1;
  // ignore swapping with self
  if((int)selected == (int)other) return 1;

  OMget_obj_val(out[selected].obj_id(), &selGroupID);
  OMget_obj_val(out[other].obj_id(), &othGroupID);
  out[selected].set_obj_ref(othGroupID, 0);
  out[other].set_obj_ref(selGroupID, 0);
  
  return 1;
}


int GroupArrMan_group_array_manager::delete_group(OMevent_mask event_mask, int seq_num)
{
  // selected (OMXint read req)
  // nGroups (OMXint read write)
  // out (group_array read write)
  int i, j;
  OMobj_id selGroupID, othGroupID;

  // just shuffle refs down 
  // finally decrease ref array size
  // delete obj
  
  if((int) nGroups == 0) return 1;
  
  OMget_obj_val(out[selected].obj_id(), &selGroupID);
  
  for(i=selected, j=selected+1; j<(int)nGroups; i++, j++)
  {
    OMget_obj_val(out[j].obj_id(), &othGroupID);
    out[i].set_obj_ref(othGroupID, 0);
  }
  
  out[i].set_obj_ref(OMnull_obj, 0);
  OMdestroy_obj(selGroupID, 0);
  
  nGroups = (int)nGroups -1;
  return 1;
}

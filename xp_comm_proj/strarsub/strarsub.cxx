// strarsub.cxx
// by James S. Perrin Time-stamp: <Wednesday 05/09/01 16:48:07 zzcgujp>

#include "xp_comm_proj/strarsub/sas_gen.h"

int StrArrSub_string_array_subset::copy(OMevent_mask event_mask, int seq_num)
{
  // in (OMXstr_array read req notify)
  // start (OMXint read req notify)
  // end (OMXint read req notify)
  // out (OMXstr_array write)
  char *buff;
  int i, size;
  
  size = in.ret_array_size()-1;


  if((int)start>size || (int)end>size || (int)start>(int)end)
    return 1;

  // clear old array
  out.set_obj_ref(OMnull_obj);
  
  for(i=start; i<=end; i++)
  {
    buff = in.ret_str_array_val(i, NULL, 0);
    out.set_str_array_val(i-start, buff);
    free(buff);
  }

  return 1;
}

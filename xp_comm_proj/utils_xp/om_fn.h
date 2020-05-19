

#ifndef OM_FN_H

#define OM_FN_H


#include <fld/Xfld.h>


int return_sizeof_type
  (int type);

void type_to_string
  (int type, char **type_pp);

int return_array_length
  (OMobj_id array_id);

OMobj_id return_parent_id
  (OMobj_id object_id);

OMobj_id return_subobj_id
  (OMobj_id parent_id, char *subobj_str_p, int mode);

OMobj_id return_element_id
  (OMobj_id array_id, int ith_element, int mode);

int return_subobj_int_val
  (OMobj_id parent_id, char *subobj_str_p);

void set_subobj_int_val
  (OMobj_id parent_id, char *subobj_str_p, int val);


#endif

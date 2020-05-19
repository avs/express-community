

#ifndef ARRAY_FN_H

#define ARRAY_FN_H


#include <fld/Xfld.h>


int array_is_data_array
  (OMobj_id array_id);

int array_is_connectivity_array
  (OMobj_id array_id);

void array_label
  (OMobj_id array_id, char **label_pp, int mode);

int array_length
  (OMobj_id array_id);

int array_type
  (OMobj_id array_id);

int array_size
  (OMobj_id array_id);

void array_ptr
  (OMobj_id array_id, void **array_pp, int mode);

void array_get_copy
  (OMobj_id array_id, char **array_pp);

void array_set_copy
  (OMobj_id array_id, char *array_p, int size, int type);


#endif


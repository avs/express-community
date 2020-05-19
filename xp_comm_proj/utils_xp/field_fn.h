

#ifndef FIELD_FN_H

#define FIELD_FN_H


#include <fld/Xfld.h>


int this_is_a_field
  (OMobj_id input_obj_id);

int this_is_a_valid_field
  (OMobj_id input_obj_id);

int this_is_poly_connectivity
  (OMobj_id cell_set_id);

int n_grid_arrays
  (OMobj_id input_obj_id);

int n_node_data_arrays
  (OMobj_id input_obj_id);

int n_cell_sets
  (OMobj_id input_obj_id);

int n_cell_data_arrays_ith_set
  (OMobj_id input_obj_id, int i);

int n_cell_data_arrays
  (OMobj_id input_obj_id);

int n_connectivity_arrays
  (OMobj_id input_obj_id);

int n_arrays
  (OMobj_id input_obj_id);

OMobj_id grid_parent_id
  (OMobj_id input_obj_id, int mode);

OMobj_id ith_node_data_parent_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_cell_data_parent_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_connectivity_parent_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_array_parent_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id grid_id
  (OMobj_id input_obj_id, int mode);

OMobj_id ith_node_data_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_cell_data_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_connectivity_id
  (OMobj_id input_obj_id, int i, int mode);

OMobj_id ith_array_id
  (OMobj_id input_obj_id, int i, int mode);


#endif


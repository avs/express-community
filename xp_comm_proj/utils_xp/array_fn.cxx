

#include <fld/Xfld.h>
#include "array_fn.h"
#include "om_fn.h"


/* -------------------------------------------------------------------------- */

int array_is_data_array
  (OMobj_id array_id)
/*
  array_id is not part of a data array if it is a null object, if its parent is
  a null object, if it is not a values object, or if its parent object does not
  contain all the following subobjects : nvals, veclen, id, min, max, min_vec,
  max_vec, labels, units.
*/
{
  OMobj_id parent_id;

  if (OMis_null_obj (array_id))
    return (0);
  parent_id = return_parent_id (array_id);
  if (OMis_null_obj (parent_id))
    return (0);
  if (!OMequal_objs(OMfind_str_subobj(parent_id,"values",OM_OBJ_RD),array_id))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"nvals",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"veclen",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"id",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"min",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"max",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"min_vec",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"max_vec",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"labels",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"units",OM_OBJ_RD)))
    return (0);
  return (1);
}

/* -------------------------------------------------------------------------- */

int array_is_connectivity_array
  (OMobj_id array_id)
/*
  array_id is not a connectivity array if it is a null object, if its parent is
  a null object, if it is not a node_connect_list object, or if its parent
  object does not contain all the following subobjects : ncells, cell_ndim,
  cell_nnodes, cell_corner_nnodes, cell_order, poly_flag.
*/
{
  OMobj_id parent_id;

  if (OMis_null_obj (array_id))
    return (0);
  parent_id = return_parent_id (array_id);
  if (OMis_null_obj (parent_id))
    return (0);
  if (!OMequal_objs(OMfind_str_subobj(parent_id,"node_connect_list",OM_OBJ_RD),
   array_id))
    return(0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"ncells",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"cell_ndim",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"cell_nnodes",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"cell_corner_nnodes",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"cell_order",OM_OBJ_RD)))
    return (0);
  if (OMis_null_obj(OMfind_str_subobj(parent_id,"poly_flag",OM_OBJ_RD)))
    return (0);
  return (1);
}

/* -------------------------------------------------------------------------- */

void array_label
  (OMobj_id array_id, char **label_pp, int mode)
/*
  If the array is part of a Data_Array get its label, else return NULL. 
*/
{
  OMobj_id parent_id, labels_id;

  *label_pp = NULL;
  parent_id = return_parent_id (array_id);
  if (!OMis_null_obj(parent_id)) {
    labels_id = OMfind_str_subobj (parent_id, "labels", mode);
    if (!OMis_null_obj(labels_id))
      OMget_str_val (labels_id, label_pp, 0);
  }
}

/* -------------------------------------------------------------------------- */

int array_length
  (OMobj_id array_id)
/*
  Return the length of the array (i.e. how many elements).
*/
{
  return (return_array_length (array_id));
}

/* -------------------------------------------------------------------------- */

int array_type
  (OMobj_id array_id)
/*
  Return the type of the array.
*/
{
  int type;

  if (OMis_null_obj(array_id))
    type = OM_TYPE_UNSET;
  else
    OMget_data_type (array_id, &type);
  return (type);
}

/* -------------------------------------------------------------------------- */

int array_size
  (OMobj_id array_id)
/*
  Return the size of the array in bytes.
*/
{
  return (return_sizeof_type(array_type(array_id))*array_length(array_id));
}

/* -------------------------------------------------------------------------- */

void array_ptr
  (OMobj_id array_id, void **array_pp, int mode)
/*
  Get a pointer to the array. Must ARRfree this pointer after use.
  This function is just a wrapper for a call to OMret_array_ptr, which in turn
  is a wrapper for OMget_array. 
  A pointer to the object manager's copy of the array will be returned if this
  function is used by a module compiled in the Express process, otherwise a copy
  of the array will be created, and a pointer to that returned.
  See the documentation for OMget_array.
*/
{
  int size, type;

  if (return_array_length(array_id)==0)
    *array_pp = NULL;
  else
    *array_pp = OMret_array_ptr (array_id, mode, &size, &type);
}

/* -------------------------------------------------------------------------- */

void array_get_copy
  (OMobj_id array_id, char **array_pp)
/*
  Copy an Express array to a C array.
  No need to ARRfree the copy, just free() it.
*/
{
  int type, ndim, dims[OM_ARRAY_MAXDIM], status;

  *array_pp = NULL;
  if (OMis_null_obj (array_id))
    return;
  if (return_array_length(array_id)==0)
    return;
  *array_pp = (char *) malloc (array_size(array_id));
  if (*array_pp == NULL) {
    fprintf (stderr, "  array_get_copy : out of memory.\n");
    return;
  }
  type = OM_TYPE_UNSET;
  ndim = 0;
  status = OMget_array (array_id, &type, array_pp, &ndim, dims, 
   OM_GET_ARRAY_RD_COPY);
  if (status == OM_STAT_ERROR)
    fprintf (stderr, "  array_get_copy : couldn't get array.\n");
}

/* -------------------------------------------------------------------------- */

void array_set_copy
  (OMobj_id array_id, char *array_p, int size, int type)
/*
  Copy a C array to an Express array.
  The Express object must be an array which has already been set to the
  correct size.
*/
{
  int type_size, status;

  if (OMis_null_obj (array_id))
    return;
  if (size==0)
    return;
  type_size = return_sizeof_type(type);
  if (type_size == 0) {
    fprintf (stderr, "  array_set_copy : type unknown.\n");
    return;
  }
  if (array_size(array_id)!=size) {
    fprintf (stderr, "  array_set_copy : express array wrong size.\n");
    return;
  }
  status = OMset_array (array_id, type, array_p, size/type_size, 
   OM_SET_ARRAY_COPY);
  if (status == OM_STAT_ERROR)
    fprintf (stderr, "  array_set_copy : couldn't put array.\n");
}

/* -------------------------------------------------------------------------- */



#include <fld/Xfld.h>
#include <stdlib.h>
#include <string.h>
#include "om_fn.h"


/* -------------------------------------------------------------------------- */

int return_sizeof_type
  (int type)
/*
  Return the size of the given data type.
  OM_TYPE_PTR, OM_TYPE_STRING, OM_TYPE_UNSET are not handled.
*/
{
  switch (type) {
    case OM_TYPE_BYTE :
      return (1);
    case OM_TYPE_CHAR :
      return (sizeof(char));
    case OM_TYPE_SHORT :
      return (sizeof(short));
    case OM_TYPE_INT :
      return (sizeof(int));
    case OM_TYPE_FLOAT :
      return (sizeof(float));
    case OM_TYPE_DOUBLE :
      return (sizeof(double));
    default :
      return (0);
  }
}

/* -------------------------------------------------------------------------- */

void type_to_string
  (int type, char **type_pp)
/*
  Return the type as a string which corresponds to the given type integer.
  OM_TYPE_PTR, OM_TYPE_STRING, OM_TYPE_UNSET are not handled.
*/
{
  switch (type) {
    case OM_TYPE_BYTE :
      *type_pp = (char *) malloc (5);
      strcpy (*type_pp, "byte");
      break;
    case OM_TYPE_CHAR :
      *type_pp = (char *) malloc (5);
      strcpy (*type_pp, "char");
      break;
    case OM_TYPE_SHORT :
      *type_pp = (char *) malloc (6);
      strcpy (*type_pp, "short");
      break;
    case OM_TYPE_INT :
      *type_pp = (char *) malloc (4);
      strcpy (*type_pp, "int");
      break;
    case OM_TYPE_FLOAT :
      *type_pp = (char *) malloc (6);
      strcpy (*type_pp, "float");
      break;
    case OM_TYPE_DOUBLE :
      *type_pp = (char *) malloc (7);
      strcpy (*type_pp, "double");
      break;
    default :
      *type_pp = NULL;
      break;
  }
}

/* -------------------------------------------------------------------------- */

int return_array_length
  (OMobj_id array_id)
/*
  Return the length of the given array. 
  Note that arrays that have no values in them can have non-zero lengths.
  Scalars and null objects return 0 length.
*/
{
  int ndims, length;

  if (OMis_null_obj(array_id))                                     //   null obj
    return (0);
  else {
    OMget_array_dims (array_id, &ndims, NULL);
    if (ndims==0)                                                  // scalar obj
      return (0);
    else {                                                         //      array
      length=0;
      OMget_array_size (array_id, &length);
      return (length);
    }
  }
}

/* -------------------------------------------------------------------------- */

OMobj_id return_parent_id
  (OMobj_id object_id)
/*
  Return the object id of the parent of the given object. The id of the actual
  subobject is returned (i.e. if it is a reference, the referenced object is 
  returned, not the id of the local template or link object which references it).
*/
{
  OMobj_id parent_id;

  if (OMis_null_obj(object_id))
    return (OMnull_obj);
  else {
    OMget_obj_parent (object_id, &parent_id);
    if (OMis_null_obj(parent_id))
      return (OMnull_obj);
    else {
      OMget_obj_val (parent_id, &parent_id);
      return (parent_id);
    }
  }
}

/* -------------------------------------------------------------------------- */

OMobj_id return_subobj_id
  (OMobj_id parent_id, char *subobj_str_p, int mode)
/*
  Return the object id of the named subobject of given parent object. The id of
  the actual subobject is returned (i.e. if it is a reference, the referenced
  object is returned, not the id of the local template or link object which 
  references it).
  `mode' may be OM_OBJ_RD or OM_OBJ_RW.
*/
{
  OMobj_id   subobj_id;
  OMobj_name subobj_name;

  if (OMis_null_obj(parent_id))
    return (OMnull_obj);
  else {
    subobj_name = OMstr_to_name (subobj_str_p);
    subobj_id =  OMfind_subobj (parent_id, subobj_name, mode);
    if (OMis_null_obj(subobj_id))
      return (OMnull_obj);
    else {
      OMget_obj_val (subobj_id, &subobj_id);
      return (subobj_id);
    }
  }
}

/* -------------------------------------------------------------------------- */

OMobj_id return_element_id
  (OMobj_id array_id, int ith_element, int mode)
/*
  Return the object id of the ith element of the given array. The id of the 
  actual element is returned (i.e. if it is a reference, the referenced object 
  is returned, not the id of the local template or link object which references
  it.
*/
{
  OMobj_id element_id;

  if (OMis_null_obj(array_id))
    return (OMnull_obj);
  else {
    OMget_array_val (array_id, ith_element, &element_id, mode);
    if (OMis_null_obj(element_id))
      return (OMnull_obj);
    else {
      OMget_obj_val (element_id, &element_id);
      return (element_id);
    }
  }
}

/* -------------------------------------------------------------------------- */

int return_subobj_int_val
  (OMobj_id parent_id, char *subobj_str_p)
/*
  Return the integer value of the named subobject of given parent object. 
*/
{
  int      val;
  OMobj_id subobj_id;

  subobj_id = return_subobj_id (parent_id, subobj_str_p, OM_OBJ_RD);
  if (OMis_null_obj(subobj_id))
    return (0);
  else
    OMget_int_val (subobj_id, &val);
  return (val);
}


/* -------------------------------------------------------------------------- */

void set_subobj_int_val
  (OMobj_id parent_id, char *subobj_str_p, int val)
/*
  Set the integer value of the named subobject of given parent object.
  Calls return_subobj_id, so follows back up to the real obj.
*/
{
  OMobj_id subobj_id;

  subobj_id = return_subobj_id (parent_id, subobj_str_p, OM_OBJ_RW);
  if (!OMis_null_obj(subobj_id))
    OMset_int_val (subobj_id, val);
}

/* -------------------------------------------------------------------------- */

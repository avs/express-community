

/*

  Copyright / Disclaimer :

  This software/documentation was produced as part of the INDEX project
  (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
  the European Community. For further details see
  http://www.man.ac.uk/MVC/research/INDEX/Public/. 

  Copyright (c) June 1998, Manchester Visualisation Centre, UK. All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its documentation
  is hereby granted without fee, provided that the above copyright notice and this
  permission notice appear in all copies of this software/documentation.

  This software/documentation is provided with no warranty, express or implied, 
  including, without limitation, warrant of merchantability or fitness for a 
  particular purpose.

*/


/*
  AVS/Express compression tool.

  Amardeep S Bhattal.
  February 1998.
  a.bhattal@mcc.ac.uk

  Compresses/uncompresses any array in input, at source.

  The following diagram shows the process involved, each object in [] is an array,
  with the following properties.

  E     - Express array
  C     - C language array

  u     - uncompressed array
  c     - compressed array
  f     - footer information

  ?     - any type
  char  - character type
  Bytef - Zlib Bytef type
  int   - integer type

  Compression :

  [E/u/?] -> [C/u/char] -> [C/c/Bytef] -> [C/c/int] -> [C/c+f/int] -> [E/c+f/int]
          1             2              3            4              5

  1 - OMget_array() in array_get_copy(), copies out of Express to C char array.
  2 - Zlib fns called from uncompr_to_compr() via deflate_fn module, type 
      conversion by cast.
  3 - type conversion by malloc int*, and memcpy in align_to_int() (necessary
      because int's cannot just be tacked onto the end of char arrays, see K&R
      section A6.6).
  4 - realloc in add_footer().
  5 - OMset_array().

  Uncompression :

  reverse of above, using same numbering but in opposite direction,

  5 - OMget_array() in array_get_copy(), copies out of Express to C char array.
  4 - realloc in remove_footer().
  3 - not necessary, jump to 2.
  2 - Zlib fns called from compr_to_uncompr() via deflate_fn module, type 
      conversion by cast.
  1 - OMset_array().

  These processes are called from different fns depending on whether compression
  or uncompression is taking place, and whether the input is a data array, node
  connectivity array or poly connectivity array. Altering the size of these arrays
  is different for each type of array because of the interaction with the rest of
  the Express field data structure (arrays may or may not be sized by ints which
  may or may not reference other ints).

*/


#include <avs/om.h>                                                     /* !!! */
#include <avs/om_att.h>                                                 /* !!! */
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "out_hdr.h"


#define VERBOSE


void uncompr_to_compr
  (Bytef **array_pp, int *size_p);

void compr_to_uncompr
  (Bytef **array_pp, int *size_p);

void align_to_int
  (void **array_pp, int *size_p);

void add_footer
  (int **array_pp, int *size_p, int i1, int i2, int i3, int i4);

void remove_footer
  (int **array_pp, int *size_p, int *i1_p, int *i2_p, int *i3_p, int *i4_p);

void compress_data_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void uncompress_data_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void compress_node_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void uncompress_node_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void compress_poly_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void uncompress_poly_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p);

void process_array
  (OMobj_id array_id);


/* -------------------------------------------------------------------------- */

void uncompr_to_compr
  (Bytef **array_pp, int *size_p)
{
  int   compr_size;
  Bytef *compr_array_p;

  compr_array_p = NULL;
  compr_size = 0;
  deflate_array (*array_pp, *size_p, &compr_array_p, &compr_size,
   Z_DEFAULT_COMPRESSION);
  if (*array_pp != NULL)
    free (*array_pp);
  *array_pp = compr_array_p;
  *size_p = compr_size;
}

/* -------------------------------------------------------------------------- */

void compr_to_uncompr
  (Bytef **array_pp, int *size_p)
{
  int   uncompr_size;
  Bytef *uncompr_array_p;

  uncompr_array_p = NULL;
  uncompr_size = 0;
  inflate_array (*array_pp, *size_p, &uncompr_array_p, &uncompr_size);
  if (*array_pp != NULL)
    free (*array_pp);
  *array_pp = uncompr_array_p;
  *size_p = uncompr_size;
}

/* -------------------------------------------------------------------------- */

void align_to_int
  (void **array_pp, int *size_p)
{
  int *array_p, size, length;

  length = ((*size_p)+sizeof(int)-1)/sizeof(int);
  size = length*sizeof(int);
  array_p = (int *) malloc (size);
  if (array_p == NULL) {
    cerr << " align_to_int : out of memory." << endl;
    exit (1);
  }
  memcpy ((char *)array_p, (char *)*array_pp, *size_p);
  if (*array_pp != NULL)
    free (*array_pp);
  *array_pp = array_p;
  *size_p = size;
}

/* -------------------------------------------------------------------------- */

void add_footer
  (int **array_pp, int *size_p, int i1, int i2, int i3, int i4)
{
  int *array_p, size, length;

  size = (*size_p) + 4*sizeof(int);
  array_p = (int *) realloc ((void *)*array_pp, size);
  if (array_p==NULL) {
    cerr << "  add_footer : out of memory." << endl;
    exit (1);
  }
  length = size/sizeof(int);
  array_p[length-4] = i1;
  array_p[length-3] = i2;
  array_p[length-2] = i3;
  array_p[length-1] = i4;
  *array_pp = array_p;
  *size_p = size;
}

/* -------------------------------------------------------------------------- */

void remove_footer
  (int **array_pp, int *size_p, int *i1_p, int *i2_p, int *i3_p, int *i4_p)
{
  int *array_p, size, length;

  length = *size_p/sizeof(int);
  *i1_p = (*array_pp)[length-4];
  *i2_p = (*array_pp)[length-3];
  *i3_p = (*array_pp)[length-2];
  *i4_p = (*array_pp)[length-1];
  size = (*size_p) - 4*sizeof(int);
  array_p = (int *) realloc ((void *)*array_pp, size);
  if (array_p==NULL) {
    cerr << "  remove_footer : out of memory." << endl;
    exit (1);
  }
  *array_pp = array_p;
  *size_p = size;
}

/* -------------------------------------------------------------------------- */

void compress_data_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, compr_size, type, nvals, veclen;
  OMobj_id parent_id;

  parent_id  = return_parent_id (array_id);
  size       = array_size (array_id);
  type       = array_type (array_id);
  nvals      = return_subobj_int_val (parent_id, "nvals");
  veclen     = return_subobj_int_val (parent_id, "veclen");
  *in_size_p = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  uncompr_to_compr ((Bytef **)&array_p, &size);
  compr_size = size;
  align_to_int (&array_p, &size);
  add_footer ((int **)&array_p, &size, compr_size, type, nvals, veclen);
  *out_size_p = size;

  OMset_obj_ref(OMfind_str_subobj(parent_id,"nvals", OM_OBJ_RW), OMnull_obj, 0);
  OMset_obj_ref(OMfind_str_subobj(parent_id,"veclen",OM_OBJ_RW), OMnull_obj, 0);
  set_subobj_int_val (parent_id, "nvals", size/sizeof(int));
  set_subobj_int_val (parent_id, "veclen", 1);
  OMparse_buffer (parent_id, "prim values[nvals][veclen];", 0);
  array_id = return_subobj_id (parent_id, "values", OM_OBJ_RW);
  OMset_data_type (array_id, OM_TYPE_INT);
  array_set_copy (array_id, (char *)array_p, size, OM_TYPE_INT);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void uncompress_data_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, compr_size, type, nvals, veclen, nvals2, veclen2, type_size;
  OMobj_id parent_id, nnodes_id, ncells_id, nspace_id;

  parent_id  = return_parent_id (array_id);
  size       = array_size (array_id);
  *in_size_p = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  remove_footer ((int **)&array_p, &size, &compr_size, &type, &nvals, 
   &veclen);
  size = compr_size;
  compr_to_uncompr ((Bytef **)&array_p, &size);
  *out_size_p = size;  

  nnodes_id = OMfind_obj (parent_id, OMstr_to_name("nnodes"), (OM_OBJ_RD ||
   OM_FIND_NO_LIBRARIES));
  ncells_id = OMfind_obj (parent_id, OMstr_to_name("ncells"), (OM_OBJ_RD || 
   OM_FIND_NO_LIBRARIES));
  if (!OMis_null_obj (ncells_id))
    OMset_obj_ref (return_subobj_id(parent_id,"nvals",OM_OBJ_RW), ncells_id, 0);
  else if (!OMis_null_obj (nnodes_id))
    OMset_obj_ref (return_subobj_id(parent_id,"nvals",OM_OBJ_RW), nnodes_id, 0);
  else
    OMset_int_val (return_subobj_id(parent_id,"nvals",OM_OBJ_RW), 0);
  OMget_int_val (return_subobj_id (parent_id, "nvals",  OM_OBJ_RD), &nvals2);

  nspace_id = OMfind_subobj (return_parent_id (parent_id), 
   OMstr_to_name("nspace"), OM_OBJ_RD);
  if (!OMis_null_obj (nspace_id))
    OMset_obj_ref (return_subobj_id(parent_id,"veclen",OM_OBJ_RW), nspace_id, 0);
  else
    OMset_int_val (return_subobj_id(parent_id,"veclen",OM_OBJ_RW), veclen);
  OMget_int_val (return_subobj_id (parent_id, "veclen", OM_OBJ_RD), &veclen2);

  if ((nvals2 != nvals) || (veclen2 != veclen)) {
    cerr << " uncompress_data_array : nvals or veclen mis-match, " << nvals;
    cerr << " != " << nvals2 << ", " << veclen << " != " << veclen2 << endl;
    exit (1);
  }
  type_size = return_sizeof_type (type);
  if (size != nvals*veclen*type_size) {
    cerr << "  uncompress_data_array : array size incorrect, " << size;
    cerr << " != " << nvals << "*" << veclen << "*" << type_size << endl;
    exit (1);
  }
  OMparse_buffer (parent_id, "prim values[nvals][veclen];", 0);
  array_id = return_subobj_id (parent_id, "values", OM_OBJ_RW);
  OMset_data_type (array_id, type);
  array_set_copy (array_id, (char *)array_p, size, type);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void compress_node_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, compr_size, type, ncells, cell_nnodes;
  OMobj_id parent_id;

  parent_id   = return_parent_id (array_id);
  size        = array_size (array_id);
  type        = array_type (array_id);
  ncells      = return_subobj_int_val (parent_id, "ncells");
  cell_nnodes = return_subobj_int_val (parent_id, "cell_nnodes");
  *in_size_p  = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  uncompr_to_compr ((Bytef **)&array_p, &size);
  compr_size = size;
  align_to_int (&array_p, &size);
  add_footer ((int **)&array_p, &size, compr_size, type, ncells, cell_nnodes);
  *out_size_p = size;

  set_subobj_int_val (parent_id, "ncells", size/sizeof(int));
  set_subobj_int_val (parent_id, "cell_nnodes", 1);
  OMparse_buffer (parent_id, "int node_connect_list[ncells*cell_nnodes];", 0);
  array_id = return_subobj_id (parent_id, "node_connect_list", OM_OBJ_RW);
  OMset_array (array_id, OM_TYPE_INT, array_p, size/sizeof(int), OM_SET_ARRAY_COPY);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void uncompress_node_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, compr_size, type, ncells, cell_nnodes;
  OMobj_id parent_id;

  parent_id  = return_parent_id (array_id);
  size       = array_size (array_id);
  *in_size_p = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  remove_footer ((int **)&array_p, &size, &compr_size, &type, &ncells, 
   &cell_nnodes);
  size = compr_size;
  compr_to_uncompr ((Bytef **)&array_p, &size);
  *out_size_p = size;  

  if (size != ncells*cell_nnodes*sizeof(int)) {
    cerr << "  uncompress_node_connectivity_array : ";
    cerr << size << " != " << ncells << "*" << cell_nnodes << "*";
    cerr << sizeof(int) << endl;
    exit (1);
  }
  set_subobj_int_val (parent_id, "ncells", ncells);
  set_subobj_int_val (parent_id, "cell_nnodes", cell_nnodes);
  OMparse_buffer (parent_id, "int node_connect_list[ncells*cell_nnodes];", 0);
  array_id = return_subobj_id (parent_id, "node_connect_list", OM_OBJ_RW);
  OMset_array (array_id, OM_TYPE_INT, array_p, size/sizeof(int), OM_SET_ARRAY_COPY);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void compress_poly_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, type, compr_size;
  OMobj_id parent_id;

  parent_id  = return_parent_id (array_id);
  size       = array_size (array_id);
  type       = array_type (array_id);
  *in_size_p = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  uncompr_to_compr ((Bytef **)&array_p, &size);
  compr_size = size;
  align_to_int (&array_p, &size);
  add_footer ((int **)&array_p, &size, compr_size, type, 0, 0);
  *out_size_p = size;

  OMparse_buffer (parent_id, "int poly_connect_list[];", 0);
  array_id = return_subobj_id (parent_id, "poly_connect_list", OM_OBJ_RW);
  OMset_array_size (array_id, size/sizeof(int));
  OMset_array (array_id, OM_TYPE_INT, array_p, size/sizeof(int), OM_SET_ARRAY_COPY);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void uncompress_poly_connectivity_array
  (OMobj_id array_id, int *in_size_p, int *out_size_p)
{
  void     *array_p;
  int      size, type, compr_size, dummy1, dummy2;
  OMobj_id parent_id;

  parent_id  = return_parent_id (array_id);
  size       = array_size (array_id);
  *in_size_p = size;

  array_get_copy (array_id, (char **)&array_p);
  OMset_obj_iprop (parent_id, OM_prop_locked, 0);
  OMuser_destroy_obj (array_id);

  remove_footer ((int **)&array_p, &size, &compr_size, &type, &dummy1, &dummy2);
  size = compr_size;
  compr_to_uncompr ((Bytef **)&array_p, &size);
  *out_size_p = size;  

  OMparse_buffer (parent_id, "int poly_connect_list[];", 0);
  array_id = return_subobj_id (parent_id, "poly_connect_list", OM_OBJ_RW);
  OMset_array_size (array_id, size/sizeof(int));
  OMset_array (array_id, OM_TYPE_INT, array_p, size/sizeof(int), OM_SET_ARRAY_COPY);
  free (array_p);
}

/* -------------------------------------------------------------------------- */

void process_array
  (OMobj_id array_id)
/*
  Compresses the given array if it is not compressed. If it is compressed it is
  uncompressed.
  Picks the correct compress or uncompress function for the given array.
*/
{
  char *array_p;
  int  is_deflated, is_data_array, is_poly_connectivity_array, size, in_size,
       out_size;

  // initialisation.
  in_size = out_size = 0;

  // figure out what we have.
  size = array_size (array_id);
  is_data_array = array_is_data_array (array_id);
  is_poly_connectivity_array = 
   this_is_poly_connectivity (return_parent_id(array_id));
  array_ptr (array_id, (void **)&array_p, OM_GET_ARRAY_RD);
  if (size < 8+2)                // compress tool footer size + zlib header size
    is_deflated = 0;
  else
    is_deflated = deflated_array ((Bytef *)array_p);
  if (array_p != NULL)
    ARRfree (array_p);

  #ifdef VERBOSE
    cout << "  input is ";
    if (size > 0) {
      if (is_deflated)
        cout << "compressed ";
      else
        cout << "uncompressed ";
      if (is_data_array)
        cout << "data array ";
      else if (is_poly_connectivity_array)
        cout << "poly connectivity ";
      else
        cout << "connectivity ";
    }
    else
      cout << "zero size obj ";
    cout << endl;
  #endif

  // do the right thing.
  if (size > 0) {
    if (is_deflated) {
      if (is_data_array)
        uncompress_data_array (array_id, &in_size, &out_size);
      else if (is_poly_connectivity_array)
        uncompress_poly_connectivity_array (array_id, &in_size, &out_size);
      else
        uncompress_node_connectivity_array (array_id, &in_size, &out_size);
    }
    else {
      if (is_data_array)
        compress_data_array (array_id, &in_size, &out_size);
      else if (is_poly_connectivity_array)
        compress_poly_connectivity_array (array_id, &in_size, &out_size);
      else
        compress_node_connectivity_array (array_id, &in_size, &out_size);
    }
  }
  else
    cerr << "  process_array : warning, not array or zero size array." << endl;

  #ifdef VERBOSE
    if (in_size > 0) {
      cout << "  " << in_size << " bytes in, " << out_size << " bytes out, ";
      cout << (100.0*out_size/in_size) << "%." << endl << endl;
    }
  #endif
}

/* -------------------------------------------------------------------------- */

int
Compress_Compress_Prim::compress_all_arrays(OMevent_mask event_mask, int seq_num)
/*
  Method called when compress_all int changes.
  Resets compress_all int to zero.
*/
{
  char     *array_p;
  int      i, size, is_deflated;
  OMobj_id field_id, array_id;

  field_id = (OMobj_id)data;
  for (i=0; i<n_arrays(field_id); i++) {
    array_id = ith_array_id (field_id, i, OM_OBJ_RD);
    size = array_size (array_id);
    array_ptr (array_id, (void **)&array_p, OM_GET_ARRAY_RD);
    if (size < 8+2)
      is_deflated = 0;
    else
      is_deflated = deflated_array ((Bytef *)array_p);
    if (array_p != NULL)
      ARRfree (array_p);
    if (!is_deflated)
      process_array (array_id);
  }
  compress_all = 0;
  return (1);
}

/* -------------------------------------------------------------------------- */

int
Compress_Compress_Prim::uncompress_all_arrays(OMevent_mask event_mask, int seq_num)
/*
  Method called when uncompress_all int changes.
  Resets uncompress_all int to zero.
*/
{
  char     *array_p;
  int      i, size, is_deflated;
  OMobj_id field_id, array_id;

  field_id = (OMobj_id)data;
  for (i=0; i<n_arrays(field_id); i++) {
    array_id = ith_array_id (field_id, i, OM_OBJ_RD);
    size = array_size (array_id);
    array_ptr (array_id, (void **)&array_p, OM_GET_ARRAY_RD);
    if (size < 8+2)
      is_deflated = 0;
    else
      is_deflated = deflated_array ((Bytef *)array_p);
    if (array_p != NULL)
      ARRfree (array_p);
    if (is_deflated)
      process_array (array_id);
  }
  uncompress_all = 0;
  return (1);
}

/* -------------------------------------------------------------------------- */

int
Compress_Compress_Prim::process_nth_array(OMevent_mask event_mask, int seq_num)
/*
  Method called when process_nth int changes.
  Resets process_nth to zero, leaves nth as it was.
*/
{
  OMobj_id field_id, array_id;

  field_id = (OMobj_id)data;
  // !!!
  // printf ("  process_nth_array :      nth is %d.\n", (int)nth);
  // printf ("  process_nth_array :    nth-1 is %d.\n", nth-1);
  // printf ("  process_nth_array : n_arrays is %d.\n", n_arrays(field_id));
  // fflush (stdout);
  // !!!
  if ((nth>0) && (nth<=n_arrays(field_id))) {
    array_id = ith_array_id (field_id, nth-1, OM_OBJ_RD);
    if (OMis_null_obj(array_id))
      cerr << "  compress, process_nth_array : error, can't find array " <<
       nth << "." << endl;
    else
      process_array (array_id);
  }
  else
    cerr << "  compress, process_nth_array : error, invalid array number " <<
     nth << "." <<  endl;
  process_nth = 0;
  return (1);
}

/* -------------------------------------------------------------------------- */

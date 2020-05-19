

#include <fld/Xfld.h>
#include "array_fn.h"
#include "field_fn.h"
#include "om_fn.h"


/* -------------------------------------------------------------------------- */

int this_is_a_field
  (OMobj_id input_obj_id)
/*
  Returns 1 if the input object contains `a field', 0 otherwise. The following
  three sub-objects must be present : 
  "nnodes", "nspace" and  "coordinates.values".
*/
{
  OMobj_id id;

  id = return_subobj_id (input_obj_id, "nnodes", OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  id = return_subobj_id (input_obj_id, "nspace", OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  id = return_subobj_id (input_obj_id, "coordinates", OM_OBJ_RD);
  id = return_subobj_id (id,           "values",      OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  return (1);
}

/* -------------------------------------------------------------------------- */

int this_is_a_valid_field
  (OMobj_id input_obj_id)
/*
  Returns 1 if the input object contains `a valid field', 0 otherwise. The 
  following three sub-objects must be present, and have a value : 
  "nnodes", "nspace" and  "coordinates.values".
*/
{
  OMobj_id id;

  id = return_subobj_id (input_obj_id, "nnodes", OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  else if (!OMvalid_obj(id, OMnull_obj, 0))
    return (0);
  id = return_subobj_id (input_obj_id, "nspace", OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  else if (!OMvalid_obj(id, OMnull_obj, 0))
    return (0);
  id = return_subobj_id (input_obj_id, "coordinates", OM_OBJ_RD);
  id = return_subobj_id (id,           "values",      OM_OBJ_RD);
  if (OMis_null_obj(id))
    return (0);
  else if (!OMvalid_obj(id, OMnull_obj, 0))
    return (0);
  return (1);
}

/* -------------------------------------------------------------------------- */

int this_is_poly_connectivity
  (OMobj_id cell_set_id)
/*
  Returns 1 if the input object contains a poly_connect_list with length 
  greater than 0.
*/
{
  OMobj_id poly_connect_id;

  poly_connect_id = return_subobj_id(cell_set_id,"poly_connect_list",OM_OBJ_RD);
  return (array_length(poly_connect_id) > 0);
}

/* -------------------------------------------------------------------------- */

int n_grid_arrays
  (OMobj_id input_obj_id)
/*
  Return the number of arrays which hold grid information in the given object.

  The co-ordinate data of the grid may not be present (i.e. is implicit), this
  is determined by examining int grid_type.

  grid type                      coords      int grid_type

  (unstructured) grid            explicit    absent
  (irregular) structured grid    explicit    unset => 0        
  rectilinear grid               implicit    2
  uniform grid                   implicit    1

  Note : getting an unset value returns 0, getting the value of a NULL object
         leaves the value unchanged.

  input_obj.coordinates.values[][], return 1 if values is present, 0 otherwise.
*/
{
  int      grid_type, status;
  OMobj_id grid_type_id;

  if (!this_is_a_valid_field(input_obj_id))
    return (0);
  grid_type_id = return_subobj_id (input_obj_id, "grid_type", OM_OBJ_RD);
  if (OMis_null_obj(grid_type_id))
    return (1);
  status = OMget_int_val (grid_type_id, &grid_type);
  if (status == OM_STAT_UNDEF)
    return (1);
  return (0);
}

/* -------------------------------------------------------------------------- */

int n_node_data_arrays
  (OMobj_id input_obj_id)
/*
  Return the number of arrays which hold node data information in the given
  object.

  Each grid position or node may have data associated with it. There may be 
  more than one data component at each node - e.g. two components such as 
  velocity (vector) and temperature (scalar) data. Each component is held in a
  separate values array.

  input_obj.node_data[n].values[][], find n.
*/
{
  int      ncomponents;
  OMobj_id node_data_id;

  node_data_id = return_subobj_id (input_obj_id, "node_data", OM_OBJ_RD);
  ncomponents = return_array_length (node_data_id);
  return (ncomponents);
}

/* -------------------------------------------------------------------------- */

int n_cell_sets
  (OMobj_id input_obj_id)
/*
  Return the number of groups of cells in the given object.

  The cells in a mesh are grouped into different geometric types. Cells of the 
  same geometric type are placed into a cell set, which describes the cells. 
  Usually there is one cell set per geometric type present.

  input_obj.cell_set[n]..., find n.
*/
{
  int      ncell_sets;
  OMobj_id cell_set_id;

  cell_set_id = return_subobj_id (input_obj_id, "cell_set", OM_OBJ_RD);
  ncell_sets = return_array_length (cell_set_id);
  return (ncell_sets);
}

/* -------------------------------------------------------------------------- */

int n_cell_data_arrays_ith_set
  (OMobj_id input_obj_id, int i)
/*
  Return the number of arrays which hold cell data information in the ith cell
  set of the given object.

  input_obj.cell_set[i].cell_data[n].values[][], find n, given i.
*/
{
  int      ncell_data;
  OMobj_id cell_set_id, ith_cell_set_id, cell_data_id;

  cell_set_id = return_subobj_id (input_obj_id, "cell_set", OM_OBJ_RD);
  ith_cell_set_id = return_element_id (cell_set_id, i, OM_OBJ_RD);
  cell_data_id = return_subobj_id (ith_cell_set_id, "cell_data", OM_OBJ_RD);
  ncell_data = return_array_length (cell_data_id);
  return (ncell_data);
}

/* -------------------------------------------------------------------------- */

int n_cell_data_arrays
  (OMobj_id input_obj_id)
/*
  Return the number of arrays which hold cell data information in all the cell
  sets of the given object.

  Each cell in a mesh may have data associated with the cell as a whole. So,
  the cells in each cell set may have associated cell data (just as nodes may
  have associated node data).

  input_obj.cell_set[i].cell_data[n].values[][], sum n over all i.
*/
{
  int i, ncell_data;

  for (ncell_data=0, i=0; i<n_cell_sets(input_obj_id); i++)
    ncell_data += n_cell_data_arrays_ith_set (input_obj_id, i);
  return (ncell_data);
}

/* -------------------------------------------------------------------------- */

int n_connectivity_arrays
  (OMobj_id input_obj_id)
/*
  Return the number of arrays which hold cell connectivity information in the 
  input object.

  Each cell in a mesh is connected to other cells; they share nodes on adjacent
  faces. The node_connect_list is an array that describes which nodes belong to
  which cells. There is one connectivity list for each cell set present.

  input_obj.cell_set[n].node_connect_list[], find n.
*/
{
  int      ncell_sets;
  OMobj_id cell_set_id;

  cell_set_id = return_subobj_id (input_obj_id, "cell_set", OM_OBJ_RD);
  ncell_sets = return_array_length (cell_set_id);
  return (ncell_sets);
}

/* -------------------------------------------------------------------------- */

int n_arrays
  (OMobj_id input_obj_id)
/*
  Return the number of arrays used to hold field information in the given 
  object.
*/
{
  return (
          n_grid_arrays (input_obj_id)         + 
          n_node_data_arrays (input_obj_id)    +
          n_cell_data_arrays (input_obj_id)    +
          n_connectivity_arrays (input_obj_id)
         );
}

/* -------------------------------------------------------------------------- */

OMobj_id grid_parent_id
  (OMobj_id input_obj_id, int mode)
/*
  Look for input_obj.coordinates
*/
{
  OMobj_id coords_id;

  coords_id = return_subobj_id (input_obj_id, "coordinates", mode);
  return (coords_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_node_data_parent_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Look for input_obj.node_data[i]
*/
{
  OMobj_id node_data_id, component_id;

  node_data_id = return_subobj_id (input_obj_id, "node_data", mode);
  component_id = return_element_id (node_data_id, i, mode);
  return (component_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_cell_data_parent_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Look for input_obj.cell_set[j].cell_data[k], 
  where i+1 = n(0) + n(1) + ... n(j-1) + k,
  given that n(x) is the number of cell_data arrays in the xth cell set.
*/
{
  int      j, k, n_so_far;
  OMobj_id cell_set_id, jth_cell_set_id, cell_data_id, kth_cell_data_id;

  i++; // get rid of 0 indexing, so i=1 means the first array
  if (i > n_cell_data_arrays(input_obj_id))
    return (OMnull_obj);
  n_so_far=0;
  j=0;
  while (n_so_far+n_cell_data_arrays_ith_set(input_obj_id, j) < i) {
    n_so_far += n_cell_data_arrays_ith_set(input_obj_id, j);
    j++;
  }
  k = i - n_so_far;
  k--; // back to 0 indexing, so k=0 means the first array in a cell set
  cell_set_id = return_subobj_id (input_obj_id, "cell_set", mode);
  jth_cell_set_id = return_element_id (cell_set_id, j, mode);
  cell_data_id = return_subobj_id (jth_cell_set_id, "cell_data", mode);
  kth_cell_data_id = return_element_id (cell_data_id, k, mode);
  return (kth_cell_data_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_connectivity_parent_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Look for input_obj.cell_set[i]
*/
{
  OMobj_id cell_set_id, ith_cell_set_id;

  cell_set_id = return_subobj_id (input_obj_id, "cell_set", mode);
  ith_cell_set_id = return_element_id (cell_set_id, i, mode);
  return (ith_cell_set_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_array_parent_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Return the parent of the i'th array in the field. The arrays are ordered in 
  the following manner : grid, node data, cell data and cell connectivity. i=0
  will result in the first array's parent being returned.
*/
{
  if (i < 0)
    return (OMnull_obj);
  if (i < n_grid_arrays(input_obj_id))
    return (grid_parent_id(input_obj_id, mode));
  else
    i -= n_grid_arrays(input_obj_id);
  if (i < n_node_data_arrays(input_obj_id))
    return (ith_node_data_parent_id(input_obj_id, i, mode));
  else
    i -= n_node_data_arrays(input_obj_id);
  if (i < n_cell_data_arrays(input_obj_id))
    return (ith_cell_data_parent_id(input_obj_id, i, mode));
  else
    i -= n_cell_data_arrays(input_obj_id);
  if (i < n_connectivity_arrays(input_obj_id))
    return (ith_connectivity_parent_id(input_obj_id, i, mode));
  else 
    i -= n_connectivity_arrays(input_obj_id);
  return (OMnull_obj);
}

/* -------------------------------------------------------------------------- */

OMobj_id grid_id
  (OMobj_id input_obj_id, int mode)
/*
  Look for input_obj.coordinates.values[][]
*/
{
  OMobj_id coords_id, values_id;

  coords_id = grid_parent_id (input_obj_id, mode);
  values_id = return_subobj_id (coords_id, "values", mode);
  return (values_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_node_data_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Look for input_obj.node_data[i].values[][]
*/
{
  OMobj_id component_id, values_id;

  component_id = ith_node_data_parent_id (input_obj_id, i, mode);
  values_id = return_subobj_id (component_id, "values", mode);
  return (values_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_cell_data_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Look for input_obj.cell_set[j].cell_data[k].values[][], 
  where i+1 = n(0) + n(1) + ... n(j-1) + k,
  given that n(x) is the number of cell_data arrays in the xth cell set.
*/
{
  OMobj_id kth_cell_data_id, values_id;

  kth_cell_data_id = ith_cell_data_parent_id (input_obj_id, i, mode);
  values_id = return_subobj_id (kth_cell_data_id, "values", mode);
  return (values_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_connectivity_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  If there is a non-zero length
   input_obj.cell_set[i].poly_connect_list[]
  then return that, else return
   input_obj.cell_set[i].node_connect_list[]
  (because the node list references the poly list if a poly list exists).
*/
{
  OMobj_id ith_cell_set_id, poly_connect_id, node_connect_id;

  ith_cell_set_id = ith_connectivity_parent_id (input_obj_id, i, mode);
  poly_connect_id = return_subobj_id (ith_cell_set_id,"poly_connect_list",mode);
  if (array_length(poly_connect_id) > 0)
    return (poly_connect_id);
  node_connect_id = return_subobj_id(ith_cell_set_id,"node_connect_list",mode);
  return (node_connect_id);
}

/* -------------------------------------------------------------------------- */

OMobj_id ith_array_id
  (OMobj_id input_obj_id, int i, int mode)
/*
  Return the i'th array in the field. The arrays are ordered in the following 
  manner : grid, node data, cell data and cell connectivity. i=0 will result
  in the first array being returned.
*/
{
  if (i < 0)
    return (OMnull_obj);
  if (i < n_grid_arrays(input_obj_id))
    return (grid_id(input_obj_id, mode));
  else
    i -= n_grid_arrays(input_obj_id);
  if (i < n_node_data_arrays(input_obj_id))
    return (ith_node_data_id(input_obj_id, i, mode));
  else
    i -= n_node_data_arrays(input_obj_id);
  if (i < n_cell_data_arrays(input_obj_id))
    return (ith_cell_data_id(input_obj_id, i, mode));
  else
    i -= n_cell_data_arrays(input_obj_id);
  if (i < n_connectivity_arrays(input_obj_id))
    return (ith_connectivity_id(input_obj_id, i, mode));
  else 
    i -= n_connectivity_arrays(input_obj_id);
  return (OMnull_obj);
}

/* -------------------------------------------------------------------------- */

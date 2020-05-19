
/*------------------------------------------------------------------------------
 *
 *    TITLE: data_field.c
 *  CREATED: 28/08/97
 * MODIFIED: 23/04/98
 *   AUTHOR: Chris Pudney (cpudney@alphapharm.pharm.uwa.edu.au)
 *           Biomedical Confocal Microscopy Research Centre
 *           Department of Pharmacology
 *           The University of Western Australia
 *           Nedlands, WA 6907, Australia
 *
 * Copyright (C) Chris Pudney, The University of Western Australia, 1997.
 * All rights reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation only for the purposes of teaching and research is hereby
 * granted without fee, provided that the above copyright notice and this
 * permission notice appear in all copies of this software/documentation and
 * that you do not sell the software.  No commercial use or distribution
 * of the software is permitted without the consent of the copyright
 * owners.  Commercial licensing is available by contacting the author(s).
 *
 * THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
 * IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* IAC Note: You may Change the following line if you don't have the FFTw
   project to either ../FFTn/gen.h or ../nstats/gen.h - this is so it will
   pick up the process of that project */
#include "../FFTw/gen.h"

#include "data_field.h"


/*-DATcreate_field--------------------------------------------------------------
 *  Description: creates a FIELD structure and assigns values
 *   Parameters: id                object ID
 *               ndim              number of data dimensions
 *               dims[ndim]        array of data dimensions
 *               nspace            spatial dimensions
 *               points[2][nspace] array of points
 *               ncomp             number of components
 *               veclen[ncomp]     array of vector lengths
 *               data_type[ncomp]  array of data types
 *               data[ncomp][prod(dims)*veclen[ncomp]]  data array
 * Return value: pointer to the FIELD or NULL if an error occurs.
 */

FIELD *
DATcreate_field(OMobj_id id, int ndim, int *dims, int nspace,
		float *points, int ncomp, int *veclen, int *data_type,
		void **data)
{
  int i, prodDims;
  FIELD *field;


  /* Allocate memory */
  field = (FIELD *)malloc(sizeof(FIELD));
  if (!field)
    {
      ERRverror("DATcreate_field", ERR_INFO, "Could not allocate field.");
      return (FIELD *)NULL;
    }


  /* Initialize pointer values */
  field->dims = field->veclen = field->data_type = (int *)NULL;
  field->data = (void **)NULL;
  field->points = (float *)NULL;


  /* Set field values */
  field->id = id;
  field->ndim = ndim;
  field->nspace = nspace;
  field->ncomp = ncomp;


  /* Allocate space for each component */
  field->dims = (int *)ARRalloc(NULL, DTYPE_INT, field->ndim, NULL);
  field->points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 2 * field->nspace, NULL);
  field->veclen = (int *)ARRalloc(NULL, DTYPE_INT, field->ncomp, NULL);
  field->data_type = (int *)ARRalloc(NULL, DTYPE_INT, field->ncomp, NULL);
  field->data = (void **)ARRalloc(NULL, DTYPE_PTR, field->ncomp, NULL);
  if (!field->dims ||
      !field->points ||
      !field->veclen ||
      !field->data_type ||
      !field->data)
    {
      ERRverror
	("DATcreate_field", ERR_INFO, "Can't allocate component arrays.");
      DATfree_field(field);
      return (FIELD *)NULL;
    }


  /* Assign dimensions */
  if (dims)
    {
      memcpy(field->dims, dims, sizeof(int) * field->ndim);
    }
  else
    {
      for (i = 0;
	   i < field->ndim;
	   i++)
	{
	  field->dims[i] = DEFAULT_DIM;
	}
    }

  /* Compute product of dimensions */
  prodDims = 1;
  for (i = 0;
       i < field->ndim;
       i++)
    {
      prodDims *= field->dims[i];
    }
  field->ndata = prodDims;


  /* Assign points */
  if (points)
    {
      memcpy(field->points, points, 2 * sizeof(float) * field->nspace);
    }
  else
    {
      for (i = 0;
	   i < nspace;
	   i++)
	{
	  field->points[i] = 0.;
	  field->points[i + nspace] = 1.;
	}
    }


  /* Assign vector lengths */
  if (veclen)
    {
      memcpy(field->veclen, veclen, sizeof(int) * field->ncomp);
    }
  else
    {
      for (i = 0;
	   i < field->ncomp;
	   i++)
	{
	  field->veclen[i] = DEFAULT_VECLEN;
	}
    }


  /* Assign data types */
  if (data_type)
    {
      memcpy(field->data_type, data_type, sizeof(int) * field->ncomp);
    }
  else
    {
      for (i = 0;
	   i < field->ncomp;
	   i++)
	{
	  field->data_type[i] = DEFAULT_DATATYPE;
	}
    }


  /* Allocate space for data */
  for (i = 0;
       i < field->ncomp;
       i++)
    {
      field->data[i] = (void *)ARRalloc
	(NULL, field->data_type[i], field->ndata * field->veclen[i], NULL);
      if (!field->data[i])
	{
	  ERRverror("DATcreate_field", ERR_INFO, "Can't allocate data arrays.");
	  DATfree_field(field);
	  return (FIELD *)NULL;
	}
    }

  /* Assign data */
  if (data)
    {
      for (i = 0;
	   i < field->ncomp;
	   i++)
	{
	  if (data[i])
	    {
	      switch (field->data_type[i])
		{
		case DTYPE_CHAR:
		case DTYPE_BYTE:
		  memcpy(field->data[i], data[i],
			 field->ndata * field->veclen[i] * sizeof(char));
		  break;  
		case DTYPE_SHORT:
		  memcpy(field->data[i], data[i],
			 field->ndata * field->veclen[i] * sizeof(short));
		  break;  
		case DTYPE_INT:
		  memcpy(field->data[i], data[i],
			 field->ndata * field->veclen[i] * sizeof(int));
		  break;  
		case DTYPE_FLOAT:
		  memcpy(field->data[i], data[i],
			 field->ndata * field->veclen[i] * sizeof(float));
		  break;  
		case DTYPE_DOUBLE:
		  memcpy(field->data[i], data[i],
			 field->ndata * field->veclen[i] * sizeof(double));
		  break;  
		default:
		  ERRverror
		    ("DATcreate_field", ERR_INFO, "Invalid field type.");
		  DATfree_field(field);
		  return (FIELD *)NULL;
		}
	    }
	}
    }
  else				/* Zero data arrays */
    {
      for (i = 0;
	   i < field->ncomp;
	   i++)
	{
	  switch (field->data_type[i])
	    {
	    case DTYPE_CHAR:
	    case DTYPE_BYTE:
	      bzero(field->data[i],
		    field->ndata * field->veclen[i] * sizeof(char));
	      break;  
	    case DTYPE_SHORT:
	      bzero(field->data[i],
		    field->ndata * field->veclen[i] * sizeof(short));
	      break;  
	    case DTYPE_INT:
	      bzero(field->data[i],
		    field->ndata * field->veclen[i] * sizeof(int));
	      break;  
	    case DTYPE_FLOAT:
	      bzero(field->data[i],
		    field->ndata * field->veclen[i] * sizeof(float));
	      break;  
	    case DTYPE_DOUBLE:
	      bzero(field->data[i],
		    field->ndata * field->veclen[i] * sizeof(double));
	      break;  
	    default:
	      ERRverror
		("DATcreate_field", ERR_INFO, "Invalid field type.");
	      DATfree_field(field);
	      return (FIELD *)NULL;
	    }
	}
    }


  /* Assign FIELD data to AVS object */
  if (!OMis_null_obj(field->id))
    {
      (void)DATfield_to_vxp(field, field->id);
    }
  return field;
}


/*-DATcreate_simple_field-------------------------------------------------------
 *  Description: creates a FIELD structure and assigns values
 *   Parameters: id                object ID
 *               ndim              number of data dimensions
 *               dims[ndim]        array of data dimensions
 *               nspace            spatial dimensions
 *               data_type[ncomp]  array of data types
 *               ncomp             number of components
 *               veclen[ncomp]     array of vector lengths
 *
 * Return value: pointer to the FIELD or NULL if an error occurs.
 */

FIELD *
DATcreate_simple_field(OMobj_id id, int ndim, int *dims, int data_type)
{
  int i, veclen;
  float *points;
  

  /* Initialise variables */
  veclen = 1;


  /* points[] = 0...dims[]-1 */
  points = (float *)ARRalloc(NULL, DTYPE_FLOAT, 2 * ndim, NULL);
  if (!points)
    {
      ERRverror
	("DATcreate_simple_field", ERR_INFO, "Can't allocate points array.");
      return (FIELD *)NULL;
    }

  for (i = 0;
       i < ndim;
       i++)
    {
      points[i] = 0.;
      points[i + ndim] = (float)(dims[i] - 1);
    }


  /* Create the field */
  return DATcreate_field
    (id, ndim, dims, ndim, points, 1, &veclen, &data_type, (void **)NULL);
}


/*-DATfree_field_hdr-----------------------------------------------------------
 *  Description: Frees a FIELD structure and arrays but not AVS field arrays
 *   Parameters: FIELD  *field    Pointer to the FIELD to be freed.
 * Return value: void
 */

void
DATfree_field_hdr(FIELD *field)
{
  if (field)
    {
      /* Free pointers dims, points, veclen, data_type, data */
      if (field->dims)
	{
	  ARRfree(field->dims);
	}
      if (field->veclen)
	{
	  ARRfree(field->veclen);
	}
      if (field->data_type)
	{
	  ARRfree(field->data_type);
	}
      if (field->data)
	{
	  ARRfree(field->data);
	}

      /* Free structure */
      free(field);
    }
}


/*-DATfree_field---------------------------------------------------------------
 *  Description: Frees a FIELD structure. 
 *   Parameters: FIELD  *field    Pointer to the FIELD to be freed.
 * Return value: void
 */

void
DATfree_field(FIELD *field)
{
  if (field)
    {
      if (field->points)
	{
	  ARRfree(field->points);
	}
      if (field->data)
	{
	  int comp;

	  for (comp = 0;
	       comp < field->ncomp;
	       comp++)
	    {
	      if (field->data[comp])
		{
		  ARRfree(field->data[comp]);
		}
	    }
	}
      DATfree_field_hdr(field);
    }
}


/*-DATfield_to_vxp--------------------------------------------------------------
 *  Description: sets VXP field objects using a FIELD structure
 *   Parameters: field             the FIELD
 *               id                object ID
 *
 * Return value: pointer to the object id.
 */

OMobj_id *
DATfield_to_vxp(FIELD *field, OMobj_id id)
{
  int i;


  if (FLDset_grid_type (id, 1) != 1 ||
      FLDset_ndim (id, field->ndim) != 1 ||
      FLDset_dims (id, field->dims) != 1 ||
      FLDset_nspace (id, field->nspace) != 1 ||
      FLDset_nnodes (id, field->ndata) != 1 ||
      FLDset_points
      (id, field->points, 2 * field->nspace, OM_SET_ARRAY_FREE) != 1 ||
      FLDset_node_data_ncomp(id, field->ncomp) != 1)
    {
      ERRverror("DATfield_to_vxp", ERR_INFO, "Cannot set field values.");
      DATfree_field(field);
      return NULL;
    }
  for (i = 0;
       i < field->ncomp;
       i++)
    {
      if (FLDset_node_data_veclen (id, i, field->veclen[i]) != 1 ||
	  FLDset_node_data_type (id, i, field->data_type[i]) != 1 ||
	  FLDset_node_data
	  (id, i, field->data[i], field->data_type[i],
	   field->ndata * field->veclen[i], OM_SET_ARRAY_FREE) != 1)
	{
	  ERRverror
	    ("DATfield_to_vxp", ERR_INFO,
	     "Cannot set node type/vector-length/values.");
	  DATfree_field(field);
	  return NULL;
	}
      if (FLDreset_node_minmax(id, i) != 1 ||
	  FLDreset_node_minmax_vec(id, i) != 1)
	{
	  ERRverror
	    ("DATfield_to_vxp", ERR_INFO, "Cannot set node minmax (vector).");
	  DATfree_field(field);
	  return NULL;
	}
    }
  return &id;
}


/*-DATvxp_to_field--------------------------------------------------------------
 *  Description: writes elements a VXP field object into a FIELD.
 *   Parameters: field_id    object id
 * Return value: pointer to the FIELD or NULL if an error occurs.
 */

FIELD *
DATvxp_to_field(OMobj_id field_id)
{
  int comp_count, grid_type, n;
  FIELD *field;


  /* Check it is a uniform field */
  if (FLDget_grid_type(field_id, &grid_type) != 1)
    {
      ERRverror("DATvxp_to_field", ERR_INFO, "Can't get grid type.");
      return (FIELD *)NULL;
    }
  if (grid_type != 1)
    {
      ERRverror("DATvxp_to_field", ERR_INFO, "Field must be uniform.");
      return (FIELD *)NULL;
    }


  /* Allocate memory */
  field = (FIELD *)malloc(sizeof(FIELD));
  if (!field)
    {
      ERRverror("DATvxp_to_field", ERR_INFO, "Could not allocate field.");
      return (FIELD *)NULL;
    }


  /* Initialize pointer values */
  field->dims = field->veclen = field->data_type = (int *)NULL;
  field->data = (void **)NULL;
  field->points = (float *)NULL;


  /* Set field values */
  field->id = field_id;
  if (FLDget_dims (field_id, &field->dims, &field->ndim) != 1 ||
      FLDget_nspace (field_id, &field->nspace) != 1 ||
      FLDget_points(field_id, &field->points, &n, OM_GET_ARRAY_RD) != 1 ||
      FLDget_nnodes (field_id, &field->ndata) != 1 ||
      FLDget_node_data_ncomp (field_id, &field->ncomp) != 1)
    {
      ERRverror("DATvxp_to_field", ERR_INFO, "Can't get field values.");
      DATfree_field(field);
      return (FIELD *)NULL;
    }

  /* Allocate space for each component */
  field->veclen = ARRalloc(NULL, DTYPE_INT, field->ncomp, NULL);
  field->data_type = ARRalloc(NULL, DTYPE_INT, field->ncomp, NULL);
  field->data = ARRalloc(NULL, DTYPE_PTR, field->ncomp, NULL);
  if (!field->veclen ||
      !field->data_type ||
      !field->data)
    {
      ERRverror
	("DATvxp_to_field", ERR_INFO, "Can't allocate component arrays.");
      DATfree_field(field);
      return (FIELD *)NULL;
    }

  /* Assign field values for each component */
  for (comp_count = 0;
       comp_count < field->ncomp;
       comp_count++)
    {
      if (FLDget_node_data_veclen
	  (field_id, comp_count, &field->veclen[comp_count]) != 1 ||
	  FLDget_node_data
	  (field_id, comp_count, &field->data_type[comp_count],
	   (char **)&field->data[comp_count], &n, OM_GET_ARRAY_RD) != 1)
	{
	  ERRverror
	    ("DATvxp_to_field", ERR_INFO,
	     "Cannot get data/vector-length/type/#values.");
	  DATfree_field(field);
	  return (FIELD *)NULL;
	}
    }
  return field;
}


/*-DATcopy_field---------------------------------------------------------------
 *  Description: Copies a FIELD and optional VXP field object.
 *   Parameters: FIELD  *field    Pointer to the FIELD to be copied.
 *               OMobj_id id      OM id of object to be associated with copy.
 *
 * Return value: The copy of the field or NULL if an error occurs.
 */

FIELD *
DATcopy_field(FIELD *field, OMobj_id id)
{
  FIELD *f;


  f = DATcreate_field
    (id, field->ndim, field->dims, field->nspace, field->points, field->ncomp,
     field->veclen, field->data_type, field->data);
  if (f && ! OMis_null_obj(id))
    {
      if (DATcopy_non_struct_values(field->id, id) != 1)
	{
	  DATfree_field(f);
	  return NULL;
	}
    }
  return f;
}


/*-DATcopy_non_struct_values----------------------------------------------------
 *  Description: Copy non-FIELD elements of a VXP field object to another VXP
 *               field object.
 *   Parameters: OMobj_id in     OM id of source object
 *               OMobj_id out    OM id of destination object
 *
 * Return value: 1 (success) 0/-1 (failure)
 */

int
DATcopy_non_struct_values(OMobj_id in, OMobj_id out)
{
  int   err, comp, comp1, comp2, data_id, null_flag, type, veclen;
  char  units[MAX_NAME_SIZE], label[MAX_NAME_SIZE];
  double null_value;


  /* Fields must have identical ncomp values */
  if (FLDget_node_data_ncomp(in, &comp1) != 1 ||
      FLDget_node_data_ncomp(out, &comp2) != 1)
    {	      
      ERRverror("DATcopy_non_struct_values", ERR_INFO, "Error getting ncomp");
      return -1;
    }
  if (comp1 != comp2)
    {
      ERRverror
	("DATcopy_non_struct_values", ERR_INFO,
	 "Fields have different ncomp values");
      return -1;
    }
 

  /* Copy coord units */
  if (FLDget_coord_units(in, units, MAX_NAME_SIZE) == 1)
    {
      err = FLDset_coord_units (out, units);
      if (err != 1)
	{
	  ERRverror
	    ("DATcopy_non_struct_values", ERR_INFO,
	     "Error setting coordinate units");
	  return err;
        }
    }


  /* Copy component values */
  for (comp = 0;
       comp < comp1;
       comp++)
    {
      if (FLDget_node_data_type(out, comp, &type) != 1)
	{	      
	  ERRverror
	    ("DATcopy_non_struct_values", ERR_INFO, "Error getting data type");
	  return -1;
	}
      if (FLDget_node_data_veclen(out, comp, &veclen) != 1)
	{	      
	  ERRverror
	    ("DATcopy_non_struct_values", ERR_INFO,
	     "Error getting vector length");
	  return -1;
	}
      if (FLDget_node_data_units(in, comp, units, MAX_NAME_SIZE) != 1)
	{
	  strcpy(units, "");
	}
      if (FLDget_node_data_label(in, comp, label, MAX_NAME_SIZE) != 1)
	{
	  strcpy(label, "");
	}
      err = FLDset_node_data_comp (out, comp, veclen, label, units);
      if (err != 1)
	{
	  ERRverror
	    ("DATcopy_non_struct_values", ERR_INFO,
	     "Error setting node units");
	  return err;
	}
      if (FLDget_node_data_id(in, comp, &data_id) == 1)
	{
	  err = FLDset_node_data_id(out, comp, data_id);
	  if (err != 1)
	    {
	      ERRverror
		("DATcopy_non_struct_values", ERR_INFO,
		 "Error setting data ID");
	      return err;
	    }
	}
      if (FLDget_node_null_data(in, comp, &null_flag, (char *)&null_value) == 1)
	{
	  if (null_flag)
	    {
	      err = FLDset_node_null_data(out, comp, (char *)&null_value, type);
	      if (err != 1)
		{
		  ERRverror
		    ("DATcopy_non_struct_values", ERR_INFO,
		     "Error setting null value");
		  return err;
		}
	    }
	  else
	    {
	      err = FLDset_node_null_flag(out, comp, 0);
	      if (err != 1)
		{
		  ERRverror
		    ("DATcopy_non_struct_values", ERR_INFO,
		     "Error setting null value");
		  return err;
		}
	    }
	}
    }
  return 1;
}


/*-DATprint_field--------------------------------------------------------------
 *  Description: Print contents of FIELD to stdout
 *   Parameters: FIELD  *field    Pointer to the FIELD to be printed
 * Return value: void
 */

void
DATprint_field(FIELD *field)
{
  printf("field=%d\n", field);
  if (field)
    {
      int i;


      printf
	("  id (elem, proc)=(%d, %d)\n", field->id.elem_id, field->id.proc_id);
      printf("  dims=");
      for (i = 0;
	   i < field->ndim;
	   i++)
	{
	  printf("%d ", field->dims[i]);
	}
      printf("\n");
      printf("  ndata=%d\n", field->ndata);
      printf("  nspace=%d\n  extents: ", field->nspace);
      for (i = 0;
	   i < 2 * field->nspace;
	   i++)
	{
	  printf("%f ", field->points[i]);
	}
      printf("\n");
      for (i = 0;
	   i < field->ncomp;
	   i++)
	{
	  printf("  Component #%d:\n", i);
	  printf("    veclen=%d\n", field->veclen[i]);
	  printf("    data_type=%d\n", field->data_type[i]);
	  printf("    data=%d\n", field->data[i]);
	}
   
      if (!OMis_null_obj(field->id))
	{
	  int   data_id, null_flag;
	  char  units[MAX_NAME_SIZE], label[MAX_NAME_SIZE];
	  double null_value;


	  printf("  Non-struct values:\n");
	  if (FLDget_coord_units(field->id, units, MAX_NAME_SIZE) == 1)
	    {
	      printf("    coordinate units %s\n", units);
	    }
	  for (i = 0;
	       i < field->ncomp;
	       i++)
	    {
	      printf("    Component #%d:\n", i);
	      if (FLDget_node_data_units
		  (field->id, i, units, MAX_NAME_SIZE) == 1)
		{
		  printf("      node units %s\n", units);
		}
	      if (FLDget_node_data_label
		  (field->id, i, label, MAX_NAME_SIZE) == 1)
		{
		  printf("      node label %s\n", label);
		}
	      if (FLDget_node_data_id(field->id, i, &data_id) == 1)
		{
		  printf("      node data ID %s\n", data_id);
		}
	      if (FLDget_node_null_data
		  (field->id, i, &null_flag, (char *)&null_value) == 1)
		{
		  printf
		    ("      null flag %d\n      null value (double) %g\n",
		     null_flag, null_value);
		}
	    }
	}
    }
}

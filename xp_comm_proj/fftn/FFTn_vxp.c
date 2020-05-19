
/*------------------------------------------------------------------------------
 *
 *    TITLE: FFTn_vxp.c
 *  CREATED: 30/10/97
 * MODIFIED: 29/04/98
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


#include "express.h"
#include "data_field.h"
#include "avs_err.h"
#include "fftn.h"


/* Local prototypes */
static float *FFTbuild_array(int ndim, int *dims, int veclen,
			     int type, void *data);

/*-FFTn_build_array-------------------------------------------------------------
 *  Description: construct an array for use with FFT routines
 *   Parameters: int    ndim    No. dimensions
 *               int    *dims   Array of dimensions
 *               int    veclen  Vector length (1/2)
 *               int    type    Data type
 *               void   *data   Pointer to data
 * Return value: pointer to the array or NULL for failure
 */

static float *
FFTbuild_array(int ndim, 
	       int *dims,
	       int veclen,
	       int type,
	       void *data)
{
  int size, i;
  float *array, *arrPtr;
  unsigned char *bytePtr;
  short *shortPtr;
  int *intPtr;
  float *floatPtr;
  double *doublePtr;


  /* Calculate length of array */
  for (size = 1, i = 0;
       i < ndim;
       i++)
    {
      size *= dims[i];
    }


  /* Allocate space for array */
  array = (float *)ARRalloc(NULL, DTYPE_FLOAT, size * 2, NULL);
  if (array)
    {
      float bVal;

      if (veclen == 1)
	{
	  switch (type)
	    {
	    case DTYPE_CHAR:
	    case DTYPE_BYTE:
	      bytePtr = (unsigned char *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  bVal = *bytePtr++;
		  *arrPtr++ = bVal;
		  *arrPtr++ = 0.;
		}
	      break;
	    case DTYPE_SHORT:
	      shortPtr = (short *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *shortPtr++;
		  *arrPtr++ = 0.;
		}
	      break;
	    case DTYPE_INT:
	      intPtr = (int *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *intPtr++;
		  *arrPtr++ = 0.;
		}
	      break;
	    case DTYPE_FLOAT:
	      floatPtr = (float *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *floatPtr++;
		  *arrPtr++ = 0.;
		}
	      break;
	    case DTYPE_DOUBLE:
	      doublePtr = (double *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *doublePtr++;
		  *arrPtr++ = 0.;
		}
	      break;
	    default:
	      ARRfree(array);
	      set_avs_err("Invalid data type");
	      array = NULL;
	    }
	}
      else if (veclen == 2)
	{
	  switch (type)
	    {
	    case DTYPE_CHAR:
	    case DTYPE_BYTE:
	      bytePtr = (unsigned char *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  bVal = *bytePtr++;
		  *arrPtr++ = bVal;
		  bVal = *bytePtr++;
		  *arrPtr++ = bVal;
		}
	      break;
	    case DTYPE_SHORT:
	      shortPtr = (short *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *shortPtr++;
		  *arrPtr++ = *shortPtr++;
		}
	      break;
	    case DTYPE_INT:
	      intPtr = (int *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *intPtr++;
		  *arrPtr++ = *intPtr++;
		}
	      break;
	    case DTYPE_FLOAT:
	      floatPtr = (float *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *floatPtr++;
		  *arrPtr++ = *floatPtr++;
		}
	      break;
	    case DTYPE_DOUBLE:
	      doublePtr = (double *)data;
	      arrPtr = array;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  *arrPtr++ = *doublePtr++;
		  *arrPtr++ = *doublePtr++;
		}
	      break;
	    default:
	      ARRfree(array);
	      set_avs_err("Invalid data type");
	      array = NULL;
	    }
	}
      else
	{
	  ARRfree(array);
	  set_avs_err("Vector length must be 1 or 2");
	  array = NULL;
	}
    }
  else
    {
      set_avs_err("Could not allocate node data");
    }
  return array;
}


/*-FFTn_vxp---------------------------------------------------------------------
 *  Description: compute the forward/inverse FFT of a field
 *   Parameters: FFT_id               module id
 *               event_mask           event_mask
 *               seq_num              seq_num
 * Return value: 0/1 on failure/success.
 */

int
FFTn_vxp(OMobj_id FFT_id, OMevent_mask event_mask, int seq_num)
{
  int i, direction, rescale, *veclen, *data_type;
  OMobj_id in_field_id, out_field_id;
  FIELD *in, *out;
  float **data, scale;


  /* Get direction and rescale valuea */ 
  direction = 0;
  (void)OMget_name_int_val(FFT_id, OMstr_to_name("direction"), &direction);
  rescale = 0;
  (void)OMget_name_int_val(FFT_id, OMstr_to_name("rescale"), &rescale);

  /* Get mesh ids */
  in_field_id = OMfind_subobj(FFT_id, OMstr_to_name("in_field"), OM_OBJ_RD);
  out_field_id = OMfind_subobj(FFT_id, OMstr_to_name("out_field"), OM_OBJ_RW);


  /* Create in field */
  in = DATvxp_to_field(in_field_id);
  if (!in)
    {
      ERRverror("FFT_vxp", ERR_INFO, avsErrStr);
      return 0;
    }


  /* Check veclen */
  for (i = 0;
       i < in->ncomp;
       i++)
    {
      if (in->veclen[i] != 1 && in->veclen[i] != 2)
	{
	  DATfree_field(in);
	  ERRverror("FFTn_vxp", ERR_INFO, "Vector length must be 1 or 2");
	  return 0;
	}
    }


  /* Allocate arrays and assign data*/
  veclen = (int *)ARRalloc(NULL, DTYPE_INT, in->ncomp, NULL);
  data_type = (int *)ARRalloc(NULL, DTYPE_INT, in->ncomp, NULL);
  data = (float **)ARRalloc(NULL, DTYPE_PTR, in->ncomp, NULL);
  if (!veclen || !data_type || !data)
    {
      ARRfree(data);
      ARRfree(veclen);
      ARRfree(data_type);
      DATfree_field(in);
      ERRverror("FFTn_vxp", ERR_INFO, "Could not allocate memory");
      return 0;
    }

  for (i = 0;
       i < in->ncomp;
       i++)
    {
      veclen[i] = 2;
      data_type[i] = DTYPE_FLOAT;
      data[i] = FFTbuild_array
	(in->ndim, in->dims, in->veclen[i], in->data_type[i], in->data[i]);
      if (!data[i])
	{
	  int j;

	  for (j = 0;
	       j < i;
	       j++)
	    {
	      ARRfree(data[j]);
	    }
	  ARRfree(data);
	  ARRfree(veclen);
	  ARRfree(data_type);
	  DATfree_field(in);
	  ERRverror("FFTn_vxp", ERR_INFO, avsErrStr);
	  return 0;
	}

      /* Set scale factor */
      switch (rescale)
	{
	  int j;
	  
	case 0:
	  scale = 0.;
	  break;
	case 1:
	  scale = -1.;
	  break;
	case 2:
	  scale = -2.;
	  break;
	default:
	  scale = 0.;
	  break;
	}

      /* Compute the transform in place */
      if (fftnf(in->ndim, in->dims, data[i], &data[i][1], 
		direction ? -2 : 2, scale))
      {
	int j;
	
	for (j = 0;
	     j < i;
	     j++)
	  {
	    ARRfree(data[j]);
	  }
	ARRfree(data);
	ARRfree(veclen);
	ARRfree(data_type);
	DATfree_field(in);
	ERRverror("FFTn_vxp", ERR_INFO, "Error computing FFT");
	return 0;
      }
    }

  
  /* Create field to store data */
  out = DATcreate_field
    (out_field_id, in->ndim, in->dims, in->nspace, NULL, in->ncomp, veclen,
     data_type, (void **)data);
  DATcopy_non_struct_values(in_field_id, out_field_id);


  /* Clean up and exit */
  ARRfree(data);
  ARRfree(veclen);
  ARRfree(data_type);
  DATfree_field(in);
  DATfree_field_hdr(out);
  return 1;
}

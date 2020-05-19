
/*------------------------------------------------------------------------------
 *
 *    TITLE: FFT_vxp.c
 *  CREATED: 30/10/97
 * MODIFIED: 28/04/98
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


#include <math.h>

#include "express.h"
#include "data_field.h"
#include "avs_err.h"
#include "fftw.h"


/* Local prototypes */
static void
FFTW_rescale(FFTW_COMPLEX *data, int len, double scale);
static FFTW_COMPLEX *FFTWbuild_array(int ndim, int *dims, int veclen,
				     int type, void *data);


/*-FFTW_rescale-----------------------------------------------------------------
 *  Description: rescale a FFTW_COMPLEX array after FFT
 *   Parameters: FFTW_COMPLEX *data  The array
 *               int          len    Length of the array
 *               double       scale  The scalar
 * Return value: void
 */

static void
FFTW_rescale(FFTW_COMPLEX *data, int len, double scale)
{
  int i;


  for (i = 0;
       i < len;
       i++)
    {
      data[i].re /= scale;
      data[i].im /= scale;
    }
}


/*-FFTW_build_array-------------------------------------------------------------
 *  Description: construct a FFTW_COMPLEX array for use with FFTW routines
 *   Parameters: int    ndim    No. dimensions
 *               int    *dims   Array of dimensions
 *               int    veclen  Vector length (1/2)
 *               int    type    Data type
 *               void   *data   Pointer to data
 * Return value: pointer to the array or NULL for failure
 */

static FFTW_COMPLEX *
FFTWbuild_array(int ndim, 
		int *dims,
		int veclen,
		int type,
		void *data)
{
  int size, i;
  FFTW_COMPLEX *array;
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
  array = (FFTW_COMPLEX *)ARRalloc(NULL, DTYPE_DOUBLE, size * 2, NULL);
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
	      for (i = 0;
		   i < size;
		   i++)
		{
		  bVal = *bytePtr++;
		  array[i].re = bVal;
		  array[i].im = 0.;
		}
	      break;
	    case DTYPE_SHORT:
	      shortPtr = (short *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *shortPtr++;
		  array[i].im = 0.;
		}
	      break;
	    case DTYPE_INT:
	      intPtr = (int *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *intPtr++;
		  array[i].im = 0.;
		}
	      break;
	    case DTYPE_FLOAT:
	      floatPtr = (float *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *floatPtr++;
		  array[i].im = 0.;
		}
	      break;
	    case DTYPE_DOUBLE:
	      doublePtr = (double *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *doublePtr++;
		  array[i].im = 0.;
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
	      for (i = 0;
		   i < size;
		   i++)
		{
		  bVal = *bytePtr++;
		  array[i].re = bVal;
		  bVal = *bytePtr++;
		  array[i].im = bVal;
		}
	      break;
	    case DTYPE_SHORT:
	      shortPtr = (short *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *shortPtr++;
		  array[i].im = *shortPtr++;
		}
	      break;
	    case DTYPE_INT:
	      intPtr = (int *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *intPtr++;
		  array[i].im = *intPtr++;
		}
	      break;
	    case DTYPE_FLOAT:
	      floatPtr = (float *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *floatPtr++;
		  array[i].im = *floatPtr++;
		}
	      break;
	    case DTYPE_DOUBLE:
	      doublePtr = (double *)data;
	      for (i = 0;
		   i < size;
		   i++)
		{
		  array[i].re = *doublePtr++;
		  array[i].im = *doublePtr++;
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


/*-FFTw_vxp---------------------------------------------------------------------
 *  Description: compute the forward/inverse FFT of a field
 *   Parameters: FFT_id               module id
 *               event_mask           event_mask
 *               seq_num              seq_num
 * Return value: 0/1 on failure/success.
 */

int
FFTw_vxp(OMobj_id FFT_id, OMevent_mask event_mask, int seq_num)
{
  int prodDims, i, rescale, direction, *veclen, *data_type;
  OMobj_id in_field_id, out_field_id;
  FIELD *in, *out;
  FFTW_COMPLEX **data;
  fftwnd_plan plan;


  /* Get direction and rescale values */ 
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
	  ERRverror("FFT_vxp", ERR_INFO, "Vector length must be 1 or 2");
	  return 0;
	}
    }


  /* Create the plan (just estimate the optimal strategy) */
  plan = fftwnd_create_plan
    (in->ndim, in->dims, direction ? FFTW_BACKWARD : FFTW_FORWARD,
     FFTW_ESTIMATE | FFTW_IN_PLACE);
  if (!plan)
    {
      DATfree_field(in);
      ERRverror("FFT_vxp", ERR_INFO, "Error creating plan");
      return 0;
    }

  /* Allocate arrays and assign data*/
  veclen = (int *)ARRalloc(NULL, DTYPE_INT, in->ncomp, NULL);
  data_type = (int *)ARRalloc(NULL, DTYPE_INT, in->ncomp, NULL);
  data = (FFTW_COMPLEX **)ARRalloc(NULL, DTYPE_PTR, in->ncomp, NULL);
  if (!veclen || !data_type || !data)
    {
      ARRfree(data);
      ARRfree(veclen);
      ARRfree(data_type);
      DATfree_field(in);
      fftwnd_destroy_plan(plan);
      ERRverror("FFT_vxp", ERR_INFO, "Could not allocate memory");
      return 0;
    }


  /* Determine scale */
  prodDims = in->dims[0];
  for (i = 1;
       i < in->ndim;
       i++)
    {
      prodDims *= in->dims[i];
    }


  for (i = 0;
       i < in->ncomp;
       i++)
    {
      veclen[i] = 2;
      data_type[i] = DTYPE_DOUBLE;
      data[i] = FFTWbuild_array
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
	  fftwnd_destroy_plan(plan);
	  ERRverror("FFT_vxp", ERR_INFO, avsErrStr);
	  return 0;
	}
      /* Compute the transform in place */
      fftwnd(plan, 1, data[i], 1, 0, 0, 0, 0);

      /* Rescale */
      switch (rescale)
	{
	case 0:			/* No rescale */
	  break;
	case 1:
	  FFTW_rescale(data[i], prodDims, (double)prodDims);
	  break;
	case 2:
	  FFTW_rescale(data[i], prodDims, sqrt((double)prodDims));
	  break;
	default:		/* No rescale */
	  break;
	}
    }

  
  /* Create field to store data */
  out = DATcreate_field
    (out_field_id, in->ndim, in->dims, in->nspace, NULL, in->ncomp, veclen,
     data_type, (void **)data);
  DATcopy_non_struct_values(in_field_id, out_field_id);


  /* Clean up and exit */
  fftwnd_destroy_plan(plan);
  ARRfree(data);
  ARRfree(veclen);
  ARRfree(data_type);
  DATfree_field(in);
  DATfree_field_hdr(out);
  return 1;
}

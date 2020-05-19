
/*------------------------------------------------------------------------------
 *
 *    TITLE: node_stats.c
 *  CREATED: 09/01/98
 * MODIFIED: 29/04/98
 *   AUTHOR: Chris Pudney (cpudney@alphapharm.pharm.uwa.edu.au)
 *           Biomedical Confocal Microscopy Research Centre
 *           Department of Pharmacology
 *           The University of Western Australia
 *           Nedlands, WA 6907, Australia
 *
 * Copyright (C) Chris Pudney, The University of Western Australia, 1998.
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

#include "express.h"
#include "data_field.h"
#include "avs_err.h"
#include "node_stats.h"


#define NSTATSLEN  1024		/* Max string length */


unsigned int *nonzero_node_count(FIELD *field, int i);
double *node_sum(FIELD *field, int i);


/*-node_sum---------------------------------------------------------------------
 *  Description: Returns a vector representing the sums of node values
 *   Parameters: field    the field of nodes
 *               i        component index for analysis
 * Return Value: the vector or null (failure)
 */

double *
node_sum(FIELD *field, int i)
{
  double *sum;


  sum = (double *)calloc(field->veclen[i], sizeof(double));
  if (sum)
    {
      switch (field->data_type[i])
	{
	  int k, j, i2c;
	  unsigned char *charPtr;
	  short *shortPtr;
	  int *intPtr;
	  float *floatPtr;
	  double *dblPtr;


	case DTYPE_CHAR:
	case DTYPE_BYTE:
	  charPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  i2c = *charPtr++;
		  sum[k] += i2c;
		}
	    }
	  break;
	case DTYPE_SHORT:
          shortPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  sum[k] += *shortPtr++;
		}
	    }
	  break;  
	case DTYPE_INT:
          intPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  sum[k] += *intPtr++;
		}
	    }
	  break;  
	case DTYPE_FLOAT:
          floatPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
		for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  sum[k] += *floatPtr++;
		}
	    }
	  break;  
	case DTYPE_DOUBLE:
 	  dblPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  sum[k] += *dblPtr++;
		}
	    }
	  break;  
	default:
	  free(sum);
	  return NULL;
	  break;
	}
    }
  return sum;
}


/*-nonzero_node_count-----------------------------------------------------------
 *  Description: Returns a vector representing the numbers of non-zero nodes
 *   Parameters: field    the field of nodes
 *               i        component index for analysis
 * Return Value: the vector or null (failure)
 */

unsigned int *
nonzero_node_count(FIELD *field, int i)
{
  unsigned int *nz;


  nz = (unsigned int *)calloc(field->veclen[i], sizeof(unsigned int));
  if (nz)
    {
      switch (field->data_type[i])
	{
	  int k, j;
	  unsigned char *charPtr;
	  short *shortPtr;
	  int *intPtr;
	  float *floatPtr;
	  double *dblPtr;


	case DTYPE_CHAR:
	case DTYPE_BYTE:
	  charPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  if (*charPtr++)
		    {
		      nz[k]++;
		    }
		}
	    }
	  break;  
	case DTYPE_SHORT:
	  shortPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  if (*shortPtr++)
		    {
		      nz[k]++;
		    }
		}
	    }
	  break;  
	case DTYPE_INT:
	  intPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  if (*intPtr++)
		    {
		      nz[k]++;
		    }
		}
	    }
	  break;  
	case DTYPE_FLOAT:
	  floatPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  if (*floatPtr++)
		    {
		      nz[k]++;
		    }
		}
	    }
	  break;  
	case DTYPE_DOUBLE:
	  dblPtr = field->data[i];
	  for (j = 0;
	       j < field->ndata;
	       j++)
	    {
	      for (k = 0;
		   k < field->veclen[i];
		   k++)
		{
		  if (*dblPtr++)
		    {
		      nz[k]++;
		    }
		}
	    }
	  break;  
	default:
	  free(nz);
	  return NULL;
	  break;
	}
    }
  return nz;
}


/*-node_stats-------------------------------------------------------------------
 *  Description: Returns a string of node statistics of a FIELD
 *   Parameters: field                the field
 * Return value: the string/NULL success/failure
 */

char *
node_stats(FIELD *field)
{
  char *stats, tmpStr[128];


  /* Allocate space for the string */
  stats = malloc(sizeof(char) * NSTATSLEN);
  if (stats)
    {
      if (field)
	{
	  int i, j;


	  sprintf(stats, "Dimensions: ");
	  for (i = 0;
	       i < field->ndim - 1;
	       i++)
	    {
	      sprintf(tmpStr, "%d x ", field->dims[i]);
	      strcat(stats, tmpStr);
	    }
	  sprintf(tmpStr, "%d\n", field->dims[i]);
	  strcat(stats, tmpStr);
	  sprintf(tmpStr, "No. Nodes: %d\n", field->ndata);
	  strcat(stats, tmpStr);


	  for (i = 0;
	       i < field->ncomp;
	       i++)
	    {
	      double *sum;
	      unsigned int *nonzero;


	      sprintf(tmpStr,
		      "Component #%d\n  Vector Length %d\n",
		      i, field->veclen[i], field->data_type[i] );
	      strcat(stats, tmpStr);

	      switch (field->data_type[i])
		{
		case DTYPE_CHAR:
		  sprintf(tmpStr, "  Data Type: Character\n");
		  break;
		case DTYPE_BYTE:
		  sprintf(tmpStr, "  Data Type: Byte\n");
		  break;  
		case DTYPE_SHORT:
		  sprintf(tmpStr, "  Data Type: Short Integer\n");
		  break;  
		case DTYPE_INT:
		  sprintf(tmpStr, "  Data Type: Integer\n");
		  break;  
		case DTYPE_FLOAT:
		  sprintf(tmpStr, "  Data Type: Single-Precision Float\n");
		  break;  
		case DTYPE_DOUBLE:
		  sprintf(tmpStr, "  Data Type: Double-Precision Float\n");
		  break;  
		default:
		  sprintf(tmpStr, "  Data Type: Other\n");
		}
	      strcat(stats, tmpStr);

	      /* Count nonzero node values */
	      nonzero = nonzero_node_count(field, i);
	      if (nonzero)
		{
		  strcat(stats, "  No. non-zero nodes: ");
		  sprintf(tmpStr, "(%u", nonzero[0]);
		  strcat(stats, tmpStr);
		  for (j = 1;
		       j < field->veclen[i];
		       j++)
		    {
		      sprintf(tmpStr, ", %u", nonzero[j]);
		      strcat(stats, tmpStr);
		    }
		  strcat(stats, ")\n");
		}
	      free(nonzero);

	      /* Sum node values */
	      sum = node_sum(field, i);
	      if (sum)
		{
		  strcat(stats, "  Node value sum: ");
		  sprintf(tmpStr, "(%f", sum[0]);
		  strcat(stats, tmpStr);
		  for (j = 1;
		       j < field->veclen[i];
		       j++)
		    {
		      sprintf(tmpStr, ", %f", sum[j]);
		      strcat(stats, tmpStr);
		    }
		  strcat(stats, ")\n");
		}
	      free(sum);
	    }
	}
    }
  else
    {
      set_avs_err("Cannot allocate memory for string.");
    }
  return stats;
}

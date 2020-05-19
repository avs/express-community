
/*------------------------------------------------------------------------------
 *
 *    TITLE: data_field.h
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


#define DEFAULT_DIM 64
#define DEFAULT_VECLEN 1
#define DEFAULT_DATATYPE DTYPE_BYTE
#define MAX_NAME_SIZE 1024


/* Field structure */
typedef
struct field_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  void **data;                  /* Array of pointers to data */
} FIELD;    

typedef
struct field_byte_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  unsigned char **data;		/* Array of pointers to data */
} FIELD_BYTE;

typedef
struct field_short_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  short **data;			/* Array of pointers to data */
} FIELD_SHORT;

typedef
struct field_int_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  int **data;			/* Array of pointers to data */
} FIELD_INT;

typedef
struct field_float_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  float **data;			/* Array of pointers to data */
} FIELD_FLOAT;

typedef
struct field_double_struct
{
  OMobj_id id;			/* Object id */
  int ndim;                     /* No. dimensions */
  int *dims;                    /* Array of dimensions */
  int nspace;                   /* No. spatial dimensions */
  float *points;                /* Spatial extents */
  int ndata;                    /* Number data points */
  int ncomp;                    /* No. components */
  int *veclen;                  /* Array of vector lengths */
  int *data_type;               /* Array of data types */
  double **data;		/* Array of pointers to data */
} FIELD_DOUBLE;



/* Function prototypes */
extern int DATcopy_non_struct_values(OMobj_id id1, OMobj_id id2);
extern void DATfree_field(FIELD *field);
extern void DATfree_field_hdr(FIELD *field);
extern void DATprint_field(FIELD *field);
extern FIELD *DATvxp_to_field(OMobj_id field_id);
extern FIELD *DATcreate_field(OMobj_id id, int ndim, int *dims, int nspace,
			      float *points, int ncomp, int *veclen,
			      int *data_type, void **data);
extern FIELD *DATcreate_simple_field(OMobj_id id, int ndim, int *dims,
				     int data_type);
extern FIELD *DATcopy_field(FIELD *field, OMobj_id id2);
extern OMobj_id *DATfield_to_vxp(FIELD *field, OMobj_id id);

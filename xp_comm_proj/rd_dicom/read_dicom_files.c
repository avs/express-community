
#ifdef MSDOS
#define FILENAME83
#endif

/* Modifications for use with CAMARA ESP */
/* by James S Perrin */
/* now uses Papyrus 3.6 */
/* ditch status reporting */
/* reads in a set of raw dicom images files (with which Papyrus is compatible) */
/* and constructs the volume, images are transposed to create correct volume */
/* sets extents and gets additional info windowing etc */

/*
  Copyright (c) 1998 by
  Advanced Visual Systems Inc.
  All Rights Reserved
  
  This software comprises unpublished confidential information of
  Advanced Visual Systems Inc. and may not be used, copied or made
  available to anyone, except in accordance with the license
  under which it is furnished.
  
  This file is under sccs control at AVS in:
  $Source: /cvs/repository/express/modules/contour.c,v $

  We used :
      PAPYRUS toolkit v3.4
      Copyright (c) 1996 by
      University Hospital of Geneva
*/



/*#define DEBUG*/
#define XP_STATUS
/* Major functional change */
/* Given the first raw image file of series read them into a single volume */
#define MAX_SLICES 512

#include "gen.h"
#include "avs/gd_def.h"
#include "Papyrus3.h"
#include "read_dicom_utils.h"

#include <stdlib.h>
#include <stdio.h>
#ifdef MSDOS
#include <string.h>
#else
#include <strings.h>
#endif

#define ERR(A)  { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A); \
}

#define ERR1(A,p1) { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A, p1); \
}

#define ERR_RETURN(A)  { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A); \
 return(0); \
}

#define ERR_RETURN1(A,p1) { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A, p1); \
 return(0); \
}

static int papy_inited=FALSE;

int Read_Dicom_Files(OMobj_id Read_Dicom_File_id,
                    OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  char *filename_ = NULL;
  FILE *fin;
  char filePattern[1024], filePrepend[1024], fileAppend[1024], filenames[MAX_SLICES][1024];
  int first, last, nImages, nImagesFnd = 0, inc = 1, nWidth = 1;
  int fok;
  OMobj_id volume_id, header_id, files_id;
  OMobj_id patient_id, general_study_id, general_series_id, general_equipment_id, image_series_id;
  int volume_ndim, *volume_dims,
    volume_nspace, volume_npoints;
  float *volume_points;
  int volume_ncomp, comp, volume_veclen;
  int volume_data_type, volume_ndata;
  char  *volume_data;
  PapyShort fileNb;
  int flip; /* whether to reorder images top to bottom */
  int i, j, k, n;
  int tmpVal;
#ifdef XP_STATUS
  int interrupt;
  int status_in_progress;
#endif
  int datatype;
  int cellsize;
  char interp[80]="";
  char *modality=NULL;
  int bits=0;
  int height=0;
  int width=0;
  float sliceSpacing = 2;
  float xSpacing = 1; 
  float ySpacing = 1; 

  
  OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 1);
#ifdef XP_STATUS
  OMstatus_check(0, "Reading Dicom File", &interrupt);
#endif
  
  if (!papy_inited) {
    Papy3Init();
    papy_inited=TRUE;
  }
  
  /***********************/
  /*  Get input values   */
  /***********************/
  volume_id = OMfind_subobj(Read_Dicom_File_id, OMstr_to_name("volume"), OM_OBJ_RW);
  
  if(OMis_null_obj(volume_id)) {
    fprintf(stderr, "--- volume_id NULL\n");
    return(0);
  }
  
  if(OMget_name_int_val(Read_Dicom_File_id, OMstr_to_name("flip"), &flip)!=1)
    flip = 0;
  
  files_id = OMfind_subobj(Read_Dicom_File_id,OMstr_to_name("filenames"), OM_OBJ_RW);
  OMget_array_size(files_id, &nImages);  
  nImagesFnd = nImages;
  
#ifdef DEBUG
  printf("total frames %d\n", nImagesFnd);
#endif

  /*  if (!nImagesFnd || fileNb<0) {*/
  if(nImages == 0)
  {
    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif
   RD_Set_Null_Mesh(volume_id);
    ERR_RETURN1("--- Can't open filename %s",filenames[0]);
  }

  /* copy all filenames into local array */
  for(i=0; i<nImages; i++)
    OMret_str_array_val(files_id, i, (char *)&filenames[i], 1023);
  
  /* open first image to get DICOM info */
  fileNb=Papy3FileOpen(filenames[0], 0, TRUE, NULL);

  /*  nImagesFnd=Papy3GetNbImages (fileNb);*/
/*  printf("nImagesFnd = %d\n",nImagesFnd);*/
  
  header_id = OMfind_subobj(
    Read_Dicom_File_id, OMstr_to_name("header"), OM_OBJ_RW);
  patient_id = OMfind_subobj(
    header_id, OMstr_to_name("patient"), OM_OBJ_RW);
  general_study_id = OMfind_subobj(
    header_id, OMstr_to_name("generalStudy"), OM_OBJ_RW);
  general_series_id = OMfind_subobj(
    header_id, OMstr_to_name("generalSeries"), OM_OBJ_RW);
  general_equipment_id = OMfind_subobj(
    header_id, OMstr_to_name("generalEquipment"), OM_OBJ_RW);
  image_series_id = OMfind_subobj(
    header_id, OMstr_to_name("imageSeries"), OM_OBJ_RW);
  
  if (OMis_null_obj(header_id))
    fprintf(stderr, "--- header_id NULL\n");
  if (OMis_null_obj(patient_id))
    fprintf(stderr, "--- patient_id NULL\n");
  if (OMis_null_obj(general_study_id))
    fprintf(stderr, "--- general_study_id NULL\n");
  if (OMis_null_obj(general_series_id))
    fprintf(stderr, "--- general_series_id NULL\n");
  if (OMis_null_obj(general_equipment_id))
    fprintf(stderr, "--- general_equipment_id NULL\n");
  
  /**************************/
  /*  Fill in image header  */
  /**************************/
  RD_Read_Patient(fileNb, 1, patient_id);
  RD_Read_General_Study(fileNb, 1, general_study_id);
  RD_Read_General_Series(fileNb, 1, general_series_id);
  RD_Read_General_Equipment(fileNb, 1, general_equipment_id);
  OMget_name_str_val(general_series_id,  OMstr_to_name("ModalityGS"), &modality, (int)NULL);
  if(nImages>1)
    RD_Read_Image_Series(fileNb, 1, image_series_id, modality, &height, &width, &bits,
			 &sliceSpacing, &xSpacing, &ySpacing, interp, filenames[1]);
  else 
    RD_Read_Image_Series(fileNb, 1, image_series_id, modality, &height, &width, &bits,
			 &sliceSpacing, &xSpacing, &ySpacing, interp, NULL);

  OMset_name_int_val( image_series_id, OMstr_to_name("Slices"), nImagesFnd);
  free(modality);
  
  /***********************/
  /*  Set output values  */
  /***********************/
  /* replaced with Read_MR_Image_Series */
  /*  Get_Image_Attributes(fileNb, 1, &height, &width, &bits, interp);*/
  if (bits<=8) {
    cellsize = 1;
  }
  else if (bits<=16) {
    cellsize = 2;
  }
  else {
    fprintf(stderr, "--- Unsupported element size %d\n", bits);
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif
    RD_Set_Null_Mesh(volume_id);
    return(1);
  }
  
  datatype = 0;
/*  datasize = 1;*/
  if (strncmp(interp, "RGB", 3) == 0) {
    datatype = 1;
/*    datasize = 3;*/
  }
  
  /***********************/
  /*  Set output values  */
  /***********************/
  /* Set volume uniform mesh */
  
  /*  Get mesh id 
   */
  volume_id = OMfind_subobj(Read_Dicom_File_id, OMstr_to_name("volume"), OM_OBJ_RW);
  
  /* Clear that puppy out...
   */
  FLDset_node_data_ncomp(volume_id, 0);
  
  /* Set mesh dimensionality, volume_ndim can be 1,2 or 3 
   */
  volume_ndim = 3;
  FLDset_ndim (volume_id, volume_ndim);
  
  /* Set mesh dims array 
   */
  volume_dims = (int *)ARRalloc(NULL, DTYPE_INT, volume_ndim, NULL);
  if (!volume_dims) {
    fprintf(stderr, "--- Can't alloc volume_dims\n");
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Dicom File", &interrupt);
    RD_Set_Null_Mesh(volume_id);
#endif
    return(1);
  }

  volume_dims[0] = width;
  volume_dims[1] = height;
  volume_dims[2] = nImagesFnd;
  FLDset_dims (volume_id, volume_dims);
  ARRfree((char *)volume_dims);
  
  /* Set mesh nspace, volume_nspace can be 1,2 or 3 
   */
  volume_nspace = 3;
  FLDset_nspace (volume_id, volume_nspace);
  
  /* Set mesh extents 
   */
  volume_npoints = 2 * volume_nspace;
  volume_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, volume_npoints, NULL);
  if (!volume_points) {
    fprintf(stderr, "--- Can't alloc volume_points\n");
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif    
    RD_Set_Null_Mesh(volume_id);
    return(1);
  }

  /* extents of the volume are set in mm */
  volume_points[0] = 0;
  volume_points[1] = 0;
  volume_points[2] = 0;
  volume_points[3] = (float)width * xSpacing;
  volume_points[4] = (float)height * ySpacing;
  volume_points[5] = (float)nImagesFnd * sliceSpacing;

  FLDset_points (volume_id, volume_points, volume_npoints, OM_SET_ARRAY_FREE);
  
  /* Set number of node data components 
   */
  volume_ncomp = 1;
  FLDset_node_data_ncomp (volume_id, volume_ncomp);
  
  /* For each node data component set veclen, type and data arry itself 
   */
#ifdef XP_STATUS
  status_in_progress=100/(1+nImagesFnd*volume_ncomp);
  OMstatus_check(status_in_progress, "Reading Dicom File", &interrupt);
#endif

  for (comp=0; comp < volume_ncomp; comp++) {
    /* Set veclen, assign volume_veclen before next call 
     */
    volume_veclen = (datatype == 1) ? 4 : 1;
    FLDset_node_data_veclen (volume_id,comp,volume_veclen);
    
    /* Set data array 
     */
    if (cellsize == 1) {
      volume_data_type = DTYPE_BYTE;
    }
    else {
      volume_data_type = DTYPE_SHORT;
    }

    RD_Set_XP_Type(volume_id,"node_data[%d].values",comp,
                volume_data_type);
    RD_Set_XP_Type(volume_id,"node_data[%d].min",comp,
                volume_data_type);
    RD_Set_XP_Type(volume_id,"node_data[%d].max",comp,
                volume_data_type);
    RD_Set_XP_Type(volume_id,"node_data[%d].null_value",comp,
                volume_data_type);
    RD_Set_XP_Type(volume_id,"node_data[%d].min_vec",comp,
                volume_data_type);
    RD_Set_XP_Type(volume_id,"node_data[%d].max_vec",comp,
                volume_data_type);

    volume_ndata = width * height * nImagesFnd;
    volume_data = (char *)ARRalloc(NULL, volume_data_type, volume_veclen*volume_ndata, NULL);

    if (!volume_data) {
      fprintf(stderr, "--- Can't alloc volume_data\n");
      Papy3FileClose(fileNb, TRUE);
      OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
      OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif
      RD_Set_Null_Mesh(volume_id);
      return(1);
    }

    /* close file used for getting info */
    Papy3FileClose(fileNb, TRUE);

    /* read images in according to bitsperpixel and colour type grey/RGBA */
    /* flip =1 will reorder images so that last image goes to the bottom of the volume */
    /* images need to be reorder top to bottom AND left to right ie transposed */
    /* before being added to the volume so that the anatomy is correct */
    /* SINCE THE LEFT TO RIGHT HAS BEEN TACKED ON THIS CODE SHOULD BE REWRITTEN TO DO A SIMPLE TRANSPOSE */
    switch(datatype)
    {
     case 0: /* Greyscale */
      /* FLDset_node_data_id(volume_id, 0, GD_COLOR_DATA_ID);     */
      
      if (cellsize == 1) {
        unsigned char *output_data = (unsigned char *)volume_data;
        
        for(n = 0; n < nImagesFnd; n++) {
	  unsigned char *input_data;
	  /* choose order to read files in */
#ifdef DEBUG
	  printf("Opening slice %d\n", first+n);
#endif
	  if(flip)
	    fileNb=Papy3FileOpen(filenames[nImagesFnd-n-1], 0, TRUE, NULL);
	  else
	    fileNb=Papy3FileOpen(filenames[n], 0, TRUE, NULL);
	  	  
	  if (fileNb < 0)
	  {
	    fprintf(stderr, "--- Can't alloc volume_data\n");
	    Papy3FileClose(fileNb, TRUE);
	    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
	    OMstatus_check(100, "Reading Dicom File", &interrupt);
	    RD_Set_Null_Mesh(volume_id);
#endif
	    return 0;
	  }

	  input_data = (unsigned char *)RD_Get_Pixel_Data(fileNb, 1, height, width, bits, interp);
#ifdef DEBUG
	  printf("Reading image from slice %d\n", first+n);
#endif
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = (n * width * height) + ((height - i) * width)+ width-1;
            for(j = 0; j < width; j++)
              output_data[k--] = *input_data++;
          }

          RD_Free_Pixel_Data(fileNb, 1);
	  Papy3FileClose(fileNb, TRUE);
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+nImagesFnd*volume_ncomp);
          OMstatus_check(status_in_progress, "Reading Dicom File", &interrupt);
#endif
        }
      }
      else {
        short *output_data = (short *)volume_data;
        
        for(n = 0; n < nImagesFnd; n++) {
	  short *input_data;
	  /* choose order to read files in */
	  if(flip)
	    fileNb=Papy3FileOpen(filenames[nImagesFnd-n-1], 0, TRUE, NULL);
	  else
	    fileNb=Papy3FileOpen(filenames[n], 0, TRUE, NULL);
	    
	  if (fileNb < 0) {
	    fprintf(stderr, "--- Can't alloc volume_data\n");
	    Papy3FileClose(fileNb, TRUE);
	    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
	    OMstatus_check(100, "Reading Dicom File", &interrupt);
	    RD_Set_Null_Mesh(volume_id);
#endif
	    return 0;
	  }

          input_data = (short *)RD_Get_Pixel_Data(fileNb, 1, height,
						      width, bits, interp);
#ifdef DEBUG
	  printf("Reading image from slice %d\n", first+n);
#endif
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = (n * width * height) + ((height - i) * width)+ width-1 ;
            for(j = 0; j < width; j++)
              output_data[k--] = *input_data++;
          }

          RD_Free_Pixel_Data(fileNb, 1);
	  Papy3FileClose(fileNb, TRUE);
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+nImagesFnd*volume_ncomp);
          OMstatus_check(status_in_progress, "Reading Dicom File", &interrupt);
#endif
        }
      }
      
      break;
      
    case 1: /* ARGB */
      FLDset_node_data_id(volume_id, 0, GD_RGB_DATA_ID);

      if (cellsize == 1) {
        unsigned char *output_data = (unsigned char *)volume_data;

        for(n = 0; n < nImagesFnd; n++) {
          unsigned char *input_data;
	  
	  /* choose order to read files in */
	  if(flip)
	    fileNb=Papy3FileOpen(filenames[nImagesFnd-n-1], 0, TRUE, NULL);
	  else
	    fileNb=Papy3FileOpen(filenames[n], 0, TRUE, NULL);
	  
	  if (fileNb < 0)
	  {
	    fprintf(stderr, "--- Can't alloc volume_data\n");
	    Papy3FileClose(fileNb, TRUE);
	    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
	    OMstatus_check(100, "Reading Dicom File", &interrupt);
	    RD_Set_Null_Mesh(volume_id);
#endif
	    return 0;
	  }
	  
          input_data = (unsigned char *)RD_Get_Pixel_Data(fileNb,(PapyShort)1,height,width,bits,interp);
#ifdef DEBUG
	  printf("Reading image from slice %d\n", first+n);
#endif
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = ((n * width * height) + ((height - i) * width) +width-1) * 4;
            for(j = 0; j < width; j++) {
              output_data[k-3] = 1;
              output_data[k-2] = *input_data++;
              output_data[k-1] = *input_data++;
              output_data[k] = *input_data++;
	      k-=4;
            }
          }

          RD_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+nImagesFnd*volume_ncomp);
          OMstatus_check(status_in_progress, "Reading Dicom File", &interrupt);
#endif
        }
      }
      else {
        short *output_data = (short *)volume_data;
        
        for(n = 0; n < nImagesFnd; n++) {
          short *input_data;
	  
	  /* choose order to read files in */
	  if(flip)
	    fileNb=Papy3FileOpen(filenames[nImagesFnd-n-1], 0, TRUE, NULL);
	  else
	    fileNb=Papy3FileOpen(filenames[n], 0, TRUE, NULL);
	  
	  if (fileNb < 0)
	  {
	    fprintf(stderr, "--- Can't alloc volume_data\n");
	    Papy3FileClose(fileNb, TRUE);
	    OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
	    OMstatus_check(100, "Reading Dicom File", &interrupt);
	    RD_Set_Null_Mesh(volume_id);
#endif
	    return 0;
	  }
	  
          input_data = (short *)RD_Get_Pixel_Data(fileNb,(PapyShort)1,height,width,bits,interp);
#ifdef DEBUG
	  printf("Reading image from slice %d\n", first+n);
#endif
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = ((n * width * height) + ((height - i) * width) +width-1) * 4;
            for(j = 0; j < width; j++) {
              output_data[k-3] = 1;
              output_data[k-2] = *input_data++;
              output_data[k-1] = *input_data++;
              output_data[k] = *input_data++;
	      k-=4;
            }
          }

          RD_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+nImagesFnd*volume_ncomp);
          OMstatus_check(status_in_progress, "Reading Dicom File", &interrupt);
#endif
        }
      }
      break;

     default:
      fprintf(stderr, "--- Invalid data type\n");
      
      for(i=0; i<nImagesFnd; i++)
	Papy3FileClose(fileNb, TRUE);

      OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
      OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif
      RD_Set_Null_Mesh(volume_id);
      return(0);
    }
    
    FLDset_node_data (volume_id, comp, volume_data, 
      volume_data_type, volume_ndata*volume_veclen, 
      OM_SET_ARRAY_FREE);
  }
  
  /*************************/
  /*  Free input variables */
  /*************************/
  OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("busy"), 0);
  OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("done"), 1);
#ifdef XP_STATUS
  OMstatus_check(100, "Reading Dicom File", &interrupt);
#endif
  
  return(1);
}


/*
  Copyright (c) 1998 by
  Advanced Visual Systems Inc.
  All Rights Reserved
  
  This software comprises unpublished confidential information of
  Advanced Visual Systems Inc. and may not be used, copied or made
  available to anyone, except in accordance with the license
  under which it is furnished.
  
  We used :
      PAPYRUS toolkit v3.4
      Copyright (c) 1996 by
      University Hospital of Geneva
*/

#ifdef MSDOS
#define FILENAME83
#endif

#include <avs/gd_def.h>
#include <avs/f_utils.h>

#include "gen.h"
#include "rd_pyutl.h"

#ifdef MSDOS
#include "PAPYRUS3.H"
#else
#include "Papyrus3.h"
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


int Read_Papyrus_File(OMobj_id Read_Papyrus_File_id,
                      OMevent_mask event_mask, int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  char *filename_ = NULL;
  char filename[AVS_PATH_MAX];

  OMobj_id output_image_id, header_id;
  OMobj_id patient_id, general_study_id, general_series_id, general_equipment_id, image_series_id;
  int output_image_ndim, *output_image_dims, output_image_nspace, output_image_npoints;
  float *output_image_points;
  int output_image_ncomp, comp, output_image_veclen;
  int output_image_data_type, output_image_ndata;
  char *output_image_data;
  PapyShort fileNb;
  /*int bits_allocated=0;*/
  int num_frames=0;
  int i, j, k, n;
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
  float sliceSpacing = 1;
  float xSpacing = 1; 
  float ySpacing = 1; 

  OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 1);
#ifdef XP_STATUS
  OMstatus_check(0, "Reading Papyrus File", &interrupt);
#endif

  if (!papy_inited) {
    Papy3Init();
    papy_inited=TRUE;
  }
  
  /***********************/
  /*  Get input values   */
  /***********************/
  output_image_id = OMfind_subobj(Read_Papyrus_File_id, OMstr_to_name("output_image"), OM_OBJ_RW);

  if (OMis_null_obj(output_image_id)) {
    fprintf(stderr, "--- output_image_id NULL\n");
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    return(0);
  }

  if (OMget_name_str_val(Read_Papyrus_File_id, OMstr_to_name("filename"), &filename_, 0) != 1)
  {
    fprintf(stderr, "--- Can't get filename\n");
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    RP_Set_Null_Mesh(output_image_id);
    return(0);
  }

  /* Use AVS/Express function to map enviroment variables */
  FILEmap_variables(filename_, filename);

  /* Free temporary filename string */
  if (filename_) free(filename_);
  
/*  printf("========================================================\n");
  printf("Reading file %s\n",filename);*/
  fileNb=Papy3FileOpen(filename, 0, TRUE);
  
  if (fileNb<0) {
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    RP_Set_Null_Mesh(output_image_id);
    ERR_RETURN1("--- Can't open filename",filename);
  }
  
  num_frames=Papy3GetNbImages (fileNb);
/*  printf("num_frames = %d\n",num_frames);*/
  
  header_id = OMfind_subobj(Read_Papyrus_File_id, OMstr_to_name("header"), OM_OBJ_RW);
  patient_id = OMfind_subobj(header_id, OMstr_to_name("patient"), OM_OBJ_RW);
  general_study_id = OMfind_subobj(header_id, OMstr_to_name("generalStudy"), OM_OBJ_RW);
  general_series_id = OMfind_subobj(header_id, OMstr_to_name("generalSeries"), OM_OBJ_RW);
  general_equipment_id = OMfind_subobj(header_id, OMstr_to_name("generalEquipment"), OM_OBJ_RW);
  image_series_id = OMfind_subobj(header_id, OMstr_to_name("imageSeries"), OM_OBJ_RW);
  
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
  if (OMis_null_obj(image_series_id))
    fprintf(stderr, "--- image_series_id NULL\n");

  /**************************/
  /*  Fill in image header  */
  /**************************/
  RP_Read_Patient           (fileNb, 1, patient_id);
  RP_Read_General_Study     (fileNb, 1, general_study_id);
  RP_Read_General_Series    (fileNb, 1, general_series_id);
  RP_Read_General_Equipment (fileNb, 1, general_equipment_id);
  OMget_name_str_val(general_series_id,  OMstr_to_name("ModalityGS"), &modality, (int)NULL);
  RP_Read_Image_Series      (fileNb, 1, image_series_id, modality, num_frames, &height, &width, &bits,
                                       &sliceSpacing, &xSpacing, &ySpacing, interp);

  OMset_name_int_val( image_series_id, OMstr_to_name("Slices"), num_frames);
  free(modality);

  /***********************/
  /*  Set output values  */
  /***********************/
  /* replaced with RP_Read_Image_Series */
  /*  RP_Get_Image_Attributes(fileNb, 1, &height, &width, &bits, interp);*/
  
  if (bits<=8) {
    cellsize = 1;
  }
  else if (bits<=16) {
    cellsize = 2;
  }
  else {
    fprintf(stderr, "--- Unsupported element size %d\n", bits);
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    RP_Set_Null_Mesh(output_image_id);
    return(0);
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
  /* Set output_image uniform mesh */
  
  /*  Get mesh id 
   */
  /* output_image_id = OMfind_subobj(Read_Papyrus_File_id, OMstr_to_name("output_image"), OM_OBJ_RW); */
  
  /* Clear that puppy out...
   */
  FLDset_node_data_ncomp(output_image_id, 0);
  
  /* Set mesh dimensionality, output_image_ndim can be 2 or 3 
   */
  if (num_frames==1)
    output_image_ndim = 2;
  else
    output_image_ndim = 3;
  FLDset_ndim (output_image_id, output_image_ndim);
  
  /* Set mesh dims array 
   */
  output_image_dims = (int *)ARRalloc(NULL, DTYPE_INT, output_image_ndim, NULL);
  if (!output_image_dims) {
    fprintf(stderr, "--- Can't alloc output_image_dims\n");
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    RP_Set_Null_Mesh(output_image_id);
    return(0);
  }

  output_image_dims[0] = width;
  output_image_dims[1] = height;
  if (num_frames!=1)
     output_image_dims[2] = num_frames;
  FLDset_dims (output_image_id, output_image_dims);
  ARRfree((char *)output_image_dims);
  
  /* Set mesh nspace, output_image_nspace can be 2 or 3 
   */
  if (num_frames==1)
    output_image_nspace = 2;
  else
    output_image_nspace = 3;
  FLDset_nspace (output_image_id, output_image_nspace);
  
  /* Set mesh extents 
   */
  output_image_npoints = 2 * output_image_nspace;
  output_image_points = (float *)ARRalloc(NULL, DTYPE_FLOAT, output_image_npoints, NULL);
  if (!output_image_points) {
    fprintf(stderr, "--- Can't alloc output_image_points\n");
    Papy3FileClose(fileNb, TRUE);
    OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
    OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
    RP_Set_Null_Mesh(output_image_id);
    return(0);
  }
  
  /* extents of the volume are set in mm */
  if (num_frames==1) {
    output_image_points[0] = 0;
    output_image_points[1] = 0;
    output_image_points[2] = (float)width * xSpacing;
    output_image_points[3] = (float)height * ySpacing;
  }
  else {
    output_image_points[0] = 0;
    output_image_points[1] = 0;
    output_image_points[2] = 0;
    output_image_points[3] = (float)width * xSpacing;
    output_image_points[4] = (float)height * ySpacing;
    output_image_points[5] = (float)num_frames * sliceSpacing;
  }

  FLDset_points (output_image_id, output_image_points, output_image_npoints, OM_SET_ARRAY_FREE);
  
  /* Set number of node data components 
   */
  output_image_ncomp = 1;
  FLDset_node_data_ncomp (output_image_id, output_image_ncomp);
  
  /* For each node data component set veclen, type and data arry itself 
   */
#ifdef XP_STATUS
  status_in_progress=100/(1+num_frames*output_image_ncomp);
  OMstatus_check(status_in_progress, "Reading Papyrus File", &interrupt);
#endif

  for (comp=0; comp < output_image_ncomp; comp++) {
    /* Set veclen, assign output_image_veclen before next call 
     */
    output_image_veclen = (datatype == 1) ? 4 : 1;
    FLDset_node_data_veclen (output_image_id,comp,output_image_veclen);
    
    /* Set data array 
     */
    if (cellsize == 1) {
      output_image_data_type = DTYPE_BYTE;
    }
    else {
      output_image_data_type = DTYPE_SHORT;
    }

    RP_Set_XP_Type(output_image_id,"node_data[%d].values",comp,
                   output_image_data_type);
    RP_Set_XP_Type(output_image_id,"node_data[%d].min",comp,
                   output_image_data_type);
    RP_Set_XP_Type(output_image_id,"node_data[%d].max",comp,
                   output_image_data_type);
    RP_Set_XP_Type(output_image_id,"node_data[%d].null_value",comp,
                   output_image_data_type);
    RP_Set_XP_Type(output_image_id,"node_data[%d].min_vec",comp,
                   output_image_data_type);
    RP_Set_XP_Type(output_image_id,"node_data[%d].max_vec",comp,
                   output_image_data_type);

    output_image_ndata = width * height * num_frames;
    output_image_data = (char *)ARRalloc(NULL, output_image_data_type, output_image_veclen*output_image_ndata, NULL);

    if (!output_image_data) {
      fprintf(stderr, "--- Can't alloc output_image_data\n");
      Papy3FileClose(fileNb, TRUE);
      OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
      OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
      RP_Set_Null_Mesh(output_image_id);
      return(0);
    }
    
    switch(datatype)
    {
     case 0:
      /* FLDset_node_data_id(output_image_id, 0, GD_COLOR_DATA_ID);     */
      
      if (cellsize == 1) {
        unsigned char *output_data = (unsigned char *)output_image_data;
        
        for(n = 0; n < num_frames; n++) {
          unsigned char *input_data = (unsigned char *)RP_Get_Pixel_Data(fileNb,(PapyShort)(n+1),height,width,bits);
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = (n * width * height) + ((height - i) * width);
            for(j = 0; j < width; j++)
              output_data[k++] = *input_data++;
          }

          RP_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+num_frames*output_image_ncomp);
          OMstatus_check(status_in_progress, "Reading Papyrus File", &interrupt);
#endif
        }
      }
      else {
        short *output_data = (short *)output_image_data;
        
        for(n = 0; n < num_frames; n++) {
          short *input_data = (short *)RP_Get_Pixel_Data(fileNb,(PapyShort)(n+1),height,width,bits);
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = (n * width * height) + ((height - i) * width);
            for(j = 0; j < width; j++)
              output_data[k++] = *input_data++;
          }

          RP_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+num_frames*output_image_ncomp);
          OMstatus_check(status_in_progress, "Reading Papyrus File", &interrupt);
#endif
        }
      }
      
      break;
      
     case 1:
      FLDset_node_data_id(output_image_id, 0, GD_RGB_DATA_ID);

      if (cellsize == 1) {
        unsigned char *output_data = (unsigned char *)output_image_data;

        for(n = 0; n < num_frames; n++) {
          unsigned char *input_data = (unsigned char *)RP_Get_Pixel_Data(fileNb,(PapyShort)(n+1),height,width,bits);
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = ((n * width * height) + ((height - i) * width)) * 4;
            for(j = 0; j < width; j++) {
              output_data[k++] = 1;
              output_data[k++] = *input_data++;
              output_data[k++] = *input_data++;
              output_data[k++] = *input_data++;
            }
          }

          RP_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+num_frames*output_image_ncomp);
          OMstatus_check(status_in_progress, "Reading Papyrus File", &interrupt);
#endif
        }
      }
      else {
        short *output_data = (short *)output_image_data;
        
        for(n = 0; n < num_frames; n++) {
          short *input_data = (short *)RP_Get_Pixel_Data(fileNb,(PapyShort)(n+1),height,width,bits);
          if (!input_data) continue;

          for(i = 1; i <= height; i++) {
            k = ((n * width * height) + ((height - i) * width)) * 4;
            for(j = 0; j < width; j++) {
              output_data[k++] = 1;
              output_data[k++] = *input_data++;
              output_data[k++] = *input_data++;
              output_data[k++] = *input_data++;
            }
          }

          RP_Free_Pixel_Data(fileNb,(PapyShort)(n+1));
#ifdef XP_STATUS
          status_in_progress=100*(1+(n+1)*(comp+1))/(1+num_frames*output_image_ncomp);
          OMstatus_check(status_in_progress, "Reading Papyrus File", &interrupt);
#endif
        }
      }
      break;
      
     default:
      fprintf(stderr, "--- Invalid data type\n");
      Papy3FileClose(fileNb, TRUE);
      OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
#ifdef XP_STATUS
      OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif
      RP_Set_Null_Mesh(output_image_id);
      return(0);
    }
    
    FLDset_node_data (output_image_id, comp, output_image_data, 
                      output_image_data_type, output_image_ndata*output_image_veclen, 
                      OM_SET_ARRAY_FREE);
   }
  
  /*************************/
  /*  Free input variables */
  /*************************/
  Papy3FileClose(fileNb, TRUE);
  OMset_name_int_val(Read_Papyrus_File_id, OMstr_to_name("busy"), 0);
  OMset_name_int_val(Read_Dicom_File_id, OMstr_to_name("done"), 1);
#ifdef XP_STATUS
  OMstatus_check(100, "Reading Papyrus File", &interrupt);
#endif

  return(1);
}


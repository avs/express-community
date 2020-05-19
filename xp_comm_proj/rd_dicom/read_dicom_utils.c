#ifdef MSDOS
#define FILENAME83
#endif

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


/* Modifications for use with CAMARA ESP */
/* by James S Perrin */
/* now uses Papyrus 3.5 */
/* ditch status reporting */
/* reads in a set of raw dicom images files (with which Papyrus is compatible) */
/* and constructs the volume */
/* sets extents and gets additional info windowing etc */
/* split utility fns into separate file */

/* #define DEBUG */
#undef XP_STATUS
/* Major functional change */
/* Given the first raw image file of series read them into a single volume */
#define MAX_SLICES 512

#include "gen.h"
#include "avs/gd_def.h"
#include "Papyrus3.h"
#include "read_dicom_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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

static Module *pixel_data_module = NULL;

void RD_Set_Null_Mesh(OMobj_id elem_id)
{
  int dims[3];

  dims[0] = 0; dims[1] = 0; dims[2] = 0;

  if (!OMis_null_obj(elem_id)) {
    FLDset_nnodes(elem_id, 0); /* no coords */
    FLDset_dims(elem_id, dims);
    FLDset_node_data_ncomp(elem_id, 0); /* no node data */
    FLDset_ncell_sets(elem_id, 0); /* no cell sets */
  }
}

void RD_Decode_String(UValue_T *in, int nbVal, char *out)
{
  UValue_T *tmp=in;
  char *p;
  int i;

  strcpy(out,"");

  for(i=0; i<nbVal; i++,tmp++) {
    if (tmp) {
      if (i) strcat(out, " ; ");
      if (tmp->a && *(tmp->a))
        strcat(out,tmp->a);
    }
    else {
      continue;
    }

    p=out;
    while(*p) {
      if (*p=='^') *p=' ';
      p++;
    }

    p=out+strlen(out)-1;
    while(p>=out && *p==' ') {
      *p='\0';
      p--;
    }
  }
}

void RD_Decode_Date(UValue_T *in, char *out)
{
  int year,month,day;

  if (in) {
    sscanf(in->a, "%4d%2d%2d", &year, &month, &day);
    sprintf(out, "%02d/%02d/%04d", day, month, year);
  }
  else {
    strcpy(out,"");
  }
}

void RD_Decode_Time(UValue_T *in, char *out)
{
  int hour, min, sec, msec;

  if (in) {
    sscanf(in->a, "%2d%2d%2d.%d", &hour, &min, &sec, &msec);
    sprintf(out, "%02d:%02d:%02d", hour, min, sec);
  }
  else {
    strcpy(out, "");
  }
}

char *RD_Read_String(Module *papy_mod,   int papy_name,
                         OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  UValue_T *val;
  static char buf[1024];

  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==AE || itemType==AS || itemType==CS || itemType==DS ||
      itemType==DT || itemType==IS || itemType==LO || itemType==LT ||
      itemType==OB || itemType==PN || itemType==SH || itemType==ST ||
      itemType==UI)
  {
    RD_Decode_String(val, nbVal, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RD_Read_String : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

char *RD_Read_Date(Module *papy_mod,   int papy_name,
                       OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  UValue_T *val;
  static char buf[1024];

  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==DA)
  {
    RD_Decode_Date(val, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RD_Read_Date : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

char *RD_Read_Time(Module *papy_mod,   int papy_name,
                       OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  UValue_T *val;
  static char buf[1024];

  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==TM)
  {
    RD_Decode_Time(val, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RD_Read_Time : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

void RD_Read_List(Module *papy_mod,   int papy_name,
                      OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  UValue_T *val;
  char buf[1024];

  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==SQ)
  {
    *buf='\0';
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RD_Read_List : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);
}

PapyUShort RD_Read_Ushort(Module *papy_mod, int papy_name,
                          OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  UValue_T *val;
  int res=0;

  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==USS)
  {
    if (val) {
      res=val->us;
/*      printf("%s : %d\n", xp_name, res);*/
    }
  }
  else {
    fprintf(stderr, "--- RD_Read_Ushort : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_int_val(xp_parent, OMstr_to_name(xp_name), res);

  return((PapyUShort)res);
}

void RD_Read_Patient(PapyShort fileNb, PapyShort imageNb,
                     OMobj_id Patient_Id)
{
  Module *module;

  module = Papy3GetModule(fileNb, (short)imageNb, Patient);
  if (!module) return;

  RD_Read_String(module,papPatientsNameP, Patient_Id, "PatientsNameP");
  RD_Read_String(module,papPatientIDP, Patient_Id, "PatientIDP");
  RD_Read_Date  (module,papPatientsBirthDateP, Patient_Id, "PatientsBirthDateP");
  RD_Read_String(module,papPatientsSexP, Patient_Id, "PatientsSexP");
  RD_Read_List  (module,papReferencedPatientSequenceP, Patient_Id, "ReferencedPatientSequenceP");
  RD_Read_Time  (module,papPatientsBirthTimeP, Patient_Id, "PatientsBirthTimeP");
  RD_Read_String(module,papOtherPatientID, Patient_Id, "OtherPatientID");
  RD_Read_String(module,papOtherPatientNamesP, Patient_Id, "OtherPatientNamesP");
  RD_Read_String(module,papEthnicGroupP, Patient_Id, "EthnicGroupP");
  RD_Read_String(module,papPatientCommentsP, Patient_Id, "PatientCommentsP");

  Papy3ModuleFree(&module, Patient, TRUE);
}

void RD_Read_General_Study(PapyShort fileNb, PapyShort imageNb,
                           OMobj_id Patient_Id)
{
  Module *module;

  module = Papy3GetModule(fileNb, (short)imageNb, GeneralStudy);
  if (!module) return;

  RD_Read_String(module,papStudyInstanceUIDGS, Patient_Id, "StudyInstanceUIDGS");
  RD_Read_Date  (module,papStudyDateGS, Patient_Id, "StudyDateGS");
  RD_Read_Time  (module,papStudyTimeGS, Patient_Id, "StudyTimeGS");
  RD_Read_String(module,papReferringPhysiciansNameGS, Patient_Id, "ReferringPhysiciansNameGS");
  RD_Read_String(module,papStudyIDGS, Patient_Id, "StudyIDGS");
  RD_Read_String(module,papAccessionNumberGS, Patient_Id, "AccessionNumberGS");
  RD_Read_String(module,papStudyDescriptionGS, Patient_Id, "StudyDescriptionGS");
  RD_Read_String(module,papNameofPhysiciansReadingStudyGS, Patient_Id, "NameofPhysiciansReadingStudyGS");
  RD_Read_List  (module,papReferencedStudySequenceGS, Patient_Id, "ReferencedStudySequenceGS");

  Papy3ModuleFree(&module, GeneralStudy, TRUE);
}

void RD_Read_General_Series(PapyShort fileNb, PapyShort imageNb,
                            OMobj_id Patient_Id)
{
  Module *module;

  module = Papy3GetModule(fileNb, (short)imageNb, GeneralSeries);
  if (!module) return;

  RD_Read_String(module, papModalityGS, Patient_Id, "ModalityGS");
  RD_Read_String(module, papSeriesInstanceUIDGS, Patient_Id, "SeriesInstanceUIDGS");
  RD_Read_String(module, papSeriesNumberGS, Patient_Id, "SeriesNumberGS");
  RD_Read_String(module, papLaterality, Patient_Id, "Laterality");
  RD_Read_Date  (module, papSeriesDate, Patient_Id, "SeriesDate");
  RD_Read_Time  (module,papSeriesTime, Patient_Id, "SeriesTime");
  RD_Read_String(module,papPerformingPhysiciansNameGS, Patient_Id, "PerformingPhysiciansNameGS");
  RD_Read_String(module,papProtocolName, Patient_Id, "ProtocolName");
  RD_Read_String(module,papSeriesDescription, Patient_Id, "SeriesDescription");
  RD_Read_String(module,papOperatorsName, Patient_Id, "OperatorsName");
  RD_Read_List  (module,papReferencedStudyComponentSequenceGS, Patient_Id, "ReferencedStudyComponentSequenceGS");
  RD_Read_String(module,papBodyPartExaminedGS, Patient_Id, "BodyPartExaminedGS");
  RD_Read_String(module,papPatientPosition, Patient_Id, "PatientPosition");
  RD_Read_Ushort(module,papSmallestPixelValueinSeries, Patient_Id, "SmallestPixelValueinSeries");
  RD_Read_Ushort(module,papLargestPixelValueinSeries, Patient_Id, "LargestPixelValueinSeries");

  Papy3ModuleFree(&module, GeneralSeries, TRUE);

  return;
}

void RD_Read_General_Equipment(PapyShort fileNb, PapyShort imageNb,
                               OMobj_id Patient_Id)
{
  Module *module;

  module = Papy3GetModule(fileNb, (short)imageNb, GeneralEquipment);
  if (!module) return;

  RD_Read_String(module,papManufacturerGE, Patient_Id, "ManufacturerGE");
  RD_Read_String(module,papInstitutionNameGE, Patient_Id, "InstitutionNameGE");
  RD_Read_String(module,papInstitutionAddressGE, Patient_Id, "InstitutionAddressGE");
  RD_Read_String(module,papStationName, Patient_Id, "StationName");
  RD_Read_String(module,papInstitutionalDepartmentName, Patient_Id, "InstitutionalDepartmentName");
  RD_Read_String(module,papManufacturersModelName, Patient_Id, "ManufacturersModelName");
  RD_Read_String(module,papDeviceSerialNumberGE, Patient_Id, "DeviceSerialNumberGE");
  RD_Read_String(module,papSoftwareVersionsGE, Patient_Id, "SoftwareVersionsGE");
  RD_Read_String(module,papSpatialResolution, Patient_Id, "SpatialResolution");
  RD_Read_Date  (module,papDateofLastCalibration, Patient_Id, "DateofLastCalibration");
  RD_Read_Time  (module,papTimeofLastCalibration, Patient_Id, "TimeofLastCalibration");
  RD_Read_Ushort(module,papPixelPaddingValue, Patient_Id, "PixelPaddingValue");

  Papy3ModuleFree(&module, GeneralEquipment, TRUE);
}


void RD_Read_Image_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Image_Series_Id,
			  char *modality,
			  int *height, int *width, int *bits,
			  float *sliceSpacing, float *XSpacing, float *YSpacing, char *interp,
			  char *filename)
{
  Module *module, *module2;
  char *tmpStr;
  float tmp, tmp2;
  PapyShort fileNb2;

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePixel);
  if (!module) return;

  strcpy(interp, RD_Read_String(module, papPhotometricInterpretationIP, OMnull_obj, "Interpretation"));
  *height = RD_Read_Ushort(module, papRows, Image_Series_Id, "Rows");
  *width = RD_Read_Ushort(module, papColumns, Image_Series_Id, "Columns");
  *bits = RD_Read_Ushort(module, papBitsAllocatedIP, Image_Series_Id, "BitsAllocated");
  Papy3ModuleFree(&module, ImagePixel, TRUE);

  /* get slice spacing */
  *sliceSpacing = 1; /* default */
  module = Papy3GetModule(fileNb, (short)imageNb, MRImage);
  if (!module) return;
  tmp = atof(RD_Read_String(module, papSpacingBetweenSlices, OMnull_obj, "SliceSpacing"));
  Papy3ModuleFree(&module, MRImage, TRUE);

  if(tmp)
  {
    *sliceSpacing = tmp;
  } else if(filename) {
    /* do it the hard way determine from image position - usually needed for CT */
    float x1, y1, z1, x2, y2, z2, x, y, z;
    fileNb2 = Papy3FileOpen(filename, 0, TRUE, NULL);
    module = Papy3GetModule(fileNb, (short)imageNb, ImagePlane);
    module2 = Papy3GetModule(fileNb2, (short)imageNb, ImagePlane);
    tmp = atof(strtok(RD_Read_String(module, papImagePositionPatient, OMnull_obj, "ImagePositionPatient"), "; "));

    if(tmp)
    {
      x1 = tmp;
      y1 = atof(strtok((char *)0, "; "));
      z1 = atof(strtok((char *)0, "; "));
      tmp2 = atof(strtok(RD_Read_String(module2, papImagePositionPatient, OMnull_obj, "ImagePositionPatient"), "; "));
      x2 = tmp2;
      y2 = atof(strtok((char *)0, "; "));
      z2 = atof(strtok((char *)0, "; "));
      x = x2-x1; y = y2-y1; z = z2-z1;
      *sliceSpacing = sqrt(x*x+y*y+z*z);
    }
    Papy3ModuleFree(&module, ImagePlane, TRUE);
    Papy3ModuleFree(&module2, ImagePlane, TRUE);
    Papy3FileClose(fileNb2, TRUE);
  }

  OMset_name_real_val(Image_Series_Id, OMstr_to_name("SliceSpacing"), *sliceSpacing);

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePlane);
  if (!module) return;
  /* pixelSpacing has the form "x ; y" */
  /* get XSpacing */
  tmpStr = RD_Read_String(module, papPixelSpacing, OMnull_obj, "PixelSpacing");
  if( tmpStr != NULL && tmpStr[0] != 0)
    tmp = atof(strtok( tmpStr, ";"));
  else tmp = 0.0;
  if( tmp != 0.0 ) {
    *XSpacing = tmp;
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("XSpacing"), tmp);
  }
  else {
    *XSpacing = 1;
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("XSpacing"), 1);
  }

  /* get YSpacing */
  if( tmpStr != NULL && tmpStr[0] != 0 )
    tmp = atof(strtok((char *)0, ";"));
  else tmp = 0.0;
  if( tmp != 0.0 ) {
    *YSpacing = tmp;
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("YSpacing"), tmp);
  }
  else {
    *YSpacing = 1;
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("YSpacing"), 1);
  }
  Papy3ModuleFree(&module, ImagePlane, TRUE);

  /* get Windowing */
  module = Papy3GetModule(fileNb, (short)imageNb, VOILUT);
  if (!module) return;

  tmp = atof( RD_Read_String(module, papWindowCenter, OMnull_obj, "WindowCentre"));
  if(tmp)
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowCentre"), tmp);
  else
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowCentre"), 100);

  tmp = atof(RD_Read_String(module, papWindowWidth, OMnull_obj, "WindowWidth"));
  if(tmp)
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowWidth"), tmp);
  else
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowWidth"), 500);

  Papy3ModuleFree(&module, VOILUT, TRUE);
}


void RD_Get_Image_Attributes(PapyShort fileNb, int imageNb,
                             int *height, int *width, int *bits,
                             char *interp)
{
  Module *module;

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePixel);
  if (!module) return;

  if (interp) {
    strcpy(interp,RD_Read_String(module,papPhotometricInterpretationIP,
                                 OMnull_obj, "PhotometricInterpretationIP"));
  }
  if (height) {
    *height = RD_Read_Ushort(module, papRows, OMnull_obj, "Rows");
  }
  if (width) {
    *width = RD_Read_Ushort(module, papColumns, OMnull_obj, "Columns");
  }
  if (bits) {
    *bits = RD_Read_Ushort(module, papBitsAllocatedIP, OMnull_obj,
                           "BitsAllocatedIP");
  }

  Papy3ModuleFree(&module, ImagePixel, TRUE);


#if 0
  /* This is how you would start in order to support directly
   * reading 3D volumes out of DICOM files.
   */
  module = Papy3GetModule(fileNb, (short)imageNb, Multi_Frame);
  if (!module) return;
  {
    UValue_T *val;
    PapyULong nbVal;
    int itemType;
    val = Papy3GetElement(module, papNumberofFrames, &nbVal, &itemType);
    if( itemType == IS ) {
      fprintf( stderr, "Number of Frames %s\n", val->a );
    }
  }
  Papy3ModuleFree(&module, Multi_Frame, TRUE);
#endif

};

PapyUShort *RD_Get_Pixel_Data(PapyShort fileNb, PapyShort imageNb,
                                  int h, int w, int b,
                                  char *inter)
{
  Module *module;
  PapyUShort *theImage=NULL;
  /*
  int height, width, bits;
  char interp[80];
  */
/*  printf("=======================\n");
  printf("Reading image %d\n",(int)imageNb);*/

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePixel);
  if (!module) return(NULL);

  /*
  strcpy(interp,RD_Read_String(module,papPhotometricInterpretationIP,
                            OMnull_obj, "PhotometricInterpretationIP"));
  height=RD_Read_Ushort(module,papRows,
                      OMnull_obj, "Rows");
  width=RD_Read_Ushort(module,papColumns,
                     OMnull_obj, "Columns");
  bits=RD_Read_Ushort(module,papBitsAllocatedIP,
                              OMnull_obj, "BitsAllocatedIP");

  if (strncmp(interp,inter,3) || height!=h || width!=w || bits!=bits) {
    ERR("--- Different image format in Get_Pixel_Data");
  }
  */
  if (b<=8)
    theImage = Papy3GetPixelData(fileNb, imageNb, module, ImagePixel);
  else if (b<=16)
    theImage = Papy3GetPixelData(fileNb, imageNb, module, ImagePixel);

  if (!theImage) {
    ERR1("--- Impossible to read image %d", (int)imageNb);
  }

  pixel_data_module = module;

  return(theImage);
};

void RD_Free_Pixel_Data(PapyShort fileNb, PapyShort imageNb)
{
/*  printf("Freeing image %d\n",(int)imageNb);*/
  if (!pixel_data_module) {
    ERR1("--- Impossible to free image %d",(int)imageNb);
    return;
  }

  Papy3ImageFree((SElement *)pixel_data_module);

  Papy3ModuleFree((SElement **)&pixel_data_module, ImagePixel, TRUE);
  pixel_data_module = NULL;
}

void RD_Set_XP_Type(OMobj_id parent, char *name, int comp, int type)
{
  OMobj_id id;
  char buf[1024];

  if (name && *name && !OMis_null_obj(parent)) {
    sprintf(buf,name,comp);
    id=OMfind_str_subobj(parent,buf,OM_OBJ_RW);
    if (!OMis_null_obj(id)) {
      OMset_data_type(id,type);
    }
    else {
      fprintf(stderr, "--- Can't set type to %s\n",name);
    }
  }
  else {
    fprintf(stderr, "--- Can't set type to %s\n",name);
  }
}

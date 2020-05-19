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
  
  We used :
      PAPYRUS toolkit v3.4
      Copyright (c) 1996 by
      University Hospital of Geneva
*/


/* Modifications by James S Perrin */
/* now uses Papyrus 3.5 */
/* ditch status reporting */
/* sets extents and gets additional info windowing etc */
/* split utility fns into separate file */

/*#define DEBUG */
#undef XP_STATUS

#include "avs/gd_def.h"

#include "gen.h"
#include "rd_pyutl.h"

#ifdef MSDOS
#include "PAPYRUS3.H"
#else
#include "Papyrus3.h"
#endif

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

static MODULE *pixel_data_module=NULL;

void RP_Set_Null_Mesh(OMobj_id elem_id) 
{
  int ndim;
  int dims2[2] = {0, 0};
  int dims3[3] = {0, 0, 0};
  
  if (OMis_null_obj(elem_id)) {
    return;
  }

  if (FLDget_ndim(elem_id, &ndim) != OM_STAT_SUCCESS) {
#ifdef DEBUG
    printf("Can't get ndim\n");
#endif
    return;
  }

  if (ndim==2) {
#ifdef DEBUG
    printf("ndim=2\n");
#endif
    /* FLDset_nnodes(elem_id, 0); */ /* no coords */
    FLDset_dims(elem_id, dims2);
    FLDset_node_data_ncomp(elem_id, 0); /* no node data */
    FLDset_ncell_sets(elem_id, 0); /* no cell sets */
  }
  else if (ndim==3) {
#ifdef DEBUG
    printf("ndim=3\n");
#endif
    /* FLDset_nnodes(elem_id, 0); */ /* no coords */
    FLDset_dims(elem_id, dims3);
    FLDset_node_data_ncomp(elem_id, 0); /* no node data */
    FLDset_ncell_sets(elem_id, 0); /* no cell sets */
  }
}


void RP_Decode_String(VALUE_T *in, int nbVal, char *out)
{
  VALUE_T *tmp=in;
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

void RP_Decode_Date(VALUE_T *in, char *out)
{
  int year,month,day;
  
  if (in) {
    sscanf(in->a,"%4d%2d%2d",&year,&month,&day);
    sprintf(out,"%02d/%02d/%04d",day,month,year);
  }
  else {
    strcpy(out,"");
  }
}

void RP_Decode_Time(VALUE_T *in, char *out)
{
  int hour,min,sec,msec;
  
  if (in) {
    sscanf(in->a,"%2d%2d%2d.%d",&hour,&min,&sec,&msec);
    sprintf(out,"%02d:%02d:%02d",hour,min,sec);
  }
  else {
    strcpy(out,"");
  }
}

char *RP_Read_String(MODULE *papy_mod,   int papy_name,
                         OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  VALUE_T *val;
  static char buf[1024];
  
  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==AE || itemType==AS || itemType==CS || itemType==DS ||
      itemType==DT || itemType==IS || itemType==LO || itemType==LT ||
      itemType==OB || itemType==PN || itemType==SH || itemType==ST ||
      itemType==UI)
  {
    RP_Decode_String(val, nbVal, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RP_Read_String : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

char *RP_Read_Date(MODULE *papy_mod,   int papy_name,
                       OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  VALUE_T *val;
  static char buf[1024];
  
  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==DA)
  {
    RP_Decode_Date(val, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RP_Read_Date : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

char *RP_Read_Time(MODULE *papy_mod,   int papy_name,
                       OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  VALUE_T *val;
  static char buf[1024];
  
  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==TM)
  {
    RP_Decode_Time(val, buf);
/*    if (*buf) printf("%s : %s\n", xp_name, buf);*/
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RP_Read_Time : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);

  return(buf);
}

void RP_Read_List(MODULE *papy_mod,   int papy_name,
                      OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  VALUE_T *val;
  char buf[1024];
  
  val = Papy3GetElement(papy_mod, papy_name, &nbVal, &itemType);
  if (itemType==SQ)
  {
    *buf='\0';
  }
  else {
    *buf='\0';
    fprintf(stderr, "--- RP_Read_List : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_str_val(xp_parent, OMstr_to_name(xp_name), buf);
}

PapyUShort RP_Read_Ushort(MODULE *papy_mod,   int papy_name,
                              OMobj_id xp_parent, char *xp_name)
{
  PapyULong nbVal;
  int itemType;
  VALUE_T *val;
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
    fprintf(stderr, "--- RP_Read_Ushort : Incompatible type %d for %s\n",
            itemType, xp_name);
  }
  if (xp_name && *xp_name && !OMis_null_obj(xp_parent))
    OMset_name_int_val(xp_parent, OMstr_to_name(xp_name), res);

  return((PapyUShort)res);
}

void RP_Read_Patient(PapyShort fileNb, PapyShort imageNb,
                         OMobj_id Patient_Id)
{
  MODULE *module;
  
  module = Papy3GetModule(fileNb, (short)imageNb, Patient);
  if (!module) return;
  
  RP_Read_String(module,papPatientsNameP, Patient_Id, "PatientsNameP");
  RP_Read_String(module,papPatientIDP, Patient_Id, "PatientIDP");
  RP_Read_Date  (module,papPatientsBirthDateP, Patient_Id, "PatientsBirthDateP");
  RP_Read_String(module,papPatientsSexP, Patient_Id, "PatientsSexP");
  RP_Read_List  (module,papReferencedPatientSequenceP, Patient_Id, "ReferencedPatientSequenceP");
  RP_Read_Time  (module,papPatientsBirthTimeP, Patient_Id, "PatientsBirthTimeP");
  RP_Read_String(module,papOtherPatientID, Patient_Id, "OtherPatientID");
  RP_Read_String(module,papOtherPatientNamesP, Patient_Id, "OtherPatientNamesP");
  RP_Read_String(module,papEthnicGroupP, Patient_Id, "EthnicGroupP");
  RP_Read_String(module,papPatientCommentsP, Patient_Id, "PatientCommentsP");
  
  Papy3ModuleFree(&module, Patient, TRUE);
}

void RP_Read_General_Study(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id)
{
  MODULE *module;
  
  module = Papy3GetModule(fileNb, (short)imageNb, GeneralStudy);
  if (!module) return;
  
  RP_Read_String(module,papStudyInstanceUIDGS, Patient_Id, "StudyInstanceUIDGS");
  RP_Read_Date  (module,papStudyDateGS, Patient_Id, "StudyDateGS");
  RP_Read_Time  (module,papStudyTimeGS, Patient_Id, "StudyTimeGS");
  RP_Read_String(module,papReferringPhysiciansNameGS, Patient_Id, "ReferringPhysiciansNameGS");
  RP_Read_String(module,papStudyIDGS, Patient_Id, "StudyIDGS");
  RP_Read_String(module,papAccessionNumberGS, Patient_Id, "AccessionNumberGS");
  RP_Read_String(module,papStudyDescriptionGS, Patient_Id, "StudyDescriptionGS");
  RP_Read_String(module,papNameofPhysiciansReadingStudyGS, Patient_Id, "NameofPhysiciansReadingStudyGS");
  RP_Read_List  (module,papReferencedStudySequenceGS, Patient_Id, "ReferencedStudySequenceGS");
  
  Papy3ModuleFree(&module, GeneralStudy, TRUE);
}

void RP_Read_General_Series(PapyShort fileNb, PapyShort imageNb,
                                OMobj_id Patient_Id)
{
  MODULE *module;
    
  module = Papy3GetModule(fileNb, (short)imageNb, GeneralSeries);
  if (!module) return;
  
  RP_Read_String(module, papModalityGS, Patient_Id, "ModalityGS");
  RP_Read_String(module, papSeriesInstanceUIDGS, Patient_Id, "SeriesInstanceUIDGS");
  RP_Read_String(module, papSeriesNumberGS, Patient_Id, "SeriesNumberGS");
  RP_Read_String(module, papLaterality, Patient_Id, "Laterality");
  RP_Read_Date  (module, papSeriesDate, Patient_Id, "SeriesDate");
  RP_Read_Time  (module,papSeriesTime, Patient_Id, "SeriesTime");
  RP_Read_String(module,papPerformingPhysiciansNameGS, Patient_Id, "PerformingPhysiciansNameGS");
  RP_Read_String(module,papProtocolName, Patient_Id, "ProtocolName");
  RP_Read_String(module,papSeriesDescription, Patient_Id, "SeriesDescription");
  RP_Read_String(module,papOperatorsName, Patient_Id, "OperatorsName");
  RP_Read_List  (module,papReferencedStudyComponentSequenceGS, Patient_Id, "ReferencedStudyComponentSequenceGS");
  RP_Read_String(module,papBodyPartExaminedGS, Patient_Id, "BodyPartExaminedGS");
  RP_Read_String(module,papPatientPosition, Patient_Id, "PatientPosition");
  RP_Read_Ushort(module,papSmallestPixelValueinSeries, Patient_Id, "SmallestPixelValueinSeries");
  RP_Read_Ushort(module,papLargestPixelValueinSeries, Patient_Id, "LargestPixelValueinSeries");
  
  Papy3ModuleFree(&module, GeneralSeries, TRUE);

  return;
}

void RP_Read_General_Equipment(PapyShort fileNb, PapyShort imageNb,
                                   OMobj_id Patient_Id)
{
  MODULE *module;
  
  module = Papy3GetModule(fileNb, (short)imageNb, GeneralEquipment);
  if (!module) return;
  
  RP_Read_String(module,papManufacturerGE, Patient_Id, "ManufacturerGE");
  RP_Read_String(module,papInstitutionNameGE, Patient_Id, "InstitutionNameGE");
  RP_Read_String(module,papInstitutionAddressGE, Patient_Id, "InstitutionAddressGE");
  RP_Read_String(module,papStationName, Patient_Id, "StationName");
  RP_Read_String(module,papInstitutionalDepartmentName, Patient_Id, "InstitutionalDepartmentName");
  RP_Read_String(module,papManufacturersModelName, Patient_Id, "ManufacturersModelName");
  RP_Read_String(module,papDeviceSerialNumberGE, Patient_Id, "DeviceSerialNumberGE");
  RP_Read_String(module,papSoftwareVersionsGE, Patient_Id, "SoftwareVersionsGE");
  RP_Read_String(module,papSpatialResolution, Patient_Id, "SpatialResolution");
  RP_Read_Date  (module,papDateofLastCalibration, Patient_Id, "DateofLastCalibration");
  RP_Read_Time  (module,papTimeofLastCalibration, Patient_Id, "TimeofLastCalibration");
  RP_Read_Ushort(module,papPixelPaddingValue, Patient_Id, "PixelPaddingValue");
  
  Papy3ModuleFree(&module, GeneralEquipment, TRUE);
}


void RP_Read_Image_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Image_Series_Id,
				 char *modality, int num_frames,
				 int *height, int *width, int *bits,
				 float *sliceSpacing, float *XSpacing, float *YSpacing, char *interp)
{
  MODULE *module;
  char *token = NULL;
  int gotSliceSpacing = 0;
  float tmp;

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePixel);
  if (!module) return;
  
  strcpy(interp, RP_Read_String(module, papPhotometricInterpretationIP, Image_Series_Id, "Interpretation"));
  *height = RP_Read_Ushort(module, papRows, Image_Series_Id, "Rows");
  *width = RP_Read_Ushort(module, papColumns, Image_Series_Id, "Columns");
  *bits = RP_Read_Ushort(module, papBitsAllocatedIP, Image_Series_Id, "BitsAllocated");
  Papy3ModuleFree(&module, ImagePixel, TRUE);

  /* if MR get SliceSpacing */
  if ( (num_frames!=1) && (!strcmp(modality, "MR")) )
  {
    module = Papy3GetModule(fileNb, (short)imageNb, MRImage);
    if (!module) return;
    
    tmp = atof(RP_Read_String(module, papSpacingBetweenSlices, OMnull_obj, "SliceSpacing"));
    if(tmp) {
      *sliceSpacing = tmp; gotSliceSpacing = 1;
      OMset_name_real_val(Image_Series_Id, OMstr_to_name("SliceSpacing"), *sliceSpacing);
    }
    
    Papy3ModuleFree(&module, ImagePixel, TRUE);
  }
  
  module = Papy3GetModule(fileNb, (short)imageNb, ImagePlane);
  if (!module) return;

  /* pixelSpacing has the form or either "x" or "x ; y" */

  /* get XSpacing */
  token = strtok(RP_Read_String(module, papPixelSpacing, OMnull_obj, "PixelSpacing"), ";");
  if (token!=NULL) {
    tmp = atof(token);

    if(tmp) {
      *XSpacing = tmp;
    } else {
      *XSpacing = 1;
    }
  
    /* get YSpacing */
    token = strtok((char *)0, ";");
    if (token!=NULL) {
      tmp = atof(token);
      if(tmp) {
        *YSpacing = tmp;
      } else {
        *YSpacing = 1;
      }
    }
    else {
      *YSpacing = *XSpacing;
    }
  }
  else {
   *XSpacing = 1;
   *YSpacing = 1;
  }

  OMset_name_real_val(Image_Series_Id, OMstr_to_name("XSpacing"), *XSpacing);
  OMset_name_real_val(Image_Series_Id, OMstr_to_name("YSpacing"), *YSpacing);

  /* if image is anything other than MR get SliceThickness */
  /* NOTE: This doesn't seem to be right for anything other than CT images */
  /*       Someone with more experience should be able to sort this */
  /* if ( (num_frames!=1) && (strcmp(modality, "MR")) )*/

  /* if CT get SliceThickness */
  if ( (num_frames!=1) && (!strcmp(modality, "CT")) )
  {
    tmp = atof(RP_Read_String(module, papSliceThickness, OMnull_obj, "SliceSpacing"));
    if(tmp) {
      *sliceSpacing = tmp; gotSliceSpacing = 1;
      OMset_name_real_val(Image_Series_Id, OMstr_to_name("SliceSpacing"), *sliceSpacing);
    }
  }
  
  Papy3ModuleFree(&module, ImagePlane, TRUE);

  if ((num_frames!=1) && (!gotSliceSpacing)) {
    *sliceSpacing = 1;
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("SliceSpacing"), *sliceSpacing);
  }

  /* get Windowing */
  module = Papy3GetModule(fileNb, (short)imageNb, VOILUT);
  if (!module) return;
  
  tmp = atof( RP_Read_String(module, papWindowCenter, OMnull_obj, "WindowCentre"));
  if(tmp)
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowCentre"), tmp);
  else 
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowCentre"), 100);

  tmp = atof(RP_Read_String(module, papWindowWidth, OMnull_obj, "WindowWidth"));
  if(tmp)
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowWidth"), tmp);
  else
    OMset_name_real_val(Image_Series_Id, OMstr_to_name("WindowWidth"), 500);

  Papy3ModuleFree(&module, VOILUT, TRUE);
}


PapyUShort *RP_Get_Pixel_Data(PapyShort fileNb, PapyShort imageNb,
                              int h, int w, int bits)
{
  MODULE *module;
  PapyUShort *theImage=NULL;

/*  printf("=======================\n");
  printf("Reading image %d\n",(int)imageNb);*/

  module = Papy3GetModule(fileNb, (short)imageNb, ImagePixel);
  if (!module) return(NULL);
  
  if (bits <= 8)
    theImage=Papy3GetPixelData(fileNb, imageNb, module, ImagePixel, OB);
  else if (bits <= 16)
    theImage=Papy3GetPixelData(fileNb, imageNb, module, ImagePixel, OW);

  if (!theImage) {
    ERR1("--- Impossible to read image %d",(int)imageNb);
  }
  pixel_data_module = module;

  return(theImage);
};


void RP_Free_Pixel_Data(PapyShort fileNb, PapyShort imageNb)
{
/*  printf("Freeing image %d\n",(int)imageNb);*/
  if (!pixel_data_module) {
    ERR1("--- Impossible to free image %d",(int)imageNb);
    return;
  }

  Papy3ImageFree(pixel_data_module);

  Papy3ModuleFree(&pixel_data_module, ImagePixel, TRUE);
  pixel_data_module = NULL;
}


void RP_Set_XP_Type(OMobj_id parent, char *name, int comp, int type)
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


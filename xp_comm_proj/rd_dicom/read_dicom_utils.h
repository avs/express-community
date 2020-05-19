
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


#include "gen.h"
#include "avs/gd_def.h"
#include "Papyrus3.h"

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

void RD_Set_Null_Mesh(OMobj_id elem_id);
void RD_Decode_String(UValue_T *in, int nbVal, char *out);
void RD_RD_Decode_Date(UValue_T *in, char *out);
void Decode_Time(UValue_T *in, char *out);
char *RD_Read_String(Module *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
char *RD_Read_Date(Module *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
char *RD_Read_Time(Module *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
void RD_Read_List(Module *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
PapyUShort Read_Ushort(Module *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
void RD_Read_Patient(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RD_Read_General_Study(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RD_Read_General_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RD_Read_General_Equipment(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RD_Read_Image_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Image_Series_Id,
			      char *modality, int *height, int *width, int *bits,
			      float *sliceSpacing, float *XSpacing, float *YSpacing, char *interp, char *filename);
void RD_Get_Image_Attributes(PapyShort fileNb, int imageNb,
				 int *height, int *width, int *bits, char *interp);
PapyUShort *RD_Get_Pixel_Data(PapyShort fileNb, PapyShort imageNb, int h, int w, int b, char *inter);
void RD_Free_Pixel_Data(PapyShort fileNb, PapyShort imageNb);
void RD_Set_XP_Type(OMobj_id parent, char *name, int comp, int type);

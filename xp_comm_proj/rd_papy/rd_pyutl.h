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


#include "avs/gd_def.h"

#include "gen.h"

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

void RP_Set_Null_Mesh(OMobj_id elem_id);
void RP_Decode_String(VALUE_T *in, int nbVal, char *out);
void RP_Decode_Date(VALUE_T *in, char *out);
void RP_Decode_Time(VALUE_T *in, char *out);

char *RP_Read_String(MODULE *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
char *RP_Read_Date(MODULE *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
char *RP_Read_Time(MODULE *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
void RP_Read_List(MODULE *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);
PapyUShort RP_Read_Ushort(MODULE *papy_mod, int papy_name, OMobj_id xp_parent, char *xp_name);

void RP_Read_Patient(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RP_Read_General_Study(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RP_Read_General_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RP_Read_General_Equipment(PapyShort fileNb, PapyShort imageNb, OMobj_id Patient_Id);
void RP_Read_Image_Series(PapyShort fileNb, PapyShort imageNb, OMobj_id Image_Series_Id,
                          char *modality, int num_frames, int *height, int *width, int *bits,
                          float *sliceSpacing, float *XSpacing, float *YSpacing, char *interp);

PapyUShort *RP_Get_Pixel_Data(PapyShort fileNb, PapyShort imageNb, int h, int w, int bits);
void RP_Free_Pixel_Data(PapyShort fileNb, PapyShort imageNb);
void RP_Set_XP_Type(OMobj_id parent, char *name, int comp, int type);


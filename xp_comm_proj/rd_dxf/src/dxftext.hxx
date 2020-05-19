/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFText class definition, which is the subclass of
DXFPrim that represents the DXF text primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_TEXT_H
#define _DXF_TEXT_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/

typedef struct
{
   DXFPoint3D point;
   DXFPoint3D extrusion;
   char text[DXF_MAX_LINE_LEN + 1];
   short align_horiz;
   short align_vert;
   short style;
   short plane;
   short path;
   short orient;
   float angle;
   float height;
   float expansion;  
   int underline;
}structtext; 

/*****************************************************************************
Class:    DXFText
Purpose:  This class is used to represent DXF text objects.
Publics:  
          
	  
*****************************************************************************/
class DXFText : public DXFPrim
{
  public:
   DXFText(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFText(const DXFText &t);
   DXFText(const structtext &t);
   DXFText(char * t, DXFPoint3D tPoint, DXFPoint3D tExtrusion, float tAngle);
   virtual ~DXFText() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   char *GetText() {return Text.text; }

   virtual int GetNumPoints()  { return 1; }

   virtual int GetPoints(float *array, int offset = 0);

   structtext GetWholeText(){return Text;}

  private:
    structtext Text; 
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













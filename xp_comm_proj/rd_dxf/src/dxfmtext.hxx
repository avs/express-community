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

#ifndef _DXF_MTEXT_H
#define _DXF_MTEXT_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFMText
Purpose:  This class is used to represent DXF multi line text objects.
Publics:  
          
	  
*****************************************************************************/
class DXFMText 
{
  public:
   DXFMText(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *textList);
   //DXFText(const DXFText &t);
   //DXFText(char * t, DXFPoint3D tPoint, DXFPoint3D tExtrusion, float tAngle);
   virtual ~DXFMText();

   void Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *textList);

   //char *GetText() {return Text.text; }

   //virtual int GetNumPoints()  { return 1; }

   //virtual int GetPoints(float *array, int offset = 0);

   //structtext GetWholeText(){return Text;}

  private:
    DXFPoint3D insertpoint;
    DXFPoint3D directionVector;
    float rectwidth;
    int addtext;	
    structtext Text; 
    char *mtext[10];
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













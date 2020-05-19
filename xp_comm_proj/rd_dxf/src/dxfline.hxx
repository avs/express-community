/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFLine class definition, which is the subclass of
DXFPrim that represents the DXF line primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_LINE_H
#define _DXF_LINE_H
#include "blistcpp.hxx"
#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFLine
Purpose:  This class is used to represent DXF line objects.
Publics:  
          
	  
*****************************************************************************/
class DXFLine : public DXFPrim
{
  public:
   DXFLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFLine(const DXFLine &l) : start(l.start), end(l.end),extrusion(l.extrusion)  {}
   DXFLine(DXFPoint3D Start,DXFPoint3D End,DXFPoint3D Extrusion,DXFLayer *layer);
   virtual ~DXFLine() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 2; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D start;
   DXFPoint3D end;
   DXFPoint3D extrusion;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













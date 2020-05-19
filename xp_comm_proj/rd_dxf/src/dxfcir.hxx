/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFCircle class definition, which is the subclass of
DXFPrim that represents the DXF Circle primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_CIRCLE_H
#define _DXF_CIRCLE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

#define DXF_CIRCLE_POINTS   30


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFCircle
Purpose:  This class is used to represent DXF circle objects.
Publics:  
          
	  
*****************************************************************************/
class DXFCircle : public DXFPrim
{
  public:
   DXFCircle(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFCircle(const DXFCircle &cir) : center(cir.center), radius(0.0)  {}
   virtual ~DXFCircle() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return DXF_CIRCLE_POINTS; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D center;
   DXFPoint3D extrusion;
   double radius;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

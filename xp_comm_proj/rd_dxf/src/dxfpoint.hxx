/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFPoint class definition, which is the subclass of
DXFPrim that represents the DXF point primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_POINT_H
#define _DXF_POINT_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFPoint
Purpose:  This class is used to represent DXF point objects.
Publics:  
          
	  
*****************************************************************************/
class DXFPoint : public DXFPrim
{
  public:
   DXFPoint(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFPoint(const DXFPoint &p);
   virtual ~DXFPoint();

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 1; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   
   DXFPoint3D point;
   DXFPoint3D extrusion;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

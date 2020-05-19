/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFFace class definition, which is the subclass of
DXFPrim that represents the DXF 3D face primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_3D_FACE_H
#define _DXF_3D_FACE_H

#include <stdio.h>
#include "blistcpp.hxx"
#include "dxfprim.hxx"
#include "avsutils.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFFace
Purpose:  This class is used to represent DXF 3D face objects.
Publics:  
          
	  
*****************************************************************************/
class DXFFace : public DXFPrim
{
  public:
   DXFFace(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFFace(const DXFFace &f);
   virtual ~DXFFace() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 4; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D corners[4];
   unsigned flags;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

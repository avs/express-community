/*****************************************************************************

******************************************************************************

This file contains the DXFBody class definition, which is the subclass of
DXFPrim that represents the DXF 3D body primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_3D_BODY_H
#define _DXF_3D_BODY_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFBody
Purpose:  This class is used to represent DXF 3D body objects.
Publics:  
          
	  
*****************************************************************************/
class DXFBody : public DXFPrim
{
  public:
   DXFBody(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFBody(const DXFBody &f);
   virtual ~DXFBody() {}

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

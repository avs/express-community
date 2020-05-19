/*****************************************************************************
TEST

******************************************************************************

This file contains the DXFRay class definition, which is the subclass of
DXFPrim that represents the DXF Ray primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_RAY_H
#define _DXF_RAY_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFRay
Purpose:  This class is used to represent DXF Ray objects.
Publics:  
          
	  
*****************************************************************************/
class DXFRay : public DXFPrim
{
  public:
   DXFRay(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFRay(const DXFRay &l) : start(l.start), dir(l.dir)  {}
   virtual ~DXFRay() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 2; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D start;
   DXFPoint3D dir;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













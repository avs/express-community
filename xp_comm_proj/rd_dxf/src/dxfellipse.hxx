/*****************************************************************************

******************************************************************************

This file contains the DXFEllipse class definition, which is the subclass of
DXFPrim that represents the DXF Ellipse primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_ELLIPSE_H
#define _DXF_ELLIPSE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

#define DXF_ELLIPSE_POINTS   30


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFEllipse
Purpose:  This class is used to represent DXF ellipse objects.
Publics:  
          
	  
*****************************************************************************/
class DXFEllipse : public DXFPrim
{
  public:
   DXFEllipse(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFEllipse(const DXFEllipse &cir) : center(cir.center), EPMajorAxis(cir.EPMajorAxis),lengthMinorAxis(cir.lengthMinorAxis),
   					 start(cir.start), finish(cir.finish){}
   virtual ~DXFEllipse() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return DXF_ELLIPSE_POINTS; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D center;
   DXFPoint3D EPMajorAxis;
   DXFPoint3D extrusion;
   float lengthMinorAxis;
   float start;
   float finish;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

/*****************************************************************************
SM
******************************************************************************

This file contains the DXFArc class definition, which is the subclass of
DXFPrim that represents the DXF Arc primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_ARC_H
#define _DXF_ARC_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

#define DXF_Arc_POINTS   30


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFArc
Purpose:  This class is used to represent DXF Arc objects.
Publics:  
          
	  
*****************************************************************************/
class DXFArc : public DXFPrim
{
  public:
   DXFArc(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFArc(const DXFArc &cir) : centre(cir.centre), radius(0.0)  {}
   DXFArc(DXFPoint3D Center,DXFPoint3D Extrusion, double Rad, double Start, double Finish, DXFLayer *layer);
   virtual ~DXFArc() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return DXF_Arc_POINTS; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D centre;
   DXFPoint3D extrusion;
   double radius;
   double start;
   double finish;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

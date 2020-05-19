/*****************************************************************************
TEST

******************************************************************************

This file contains the DXFXLine class definition, which is the subclass of
DXFPrim that represents the DXF Ray primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_XLINE_H
#define _DXF_XLINE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFXLine
Purpose:  This class is used to represent DXF XLine objects.
Publics:  
          
	  
*****************************************************************************/
class DXFXLine : public DXFPrim
{
  public:
   DXFXLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFXLine(const DXFXLine &l) : start(l.start), dir(l.dir)  {}
   virtual ~DXFXLine() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 2; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D start;
   DXFPoint3D dir;
   float k1;
   float k2;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













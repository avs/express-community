/*****************************************************************************

******************************************************************************

This file contains the DXFSolid class definition, which is the subclass of
DXFPrim that represents the DXF 3D solid primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_TRACE_H
#define _DXF_TRACE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFTrace
Purpose:  This class is used to represent DXF trace objects.
Publics:  
          
	  
*****************************************************************************/
class DXFTrace : public DXFPrim
{
  public:
   DXFTrace(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFTrace(const DXFTrace &f);
   virtual ~DXFTrace() {}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return 4; }

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D corners[4];
   DXFPoint3D extrusion;
   float thickness;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

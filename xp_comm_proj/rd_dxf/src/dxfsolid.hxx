/*****************************************************************************

******************************************************************************

This file contains the DXFSolid class definition, which is the subclass of
DXFPrim that represents the DXF 3D solid primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_3D_SOLID_H
#define _DXF_3D_SOLID_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFSolid
Purpose:  This class is used to represent DXF 3D solid objects.
Publics:  
          
	  
*****************************************************************************/
class DXFSolid : public DXFPrim
{
  public:
   DXFSolid(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFSolid(const DXFSolid &f);
   virtual ~DXFSolid() {}

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

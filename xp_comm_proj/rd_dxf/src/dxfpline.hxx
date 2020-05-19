/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFPolyline class definition, which is the subclass of
DXFPrim that represents the DXF polyline primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_POLYLINE_H
#define _DXF_POLYLINE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/

class DXFPolyline;


/*****************************************************************************
Class:    DXFVertex
Purpose:  This class is used to represent DXF vertex objects.
          Although this class is similar to others derived from DXFPrim,
	  instances of this class will appear only as subobjects of
	  DXFPolyline.
Publics:  
          
	  
*****************************************************************************/
class DXFVertex  // : public DXFPrim
{
   friend class DXFPolyline;

  protected:
   DXFVertex *next;
   DXFPoint3D point;
   double startingWidth;
   double endingWidth;
   double bulge;
   double curveFitTangentAngle;
   unsigned flags;
   double data;
   int hasData;
   int *faceVertexIndex;   // optional array of 4 ints specifying a quad

   DXFVertex(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFVertex(const DXFVertex &v);
   ~DXFVertex() {if (faceVertexIndex) delete [] faceVertexIndex; }

   static DXFVertex *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   char IsValid()  { return isValid; }
   int HasData()   { return hasData; }
   virtual void Dump();

  private:
   char isValid;
};


/*****************************************************************************
Class:    DXFPolyline
Purpose:  This class is used to represent DXF polyline objects.
Publics:  
          
	  
*****************************************************************************/
class DXFPolyline : public DXFPrim
{
  public:
   DXFPolyline(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFPolyline(const DXFPolyline &pline);
   virtual ~DXFPolyline();

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints()  { return (vertexCount<2) ? 0 : vertexCount; }
   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  protected:
   DXFVertex *verticesHead;
   DXFVertex *verticesTail;
   unsigned flags;
   double elevation;
   int mVertices;
   int nVertices;
   int vertexCount;
   int mSmooth;
   int nSmooth;
   int curves;
   double data;
   int hasData;
   DXFPoint3D extrusion;

   char DefineCellSets(char reverseNormals = 0);
   char DefineMeshCellSet(char reverseNormals = 0);
   char DefinePolyfaceMeshCellSet(char reverseNormals = 0);
   char DefinePolylineCellSet(char reverseNormals = 0);
   void SetCellData(double data);
   int HasData() { return hasData; }
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

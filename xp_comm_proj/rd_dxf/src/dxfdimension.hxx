/*****************************************************************************
SM
******************************************************************************

This file contains the DXFDimension class definition, which is the subclass of
DXFPrim that represents the DXF Arc primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_DIMENSION_H
#define _DXF_DIMENSION_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"
#include "dxfline.hxx"
#include "dxftext.hxx"
#include "dxfarc.hxx"

/*********************  Macros and Manifest Constants  **********************/


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFDimension
Purpose:  This class is used to represent DXF Dimension objects.
Publics:  
          
	  
*****************************************************************************/
class DXFDimension
{
  public:
   DXFDimension(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *entityList,
		CLIST <DXFPrim> *textList);
   //DXFDimension(const DXFDimension &dim);
   virtual ~DXFDimension() {}

   void Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *entityList,
		         CLIST <DXFPrim> *textList);

   virtual void Dump();

  private:

   void linear(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void aligned(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void diameter(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void radius(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void angular(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void angular3(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);
   void ordinate(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList);

   DXFPoint3D defPoint;
   DXFPoint3D MiddlePoint;
   DXFPoint3D extrusion;
   char text[DXF_MAX_LINE_LEN + 1];
   int dimType;
   float angle;
   float horizontalDirection;

   DXFPoint3D insertPoint;
   DXFPoint3D valueOfDefPoint1;
   DXFPoint3D valueOfDefPoint2;
   DXFPoint3D DefPointRadDia;
   DXFPoint3D locarc;

   DXFLine *line;
   DXFLine *line1;
   DXFLine *line2;
   DXFLine *line3;
   DXFLine *line4;
   DXFLine *line5;
   DXFLine *line6;
   DXFText *nText;
   DXFArc  *nArc;
   DXFLayer *curLayer;
   
};


/************************  Public Global Variables  *************************/



/********************  Prototypes for Public Functions  ********************/



#endif

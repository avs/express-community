/*****************************************************************************

******************************************************************************

This file contains the DXFLeader class definition, which is the subclass of
DXFPrim that represents the DXF leader primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_LEADER_H
#define _DXF_LEADER_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFLeader
Purpose:  This class is used to represent DXF leader objects.
Publics:  
          
	  
*****************************************************************************/
class DXFLeader : public DXFPrim
{
  public:
   DXFLeader(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFLeader(const DXFLeader &l);
   virtual ~DXFLeader() {delete [] Vertex;}

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstlayer);

   virtual int GetNumPoints();

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   short Arrowhead;
   short LeaderPathType;
   short LeaderCreationFlag;
   short HooklineDirectionFlag;
   short HooklineFlag;
   float TextHeight;
   float TextWidth;
   int   NrOfVertex;
   DXFPoint3D *Vertex;
   DXFPoint3D NormalVector;
   DXFPoint3D HorizontalDirectionLeader;
   DXFPoint3D DirectionAnnotationOffset;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













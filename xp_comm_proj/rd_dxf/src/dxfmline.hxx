/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFMLine class definition, which is the subclass of
DXFPrim that represents the DXF mline primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_MLINE_H
#define _DXF_MLINE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/



/************************  Public Type Definitions  *************************/


/*****************************************************************************
Class:    DXFMLine
Purpose:  This class is used to represent DXF mline objects.
Publics:  
          
	  
*****************************************************************************/
class DXFMLine : public DXFPrim
{
  public:
   DXFMLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFMLine(const DXFMLine &ml);
   virtual ~DXFMLine() {delete [] Vertex;
			delete [] SegmentVector;
			delete [] MiterVector;
			delete [] NrOfAreaFillParameters;
			delete [] ElementParameters;
			delete [] NrOfParameters;
			delete [] AreaFillParameters;
		       }

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   virtual int GetNumPoints();

   virtual int GetPoints(float *array, int offset = 0);

   virtual void Dump();

  private:
   DXFPoint3D start;
   DXFPoint3D extrusion;
   float scalefactor;
   short justification;
   short open_close_flag;
   int nrOfElements;
   DXFPoint3D *Vertex;
   DXFPoint3D *SegmentVector;
   DXFPoint3D *MiterVector;
   int *NrOfParameters;
   float **ElementParameters;
   int *NrOfAreaFillParameters;
   float **AreaFillParameters;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif

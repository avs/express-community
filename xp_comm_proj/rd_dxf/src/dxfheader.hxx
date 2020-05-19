/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************/

#ifndef _DXF_HEADER_H
#define _DXF_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include "dxfprim.hxx"


/*****************************************************************************
Class:    DXFHeader
Purpose:  This class is used to represent variables defined in the HEADER 
	  section of the dxf-file. It reads only the intersted variables
*****************************************************************************/
class DXFHeader
{
  public:
   DXFHeader(DXFGroup &group);
   ~DXFHeader() {}
   static DXFHeader *Make(DXFGroup &group);
   DXFPoint3D GetExtMax() {return extmax;}
   DXFPoint3D GetExtMin() {return extmin;}
   DXFPoint3D GetRExtMax() {return rextmax;}
   DXFPoint3D GetRExtMin() {return rextmin;}

  protected:
   DXFPoint3D extmax;		//x,y,z values drawing extents upper-right corner
   DXFPoint3D extmin;		//x,y,z values drawing extents lower-left corner
   DXFPoint3D rextmax;		//x,y,z values drawing extents real upper-right corner
   DXFPoint3D rextmin;		//x,y,z values drawing extents real lower-left corner
};

#endif

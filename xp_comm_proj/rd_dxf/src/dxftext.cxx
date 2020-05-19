
/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFText class, which represents DXF text objects.

******************************************************************************
*****************************************************************************/
#include "blistcpp.hxx"
//#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxftext.hxx"
#include "dxfprim.hxx"
#include "dxfdefine.hxx"
#include "../gen.hxx"
#include <string.h>

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFLine" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFText::Make()
Purpose:  This is the static "virtual constructor" for the DXFLine class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a text, then
	  an instance of DXFText is created, and its attributes are read
	  from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFText object (cast to DXFPrim *) if the current
          object was recognized as a DXF text, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFText::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("TEXT")) 
      instance = new DXFText(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFText::DXFText()
Purpose:  A Constructor for the DXFText class.  Reads the center point
          and radius from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFText::DXFText(DXFGroup &group, CLIST <DXFLayer> *firstLayer) 
{
   short temp=0;

   Text.point.SetX(0);
   Text.point.SetY(0);
   Text.point.SetZ(0);
   isValid = 0;
   Text.orient=4;
   Text.extrusion.SetX(0);
   Text.extrusion.SetY(0);
   Text.extrusion.SetZ(1);
   Text.align_horiz=0;
   Text.align_vert=0;
   Text.path=0;
   Text.angle=0;
   Text.height=0;
   Text.expansion=1;
   Text.plane=0;
   Text.style=0;
   Text.underline=0;

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // text x coordinate
	 Text.point.SetX(group.DataAsFloat());
	 break;

	case 20:   // text y coordinate
	 Text.point.SetY(group.DataAsFloat());
	 break;

	case 30:   // text z coordinate
	 Text.point.SetZ(group.DataAsFloat());
	 break;

	case 1:    // text string
	 strcpy(Text.text, group.Data());
	 break;

	case 72:    // Horizontal alignment
	 Text.align_horiz=(short) group.DataAsInt();
         if(Text.align_horiz>2)Text.align_horiz=0;	//Aligned, Middle, Fit change to left
	 break;

	case 73:    // Vertical alignment
	 Text.align_vert=(short) group.DataAsInt();
	 break;

	case 71:    // Text generation flags
	 temp=(short) group.DataAsInt();
	 if(temp==2) Text.path=2;
         if(temp==4) Text.orient=2;
	 break;

	case 50:    // Rotation angle
	 Text.angle=group.DataAsFloat();
	 break;

	case 40:    // Textheight
	 Text.height=group.DataAsFloat();
	 break;

	case 41:    // scalfactor x
	 Text.expansion=group.DataAsFloat();
	 break;

        case 210:
	 Text.extrusion.SetX(group.DataAsFloat());
	 break;

        case 220:
	 Text.extrusion.SetY(group.DataAsFloat());
	 break;

	case 230:
	 Text.extrusion.SetZ(group.DataAsFloat());
	 break;


	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // There is no cell set for a text - they are exported separately
 
   isValid = 1;
   
}


DXFText::DXFText(const DXFText &t) 
{
	isValid=t.isValid;
	Text=t.Text;
}

DXFText::DXFText(const structtext &t)
{
	isValid=1;
	Text=t;
}


DXFText::DXFText(char * t, DXFPoint3D tPoint, DXFPoint3D tExtrusion, float tAngle)
{
strcpy(Text.text,t);
printf("DXFText::DXFText Angle %f \n",tAngle);
Text.angle=tAngle;
Text.point.SetX(tPoint.GetX());
Text.point.SetY(tPoint.GetY());
Text.point.SetZ(tPoint.GetZ());
Text.extrusion.SetX(tExtrusion.GetX());
Text.extrusion.SetY(tExtrusion.GetY());
Text.extrusion.SetZ(tExtrusion.GetZ());
isValid = 1;
Text.orient=4;
Text.align_horiz=1;
Text.align_vert=2;
Text.path=0;
Text.height=8;
Text.expansion=1;
Text.plane=0;
Text.style=0;
Text.underline=0;
}



/*****************************************************************************
Method:   DXFText::GetPoints()
Purpose:  This function generates the vertices associated with this text
          
Params:   array   A float array in which to place the coordinates.  It is
                  assumed that this array is at least < GetNumPoints() >
		  floats in length.
	  offset  An optional offset into the array, at which to start
                  placing coordinates.  Thus, if this parameter is supplied
		  the array length must be < GetNumPoints() + offset >
		  floats long.
State:    ---
Returns:  The number of *NUMBERS* (i.e. 3 times the number of vertices)
          placed in the array PLUS the starting offset, if any.
*****************************************************************************/
int DXFText::GetPoints(float *array, int offset)
{
   array[offset++] = Text.point.GetX();
   array[offset++] = Text.point.GetY();
   array[offset++] = Text.point.GetZ();

   return offset;
}


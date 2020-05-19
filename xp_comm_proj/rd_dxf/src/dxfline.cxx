/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFLine class, which represents DXF line objects.

******************************************************************************
*****************************************************************************/
#include <math.h>
#include "dxfline.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"

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
Method:   DXFLine::Make()
Purpose:  This is the static "virtual constructor" for the DXFLine class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a line, then
	  an instance of DXFLine is created, and its attributes are read
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
Returns:  A pointer to a DXFLine object (cast to DXFPrim *) if the current
          object was recognized as a DXF line, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFLine::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("LINE"))
      instance = new DXFLine(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFLine::DXFLine()
Purpose:  A Constructor for the DXFLine class.  Reads the center point
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
DXFLine::DXFLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : start(), end()
{
   isValid = 0;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // line start point x coordinate
	 start.SetX(group.DataAsFloat());
	 break;

	case 20:   // line start point y coordinate
	 start.SetY(group.DataAsFloat());
	 break;

	case 30:   // line start point z coordinate
	 start.SetZ(group.DataAsFloat());
	 break;

	case 11:   // line end point x coordinate
	 end.SetX(group.DataAsFloat());
	 break;

	case 21:   // line end point y coordinate
	 end.SetY(group.DataAsFloat());
	 break;

	case 31:   // line end point z coordinate
	 end.SetZ(group.DataAsFloat());
	 break;

        case 210:
	 extrusion.SetX(group.DataAsFloat());
	 break;

        case 220:
	 extrusion.SetY(group.DataAsFloat());
	 break;

	case 230:
	 extrusion.SetZ(group.DataAsFloat());
	 break;

	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the line data (its end points), we need to
   // generate the connectivity information.
   //
   if (AddCellSet(2, 1, DXF_CELL_LINE))
   {
      cellSetsTail->AddVertexIndex(0);
      cellSetsTail->AddVertexIndex(1);

      isValid = 1;
   }
}

/*****************************************************************************
Method:   DXFLine::DXFLine()
Purpose:  A Constructor for the DXFLine class.  Reads the center point
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
DXFLine::DXFLine(DXFPoint3D Start,DXFPoint3D End,DXFPoint3D Extrusion,DXFLayer *layer) : start(Start),end(End),extrusion(Extrusion)
{
   if (AddCellSet(2, 1, DXF_CELL_LINE))
   {
      cellSetsTail->AddVertexIndex(0);
      cellSetsTail->AddVertexIndex(1);
      curLayer=layer;
      isValid = 1;
   }


}

/*****************************************************************************
Method:   DXFLine::GetPoints()
Purpose:  This function generates the vertices associated with this line
          (i.e. its end points).
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
int DXFLine::GetPoints(float *array, int offset)
{
   DXFPoint3D Ax,Ay;
   Ax.Set(1,0,0),Ay.Set(0,1,0);
   float X,Y,Z;
   printf("DXFLine::GetPoints\n");
   float c=(extrusion.GetX()<0) ? (extrusion.GetX()*(-1)) : extrusion.GetX();
   float d=(extrusion.GetY()<0) ? (extrusion.GetY()*(-1)) : extrusion.GetY();
   float e=1.0/64.0;
   printf("c %f d %f e: %f\n",c,d,e);
   if((c<e) && (d<e))
	{
	Ax.SetX(extrusion.GetZ());
	Ax.SetY(0);
	Ax.SetZ(-1*extrusion.GetX());
	}
   else
	{
	/*Ax.SetX(-1*extrusion.GetY());
	Ax.SetY(extrusion.GetX());
	Ax.SetZ(0);
 	*/
	}

   float k=sqrt(1/(Ax.GetX()*Ax.GetX()+Ax.GetY()*Ax.GetY()+Ax.GetZ()*Ax.GetZ()));
   Ax.SetX(Ax.GetX()*k);
   Ax.SetY(Ax.GetY()*k);
   Ax.SetZ(Ax.GetZ()*k);
   extrusion.Set(0,0,1);
   /*Ay.SetX(extrusion.GetY()*Ax.GetZ()-extrusion.GetZ()*Ax.GetY());
   Ay.SetY(extrusion.GetZ()*Ax.GetX()-extrusion.GetX()*Ax.GetZ());
   Ay.SetZ(extrusion.GetX()*Ax.GetY()-extrusion.GetY()*Ax.GetX());

   */

   k=sqrt(1/(Ay.GetX()*Ay.GetX()+Ay.GetY()*Ay.GetY()+Ay.GetZ()*Ay.GetZ()));
   Ay.SetX(Ay.GetX()*k);
   Ay.SetY(Ay.GetY()*k);
   Ay.SetZ(Ay.GetZ()*k);

   X = start.GetX();
   Y = start.GetY();
   Z = start.GetZ();

   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();

   X = end.GetX();
   Y = end.GetY();
   Z = end.GetZ();

   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();

Dump();
   return offset;
}




void DXFLine::Dump()
{
//  printf("   Line\n");
//  printf("      Start = (%f, %f, %f)\n",
//	  start.GetX(), start.GetY(), start.GetZ());
//   printf("        End = (%f, %f, %f)\n", end.GetX(), end.GetY(), end.GetZ());
}







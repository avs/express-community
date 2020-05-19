/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFPoint class, which represents DXF point objects.

******************************************************************************
*****************************************************************************/

#include <math.h>
#include "dxfpoint.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
*********--------   Implementations of "DXFPoint" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFPoint::Make()
Purpose:  This is the static "virtual constructor" for the DXFPoint class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a point, then
	  an instance of DXFPoint is created, and its attributes are read
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
Returns:  A pointer to a DXFPoint object (cast to DXFPrim *) if the current
          object was recognized as a DXF point, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFPoint::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("POINT"))
      instance = new DXFPoint(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFPoint::DXFPoint()
Purpose:  A Constructor for the DXFPoint class.  Reads the center point
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
DXFPoint::DXFPoint(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   isValid=0;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);

   while (group.Read())
   {
      if (group.Code() ==0 )
	 break;
      switch (group.Code())
      {
	case 10:   // point start point x coordinate
	 point.SetX(group.DataAsFloat());
	 break;

	case 20:   // point start point y coordinate
	 point.SetY(group.DataAsFloat());
	 break;

	case 30:   // point start point z coordinate
	 point.SetZ(group.DataAsFloat());
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

	default:   // ignore (and read past) all other group codes
	   //ReadCommon(group, firstLayer);
		break;
     }
  }
  if (AddCellSet(1,1,DXF_CELL_POINT))
  {
      cellSetsTail->AddVertexIndex(0);
      isValid=1;
  }
  else
     errorDesc="Unable to read POINT definition from DXF file";
   
}


/*****************************************************************************
Method:   DXFPoint::DXFPoint()
Purpose:  Copy constructor for the DXFPoint class.
Params:   p   The DXFPoint object to copy.
State:    The linked list of points is cloned.
*****************************************************************************/
DXFPoint::DXFPoint(const DXFPoint &p) : point(p.point)
{
   isValid = p.isValid;
}


/*****************************************************************************
Method:   DXFPoint::~DXFPoint()
Purpose:  Destructor for the DXFPoint class.
Params:   ---
State:    Destroys the point list.
*****************************************************************************/
DXFPoint::~DXFPoint()
{
}


/*****************************************************************************
Method:   DXFPoint::GetPoints()
Purpose:  This function generates the vertices associated with this point
          (i.e. itself).
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
int DXFPoint::GetPoints(float *array, int offset)
{
   DXFPoint3D Ax,Ay;
   float X,Y,Z;

   float c=(extrusion.GetX()<0) ? (extrusion.GetX()*(-1)) : extrusion.GetX();
   float d=(extrusion.GetY()<0) ? (extrusion.GetY()*(-1)) : extrusion.GetY();
   float e=1.0/64.0;
   if((c<e) && (d<e))
	{
	Ax.SetX(extrusion.GetZ());
	Ax.SetY(0);
	Ax.SetZ(-1*extrusion.GetX());
	}
   else
	{
	Ax.SetX(-1*extrusion.GetY());
	Ax.SetY(extrusion.GetX());
	Ax.SetZ(0);
	}

   float k=sqrt(1/(Ax.GetX()*Ax.GetX()+Ax.GetY()*Ax.GetY()+Ax.GetZ()*Ax.GetZ()));
   Ax.SetX(Ax.GetX()*k);
   Ax.SetY(Ax.GetY()*k);
   Ax.SetZ(Ax.GetZ()*k);

   Ay.SetX(extrusion.GetY()*Ax.GetZ()-extrusion.GetZ()*Ax.GetY());
   Ay.SetY(extrusion.GetZ()*Ax.GetX()-extrusion.GetX()*Ax.GetZ());
   Ay.SetZ(extrusion.GetX()*Ax.GetY()-extrusion.GetY()*Ax.GetX());

   k=sqrt(1/(Ay.GetX()*Ay.GetX()+Ay.GetY()*Ay.GetY()+Ay.GetZ()*Ay.GetZ()));
   Ay.SetX(Ay.GetX()*k);
   Ay.SetY(Ay.GetY()*k);
   Ay.SetZ(Ay.GetZ()*k);

   X = point.GetX();
   Y = point.GetY();
   Z = point.GetZ();
   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
Dump();
   return offset;
}




void DXFPoint::Dump()
{
//   int num = 0;
//   DXFPoint3D *el = Pointlist.GetFirst();

//   printf("   Points\n");

//   while (el)
//   {
//      printf("      Point %3d = (%f, %f, %f)\n", num++, el->point.GetX(),
//	     el->point.GetY(), el->point.GetZ());

//      el = Pointlist.GetNext();
//   }
}

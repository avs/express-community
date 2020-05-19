/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFCircle class, which represents DXF circle objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfcir.hxx"
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
*********--------   Implementations of "DXFCircle" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFCircle::Make()
Purpose:  This is the static "virtual constructor" for the DXFCircle class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a circle,
	  then an instance of DXFCircle is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFCircle object (cast to DXFPrim *) if the current
          object was recognized as a DXF circle, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFCircle::Make(DXFGroup &group,CLIST <DXFLayer>  *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("CIRCLE"))
      instance = new DXFCircle(group, firstLayer);
   return instance;
}


/*****************************************************************************
Method:   DXFCircle::DXFCircle()
Purpose:  A Constructor for the DXFCircle class.  Reads the center point
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
DXFCircle::DXFCircle(DXFGroup &group, CLIST <DXFLayer> *firstLayer) :
                     center(), radius(0.0)
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
	case 10:   // circle center x coordinate
	 center.SetX(group.DataAsFloat());
	 break;

	case 20:   // circle center y coordinate
	 center.SetY(group.DataAsFloat());
	 break;

	case 30:   // circle center z coordinate
	 center.SetZ(group.DataAsDouble());
	 break;

	case 40:   // circle radius
	 radius = group.DataAsFloat();
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

	default:                                  // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the circle data (radius, center), we need to
   // generate the connectivity information.
   //
   int m = GetNumPoints();

   if (AddCellSet(m, 1, DXF_CELL_CHOOSE, DXF_CLOSED_POLYGON + DXF_CONVEX_POLYGON))
   {
      if (group.ReverseNormals())
	{
	 for (int q = m - 1; q >= 0; q--)
	    cellSetsTail->AddVertexIndex(q);
	}
      else
	{
	 for (int q = 0; q < m; q++)
	    cellSetsTail->AddVertexIndex(q);
	}
	
      isValid = 1;
   }
}


/*****************************************************************************
Method:   DXFCircle::GetPoints()
Purpose:  This function generates the vertices associated with this circle.
          The center and radius are given.  The circle is assumed to lie in
	  a plane parallel to the x-z plane.
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
int DXFCircle::GetPoints(float *array, int offset)
{
   int i;
   DXFPoint3D Ax,Ay;
   int n = GetNumPoints();
   float inc = 6.28318530718 / (float)(n-1);     //  2*PI / n
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

   printf("DXFCircle::GetPoints Az: %f %f %f Ax: %f %f %f Ay: %f %f %f \n",extrusion.GetX(),extrusion.GetY(),extrusion.GetZ(),
	Ax.GetX(),Ax.GetY(),Ax.GetZ(),Ay.GetX(),Ay.GetY(),Ay.GetZ());
   
   for (i = 0, d = 0.0; i < n-1; i++, d += inc)
   {
      X = (radius * cos(d)) + center.GetX();
      Y = (radius * sin(d)) + center.GetY();
      Z = center.GetZ();
      array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
      array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
      array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
				
   }
   X = radius + center.GetX();
   Y = center.GetY();
   Z = center.GetZ();
   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();

Dump();
   return offset;
}




void DXFCircle::Dump()
{
   float x, y, z;

   center.Get(&x, &y, &z);

//   printf("   Circle\n");
//   printf("      Center = (%f, %f, %f)\n", x, y, z);
//   printf("      Radius = %f\n", radius);
}

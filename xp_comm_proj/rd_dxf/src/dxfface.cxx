/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFFace class, which represents DXF 3D face objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfface.hxx"
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
**********--------   Implementations of "DXFFace" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFFace::Make()
Purpose:  This is the static "virtual constructor" for the DXFFace class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a 3D face,
	  then an instance of DXFFace is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
		  firstLayer   start of the linked list fo LAYER information. This 
		  will be used to assign color if appropriate
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFFace object (cast to DXFPrim *) if the current
          object was recognized as a DXF 3D face, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFFace::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("3DFACE"))
      instance = new DXFFace(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFFace::DXFFace()
Purpose:  A Constructor for the DXFFace class.  Reads the corner points
          and flags from the DXF file.
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
DXFFace::DXFFace(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : flags(0)
{
   int code;

   isValid = 0;

   while (group.Read())
   {
      code = group.Code();

      if (code == 0)   // stop at next entity definition
	 break;

      switch (code)
      {
	case 10:   // Corner X coordinate
	case 11:
	case 12:
	case 13:
	 corners[code - 10].SetX(group.DataAsFloat());
	 break;

	case 20:   // Corner Y coordinate
	case 21:
	case 22:
	case 23:
	 corners[code - 20].SetY(group.DataAsFloat());
	 break;

	case 30:   // Corner Z coordinate
	case 31:
	case 32:
	case 33:
	 corners[code - 30].SetZ(group.DataAsFloat());
	 break;

	case 70:   // flags
	 flags = group.DataAsUnsigned();
	 break;

	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the four corner points, we need to generate the
   // connectivity information.
   //
   if (AddCellSet(5, 1, DXF_CELL_CHOOSE, DXF_CLOSED_POLYGON))
   {
      if (group.ReverseNormals())
	{
	 for (int q = 3; q >= 0; q--)
	    cellSetsTail->AddVertexIndex(q);
         cellSetsTail->AddVertexIndex(3);
	}
      else
	{
	 for (int q = 0; q < 4; q++)
	    cellSetsTail->AddVertexIndex(q);
         cellSetsTail->AddVertexIndex(0);
	}

      isValid = 1;
   }
}


/*****************************************************************************
Method:   DXFFace::DXFFace()
Purpose:  Copy constructor for the DXFFace class.
Params:   f   The DXFFace object to copy.
State:    ---
*****************************************************************************/
DXFFace::DXFFace(const DXFFace &f) : flags(0)
{
   for (int i = 0; i < 4; i++)
      corners[i] = f.corners[i];
}


/*****************************************************************************
Method:   DXFFace::GetPoints()
Purpose:  This function gets the vertices associated with this 3D face.
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
int DXFFace::GetPoints(float *array, int offset)
{
   for (int i = 0; i < 4; i++)
   {
      array[offset++] = corners[i].GetX();
      array[offset++] = corners[i].GetY();
      array[offset++] = corners[i].GetZ();
   }
Dump();
   return offset;
}




void DXFFace::Dump()
{
//   printf("   3D face\n");
//   printf("      flags = %04X\n", flags);

   for (int i = 0; i < 4; i++);
//      printf("      corner[%d] = (%f, %f, %f)\n", i, corners[i].GetX(),
//	     corners[i].GetY(), corners[i].GetZ());
}

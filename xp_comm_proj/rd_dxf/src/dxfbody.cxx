/*****************************************************************************
minimal
******************************************************************************

Implementation of the DXFBody class, which represents DXF 3D body objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfbody.hxx"
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
**********--------   Implementations of "DXFBody" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFBody::Make()
Purpose:  This is the static "virtual constructor" for the DXFBody class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a 3D body,
	  then an instance of DXFBody is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFBody object (cast to DXFPrim *) if the current
          object was recognized as a DXF 3D body, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFBody::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("BODY"))
      instance = new DXFBody(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFBody::DXFBody()
Purpose:  A Constructor for the DXFBody class.  Reads the corner points
          and flags from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFBody::DXFBody(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : flags(0)
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

	default:                                  // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the four corner points, we need to generate the
   // connectivity information.
   //
   if (AddCellSet(4, 1, DXF_CELL_FACE))
   {
      if (group.ReverseNormals())
	{
	 for (int q = 3; q >= 0; q--)
	    cellSetsTail->AddVertexIndex(q);
	}
      else
	{
	 for (int q = 0; q < 4; q++)
	    cellSetsTail->AddVertexIndex(q);
	}

      isValid = 1;
   }
}


/*****************************************************************************
Method:   DXFBody::DXFBody()
Purpose:  Copy constructor for the DXFBody class.
Params:   f   The DXFBody object to copy.
State:    ---
*****************************************************************************/
DXFBody::DXFBody(const DXFBody &f) : flags(0)
{
   for (int i = 0; i < 4; i++)
      corners[i] = f.corners[i];
}


/*****************************************************************************
Method:   DXFBody::GetPoints()
Purpose:  This function gets the vertices associated with this 3D body.
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
int DXFBody::GetPoints(float *array, int offset)
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




void DXFBody::Dump()
{
//   printf("   3D body\n");
//   printf("      flags = %04X\n", flags);

//   for (int i = 0; i < 4; i++)
//      printf("      corner[%d] = (%f, %f, %f)\n", i, corners[i].GetX(),
//	     corners[i].GetY(), corners[i].GetZ());
}

/*****************************************************************************
Partial at best
******************************************************************************

Implementation of the DXFTrace class, which represents DXF trace objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxftrace.hxx"
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
**********--------   Implementations of "DXFTrace" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFTrace::Make()
Purpose:  This is the static "virtual constructor" for the DXFSolid class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a trace,
	  then an instance of DXFTrace is created, and its attributes
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
Returns:  A pointer to a DXFTrace object (cast to DXFPrim *) if the current
          object was recognized as a DXF trace, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFTrace::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("TRACE"))
      instance = new DXFTrace(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFTrace::DXFTrace()
Purpose:  A Constructor for the DXFTrace class.  Reads the corner points
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
DXFTrace::DXFTrace(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   int code;
   extrusion.Set(0.0,0.0,1.0);
   thickness=0;

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

	case 39:   // flags
	 thickness = group.DataAsFloat();
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
Method:   DXFTrace::DXFTrace()
Purpose:  Copy constructor for the DXFTrace class.
Params:   f   The DXFTrace object to copy.
State:    ---
*****************************************************************************/
DXFTrace::DXFTrace(const DXFTrace &f)
{
   extrusion.Set(f.extrusion);
   thickness=f.thickness;
   for (int i = 0; i < 4; i++)
      corners[i] = f.corners[i];
}


/*****************************************************************************
Method:   DXFTrace::GetPoints()
Purpose:  This function gets the vertices associated with this trace.
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
int DXFTrace::GetPoints(float *array, int offset)
{
   DXFPoint3D Ax,Ay;
   float X,Y,Z;
   int p=0;
   for (int i = 0; i < 4; i++)
   {
      p = (i==2) ? 1 : (i==3) ? -1 : 0;
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


      X = corners[i+p].GetX();
      Y = corners[i+p].GetY();
      Z = corners[i+p].GetZ();
 
      array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
      array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
      array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
   }
Dump();
   return offset;
}




void DXFTrace::Dump()
{
//   printf("   3D Solid\n");
//   printf("      flags = %04X\n", flags);

//   for (int i = 0; i < 4; i++)
//      printf("      corner[%d] = (%f, %f, %f)\n", i, corners[i].GetX(),
//	     corners[i].GetY(), corners[i].GetZ());
}

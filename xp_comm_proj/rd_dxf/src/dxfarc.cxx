/*****************************************************************************
SM
******************************************************************************

Implementation of the DXFArc class, which represents DXF Arc objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfarc.hxx"
#include "dxfprim.hxx"
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
*********--------   Implementations of "DXFArc" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFArc::Make()
Purpose:  This is the static "virtual constructor" for the DXFArc class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a Arc,
	  then an instance of DXFArc is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFArc object (cast to DXFPrim *) if the current
          object was recognized as a DXF Arc, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFArc::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("ARC"))
      instance = new DXFArc(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFArc::DXFArc()
Purpose:  A Constructor for the DXFArc class.  Reads the centre point,radius 
          and the start and finish angles from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFArc::DXFArc(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : centre(), radius(0.0)
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
	case 10:   // Arc centre x coordinate
	 centre.SetX(group.DataAsFloat());
	 break;

	case 20:   // Arc centre y coordinate
	 centre.SetY(group.DataAsFloat());
	 break;

	case 30:   // Arc centre z coordinate
	 centre.SetZ(group.DataAsDouble());
	 break;

	case 40:   // Arc radius
	 radius = group.DataAsFloat();
	 break;

        case 50:   // Arc start angle
	 start = group.DataAsFloat()*3.141592654/180;
	 break;

        case 51:   // Arc finish angle
	 finish = group.DataAsFloat()*3.141592654/180;
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

   // Now that we have the Arc data (radius, centre, start and finish angles),
   // we need to generate the connectivity information.
   //
   int m = GetNumPoints();

   if (AddCellSet(m, 1, DXF_CELL_LINE))
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

DXFArc::DXFArc(DXFPoint3D Center,DXFPoint3D Extrusion, double Rad, double Start, double Finish,DXFLayer *layer)
{
curLayer=layer;
centre.SetX(Center.GetX());
centre.SetY(Center.GetY());
centre.SetZ(Center.GetZ());
extrusion.SetX(Extrusion.GetX());
extrusion.SetY(Extrusion.GetY());
extrusion.SetZ(Extrusion.GetZ());
radius=Rad;
start=Start;
finish=Finish;
int m = GetNumPoints();

if (AddCellSet(m, 1, DXF_CELL_LINE))
    {
    for (int q = 0; q < m; q++)
        cellSetsTail->AddVertexIndex(q);
    }
	
isValid = 1;
}

/*****************************************************************************
Method:   DXFArc::GetPoints()
Purpose:  This function generates the vertices associated with this Arc.
          The centre and radius are given.  The Arc is assumed to lie in
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
int DXFArc::GetPoints(float *array, int offset)
{
   int i;
   float inc;
   DXFPoint3D Ax,Ay;
   int n = GetNumPoints();
   float X,Y,Z;
   //float inc = 6.28318530718 / (float)n;     //  2*PI / n
   //float inc = (6.283185307-start+finish)*(-1) / (float)(n-1);
   if(finish<start) inc=(6.283185307-start+finish)/ (float)(n-1);
   else inc=(finish-start)/(float)(n-1);

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

   printf("DXFArc::GetPoints Az: %f %f %f Ax: %f %f %f Ay: %f %f %f \n",extrusion.GetX(),extrusion.GetY(),extrusion.GetZ(),
	Ax.GetX(),Ax.GetY(),Ax.GetZ(),Ay.GetX(),Ay.GetY(),Ay.GetZ());
   

   printf("DXFArc::GetPoints start: %f finish: %f inc: %f other:%f\n",start,finish,inc,(finish-start)/(n-1));

   for (i = 1, d = start; i < n; i++, d += inc)
   {
      X = (radius * cos(d)) + centre.GetX();
      Y = (radius * sin(d)) + centre.GetY();
      Z = centre.GetZ();
      array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
      array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
      array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
   }
   X = (radius * cos(finish)) + centre.GetX();	//last Point exactly finish
   Y = (radius * sin(finish)) + centre.GetY();
   Z = centre.GetZ();
   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();


Dump();
   return offset;
}

   void DXFArc::Dump()
{
   float x, y, z;

   centre.Get(&x, &y, &z);

//   printf("   Arc\n");
//   printf("      Centre = (%f, %f, %f)\n", x, y, z);
//   printf("      Radius = %f\n", radius);
//   printf("      Start angle = %f/n", start);
//   printf("      Finish angle = %f/n", finish);
}


/*****************************************************************************
Minimal
******************************************************************************

Implementation of the DXFEllipse class, which represents DXF ellipse objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfellipse.hxx"
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
*********--------   Implementations of "DXFEllipse" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFEllipse::Make()
Purpose:  This is the static "virtual constructor" for the DXFEllipse class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a ellipse,
	  then an instance of DXFEllipse is created, and its attributes
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
Returns:  A pointer to a DXFEllipse object (cast to DXFPrim *) if the current
          object was recognized as a DXF ellipse, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFEllipse::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("ELLIPSE"))
      instance = new DXFEllipse(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFEllipse::DXFEllipse()
Purpose:  A Constructor for the DXFEllipse class.  Reads the center point
          and radius from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFEllipse::DXFEllipse(DXFGroup &group, CLIST <DXFLayer> *firstLayer) :
                       center(), EPMajorAxis (), lengthMinorAxis(0),
                       start(0),finish(6.283185307)
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
	case 10:   // ellipse center x coordinate
	 center.SetX(group.DataAsFloat());
	 break;

	case 20:   // ellipse center y coordinate
	 center.SetY(group.DataAsFloat());
	 break;

	case 30:   // ellipse center z coordinate
	 center.SetZ(group.DataAsDouble());
	 break;

	case 11:   // ellipse endpoint major axis
	 EPMajorAxis.SetX(group.DataAsFloat());
	 break;

	case 21:   // ellipse endpoint major axis
	 EPMajorAxis.SetY(group.DataAsFloat());
	 break;
	 
	case 31:   // ellipse endpoint major axis
	 EPMajorAxis.SetZ(group.DataAsFloat());
	 break;
	 
	case 40:   // ellipse length of minor axis
	 lengthMinorAxis=group.DataAsFloat();
	 break;
	 
	case 41:   // ellipse startparameter
	 start=group.DataAsFloat();//*3.141592654/180;
	 break;
	
	case 42:   // ellipse endparameter
	 finish=group.DataAsFloat();//*3.141592654/180;
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

   // Now that we have the ellipse data (radius, center), we need to
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
Method:   DXFEllipse::GetPoints()
Purpose:  This function generates the vertices associated with this ellipse.
          The center and radius are given.  The ellipse is assumed to lie in
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
int DXFEllipse::GetPoints(float *array, int offset)
{
   int i;
   DXFPoint3D Ax,Ay;
   float X,Y,Z;
   float pi=3.141592654;
   int n = GetNumPoints();
   float inc = (finish-start) / (float)(n-1);
   printf("DXFEllipse::GetPoints start: %f end: %f\n",start,finish);
   float x,y;
   
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


   printf("DXFEllipse::GetPoints Az: %f %f %f Ax: %f %f %f Ay: %f %f %f \n",extrusion.GetX(),extrusion.GetY(),extrusion.GetZ(),
	Ax.GetX(),Ax.GetY(),Ax.GetZ(),Ay.GetX(),Ay.GetY(),Ay.GetZ());

   //float betta=cos((EPMajorAxis.GetY()-center.GetY())/sqrt((EPMajorAxis.GetX()-center.GetX())*(EPMajorAxis.GetX()-center.GetX())+(EPMajorAxis.GetY()-center.GetY())*(EPMajorAxis.GetY()-center.GetY())));
   float betta2=1/tan((EPMajorAxis.GetX()-center.GetX())/(EPMajorAxis.GetX()-center.GetX()));
   

   float lengthMA=sqrt(EPMajorAxis.GetX()*EPMajorAxis.GetX()+EPMajorAxis.GetY()*EPMajorAxis.GetY());
   float betta=asin(EPMajorAxis.GetY()/lengthMA);
   //if(EPMajorAxis.GetX()<0) betta=betta*(-1);
   if(EPMajorAxis.GetX()<0) betta=pi-betta;
   printf("DXFEllipse::GetPoints betta: %f betta2: %f\n",betta*180/pi,betta2*180/pi);
   //float alpha=betta+pi/2;
   //printf("DXFEllipse::GetPoints alpha: %f\n",(alpha-betta)*180/pi);
  // float v=sin(alpha)*lengthMinorAxis;		//y-value of minor axis center (0;0)
  // float u=cos(alpha)*lengthMinorAxis;		//x-value of minor axis center (0;0)
   
   printf("DXFEllipse::GetPoints lengthMinorAxis: %f lengthMajorAxis: %f \n",lengthMinorAxis,lengthMA);
   float scalx=1;    //lengthMA/lengthMinorAxis;	//EPMajorAxis.GetX()/u;
   float scaly=lengthMinorAxis;				//EPMajorAxis.GetY()/v;

   printf("DXFEllipse::GetPoints  scalx %f scaly %f\n",scalx,scaly);

   float cosAngle=cos(betta);
   float sinAngle=sin(betta);
   //for (i = 0, d = (start-betta); i < n-1; i++, d += inc)
   for (i = 0, d = (start); i < n-1; i++, d += inc)
   {
      x = (lengthMA * cos(d)); 
      y = (lengthMA * sin(d)); 
      X = cosAngle*x*scalx-sinAngle*y*scaly+center.GetX();
      Y = sinAngle*x*scalx+cosAngle*y*scaly+center.GetY();
      Z = center.GetZ();
      array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
      array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
      array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
   }
   x = lengthMA * cos(finish); 
   y = lengthMA * sin(finish); 
   //x = lengthMA * cos(finish-betta); 
   //y = lengthMA * sin(finish-betta); 
   X = cosAngle*x*scalx-sinAngle*y*scaly+center.GetX();
   Y = sinAngle*x*scalx+cosAngle*y*scaly+center.GetY();
   Z = center.GetZ(); 
   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();    
Dump();
   return offset;
}




void DXFEllipse::Dump()
{
   float x, y, z;

   center.Get(&x, &y, &z);

//   printf("   Ellipse\n");
//   printf("      Center = (%f, %f, %f)\n", x, y, z);
//   printf("      Radius = %f\n", radius);
}

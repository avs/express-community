/*****************************************************************************

******************************************************************************

Implementation of the DXFLeader class, which represents DXF leader objects.

******************************************************************************
*****************************************************************************/
#include <math.h>
#include "dxfleader.hxx"
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
**********--------   Implementations of "DXFLeader" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFLeader::Make()
Purpose:  This is the static "virtual constructor" for the DXFLeader class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a leader, then
	  an instance of DXFLeader is created, and its attributes are read
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
Returns:  A pointer to a DXFLeader object (cast to DXFPrim *) if the current
          object was recognized as a DXF leader, or
	  NULL if it wasn't.
*****************************************************************************/
DXFPrim *DXFLeader::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("LEADER"))
      instance = new DXFLeader(group, firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFLeader::DXFLeader()
Purpose:  A Constructor for the DXFLeader class.  
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
DXFLeader::DXFLeader(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   int count1=0;
   NormalVector.Set(0.0,0.0,1.0);
   isValid = 0;
   LeaderCreationFlag=3;
   NrOfVertex=0;
   HooklineFlag=-1;
   Arrowhead=1;

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 71:   // Arrowhead flag 0 : disabled 1: anabled
	 Arrowhead=(short)group.DataAsInt();
	 break;

	case 72:   // Leader Path type 0 : straight line segments 1 : spline
	 LeaderPathType=(short)group.DataAsInt();
	 break;

	case 73:   // Leader creation flag default 3
	 LeaderCreationFlag=(short)group.DataAsInt();
	 break;

	case 74:   // Hookline direction flag
	 HooklineDirectionFlag=(short)group.DataAsInt();
         NrOfVertex++;
	 break;

	case 75:   // Hookline flag
	 HooklineFlag=(short)group.DataAsInt();
	 break;

	case 40:   // Text annotion height
	 TextHeight=group.DataAsFloat();
	 break;

	case 41:   // Text annotion width
	 TextWidth=group.DataAsFloat();
	 break;

	case 76:   // Number of vertex in leader
         printf("DXFLeader::DXFLeader %i %i\n",NrOfVertex,group.DataAsInt());
	 NrOfVertex=NrOfVertex+group.DataAsInt();
	 Vertex=new DXFPoint3D[NrOfVertex];
	 break;

	case 77:   // Colour
	 color=group.DataAsInt();
	 break;

	case 10:   // vertex x coordinate
	 Vertex[count1].SetX(group.DataAsFloat());
	 printf("DXFLeader::DXFLeader x: %f\n",Vertex[count1].GetX());
	 break;

	case 20:   // vertex y coordinate
	 Vertex[count1].SetY(group.DataAsFloat());
	 break;

	case 30:   // vertex z coordinate
	 Vertex[count1].SetZ(group.DataAsFloat());
         count1++;
	 break;

	case 210:  // normal vector x coordinate
	 NormalVector.SetX(group.DataAsFloat());
	 break;

	case 220:  // normal vector y coordinate
	 NormalVector.SetY(group.DataAsFloat());
	 break;

	case 230:  // normal vector z coordinate
	 NormalVector.SetZ(group.DataAsFloat());
	 break;

	case 211:  // horizontal direction leader x coordinate
	 HorizontalDirectionLeader.SetX(group.DataAsFloat());
	 break;

	case 221:  // horizontal direction leader y coordinate
	 HorizontalDirectionLeader.SetY(group.DataAsFloat());
	 break;

	case 231:  // horizontal direction leader z coordinate
	 HorizontalDirectionLeader.SetZ(group.DataAsFloat());
	 break;

	case 212:   // direction of annotation offset from last leader vertex x coordinate
	 DirectionAnnotationOffset.SetX(group.DataAsFloat());
	 break;

	case 222:   // direction of annotation offset from last leader vertex y coordinate
	 DirectionAnnotationOffset.SetY(group.DataAsFloat());
	 break;

	case 232:   // direction of annotation offset from last leader vertex z coordinate
	 DirectionAnnotationOffset.SetZ(group.DataAsFloat());
	 break;

	default:                                  // we don't care about this group code
	 DXFPrim::ReadCommon(group, firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the leader data (its end points), we need to
   // generate the connectivity information.
   //
  
   int arrow=0;
   NrOfVertex--;
   if (Arrowhead)arrow=3; 
   if (AddCellSet(2, NrOfVertex+arrow, DXF_CELL_LINE))
   {
      for (int q = 0 ; q < (NrOfVertex+arrow) ; q++)
      { 
          cellSetsTail->AddVertexIndex(q);
          cellSetsTail->AddVertexIndex(q+1);
      }
      isValid = 1;
   }
}

/*****************************************************************************
Method:   DXFLeader::DXFLeader()
Purpose:  Copy constructor for the DXFLeader class.
Params:   v   A reference to the DXFLeader object to copy.
State:    ---
*****************************************************************************/
DXFLeader::DXFLeader(const DXFLeader &v)
{
Arrowhead=v.Arrowhead;
LeaderPathType=v.LeaderPathType;
LeaderCreationFlag=v.LeaderCreationFlag;
HooklineDirectionFlag=v.HooklineDirectionFlag;
HooklineFlag=v.HooklineFlag;
TextHeight=v.TextHeight;
TextWidth=v.TextWidth;
NrOfVertex=v.NrOfVertex;
Vertex=new DXFPoint3D[NrOfVertex+1];
for(int i=0;i<=NrOfVertex;i++)
   Vertex[i]=v.Vertex[i];
color=v.color;
NormalVector=v.NormalVector;
HorizontalDirectionLeader=v.HorizontalDirectionLeader;
DirectionAnnotationOffset=v.DirectionAnnotationOffset;
isValid=v.isValid;
}


/*****************************************************************************
Method:   DXFLeader::GetPoints()
Purpose:  This function generates the vertices associated with this leader
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
int DXFLeader::GetPoints(float *array, int offset)
{
   DXFPoint3D Ax,Ay;
   float X,Y,Z;
   printf("DXFLine::GetPoints\n");
   float c=(NormalVector.GetX()<0) ? (NormalVector.GetX()*(-1)) : NormalVector.GetX();
   float d=(NormalVector.GetY()<0) ? (NormalVector.GetY()*(-1)) : NormalVector.GetY();
   float e=1.0/64.0;
   printf("c %f d %f e: %f\n",c,d,e);
   if((c<e) && (d<e))
	{
	Ax.SetX(NormalVector.GetZ());
	Ax.SetY(0);
	Ax.SetZ(-1*NormalVector.GetX());
	}
   else
	{
	Ax.SetX(-1*NormalVector.GetY());
	Ax.SetY(NormalVector.GetX());
	Ax.SetZ(0);
	}

   Ay.SetX(NormalVector.GetY()*Ax.GetZ()-NormalVector.GetZ()*Ax.GetY());
   Ay.SetY(NormalVector.GetZ()*Ax.GetX()-NormalVector.GetX()*Ax.GetZ());
   Ay.SetZ(NormalVector.GetX()*Ax.GetY()-NormalVector.GetY()*Ax.GetX());


if(HooklineDirectionFlag==0 || HooklineDirectionFlag==1)
   {
   Vertex[NrOfVertex].Set(Vertex[NrOfVertex-1]);
   if (HooklineDirectionFlag==0)
      Vertex[NrOfVertex].SetX(Vertex[NrOfVertex-1].GetX()+TextWidth);
   if (HooklineDirectionFlag==1)
      Vertex[NrOfVertex].SetY(Vertex[NrOfVertex-1].GetY()+TextWidth);
   }

for (int q=NrOfVertex;q>=0;q--)
   {
   X = Vertex[q].GetX();
   Y = Vertex[q].GetY();
   Z = Vertex[q].GetZ();
   array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*NormalVector.GetX();
   array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*NormalVector.GetY();
   array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*NormalVector.GetZ();
   }

//  Arrowhead 
if(Arrowhead)
{
   float Gamma=0.261799387;         //15 degree angle
   double sina=sin(Gamma), cosa=cos(Gamma), sinb=sin(6.283185307-Gamma), cosb=cos(6.283185307-Gamma);
   float x1=(Vertex[1].GetX()-Vertex[0].GetX())/20;
   float y1=(Vertex[1].GetY()-Vertex[0].GetY())/20;
   float x2=(Vertex[0].GetX()-Vertex[1].GetX())/20;
   float y2=(Vertex[0].GetY()-Vertex[1].GetY())/20;
   DXFPoint3D p1,p2;
   p1.SetX(cosa*x1+(-1)*sina*y1+Vertex[0].GetX());
   p1.SetY(sina*x1+cosa*y1+Vertex[0].GetY());
   p1.SetZ(Vertex[0].GetZ());

   p2.SetX(cosb*x1+(-1)*sinb*y1+Vertex[0].GetX());
   p2.SetY(sinb*x1+cosb*y1+Vertex[0].GetY());
   p2.SetZ(Vertex[0].GetZ());

   array[offset++]=p1.GetX()*Ax.GetX()+p1.GetY()*Ay.GetX()+p1.GetZ()*NormalVector.GetX();
   array[offset++]=p1.GetX()*Ax.GetY()+p1.GetY()*Ay.GetY()+p1.GetZ()*NormalVector.GetY();
   array[offset++]=p1.GetX()*Ax.GetZ()+p1.GetY()*Ay.GetZ()+p1.GetZ()*NormalVector.GetZ();

   array[offset++]=Vertex[0].GetX()*Ax.GetX()+Vertex[0].GetY()*Ay.GetX()+Vertex[0].GetZ()*NormalVector.GetX();
   array[offset++]=Vertex[0].GetX()*Ax.GetY()+Vertex[0].GetY()*Ay.GetY()+Vertex[0].GetZ()*NormalVector.GetY();
   array[offset++]=Vertex[0].GetX()*Ax.GetZ()+Vertex[0].GetY()*Ay.GetZ()+Vertex[0].GetZ()*NormalVector.GetZ();

   array[offset++]=p2.GetX()*Ax.GetX()+p2.GetY()*Ay.GetX()+p2.GetZ()*NormalVector.GetX();
   array[offset++]=p2.GetX()*Ax.GetY()+p2.GetY()*Ay.GetY()+p2.GetZ()*NormalVector.GetY();
   array[offset++]=p2.GetX()*Ax.GetZ()+p2.GetY()*Ay.GetZ()+p2.GetZ()*NormalVector.GetZ();
}

return offset;
}

int DXFLeader::GetNumPoints()
{
if(Arrowhead) return NrOfVertex+4;
return NrOfVertex+1;
}


void DXFLeader::Dump()
{
//   printf("   Leader\n");
//   printf("      Start = (%f, %f, %f)\n",
//	  start.GetX(), start.GetY(), start.GetZ());
//   printf("        End = (%f, %f, %f)\n", end.GetX(), end.GetY(), end.GetZ());
}

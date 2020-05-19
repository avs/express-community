/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFMLine class, which represents DXF mline objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfmline.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"


#define sq(a) (a*a)

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
DXFPrim *DXFMLine::Make(DXFGroup &group,CLIST <DXFLayer>  *firstLayer)
{
   DXFPrim *instance = 0;

   if (group.DataCmp("MLINE"))
      instance = new DXFMLine(group, firstLayer);
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
DXFMLine::DXFMLine(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   isValid = 0;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);

   int i=-1,j,k;
   float *elementparameter;
   float *areafillparameter;

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // start point x coordinate
	 start.SetX(group.DataAsFloat());
	 break;

	case 20:   // start point y coordinate
	 start.SetY(group.DataAsFloat());
	 break;

	case 30:   // start point z coordinate
	 start.SetZ(group.DataAsDouble());
	 break;

	case 40:   // scale factor
	 scalefactor = group.DataAsFloat();
	 break;

	case 70:   // justification 0=top 1=zero 2=bottom
	 justification = (short)group.DataAsInt();
	 break;

	case 71:   // open close flag 1=open 3=close
	 open_close_flag = (short)group.DataAsInt();
	 break;

	case 72:   // number of vertex
	 nrOfElements = group.DataAsInt();
         Vertex = new DXFPoint3D[nrOfElements];
         SegmentVector = new DXFPoint3D[nrOfElements];
         MiterVector = new DXFPoint3D[nrOfElements];
         NrOfParameters = new int[nrOfElements];
         ElementParameters = new float*[nrOfElements];
	 NrOfAreaFillParameters = new int[nrOfElements];
	 AreaFillParameters = new float*[nrOfElements];
	 break;

	case 11:   // vertex point x coordinate
	 i++;
	 Vertex[i].SetX(group.DataAsFloat());
	 break;

	case 21:   // vertex point y coordinate
	 Vertex[i].SetY(group.DataAsFloat());
	 break;

	case 31:   // vertex point z coordinate
	 Vertex[i].SetZ(group.DataAsDouble());
	 break;

	case 12:   // segment vector x coordinate
	 SegmentVector[i].SetX(group.DataAsFloat());
	 break;

	case 22:   // segment vector y coordinate
	 SegmentVector[i].SetY(group.DataAsFloat());
	 break;

	case 32:   // segment vector z coordinate
	 SegmentVector[i].SetZ(group.DataAsDouble());
	 break;

	case 13:   // miter vector x coordinate
	 MiterVector[i].SetX(group.DataAsFloat());
	 break;

	case 23:   // miter vector y coordinate
	 MiterVector[i].SetY(group.DataAsFloat());
	 break;

	case 33:   // miter vector z coordinate
	 MiterVector[i].SetZ(group.DataAsDouble());
	 break;

	case 74:   // number of parameters in this element
	 NrOfParameters[i] = group.DataAsInt();
	 elementparameter = new float[NrOfParameters[i]];
	 ElementParameters[i]=elementparameter;
	 j=-1;
	 break;

	case 41:   // element parameter
	 j++;
	 elementparameter[j] = group.DataAsFloat();
	 break;

	case 75:   // number of area fill parameters in this element
	 NrOfAreaFillParameters[i] = group.DataAsInt();
	 areafillparameter = new float[NrOfAreaFillParameters[i]];
	 AreaFillParameters[i]=areafillparameter;
	 k=-1;
	 break;

	case 42:   // area fill parameter
	 k++;
	 areafillparameter[k] = group.DataAsFloat();
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
   int q;

   if (open_close_flag==3)   // if a closed mline
   {
      if (AddCellSet(m/2, 1, DXF_CELL_CHOOSE, DXF_CLOSED_POLYGON))
      {
	 if (group.ReverseNormals())
	 {
	    for (q = m/2 - 1; q >= 0; q--)
	       cellSetsTail->AddVertexIndex(q);
	 }
	 else
	    for (q = 0; q < m/2; q++)
	       cellSetsTail->AddVertexIndex(q);
      }
      if (AddCellSet(m/2, 1, DXF_CELL_CHOOSE, DXF_CLOSED_POLYGON))
      {
        if (group.ReverseNormals())
	{
	    for (q=m-1;q>=m/2;q--)
	       cellSetsTail->AddVertexIndex(q);
	}
	else
	    for (q = m/2; q < m; q++)
	       cellSetsTail->AddVertexIndex(q);
      }
   }
   else   // it's a non-closed mline
   {
      if (AddCellSet(2, (m/2-1), DXF_CELL_LINE))
      {
	 for (q = 0; q < ((m/2)-1); q++)
	 {
	    cellSetsTail->AddVertexIndex(q);
	    cellSetsTail->AddVertexIndex(q + 1) ;
            printf("dxfmline %i %i \n",q,q+1);
	 }
      }
      if (AddCellSet(2, ((m/2)-1), DXF_CELL_LINE))
      {
	 for (q = m/2; q < (m-1); q++)
	 {
	    cellSetsTail->AddVertexIndex(q);
	    cellSetsTail->AddVertexIndex(q + 1); 
	    printf("dxfmline %i %i \n",q,q+1); 
         }

      }
   }
	
   isValid = 1;
}


/*****************************************************************************
Method:   DXFMLine::DXFMLine()
Purpose:  Copy constructor for the DXFMLine class.
Params:   f   The DXFMline object to copy.
State:    ---
*****************************************************************************/
DXFMLine::DXFMLine(const DXFMLine &f)
{
int i,j;
start.Set(f.start);
extrusion.Set(f.extrusion);
scalefactor = f.scalefactor;
justification = f.justification;
open_close_flag = f.open_close_flag;
nrOfElements = f.nrOfElements;
Vertex = new DXFPoint3D[nrOfElements];
for (i = 0; i < nrOfElements; i++)
   Vertex[i] = f.Vertex[i];
SegmentVector = new DXFPoint3D[nrOfElements];
for (i = 0; i < nrOfElements; i++)
   SegmentVector[i] = f.SegmentVector[i];
MiterVector = new DXFPoint3D[nrOfElements];
for (i = 0; i < nrOfElements; i++)
   MiterVector[i] = f.MiterVector[i];
NrOfParameters = new int[nrOfElements];
ElementParameters = new float*[nrOfElements];
for (i = 0; i < nrOfElements; i++)
   {
   NrOfParameters[i] = f.NrOfParameters[i];
   ElementParameters[i] = new float [NrOfParameters[i]];
   for (j = 0; j < NrOfParameters[i]; j++)
       ElementParameters[i][j]=f.ElementParameters[i][j];
   }
NrOfAreaFillParameters = new int[nrOfElements];
AreaFillParameters = new float*[nrOfElements];
for (i = 0; i < nrOfElements; i++)
   {
   NrOfAreaFillParameters[i] = f.NrOfAreaFillParameters[i];
   AreaFillParameters[i]=new float[NrOfAreaFillParameters[i]];
   for(j = 0;j < NrOfAreaFillParameters[i]; j++)
       AreaFillParameters[i][j]=f.AreaFillParameters[i][j];
   }  
}


/*****************************************************************************
Method:   DXFMLine::GetNumPoints()
Purpose:  This function returns the number of points of this MLine object
Params:   ---
State:    ---
Returns:  The number of Points
*****************************************************************************/
int DXFMLine::GetNumPoints()
{
if(open_close_flag==3) return nrOfElements*2+2;
return nrOfElements*2;
}


/*****************************************************************************
Method:   DXFMLine::GetPoints()
Purpose:  This function generates the vertices associated with this mline.
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
int DXFMLine::GetPoints(float *array, int offset)
{
   int i;
   float s;
   DXFPoint3D Ax,Ay;
   int n = GetNumPoints();
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


   if(justification==0 || justification==2)
   {
     for(i = 0;i < nrOfElements; i++)
     {
       array[offset++]=Vertex[i].GetX()*Ax.GetX()+Vertex[i].GetY()*Ay.GetX()+Vertex[i].GetZ()*extrusion.GetX();
       array[offset++]=Vertex[i].GetX()*Ax.GetY()+Vertex[i].GetY()*Ay.GetY()+Vertex[i].GetZ()*extrusion.GetY();
       array[offset++]=Vertex[i].GetX()*Ax.GetZ()+Vertex[i].GetY()*Ay.GetZ()+Vertex[i].GetZ()*extrusion.GetZ();
     }
     if(open_close_flag==3)
     {
       array[offset++]=Vertex[0].GetX()*Ax.GetX()+Vertex[0].GetY()*Ay.GetX()+Vertex[0].GetZ()*extrusion.GetX();
       array[offset++]=Vertex[0].GetX()*Ax.GetY()+Vertex[0].GetY()*Ay.GetY()+Vertex[0].GetZ()*extrusion.GetY();
       array[offset++]=Vertex[0].GetX()*Ax.GetZ()+Vertex[0].GetY()*Ay.GetZ()+Vertex[0].GetZ()*extrusion.GetZ();
     }
   }
   if(justification==2)
   {
     float pi=3.141592654;
     X=(cos(pi/2)*SegmentVector[0].GetX()-sin(pi/2)*SegmentVector[0].GetY())*scalefactor + Vertex[0].GetX();
     Y=(sin(pi/2)*SegmentVector[0].GetX()+cos(pi/2)*SegmentVector[0].GetZ())*scalefactor + Vertex[0].GetY();
     Z=SegmentVector[0].GetZ()*scalefactor + Vertex[0].GetZ();

     int remOffset=offset;


     for(i=0; i < nrOfElements; i++)
     {
	if(i==0)
	   s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()+(Y-Vertex[i].GetY())*MiterVector[i].GetX())/
             (SegmentVector[i].GetX()*MiterVector[i].GetY()-SegmentVector[i].GetY()*MiterVector[i].GetX())*
             SegmentVector[i].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY();
        else
          s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()+(Y-Vertex[i].GetY())*MiterVector[i].GetX())/
             (SegmentVector[i-1].GetX()*MiterVector[i].GetY()-SegmentVector[i-1].GetY()*MiterVector[i].GetX())*
             SegmentVector[i-1].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY();
        X=Vertex[i].GetX()+s*MiterVector[i].GetX();
        Y=Vertex[i].GetY()+s*MiterVector[i].GetY();
        Z=Vertex[i].GetZ()+s*MiterVector[i].GetZ();
        array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
        array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
        array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
     }     

     if(open_close_flag==3)
     {
        array[offset++]=array[remOffset];
        array[offset++]=array[remOffset+1];
        array[offset++]=array[remOffset+2];
     }
   }
   if(justification==0)
   {
     float pi=3.141592654;
     X=(cos(-pi/2)*SegmentVector[0].GetX()-sin(-pi/2)*SegmentVector[0].GetY())*scalefactor + Vertex[0].GetX();
     Y=(sin(-pi/2)*SegmentVector[0].GetX()+cos(-pi/2)*SegmentVector[0].GetZ())*scalefactor + Vertex[0].GetY();
     Z=SegmentVector[0].GetZ()*scalefactor + Vertex[0].GetZ();

     int remOffset=offset;

     for(i=0; i < nrOfElements; i++)
     {
	if(i==0)
	   s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()*(-1)+(Y-Vertex[i].GetY())*MiterVector[i].GetX()*(-1))/
             (SegmentVector[i].GetX()*MiterVector[i].GetY()*(-1)-SegmentVector[i].GetY()*MiterVector[i].GetX()*(-1))*
             SegmentVector[i].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY()*(-1);
        else
          s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()*(-1)+(Y-Vertex[i].GetY())*MiterVector[i].GetX()*(-1))/
             (SegmentVector[i-1].GetX()*MiterVector[i].GetY()*(-1)-SegmentVector[i-1].GetY()*MiterVector[i].GetX()*(-1))*
             SegmentVector[i-1].GetY()-Vertex[i].GetY())/
          (MiterVector[i].GetY()*(-1));
        X=Vertex[i].GetX()+s*MiterVector[i].GetX()*(-1);
        Y=Vertex[i].GetY()+s*MiterVector[i].GetY()*(-1);
        Z=Vertex[i].GetZ()+s*MiterVector[i].GetZ()*(-1);
        array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
        array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
        array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
     }     
     if(open_close_flag==3)
     {
	array[offset++]=array[remOffset];
        array[offset++]=array[remOffset+1];
        array[offset++]=array[remOffset+2];
     }
   }

   if(justification==1)
   {
     float pi=3.141592654;
     X=(cos(pi/2)*SegmentVector[0].GetX()-sin(pi/2)*SegmentVector[0].GetY())*scalefactor/2 + Vertex[0].GetX();
     Y=(sin(pi/2)*SegmentVector[0].GetX()+cos(pi/2)*SegmentVector[0].GetZ())*scalefactor/2 + Vertex[0].GetY();
     Z=SegmentVector[0].GetZ()*scalefactor + Vertex[0].GetZ();

     int remOffset=offset;


     for(i=0; i < nrOfElements; i++)
     {
	if(i==0)
	   s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()+(Y-Vertex[i].GetY())*MiterVector[i].GetX())/
             (SegmentVector[i].GetX()*MiterVector[i].GetY()-SegmentVector[i].GetY()*MiterVector[i].GetX())*
             SegmentVector[i].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY();
        else
          s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()+(Y-Vertex[i].GetY())*MiterVector[i].GetX())/
             (SegmentVector[i-1].GetX()*MiterVector[i].GetY()-SegmentVector[i-1].GetY()*MiterVector[i].GetX())*
             SegmentVector[i-1].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY();
        X=Vertex[i].GetX()+s*MiterVector[i].GetX();
        Y=Vertex[i].GetY()+s*MiterVector[i].GetY();
        Z=Vertex[i].GetZ()+s*MiterVector[i].GetZ();
        array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
        array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
        array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
     }     

     if(open_close_flag==3)
     {
	array[offset++]=array[remOffset];
        array[offset++]=array[remOffset+1];
        array[offset++]=array[remOffset+2];
     }

     X=(cos(-pi/2)*SegmentVector[0].GetX()-sin(-pi/2)*SegmentVector[0].GetY())*scalefactor/2 + Vertex[0].GetX();
     Y=(sin(-pi/2)*SegmentVector[0].GetX()+cos(-pi/2)*SegmentVector[0].GetZ())*scalefactor/2 + Vertex[0].GetY();
     Z=SegmentVector[0].GetZ()*scalefactor + Vertex[0].GetZ();

     remOffset=offset;



     for(i=0; i < nrOfElements; i++)
     {
	if(i==0)
	   s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()*(-1)+(Y-Vertex[i].GetY())*MiterVector[i].GetX()*(-1))/
             (SegmentVector[i].GetX()*MiterVector[i].GetY()*(-1)-SegmentVector[i].GetY()*MiterVector[i].GetX()*(-1))*
             SegmentVector[i].GetY()-Vertex[i].GetY())/
          MiterVector[i].GetY()*(-1);
        else
          s=(Y+((Vertex[i].GetX()-X)*MiterVector[i].GetY()*(-1)+(Y-Vertex[i].GetY())*MiterVector[i].GetX()*(-1))/
             (SegmentVector[i-1].GetX()*MiterVector[i].GetY()*(-1)-SegmentVector[i-1].GetY()*MiterVector[i].GetX()*(-1))*
             SegmentVector[i-1].GetY()-Vertex[i].GetY())/
          (MiterVector[i].GetY()*(-1));
        X=Vertex[i].GetX()+s*MiterVector[i].GetX()*(-1);
        Y=Vertex[i].GetY()+s*MiterVector[i].GetY()*(-1);
        Z=Vertex[i].GetZ()+s*MiterVector[i].GetZ()*(-1);
        array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
        array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
        array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();
     }     
     if(open_close_flag==3)
     {
	array[offset++]=array[remOffset];
        array[offset++]=array[remOffset+1];
        array[offset++]=array[remOffset+2];
     }
   }


Dump();
   return offset;
}




void DXFMLine::Dump()
{
//   float x, y, z;

//  center.Get(&x, &y, &z);

//   printf("   Circle\n");
//   printf("      Center = (%f, %f, %f)\n", x, y, z);
//   printf("      Radius = %f\n", radius);
}

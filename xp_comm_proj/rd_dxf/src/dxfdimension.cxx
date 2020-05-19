/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFDimension class, which represents DXF dimension objects.

******************************************************************************
*****************************************************************************/

#include <math.h>

#include "dxfdimension.hxx"
#include "dxfprim.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"
#include "dxfline.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

double Gamma=0.261799387; 		//15 degree angle of arrow pik in linear, ... dimensions
double Alpha=0.06981317;		//4 degree angle from middlepoint in radius, ... dimensions for arrow pik
double Betta=0.017453292;		// 3 per cent of radius (length of arrow pik)


/*****************************************************************************
*********--------   Implementations of "DXFCircle" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFDimension::Make()
Purpose:  This is the static "virtual constructor" for the DXFDimension class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a dimension,
	  then an instance of DXFBlock is created, and its attributes
	  are read from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  void entities and text will included in lists
*****************************************************************************/
void DXFDimension::Make(DXFGroup &group,CLIST <DXFLayer>  *firstLayer, 
	                    CLIST <DXFPrim> *entityList, CLIST <DXFPrim> *textList)
{
   DXFDimension *instance;
   if (group.DataCmp("DIMENSION"))
      instance = new DXFDimension(group, firstLayer, entityList, textList);
 
}


/*****************************************************************************
Method:   DXFDimension::DXFDimension()
Purpose:  A Constructor for the DXFDimension class.  Reads the all necessary 
	  data from file.
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
DXFDimension::DXFDimension(DXFGroup &group, CLIST <DXFLayer> *firstLayer,
		           CLIST <DXFPrim> *entityList, CLIST <DXFPrim> *textList)
{
   curLayer=NULL;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);
   angle=0.0;

   DXFPrim *obj=entityList->GetFirst();
   if(obj) printf("DXFDimension::DXFDimension %i\n",obj->GetNumPoints());

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   //  definition point x coordinate
	 defPoint.SetX(group.DataAsFloat());
	 break;

	case 20:   // definition point y coordinate
	 defPoint.SetY(group.DataAsFloat());
	 break;

	case 30:   // definition point z coordinate
	 defPoint.SetZ(group.DataAsDouble());
	 break;

	case 11:   //  middle point text x coordinate
	 MiddlePoint.SetX(group.DataAsFloat());
	 break;

	case 21:   // middle point text y coordinate
	 MiddlePoint.SetY(group.DataAsFloat());
	 break;

	case 31:   // middle point text z coordinate
	 MiddlePoint.SetZ(group.DataAsDouble());
	 break;

	case 70:   // dimension type
	 dimType=group.DataAsInt();
	 break;

	case 1:	   // dimension text
 	 strcpy(text,group.Data());
	 break; 

	case 50:   // rotation angle of dimension text
	 angle = group.DataAsFloat();
	 printf("DXFDimension::DXFDimension angle %f/n",angle);
	 break;

	case 51:   // horizontal direction
	 horizontalDirection = group.DataAsFloat();
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

	case 12:   // insert point x coordinate only aligned, linear, rotated
	 insertPoint.SetX(group.DataAsFloat());
	 break;

	case 22:   // insert point y coordinate only aligned, linear, rotated
	 insertPoint.SetY(group.DataAsFloat());
	 break;

	case 32:   // insert point z coordinate only aligned, linear, rotated
	 insertPoint.SetZ(group.DataAsDouble());
	 break;

	case 13:   // value of definition point1 x coordinate only aligned, linear, rotated
	 valueOfDefPoint1.SetX(group.DataAsFloat());
	 break;

	case 23:   // value of definition point1 y coordinate only aligned, linear, rotated
	 valueOfDefPoint1.SetY(group.DataAsFloat());
	 break;

	case 33:   // value of definition point1 z coordinate only aligned, linear, rotated
	 valueOfDefPoint1.SetZ(group.DataAsDouble());
	 break;

	case 14:   // value of definition point2 x coordinate only aligned, linear, rotated
	 valueOfDefPoint2.SetX(group.DataAsFloat());
	 break;

	case 24:   // value of definition point2 y coordinate only aligned, linear, rotated
	 valueOfDefPoint2.SetY(group.DataAsFloat());
	 break;

	case 34:   // value of definition point2 z coordinate only aligned, linear, rotated
	 valueOfDefPoint2.SetZ(group.DataAsDouble());
	 break;

	case 15:   // definition point for diameter radial x-value
	 DefPointRadDia.SetX(group.DataAsFloat());
	 break;

	case 25:   // definition point for diameter radial y-value
	 DefPointRadDia.SetY(group.DataAsFloat());
	 break;

	case 35:   // definition point for diameter radial z-value
	 DefPointRadDia.SetZ(group.DataAsDouble());
	 break;

	case 16:   // location of dimension line arc
	 locarc.SetX(group.DataAsFloat());
	 break;

	case 26:   // location of dimension line arc
	 locarc.SetY(group.DataAsFloat());
	 break;

	case 36:   // location of dimension line arc
	 locarc.SetZ(group.DataAsDouble());
	 break;

        case 8:    // Layer name
     	//printf("DXFPrim::ReadCommon colour1: %i\n",color);
     		
	   if(firstLayer->GetFirst())
		 {	 
		 DXFLayer *labelLayer = firstLayer->GetFirst();
		 while (!(labelLayer->NameCmp(group.Data())))
		     labelLayer=firstLayer->GetNext();
		 if(labelLayer->NameCmp(group.Data())==1)
		 	 {
			 curLayer=labelLayer;
			 }
		 //flags = labelLayer->GetFlags();
		 //printf("DXFPrim::ReadCommon colour2: %i\n",color);
         }

      }
   }

   // create entities
   //
   switch (dimType)
      {
	case 0:
	case 32:   // rotated, horizontal, vertical
	    linear(entityList,textList);
        break;

	case 1:
	case 33:   // aligned
	    aligned(entityList,textList);
	break;

	case 2:
	case 34:   //angular
	    angular(entityList,textList);
	break;

	case 3: 
	case 163:   //diameter
	    diameter(entityList,textList);
	break;

	case 4: 
	case 164:   //radius
	    radius(entityList,textList);
	break;

	case 5:
	case 37:    //angular 3 point
            angular3(entityList,textList);
	break;
	    
	case 6: 
	case 38:   //ordinate
	    ordinate(entityList,textList);
        break;

	case 64:   //ordinate type
        break;

	case 128:  //text user defined position
        break;
      }
      printf("DXFDimension::DXFDimension\n");
}

/*****************************************************************************
Method:   DXFDimension::linear()
*****************************************************************************/

void DXFDimension::linear(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
float length=sqrt((valueOfDefPoint1.GetX()-valueOfDefPoint2.GetX())*(valueOfDefPoint1.GetX()-valueOfDefPoint2.GetX())+
                  (valueOfDefPoint1.GetY()-valueOfDefPoint2.GetY())*(valueOfDefPoint1.GetY()-valueOfDefPoint2.GetY())+
	          (valueOfDefPoint1.GetZ()-valueOfDefPoint2.GetZ())*(valueOfDefPoint1.GetZ()-valueOfDefPoint2.GetZ()));
sprintf(text," %f",length);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);

DXFPoint3D secendPoint;
line=new DXFLine(defPoint,valueOfDefPoint2,extrusion,curLayer);
entityList->InsertTail(line);

if(defPoint.GetX()-valueOfDefPoint2.GetX()==0)
	{
	secendPoint.SetX(valueOfDefPoint1.GetX());
	secendPoint.SetY(defPoint.GetY());
	secendPoint.SetZ(defPoint.GetZ());
	}
else
	{
	secendPoint.SetX(defPoint.GetX());
	secendPoint.SetY(valueOfDefPoint1.GetY());
	secendPoint.SetZ(defPoint.GetZ());
	}
line1=new DXFLine(secendPoint,valueOfDefPoint1,extrusion,curLayer);
entityList->InsertTail(line1);

line2=new DXFLine(secendPoint,defPoint,extrusion,curLayer);
entityList->InsertTail(line2);

double sina=sin(Gamma), cosa=cos(Gamma), sinb=sin(6.283185307-Gamma), cosb=cos(6.283185307-Gamma);
float x1=(secendPoint.GetX()-defPoint.GetX())/20;
float y1=(secendPoint.GetY()-defPoint.GetY())/20;
float x2=(defPoint.GetX()-secendPoint.GetX())/20;
float y2=(defPoint.GetY()-secendPoint.GetY())/20;
DXFPoint3D p1,p2,p3,p4;
p1.SetX(cosa*x1+(-1)*sina*y1+defPoint.GetX());
p1.SetY(sina*x1+cosa*y1+defPoint.GetY());
p1.SetZ(defPoint.GetZ());
line3=new DXFLine(defPoint,p1,extrusion,curLayer);
entityList->InsertTail(line3);

p2.SetX(cosb*x1+(-1)*sinb*y1+defPoint.GetX());
p2.SetY(sinb*x1+cosb*y1+defPoint.GetY());
p2.SetZ(defPoint.GetZ());
line4=new DXFLine(defPoint,p2,extrusion,curLayer);
entityList->InsertTail(line4);

p3.SetX(cosa*x2+(-1)*sina*y2+secendPoint.GetX());
p3.SetY(sina*x2+cosa*y2+secendPoint.GetY());
p3.SetZ(secendPoint.GetZ());
line5=new DXFLine(secendPoint,p3,extrusion,curLayer);
entityList->InsertTail(line5);

p4.SetX(cosb*x2+(-1)*sinb*y2+secendPoint.GetX());
p4.SetY(sinb*x2+cosb*y2+secendPoint.GetY());
p4.SetZ(secendPoint.GetZ());
line6=new DXFLine(secendPoint,p4,extrusion,curLayer);
entityList->InsertTail(line6);
}

/*****************************************************************************
Method:   DXFDimension::aligned()
*****************************************************************************/

void DXFDimension::aligned(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
float length=sqrt((valueOfDefPoint1.GetX()-valueOfDefPoint2.GetX())*(valueOfDefPoint1.GetX()-valueOfDefPoint2.GetX())+
                  (valueOfDefPoint1.GetY()-valueOfDefPoint2.GetY())*(valueOfDefPoint1.GetY()-valueOfDefPoint2.GetY())+
	          (valueOfDefPoint1.GetZ()-valueOfDefPoint2.GetZ())*(valueOfDefPoint1.GetZ()-valueOfDefPoint2.GetZ()));
sprintf(text," %f",length);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);

DXFPoint3D secendPoint;
line=new DXFLine(defPoint,valueOfDefPoint2,extrusion,curLayer);
entityList->InsertTail(line);

secendPoint.SetX(valueOfDefPoint1.GetX()+defPoint.GetX()-valueOfDefPoint2.GetX());
secendPoint.SetY(valueOfDefPoint1.GetY()+defPoint.GetY()-valueOfDefPoint2.GetY());
secendPoint.SetZ(valueOfDefPoint1.GetZ()+defPoint.GetZ()-valueOfDefPoint2.GetZ());
line1=new DXFLine(secendPoint,valueOfDefPoint1,extrusion,curLayer);
entityList->InsertTail(line1);

line2=new DXFLine(secendPoint,defPoint,extrusion,curLayer);
entityList->InsertTail(line2);

double sina=sin(Gamma), cosa=cos(Gamma), sinb=sin(6.283185307-Gamma), cosb=cos(6.283185307-Gamma);
float x1=(secendPoint.GetX()-defPoint.GetX())/20;
float y1=(secendPoint.GetY()-defPoint.GetY())/20;
float x2=(defPoint.GetX()-secendPoint.GetX())/20;
float y2=(defPoint.GetY()-secendPoint.GetY())/20;
DXFPoint3D p1,p2,p3,p4;
p1.SetX(cosa*x1+(-1)*sina*y1+defPoint.GetX());
p1.SetY(sina*x1+cosa*y1+defPoint.GetY());
p1.SetZ(defPoint.GetZ());
line3=new DXFLine(defPoint,p1,extrusion,curLayer);
entityList->InsertTail(line3);

p2.SetX(cosb*x1+(-1)*sinb*y1+defPoint.GetX());
p2.SetY(sinb*x1+cosb*y1+defPoint.GetY());
p2.SetZ(defPoint.GetZ());
line4=new DXFLine(defPoint,p2,extrusion,curLayer);
entityList->InsertTail(line4);

p3.SetX(cosa*x2+(-1)*sina*y2+secendPoint.GetX());
p3.SetY(sina*x2+cosa*y2+secendPoint.GetY());
p3.SetZ(secendPoint.GetZ());
line5=new DXFLine(secendPoint,p3,extrusion,curLayer);
entityList->InsertTail(line5);

p4.SetX(cosb*x2+(-1)*sinb*y2+secendPoint.GetX());
p4.SetY(sinb*x2+cosb*y2+secendPoint.GetY());
p4.SetZ(secendPoint.GetZ());
line6=new DXFLine(secendPoint,p4,extrusion,curLayer);
entityList->InsertTail(line6);
}

/*****************************************************************************
Method:   DXFDimension::diameter()
*****************************************************************************/

void DXFDimension::diameter(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
float length=sqrt((defPoint.GetX()-DefPointRadDia.GetX())*(defPoint.GetX()-DefPointRadDia.GetX())+
                  (defPoint.GetY()-DefPointRadDia.GetY())*(defPoint.GetY()-DefPointRadDia.GetY())+
	          (defPoint.GetZ()-DefPointRadDia.GetZ())*(defPoint.GetZ()-DefPointRadDia.GetZ()));
sprintf(text," %f",length);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);

line=new DXFLine(defPoint,DefPointRadDia,extrusion,curLayer);
entityList->InsertTail(line);

double sina=sin(Gamma), cosa=cos(Gamma), sinb=sin(6.283185307-Gamma), cosb=cos(6.283185307-Gamma);
float x1=(DefPointRadDia.GetX()-defPoint.GetX())/20;
float y1=(DefPointRadDia.GetY()-defPoint.GetY())/20;
float x2=(defPoint.GetX()-DefPointRadDia.GetX())/20;
float y2=(defPoint.GetY()-DefPointRadDia.GetY())/20;
DXFPoint3D p1,p2,p3,p4;
p1.SetX(cosa*x1+(-1)*sina*y1+defPoint.GetX());
p1.SetY(sina*x1+cosa*y1+defPoint.GetY());
p1.SetZ(defPoint.GetZ());
line3=new DXFLine(defPoint,p1,extrusion,curLayer);
entityList->InsertTail(line3);

p2.SetX(cosb*x1+(-1)*sinb*y1+defPoint.GetX());
p2.SetY(sinb*x1+cosb*y1+defPoint.GetY());
p2.SetZ(defPoint.GetZ());
line4=new DXFLine(defPoint,p2,extrusion,curLayer);
entityList->InsertTail(line4);

p3.SetX(cosa*x2+(-1)*sina*y2+DefPointRadDia.GetX());
p3.SetY(sina*x2+cosa*y2+DefPointRadDia.GetY());
p3.SetZ(DefPointRadDia.GetZ());
line5=new DXFLine(DefPointRadDia,p3,extrusion,curLayer);
entityList->InsertTail(line5);

p4.SetX(cosb*x2+(-1)*sinb*y2+DefPointRadDia.GetX());
p4.SetY(sinb*x2+cosb*y2+DefPointRadDia.GetY());
p4.SetZ(DefPointRadDia.GetZ());
line6=new DXFLine(DefPointRadDia,p4,extrusion,curLayer);
entityList->InsertTail(line6);
}

/*****************************************************************************
Method:   DXFDimension::radius()
*****************************************************************************/

void DXFDimension::radius(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
float length=sqrt((defPoint.GetX()-DefPointRadDia.GetX())*(defPoint.GetX()-DefPointRadDia.GetX())+
                  (defPoint.GetY()-DefPointRadDia.GetY())*(defPoint.GetY()-DefPointRadDia.GetY())+
	          (defPoint.GetZ()-DefPointRadDia.GetZ())*(defPoint.GetZ()-DefPointRadDia.GetZ()));
sprintf(text," %f",length);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);

line=new DXFLine(defPoint,DefPointRadDia,extrusion,curLayer);
entityList->InsertTail(line);
double sina=sin(Gamma), cosa=cos(Gamma), sinb=sin(6.283185307-Gamma), cosb=cos(6.283185307-Gamma);
float x2=(defPoint.GetX()-DefPointRadDia.GetX())/10;
float y2=(defPoint.GetY()-DefPointRadDia.GetY())/10;
DXFPoint3D p3,p4;

p3.SetX(cosa*x2+(-1)*sina*y2+DefPointRadDia.GetX());
p3.SetY(sina*x2+cosa*y2+DefPointRadDia.GetY());
p3.SetZ(DefPointRadDia.GetZ());
line5=new DXFLine(DefPointRadDia,p3,extrusion,curLayer);
entityList->InsertTail(line5);

p4.SetX(cosb*x2+(-1)*sinb*y2+DefPointRadDia.GetX());
p4.SetY(sinb*x2+cosb*y2+DefPointRadDia.GetY());
p4.SetZ(DefPointRadDia.GetZ());
line6=new DXFLine(DefPointRadDia,p4,extrusion,curLayer);
entityList->InsertTail(line6);
}

/*****************************************************************************
Method:   DXFDimension::angular()
*****************************************************************************/

void DXFDimension::angular(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
DXFPoint3D middlePoint;
float x1=defPoint.GetX();
float y1=defPoint.GetY();
float x2=DefPointRadDia.GetX();
float y2=DefPointRadDia.GetY();
float x3=valueOfDefPoint2.GetX();
float y3=valueOfDefPoint2.GetY();
float x4=valueOfDefPoint1.GetX();
float y4=valueOfDefPoint1.GetY();


float u=( y1-y3+ ( x3* ( y2-y1 ) -x1* ( y2-y1 ) ) / ( x2-x1 ) ) /
	( ( y4-y3 ) - ( (x4-x3)*(y2-y1) ) / ( x2-x1 ) );
middlePoint.SetX( valueOfDefPoint2.GetX()+u*( valueOfDefPoint1.GetX()-valueOfDefPoint2.GetX() ) );
middlePoint.SetY( valueOfDefPoint2.GetY()+u*( valueOfDefPoint1.GetY()-valueOfDefPoint2.GetY() ) );
middlePoint.SetZ( valueOfDefPoint2.GetZ()+u*( valueOfDefPoint1.GetZ()-valueOfDefPoint2.GetZ() ) );
printf("DXFDimension::angular u: %f points%f, %f, %f\n",u,middlePoint.GetX(),middlePoint.GetY(),middlePoint.GetZ());

float r=sqrt((locarc.GetX()-middlePoint.GetX())*(locarc.GetX()-middlePoint.GetX())+
	     (locarc.GetY()-middlePoint.GetY())*(locarc.GetY()-middlePoint.GetY())+
	     (locarc.GetZ()-middlePoint.GetZ())*(locarc.GetZ()-middlePoint.GetZ()));

float c1=sqrt((valueOfDefPoint2.GetX()-middlePoint.GetX())*(valueOfDefPoint2.GetX()-middlePoint.GetX())+
	      (valueOfDefPoint2.GetY()-middlePoint.GetY())*(valueOfDefPoint2.GetY()-middlePoint.GetY())+
	      (valueOfDefPoint2.GetZ()-middlePoint.GetZ())*(valueOfDefPoint2.GetZ()-middlePoint.GetZ()));

float c2=sqrt((DefPointRadDia.GetX()-middlePoint.GetX())*(DefPointRadDia.GetX()-middlePoint.GetX())+
	      (DefPointRadDia.GetY()-middlePoint.GetY())*(DefPointRadDia.GetY()-middlePoint.GetY())+
	      (DefPointRadDia.GetZ()-middlePoint.GetZ())*(DefPointRadDia.GetZ()-middlePoint.GetZ()));

printf("DXFDimension::angular c1 %f c2 %f  a1 %f a2 %f\n",c1,c2,(valueOfDefPoint2.GetX()-middlePoint.GetX()),(DefPointRadDia.GetX()-middlePoint.GetX()));
float angle3=asin((valueOfDefPoint2.GetX()-middlePoint.GetX())/c1);
float angle4=asin((DefPointRadDia.GetX()-middlePoint.GetX())/c2);
float angle1, angle2;
if (angle3<0.0) angle1=angle3*(-1); else angle1=angle3;
if (angle4<0.0) angle2=angle4*(-1); else angle2=angle4;
angle1=angle1+angle2;

sprintf(text," %f",angle1*180/3.141592654);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);
double start=4.71238898+angle4;
double end=4.71238898+angle3;
nArc=new DXFArc(middlePoint,extrusion,r,start,end,curLayer);
entityList->InsertTail(nArc);

DXFPoint3D StartPoint,EndPoint;
StartPoint.SetX(r*cos(start)+middlePoint.GetX());
StartPoint.SetY(r*sin(start)+middlePoint.GetY());
StartPoint.SetZ(middlePoint.GetZ());
line=new DXFLine(defPoint,StartPoint,extrusion,curLayer);
entityList->InsertTail(line);

EndPoint.SetX(r*cos(end)+middlePoint.GetX());
EndPoint.SetY(r*sin(end)+middlePoint.GetY());
EndPoint.SetZ(middlePoint.GetZ());
line1=new DXFLine(valueOfDefPoint2,EndPoint,extrusion,curLayer);
entityList->InsertTail(line1);

Betta=r*0.015;
DXFPoint3D point1, point2;
point1.SetX((r+Betta)*cos(start+Alpha)+middlePoint.GetX());
point1.SetY((r+Betta)*sin(start+Alpha)+middlePoint.GetY());
point1.SetZ(middlePoint.GetZ());
line2=new DXFLine(StartPoint,point1,extrusion,curLayer);
entityList->InsertTail(line2);

point2.SetX((r-Betta)*cos(start+Alpha)+middlePoint.GetX());
point2.SetY((r-Betta)*sin(start+Alpha)+middlePoint.GetY());
point2.SetZ(middlePoint.GetZ());
line3=new DXFLine(StartPoint,point2,extrusion,curLayer);
entityList->InsertTail(line3);

DXFPoint3D point3, point4;
point3.SetX((r+Betta)*cos(end-Alpha)+middlePoint.GetX());
point3.SetY((r+Betta)*sin(end-Alpha)+middlePoint.GetY());
point3.SetZ(middlePoint.GetZ());
line4=new DXFLine(EndPoint,point3,extrusion,curLayer);
entityList->InsertTail(line4);

point4.SetX((r-Betta)*cos(end-Alpha)+middlePoint.GetX());
point4.SetY((r-Betta)*sin(end-Alpha)+middlePoint.GetY());
point4.SetZ(middlePoint.GetZ());
line5=new DXFLine(EndPoint,point4,extrusion,curLayer);
entityList->InsertTail(line5);
}
/*****************************************************************************
Method:   DXFDimension::angular3()
*****************************************************************************/

void DXFDimension::angular3(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
float r=sqrt((defPoint.GetX()-DefPointRadDia.GetX())*(defPoint.GetX()-DefPointRadDia.GetX())+
             (defPoint.GetY()-DefPointRadDia.GetY())*(defPoint.GetY()-DefPointRadDia.GetY())+
             (defPoint.GetZ()-DefPointRadDia.GetZ())*(defPoint.GetZ()-DefPointRadDia.GetZ()));

float c1=sqrt((valueOfDefPoint2.GetX()-DefPointRadDia.GetX())*(valueOfDefPoint2.GetX()-DefPointRadDia.GetX())+
	      (valueOfDefPoint2.GetY()-DefPointRadDia.GetY())*(valueOfDefPoint2.GetY()-DefPointRadDia.GetY())+
	      (valueOfDefPoint2.GetZ()-DefPointRadDia.GetZ())*(valueOfDefPoint2.GetZ()-DefPointRadDia.GetZ()));

float c2=sqrt((valueOfDefPoint1.GetX()-DefPointRadDia.GetX())*(valueOfDefPoint1.GetX()-DefPointRadDia.GetX())+
	      (valueOfDefPoint1.GetY()-DefPointRadDia.GetY())*(valueOfDefPoint1.GetY()-DefPointRadDia.GetY())+
	      (valueOfDefPoint1.GetZ()-DefPointRadDia.GetZ())*(valueOfDefPoint1.GetZ()-DefPointRadDia.GetZ()));

float angle3=asin((valueOfDefPoint2.GetX()-DefPointRadDia.GetX())/c1);
float angle4=asin((valueOfDefPoint1.GetX()-DefPointRadDia.GetX())/c2);
float angle1, angle2;
if (angle3<0.0) angle1=angle3*(-1); else angle1=angle3;
if (angle4<0.0) angle2=angle4*(-1); else angle2=angle4;
angle1=angle1+angle2;

sprintf(text," %f",angle1*180/3.141592654);
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);

double start=4.71238898+angle4;
double end=4.71238898+angle3;
nArc=new DXFArc(DefPointRadDia,extrusion,r,start,end,curLayer);
entityList->InsertTail(nArc);

DXFPoint3D StartPoint,EndPoint;
StartPoint.SetX(r*cos(start)+DefPointRadDia.GetX());
StartPoint.SetY(r*sin(start)+DefPointRadDia.GetY());
StartPoint.SetZ(DefPointRadDia.GetZ());
line=new DXFLine(valueOfDefPoint2,StartPoint,extrusion,curLayer);
entityList->InsertTail(line);

EndPoint.SetX(r*cos(end)+DefPointRadDia.GetX());
EndPoint.SetY(r*sin(end)+DefPointRadDia.GetY());
EndPoint.SetZ(DefPointRadDia.GetZ());
line1=new DXFLine(valueOfDefPoint1,EndPoint,extrusion,curLayer);
entityList->InsertTail(line1);

Betta=r*0.015;
DXFPoint3D point1, point2;
point1.SetX((r+Betta)*cos(start+Alpha)+DefPointRadDia.GetX());
point1.SetY((r+Betta)*sin(start+Alpha)+DefPointRadDia.GetY());
point1.SetZ(DefPointRadDia.GetZ());
line2=new DXFLine(StartPoint,point1,extrusion,curLayer);
entityList->InsertTail(line2);

point2.SetX((r-Betta)*cos(start+Alpha)+DefPointRadDia.GetX());
point2.SetY((r-Betta)*sin(start+Alpha)+DefPointRadDia.GetY());
point2.SetZ(DefPointRadDia.GetZ());
line3=new DXFLine(StartPoint,point2,extrusion,curLayer);
entityList->InsertTail(line3);

DXFPoint3D point3, point4;
point3.SetX((r+Betta)*cos(end-Alpha)+DefPointRadDia.GetX());
point3.SetY((r+Betta)*sin(end-Alpha)+DefPointRadDia.GetY());
point3.SetZ(DefPointRadDia.GetZ());
line4=new DXFLine(EndPoint,point3,extrusion,curLayer);
entityList->InsertTail(line4);

point4.SetX((r-Betta)*cos(end-Alpha)+DefPointRadDia.GetX());
point4.SetY((r-Betta)*sin(end-Alpha)+DefPointRadDia.GetY());
point4.SetZ(DefPointRadDia.GetZ());
line5=new DXFLine(EndPoint,point4,extrusion,curLayer);
entityList->InsertTail(line5);
}

/*****************************************************************************
Method:   DXFDimension::ordinate()
*****************************************************************************/

void DXFDimension::ordinate(CLIST <DXFPrim> *entityList,CLIST <DXFPrim> *textList)
{
sprintf(text," %f",valueOfDefPoint1.GetY());
nText=new DXFText(text,MiddlePoint,extrusion,angle);
textList->InsertTail(nText);
DXFPoint3D point1;
point1.SetX((valueOfDefPoint2.GetX()-valueOfDefPoint1.GetX())*.52+valueOfDefPoint1.GetX());
point1.SetY(valueOfDefPoint1.GetY());
point1.SetZ(valueOfDefPoint1.GetZ());
line=new DXFLine(valueOfDefPoint1,point1,extrusion,curLayer);
entityList->InsertTail(line);

DXFPoint3D point2;
point2.SetX((valueOfDefPoint2.GetX()-valueOfDefPoint1.GetX())*.48+valueOfDefPoint1.GetX());
point2.SetY(valueOfDefPoint2.GetY());
point2.SetZ(valueOfDefPoint2.GetZ());
line1=new DXFLine(valueOfDefPoint2,point2,extrusion,curLayer);
entityList->InsertTail(line1);

line2=new DXFLine(point1,point2,extrusion,curLayer);
entityList->InsertTail(line2);
}

void DXFDimension::Dump()
{
 //  float x, y, z;

   //center.Get(&x, &y, &z);

//   printf("   Circle\n");
//   printf("      Center = (%f, %f, %f)\n", x, y, z);
//   printf("      Radius = %f\n", radius);
}

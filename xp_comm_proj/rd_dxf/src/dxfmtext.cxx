
/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFText class, which represents DXF text objects.

******************************************************************************
*****************************************************************************/
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxftext.hxx"
#include "dxfprim.hxx"
#include "dxfdefine.hxx"
#include "dxfmtext.hxx"
#include "../gen.hxx"
#include <string.h>
#include <math.h>

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFLine" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFMText::Make()
Purpose:  This is the static "virtual constructor" for the DXFMText class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a text, then
	  an instance of DXFText is created, and its attributes are read
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
Returns:  A pointer to a DXFText object (cast to DXFPrim *) if the current
          object was recognized as a DXF text, or
	  NULL if it wasn't.
*****************************************************************************/
void DXFMText::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *textList)
{
   DXFMText *instance = 0;

   if (group.DataCmp("MTEXT")) 
      instance = new DXFMText(group, firstLayer,textList);

   return;
}


/*****************************************************************************
Method:   DXFText::DXFText()
Purpose:  A Constructor for the DXFText class.  Reads the center point
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
DXFMText::DXFMText(DXFGroup &group, CLIST <DXFLayer> *firstLayer, CLIST <DXFPrim> *textList) 
{
   short draw=0;
   short temp=0;

   addtext=0;
   Text.point.SetX(0);
   Text.point.SetY(0);
   Text.point.SetZ(0);
   Text.orient=4;
   Text.extrusion.SetX(0);
   Text.extrusion.SetY(0);
   Text.extrusion.SetZ(1);
   Text.align_horiz=0;
   Text.align_vert=0;
   Text.path=0;
   Text.angle=0;
   Text.height=0;
   Text.expansion=1;
   Text.plane=0;
   Text.style=0;
   directionVector.Set(0.0,0.0,1.0);

   
   printf("DXFMText::DXFMText\n");

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
	case 10:   // insert point x coordinate
	 insertpoint.SetX(group.DataAsFloat());
	 break;

	case 20:   // insert point y coordinate
	 insertpoint.SetY(group.DataAsFloat());
	 break;

	case 30:   // insert point z coordinate
	 insertpoint.SetZ(group.DataAsFloat());
	 break;

	case 1:    // text string
	case 3:    // additional text lines
	 if (addtext>9) break;
	 mtext[addtext]=(char *)malloc(256);
	 strcpy(mtext[addtext], group.Data());
         addtext++;
	 break;

	case 72:    // drawing direction
	 draw=(short) group.DataAsInt();
         if(draw==1) Text.path=0;
	 if(draw==2) Text.path=2;
	 if(draw==3) Text.path=3;
	 if(draw==4) Text.path=1;
	 break;

	case 71:    // align_horizontal, align_vetical
	 temp=(short) group.DataAsInt();
	 Text.align_horiz=(temp+2)%3;
	 if(temp==1 || temp==2 || temp==3) Text.align_vert=3;
	 if(temp==4 || temp==5 || temp==6) Text.align_vert=2;
	 if(temp==7 || temp==8 || temp==9) Text.align_vert=1;
	 break; 

	case 50:    // Rotation angle
	 Text.angle=group.DataAsFloat();
	 break;

	case 40:    // Textheight
	 Text.height=group.DataAsFloat();
	 break;

	case 41:    // reference rectangle width
	 rectwidth=group.DataAsFloat();
	 if (rectwidth==0.0) rectwidth=256.0;
	 break;

        case 210:   // normal vector
	 Text.extrusion.SetX(group.DataAsFloat());
	 break;

        case 220:   // normal vector
	 Text.extrusion.SetY(group.DataAsFloat());
	 break;

	case 230:   // normal vector
	 Text.extrusion.SetZ(group.DataAsFloat());
	 break;

        case 11:    // direction vector
	 directionVector.SetX(group.DataAsFloat());
	 break;

        case 21:    // direction vector
	 directionVector.SetY(group.DataAsFloat());
	 break;

	case 31:    // direction vector
	 directionVector.SetZ(group.DataAsFloat());
	 break;

      }
   }

   float angle1=asin(directionVector.GetY()/
     sqrt(directionVector.GetX()*directionVector.GetX()+
          directionVector.GetY()*directionVector.GetY()+
	  directionVector.GetZ()*directionVector.GetZ()));
   if(directionVector.GetX()<0)angle1=3.141592654-angle1;
   printf("DXFMText::DXFMText angle %f %f\n",angle1,rectwidth);
   Text.angle=angle1*180/3.141592654;
   float deltay=sin(1.570796327-angle1)*(Text.height+1);
   float deltax=sin(angle1)*(Text.height+1);
   int nrOfChar=rectwidth/(Text.height);
   printf("DXFMText::DXFMText delta %f %f nrOfChar %i %s\n",deltax,deltay,nrOfChar,mtext[0]);
   for (int i=0;i<addtext;i++)
      	{
	int a=0, b=0;
        while(mtext[i][a]!='\0' && mtext[i][a]!='\n')
	    {
	    if(mtext[i][a]=='\\')
	       {
	       a++;if(b>=0)b--;
	       if(mtext[i][a]=='L')		//underline
	           {
	           Text.underline=1;
	           }
               if(mtext[i][a]=='l')		//underline end
                   {
                   Text.underline=0;
                   }
	       if(mtext[i][a]=='P')		//new line
		   {
                   b++;
		   Text.text[b]='\0';
		   b=-1;
		   Text.point=insertpoint;
		   insertpoint.SetX(insertpoint.GetX()+deltax);
		   insertpoint.SetY(insertpoint.GetY()-deltay);
		   DXFPrim *obj=new DXFText(Text);
		   textList->InsertBehind(obj);
		   }
	       if(mtext[i][a]=='A')
		   {
		   printf("dxfmtext  I'm in A\n");
		   while(mtext[i][a]!=';')a++;
		   }
	       }
	    else if(b<=nrOfChar)
	       {
	       printf("dxfmtext char %c",mtext[i][a]);	
	       Text.text[b]=mtext[i][a];
	       }
	    else
		{
		b--;a--;
		for (;Text.text[b]!=' ' && b>=0;b--,a--);
		if (Text.text[b]==' ')
                   {
	           Text.text[b]='\0';
		   b=-1;
		   Text.point=insertpoint;
		   insertpoint.SetX(insertpoint.GetX()+deltax);
		   insertpoint.SetY(insertpoint.GetY()-deltay);
		   DXFPrim *obj=new DXFText(Text);
		   textList->InsertBehind(obj);
		   }
		else
		   {
		   for(b=0,a++;mtext[i][a]!=' ' && mtext[i][a]!='\0' && mtext[i][a]!='\n'
			&& !(mtext[i][a]=='\\' && mtext[i][a+1]=='P') ; b++,a++)
			{
			if(mtext[i][a]=='\\')
	       		   {
	       		   a++;
	         	   if(mtext[i][a]=='L')		//underline
	            	       {
	           	       Text.underline=1;
	           	       }
	                   if(mtext[i][a]=='A')
		               {
		               while(mtext[i][a]!=';') a++;
		               }
	                   }
			else 
		           Text.text[b]=mtext[i][a];
		        }
		    if(mtext[i][a]=='\\' && mtext[i][a+1]=='P') a++;
	            Text.text[b]='\0';
	            b=-1;
	            if(mtext[i][a]=='\0' || mtext[i][a]=='\n') a--;
 		    Text.point=insertpoint;
		    insertpoint.SetX(insertpoint.GetX()+deltax);
		    insertpoint.SetY(insertpoint.GetY()-deltay);
		    DXFPrim *obj=new DXFText(Text);
		    textList->InsertBehind(obj);    
		    }
		}
   	     b++;
	     a++;
	     }
	Text.text[b]='\0';
        Text.point=insertpoint;
        DXFPrim *obj=new DXFText(Text);
        textList->InsertBehind(obj);
	}
   
}

DXFMText::~DXFMText()
{
for (int i=0;i<addtext;i++) 
	delete mtext[i];
}


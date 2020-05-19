/**************************************************************************
Minimal

******************************************************************************

Implementation of the DXFInsert class, which represents DXF insert objects.

******************************************************************************
*****************************************************************************/
#include <string.h>
#include <math.h>
#include "dxfinsert.hxx"
#include "dxfprim.hxx"
#include "dxfblock.hxx"
#include "dxftext.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFInsert" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFInsert::Make()
Purpose:  This is the static "virtual constructor" for the DXFInsert class.
          If the contents of the specified "group" object is recognized
	  by this function as the start of the definition of a insert, then
	  an instance of DXFInsert is created, and its attributes are read
	  from the DXF file (via the "group" object).
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFInsert object (cast to DXFPrim *) if the current
          object was recognized as a DXF insert, or
	  NULL if it wasn't.
*****************************************************************************/
DXFInsert *DXFInsert::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   DXFInsert *instance = 0;

   if (group.DataCmp("INSERT"))
      instance = new DXFInsert(group,firstLayer);

   return instance;
}


/*****************************************************************************
Method:   DXFInsert::DXFInsert()
Purpose:  A Constructor for the DXFInsert class.  Reads the start point and 
          (optional) scale factor from the DXF file.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFInsert::DXFInsert(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : start(), Blockname(), firstPointIndex(0)
{
   
   isValid = 0;
   scaleFactorx=1;
   scaleFactory=1;
   scaleFactorz=1;
   rotationAngle=0;
   extrusion.SetX(0);
   extrusion.SetY(0);
   extrusion.SetZ(1);

   while (group.Read())
   {
      if (group.Code() == 0)   // stop at next entity definition
	 break;

      switch (group.Code())
      {
      
        case 2:
         Blockname=group.Data();
         break;
         
	case 10:   // insert start point x coordinate
	 start.SetX(group.DataAsFloat());
	  printf("insert x: %f\n",start.GetX());
	 break;

	case 20:   // insert start point y coordinate
	 start.SetY(group.DataAsFloat());
	  printf("insert y: %f\n",start.GetY());
	 break;

	case 30:   // insert start point z coordinate
	 start.SetZ(group.DataAsFloat());
	  printf("insert z: %f\n",start.GetZ());
	 break;

	case 41:   //scale factor x
	 scaleFactorx=group.DataAsFloat();
	 break;
	
	case 42:
	 scaleFactory=group.DataAsFloat();
	 break;
	 
	case 43:
	 scaleFactorz=group.DataAsFloat();
	 break;
	 
	case 50:
	 rotationAngle=group.DataAsFloat()*3.141592654/180;
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

	 
	default:                      // we don't care about this group code
	 DXFPrim::ReadCommon(group,firstLayer);  // see if the base class wants it
	 break;
      }
   }

   // Now that we have the insert data (its end points), we need to
   // generate the connectivity information.
   //
/*   if (AddCellSet(2, 1))
   {
      cellSetsTail->AddVertexIndex(0);
      cellSetsTail->AddVertexIndex(1);

      isValid = 1;
   }*/
   isValid=1;
}

/*****************************************************************************
Method:   DXFInsert::GetNumPoints()
Purpose:  Counts the total number of points (vertices) in the block
Params:   DXFBlockList * list of blocks
State:    ---
Returns:  The total number of points (vertices) in the block
*****************************************************************************/
int DXFInsert::GetNumPoints(CLIST <DXFBlock> *blocklist, DXFLayer * layer)
{
curLayer=layer;
block=NULL;
if (blocklist)
  {
  block=blocklist->GetFirst();
  while (block)
	{
	if(strcmp(block->Blockname.Get(),Blockname.Get())!=0)
		block=blocklist->GetNext();
	else
		break;
	}
  }
//if(strcmp(blocklist->object->Blockname.Get(),Blockname.Get())==0)
if(block)
	{
	printf("dxfinsert GetNumPoints Block gefunden.\n");
	return block->GetNumPoints(blocklist);
	}
return 0;
}

int DXFInsert::GetNumPoints(CLIST <DXFBlock> *blocklist)
{
block=NULL;
if (blocklist)
  {
  block=blocklist->GetFirst();
  while (block)
	{
	if(strcmp(block->Blockname.Get(),Blockname.Get())!=0)
		block=blocklist->GetNext();
	else
		break;
	}
  }
//if(strcmp(blocklist->object->Blockname.Get(),Blockname.Get())==0)
if(block)
	{
	// printf("dxfinsert GetNumPoints Block gefunden.\n");
	return block->GetNumPoints(blocklist);
	}
return 0;
}

int DXFInsert::GetNumPoints()
{
if(block)
	{
	// printf("dxfinsert GetNumPoints Block gefunden.\n");
	return block->GetNumPoints();
	}
return 0;
}

/*****************************************************************************
Method:   DXFInsert::GetPoints()
Purpose:  This function generates the vertices associated with this insert
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
int DXFInsert::GetPoints(float *array, int offset)
{

int a;
int numPoints;
DXFPoint3D Ax,Ay;
float x,X,y,Y,z,Z,Tx,Ty,Tz;

//if (curLayer!=GetLayer())return 0;
if(block)
	{
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
        
        printf("DXFInsert::GetPoints Az: %f %f %f Ax: %f %f %f Ay: %f %f %f \n Block: %s \n",extrusion.GetX(),extrusion.GetY(),extrusion.GetZ(),
	Ax.GetX(),Ax.GetY(),Ax.GetZ(),Ay.GetX(),Ay.GetY(),Ay.GetZ(),Blockname.Get());


	numPoints=block->GetNumPoints();
	printf("DXFInsert::GetPoints numPoints: =%i\n",numPoints);
	block->GetPoints(array,offset);
        printf("DXFInsert::GetPoints after GetPoints\n");
	firstPointIndex=(offset)/3;
	printf("DXFInsert::GetPoints FPI:%i\n",firstPointIndex);
	
	float sinAlpha=sin(rotationAngle);
	float cosAlpha=cos(rotationAngle);

	a=offset;
	// printf("DXFInsert::GetPoints %f %f %f\n",array[offset],array[offset+1],array[offset+2]);
	printf("DXFInsert GetPoints new offset= %i\n",x);
	
	for(int i=0;i<numPoints;i++)
		{
		Tx=start.GetX()-block->GetBasePoint().GetX();
		Ty=start.GetY()-block->GetBasePoint().GetY();
		Tz=start.GetZ()-block->GetBasePoint().GetZ();
		//printf("start.GetZ(): %f block->GetBasePoint().GetZ(): %f Tz: %f\n",start.GetZ(),block->GetBasePoint().GetZ(),Tz);
		x=array[offset];
		y=array[offset+1];
		z=array[offset+2];
		//printf("insert 1   x:%f y:%f\n",x,y);

 		X=cosAlpha*x*scaleFactorx-sinAlpha*y*scaleFactory+Tx;
 		Y=sinAlpha*x*scaleFactorx+cosAlpha*y*scaleFactory+Ty;
 		Z=scaleFactorz*z+Tz;
		//printf("insert       x:%f y:%f z:%f\n",X,Y,Z);
		array[offset++]=X*Ax.GetX()+Y*Ay.GetX()+Z*extrusion.GetX();
		array[offset++]=X*Ax.GetY()+Y*Ay.GetY()+Z*extrusion.GetY();
		array[offset++]=X*Ax.GetZ()+Y*Ay.GetZ()+Z*extrusion.GetZ();

		printf("insert Points: %f   %f   %f\n",array[offset-3],array[offset-2],array[offset-1]);
		
		}
	}
return offset;
}

/*****************************************************************************
Method:   DXFFile::GetNumCellSets()
Purpose:  Gets the number of cell sets in all the objects inserted
Params:   ---
State:    ---
Returns:  The number of cell sets in all the objects inserted
*****************************************************************************/
int DXFInsert::GetNumCellSets()
{
   int ncs = 0;
   //if (curLayer!=GetLayer())return 0;
   if(block)
	ncs=block->GetNumCellSets();
   printf("DXFInsert::GetNumCellSets %i \n",ncs);
   return ncs;
}

void DXFInsert::MoveToFirstCellSet()
{
   if(block)
      block->MoveToFirstCellSet();
}

char DXFInsert::MoreCellSets()
{
   if(block)
     return(block->MoreCellSets());
   return 0;
}

DXFCellSet *DXFInsert::GetNextCellSet()
{
   if(block)
     {
     DXFCellSet *cs = block->GetNextCellSet();
     
     if (firstPointIndex>0 && cs)
	 {
	    int *index = cs->GetCells();
	    // int *index1=index;
	    // printf("DXFInsert::GetNextCellSet index before FPI: %i\n",index[0]);
	   
	    // printf("NumVertices: %i index: %i\n",cs->GetNumVertices(),*index);	

	    for (int i = cs->GetNumVertices(); i > 0; i--, index++)
	    	{
	       *index += firstPointIndex;
	       //printf(" 1: %i ",*index);
	       }
	    //printf("DXFInsert::GetNextCellSet() index after FPI: %i\n",index1[0]);
	    //block->setOldPointIndex(firstPointIndex);

	 }
      /*if(!cs) 
         {
          printf("/t/tDXFInsert::GetNextCellSet set oldPointIndex\n");
          block->setOldPointIndex(firstPointIndex);
         //firstPointIndex = 0;
         } */
      printf("DXFInsert::GetNextCellSet end\n");   
      return cs;
      }
   return 0;
}

/*****************************************************************************
Method:   DXFInsert::GetColorNums()
Purpose:  Gets the color number of insert object read from the DXF file.  The
          color number corresponds, through some AutoCAD-specific mapping,
	  to a particular colour.
Params:   array   The array in which to put the color numbers.  This array
                  should be < DXFFile::GetNumPoints() > short integers long.
          offset  give the start point to begin with writing
State:    This function is intended to be used to assign the colour number
          of each object (each DXF object has a single colour number) as node
	  data corresponding to the nodes of that object.
Returns:  The number of short integers placed in the specified output array.
*****************************************************************************/
int DXFInsert::GetColorNums(short int *array,int offset)
{
if(block)
	return block->GetColorNums(array,offset);
return offset;
}

/*****************************************************************************
Method:   DXFInsert::GetNumText()
Purpose:  Counts the total number of annotations read from inserted block.
Params:   ---
State:    ---
Returns:  The total number of annotations read
          from inserted block.
*****************************************************************************/
int DXFInsert::GetNumText()
{
if(block)
	return block->GetNumText();
return 0;
}

/*****************************************************************************
Method:   DXFInsert::GetTextPoints()
Purpose:  Gets the coordinate values for every annotation read
          from block.
Params:   array   The array in which to put the coordinate values.  This
                  array should be 3 * < DXFFile::GetNumText() > floats
		  long.
	  offset  Point to start writing in array. 
State:    For a point whose index into "array" is i, its coordinates are
          placed as follows:

	     x --> array[3*i + 0]
	     y --> array[3*i + 1]
	     z --> array[3*i + 2]
	  
Returns:  The number of floats placed in the specified output array.
*****************************************************************************/
int DXFInsert::GetTextPoints(float *array, int offset)
{
int numPoints=0;

if(block)
	{
	firstPointIndex=offset/3;
	numPoints=GetNumText();
	int x=block->GetTextPoints(array,offset);
	// printf("DXFInsert GetTextPoints new offset= %i\n",x);
	for(int i=0;i<numPoints;i++)
		{
		float Tx=start.GetX()-block->GetBasePoint().GetX();
		float Ty=start.GetY()-block->GetBasePoint().GetY();
		float Tz=start.GetZ()-block->GetBasePoint().GetZ();
		float x=array[offset];
		float y=array[offset+1];
		float sinAngle=sin(rotationAngle);
		float cosAngle=cos(rotationAngle);
		// printf("x:%f y:%f\n",x,y);
		
 		array[offset++]=cosAngle*x*scaleFactorx-sinAngle*y*scaleFactory+Tx;
 		array[offset++]=sinAngle*x*scaleFactorx+cosAngle*y*scaleFactory+Ty;
 		array[offset++]=scaleFactorz*(array[offset])+Tz;
 				
		// printf("Points: %f %f %f\n",array[offset-3],array[offset-2],array[offset-1]);
		
		
		}
	}
return offset;
}

/*****************************************************************************
Method:   DXFInsert::MoveToFirstText()
Purpose:  Initialise a pointer to the first DXFText in the block.  
Params:   ---
*****************************************************************************/
void DXFInsert::MoveToFirstText()
{
if(block)
	block->MoveToFirstText();
}

/*****************************************************************************
Method:   DXFInsert::GetNextText()
Purpose:  Gets a pointer to the next DXFText in the block.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A pointer to the next text string
*****************************************************************************/
DXFText *DXFInsert::GetNextText()
{
// printf("DXFInsert::GetNextText\n");
if(block)
	return(block->GetNextText());
return 0;
}


/*****************************************************************************
Method:   DXFInsert::GetNextWholeText()
Purpose:  Gets a pointer to the next DXFText in the block.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A pointer to the next text string
*****************************************************************************/
structtext *DXFInsert::GetNextWholeText()
{
printf("DXFInsert::GetNextWholeText\n");
structtext text;
structtext *text1;//=(structtext *)malloc(sizeof(structtext));
if(block)
   {
   text1=block->GetNextWholeText();
   printf("DXFInsert::GetNextWholeText text1 \n");
   if(text1)
	{
	//text=dxftext->GetWholeText();
	printf("DXFInsert::GetNextWholeTextstring %s\n",text1->text);
	float Tx=start.GetX()-block->GetBasePoint().GetX();
	float Ty=start.GetY()-block->GetBasePoint().GetY();
	float Tz=start.GetZ()-block->GetBasePoint().GetZ();
	float x=text1->point.GetX();
	float y=text1->point.GetY();
	float sinAngle=sin(rotationAngle);
	float cosAngle=cos(rotationAngle);
	// printf("x:%f y:%f\n",x,y);
		
 	text1->point.SetX(cosAngle*x*scaleFactorx-sinAngle*y*scaleFactory+Tx);
 	text1->point.SetY(sinAngle*x*scaleFactorx+cosAngle*y*scaleFactory+Ty);
 	text1->point.SetZ(scaleFactorz*start.GetZ()+Tz);
	if(extrusion.GetX()-text1->extrusion.GetX()<=1.0/64.0 && extrusion.GetZ()-text1->extrusion.GetZ()<=1.0/64.0 && (extrusion.GetY()-text1->extrusion.GetY())<=1.0/64.0 &&
	extrusion.GetX()-text1->extrusion.GetX()>=-1.0/64.0 && extrusion.GetZ()-text1->extrusion.GetZ()>=-1.0/64.0 && (extrusion.GetY()-text1->extrusion.GetY())>=-1.0/64.0)
	   {
	   text1->angle+=rotationAngle;
	   text1->height*=scaleFactory;
	   text1->expansion*=scaleFactorx;
	   }
        else
           {
	   text1->angle=rotationAngle;
	   text1->expansion=scaleFactorx;
	   }
	if(text1->angle<-180)text1->angle=360+text1->angle;
	if(text1->angle>180)text1->angle=360-text1->angle;
	if(rotationAngle!=0)
	   {
	   if(extrusion.GetX()<=1.0/64.0 && extrusion.GetZ()<=1.0/64.0 && (extrusion.GetY()-1)<=1.0/64.0 &&
	   extrusion.GetX()>=-1.0/64.0 && extrusion.GetZ()>=-1.0/64.0 && (extrusion.GetY()-1)>=-1.0/64.0)
		text1->plane=1;
	   if(extrusion.GetY()<=1.0/64.0 && extrusion.GetZ()<=1.0/64.0 && (extrusion.GetX()-1)<=1.0/64.0 &&
	   extrusion.GetY()>=-1.0/64.0 && extrusion.GetZ()>=-1.0/64.0 && (extrusion.GetX()-1)>=-1.0/64.0)
		text1->plane=2;
	   if(extrusion.GetY()<=1.0/64.0 && extrusion.GetX()<=1.0/64.0 && (extrusion.GetZ()-1)<=1.0/64.0 &&
	   extrusion.GetY()>=-1.0/64.0 && extrusion.GetX()>=-1.0/64.0 && (extrusion.GetZ()-1)>=-1.0/64.0)
		text1->plane=0;
	   }
	printf("DXFInsert::GetNextWholeText end\n");
	//*text1=text;
	if (text1) return text1;
	}
   }
return 0;
}

void DXFInsert::SetOldPointIndex(int offset) 
{
// printf("\n\t\aDXFInsert::SetOldPointIndex offset:%i new OPI:%i\n",offset,firstPointIndex+offset);
if (block) 
	block->setOldPointIndex(firstPointIndex+offset);
}

void DXFInsert::Dump()
{
//    printf("   Insert\n");
//    printf("      Start = (%f, %f, %f)\n",
//	  start.GetX(), start.GetY(), start.GetZ());
//    printf("        End = (%f, %f, %f)\n", end.GetX(), end.GetY(), end.GetZ());
}

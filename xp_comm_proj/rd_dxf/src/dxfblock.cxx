/*****************************************************************************
DXFBlock.cxx -- test
******************************************************************************

Implementation of the DXFBlock class, which provides the basis for subclasses
to permit reading of various AutoCAD primitives.

******************************************************************************
*****************************************************************************/

#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dxfcir.hxx"
#include "dxfline.hxx"
#include "dxfface.hxx"
#include "dxfpline.hxx"
#include "dxfpoint.hxx"
#include "dxfray.hxx"
#include "dxfarc.hxx"
#include "dxfsolid.hxx"
#include "dxfbody.hxx"
#include "dxfellipse.hxx"
#include "dxfinsert.hxx"
#include "dxfleader.hxx"
#include "dxfmline.hxx"
#include "dxfdimension.hxx"
#include "dxftrace.hxx"
#include "dxfblock.hxx"
#include "dxfprim.hxx"
#include "dxftext.hxx"
#include "dxfxline.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"
#include "dxfinsert.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */

/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/


typedef DXFPrim *(*DXFBlockMakeFunction)(DXFGroup &group,CLIST <DXFLayer> *firstLayer);


/*******************  Private (Static) Global Variables  ********************/

static DXFBlockMakeFunction S_MakeFunctionList[] = {
   DXFCircle::Make,
   DXFLine::Make,
   DXFFace::Make,
   DXFPolyline::Make,
   DXFPoint::Make,
   DXFArc::Make,
   DXFRay::Make,
   DXFSolid::Make,
   DXFBody::Make,
   DXFEllipse::Make,
   DXFLeader::Make,
   DXFMLine::Make,
   DXFTrace::Make,
   DXFXLine::Make,
   0 };    // This marks the end of the array; don't delete it!


/*****************************************************************************
**********--------   Implementations of "DXFBlock" Methods   --------**********
*****************************************************************************/

//This needs the code which reads in the block name and then finds its description. It is then jsut follows on.
/*****************************************************************************
Method:   DXFBlock::Make()
Purpose:  This is the static "virtual constructor" for the DXFBlock class.
          This method revectors this "create object" request to the Make()
	  method of each subclass of DXFBlock of which it is aware.  The
	  actual instance is created by the first subclass whose Make()
	  function returns a non-NULL pointer.
	  This could have been implemented more elegantly using the
	  exemplars idiom, but this is less involved and not too onerous
	  to modify.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a DXFBlock object if the current object was not
          recognized, or
	  A pointer to a subclass of DXFBlock if it was.
*****************************************************************************/
DXFBlock *DXFBlock::Make(DXFGroup &group,CLIST <DXFLayer> *firstLayer)
{
int i;	 
DXFBlock *instance = 0;

   // To add a new supported object type, it must be a subclass of DXFBlock
   // and must implement a Make() function that will create an instance of
   // that class if the supplied group data is appropriate.  This new
   // implementation of Make() should be added to the S_MakeFunctionList[]
   // array defined as a global for this module.  Also, whatever needed
   // header files should be #included at the top of this file.
   //


instance= new DXFBlock(group,firstLayer);
return instance;
}


/*****************************************************************************
Method:   DXFBlock::DXFBlock()
Purpose:  A Constructor for the DXFBlock class.  Reads enough of the specified
          file to advance the file pointer past the current DXF primitive,
	  but does not retain any information about that primitive.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
DXFBlock::DXFBlock(DXFGroup &group,CLIST <DXFLayer> *firstLayer) : isValid(0), errorDesc(),
        numCellSets(0),color(DXF_NO_COLOR),
        Blockname(),basepoint(0.0,0.0,0.0),flagOPI(0)
{


char moreInFile=1;
moreInFile=group.Read();
while(moreInFile)
	{
	if(group.Code()==0)
	    {
	    // SECTION and ENDSEC maybe impossible
    	    if(!(group.DataCmp("SECTION") || group.DataCmp("ENDSEC") || 
    	         group.DataCmp("ENDBLK") || group.DataCmp("EOF")))
		{
		
		//if (debug) printf("DXFBlock constructor Data: %s\n",group.Data());
		
		int isText=0;
		if(group.DataCmp("TEXT"))
			isText=1;
		char ret=complexEntity(group,firstLayer,&entityList,
	     	  			  &textList,&insertList);
		   if (ret==0) 
		   {
		       isValid=0;				//isValid from Block
		       errorDesc="Unreadable DXF file";
		   }
		   if (ret==2)
		   {
			DXFPrim *newObj=DXFPrim::Make(group,firstLayer);
			if(newObj->IsValid())
				{
			
				//int a=newObj->GetNumPoints();
				//if (debug) printf("newObj Num of Points: %i\n",a);
						
				numCellSets=numCellSets+newObj->GetNumCellSets();
				if(isText)
					{
					textList.InsertBehind(newObj);
					//if(textHead) if (debug) printf("DXFBlock::DXFBlock textHead exist\n");
					}
 				else
					entityList.InsertBehind(newObj);
				}
			else
				{
				isValid=0;
				errorDesc="Unreadable DXF file";
				break;
				}
			
                    }
		}
	    if(group.DataCmp("ENDBLK")) 
 	   	{
 	   	//if (debug) printf("DXFBlock constructor1 %s\n",Blockname.Get());
    		isValid=1;
		break;
		}
	    if(group.DataCmp("EOF"))
	       moreInFile=0;
       	    }
       	else
       	  {
       	  switch(group.Code())
       	    {
       	    case 2:  Blockname=group.Data();
       	    		if (debug) printf("Blockname %s\n",Blockname.Get());
       	    	     break;
       	    case 3:  Blockname=group.Data();
       	    	     break;
       	    case 10: basepoint.SetX(group.DataAsFloat());
       	             break;
       	    case 20: basepoint.SetY(group.DataAsFloat());
       	             break;
       	    case 30: basepoint.SetZ(group.DataAsFloat());
       	             break;
       	    }
       	   moreInFile=group.Read();
       	   }
	} 
      //isValid = group.Code() == 0;

}


/*****************************************************************************
Method:   DXFBlock::~DXFBlock()
Purpose:  Destructor for the DXFBlock class.
Params:   ---
State:    Destroys the cell set list.
*****************************************************************************/
DXFBlock::~DXFBlock()
{
deleteLists();
}


/*****************************************************************************
Method:   DXFBlock::DXFBlock()
Purpose:  Copy constructor for the DXFBlock class.
Params:   prim   The DXFBlock object to copy.
State:    ---
*****************************************************************************/
DXFBlock::DXFBlock(DXFBlock &prim) : isValid(prim.isValid),
      errorDesc(prim.errorDesc),
      numCellSets(prim.numCellSets), color(prim.color),
      Blockname(prim.Blockname),basepoint(prim.basepoint),
      flagOPI(prim.flagOPI)
{
   DXFPrim *cs = prim.entityList.GetFirst();
   DXFInsert *ins = prim.insertList.GetFirst();

   while (cs)
   {
      entityList.InsertBehind(cs);
      cs = prim.entityList.GetNext();
   }
   
   cs=prim.textList.GetFirst();
   
   while (cs)
   {
      textList.InsertBehind(cs);
      cs = prim.textList.GetNext();
   }
   
   while (ins)
   {
     insertList.InsertBehind(ins);
     ins=prim.insertList.GetNext();
   }
}


/*****************************************************************************
Method:   DXFBlock::ReadCommon()
Purpose:  Reads common (i.e. applicable to all or most AutoCAD objects)
          attributes.
Params:   group   The DXFGroup object being used to read the DXF file.
                  This function will not change this object, nor advance
		  the DXF file pointer.
State:    This function should be called by each derived class after that
          class has determined that the current group code is uninteresting.
Returns:  ---
*****************************************************************************/
void DXFBlock::ReadCommon(const DXFGroup &group)
{
   switch (group.Code())
   {
     case -1:   // Entity name
      break;

     case 8:    // Layer name
      break;

     case 62:   // Color number
      color = group.DataAsInt();
      break;

     default:
      break;
   }
}

/*****************************************************************************
Mehtod:		DXFBlock::GetNumPoints()
Purpose:	Counts the total number of points (verices) in all objects read 
		from the block.
Params:		---
State:		---
Returns:	The total number of ponts (vetices) in all the objects read
		from block.
******************************************************************************/
int DXFBlock::GetNumPoints(CLIST <DXFBlock> *blocklist)
{
int numPoints=0;
DXFPrim * ent= entityList.GetFirst();
DXFInsert *ins = insertList.GetFirst();
printf("\nDXFBlock::GetNumPoints before ent\n");
while (ent)
	{
        printf("DXFBlock::GetNumPoints before ent\n");
	numPoints+=ent->GetNumPoints();
	ent=entityList.GetNext();
	}
	
while (ins)
	{
	if(blocklist)numPoints+=ins->GetNumPoints(blocklist);
        else numPoints+=ins->GetNumPoints();
	//if (debug) printf("points in ins DXFBlock insert: %i\n",numPoints);
	ins=insertList.GetNext();
	}
	
//if (debug) printf("points  after ins DXFBlock: %i\n",numPoints);
return numPoints;
}

/*****************************************************************************
Method:		DXFBlock::GetPoints()
Purpose:	Get the coordinate values for every point in every object read
		fromn the DXF file.
Params:		array	The array in which to put the xoordinte values. This
			array should be 3 * < DXFBlock::GetNumPoints() > floats
			long.
State:		For a poin whose index into "array" is i, its coordinates are
		placed as follows:
		
		  x --> array[3*i + 0]
		  y --> array[3*i + 1]
		  z --> array[3*i + 2]
		  
Returns:	The number of floats placed in the specified output array.
*****************************************************************************/
int DXFBlock::GetPoints(float *array,int i)
{
DXFPrim *ent=entityList.GetFirst();
DXFInsert *ins=insertList.GetFirst();

int num=i;

while (ent)
	{
	ent->SetFirstPointIndex((i-num)/3);
	//printf("DXFBlock::GetPoints FPI entity:%i\n",(i-num)/3);
	i=ent->GetPoints(array,i);
	//printf("DXFBlock::GetPoints %f %f %f",array[i-3],array[i-2],array[i-1]);
	ent=entityList.GetNext();
	//printf("DXFBlock::GetPoints  i: %i \n",i);
	}

printf("DXFBlock::GetPoints after ent\n");


while (ins)
	{
	int a=(i-num)/3;
	i=ins->GetPoints(array,i);
	//printf("DXFBlock::GetPoints in ins offset: %i\n",i);
	ins->SetFirstPointIndex(a);
	//printf("DXFBlock::GetPoints FPI insert:%i\n",a);
	ins=insertList.GetNext();
	}
printf("DXFBlock::GetPoints offset: %i\n",i);
	
return i;
}

/*****************************************************************************
Method:   DXFBlock::GetNumCellSets()
Purpose:  Gets the number of cell sets in all the objects inserted
Params:   ---
State:    ---
Returns:  The number of cell sets in all the objects inserted
*****************************************************************************/
int DXFBlock::GetNumCellSets()
{
int a=0;
DXFPrim *ent=entityList.GetFirst();
DXFInsert *ins=insertList.GetFirst();
while (ent)
  {
  printf("DXFBlock::GetNumCellSets in ent\n");
  a=a+ent->GetNumCellSets();
  ent=entityList.GetNext();
  }

while (ins)
  {
  printf("DXFBlock::GetNumCellSets in ins\n");
  a=a+ins->GetNumCellSets();
  ins=insertList.GetNext();
  }
printf("DXFBlock::GetNumCellSets %i %i\n",numCellSets,a);
return (a);
}

/*****************************************************************************
Method:   DXFBlock::MoveToFirstCellSet()
Purpose:  Sets an internal DXFCellSet pointer to point to the first cell
          set in the first object in the internal list of objects read from
	  the DXF file.
Params:   ---
State:    This function must be called before the GetNextCellSet() function
          can be used.
Returns:  ---
*****************************************************************************/
void DXFBlock::MoveToFirstCellSet()
{
   curObj = entityList.GetFirst();
   curInsert = insertList.GetFirst();
  
   if (curObj)
      curObj->MoveToFirstCellSet();
   if (curInsert)
      curInsert->MoveToFirstCellSet();
   //if (debug) printf("DXFBlock::MoveToFirstCellSet\n");
}

/*****************************************************************************
Method:   DXFBlock::MoreCellSets()
Purpose:  Determines whether there are more cell sets to be read from the
          object list.
Params:   ---
State:    This function should not be called until MoveToFirstCellSet()
          has been called at least once.
	* This function assumes that every object will have at least one
	  cell set, and will not work properly otherwise.
Returns:  True   if the next call to the GetNextCellSet() method will
                 succeed, or
	  False  if it won't, because there are no more cell sets to find.
*****************************************************************************/
char DXFBlock::MoreCellSets()
{
   char more = 0;

   if (curObj)
      if (entityList.Next() || curObj->MoreCellSets())
      	   {
           return 1;
           }
   if (curInsert)
      if (insertList.Next() || curInsert->MoreCellSets())
           return 2;
   return more;
}

/*****************************************************************************
Method:   DXFBlock::GetNextCellSet()
Purpose:  Gets a pointer to the next DXFCellSet in the block.  Each object
          will have one or more cell set; this function can be used to
	  traverse every cell set in all the objects.
Params:   ---
State:    This function should not be called until MoveToFirstCellSet()
          has been called at least once.
	* This function adds offsets to the vertex indexes in each cell set
	  so that the proper coordinates are used.  It also sets the
	  firstPointIndex attribute of the DXFPrimListEl containing the
	  cell set to 0, so that if this function is called a second time,
	  the offsets will still be valid.
Returns:  A pointer to the next cell set associated with the current
          object, or
	  A pointer to the first sell set associated with the next object,
	  if the current object has no (more) cell sets, or
	  NULL if the current object is the last object, and the current
	  cell set is the last cell set for that object.
*****************************************************************************/
DXFCellSet *DXFBlock::GetNextCellSet()
{
   DXFCellSet *cs = 0;
   printf("DXFBlock::GetNextCellSet\n");
   while ((curObj || curInsert) && (!cs))
   {
     if (curObj)
     {
       if (curObj->MoreCellSets())
       {
	 cs = curObj->GetNextCellSet();
	 int *index = cs->GetCells();

	//if (debug) printf("DXFBlock::GetNextCellSet index before FPI: %i\n",index[0]);

	 if (curObj->GetFirstPointIndex() > 0)
	 {

	    for (int i = cs->GetNumVertices(); i > 0; i--, index++)
	       {
	       *index += curObj->GetFirstPointIndex();
	       //printf("1: %i ",*(index));
	       }
	    
	    curObj->SetFirstPointIndex(0);
	 }
	 else
	 {
	    curObj->SetFirstPointIndex(0);
	    for (int i = cs->GetNumVertices(); i > 0 && (flagOPI); i--, index++)
	    	{
	        *index -= *OldFirstPointOffset.GetFirst();
	        //printf("2: %i OldFirstPointOffset: %i",*(index),OldFirstPointOffset);
	        }
	    //printf("sub ofpi %i ",*OldFirstPointOffset.GetFirst());
            if(flagOPI) {printf("next removeItem\n");
				OldFirstPointOffset.RemoveItem();}

//have to delte Item
           
	 }
	 //printf("DXFBlock::GetNextCellSet index after FPI: %i\n",index1[0]);
       }
       else
       {
	 curObj = entityList.GetNext();

	 if (curObj)
	    curObj->MoveToFirstCellSet();
       }
     }
     else
     {
     //printf("DXFBlock::GetNextCellSet handle with inserts\n");
       char a=curInsert->MoreCellSets();
       if(a)
          {
          curInsert->SetOldPointIndex(*OldFirstPointOffset.GetLast());

          cs=curInsert->GetNextCellSet();
	  
	  OldFirstPointOffset.GetFirst();
	  if(flagOPI) OldFirstPointOffset.RemoveItem();

//have to delte item

	  if(!curInsert->MoreCellSets())
	        curInsert->SetFlag();
	 
	 }
       else
         {
         //printf("DXFBlock::GetNextCellSet set ofpi\n");
         //curInsert->SetOldPointIndex(OldFirstPointOffset);
         //curInsert->SetFirstPointIndex(0);
         curInsert=insertList.GetNext();
         
         if(curInsert)
            curInsert->MoveToFirstCellSet();
         }
     }
   }
   printf("DXFBlock::GetNextCellSet end\n");
   return cs;
}

/*****************************************************************************
Method:   DXFBlock::GetColorNums()
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
int DXFBlock::GetColorNums(short int *array, int offset)
{
   int j;
   short int colorNum;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();

   while (ent)
   {
//      if (debug) printf("DXFFile: Adding color numbers node data at index %d\n", i);

      colorNum = (short int)ent->GetColorNumber();
      //if (debug) printf("DXFBlock::GetColorNums numPoints: %i offset: %i colour: %hi\n",ent->object->GetNumPoints(),offset),colorNum;
      for (j = ent->GetNumPoints(); j > 0; j--){
	 array[offset++] = colorNum;
	 //if (debug) printf("DXFBlock::GetColorNums colour: %i\n",array[offset-1]);
	  }
      ent = entityList.GetNext();
   }
   
   while (ins)
   {
   	offset=ins->GetColorNums(array,offset);
   	ins=insertList.GetNext();
   }
   
   return offset;
}

/*****************************************************************************
Method:   DXFBlock::GetNumText()
Purpose:  Counts the total number of annotations read from block.
Params:   ---
State:    ---
Returns:  The total number of annotations read
          from block.
******************************************************************************/
int DXFBlock::GetNumText()
{
int numText=0;
DXFPrim * ent= textList.GetFirst();
DXFInsert *ins=insertList.GetFirst();

while (ent)
	{
	numText++;
	ent=textList.GetNext();
	//printf("DXFBlock::GetNumText %i\n",numText);
	}

while (ins)
	{
	numText+=ins->GetNumText();
	ins=insertList.GetNext();
	}
printf("DXFBlock::GetNumText %i\n",numText);
return numText;
}

/*****************************************************************************
Method:   DXFBlock::GetTextPoints()
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
int DXFBlock::GetTextPoints(float *array, int offset)
{
   int i = offset;
   DXFPrim *t = textList.GetFirst();
   DXFInsert *ins=insertList.GetFirst();

   while (t)
   {
      t->SetFirstPointIndex((offset-i) / 3);
      offset = t->GetPoints(array, offset);

      t = textList.GetNext();
   }
   
   while (ins)
   {
       int a=(offset-i)/3;
       offset=ins->GetTextPoints(array,offset);
       ins->SetFirstPointIndex(a);
       ins=insertList.GetNext();
   }

   return offset;
}   

/*****************************************************************************
Method:   DXFBlock::MoveToFirstText()
Purpose:  Initialise a pointer to the first DXFText in the block.  
Params:   ---
*****************************************************************************/
void DXFBlock::MoveToFirstText()
{
curText=textList.GetFirst();
curInsert=insertList.GetFirst();
if(curInsert)
     curInsert->MoveToFirstText();
}

/*****************************************************************************
Method:   DXFBlock::GetNextText()
Purpose:  Gets a pointer to the next DXFText in the block.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A pointer to the next text string
*****************************************************************************/
DXFText *DXFBlock::GetNextText()
{
//if (debug) printf("DXFBlock::GetNextText\n");
DXFText *text=0;
if(curText)
    {
    text=(DXFText *)curText;
    curText=textList.GetNext();
    char * textpointer=text->GetText();
    //(textpointer) ? if (debug) printf("DXFBlock::GetNextText %s\n",textpointer) : if (debug) printf("DXFBlock::GetNextText textpointer not exsitst\n");
    //(text) ? if (debug) printf("DXFBlock::GetNextText text exsitst\n") : if (debug) printf("DXFBlock::GetNextText text not exsitst\n");
    
    }
else
    {
    while(!text && curInsert)
       {
       text=curInsert->GetNextText();
       if (!text)
          {
          curInsert=insertList.GetNext();
          curInsert->MoveToFirstText();
          }
        }
     }
    
return text;
} 

/*****************************************************************************
Method:   DXFBlock::GetNextWholeText()
Purpose:  Gets a pointer to the next DXFText in the block.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A pointer to the next text string
*****************************************************************************/
structtext *DXFBlock::GetNextWholeText()
{
printf("DXFBlock::GetNextWholeText\n");
DXFText *text=0;
structtext stext;
structtext *stext1=(structtext *)malloc(sizeof(structtext));
if(curText)
    {
    text=(DXFText *)curText;
    stext=text->GetWholeText();
    printf("DXFBlock::GetNextWholeText string %s \n",stext.text);
    *stext1=stext;
    curText=textList.GetNext();
    //char * textpointer=text->GetText();
    //(textpointer) ? if (debug) printf("DXFBlock::GetNextText %s\n",textpointer) : if (debug) printf("DXFBlock::GetNextText textpointer not exsitst\n");
    //(text) ? if (debug) printf("DXFBlock::GetNextText text exsitst\n") : if (debug) printf("DXFBlock::GetNextText text not exsitst\n");
    
    }
else
    {
    while(!text && (strcmp(stext1->text,"")==0) && curInsert)
       {
       stext1=curInsert->GetNextWholeText();
       //printf("DXFBlock::GetNextWholeText after curInsert->GetNextWholeText\n");
       if (!stext1)
          {
          curInsert=insertList.GetNext();
          if(curInsert) curInsert->MoveToFirstText();
          }
        }
     }
//printf("DXFBlock::GetNextWholeText end\n");
return stext1;
} 


void DXFBlock::deleteLists()
{
}





/*****************************************************************************
Method:   DXFBlock::AddCellSet()
Purpose:  Adds a cell set (a DXFCellSet object) to the linked list of cell
          sets.
Params:   nverts   The number of vertices per cell.
          ncells   The number of cells in the cell set.
	  flags    Optional flags, which can be an OR-combination of the
	           following masks:

		     DXF_CLOSED_POLYGON   indicates that the new cell set
		                          describes a closed polygon.

		     DXF_CONVEX_POLYGON   indicates that the new cell set
                                          describes a convex polygon.

		 * The default is 0 (no flags set)
		 * These flags apply only to polylines; not meshes,
		   3d faces, etc.
State:    The DXFCellSet object is created, but the vertices making up the
          node connect list still have to be supplied.
Returns:  True   if the new DXFCellSet object was successfully added,
          False  otherwise, in which case the current cell set list is
	         unchanged.
*****************************************************************************/
/*char DXFBlock::AddCellSet(int nverts, int ncells, char flags)
{
   char ok = 0;
   DXFCellSet *cellSet = new DXFCellSet(nverts, ncells, flags);

   if (cellSet && cellSet->IsValid())
   {
      if (cellSetsTail)
	 cellSetsTail->next = cellSet;
      else
	 cellSetsHead = cellSet;

      cellSetsTail = cellSet;

      ++numCellSets;

      ok = 1;
   }

   return ok;
}*/



/**********************************************************************************
Method:		DXFBlock::AddToList()
Purpose:	This private method adds the specified new DXFPrim item to the
		specified list.
Params:		head, tail	The head ant tail of the list to which to add the
				specified new item. If these are both NULL, they
				will be set to point to teh new item. Otherwise,
				tail will be set to point to the new item,
				head will remain unchanged.
		newObj		The new DXFPrim object to add.
State:		---
Returns:	---
************************************************************************************/
/*void DXFBlock::AddToList(DXFPrimListEl *&head, DXFPrimListEl *&tail,
			DXFPrim *newObj)
{
DXFPrimListEl *newEl = new DXFPrimListEl(newObj);

if (newEl)
	{
	if (tail)
		tail->next=newEl;
	else
		head=newEl;
		
	tail=newEl;
	}
} */



char complexEntity(DXFGroup &group,
	          CLIST <DXFLayer> *layerList,
		  CLIST <DXFPrim> *entityList,
	     	  CLIST <DXFPrim> *textList,
		  CLIST <DXFInsert> *insertList)
{
printf("complexEntity\n");

  if(group.DataCmp("INSERT"))
  {
	DXFInsert *newIns=DXFInsert::Make(group,layerList);
	if (newIns->IsValid())
		insertList->InsertBehind(newIns);
	else
		return 0;
	return 1;
  }

  if(group.DataCmp("DIMENSION"))
  {
	DXFDimension *x=new DXFDimension(group,layerList,entityList,textList);
	return 1;
  }

  if(group.DataCmp("MTEXT"))
  {
	
        printf("complexEntity in MTEXT\n");

	DXFMText *mtextnew=new DXFMText(group, layerList,textList);
	return 1;
  }

return 2;			//it was not an entity from here
}		

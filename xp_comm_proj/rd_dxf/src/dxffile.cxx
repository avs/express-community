/*****************************************************************************

******************************************************************************

Implementation of the DXFfile class, which reads and parses DXF files.
Amended to include definition of blocks.
******************************************************************************
*****************************************************************************/

#include <stdio.h>

#include "dxfprim.hxx"
#include "dxffile.hxx"
#include "dxftext.hxx"
#include "dxfmtext.hxx"
#include "dxfmline.hxx"
#include "dxfblock.hxx"
#include "dxfinsert.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"
#include "dxfdimension.hxx"
#include "dxfheader.hxx"


/*********************  Macros and Manifest Constants  **********************/



/************************  Public Global Variables  *************************/



/************************  Private Type Definitions  ************************/

typedef enum
{
   DXF_OTHER_SECTION = 0,
   DXF_BLOCKS_SECTION,
   DXF_ENTITIES_SECTION,
   DXF_TABLES_SECTION, //blocks are forward defined here
   DXF_HEADER_SECTION
} DXFSectionId;


/*******************  Private (Static) Global Variables  ********************/

/* <none> */



/*****************************************************************************
**********--------   Implementations of "DXFFile" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFFile::DXFFile()
Purpose:  A constructor for the DXFFile class that reads and parses the DXF
          file with the specified name.
Params:   fileName         The name of the DXF file to read.
          reverseNormals   A flag indicating that all filled faces
	                   (including filled polylines, faces, and meshes)
			   should be connected in such a way that their
			   surface normals will point in the opposite
			   direction from standard.  This is useful because
			   AutoCAD doesn't seem to be consistent in the
			   order in which it writes points to the DXF file,
			   and therefore there is no way to know how to
			   traverse the points to ensure that the surface
			   normal points to the outside of the object.
State:    If successful, the contents of the specified DXF file is read
          into an internal representation.
	  If IsValid() returns True, then this instance can be used.
	  If it returns False, then this instance should be destroyed
	  immediately after (optionally) obtaining error information from
	  GetErrorDesc().
*****************************************************************************/
DXFFile::DXFFile(const char *fileName, int reverseNormals) : isValid(0),
          errorDesc()

{
  if (fileName)
  {
    FILE *fp = fopen(fileName, "rt");
    int count = 0;
    if (fp)
    {
      char moreInFile = 1;
      DXFSectionId section = DXF_OTHER_SECTION;
      DXFGroup group(fp, reverseNormals != 0);

      isValid = 1;

      moreInFile = group.Read();

      while (moreInFile)
      {
	 if ((section == DXF_BLOCKS_SECTION) ||
	     (section == DXF_ENTITIES_SECTION) || 
	     (section == DXF_TABLES_SECTION))
	 {
	    if (group.Code() == 0)
	    {
	       if (group.DataCmp("ENDSEC"))
	       {
		   section = DXF_OTHER_SECTION;
		   moreInFile = group.Read();
	       }
	       else
	       {
		   int isText = 0;
		   if(group.DataCmp("TEXT"))
		   isText = 1;
/*  --- new --- */

		   char ret=complexEntity(group,&layerList,&entityList,
	     	  			  &textList,&blockList,&insertList);
		   if (ret==0) 
		   {
		       isValid=0;				//isValid from Block
		       errorDesc="Unreadable DXF file";
		   }
		   if (ret==2)
		   {
		       DXFPrim *newObj = DXFPrim::Make(group, &layerList);
		       if (newObj->IsValid())
		       { 
			   switch (section) 
		           {
			       case DXF_BLOCKS_SECTION:
			          break;

			       case DXF_ENTITIES_SECTION:
				  if(isText)
				  {
				      textList.InsertBehind(newObj);
				  }
				  else
			              	entityList.InsertBehind(newObj);
			          break;

		               case DXF_TABLES_SECTION:
			          tableList.InsertBehind(newObj);
			          if(newObj->HasLayer())
				  {
				      layerList.InsertBehind(newObj->GetLayer());
			          }
			           	//call code to save description
			          break;

		               default:
			          entityList.InsertBehind(newObj); 
			          break;
		            } 			//end switch
		       }
			  
		       else		        //newObj is not valid
		       {
			    isValid = 0;
			    errorDesc = "Unreadable DXF file";
			    break;
		       }

		   }			//end if(ret==2)
   	       }			//end else if (group.DataCmp("ENDSEC"))
	    }				//end if (group.Code() == 0)
	    
	    else  // the group code wasn't 0, but should have been
	    {
	       isValid = 0;
	       errorDesc = "Unreadable DXF file";
	       break;
	    }
	 }				//end if(section== ...
         else if (section == DXF_HEADER_SECTION) 
	 {
	    if (group.Code() == 0)
	    {
               if(group.DataCmp("ENDSEC"))
	       {
                  section = DXF_OTHER_SECTION;
                  moreInFile = group.Read();
	       }
            }
            else
               globalVar=DXFHeader::Make(group);
	 }
         else    // we're not in the ENTITIES or BLOCKS section
	 {
	      
	    if (group.Code() == 0)            // the entity group code
	    if (group.DataCmp("SECTION"))  // if this is a new section
            if (group.Read())           // read its name (next grp)
	    if (group.Code() == 2)   // section name group code
	    {
		if (group.DataCmp("HEADER"))
		{
		   section = DXF_HEADER_SECTION;
		}
	        else if (group.DataCmp("ENTITIES"))
		{
	           section = DXF_ENTITIES_SECTION;
	        }
	        else if (group.DataCmp("BLOCKS"))
	        {
		   section = DXF_BLOCKS_SECTION;
	        }
	        else if (group.DataCmp("TABLES"))
	        {
		   section = DXF_TABLES_SECTION;
	        }
	    }

	    moreInFile = group.Read();
	}
      }				//end while more in file

      fclose(fp);
    }
    else
      errorDesc = "Unable to open DXF file";
   }
   else
     errorDesc = "No DXF file name specified";
}



/*****************************************************************************
Method:   DXFFile::GetNumPoints()
Purpose:  Counts the total number of points (vertices) in all objects read
          from the DXF file.
Params:   ---
State:    ---
Returns:  The total number of points (vertices) in all the objects read
          from the DXF file.
*****************************************************************************/
int DXFFile::GetNumPoints()
{
   int numPoints = 0;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();

   while (ent)
   {
      numPoints += ent->GetNumPoints();
      ent = entityList.GetNext();
      //printf("DXFFile GetNumPoints numPoints: %i\n",numPoints);
   }


   while (ins)
   {
   	numPoints += ins->GetNumPoints(&blockList,curLayer);
   	//printf("DXFFile::GetNumPoints %i\n",ins->GetNumPoints(&blockList));
   	ins = insertList.GetNext();
   }
   if (debug) printf("dxffile GetNumPoints insertPoints: %i\n",numPoints);
   return numPoints;
}


/*****************************************************************************
Method:   DXFFile::GetNumPoints()
Purpose:  Counts the total number of points (vertices) in all objects read
          from the DXF file.
Params:   layernumber  actual layer in layerlist;
State:    ---
Returns:  The total number of points (vertices) in all the objects read
          from the DXF file.
*****************************************************************************/
int DXFFile::GetNumPoints(int layernumber)
{
   int numPoints = 0;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();
   DXFLayer *layer;

   curLayer = layerList.GetIndexed(layernumber);
   while (ent)
   {
      if(curLayer==ent->GetLayer())
      {
         printf("DXFFile GetNumPoints before counting entity\n");
         numPoints += ent->GetNumPoints();
         printf("DXFFile GetNumPoints numPoints: %i\n",numPoints);
      }
      ent = entityList.GetNext(); 
   }


   while (ins)
   {
      if(curLayer==ins->GetLayer())
      {
   	numPoints += ins->GetNumPoints(&blockList);
   	printf("DXFFile::GetNumPoints %i\n",numPoints);
      }
      ins = insertList.GetNext();
   }
   printf("dxffile GetNumPoints insertPoints: %i\n",numPoints);
   return numPoints;
}
/*****************************************************************************
Method:   DXFFile::GetPoints()
Purpose:  Gets the coordinate values for every point in every object read
          from the DXF file.
Params:   array   The array in which to put the coordinate values.  This
                  array should be 3 * < DXFFile::GetNumPoints() > floats
		  long.
State:    For a point whose index into "array" is i, its coordinates are
          placed as follows:

	     x --> array[3*i + 0]
	     y --> array[3*i + 1]
	     z --> array[3*i + 2]
	  
Returns:  The number of floats placed in the specified output array.
*****************************************************************************/
int DXFFile::GetPoints(float *array)
{
   int i = 0;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();   

   while (ent)
   {
      printf("DXFFile: Adding vertices starting at index %d\n", i);

      if(curLayer==ent->GetLayer())
      {
      	ent->SetFirstPointIndex (i / 3);
      	i = ent->GetPoints(array, i);
      }
      ent = entityList.GetNext();
   }
   printf("DXFFile GetPoints offset: %i\n",i);
  
   while (ins)
   {
      if(curLayer==ins->GetLayer())
      {
       i=ins->GetPoints(array,i);
       printf("DXFFile::GetPoints offset: %i\n",i);
      }
      ins=insertList.GetNext();
   }
   

   return i;
}
/*****************************************************************************
Method:   DXFFile::GetColorNums()
Purpose:  Gets the color number of each object read from the DXF file.  The
          color number corresponds, through some AutoCAD-specific mapping,
	  to a particular color.
Params:   array   The array in which to put the color numbers.  This array
                  should be < DXFFile::GetNumPoints() > short integers long.
State:    This function is intended to be used to assign the color number
          of each object (each DXF object has a single color number) as node
	  data corresponding to the nodes of that object.
Returns:  The number of short integers placed in the specified output array.
*****************************************************************************/
int DXFFile::GetColorNums(short int *array)
{
   int i = 0;
   int j;
   short int colorNum;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();

   while (ent)
   {
      if(curLayer==ent->GetLayer())
      {
//      printf("DXFFile: Adding color numbers node data at index %d\n", i);

        colorNum = (short int)ent->GetColorNumber();
 //     printf("DXFFile::GetColorNums %i\n",colorNum);
        for (j = ent->GetNumPoints(); j > 0; j--){
	array[i++] = colorNum;
        }
      }
      ent = entityList.GetNext();
   }

   while (ins)
   {
      if(curLayer==ins->GetLayer())
         i=ins->GetColorNums(array,i);
      ins=insertList.GetNext();
   }

   return i;
}


/*****************************************************************************
Method:   DXFFile::GetNumCellSets()
Purpose:  Gets the total number of cell sets in all the objects read from
          the DXF file.
Params:   ---
State:    ---
Returns:  The total number of cell sets in all the objects read from the
          DXF file.
*****************************************************************************/
int DXFFile::GetNumCellSets()
{
   int ncs = 0;
   DXFPrim *ent = entityList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();

   while (ent)
   {
      printf("DXFFile GetNumCellSets before getnumcellsets\n");
      if(curLayer==ent->GetLayer())
         ncs += ent->GetNumCellSets();
      ent = entityList.GetNext();
   }

   printf("DXFFile GetNumCellSets entity cells: %i\n",ncs);

   while (ins)
   {
      if(curLayer==ins->GetLayer())
          ncs+= ins->GetNumCellSets();
      ins = insertList.GetNext();
   }
   printf("DXFFile GetNumCellSets insert cells: %i\n",ncs);
   return ncs;
   
   
}


/*****************************************************************************
Method:   DXFFile::GetNumLayers()
Purpose:  Gets the total number of layers from
          the DXF file.
Params:   ---
State:    ---
Returns:  The total number of layers from the
          DXF file.
*****************************************************************************/
int DXFFile::GetNumLayers()
{
    int counter=0;
    for(DXFLayer *countlayer=layerList.GetFirst(); countlayer;
        counter++,countlayer=layerList.GetNext())
    printf("DXFFile::GetNumLayers Layer: %s\n",countlayer->GetName());
   
return counter;
}

/*****************************************************************************
Method:   DXFFile::GetLayername()
Purpose:  Return the name of the i'th layer
Params:   int i
State:    ---
Returns:  Name of i'th layer from list
*****************************************************************************/
char * DXFFile::GetLayername(int i)
{
   return layerList.GetIndexed(i)->GetName();
}

/*****************************************************************************
Method:   DXFFile::MoveToFirstCellSet()
Purpose:  Sets an internal DXFCellSet pointer to point to the first cell
          set in the first object in the internal list of objects read from
	  the DXF file.
Params:   ---
State:    This function must be called before the GetNextCellSet() function
          can be used.
Returns:  ---
*****************************************************************************/
void DXFFile::MoveToFirstCellSet()
{
   for(curObj = entityList.GetFirst();curObj && !(curLayer==curObj->GetLayer());
       curObj = entityList.GetNext());
   for(curInsert = insertList.GetFirst();curInsert && !(curLayer==curInsert->GetLayer());
       curInsert = insertList.GetNext());

   if (curObj)
      curObj->MoveToFirstCellSet();
      
   if (curInsert)
   {
      printf("DXFFile::MoveToFirstCellSet there is curInsert\n");
      curInsert->MoveToFirstCellSet();
   }
}


/*****************************************************************************
Method:   DXFFile::MoreCellSets()
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
char DXFFile::MoreCellSets()
{
   char more = 0;
 
   printf("DXFFile::MoreCellSets\n");
   if (curObj)
   {
      if (curObj->MoreCellSets())
         return 1;
      else
      {  
         curObj->SetFirstPointIndex(0);		//test !!
         for(curObj = entityList.GetNext();
             curObj && !(curLayer==curObj->GetLayer());
             curObj = entityList.GetNext());
         if(curObj) 
         {
	    curObj->MoveToFirstCellSet();
            return 1;
         }  
      }
   }
   if (curInsert)
   {
      printf("DXFFile::MoreCellSets there is curInsert\n");
      if (curInsert->MoreCellSets())
      {
         more = 1;
         printf("DXFFile::MoreCellSets more inertcellsets 1\n");
      }
      else
      {
	  curInsert->SetFlag();
	  for(curInsert = insertList.GetNext();
              curInsert && !(curLayer==curInsert->GetLayer());
              curInsert = insertList.GetNext());
	  if (curInsert)
          {
	     curInsert->MoveToFirstCellSet();
	     printf("DXFFile::MoreCellSets more inertcellsets 2\n");
             more=1;
	  }
      }
   }

  // printf("In DXFFile::MoreCellSets():  more = %s\n", more ? "YES" : "NO");

   return more;
}


/*****************************************************************************
Method:   DXFFile::GetNextCellSet()
Purpose:  Gets a pointer to the next DXFCellSet in the file.  Each object
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
DXFCellSet *DXFFile::GetNextCellSet()
{
   DXFCellSet *cs = 0;

   while ((curObj || curInsert) && (!cs))
   {
      if(curObj)
      {
         if (curObj->MoreCellSets())
         {
	 	cs = curObj->GetNextCellSet();

	 	if (curObj->GetFirstPointIndex() > 0)
	 	{
	    	   int *index = cs->GetCells();

	    	   for (int i = cs->GetNumVertices(); i > 0; i--, index++)
	    	   	{
	       		*index += curObj->GetFirstPointIndex();
	       		//printf("DXFFile::GetNextCellSet%i ",*index);
	       		}

	    	   //curObj->SetFirstPointIndex(0);
	 	}
          }
       	}
	else
	{
	  if (curInsert->MoreCellSets())
      		{
      		curInsert->SetOldPointIndex();
	 	cs = curInsert->GetNextCellSet();
		// printf("DXFFile::GetNextCellSet after CellSet\n");
	 /*	if(!curInsert->MoreCellSets())
	                curInsert->SetFlag();*/
      		}
        }
   }
   printf("DXFFile::GetNextCellSet return\n");
   int *index = cs->GetCells();
   for (int i = cs->GetNumVertices(); i > 0; i--, index++)
	   {
	   //if (debug) printf("3: %i ",*(index));
	   }
   return cs;
}


/*****************************************************************************
Method:   DXFFile::GetNumObjects()
Purpose:  Counts the number of objects read from the DXF file.
Params:   ---
State:    ---
Returns:  The number of objects read from the DXF file.
*****************************************************************************/
int DXFFile::GetNumObjects()
{
   int numObjects = 0;
   DXFPrim *ent;
   for(ent = entityList.GetFirst();ent;
       ent = entityList.GetNext());
   {
      if(curLayer==ent->GetLayer())
      	++numObjects;
   }
   return numObjects;
}

/*****************************************************************************
Method:   DXFFile::ListLayer()
Purpose:  This private method adds the specified new DXFLayer item to the
          specified list.
Params:   head, tail   The head and tail of the list to which to add the
                       specified new item.  If these are both NULL, they
		       will be set to point to the new item.  Otherwise,
		       tail->next will be set to point to the new item,
		       tail will be changed to point to the new item, and
		       head will remain unchanged.
	  newLayer       The new DXFLayer object to add.
State:    ---
Returns:  ---
*****************************************************************************
void DXFFile::ListLayer(DXFLayerListEl *&head, DXFLayerListEl *&tail,
			DXFLayer *newLayer)
{
   DXFLayerListEl *newEl = new DXFLayerListEl(newLayer);

   if (newEl)
   {
      if (tail)
	     tail->next = newEl;
      else
	     head = newEl;

      tail = newEl;
   }
}

*/

/*****************************************************************************
Method:   DXFFile::GetNumText()
Purpose:  Counts the total number of annotations read from the DXF file.
Params:   ---
State:    ---
Returns:  The total number of annotations read
          from the DXF file.
*****************************************************************************/
int DXFFile::GetNumText()
{
   if (debug) printf("DXFFile::GetNumText\n");
   int numText = 0;
   DXFPrim *t = textList.GetFirst();
   DXFInsert *ins=insertList.GetFirst();

   while (t)
   {
      numText++; 
      t = textList.GetNext();
   }

   while (ins)
   {
      numText+=ins->GetNumText();
      ins=insertList.GetNext();
   }
   if (debug) printf("DXFFile::GetNumText %i\n",numText);
   return numText;
}


/*****************************************************************************
Method:   DXFFile::GetTextPoints()
Purpose:  Gets the coordinate values for every annotation read
          from the DXF file.
Params:   array   The array in which to put the coordinate values.  This
                  array should be 3 * < DXFFile::GetNumText() > floats
		  long.
State:    For a point whose index into "array" is i, its coordinates are
          placed as follows:

	     x --> array[3*i + 0]
	     y --> array[3*i + 1]
	     z --> array[3*i + 2]
	  
Returns:  The number of floats placed in the specified output array.
*****************************************************************************/
int DXFFile::GetTextPoints(float *array)
{
   int i = 0;
   DXFPrim *t = textList.GetFirst();
   DXFInsert *ins = insertList.GetFirst();

   //if (debug) printf("DXFFile::GetTextPoints\n");
   
   while (t)
   {
      t->SetFirstPointIndex(i / 3);
      i = t->GetPoints(array, i);

      t = textList.GetNext();
   }
   
   while (ins)
   {
       i=ins->GetTextPoints(array,i);
       ins=insertList.GetNext();
   }

   return i;
}

/*****************************************************************************
Method:   DXFFile::MoveToFirstText()
Purpose:  Initialise a pointer to the first DXFText in the file.  
Params:   ---
*****************************************************************************/
void DXFFile::MoveToFirstText()
{
	if (debug) printf("DXFFile::MoveToFirstText\n");
	textObj = textList.GetFirst();
	curInsert = insertList.GetFirst();
	if(curInsert) curInsert->MoveToFirstText();
}


/*****************************************************************************
Method:   DXFFile::GetNextText()
Purpose:  Gets a pointer to the next DXFText in the file.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A pointer to the next text string
*****************************************************************************/
char *DXFFile::GetNextText()
{
//printf("DXFFile::GetNextText\n");
DXFText *text=0;
char *t=0;
if(textObj)
   {
   text = (DXFText *)textObj;
   textObj = textList.GetNext();
   }
else
   {
   while(!text && curInsert)
       {
       text = curInsert->GetNextText(); 
       if(!text)
           { 
           curInsert=insertList.GetNext();
           curInsert->MoveToFirstText();
           }
       }
   }
if(text)     
   return text->GetText();          
return t;
}



/*****************************************************************************
Method:   DXFFile::GetNextWholeText()
Purpose:  Gets the next DXFText in the file.  Each object
          in the list will have exactly one text string.
Params:   ---
State:    This function should not be called until MoveToFirstText()
          has been called at least once.
Returns:  A struct with the next text string
*****************************************************************************/
structtext* DXFFile::GetNextWholeText()
{
DXFText *text=0;
structtext wholeText1;
structtext *wholeText=(structtext *)malloc(sizeof(structtext));
if(textObj)
   {
   text = (DXFText *)textObj;
   textObj = textList.GetNext();
   }
else
   {
   while(!text && curInsert && (strcmp(wholeText->text,"")==0))
       {
       wholeText = curInsert->GetNextWholeText(); 
       if(strcmp(wholeText->text,"")==0)
           { 
           curInsert=insertList.GetNext();
           if(curInsert) curInsert->MoveToFirstText();
           }
       }
   }
if(text)  
   {
   *wholeText=text->GetWholeText(); 
   return wholeText;
   }
return wholeText;          
}


/*****************************************************************************
Method:   DXFFile::GetFirstBlock()
Purpose:  Give first Block from List back 
Params:   ---
State:    ---
Returns:  A pointer to the first Block
*****************************************************************************
DXFBlock *DXFFile::GetFirstBlock()
{
   return curBlock=blockList.GetFirst();
}

*****************************************************************************
Method:   DXFFile::GetNextBlock()
Purpose:  Give next Block from List back 
Params:   ---
State:    ---
Returns:  A pointer to the next Block
*****************************************************************************
DXFBlock *DXFFile::GetNextBlock()
{
   return curBlock=blockList.GetNext();
}*/


void DXFFile::Dump()
{
 /*  DXFPrimListEl *el;

//   if (debug) printf("\nBLOCKS:\n");

   el = textHead;

   while (el)
   {
      el->object->Dump();
      el = el->next;
   }

//   if (debug) printf("\nENTITIES:\n");

   el = entitiesHead;

   while (el)
   {
      el->object->Dump();
      el = el->next;
   }*/
}

void DXFFile::deleteLists()
{
}


/***************************************************************************
function: complexEntity
purpose:  only to improve the readability of constructor DXFFile
params:   all neccessary lists and DXFGroup
return value: char
	       1 entity was be handled correctly
***************************************************************************/

char complexEntity(DXFGroup &group,
	          CLIST <DXFLayer> *layerList,
		  CLIST <DXFPrim> *entityList,
	     	  CLIST <DXFPrim> *textList,
		  CLIST <DXFBlock> *blockList,
		  CLIST <DXFInsert> *insertList)
{
//printf("complexEntity\n");
  if(group.DataCmp("BLOCK"))
  {
	DXFBlock *newBlock = DXFBlock::Make(group,layerList);
	if(newBlock->IsValid())
		blockList->InsertBehind(newBlock);
	else
		return 0;
     	return 1;
  }

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
	
        //printf("complexEntity in MTEXT\n");

	DXFMText *mtextnew=new DXFMText(group, layerList,textList);
	return 1;
  }

return 2;			//it was not an entity from here
}		

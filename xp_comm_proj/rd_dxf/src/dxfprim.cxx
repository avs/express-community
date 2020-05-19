/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

Implementation of the DXFPrim class, which provides the basis for subclasses
to permit reading of various AutoCAD primitives.

******************************************************************************
*****************************************************************************/

#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "dxfprim.hxx"
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
//#include "dxfinsert.hxx"
#include "dxfleader.hxx"
#include "dxfblock.hxx"
#include "dxfrecordblock.hxx"
#include "dxftext.hxx"
#include "dxfmline.hxx"
#include "dxfxline.hxx"
#include "dxftrace.hxx"
#include "blistcpp.hxx"
#include "blistcpp.cxx"   //## AWD: Added to resolve templates problem
#include "dxfdefine.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

typedef DXFPrim *(*DXFPrimMakeFunction)(DXFGroup &group, CLIST <DXFLayer> *tableHead);


/*******************  Private (Static) Global Variables  ********************/

static DXFPrimMakeFunction S_MakeFunctionList[] = {
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
   DXFRecordBlock::Make,
   DXFText::Make,
   DXFMLine::Make,
   DXFTrace::Make,
   DXFXLine::Make,
   0 };    // This marks the end of the array; don't delete it!



/*****************************************************************************
*********--------   Implementations of "DXFGroup" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFGroup::Read()
Purpose:  Reads one group from the DXF file.
          A group consists of an integer group code and string data (which
	  may represent a string, and integer, or a floating-point number).
Params:   ---
State:    This method assumes that the "fp" attribute is valid.
          If successful, this function will advance the file pointer in the
	  DXF file past the current group.
Return:   True   if the current group was read successfully, or
          False  if the "fp" attribute is invalid, or an illegal DXF
	         construct is encountered, or if the file could not be
		 read due to EOF, etc.
*****************************************************************************/
char DXFGroup::Read()
{
   char rCode = 0;
   int groupCode;

   code = DXF_INVALID_GROUP;

   if (GetLine())
   {
      char *ep;

      groupCode = (int)strtol(data, &ep, 10);

      if (*ep == '\0')
	 if (GetLine())
	 {
	    code = groupCode;
	    rCode = 1;
	 }
   }

   return rCode;
}


/*****************************************************************************
Method:   DXFGroup::DataCmp()
Purpose:  Performs a case-insensitive string compare to determine if the
          contents of the "data" attribute matches the specified string.
Params:   s   The string to compare with the contents of "data".
State:    This function assumes that the character encoding is ASCII;
          specifically that [A-Z] and [a-z] are separate contiguous ranges,
	  and that the characters are ordered low-to-high as indicated.
Return:   True   if the contents of the "data" attribute matches the
                 specified string,
          False  if the strings don't match, OR
	         if "code" is DXF_INVALID_GROUP
*****************************************************************************/
char DXFGroup::DataCmp(const char *s) const
{
   char rCode = 0;

   if (code != DXF_INVALID_GROUP)
   {
      const char *x = data;
      char a, b;
      char conv = 'a' - 'A';

      rCode = 1;   // assume strings are equal until we find a mismatch

      do
      {
	 a = *x++;
	 b = *s++;

	 if ((a >= 'a') && (a <= 'z'))   // convert to upper case
	    a -= conv;

	 if ((b >= 'a') && (b <= 'z'))   // convert to upper case
	    b -= conv;

	 if (a != b)
	 {
	    rCode = 0;
	    break;
	 }
      } while (a && b);
   }

   return rCode;
}


/*****************************************************************************
Method:   DXFGroup::GetLine()
Purpose:  Reads a line (i.e. from the current position up to the first
          newline character, or until EOF) from the DXF file.  The line
	  is read into the "data" attribute, and any leading or trailing
	  whitespace (including newlines) is removed.
Params:   ---
State:    ---
Return:   True   if a line was read from the DXF file successfully, or
          False  if the "dxfFile" attribute is NULL, or if EOF has been
	         reached.
*****************************************************************************/
char DXFGroup::GetLine()
{
   char rCode = 0;

   if (dxfFile)   // if dxfFile is NULL, ignore it
   {
      if (fgets(data, DXF_MAX_LINE_LEN, dxfFile))   // if read successfully
      {
	 char *cp = data;

	 data[DXF_MAX_LINE_LEN] = '\0';  // ensure null-termination

	 if (isspace(*cp))  // if there is leading whitespace
	 {
	    while (isspace(*(++cp)));   // find the first non-blank
	    strcpy(data, cp);           // get rid of leading blanks
	 }

	 cp = data + strlen(data);

	 while (isspace(*(--cp)));   // skip any trailing blanks

	 *(cp + 1) = '\0';

	 rCode = 1;
      }
   }

   return rCode;
}


/*****************************************************************************
********--------   Implementations of "DXFLayer" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFLayer::DXFLayer(char *Name, int Color, unsigned Flags)
Purpose:  A constructor for the DXFLayer class.
Params:   Name, Color, Flags
State:    ---
*****************************************************************************/
DXFLayer::DXFLayer(char *Name, int Color, unsigned Flags)
{
   strcpy(name, Name);
   color = Color;
   flags = Flags;
   return;
}


/*****************************************************************************
Method:   DXFLayer::DXFLayer()
Purpose:  Copy constructor for the DXFLayer class.
Params:   l   The DXFlayer object to copy.
State:    ---
*****************************************************************************/
DXFLayer::DXFLayer(const DXFLayer &l) : color(l.color), flags(l.flags)
{
	strcpy(name, l.name);

}

/*****************************************************************************
Method:   DXFLayer::NameCmp()
Purpose:  Performs a case-insensitive string compare to determine if the
          contents of the "data" attribute matches the specified string.
Params:   s   The string to compare with the contents of "data".
State:    This function assumes that the character encoding is ASCII;
          specifically that [A-Z] and [a-z] are separate contiguous ranges,
	  and that the characters are ordered low-to-high as indicated.
Return:   True   if the contents of the "data" attribute matches the
                 specified string,
          False  if the strings don't match
*****************************************************************************/
char DXFLayer::NameCmp(const char *s) const
{
   char rCode = 0;

   const char *x = name;
   char a, b;
   char conv = 'a' - 'A';

   rCode = 1;   // assume strings are equal until we find a mismatch

   do
     {
	 a = *x++;
	 b = *s++;

	 if ((a >= 'a') && (a <= 'z'))   // convert to upper case
	    a -= conv;

	 if ((b >= 'a') && (b <= 'z'))   // convert to upper case
	    b -= conv;

	 if (a != b)
	    {
	    rCode = 0;
	    break;
	    }
     } while (a && b);
   
   return rCode;
}
/*****************************************************************************
********--------   Implementations of "DXFCellSet" Methods   --------*********
*****************************************************************************/


/*****************************************************************************
Method:   DXFCellSet::DXFCellSet()
Purpose:  A constructor for the DXFCellSet class.
Params:   cs   The DXFCellSet object to copy.
State:    ---
*****************************************************************************/
DXFCellSet::DXFCellSet(int nverts, int ncells, int ctype, char flg) : isValid(0),
      flags(flg), numVerticesPerCell(nverts), numCells(ncells),
      cells(0), next(0), usedVertices(0)
{
   cells = new int[GetNumVertices()];

   if (cells)
      isValid = 1;
   hasData = 0;
   cellType = ctype;
}


/*****************************************************************************
Method:   DXFCellSet::DXFCellSet()
Purpose:  Copy constructor for the DXFCellSet class.
Params:   cs   The DXFCellSet object to copy.
State:    ---
*****************************************************************************/
DXFCellSet::DXFCellSet(const DXFCellSet &cs) : isValid(cs.isValid),
      flags(cs.flags), numVerticesPerCell(cs.numVerticesPerCell),
      numCells(cs.numCells), cells(0), next(0), usedVertices(cs.usedVertices),
      cellType(cs.cellType), hasData(cs.hasData), data(cs.data)
{
   if (isValid)
   {
      cells = new int[GetNumVertices()];

      if (cells)
      {
	 for (int i = 0; i < usedVertices; i++)
	    cells[i] = cs.cells[i];
      }
      else
	 isValid = 0;
   }
}


/*****************************************************************************
Method:   DXFCellSet::AddVertexIndex()
Purpose:  Adds an index to the list of vertex indexes.
Params:   vi   The vertex index to add.
State:    This function will terminate the program if an attempt is made to
          add too many vertex indexes.
Return:   ---
*****************************************************************************/
void DXFCellSet::AddVertexIndex(int vi)
{
   if (usedVertices < GetNumVertices())
      cells[usedVertices++] = vi;
   else
   {
//      fprintf(stderr, "Array overflow in DXFCellSet::AddVertexIndex()\n");
//      fprintf(stderr, "   Array length = %d\n\n", GetNumVertices());
      exit(3);
   }
}

void DXFCellSet::setCellData(int ndata, double indata)
{
	int i;
	hasData = 1;
	data = new double[ndata];
	for(i = 0; i < ndata; i++)
		data[i] = indata;
	return;
}
void DXFCellSet::Dump() const
{
//   printf("Cell set: %d vertices per cell, %d cells\n", GetNumVertices(),
//	  numCells);

   if (isValid)
   {
//      printf("   total vertices: %d\n   Cells:\n", usedVertices);

      for (int i = 0; i < usedVertices; )
      {
//	 printf("      ");

	 for (int j = 0; j < numVerticesPerCell; j++, i++);
//	    if (i < usedVertices)
//	       printf("%5d", cells[i]);

//	 printf("\n");
      }
   }
   else
      printf("********* INVALID! ***********\n\n");
}



/*****************************************************************************
**********--------   Implementations of "DXFPrim" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   DXFPrim::Make()
Purpose:  This is the static "virtual constructor" for the DXFPrim class.
          This method revectors this "create object" request to the Make()
	  method of each subclass of DXFPrim of which it is aware.  The
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
Returns:  A pointer to a DXFPrim object if the current object was not
          recognized, or
	  A pointer to a subclass of DXFPrim if it was.
*****************************************************************************/
DXFPrim *DXFPrim::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
int i;	 
FILE *file_ptr;   
DXFPrim *instance = 0;

   // To add a new supported object type, it must be a subclass of DXFPrim
   // and must implement a Make() function that will create an instance of
   // that class if the supplied group data is appropriate.  This new
   // implementation of Make() should be added to the S_MakeFunctionList[]
   // array defined as a global for this module.  Also, whatever needed
   // header files should be #included at the top of this file.
   //
   for (i = 0; S_MakeFunctionList[i] && (!instance); i++)
   {
     instance = (*S_MakeFunctionList[i])(group, firstLayer);
      if (instance)
	    if (!instance->IsValid())
	    {
	       fprintf(stdout,"entity %s not valid.\n",group.Data());
               file_ptr=fopen("entity.err","a");
	       fprintf(file_ptr,"%s\n",group.Data());
	       fclose(file_ptr);	
	       delete instance;
	       instance = 0;
	    }
   }
   if (!instance) {  // if nobody else admits ownership
       
	 if ((group.DataCmp("BLOCK")) || (group.DataCmp("ENDBLK")) ||
		 (group.DataCmp("LAYER")) )
	 {
       } 
	
	 else 
	 {
	   fprintf(stdout,"entity %s not implemented yet.\n",group.Data());
	   file_ptr=fopen("error.err","a");
	   fprintf(file_ptr,"%s\n",group.Data());
           fclose(file_ptr);
	 }
	 instance = new DXFPrim(group, firstLayer);
  }
  
   return instance;
}


/*****************************************************************************
Method:   DXFPrim::DXFPrim()
Purpose:  A Constructor for the DXFPrim class.  Reads enough of the specified
          file to advance the file pointer past the current DXF primitive,
	  but does not retain any information about that primitive.
Params:   group   The DXFGroup object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section. - not any more
*****************************************************************************/
DXFPrim::DXFPrim(DXFGroup &group, CLIST <DXFLayer> *firstLayer) : isValid(0), 
                errorDesc(), cellSetsHead(0), cellSetsTail(0), numCellSets(0),
				color(DXF_NO_COLOR), hasLayer(0), curLayer(0)
   // The DXF file contains a 0 group specifying the start of each new
   // entity.  The 0 group also signals the end of the definition of the
   // previous entity, if any.  Thus, in the simplest case we need only
   // verify that the current group code is 0, and keep reading groups
   // until we hit another group code of 0 (or EOF).
   // Subcomponents of the current object may follow its definition;
   // if so, a group 66 ("Entities follow") flag value of 1 will be
   // present.  Thus, to handle such compound objects, we need to keep
   // track of the nesting level, and only consider the current object's
   // definition to be complete when the end of the last subobject's
   // definition has been encountered (or EOF).
{
	//printf("In DXFPrim::DXFPrim() with group code %d, data = \"%s\"\n",
//       group.Code(), group.Data());

   if (group.Code() == 0)  // if this is an entity definition
   {
       if(group.DataCmp("LAYER"))
		   hasLayer = 1;
	   char verticesFollow = 0;
	   char Name[DXF_MAX_LINE_LEN+1];
	   int Color = DXF_NO_COLOR;
	   unsigned Flags = 0;;
      while (group.Read())
        {
	      if (group.Code() == 2 && hasLayer){
			  strcpy(Name, group.Data());
		  }
		  if (group.Code() == 62 && hasLayer)	{
			  Color = group.DataAsInt();
		  }
		  if (group.Code() == 70 && hasLayer){
			  Flags = group.DataAsUnsigned();
		  }
	    if (group.Code() == 0)
	       break;

	 if ((group.Code() == 66) && (*(group.Data()) == '1'))
	    verticesFollow = 1;
      }

      // If a vertex list follows the object definition, read past those
      // vertices to the "SEQEND" entity.
      //
      if (verticesFollow)
      {
	 while (group.Read())
	 {
	    if (group.Code() == 0)
	       if (group.DataCmp("SEQEND"))
	       {
		  // Now read to the end of this SEQEND object, to leave
		  // the file pointer in the proper place.
		  //
		  while (group.Read())
		     if (group.Code() == 0)
			break;

		  break;
	       }
	 }
      }

      isValid = group.Code() == 0;
	  if(hasLayer)
		  curLayer = new DXFLayer(Name, Color, Flags);
   }
}


/*****************************************************************************
Method:   DXFPrim::~DXFPrim()
Purpose:  Destructor for the DXFPrim class.
Params:   ---
State:    Destroys the cell set list.
*****************************************************************************/
DXFPrim::~DXFPrim()
{
   DXFCellSet *cs = cellSetsHead;
   DXFCellSet *doomedCellSet;

   while (cs)
   {
      doomedCellSet = cs;
      cs = cs->next;

      delete doomedCellSet;
   }
}


/*****************************************************************************
Method:   DXFPrim::DXFPrim()
Purpose:  Copy constructor for the DXFPrim class.
Params:   prim   The DXFPrim object to copy.
State:    ---
*****************************************************************************/
DXFPrim::DXFPrim(const DXFPrim &prim) : isValid(prim.isValid),
      errorDesc(prim.errorDesc), cellSetsHead(0), cellSetsTail(0),
      numCellSets(0), color(DXF_NO_COLOR)
{
   DXFCellSet *cs = prim.cellSetsHead;

   while (cs)
   {
      if (AddCellSet(cs->GetNumVerticesPerCell(), cs->GetNumCells(), 
                     cs->GetCellType()))
      {
	 const int *v = cs->GetCells();
	 int totalVertices = cs->GetNumVertices();

	 for (int i = 0; i < totalVertices; i++, v++)
	    cellSetsTail->AddVertexIndex(*v);
      }

      cs = cs->next;
   }
}


/*****************************************************************************
Method:   DXFPrim::ReadCommon()
Purpose:  Reads common (i.e. applicable to all or most AutoCAD objects)
          attributes.
Params:   group   The DXFGroup object being used to read the DXF file.
                  This function will not change this object, nor advance
		  the DXF file pointer.
State:    This function should be called by each derived class after that
          class has determined that the current group code is uninteresting.
Returns:  ---
*****************************************************************************/
void DXFPrim::ReadCommon(const DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
   //printf("DXFPrim::ReadCommon colour0: %i\n",color);
   switch (group.Code())
   {
     case -1:   // Entity name
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
		 	 //printf("DXFPrim::ReadCommon colour1.1: %i %s %s\n",color,group.Data(),labelLayer->GetName());
			 color = labelLayer->GetColor();
			 curLayer=labelLayer;
			 //printf("DXFPrim::ReadCommon colour1.2: %i \n",color);
			 }
		 //flags = labelLayer->GetFlags();
		 //printf("DXFPrim::ReadCommon colour2: %i\n",color);
         }
      break;

     case 62:   // Color number
      color = group.DataAsInt();
	   //printf("color is %d\n", color);
      break;

     default:
      break;
   }
   //printf("DXFPrim::ReadCommon colour: %i\n",color);
}


/*****************************************************************************
Method:   DXFPrim::AddCellSet()
Purpose:  Adds a cell set (a DXFCellSet object) to the linked list of cell
          sets.
Params:   nverts   The number of vertices per cell.
          ncells   The number of cells in the cell set.
          ctype    Flag to indicate whether outlines or faces are required
                   DXF_CELL_LINE - always a line
                   DXF_CELL_FACE - always a face
                   DXF_CELL_CHOOSE - controlled by user
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
char DXFPrim::AddCellSet(int nverts, int ncells, int ctype, char flags)
{
   char ok = 0;
   DXFCellSet *cellSet = new DXFCellSet(nverts, ncells, ctype, flags);

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
}


/*****************************************************************************
Method:   DXFPrim::GetNextCellSet()
Purpose:  Gets a pointer to the next DXFCellSet associated with this object.
Params:   ---
State:    ---
Returns:  A pointer to the next cell set associated with the this object, or
	  NULL if there are no more cell sets associated with the current
	  object.
*****************************************************************************/
DXFCellSet *DXFPrim::GetNextCellSet()
{
   DXFCellSet *cs = curCellSet;

   if (curCellSet)
      curCellSet = curCellSet->next;

   return cs;
}
/*****************************************************************************
Implementations of DXFLayerListEl methods
*****************************************************************************/

/*****************************************************************************
Method:   DXFLayerListEl::GetLayer()
Purpose:  Sets an internal DXFLayer pointer to point to the layer
          in the internal list of layers read from the DXF file, which 
		  matches the name.
Params:   char *Name
Returns:  DXFlayer *
*************************************************************************/
DXFLayer *DXFLayerListEl::GetLayer(const char *Name)
{
	DXFLayer *testLayer = Layer;
	int match = 0;
    DXFLayer *reqLayer = 0;
    DXFLayerListEl *nextLayerList = next;
	while(!match){
	   if(testLayer->NameCmp(Name)){
		  match = 1;
	      reqLayer = testLayer;
	   }
       if(nextLayerList){
	     testLayer = nextLayerList->Layer;
		 nextLayerList = nextLayerList->next;
	   }
	   else 
		 match = 2;
	}
	if(match == 1)
		return reqLayer;
	else
		return 0;
} 


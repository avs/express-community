/*****************************************************************************
SM -- test
******************************************************************************
Similar to DXFPrim.cxx, stores a description of a block
******************************************************************************
*****************************************************************************/

#include <stdio.h>

#include "blockprim.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

//typedef BlockPrim *(*DXFPrimMakeFunction)(DXFGroup &group);


/*******************  Private (Static) Global Variables  ********************/

/*  <none> */

/*****************************************************************************
BlockPrim methods*/

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
char BlockPrim::Read()
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
Method:   BlockPrim::DataCmp()
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
char BlockPrim::DataCmp(const char *s) const
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
Method:   BlockPrim::GetLine()
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
char BlockPrim::GetLine()
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
/*****************************************************************************
**********--------   Implementations of "BlockPrim" Methods   --------**********
*****************************************************************************/


/*****************************************************************************
Method:   BlockPrim::Make()
Purpose:  This is the static "virtual constructor" for the BlockPrim class.
          This method revectors this "create object" request to the Make()
	  method of each subclass of BlockPrim of which it is aware.  The
	  actual instance is created by the first subclass whose Make()
	  function returns a non-NULL pointer.
	  This could have been implemented more elegantly using the
	  exemplars idiom, but this is less involved and not too onerous
	  to modify.
Params:   group   The BlockPrim object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
Returns:  A pointer to a BlockPrim object if the current object was not
          recognized, or
	  A pointer to a subclass of BlockPrim if it was.
*****************************************************************************/
BlockPrim *BlockPrim::Make(BlockPrim &group)
{
int i;	 
FILE *file_ptr;   
BlockPrim *instance = 0;

   // To add a new supported object type, it must be a subclass of BlockPrim
   // and must implement a Make() function that will create an instance of
   // that class if the supplied group data is appropriate.  This new
   // implementation of Make() should be added to the S_MakeFunctionList[]
   // array defined as a global for this module.  Also, whatever needed
   // header files should be #included at the top of this file.
   //
   for (i = 0; S_MakeFunctionList[i] && (!instance); i++)
   {
      instance = (*S_MakeFunctionList[i])(group);

      if (instance)
	 if (!instance->IsValid())
	 {
	    delete instance;
	    instance = 0;
	 }
   }

   if (!instance) {  // if nobody else admits ownership
        instance = new BlockPrim(group);
	file_ptr=fopen("error.err","a");
	if ((group.DataCmp("BLOCK")) || (group.DataCmp("ENDBLK")))
	{
       } 
	else 
	{
	fprintf(file_ptr,"%s\n",group.Data());
       }
fclose(file_ptr);
	}
   return instance;
}


/*****************************************************************************
Method:   BlockPrim::BlockPrim()
Purpose:  A Constructor for the BlockPrim class.  Reads enough of the specified
          file to advance the file pointer past the current DXF primitive,
	  but does not retain any information about that primitive.
Params:   group   The BlockPrim object being used to read the DXF file.
                  It is assumed that this object will initially contain
		  the group that terminated the previous object definition.
		  On exit, it will contain the group that terminates this
		  object definition.
State:    It is assumed that the file pointer into the DXF file has been
          advanced into the BLOCKS or ENTITIES section.
*****************************************************************************/
BlockPrim::BlockPrim(BlockPrim &group) : isValid(0), errorDesc(), cellSetsHead(0),
      cellSetsTail(0), numCellSets(0), color(DXF_NO_COLOR)
{
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

//printf("In BlockPrim::BlockPrim() with group code %d, data = \"%s\"\n",
       group.Code(), group.Data());

   if (group.Code() == 0)  // if this is an entity definition
   {
      char verticesFollow = 0;

      while (group.Read())
      {
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
   }
}


/*****************************************************************************
Method:   BlockPrim::~BlockPrim()
Purpose:  Destructor for the BlockPrim class.
Params:   ---
State:    Destroys the cell set list.
*****************************************************************************/
BlockPrim::~BlockPrim()
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
Method:   BlockPrim::BlockPrim()
Purpose:  Copy constructor for the BlockPrim class.
Params:   prim   The BlockPrim object to copy.
State:    ---
*****************************************************************************/
BlockPrim::BlockPrim(const BlockPrim &prim) : isValid(prim.isValid),
      errorDesc(prim.errorDesc), cellSetsHead(0), cellSetsTail(0),
      numCellSets(0), color(DXF_NO_COLOR)
{
   DXFCellSet *cs = prim.cellSetsHead;

   while (cs)
   {
      if (AddCellSet(cs->GetNumVerticesPerCell(), cs->GetNumCells()))
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
Method:   BlockPrim::ReadCommon()
Purpose:  Reads common (i.e. applicable to all or most AutoCAD objects)
          attributes.
Params:   group   The BlockPrim object being used to read the DXF file.
                  This function will not change this object, nor advance
		  the DXF file pointer.
State:    This function should be called by each derived class after that
          class has determined that the current group code is uninteresting.
Returns:  ---
*****************************************************************************/
void BlockPrim::ReadCommon(const BlockPrim &group)
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



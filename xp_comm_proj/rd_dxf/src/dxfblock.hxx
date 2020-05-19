/*****************************************************************************
DXFBlock.hxx
******************************************************************************

This file contains the DXFBlock class definition.  
It is based on DXFPrim.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_BLOCK_H
#define _DXF_BLOCK_H


#include <stdio.h>
#include <stdlib.h>

#include "avsutils.hxx"
#include "dxfprim.hxx"
#include "dxftext.hxx"
#include "blistcpp.hxx"
#include "dxfdefine.hxx"
#include "dxfdimension.hxx"
#include "dxfmtext.hxx"

/*********************  Macros and Manifest Constants  **********************/

#define DXF_INVALID_GROUP     -1
#define DXF_MAX_LINE_LEN     255
#define DXF_NO_COLOR         256
#define DXF_CLOSED_POLYGON  0x01
#define DXF_CONVEX_POLYGON  0x02


/************************  Public Type Definitions  *************************/

/*typedef enum
{
   DXF_LINE_CELL = 1,
   DXF_TRI_CELL,
   DXF_QUAD_CELL
} DXFCellSetType;*/


/*****************************************************************************
Class:    DXFBlock
Purpose:  This class is used to represent DXF objects in which we are not
          really interested.  It knows how to read any single entity from
	  the DXF file, and is used primarily to advance the file pointer
	  past those primitives in which we are not interested.
	  This is also the superclass for more refined classes to represent
	  each of the DXF primitive object types in which we are interested.
*****************************************************************************/

class DXFBlock
{
friend class DXFInsert;
  public:
   DXFBlock(DXFGroup &group,CLIST <DXFLayer> *firstLayer);
   DXFBlock(DXFBlock &prim);
   ~DXFBlock();

   static DXFBlock *Make(DXFGroup &group,CLIST <DXFLayer> *firstLayer);

   char IsValid()       { return isValid; }

   virtual int GetColorNumber()  { return color; }
   int GetColorNums(short int *array, int offset);

   virtual int GetNumPoints(CLIST <DXFBlock> *blocklist = 0);
   virtual int GetPoints(float *, int offset = 0);

   int GetNumText();
   int GetTextPoints(float *array,int offset=0);
   void MoveToFirstText();
   DXFText *GetNextText();
   structtext *GetNextWholeText();
   
   int GetNumCellSets();
   void MoveToFirstCellSet();  
   char MoreCellSets();    
   DXFCellSet *GetNextCellSet();

   virtual void Dump()  { printf("   Primitive generic\n"); }
   DXFPoint3D GetBasePoint()	{return basepoint;}
   
   void setOldPointIndex(int firstPointIndex)
	{
	int *a=new int();
	*a=firstPointIndex;
	printf("setOldPointIndex %i",*a);
	OldFirstPointOffset.InsertTail(a);printf("DXFBlock::setOldPointIndex %i ",firstPointIndex);
	}
   void setFlagOPI(char flag) {flagOPI=flag;}
  
   void deleteLists();

  protected:
   char isValid;
   AVSString errorDesc;
   AVSString Blockname;
   DXFPoint3D basepoint;
   
   
   CLIST <DXFPrim> entityList;
   CLIST <DXFPrim> textList;
   CLIST <DXFInsert> insertList;
   
   DXFPrim * curObj;
   DXFPrim * curText;
   DXFInsert * curInsert;
   
   
   
   int numCellSets;
   int color;
   
   CLIST <int> OldFirstPointOffset;
   char flagOPI;

   // This constructor (with no parameters) is automatically called when
   // a subclass of DXFBlock is created.  It is important not to call either
   // of the parameterized constructors, as they perform unwanted
   // initializations.
   //
   DXFBlock() : isValid(0), errorDesc(), 
               numCellSets(0), color(DXF_NO_COLOR)  {}

   void ReadCommon(const DXFGroup &group);
   //char AddCellSet(int nverts, int ncells, char flags = 0);

  private:
//   void AddToList(DXFPrimListEl *&head, DXFPrimListEl *&tail,DXFPrim *newObj);
//   void AddInsertToList(DXFInsertList *&head, DXFInsertList *&tail,
//  			DXFInsert *newInsert);	
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

char complexEntity(DXFGroup &group,
	          CLIST <DXFLayer> *layerList,
		  CLIST <DXFPrim> *entityList,
	     	  CLIST <DXFPrim> *textList,
		  CLIST <DXFInsert> *insertList);




#endif











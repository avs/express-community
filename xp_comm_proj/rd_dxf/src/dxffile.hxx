/*****************************************************************************
Amended SM
******************************************************************************

This file contains the DXFFile class definition, which reads a DXF file and
parses it into a list of DXFPrim (or a subclass thereof) objects.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_FILE_H
#define _DXF_FILE_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "dxfblock.hxx"
#include "dxfinsert.hxx"
#include "blistcpp.hxx"
#include "dxfheader.hxx"



/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/

/* <none> */

/*****************************************************************************
Class:    DXFFile
Purpose:  This class is used to represent DXF files as a list of DXFPrim
          (or a subclass thereof) objects.
Publics:  
          
	  
*****************************************************************************/
class DXFFile
{
  public:
   DXFFile(const char *fileName, int reverseNormals = 0);
   DXFFile(const DXFFile &dxff);
   virtual ~DXFFile()  {deleteLists();}

   char IsValid()              { return isValid; }
   const char *GetErrorDesc()  { return errorDesc.Get(); }

   int GetNumPoints();     // total of all points of all objects in DXF file
   int GetNumObjects();    // total number of objects defined in DXF file
   int GetNumCellSets();   // total number of cell sets in all object
   int GetNumText();       // total number of annotations definied in DXF file
   int GetNumLayers();	   // total number of layers in DXF file
   char *GetLayername(int i); // name of i'th layer in layerlist
   int GetPoints(float *array);
   int GetTextPoints(float *array);
   int GetColorNums(short *array);
   int GetNumPoints(int layernumber);     // total of all points of all objects in layer[layernumber]
   void MoveToFirstCellSet();
   char MoreCellSets();
   DXFCellSet *GetNextCellSet();
   
   void MoveToFirstText();
   char *GetNextText();
   structtext *GetNextWholeText();

   char BlockList[100];

   void deleteLists();
   void Dump();

  private:
   char isValid;
   AVSString errorDesc;
   DXFCellSet *curCellSet;
   
   CLIST <DXFPrim> entityList;
   CLIST <DXFPrim> tableList;
   CLIST <DXFPrim> textList;
   CLIST <DXFInsert> insertList;
   CLIST <DXFLayer> layerList;
   CLIST <DXFBlock> blockList;

   DXFPrim *curObj;
   DXFPrim *textObj;
   DXFInsert *curInsert;
   DXFBlock *curBlock;
   DXFLayer *curLayer;
   
   /*DXFInsertList *insertHead;
   DXFInsertList *insertTail;
   DXFInsertList *curInsert;
   DXFPrimListEl *entitiesHead;
   DXFPrimListEl *entitiesTail;
   DXFPrimListEl *tablesHead;
   DXFPrimListEl *tablesTail;
   DXFPrimListEl *textHead;
   DXFPrimListEl *textTail;
   DXFLayerListEl *firstLayer;
   DXFLayerListEl *lastLayer;
   void AddToList(DXFPrimListEl *&head, DXFPrimListEl *&tail,
		  DXFPrim *newObj);
   void DestroyList(DXFPrimListEl *head);
   void DestroyList(DXFBlockList *head);
   void DestroyList(DXFInsertList *head);
   void ListLayer(DXFLayerListEl *&head, DXFLayerListEl *&tail,
	              DXFLayer *newLayer);
   DXFBlockList *tableHead;  
   DXFBlockList *tableTail;
  void AddBlockToList(DXFBlockList *&head, DXFBlockList *&tail,
		      DXFBlock *newBlock);
  void AddInsertToList(DXFInsertList *&head, DXFInsertList *&tail,
  			DXFInsert *newInsert);		
  void DestroyBlockList(DXFBlockList *head);*/
  

};


/************************  Public Global Variables  *************************/

extern DXFHeader *globalVar;

/********************  Prototypes for Public Functions  ********************/

char complexEntity(DXFGroup &group,
	          CLIST <DXFLayer> *layerList,
		  CLIST <DXFPrim> *entityList,
	     	  CLIST <DXFPrim> *textList,
		  CLIST <DXFBlock> *blockList,
		  CLIST <DXFInsert> *insertList);



#endif











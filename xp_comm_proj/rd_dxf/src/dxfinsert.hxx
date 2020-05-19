/*****************************************************************************
******************************************************************************

This file contains the DXFInsert class definition, which is the subclass of
DXFPrim that represents the DXF insert primitive.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_INSERT_H
#define _DXF_INSERT_H

#include <stdio.h>

#include "dxfprim.hxx"
#include "avsutils.hxx"
#include "dxftext.hxx"
#include "dxfblock.hxx"
#include "blistcpp.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/


class DXFBlockList
  {
friend class DXFFile;
friend class DXFInsert;
private:
  DXFBlockList(DXFBlock *obj) : object(obj), next(0), listPtr(0) {}
  ~DXFBlockList() {;}//if (object) delete object; }

//  DXFPrim *object;

  DXFBlock *object;		//own datatype for blocks (also new)

  int listPtr;
  DXFBlockList *next;
};

/*****************************************************************************
Class:    DXFInsert
Purpose:  This class is used to represent DXF insert objects.
Publics:  
          
	  
*****************************************************************************/
class DXFInsert : public DXFPrim
{
  public:
   DXFInsert(DXFGroup &group,CLIST <DXFLayer> *firstLayer);
   DXFInsert(const DXFInsert &l) : start(l.start), Blockname(l.Blockname), scaleFactorx(l.scaleFactorx),
                                   scaleFactory(l.scaleFactory), scaleFactorz(l.scaleFactorz) {}
   virtual ~DXFInsert() {block=NULL; printf("destructor insert\n");}

   static DXFInsert *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   int GetNumPoints(CLIST <DXFBlock> *blocklist,DXFLayer *curLayer);
   int GetNumPoints(CLIST <DXFBlock> *blocklist);
   int GetNumPoints();
   int GetPoints(float *array, int offset = 0);
   
   int GetNumText();
   int GetTextPoints(float *array,int offset=0);   
   void MoveToFirstText();
   DXFText *GetNextText();
   structtext *GetNextWholeText();
   
   int GetColorNums(short int *array,int offset=0);
   
   int GetNumCellSets(); 
   DXFCellSet *GetNextCellSet();  
   void MoveToFirstCellSet();
   char MoreCellSets();
   
   int  GetFirstPointIndex() {return firstPointIndex;}
   void SetFirstPointIndex(int index) {firstPointIndex=index;}
   void SetOldPointIndex(int offset=0);
   void SetFlag() {if(block) block->setFlagOPI(1);} 

   void Dump();

  private:
   DXFPoint3D start;
   AVSString Blockname;
   float scaleFactorx;
   float scaleFactory;
   float scaleFactorz;
   DXFPoint3D extrusion;
   float rotationAngle;
   DXFBlock *block;
   int firstPointIndex;
   DXFLayer *curLayer;
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif













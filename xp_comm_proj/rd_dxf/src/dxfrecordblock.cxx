/*****************************************************************************
minimal
******************************************************************************

Creates a list of the blocks.

******************************************************************************
*****************************************************************************/

#include <stdio.h>

#include "dxfprim.hxx"
#include "dxffile.hxx"
#include "dxfrecordblock.hxx"

/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */


/************************  Private Type Definitions  ************************/

/* <none> */


/*******************  Private (Static) Global Variables  ********************/

/* <none> */


/******** Code *********/


DXFPrim *DXFRecordBlock::Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer)
{
DXFPrim *instance = 0;
if (group.DataCmp("BLOCK_RECORD"))
  instance = new DXFRecordBlock(group);

  return instance;
}



DXFRecordBlock::DXFRecordBlock(DXFGroup &group)
{
isValid = 0;
while (group.Read())
  {
if (group.Code() == 0)
break;
//AddEntityToBlockList(blockHead, blockTail, newObj);
  }
isValid = 1;
//printf("%a,Got here!\n");
}



/*void DXFRecordBlock::AddEntityToBlockList(DXFEntityBlockList *&head, 
                         DXFEntityBlockList *&tail, DXFPrim *newObj)
{
   DXFEntityBlockList *newEl = new DXFEntityBlockList(newObj);

   if (newEl)
   {
     if (tail)
       tail->next = newEl;
     else
       head = newEl;

     tail = newEl;
   }
}

void DXFFile::DestroyBlockList(DXFBlockList *head)
{
   DXFBlockList *el;
   DXFBlockList *doomedEl;

   el = head;

   while (el)
   {
      doomedEl = el;
      el = el->next;

      delete doomedEl;
   }
}*/






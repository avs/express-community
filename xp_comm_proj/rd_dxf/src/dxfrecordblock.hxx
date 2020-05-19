/*****************************************************************************

******************************************************************************
TEST -- header file for code which lists entities within a block.

******************************************************************************
*****************************************************************************/
#ifndef _DXF_BLOCK_RECORD_H
#define _DXF_BLOCK_RECORD_H

#include <stdio.h>

#include "dxfprim.hxx"
//#include "dxffile.hxx"
#include "avsutils.hxx"

class DXFEntityBlockList
{
friend class DXFFile;
private:
  DXFEntityBlockList(DXFPrim *obj) : object(obj), next(0), attribs(0) {}
  ~DXFEntityBlockList() {if (object) delete object; }

  DXFPrim *object;
  int attribs;
  DXFEntityBlockList *next;
};


class DXFRecordBlock : public DXFPrim
{
public:
  DXFRecordBlock(DXFGroup &group);
  DXFRecordBlock(const DXFRecordBlock &block);
  virtual ~DXFRecordBlock() {}

  static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

  //DXFEntityBlockList *tablesHead;  
  //DXFEntityBlockList *tablesTail;
  DXFEntityBlockList *blockHead;
  DXFEntityBlockList *blockTail;
  void AddEntityToBlockList(DXFEntityBlockList *&head, 
                            DXFEntityBlockList *&tail, DXFPrim *newObj);
  void DestroyEntityBlockList(DXFEntityBlockList *head);
};



#endif














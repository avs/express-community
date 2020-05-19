/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

This file contains the DXFPrim class definition.  DXFPrim provides a non-
abstract base for DXF (AutoCAD) primitive objects.  Classes can be derived
from this one in order to implement new kinds of primitive objects.

Each subclass must know how to read it's data from the DXF file, and must
know how to generate grid and connectivity data for Express.

******************************************************************************
*****************************************************************************/

#ifndef _DXF_PRIM_H
#define _DXF_PRIM_H

#include <stdio.h>
#include <stdlib.h>
//#include "dxffile.hxx"

#include "avsutils.hxx"
#include "blistcpp.hxx"

/*********************  Macros and Manifest Constants  **********************/

#define DXF_INVALID_GROUP     -1
#define DXF_MAX_LINE_LEN     255
#define DXF_NO_COLOR         256
#define DXF_CLOSED_POLYGON  0x01
#define DXF_CONVEX_POLYGON  0x02

#define DXF_CELL_POINT      1
#define DXF_CELL_LINE       2
#define DXF_CELL_FACE       3
#define DXF_CELL_CHOOSE     0

/************************  Public Type Definitions  *************************/

typedef enum
{
   DXF_LINE_CELL = 1,
   DXF_TRI_CELL,
   DXF_QUAD_CELL
} DXFCellSetType;



/*****************************************************************************
Class:    DXFGroup
Purpose:  This simple class defines an object to read a single group (code
          plus data) from a DXF file.  It does not impose any interpertation
	  on the data it reads, other than to convert the first line read
	  into an integer.
Publics:  ---
          This is not intended to be a public class, so its constructors
	  are protected and DXFPrim is declard to be a friend.
*****************************************************************************/
class DXFGroup
{
  public:
   DXFGroup(FILE *fp, char rn) : dxfFile(fp), code(-1), reverseNormals(rn)
      { data[0] = '\0'; }

   char Read();

   int Code() const                 { return code; }
   const char *Data() const         { return (const char *)data; }
   long DataAsLong() const          { char *ep; return strtol(data, &ep, 0); }
   int DataAsInt() const            { return (int)DataAsLong(); }
   unsigned DataAsUnsigned() const  { return (unsigned)DataAsLong(); }
   double DataAsDouble() const      { char *ep; return strtod(data, &ep); }
   float DataAsFloat() const        { return (float)DataAsDouble(); }

   char DataCmp(const char *s) const;

   char ReverseNormals()  { return reverseNormals; }

  private:
   FILE *dxfFile;
   int code;
   char data[DXF_MAX_LINE_LEN + 1];
   char reverseNormals;

   char GetLine();
};


/*****************************************************************************
Class:    DXFPoint3D
Purpose:  A simple point class, similar to AVSPoint but adding the ability
          to get and set the x, y, and z coordinates individually.
Publics:  
          
*****************************************************************************/
class DXFPoint3D : public AVSPoint
{
  public:
   DXFPoint3D()  {}
   DXFPoint3D(const DXFPoint3D &p) : AVSPoint((const AVSPoint &)p) {}
   DXFPoint3D(float a, float b, float c) : AVSPoint(a, b, c) {}
   ~DXFPoint3D() {}

   void Set(float a, float b, float c)           { AVSPoint::Set(a, b, c); }
   void Get(float *a, float *b, float *c) const  { AVSPoint::Get(a, b, c); }
   void Set(const DXFPoint3D &p)                 { p.Get(&x, &y, &z); }
   void Get(DXFPoint3D &p) const                 { p.Set(x, y, z); }

   DXFPoint3D &operator=(const DXFPoint3D &p)  { Set(p); return *this; }

   float GetX()  { return x; }
   float GetY()  { return y; }
   float GetZ()  { return z; }

   void SetX(float nx)  { x = nx; }
   void SetY(float ny)  { y = ny; }
   void SetZ(float nz)  { z = nz; }
};


/*****************************************************************************
Class:    DXFLayer
Purpose:  This class is used to represent layer definitions.  Each layer 
          has a unique name, and stores default color & flags for all 
		  ENTITIES (and BLOCKS??) associated with that layer.
*****************************************************************************/
class DXFLayer
{
   public:
	  DXFLayer(char *Name, int Color, unsigned Flags);
	  DXFLayer(const DXFLayer &l);
	  ~DXFLayer() {}

	  int GetColor() { return color; }
	  unsigned GetFlags() {return flags; }
	  char *GetName() {return name; }
      char NameCmp(const char *s) const;

   private:
      int color;
	  unsigned flags;
	  char name[DXF_MAX_LINE_LEN + 1];
};
/*****************************************************************************
Class:    DXFLayerListEl
Purpose:  This class is used to allow the DXFFile class to keep a linked
          list of DXFLayer objects.
Publics:  
          
	  
*****************************************************************************/
class DXFLayerListEl
{
   friend class DXFFile;
   
  private:
     DXFLayerListEl(DXFLayer *layer) : Layer(layer), next(0)  {}
     ~DXFLayerListEl()  { if (Layer) delete Layer; }

     DXFLayer *Layer;
     DXFLayerListEl *next;
   
  public:
     DXFLayer *GetLayer(const char *Name);
};
/*****************************************************************************
Class:    DXFCellSet
Purpose:  This class is used to represent cell sets.  Each cell set consists
          of a count of vertices per cell (face) and a node connect list,
	  which specifies which vertices are connected together to form
	  the faces.
*****************************************************************************/
class DXFCellSet
{
  public:
   DXFCellSet *next;

   DXFCellSet(int nverts, int ncells, int ctype, char flg = 0);
   DXFCellSet(const DXFCellSet &cs);
   ~DXFCellSet()  { if (cells) delete [] cells; }

   char IsValid() const   { return isValid; }

   void AddVertexIndex(int vi);
   void setCellData(int ndata, double indata);
   int GetNumVerticesPerCell() const  { return numVerticesPerCell; }
   int HasData() { return hasData; }
   int GetNumCells() const  { return numCells; }
   int *GetCells() const    { return cells; }
   double *GetData() const  { return data; }
   char IsClosed()          { return (flags & DXF_CLOSED_POLYGON) != 0; }
   char IsConvex()          { return (flags & DXF_CONVEX_POLYGON) != 0; }

   int GetNumVertices() const  { return numVerticesPerCell * numCells; }
   int GetCellType() const { return cellType; }
   void Dump() const;

  protected:
   char isValid;
   char flags;
   int numVerticesPerCell;
   int numCells;
   int *cells;
   int usedVertices;
   int hasData;
   double *data;
   int cellType;
};


/*****************************************************************************
Class:    DXFPrim
Purpose:  This class is used to represent DXF objects in which we are not
          really interested.  It knows how to read any single entity from
	  the DXF file, and is used primarily to advance the file pointer
	  past those primitives in which we are not interested.
	  This is also the superclass for more refined classes to represent
	  each of the DXF primitive object types in which we are interested.
*****************************************************************************/
class DXFPrim
{
  public:
   DXFPrim(DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   DXFPrim(const DXFPrim &prim);
   virtual ~DXFPrim();

   static DXFPrim *Make(DXFGroup &group, CLIST <DXFLayer> *firstLayer);

   char IsValid()       { return isValid; }

   virtual int GetColorNumber()  { return color; }

   virtual int GetNumPoints()                      { return 0; }
   virtual int GetPoints(float *, int offset = 0)  { return offset; }

   int GetNumCellSets()       { return numCellSets; }
   void MoveToFirstCellSet()  { curCellSet = cellSetsHead; }
   char MoreCellSets()        { return curCellSet != 0; }
   DXFCellSet *GetNextCellSet();

   int HasLayer() { return hasLayer; }
   DXFLayer *GetLayer() { return curLayer; }
   void SetFirstPointIndex(int firstPointI) {firstPointIndex =firstPointI;}
   int GetFirstPointIndex() {return firstPointIndex;}
   virtual void Dump()  { printf("   Primitive generic\n"); }

  protected:
   char isValid;
   AVSString errorDesc;
	    
   DXFCellSet *curCellSet;
   DXFCellSet *cellSetsHead;
   DXFCellSet *cellSetsTail;
   DXFLayer *curLayer;
   int numCellSets;
   int color;
   int hasLayer;
   int firstPointIndex;
   // This constructor (with no parameters) is automatically called when
   // a subclass of DXFPrim is created.  It is important not to call either
   // of the parameterized constructors, as they perform unwanted
   // initializations.
   //
   DXFPrim() : isValid(0), errorDesc(), curCellSet(0), cellSetsHead(0),
               cellSetsTail(0), numCellSets(0), color(DXF_NO_COLOR),
               curLayer(0), hasLayer(0){}

   void ReadCommon(const DXFGroup &group, CLIST <DXFLayer> *firstLayer);
   char AddCellSet(int nverts, int ncells, int ctype, char flags = 0);
   char AddLayer(char *Name, int Color, unsigned flags);
  private:
};

/*****************************************************************************
Class:    DXFPrimListEl
Purpose:  This class is used to allow the DXFFile class to keep a linked
          list of DXFPrim objects.
Publics:  
          
	  
*****************************************************************************/
class DXFPrimListEl
{
   friend class DXFFile;
   friend class DXFBlock;
   
  private:
   DXFPrimListEl(DXFPrim *obj) : object(obj), next(0), firstPointIndex(0)  {}
   ~DXFPrimListEl()  { if (object) delete object; }

   DXFPrim *object;
   int firstPointIndex;
   DXFPrimListEl *next;
   
};



/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif











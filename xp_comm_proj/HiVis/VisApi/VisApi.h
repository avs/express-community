// Technische Universität München - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

#ifndef visVISAPI_H
#define visVISAPI_H

/*
  abbrevs as follows:

  dim: dimension
  attrs: number of attributes
  attrDim: attribute dimension (e.g. scalar attribute <==> attrDim == 1)
  size: array size
  nodes: number of nodes of a cell
  a: attribute type (0 <= a < attrs
  d: dimension (0 <= d < dim)
  p: position (0 <= p[i] < size[i])
  n: node (0 <= n < nodes)
 */


#include "platform.h"

/* -----------------------------------------------------------------------------
   Attribute specification and implementations.
   -------------------------------------------------------------------------- */

/**
   @short Attribute specification.

   Defines the basic functionality of attributes in VisApi: The mapping to a
   scalar or a vector and their arithmetic operations. The reason why they are
   not using operator overloading is the unhandy usage with pointers that need
   to be used with virtual functions.

   An attribute does not store its vector dimension. The vector dimension must
   be obtained from the corresponding attribute factory. This saves unnecessary
   memory usage.

   @author Peter Breitling
   @version 03-20-98 */
class visAttribute : public visObject {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttributeId) return true;
    return visObject::instanceOf(id);
  }

  /**
     Constructor.
  */
  visAttribute();

  /**
     Destructor calls deallocate method.
   */
  virtual ~visAttribute();

  virtual bool allocate();
  virtual void deallocate();

  /**
     Return i-th vector component (0 < i < vector dimension).
  */
  virtual double getValue(int i) = 0;

  /**
     Set this attribute to given constant.
  */
  virtual void setValue(int dim, visAttrVal val) = 0;

  /**
     Sets i-th vector component (0 < i < vector dimension).
  */
  virtual void setValue(int i, double x) = 0;

  /**
     Set attribute value to attr.
  */
  virtual void setValue(int dim, visAttribute *attr) = 0;

  /**
     Return scalar value of object.
  */
  virtual double getScalarValue(int dim) = 0;

  /**
     Return buffer to vector data for writing. Use takebackBuffer() to 
     write contents back to attribute.
  */
  double *lendBuffer(int dim);

  /** 
     Write vector data to attribute. The buffer must be obtained by
     lendBuffer!
  */
  void takebackBuffer(int dim, double *buffer);

  /**
     Returns true if this object has the same value as the given
     attribute.
   */
  virtual bool isEqual(int dim, visAttribute *) = 0;

  /**
     Returns true if this objects value is less than the given attribute.
   */
  virtual bool isLess(int dim, visAttribute *) = 0;

  /**
     Add given attribute value to this attribute. Dimension must be equal.
   */
  virtual void add(int dim, visAttribute *) = 0;

  /**
     Substract the given attribute from this attribute. Dimension must be
     equal. 
   */
  virtual void sub(int dim, visAttribute *) = 0;

  /**
     Multiply attribute value by the given scalar.
   */
  virtual void mult(int dim, double) = 0;

  /**
     Divide attribute value by the given scalar.
   */
  virtual void div(int dim, double) = 0;

};

/**
   @short Scalar attribute implementation.

   An Instance of this class holds one scalar attribute.
   
   @author Peter Breitling
   @version 03-24-98
   @see visScalarAttribute
*/
class visScalarAttribute : public visAttribute {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visScalarAttributeId) return true;
    return visAttribute::instanceOf(id);
  }
  visScalarAttribute();
  visScalarAttribute(double x);
  virtual ~visScalarAttribute();

  virtual bool allocate(double x);
  virtual void deallocate();

  virtual double getValue(int);
  virtual void setValue(int, visAttrVal val);
  virtual void setValue(int, double x);
  virtual void setValue(int dim, visAttribute *attr);
  virtual double getScalarValue(int dim);
  double *lendBuffer(int);
  void takebackBuffer(int, double *buffer);

  virtual bool isEqual(int dim, visAttribute *);
  virtual bool isLess(int dim, visAttribute *);
  virtual void add(int dim, visAttribute *);
  virtual void sub(int dim, visAttribute *);
  virtual void mult(int dim, double);
  virtual void div(int dim, double);
  
private:
  double myValue;
};

/**
   @short Vector attribute implementation.

   This class represents vector values. It's mapping to a scalar value is 
   the vector length.

   @author Peter Breitling
   @version 03-25-98
*/
class visVectorAttribute : public visAttribute {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visVectorAttributeId) return true;
    return visAttribute::instanceOf(id);
  }
  visVectorAttribute();
  visVectorAttribute(int dim, double* x);
  virtual ~visVectorAttribute();

  virtual bool allocate(int dim, double* x);
  virtual void deallocate();

  virtual double getValue(int);
  virtual void setValue(int dim, visAttrVal val);
  virtual void setValue(int i, double x);
  virtual void setValue(int dim, visAttribute *attr);
  virtual double getScalarValue(int dim);
  double *lendBuffer(int dim);
  void takebackBuffer(int dim, double *buffer);

  virtual bool isEqual(int dim, visAttribute *);
  virtual bool isLess(int dim, visAttribute *);
  virtual void add(int dim, visAttribute *);
  virtual void sub(int dim, visAttribute *);
  virtual void mult(int dim, double);
  virtual void div(int dim, double);
  
private:
  double *myValue;
};
 
/* -----------------------------------------------------------------------------
   Attribute Factory specification and implementations.
   -------------------------------------------------------------------------- */

/**
   @short Attribute factory specification.
   
   This class is used to instance new attributes of a specific type. It can be
   passed to other objects that need to create new attribute instances. It holds
   the dimension of the attribute. Special implementations exist for every
   attribute type.

   @author Peter Breitling
   @version 03-17-98
*/
class visAttributeFactory : public visObject {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttributeFactoryId) return true;
    return visObject::instanceOf(id);
  }
  /**
     Return vector dimension.
   */
  virtual int getDim() = 0;
  /**
     Returns new instance of attribute.
   */
  virtual visAttribute* getNewAttribute() = 0;
};

/**
   @short Scalar attribute factory implementation.

   This class implements the attribute factory for scalar attributes.

   @author Peter Breitling
   @version 03-17-98
*/
class visScalarAttributeFactory : public visAttributeFactory {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visScalarAttributeFactoryId) return true;
    return visAttributeFactory::instanceOf(id);
  }
  visScalarAttributeFactory();
  virtual ~visScalarAttributeFactory();
  virtual int getDim();
  virtual visAttribute* getNewAttribute();
};

/**
   @short Vector attribute factory implementation.

   This class implements the attribute factory for vector attributes.

   @author Peter Breitling
   @version 03-17-98
*/
class visVectorAttributeFactory : public visAttributeFactory {
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visVectorAttributeFactoryId) return true;
    return visAttributeFactory::instanceOf(id);
  }
  visVectorAttributeFactory(int dim);
  virtual ~visVectorAttributeFactory();
  virtual int getDim();
  virtual visAttribute* getNewAttribute();
private:
  int myDim;
};

/* -----------------------------------------------------------------------------
   Attribute Access specification and implementations.
   -------------------------------------------------------------------------- */

/**
   @short Attribute access specification.
   
   This class represents a simple vector of attributes. It is passed to
   attribute combiner classes and in general is implemented as a mapping to the
   internal storage of attributes.

   @author Peter Breitling 
   @version 03-17-98
*/
class visAttributeAccess : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttributeAccessId) return true;
    return visObject::instanceOf(id);
  }
  /**
     Return number of attributes.
   */
  virtual int getSize() = 0;
  /**
     Return number of attribute types.
   */
  virtual int getAttributes() = 0;
  /**
     Return n-th attribute of type t.
   */
  virtual visAttribute *getCellAttribute(int n, int t) = 0;
  /**
     Return attribute factory for type t.
   */
  virtual visAttributeFactory *getAttrFactory(int t) = 0;
};

/**
   @short Compare attribute access implementation.

   This class implements the attribute access for use with two attributes. It
   can be used along with an attribute combiner to compare two attributes.

   @author Peter Breitling
   @version 03-17-98
*/
class visCompareAttrAccess : public visAttributeAccess 
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCompareAttrAccessId) return true;
    return visAttributeAccess::instanceOf(id);
  }
  /**
     Initialize this class with fieldAccess and attribute
     val. The fieldAccess will be used to get an attribute factory for an
     attribute. 
  */
  visCompareAttrAccess(visAdaptiveGrid *fieldAccess, visAttrVal val);
  ~visCompareAttrAccess();
  virtual int getSize();
  virtual int getAttributes();
  virtual visAttribute *getCellAttribute(int n, int t);
  virtual visAttributeFactory *getAttrFactory(int t);
  /**
     Set second attribute.
   */
  void setCompareAttribute(visAttribute **attr);
private:
  visAttribute **myAttr;
  visAttribute **myCompareAttr;
  visAdaptiveGrid *myFieldAccess;
};

/* -----------------------------------------------------------------------------
   Attribute Combine specification and implementations.
   -------------------------------------------------------------------------- */

class visAttrCombiner;
typedef visStack<visAttrCombiner*> visAttrCombinerStack;

/**
   @short Attribute-Combiner specification.
   
   visAttrCombiner determines the attribute itself and its error semantic
   (e.g. used to determine if attribute can be combined dependend of an epsilon
   value).

   @author Peter Breitling
   @version 05-09-99 */
class visAttrCombiner : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttrCombinerId) return true;
    return visObject::instanceOf(id);
  }
  visAttrCombiner(visAttributeFactory *factory);
  virtual ~visAttrCombiner();
  
  /**
     Associate class with given attribute set.
  */
  virtual bool allocate(visAttribute *attr = NULL);
  virtual void deallocate();
  
  virtual void addAttrCombiner(visAttrCombiner *attrCombiner);
  virtual bool mayCombineAverage() = 0;
  virtual bool mayCombineRemove() = 0;
  virtual void combineAverage() = 0;
  virtual void combineRemove() = 0;
  
  /**
     Return combined attribute.
  */
  virtual visAttribute *getCombinedAttr() = 0;
  virtual visAttributeFactory *getAttrFactory();

protected:
  visAttribute *myAttr;
  visAttributeFactory *myAttrFactory;
  visAttrCombinerStack myAttrCombinerStack;
};
/**
   @short Attribute-Combine-Default implements Attribute-Combine.

   This is the default attribute combiner.

   @author Peter Breitling
   @version 05-09-99
*/
class visAttrCombinerDefault : public visAttrCombiner
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttrCombinerDefaultId) return true;
    return visAttrCombiner::instanceOf(id);
  }
  visAttrCombinerDefault(visAttributeFactory *factory);
  virtual ~visAttrCombinerDefault();

  virtual bool allocate(visAttribute *attr = NULL);
  virtual void addAttrCombiner(visAttrCombiner *attrCombiner);

  virtual bool mayCombineAverage();
  virtual bool mayCombineRemove();
  virtual void combineAverage() { };
  virtual void combineRemove() { };
  virtual visAttribute *getCombinedAttr(void);

private:
  bool myIsEqual;
};
#ifdef USUCKS
/**
   @short Attribute-Combiner-Epsilon implements Attribute-Combiner.

   This is an implementation based on an epsilon value for one selected
   attribute. The epsilon is a scalar value.

   @author Peter Breitling
   @version 03-20-98
*/
class visAttrCombinerEpsilon : public visAttrCombiner
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttrCombinerEpsilonId) return true;
    return visAttrCombiner::instanceOf(id);
  }
  visAttrCombinerEpsilon(visAttributeFactory *factory,
			 int attr, double epsilon,
			 visAttrCombCalculateByType calcBy,
			 visAttrCombApplyOnType applyOn);
  virtual ~visAttrCombinerEpsilon();
  
  virtual bool mayCombineAverage();
  virtual bool mayCombineRemove();
  virtual void combineAverage();
  virtual void combineRemove();
  
private:
  bool myIsEqual;
  double myEpsilon;
  visAttrCombCalculateByType myCalcBy;
  visAttrCombApplyOnType myApplyOn;
  bool myDoCombine;
  int myAttr;
};
#endif
class visAttrComb : public visObject
{
public:
  virtual bool instanceOf(visClassId id) {
    if (id == visAttrCombId) return true;
    return visObject::instanceOf(id);
  }
  visAttrComb();
  virtual ~visAttrComb();

  /**
     Associate class with given attribute set.
  */
  virtual bool allocate(visAttributeAccess *attrAccess);
  virtual void deallocate();

  /**
     Return associated attribute set.
  */
  virtual visAttributeAccess *getAttrAccess(void);

  /**
     True if attributes are equal according to specific rules.
  */
  virtual bool isEqual() = 0;

  /**
     Return average attribute calculated by specific algorithms.
  */
  virtual visAttribute *getAverageAttribute(int n) = 0;

protected:
  visAttributeAccess *myAttrAccess;
};

/**
   @short Attribute-Combine-Default implements Attribute-Combine.

   This is the default attribute combiner.

   @author Peter Breitling
   @version 03-20-98
*/
class visAttrCombDefault : public visAttrComb
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttrCombDefaultId) return true;
    return visAttrComb::instanceOf(id);
  }
  visAttrCombDefault();
  virtual ~visAttrCombDefault();

  virtual bool allocate(visAttributeAccess *attrAccess);
  virtual void deallocate();

  /**
     True if all attributes have the same value.
  */
  virtual bool isEqual();

  /**
     Return arithmetic average of all attributes.
  */
  virtual visAttribute *getAverageAttribute(int attr);

private:
  bool myIsEqual;
};

/**
   @short Attribute-Combine-Epsilon implements Attribute-Combine.

   This is an implementation based on an epsilon value for the indexed
   attribute. The epsilon is a scalar value.

   @author Peter Breitling
   @version 03-20-98
*/
class visAttrCombEpsilon : public visAttrComb
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttrCombEpsilonId) return true;
    return visAttrComb::instanceOf(id);
  }
  visAttrCombEpsilon(int attr, double epsilon,
		     visAttrCombCalculateByType calcBy,
		     visAttrCombApplyOnType applyOn);
  virtual ~visAttrCombEpsilon();

  virtual bool allocate(visAttributeAccess *attrAccess);
  virtual void deallocate();

  virtual bool isEqual();
  virtual visAttribute *getAverageAttribute(int attr);

private:
  bool myIsEqual;
  double myEpsilon;
  visAttrCombCalculateByType myCalcBy;
  visAttrCombApplyOnType myApplyOn;
  bool myDoCombine;
  int myAttr;
};


/* -----------------------------------------------------------------------------
   Attribute Enumeration 
   ---------------------------------------------------------------------------*/

/**
  @short Attribute-Enumeration specification.
   
   This class is the base class for an enumeration of all cells of the field
   associated field. This class can be applied on all fields. However it is
   better to use the getCellEnumeration() Method of the grid since it might
   return an optimized version for that field.

   @author Peter Breitling
   @version 03-20-98 */
class visAttributeEnumeration : public visObject 
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAttributeEnumerationId) return true;
    return visObject::instanceOf(id);
  }
  visAttributeEnumeration(visAdaptiveGrid *fieldAccess);
  virtual ~visAttributeEnumeration();

  virtual visAdaptiveGrid *getFieldAccess() = 0;
  virtual void setCursorFirstElement() = 0;
  virtual void setCursorNextElement() = 0;
  virtual bool isValidElement() = 0;
  virtual int getCellIndex() = 0;
  virtual int getNodeIndex(int n) = 0;
protected:
  visAdaptiveGrid *myFieldAccess;
  int myDim;
};

class visAdaptiveGridCellEnumeration : public visAttributeEnumeration
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAdaptiveGridCellEnumerationId) return true;
    return visAttributeEnumeration::instanceOf(id);
  }
  visAdaptiveGridCellEnumeration(visAdaptiveGrid *fieldAccess);
  virtual ~visAdaptiveGridCellEnumeration();

  virtual visAdaptiveGrid *getFieldAccess();
  virtual void setCursorFirstElement();
  virtual void setCursorNextElement();
  virtual bool isValidElement();
  virtual int getCellIndex();
  virtual int getNodeIndex(int n);
private:
  visStack<int> myIntStack;
  int *myMask, *myMaskEnd;
  int myMaxStackSize, myNumberOfSetCursorCalls;
  bool myIsValidElement;
  int myCellIndex;
};

/* -----------------------------------------------------------------------------
   Field Access specification and implementations.
   -------------------------------------------------------------------------- */

class visGrid;
class visCell;
class visCellRenderer;

class visCell : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCellId) return true;
    return visObject::instanceOf(id);
  }
  visCell() { };
  virtual ~visCell() = 0;

  virtual bool allocate(visGrid *grid) = 0;
  virtual void deallocate() = 0;

  /**
     Return position of node n, where n defines the node (see getCellNodes()).
     p is of dimension getGridDim().
  */
  virtual void getNodePos(int n, int *p) = 0;

  virtual void getNodeAttribute(int n) = 0;
  virtual void getCellAttribute(int n) = 0;
};

class visGrid : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visGridId) return true;
    return visObject::instanceOf(id);
  }

  static visGrid *readField(istream &in);
  bool writeField(ostream &out);

  visGrid();
  virtual ~visGrid();

  virtual bool allocate(int dim, int *size,
			int attrCount, visAttributeFactory** factory,
			visProperties *properties = NULL);

  virtual void deallocate();

  /**
     Return field dimension.
   */
  inline unsigned int getDim() { return myDim; };

  /**
     Return field size in dimension i.

     PRE: 0 <= i < this->getDim()
  */
  inline unsigned int getSize(unsigned int i) { return mySize[i]; };

  /**
     Get size of field in array d.

     PRE: d must be allocated as array of size this->getDim()
  */
  void getSize(int *d);

  /**
     Return number of attributes of this field. May be 0.
   */
  inline int getAttributes() { return myAttrCount; }

  /**
     Return number of cell types.
  */
  virtual int getCellTypes();

  /**
     Return cell dimension of cell type c.
  */
  virtual int getCellDim() = 0;

  /**
     Return number of nodes of cell type c.
  */
  virtual int getCellNodes() = 0;

  /**
     Set cursor to position p. 

     PRE: 0 <= p[i] < this->getSize(i), 0 <= i < this->getDim()
  */
  virtual void setCursor(const int *p) = 0;

  /**
     Get position of cursor.
     
     PRE: 0 <= i < this->getDim()
  */
  virtual int getCursor(unsigned int i) = 0;

  /**
     Get position of cursor in array p.
     
     PRE: p must be allocated as array of size this->getDim()
  */
  void getCursor(int *p);

  /**
     Return cell of current cursor.
  */
  //  virtual visCell* getCell() = 0;

  /**
     Return cell renderer
  */
  //virtual visCellRenderer* getCellRenderer() = 0;

  virtual const char *getAttrName(int t) = 0;
  virtual const visAttribute* getMinAttribute(int t) = 0;
  virtual const visAttribute* getMaxAttribute(int t) = 0;

  /**
     Return number of cells in the grid.
  */
  virtual int getCells() = 0;

  /**
     Return number of nodes in the grid.
   */
  virtual int getNodes() = 0;

  /*
  virtual int getAttrDim(int a) = 0;

  virtual getCellAttr() = 0;
  virtual getNodeAttr() = 0;
  */
  visAttributeFactory* getAttrFactory(int t);
  //void setAttributeFactory(int comp, visAttributeFactory* factory);
  
  /**
     Return attribute i of actual cursor pos.
   */
  virtual visAttribute* getCellAttribute(int t);

  /**
     Write attribute t to attr.
   */
  virtual void getCellAttribute(int t, visAttribute* attr) = 0;

  virtual visAttribute *getNodeAttribute(int t, int n);
  virtual void getNodeAttribute(int t, visAttribute *attr, int n);
  virtual void getNodePos(float *p, int n) = 0;

protected:
  int myDim;
  int *mySize;
  int myAttrCount;
  visAttributeFactory **myAttrFactory;
  visProperties *myProperties;
};

/**
 */
class visUnstructuredGrid : public visGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visUnstructuredGridId) return true;
    return visGrid::instanceOf(id);
  }
  visUnstructuredGrid();
  virtual ~visUnstructuredGrid();
  
  virtual bool allocate(int dim, int *size, int attrCount,
			visAttributeFactory** factory);
  virtual void deallocate();
  
  virtual int getCellDim() = 0;
  
  virtual int getCellNodes() = 0;
protected:
  visStack<float> myCoords;
  visStack<visAttribute*>* myData;
  visStack<int> myConn;
  int myCellDim;
  int myCellNodes;
};

/**
 */
class visStructuredGrid : public visGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visStructuredGridId) return true;
    return visGrid::instanceOf(id);
  }
  visStructuredGrid();
  virtual ~visStructuredGrid();
  
  virtual bool allocate(int dim, int *size,
			int attrCount, visAttributeFactory** factory);
  virtual void deallocate();
  
  virtual int getCellDim() { return myDim; }
  
  virtual int getCellNodes() { return 1 << myDim; }
  
  virtual int getNodes() { return (1 << myDim) * this->getCells(); }
  
  //virtual bool isUniform() = 0;
  //virtual bool isRectilinear() = 0;
};

/**
   @short Field-Access specification.
   
   This class is in the focus of interest in HiVis. It abstracts the data for
   its visualization. The field is rectilinear and its size in each dimension is
   known. For each point the position of the cell that contains that point can
   be polled. The cells in the field do not overlap and fill the field
   completely.

   @author Peter Breitling
   @version 03-20-98
*/
class visAdaptiveGrid : public visStructuredGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAdaptiveGridId) return true;
    return visStructuredGrid::instanceOf(id);
  }
  visAdaptiveGrid();
  virtual ~visAdaptiveGrid();

  virtual bool allocate(int dim, int *size,
			int attrCount, visAttributeFactory** factory);
  virtual void deallocate();

  virtual int getCellPos(unsigned int i) = 0;
  void getCellPos(int *p);

  virtual int getCellSize(unsigned int) = 0;
  void getCellSize(int *d);

  virtual void getNodePos(float *p, int n) { 
    int i;
    for (i = 0; i < myDim; i++) {
      p[i] = this->getCellPos(i);
      if (n & (1 << i)) {
	p[i] += this->getCellSize(i);
      }
    }
  }

  /**
     Get data association for this field
     (visHasNoData|visHasNodeData|visHasCellData).
  */
  virtual int getAttrAssoc() { return visHasNoData; }

  // [rn] getCellInterpAttribute
  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);
  // convenience functions
  virtual bool setCursorF(float *pos);
  virtual void getCellAttributeF(int comp, visAttribute *attr, float *pos);
  virtual bool setCursorN(int *pos);
  virtual void getAttributeN(int comp, visAttribute *attr);

private:
  // for setCursorF getCellAttributeF and getInterpAttribute
  int *myIntPos;
  int myNode;
  float *myFloatPos;
  float *myFloatPosBuf;
  visAttribute **myEdgeAttribute;
};

struct visAdaptiveGridCacheNode
{
  int *cellSize;
  visAdaptiveGridCacheNode **child;
};

struct visAdaptiveGridCacheLeaf : public visAdaptiveGridCacheNode
{
  visAttribute **val;
};

/**
   @short Adaptive-Grid-Cache-Access implements Field-Access.

   Completely cache adaptive grids for fast access.

   @author Peter Breitling
   @version 02-22-99
*/
class visAdaptiveGridCache : public visAdaptiveGrid {
  //friend class HiVis_HiVisMods_AG_Octree;
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAdaptiveGridCacheId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  visAdaptiveGridCache();
  virtual ~visAdaptiveGridCache();

  /**
     Allocate cache with given adaptive grid.
   */
  virtual bool allocate(visAdaptiveGrid *source);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);
  
  virtual void getCellAttribute(int, visAttribute*);
  virtual const char *getAttrName(int comp);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);
  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int i);
  virtual int getCells();
  virtual int getAttrAssoc() { return myAttrAssoc; }

private:
  int myAttrAssoc;

  // Standard field desciption

  char **myName;
  int *myCursor;
  int myCells;
  visAttribute **myMinAttribute, **myMaxAttribute;

  // buildTree "global" variables
  
  visAdaptiveGrid *myGrid;

  // Adjust min max flag

  bool myAdjustMinMax;

  // For intermediate calculations

};

/**
 */
class visMapAdaptiveGrid : public visUnstructuredGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visMapAdaptiveGridId) return true;
    return visGrid::instanceOf(id);
  }
  visMapAdaptiveGrid();
  virtual ~visMapAdaptiveGrid();
  
  virtual bool allocate(visAdaptiveGrid *adaptiveGrid,
			float ipad = 0.0f, int renderType = 0,
			visStack<float> *rangeMinMax = NULL, 
			int preferredAssoc = 0);
  virtual void deallocate();
  
  virtual int getCellDim() = 0;
  virtual int getCellNodes() = 0;
  virtual void setCursor(const int *p) = 0;
  virtual int getCursor(unsigned int i) = 0;
  virtual const char *getAttrName(int t) = 0;
  virtual const visAttribute* getMinAttribute(int t) = 0;
  virtual const visAttribute* getMaxAttribute(int t) = 0;
  virtual int getCells() = 0;
  virtual int getNodes() = 0;
  virtual void getCellAttribute(int t, visAttribute* attr) = 0;
  virtual void getNodeAttribute(int t, visAttribute *attr, int n) = 0;
  virtual void getNodePos(float *p, int n) = 0;
};

/**
   Return cell renderer
*/
class visCellRenderer : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCellRendererId) return true;
    return visObject::instanceOf(id);
  }
  visCellRenderer() { };
  virtual ~visCellRenderer() = 0;

  virtual bool allocate(visGrid *grid) = 0;
  virtual void deallocate() = 0;

//-----------------------------
// IAC modification
//-----------------------------
//  virtual renderCell(visCell *cell) = 0;
  virtual int renderCell(visCell *cell) = 0;
//-----------------------------
// end of IAC modification
//-----------------------------

};


/**
   @short Uniform-Access implements Field-Access.

   This is an implementation of field access by an array. This is the easiest
   and most run-time efficient implementation but also the most memory
   consuming.

   @author Peter Breitling
   @version 03-20-98
*/
class visUniformGrid : public visAdaptiveGrid {
  friend class HiVis_HiVisMods_Field_To_HiVis;
public:
   virtual bool instanceOf(visClassId id) { 
    if (id == visUniformGridId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
 visUniformGrid();
  virtual ~visUniformGrid();
  
  virtual bool allocate(FLD_Grid_Struct *grid_struct,
			FLD_Node_Data *node_data,
			FLD_Cell_Data *cell_data,
			int attrAssoc);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);
  virtual void getCellAttribute(int, visAttribute*);
  virtual void getNodeAttribute(int a, visAttribute *attr, int n);
  virtual const char *getAttrName(int comp);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);
  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);

  virtual int getCells();
  virtual int getNodes();
  
  virtual int getAttrAssoc() { return myAttrAssoc; }
private:
  int *myOffset;
  int *myCursor;
  int myIndex, *myMaxIndex;
  int myCells, myNodes;
  char **myName;
  double **myData;
  visAttribute **myMinAttribute, **myMaxAttribute;
  int myAttrAssoc;
};

class visOctreeAttrAccess;
struct visOctreeNode;
struct visCursorPathElement;

/**
   @short Octree-Access implements Field-Access.

   This is an implementation of field access by an octree. 

   @author Peter Breitling
   @version 03-20-98
*/
class visOctree : public visAdaptiveGrid {
  friend class HiVis_HiVisMods_HiVis_Read_Octree;
  friend class HiVis_HiVisMods_AG_Octree;
  friend class visOctreeAttrAccess;
  friend struct visOctreeNode;
  friend struct visCursorPathElement;
public:
  virtual bool instanceOf(visClassId id) { 
    visMsgDbg << "c3" << visMsgEnd;
    if (id == visOctreeId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  visOctree();
  virtual ~visOctree();

  /**
     Allocate Octree with another Field. AttrComb is used as a criteria for
     merging nodes and the build tree won't have a height greater than
     maxlevel (root node is defined to have level = 0!).
   */
  virtual bool allocate(visAdaptiveGrid *source,
			visAttrComb* attrComb,
			int maxlevel);
  /**
     Allocate Octree with given stream. The stream in general is a file stream
     and the format is described in http://??? .
   */
  virtual bool allocate(istream &in);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);
  
  virtual visAttribute* getCellAttribute(int comp);
  virtual void getCellAttribute(int, visAttribute*);
  virtual const char *getAttrName(int comp);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);
  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int i);
  virtual int getCells();
  virtual int getAttrAssoc() { return visHasCellData; }

private:

  void dumpTree(visOctreeNode *node, int level, int *pos);
  int buildTree(visOctreeNode *node, istream &in, int level,
		const int dim, const int attributeCount,
		visAttributeFactory** factory);
  int buildTree(visOctreeNode *node, istream &header, istream &data,
		int level, const int dim, const int attributeCount,
		visAttributeFactory** factory);
  bool buildTree();
  void deleteTree(visOctreeNode *node, int level);
  void initNumberOfCells(visOctreeNode *node, int level);
  bool isValidPos(int *pos);

  // Standard field desciption

  char **myName;
  int *myCells;
  int *myCursor;
  visAttribute **myMinAttribute, **myMaxAttribute;

  // Extended field desciption

  visOctreeNode *myOctreeRoot;
  int myVirtualSize;
  int myMaxLevel;

  // Cursor related information

  bool myIsValidCursor;
  int myCursorLevel;
  visCursorPathElement *myCursorPath;

  int myMaxVirtualLevel;

  // buildTree "global" variables
  
  visAdaptiveGrid *myFieldAccess;

  // Attribute Combiner

  visAttrComb *myAttrComb;

  // Adjust min max flag

  bool myAdjustMinMax;

  // For intermediate calculations

};

struct visOctreeNode {
  visOctreeNode(int dim, int attributeCount)
    { 
      child = new visOctreeNode*[1 << dim];
      attribute = new visAttribute*[attributeCount];
    };
  ~visOctreeNode() {
    if (child) {
      delete[] child;
    }
    if (attribute) {
      delete[] attribute;
    }
  }
  
  // Attributes - array is of size attributeCount
  visAttribute **attribute;
  
  // Childs - array is of size (1 << field dimension) - may have NULL entries
  visOctreeNode **child;
};

struct visCursorPathElement {
  visCursorPathElement() : node(NULL), pos(NULL) { }
  ~visCursorPathElement() {
    if (pos) delete[] pos;
  }
  
  void init(int dim) {
    pos = new int[dim];
    int i;
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
  }
  
  visOctreeNode *node;
#ifdef NEWCOMB
  visAttrCombiner **attrCombiner;
#endif
  int *pos;
};

class visOctreeAttrAccess : public visAttributeAccess 
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visOctreeAttrAccessId) return true;
    return visAttributeAccess::instanceOf(id);
  }
  visOctreeAttrAccess(visOctreeNode *octreeNode,
		      visOctree *octreeAccess) :
    myOctreeNode(octreeNode), myOctree(octreeAccess)
    { 
      myAttributeCount = octreeAccess->getAttributes();
      mySize = 1 << octreeAccess->getDim();
  }
  ~visOctreeAttrAccess() {
  }
  virtual int getSize() {
    return mySize;
  }
  virtual int getAttributes() {
    return myAttributeCount;
  }
  virtual visAttribute *getCellAttribute(int n, int attr) {
    if (n < 0 || n >= mySize) {
      visMsgError << "visAttribute::getCellAttribute(int, int)"
		  << ": 1. parameter out of bounds"
		  << visMsgErrorEnd;
      return NULL;
    }
    if (attr < 0 || attr >= myAttributeCount) {
      visMsgError << "visAttribute::getCellAttribute(int, int)"
		  << "2. parameter out of bounds"
		  << visMsgErrorEnd;
      return NULL;
    }
    if (myOctreeNode->child && myOctreeNode->child[n]) {
      visAttribute *p = myOctreeNode->child[n]->attribute[attr];
      if (p) { p->incRef(); }
      return p;
    }
    return NULL;
  }
  virtual visAttributeFactory *getAttrFactory(int comp) {
    return myOctree->getAttrFactory(comp);
  }
private:
  visOctreeNode *myOctreeNode;
  visOctree *myOctree;
  int myAttributeCount, mySize;
};

struct visSparseGridFactorInfo {
  int *pos;
  float factor;
};

struct visSparseGridNode {
  visSparseGridNode *left, *right;
};

struct visSparseGridStructure : public visSparseGridNode {
  visSparseGridNode *next;
};

struct visSparseGridVal : public visSparseGridNode {
  visAttribute **attr;
};

/**
   @short Convert cell associated grid to node associated grid

   @author Peter Breitling
   @version 03-20-98
*/
class visCell2Node : public visAdaptiveGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCell2NodeId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  virtual bool allocate(visAdaptiveGrid* fieldAccess) = 0;
  virtual bool allocate(int dim, int *size,
			int attrCount, visAttributeFactory** factory);
  virtual void deallocate() = 0;
};

/**
   @short Convert node associated grid to cell associated grid

   @author Peter Breitling
   @version 03-20-98
*/
class visNode2Cell : public visAdaptiveGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visNode2CellId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  virtual bool allocate(visAdaptiveGrid* fieldAccess) = 0;
  virtual bool allocate(int dim, int *size,
			int attrCount, visAttributeFactory** factory);
  virtual void deallocate() = 0;
private:
};

/**
   @short SparseGrid-Access implements Field-Access.

   This is an implementation of field access by a sparse-grid. 

   @author Peter Breitling
   @version 03-20-98
*/
class visSparseGrid : public visAdaptiveGrid {
  friend class HiVis_HiVisMods_AG_SparseGrid;
  friend class HiVis_HiVisMods_HiVis_Read_SparseGrid;
  friend class HiVis_HiVisMods_HiVis_Render_SparseGrid;
  friend class visSparseGridCellInterp;
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visSparseGridId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  visSparseGrid();
  virtual ~visSparseGrid();

  virtual bool allocate(visAdaptiveGrid *source, visAttrComb *attrComb, 
			int maxlevel, int maxLdScopeDiff);
  virtual bool allocate(istream &in, visAttrComb *attrComb, int maxlevel,
			int maxLdScopeDiff);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);
  
  virtual visAttribute* getCellAttribute(int comp);
  virtual void getCellAttribute(int, visAttribute*);
  virtual const char *getAttrName(int comp);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);
  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int i);
  virtual int getCells();
  virtual int getNodes();

  virtual void getNodeAttribute(int comp, visAttribute *attr, int edge);
  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);

  virtual int getAttrAssoc() { return visHasNodeData; }
private:
  int getSparseGridSizeAndDim(visSparseGridNode *&node,
			      istream &in, int dim, int level);
  void buildTree(visSparseGridNode *&node, int index, int dim);
  void deleteTree(visSparseGridNode *&node, int dim);
  void getCellBounds(visSparseGridNode *node, int level, int dim);
  void evaluate(visSparseGridNode *node, int level, int dim);
  void adjustMinMax(visSparseGridNode *node, int level, int dim);
  inline int visGetScopeByPos(int pos, int dim);
  inline void getNodeAttributes();
  inline bool isValidPos(int *pos);
  /*
  void rGetAttribute(int dim, float fac);
  void rGetAttribute(int dim);
  void rGetAttribute(visSparseGridNode *node, int treeLevel);
  inline int getSubSpaceDeltaByPos(int pos, int dim); 
  inline int visGetSubLevelByPos(int pos, int dim);
  inline int getLevelByPos(int *pos);
  void buildTree(visSparseGridNode *&node, int *pos, int *size);
  void dumpTree(bool dumpVirtual);
  void dumpTree(visSparseGridNode *node, int level, bool dumpVirtual);
  void deleteTree(visSparseGridNode *node);
  */

  /**
   */
  visStack<int> myMaxLevelStack;

  // Node data options

  bool myIsScopeClipped;

  // Buildtree globals

  visAdaptiveGrid *myFieldAccess;
  
  // Standard field desciption

  char **myName;
  visAttribute **myMinAttribute, **myMaxAttribute;

  // myLdSize[i] = log2(this->getSize(i) - 1); 0 <= i < this->getDim()

  int *myLdSize;
  int *myLdScope;
  int *myPos;
  int *myEvalLdScope;
  int *myEvalPos;

  // Cursor information

  int *myCursorPos;
  int *myCursorScope;
  int *myCursorBuf;

  int *myCellPos1;
  int *myCellPos2;

  // Extended field desciption

  int myCells;
  int myMaxLevel;
  int myLevel;

  visCompareAttrAccess *myAttrCompare;

  // Data storage

  visSparseGridNode *myRootNode;

  // For intermediate calculations

  int myFactors;
  visSparseGridFactorInfo *myFactorInfo;
  visAttribute **myResAttribute;
  visAttribute **myAttributeBuffer;
  int myAttrValid;
  visAttribute ***myEdgeAttribute;
  float *myFloatPosBuf;
  
  // Statistics

  int myNodesMax;
  int myNodes;
  
  // percentage temporaries

  float myNode;

  // Attribute Combiner

  visAttrComb *myAttrComb;

  // Cell bounds calculation parameters

  int myMaxLdScopeDiff;
};

/**
   @short Orthoslice of an adaptive field (reduction of one dimension)

   Cut a slice out of the given field at a given axis and position. The
   resulting field has one dimension less than the original field.  It can be
   called a proxy field, since most of the methods call the corresponding
   methods of the associated source field and the object can't be used
   independently of the source field.

   @author Peter Breitling
   @version 03-20-98
*/

class visAdaptiveGridSlice : public visAdaptiveGrid
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visAdaptiveGridSliceId) return true;
    return visAdaptiveGrid::instanceOf(id);
  }
  visAdaptiveGridSlice();
  virtual ~visAdaptiveGridSlice();
  
  virtual bool allocate(visAdaptiveGrid *fieldAccess, int axis, float pos,
			bool adjustMinMax);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);

  virtual visAttribute* getCellAttribute(int i);
  virtual void getCellAttribute(int, visAttribute*);
  virtual visAttribute *getNodeAttribute(int t, int n);
  virtual void getNodeAttribute(int t, visAttribute *attr, int n);

  virtual const char *getAttrName(int i);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);
  
  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int i);

  virtual int getCells();

  virtual int getAttrAssoc() { 
    if (this->isAllocated()) return myFieldAccess->getAttrAssoc();
    return visHasNoData;
  }

private:
  void iterateThroughSlice(int *pos);

  visAdaptiveGrid *myFieldAccess;
  int myAxis;
  int myPos;
  int myCells;
  visAttribute **myMinAttribute, **myMaxAttribute;
  int myFieldAttributeCount;
};

/**
   @short Cell-Interpolate-Direct implements Cell-Interpolate.

   @author Peter Breitling
   @version 03-20-98
*/
class visCell2NodeDirect : public visCell2Node
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCell2NodeDirectId) return true;
    return visCell2Node::instanceOf(id);
  }
  visCell2NodeDirect();
  virtual ~visCell2NodeDirect();

  virtual bool allocate(visAdaptiveGrid* fieldAccess);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);

  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);

  virtual visAttribute* getCellAttribute(int);
  virtual void getCellAttribute(int, visAttribute*);

  virtual const char *getAttrName(int);
  virtual const visAttribute* getMinAttribute(int);
  virtual const visAttribute* getMaxAttribute(int);

  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);
  virtual int getCells();

  virtual int getAttrAssoc() { return visHasNodeData; }

protected:
  visAdaptiveGrid *myFieldAccess;
};

/**
   @short Cell-Interpolate-Block implements Cell-Interpolate.

   @author Peter Breitling
   @version 03-20-98
*/
class visCell2NodeBlock : public visCell2Node
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCell2NodeBlockId) return true;
    return visCell2Node::instanceOf(id);
  }
  visCell2NodeBlock();
  visCell2NodeBlock(visAdaptiveGrid* fieldAccess);
  virtual ~visCell2NodeBlock();

  virtual bool allocate(visAdaptiveGrid* fieldAccess);
  virtual void deallocate();
  
  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);

  virtual void getNodeAttribute(int comp, visAttribute *attr, int edge);
  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);

  virtual visAttribute* getCellAttribute(int);
  virtual void getCellAttribute(int, visAttribute*);

  virtual const char *getAttrName(int);
  virtual const visAttribute* getMinAttribute(int);
  virtual const visAttribute* getMaxAttribute(int);

  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);
  virtual int getCells();

  virtual int getAttrAssoc() { return visHasNodeData; }

private:
  visAdaptiveGrid *myFieldAccess;
  int myCells;
  int *myCellPos;
  int *myCellSize;
  int *myAttrDim;
  int *myCursorBuf, *myCellPosBuf, *myCellSizeBuf;
  float *myFloatPosBuf;
  visAttribute*** myEdgeAttribute;
  bool myHasEqualEdges;
};

/**
   @short Cell-Interpolate-OLinear implements Cell-Interpolate.

   @author Peter Breitling
   @version 03-20-98
*/
class visCell2NodeOLinear : public visCell2Node
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCell2NodeOLinearId) return true;
    return visCell2Node::instanceOf(id);
  }
  visCell2NodeOLinear();
  visCell2NodeOLinear(visAdaptiveGrid* fieldAccess);
  virtual ~visCell2NodeOLinear();

  virtual bool allocate(visAdaptiveGrid* fieldAccess);
  virtual void deallocate();
  
  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);

  virtual void getNodeAttribute(int comp, visAttribute *attr, int edge);
  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);

  virtual visAttribute* getCellAttribute(int);
  virtual void getCellAttribute(int, visAttribute*);

  virtual const char *getAttrName(int);
  virtual const visAttribute* getMinAttribute(int);
  virtual const visAttribute* getMaxAttribute(int);

  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);
  virtual int getCells();

  virtual int getAttrAssoc() { return visHasNodeData; }

private:
  void deallocateCache();
  void getLinearInterpValue(const int *pos,
			    visAttribute **resAttr);
  visAdaptiveGrid *myFieldAccess;
  int myCells;
  visAttribute **myAttribute;
  int *myCellPos;
  int *myAttrDim;
  visAttribute** myAttributeBuffer;

  visAttribute**** myCache;
  int* myCacheSize;
  bool myIsValidCache;
  int *myFieldCellPos;
  int *myFieldCellSize;
  float *myFloatPosBuf;
};

/**
   @short Cell-Interpolate-MLinear implements Cell-Interpolate.

   @author Peter Breitling
   @version 03-20-98
*/
class visCell2NodeMLinear : public visCell2Node
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCell2NodeMLinearId) return true;
    return visCell2Node::instanceOf(id);
  }
  visCell2NodeMLinear();
  visCell2NodeMLinear(visAdaptiveGrid* fieldAccess);
  virtual ~visCell2NodeMLinear();

  virtual bool allocate(visAdaptiveGrid* fieldAccess);
  virtual void deallocate();
  
  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);

  virtual void getNodeAttribute(int comp, visAttribute *attr, int edge);
  virtual void getInterpAttribute(int comp, visAttribute *attr, float *pos);

  virtual visAttribute* getCellAttribute(int);
  virtual void getCellAttribute(int, visAttribute*);

  virtual const char *getAttrName(int);
  virtual const visAttribute* getMinAttribute(int);
  virtual const visAttribute* getMaxAttribute(int);

  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);
  virtual int getCells();

  virtual int getAttrAssoc() { return visHasNodeData; }

private:
  void deallocateCache();
  void getLinearInterpValue(int interpDim, int *interp,
			    const int *pos,
			    visAttribute **resAttr, bool mayCache);

  visAdaptiveGrid *myFieldAccess;
  
  int myCells;
  visAttribute **myAttribute;
  int *myCellPos;
  int *myAttrDim;
  visAttribute** myAttributeBuffer;

  // "First level" cache

  visAttribute*** myEdgeAttributeCache;

  // "Second level" cache

  visAttribute**** myCache;
  int* myCacheSize;
  bool myIsValidCache;
  int *myFieldCellPos;
  int *myFieldCellSize;
  float *myFloatPosBuf;

  // Space for intermediate calculations of getLinearInterpValue

  int *myIFieldCellPos, *myIFieldCellSize;
  visAttribute ***myIAttr;
  float *myIMult;
  int *myINewPos, *myINewInterp;

public:
  /// for profiling
  int myGetLinearInterpValueCalls;
  int myGetLinearInterpValueCachedCalls;
  int myGetEdgeAttributeCalls;
  int myGetEdgeAttributeCachedCalls;
  int mySetCursorCalls;
  int mySetCursorCachedCalls;

};

#ifdef OBS
/**
   @short Streamline-Euler implements Field-Access.

   @author Peter Breitling
   @version 03-20-98
*/
/*
class visStreamLineEuler : public visAdaptiveGrid {

public:
  visStreamLineEuler();
  virtual ~visStreamLineEuler();
  
  virtual bool allocate(visAdaptiveGrid *fieldAccess, 
			visCell2Node *cellInterp,
			int vectorAttribute,
			float *probePos,
			int direction,
			int stepsPerCell,
			int maxSteps);
  virtual void deallocate();

  virtual void setCursor(const int *p);
  virtual int getCursor(unsigned int i);
  virtual void getCellAttribute(int, visAttribute*);
  virtual const char *getAttrName(int comp);
  virtual const visAttribute* getMinAttribute(int comp);
  virtual const visAttribute* getMaxAttribute(int comp);

  virtual int getCellPos(unsigned int i);
  virtual int getCellSize(unsigned int);
  virtual int getCells();
private:

  enum visStreamLineAttrType {
    visStreamLineAttrPosition = 0,
    visStreamLineAttrDirection,
    visStreamLineAttrNormal,
    visStreamLineAttrTypes
  };
  
  int myCells;
  char **myName;
  visAttribute **myMinAttribute, **myMaxAttribute;

  visStack<visAttribute*> *myStreamAttribute;
  visStack<float> *myStreamCoord;
  visStack<float> *myStreamNormal;
  visStack<float> *myStreamDir;   


  visAdaptiveGrid *myFieldAccess;
  visAdaptiveGrid *myCellInterp;
  int myVectorAttribute;
  int myMaxSteps;
  int myVectorDim;

  int myCursor;

  // temporaries

  float *myPosA;
  float *myPosB;
};
*/
/*

Point Index:
    0 1 2 3 4 5 6 7
X:  0 1 0 1 0 1 0 1
Y:  0 0 1 1 0 0 1 1
Z:  0 0 0 0 1 1 1 1

Vertex Index:
    0 1 2 3 4 5 6 7 8 9 10 11
P1: 0 0 0 1 1 2 2 3 4 4  5  6
P2: 1 2 4 3 5 3 6 7 5 6  7  7

00000000 -1
10000000 

class visIsoSurface : public visObject 
{
public:
  visIsoSurface() { }
  virtual ~visIsoSurface() { }
  
  allocate(visAdaptiveGrid *fieldAccess, float isoVal,
	   HiVis_HiVisMods_HiVis_Isosurface_out) {
  }

  deallocate(
  
private:

  void tagIsoLines(int *mask, int *maskEnd) {
    
    int i;
    for (i = 0; i < myFieldAccess->getDim(); i++) {
      if (mask[i] >= maskEnd[i]) { 
	return;
      }
    }
    
    myFieldAccess->setCursor(mask);
    
    bool isValid = true;
    for (i = 0; i < myFieldAccess->getDim(); i++) {
      if (myFieldAccess->getCellPos(i) < mask[i]) { 
	isValid = false;
	break;
      }
    }
    if (isValid) { 
      // dump
      myEnumerationCount++;
    }
    
    int *currentCellEnd = new int[myFieldAccess->getDim()];
    int *newMask = new int[myFieldAccess->getDim()];
    int *newMaskEnd = new int[myFieldAccess->getDim()];

    for (i = 0; i < myFieldAccess->getDim(); i++) {
      currentCellEnd[i] = 
	myFieldAccess->getCellPos(i) + myFieldAccess->getCellSize(i);
    }    
    
    int axis;
    for (axis = 0; axis < myFieldAccess->getDim(); axis++) {
      for (i = 0; i < myFieldAccess->getDim(); i++) {
	if (i == axis) {
	  newMask[axis] = currentCellEnd[axis];
	} else {
	  newMask[i] = mask[i];
	}
      }
      for (i = 0; i < axis + 1; i++) {
	newMaskEnd[i] = maskEnd[i];
      }
      for (i = axis + 1; i < myFieldAccess->getDim(); i++) {
	newMaskEnd[i] =
	  currentCellEnd[i] < maskEnd[i] ? currentCellEnd[i] : maskEnd[i];
      }
      enumerate(newMask, newMaskEnd);
    }
    
    delete[] newMask;
    delete[] newMaskEnd;
    delete[] currentCellEnd;
  }


  float myIsoVal;
};

*/





  /*
    visMsgCall << "mask = (" 
		       << mask[0] << "," << mask[1] << "," << mask[2] << ") "
		       << "maskEnd = (" 
		       << maskEnd[0] << "," << maskEnd[1] << "," << maskEnd[2] 
		       << ")"
		       << visMsgEnd;

		       */
    



  /*  void enumerateRec(int *mask, int *maskEnd) {
    
    int i;
    for (i = 0; i < myFieldAccess->getDim(); i++) {
      if (mask[i] >= maskEnd[i]) {
	return;
      }
    }
    
    myFieldAccess->setCursor(mask);
    
    bool isValid = true;
    for (i = 0; i < myFieldAccess->getDim(); i++) {
      if (myFieldAccess->getCellPos(i) < mask[i]) {
	isValid = false;
	break;
      }
    }
    if (isValid) {
      // dump
      myEnumerationCount++;
    }
    
    int *currentCellEnd = new int[myFieldAccess->getDim()];
    int *newMask = new int[myFieldAccess->getDim()];
    int *newMaskEnd = new int[myFieldAccess->getDim()];

    for (i = 0; i < myFieldAccess->getDim(); i++) {
      currentCellEnd[i] = 
	myFieldAccess->getCellPos(i) + myFieldAccess->getCellSize(i);
    }    
    
    int axis;
    for (axis = 0; axis < myFieldAccess->getDim(); axis++) {
      for (i = 0; i < myFieldAccess->getDim(); i++) {
	if (i == axis) {
	  newMask[axis] = currentCellEnd[axis];
	} else {
	  newMask[i] = mask[i];
	}
      }
      for (i = 0; i < axis + 1; i++) {
	newMaskEnd[i] = maskEnd[i];
      }
      for (i = axis + 1; i < myFieldAccess->getDim(); i++) {
	newMaskEnd[i] =
	  currentCellEnd[i] < maskEnd[i] ? currentCellEnd[i] : maskEnd[i];
      }
      enumerate(newMask, newMaskEnd);
    }
    
    delete[] newMask;
    delete[] newMaskEnd;
    delete[] currentCellEnd;
  }
  */
#endif
#endif

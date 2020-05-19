// Technische Universität München - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

// Ansi C

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

// Ansi C++

#include <iostream.h>

#ifdef _MSC_VER
#include <strstrea.h>
#else
#include <strstream.h>
#include <stream.h>
#endif

// AVS/Express

#include "AVS/avs.h"

// Basic definitions

#ifndef _MSC_VER
//#define bool char
//typedef unsigned char bool;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

// Message definitions

#define visDoMsgDbg true
#define visDoMsgCall true
#define visDoMsgError true
#define visDoMsgProfile true
#define visDoCheckAssert true

//#define visMsgLevel visMsgCall
//#define visMsg(level) \
//if (visMsgLevel >= level) { strstream inst; inst 
//#define visMsg(level) \
//if (visMsgLevel >= level) { fout
//enum visMsgLevels { visMsgNone = 0, 
//		    visMsgError, 
//		    visMsgWarning,
//		    visMsgCall };
//

#define visMsgDbg if (visDoMsgDbg && visSYSTEM.out) { *visSYSTEM.out
#define visMsgCall if (visDoMsgCall && visSYSTEM.out) { *visSYSTEM.out
#define visMsgFatal if (visDoMsgCall && visSYSTEM.out) { *visSYSTEM.out
#define visMsgFatalEnd endl; exit(1); }

#define visMsgError { strstream inst; inst 
#define visMsgErrorEnd endl << ends; \
 ERRverror("",ERR_NO_HEADER | ERR_PRINT, inst.str());\
if (visSYSTEM.out) { *visSYSTEM.out << inst.str();}\
}
//#define visMsgError if (visDoMsgError) { 
//#define visMsgErrorEnd \
//endl << ends; ERRverror("",ERR_NO_HEADER | ERR_PRINT, inst.str()); }
#define visMsgProfile if (visDoMsgProfile && visSYSTEM.out) { *visSYSTEM.out
#define visMsgEnd endl; }

#define visStatOk          0x00
#define visStatInterrupt   0x01

#define visField1D ((unsigned int)1)
#define visField2D ((unsigned int)2)
#define visField3D ((unsigned int)3)
#define visX ((unsigned int)0)
#define visY ((unsigned int)1)
#define visZ ((unsigned int)2)

#if visDoCheckAssert == true
#define visAssert(x) if (!(x) && visSYSTEM.out) { \
     *visSYSTEM.out << "HiVis: assertion failed in " \
           << __FILE__ << ":" << __LINE__ << " - please contact author!" \
           << endl; \
  }
#else
#define visAssert(COND)
#endif

// Stream function definitions

#ifdef _MSC_VER
#define visEATWHITE(x) x ## .eatwhite()
#else
#define visEATWHITE(x) eatwhite( ## x ## )
#endif

typedef double visGTATTR;
typedef int visSGTPOS;
typedef int visSGTSIZE;

//bool visEATBLANK(istream &in);
char visTRY(istream &in, char c);
bool visREADTOKEN(istream &in, int (*m)(int), char* s = NULL, int length = 0);
int visIsBlank(int c);

/**
   @short VisApi system interaction.

   Global system interaction including output streams (to be used instead of
   stdout/stderr), file io, progress indicators, etc. This class is used as a
   singleton and is globaly instantiated as visSYSTEM.

   @author Peter Breitling
   @version 04-28-98 
*/

class visSystem
{
public:
  visSystem();
  ~visSystem();

  void setStatus(char *msg, float progress);
  void setStatus(char *msg);
  int getStatus();

  /**
     Output stream for debug and error messages. Use the macros visMsgDbg,
     visMsgCall, visMsgError and visMsgFatal instead of this stream directly.
  */
  ostream *out;
private:
  clock_t myLastUpdateTime;
  float myMinUpdatesDelay;
  char *myLastMsg;
  float myLastProgress;
  int myStatus;
};

extern visSystem visSYSTEM;

#define visSETSTATUSIDLE visSYSTEM.setStatus("<idle>", 1.0f);

/**
   @short Attribute constants.
   
   These constants can be used to set the value of an attribute to a
   constant. The real values depend on the attribute specific operators
   implementation.
   
   @author Peter Breitling
   @version 05-11-98
*/

enum visAttrVal
{
  visAttrAddNeutral,
  visAttrMultNeutral,
  visAttrMin,
  visAttrMax
};

enum visAttrType
{
  visAttrTypeScalar, 
  visAttrTypeVector
};

enum visClassId
{
  visAttrCombId,
  visAttrCombDefaultId,
  visAttrCombEpsilonId,  
  visAttrCombinerId,
  visAttrCombinerDefaultId,
  visAttrCombinerEpsilonId,  
  visAttributeId,
  visScalarAttributeId,
  visVectorAttributeId,  
  visAttributeAccessId,
  visCompareAttrAccessId,
  visOctreeAttrAccessId,
  visAttributeEnumerationId,
  visAdaptiveGridCellEnumerationId,
  visAttributeFactoryId,
  visScalarAttributeFactoryId,
  visVectorAttributeFactoryId,
  visCSetId,
  visCellId,
  visCellRendererId,
  visPropertiesId,
  visGridId,
  visStructuredGridId,
  visAdaptiveGridId,
  visAdaptiveGridCacheId,
  visAdaptiveGridSliceId,
  visMapAdaptiveGridId,
  visCell2NodeId,
  visCell2NodeBlockId,
  visCell2NodeDirectId,
  visCell2NodeMLinearId,
  visCell2NodeOLinearId,
  visNode2CellId,
  visOctreeId,
  visSparseGridId,
  visUniformGridId,
  visUnstructuredGridId,
  visStackId,
  visObjectId,
  visClasses
};

enum visFieldType
{
  visTypeAnyGrid,
  visTypeUnstructuredGrid,
  visTypeStructuredGrid,
  visTypeAdaptiveGrid,
  visTypeOctreeAccess, 
  visTypeSparseGridAccess, 
  visTypeSparseGridCellInterp, 
  visTypeUniformAccess,
  visTypeFieldSliceAccess,
  visTypeRenderAccessDirect,
  visTypeRenderAccessBlock,
  visTypeLinearInterpAccess,
  visTypeMLinearInterpAccess,
  visTypeStreamLineEuler
};

#define visHasNoData   (0)
#define visHasNodeData (1<<0)
#define visHasCellData (1<<1)

enum visCell2NodeType
{
  visCell2NodeTypeBlock = 0,
  visCell2NodeTypeOLinear,
  visCell2NodeTypeMLinear,
  visCell2NodeTypes
};

extern char* visCell2NodeStrings[visCell2NodeTypes];

enum visAttrCombType
{
  visAttrCombTypeDefault = 0,
  visAttrCombTypeEpsilon,
  visAttrCombTypes
};

// not optimal but sure to be in bounds and ok for now

/*
#define visMAXINT   (1<<30)
#define visMININT   (-(1<<30))
#define visMAXFLOAT (1e31f)
#define visMINFLOAT (-1e31f)
#define visMAXDOUBLE (1e31f)
#define visMINDOUBLE (-1e31f)
*/
#define visMAXINT    (INT_MAX)
#define visMININT    (INT_MIN)
#define visMAXFLOAT  (FLT_MAX)
#define visMINFLOAT  (1-FLT_MAX)
#define visMAXDOUBLE (DBL_MAX)
#define visMINDOUBLE (1-DBL_MAX)

typedef int visAttrCombCalculateByType;
#define visAttrCombCalculateByScalarDiff (0)
#define visAttrCombCalculateByVectorDiff (1)
#define visAttrCombCalculateByTypes      (2)

typedef int visAttrCombApplyOnType;
#define visAttrCombApplyOnMaxDiff  (0)
#define visAttrCombApplyOnAvgDiff  (1)
#define visAttrCombApplyOnTypes    (2)

class visAttribute;
class visAttributeFactory;
class visAttributeAccess;
class visAttrComb;
class visAttrInterp;
class visCell2Node;
class visNode2Cell;

/**
   @short Base class for all objects in the VisApi. 
   
   Specifies the basic functionality of objects in the VisApi: A reference
   counter to be able to provide cleaner interfaces in cases where instances of
   objects are returned. Allocation/Deallocation specification - an extension to
   the simple construction, destruction scheme where objects can be instanciated
   without being initialized (allocated).

   @author Peter Breitling
   @version 03-17-98
*/

class visObject 
{
public:
  /**
     virtual bool instanceOf(visClassId id) { 
       if (id == Id) return true;
       return ::instanceOf(id);
     }
   */

  virtual bool instanceOf(visClassId id) { 
    if (id == visObjectId) return true;
    return false;
  }

  /**
    Constructor sets reference count to one and allocated to false.
  */
  visObject();
  /**
    Destructor.
  */
  virtual ~visObject();
  /**
    Allocate object. Parameters may be added in sub classes.
   */
  virtual bool allocate();
  /**
     Deallocate object and free resources. Is internally called by the
     destructor and reallocations.
  */
  virtual void deallocate();
  /**
     Returns true if object is allocated with the allocate() method.
  */
  virtual inline bool isAllocated() { return myIsAllocated; };
  /**
     Return id of last change. By default, everytime this class is allocated it
     is known to be changed. Derivates can also define new methods that also
     provide a substancial change that is included here. This behaviour is to be
     documented in those methods.  
  */
  //virtual inline int lastChange() { return 0; };
  /**
     Returns true if object was changed since state defined by changeId. See
     lastChange method.
  */
  //virtual inline bool isChanged(int changeId) { return false; };
  /**
     Overloading of the operator delete. On some systems this might make
     trouble when using with const pointers so you should prefer the decRef()
     method.
  */
  void operator delete(void *theObject);
  /**
     Return number of references to this object. Initial value at
     construction time is one. If this values becomes zero then this object
     deletes itself.
  */
  virtual unsigned int getNumberOfRefs() const;
  /**
     Increment number of refences by one.
  */
  virtual void incRef();
  /**
     Decrement number of refences by one. If this values becomes zero then
     this object deletes itself.
  */
  virtual void decRef();

protected:
  /**
     Call this method to sign a substancial change in this object. If you do so
     then properly document this behavour in all methods and conditions that
     cause this change. Also think of adding new methods to handle
     changes. E.g. if members of an array change then you could add methods like
     isModified() in the public declaration or a change of a window size could
     result in a new method like isChangedWindowSize() etc.
  */
  virtual inline void hasChanged() { };
  
private:
  /// Number of references.
  unsigned int myRefCount;
  /// True if allocated.
  bool myIsAllocated;
};

/**
   @short VisApi stack container implementation.
   A typical stack that can be used as a template implementation along with
   primitve datatypes.

   @author Peter Breitling
   @version 03-18-98
*/

template<class T> class visStack : public visObject 
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visStackId) return true;
    return visObject::instanceOf(id);
  }
  /**
    construct vector of any size, given size or given size and initialized
    array.
  */
  visStack() : mySize(0), myIncrement(256) {
    myData = (T*) malloc(myIncrement * sizeof(T));
    myCapacity = myIncrement;
  };
  virtual ~visStack() {
    if (getNumberOfRefs() <= 0) return;
    free(myData);
  };

#if false
  /*
    Returns a pointer to the array of contained objects.
  */
  operator T*() {
    return myData;
  }
#endif

  /**
     Clear all elements from stack. Size becomes zero.
  */
  void empty() {
    free(myData);
    myData = (T*) malloc(myIncrement * sizeof(T));
    myCapacity = myIncrement;
    mySize = 0;
  }

  /**
     Return the i-th element in the stack.
  */
  T& operator[](unsigned int i) {
    if (i >= mySize) {
      visMsgFatal << "Out of bounds in " << __FILE__ << ":" << __LINE__ 
	       << visMsgFatalEnd;
    }
    return myData[i];
  }

  /**
    Push element x on the top of the stack.
  */
  void push(T x) {
    if (mySize == myCapacity) {
      myCapacity += myIncrement;
      myData = (T *) realloc(myData, myCapacity * sizeof(T));
    }
    myData[mySize] = x;
    mySize++;
  }

  /**
    Pop i-th element from the top of the stack and return it.
  */
  T pop() {
    if (mySize <= 0) {
      visMsgFatal << "Pop() called on empty stack in "
	       << __FILE__ << ":" << __LINE__ 
	       << visMsgFatalEnd;
    }
    mySize--;
    if (mySize < myCapacity - myIncrement) {
      myCapacity -= myIncrement;
      myData = (T *) realloc(myData, myCapacity * sizeof(T));
    }
    return myData[mySize];
  }

  /**
    Return the number of elements in the stack.
  */
  int getSize() {
    return mySize;
  }
private:
  int mySize;
  int myCapacity, myIncrement;
  T *myData;
};

/**
   @short VisApi tree container implementation.

   @author Peter Breitling
   @version 02-14-99
*/

template<class T> class visCSet : public visObject 
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visCSetId) return true;
    return visObject::instanceOf(id);
  }
  /**
    construct vector of any size, given size or given size and initialized
    array.
  */
  visCSet() {
  };
  virtual ~visCSet() {
  };

  void empty() {
    free(myData);
    myData = (T*) malloc(myIncrement * sizeof(T));
    myCapacity = myIncrement;
    mySize = 0;
  }

  /*
    Push element x on the top of the stack.
  */
  bool insert(T &x) {
  }

  /*
    Push element x on the top of the stack.
  */
  void put(T &x) {
  }

  /*
    Push element x on the top of the stack.
  */
  void get(T &x) {
  }
  
  /*
    Return the number of elements in the stack.
  */
  int getSize() {
    return mySize;
  }
private:
  int mySize;
  int myCapacity, myIncrement;
  T *myData;
};

/**
   @short VisApi tree container implementation.

   @author Peter Breitling
   @version 03-18-99
*/

template <class T> class visAVLTree 
{
  struct avl {
    avl *left; T data; int i; avl *right;
    avl(T init) : left(NULL), data(init), i(0), right(NULL) { };
  };
  avl *root;

#define AVL_LEFTBALANCE                                                      \
  if (leaf->i == 2) {                                                        \
    if ((leaf->left)->i == 1) {                                              \
      leaf->i = 0;                                                           \
      avl *tmp = leaf->left; leaf->left = tmp->right; tmp->right = leaf;     \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
    else if ((leaf->left)->i == -1) {                                        \
      avl *tmp = leaf->left->right; leaf->left->right = tmp->left;           \
      tmp->left = leaf->left; leaf->left = tmp->right; tmp->right = leaf;    \
      tmp->left->i  = (tmp->i > 0 ?  0 : 1);                                 \
      tmp->right->i = (tmp->i > 0 ? -1 : 0);                                 \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
  }

#define AVL_RIGHTBALANCE                                                     \
  if (leaf->i == -2) {                                                       \
    if ((leaf->right)->i == -1) {                                            \
      leaf->i = 0;                                                           \
      avl *tmp = leaf->right; leaf->right = tmp->left; tmp->left = leaf;     \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
    else if ((leaf->right)->i == 1) {                                        \
      avl *tmp = leaf->right->left; leaf->right->left = tmp->right;          \
      tmp->right = leaf->right; leaf->right = tmp->left; tmp->left = leaf;   \
      tmp->right->i = (tmp->i > 0 ?  0 : -1);                                \
      tmp->left->i  = (tmp->i > 0 ?  1 : 0);                                 \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
  }

  int avl_insert(avl *&leaf, T &data)
  {
    if (leaf == NULL) {
      leaf = new avl(data);
      return 1;
    }
    if (data < leaf->data) {
      int old_i = leaf->i;
      leaf->i += avl_insert(leaf->left, data);
      AVL_LEFTBALANCE
      return (old_i == 0 && leaf->i != 0);
    } else if (data > leaf->data) {
      int old_i = leaf->i;
      leaf->i -= avl_insert(leaf->right, data);
      AVL_RIGHTBALANCE
      return (old_i == 0 && leaf->i != 0);
    } else {
      return 0;
    }
  };

  int avl_exist(avl *&leaf, T &data)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (data < leaf->data) {
      return avl_exist(leaf->left, data);
    } else if (data > leaf->data) {
      return avl_exist(leaf->right, data);
    } else {
      return 1;
    }
  };
  
  int avl_get(avl *&leaf, T &data)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (data < leaf->data) {
      return avl_get(leaf->left, data);
    } else if (data > leaf->data) {
      return avl_get(leaf->right, data);
    } else {
      data = leaf->data;
      return 1;
    }
  };
  
  int avl_remgreatest(avl *&leaf, T &data)
  {
    if (leaf == NULL)
      return 0;
    if (leaf->right == NULL) {
      data = leaf->data; leaf = leaf->left; return -1;
    }
    int old_i = leaf->i;
    leaf->i -= avl_remgreatest(leaf->right, data);
    AVL_LEFTBALANCE
    return (old_i != 0 && leaf->i == 0) ? -1 : 0;
  }
  int avl_remsmallest(avl *&leaf, T &data)
  {
    if (leaf == NULL)
      return 0;
    if (leaf->left == NULL) {
      data = leaf->data; leaf = leaf->right; return -1;
    }
    int old_i = leaf->i;
    leaf->i += avl_remsmallest(leaf->left, data);
    AVL_RIGHTBALANCE
    return (old_i != 0 && leaf->i == 0) ? -1 : 0;
  }
  int avl_remove(avl *&leaf, T &data)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (data > leaf->data) {
      int old_i = leaf->i;
      leaf->i -= avl_remove(leaf->right, data);
      AVL_LEFTBALANCE
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    } else if (data < leaf->data) {
      int old_i = leaf->i;
      leaf->i += avl_remove(leaf->left, data);
      AVL_RIGHTBALANCE
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    } else {
      if (!leaf->left) {
        leaf = leaf->right; return -1;
      }
      if (!leaf->right) {
        leaf = leaf->left; return -1;
      }
      int old_i = leaf->i;
      if (old_i > 0)
        leaf->i += avl_remgreatest(leaf->left, leaf->data);
      else
        leaf->i -= avl_remsmallest(leaf->right, leaf->data);
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    }
  }
  /*
    void rec_dump(ostream &s, avl *p) const
  {
    if (p == NULL) { s << "BOTTOM"; return; }
    s << "(";
    rec_dump(s, p->left);
    s << ", " << p->data << " ; " << p->i << ", ";
    rec_dump(s, p->right);
    s << ")";
  };
  void rec_dump2(ostream &s, avl *p) const
  {
    if (p == NULL) { return; }
    rec_dump2(s, p->left);
    s << " ";
    s << p->data;
    s << " ";
    rec_dump2(s, p->right);
    s << " ";
  };
  */
  void avl_deleteTree(avl *p)
  {
    if (p == NULL) { return; }
    avl_deleteTree(p->left);
    avl_deleteTree(p->right);
    delete p;
  };
  
public:
  visAVLTree() : root(NULL) {};
  virtual ~visAVLTree() {};
  bool isempty(void) const;
  void dump(ostream &s) const;
  void dump2(ostream &s) const;
  bool exist(T &d);
  bool get(T &d);
  bool insert(T &d);
  void put(T &d);
  void remove(T &d);
  void empty();
};

template <class T> bool visAVLTree<T>::isempty(void) const
{
  return (root == NULL);
};
template <class T> void visAVLTree<T>::dump(ostream &s) const
{
  //rec_dump(s, root); s << endl;
};
template <class T> void visAVLTree<T>::dump2(ostream &s) const
{
  //rec_dump2(s, root); s << endl;
};
template <class T> bool visAVLTree<T>::exist(T &d)
{
  return avl_exist(root, d);
};
template <class T> bool visAVLTree<T>::insert(T &d)
{
  return avl_insert(root, d);
};
template <class T> void visAVLTree<T>::put(T &d)
{
  T x = d;
  avl_remove(root, x);
  avl_insert(root, d);
};
template <class T> bool visAVLTree<T>::get(T &d)
{
  return avl_get(root, d);
};
template <class T> void visAVLTree<T>::remove(T &d)
{
  avl_remove(root, d);
};
template <class T> void visAVLTree<T>::empty()
{
  avl_deleteTree(root);
};

/**
   @short VisApi tree container implementation.

   @author Peter Breitling
   @version 03-18-99
*/

template <class K, class T> class visKAVLTree 
{
  struct avl {
    avl *left; K key; T data; int i; avl *right;
    avl(K &k, T &init) : left(NULL), key(k), data(init), i(0), right(NULL) { };
  };
  avl *root;

#define AVL_LEFTBALANCE                                                      \
  if (leaf->i == 2) {                                                        \
    if ((leaf->left)->i == 1) {                                              \
      leaf->i = 0;                                                           \
      avl *tmp = leaf->left; leaf->left = tmp->right; tmp->right = leaf;     \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
    else if ((leaf->left)->i == -1) {                                        \
      avl *tmp = leaf->left->right; leaf->left->right = tmp->left;           \
      tmp->left = leaf->left; leaf->left = tmp->right; tmp->right = leaf;    \
      tmp->left->i  = (tmp->i > 0 ?  0 : 1);                                 \
      tmp->right->i = (tmp->i > 0 ? -1 : 0);                                 \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
  }

#define AVL_RIGHTBALANCE                                                     \
  if (leaf->i == -2) {                                                       \
    if ((leaf->right)->i == -1) {                                            \
      leaf->i = 0;                                                           \
      avl *tmp = leaf->right; leaf->right = tmp->left; tmp->left = leaf;     \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
    else if ((leaf->right)->i == 1) {                                        \
      avl *tmp = leaf->right->left; leaf->right->left = tmp->right;          \
      tmp->right = leaf->right; leaf->right = tmp->left; tmp->left = leaf;   \
      tmp->right->i = (tmp->i > 0 ?  0 : -1);                                \
      tmp->left->i  = (tmp->i > 0 ?  1 : 0);                                 \
      tmp->i = 0; leaf = tmp; return 0;                                      \
    }                                                                        \
  }

  int avl_insert(avl *&leaf, K &key, T &data)
  {
    if (leaf == NULL) {
      leaf = new avl(key, data);
      return 1;
    }
    if (key < leaf->key) {
      int old_i = leaf->i;
      leaf->i += avl_insert(leaf->left, key, data);
      AVL_LEFTBALANCE
      return (old_i == 0 && leaf->i != 0);
    } else if (key > leaf->key) {
      int old_i = leaf->i;
      leaf->i -= avl_insert(leaf->right, key, data);
      AVL_RIGHTBALANCE
      return (old_i == 0 && leaf->i != 0);
    } else {
      return 0;
    }
  };

  int avl_exist(avl *&leaf, K &key)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (key < leaf->key) {
      return avl_exist(leaf->left, key);
    } else if (key > leaf->key) {
      return avl_exist(leaf->right, key);
    } else {
      return 1;
    }
  };
  
  int avl_get(avl *&leaf, K &key, T &data)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (key < leaf->key) {
      return avl_get(leaf->left, key, data);
    } else if (key > leaf->key) {
      return avl_get(leaf->right, key, data);
    } else {
      data = leaf->data;
      return 1;
    }
  };
  
  int avl_remgreatest(avl *&leaf, K &key, T &data)
  {
    if (leaf == NULL)
      return 0;
    if (leaf->right == NULL) {
      key = leaf->key; data = leaf->data; leaf = leaf->left; return -1;
    }
    int old_i = leaf->i;
    leaf->i -= avl_remgreatest(leaf->right, key, data);
    AVL_LEFTBALANCE
    return (old_i != 0 && leaf->i == 0) ? -1 : 0;
  }
  int avl_remsmallest(avl *&leaf, K &key, T &data)
  {
    if (leaf == NULL)
      return 0;
    if (leaf->left == NULL) {
      key = leaf->key; data = leaf->data; leaf = leaf->right; return -1;
    }
    int old_i = leaf->i;
    leaf->i += avl_remsmallest(leaf->left, key, data);
    AVL_RIGHTBALANCE
    return (old_i != 0 && leaf->i == 0) ? -1 : 0;
  }
  int avl_remove(avl *&leaf, K &key, T &data)
  {
    if (leaf == NULL) {
      return 0;
    }
    if (key > leaf->key) {
      int old_i = leaf->i;
      leaf->i -= avl_remove(leaf->right, key, data);
      AVL_LEFTBALANCE
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    } else if (key < leaf->key) {
      int old_i = leaf->i;
      leaf->i += avl_remove(leaf->left, key, data);
      AVL_RIGHTBALANCE
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    } else {
      if (!leaf->left) {
        leaf = leaf->right; return -1;
      }
      if (!leaf->right) {
        leaf = leaf->left; return -1;
      }
      int old_i = leaf->i;
      if (old_i > 0)
        leaf->i += avl_remgreatest(leaf->left, leaf->key, leaf->data);
      else
        leaf->i -= avl_remsmallest(leaf->right, leaf->key, leaf->data);
      return (old_i != 0 && leaf->i == 0) ? -1 : 0;
    }
  }
  /*
    void rec_dump(ostream &s, avl *p) const
  {
    if (p == NULL) { s << "BOTTOM"; return; }
    s << "(";
    rec_dump(s, p->left);
    s << ", " << p->data << " ; " << p->i << ", ";
    rec_dump(s, p->right);
    s << ")";
  };
  void rec_dump2(ostream &s, avl *p) const
  {
    if (p == NULL) { return; }
    rec_dump2(s, p->left);
    s << " ";
    s << p->data;
    s << " ";
    rec_dump2(s, p->right);
    s << " ";
  };
  */
  void avl_deleteTree(avl *p)
  {
    if (p == NULL) { return; }
    avl_deleteTree(p->left);
    avl_deleteTree(p->right);
    delete p;
  };
  
public:
  visKAVLTree() : root(NULL) {};
  virtual ~visKAVLTree() { this->empty(); };
  bool isempty(void) const;
  void dump(ostream &s) const;
  void dump2(ostream &s) const;
  bool exist(K &k);
  bool get(K &k, T &d);
  bool insert(K &k, T &d);
  void put(K &k, T &d);
  void remove(K &k, T &d);
  void empty();
};

template <class K, class T> bool visKAVLTree<K,T>::isempty(void) const
{
  return (root == NULL);
};
template <class K, class T> void visKAVLTree<K,T>::dump(ostream &s) const
{
  //rec_dump(s, root); s << endl;
};
template <class K, class T> void visKAVLTree<K,T>::dump2(ostream &s) const
{
  //rec_dump2(s, root); s << endl;
};
template <class K, class T> bool visKAVLTree<K,T>::exist(K &k)
{
  return avl_exist(root, k);
};
template <class K, class T> bool visKAVLTree<K,T>::insert(K &k, T &d)
{
  return avl_insert(root, k, d);
};
template <class K, class T> void visKAVLTree<K,T>::put(K &k, T &d)
{
  T x = d;
  avl_remove(root, k, x);
  avl_insert(root, k, d);
};
template <class K, class T> bool visKAVLTree<K,T>::get(K &k, T &d)
{
  return avl_get(root, k, d);
};
template <class K, class T> void visKAVLTree<K,T>::remove(K &k, T &d)
{
  avl_remove(root, k, d);
};
template <class K, class T> void visKAVLTree<K,T>::empty()
{
  avl_deleteTree(root);
};

typedef char* visCharP;

class visProperty
{
public:
  visProperty();
  visProperty(visProperty &x);
  visProperty(char *name, char *value = NULL);
  virtual ~visProperty();
  
  /**
     Return the i-th value of the property.
  */
  virtual char* operator[](unsigned int i);
  virtual int operator<(const visProperty &x);
  virtual int operator>(const visProperty &x);
  virtual int operator<=(const visProperty &x);
  virtual int operator>=(const visProperty &x);
  virtual int operator==(const visProperty &x);
  virtual void operator=(visProperty &x);
  virtual inline void setName(const char *name);
  virtual inline int getDim();
  virtual inline const char *getName();
  virtual inline const char *getValue(unsigned int n = 0);
  virtual inline void pushValue(char *value);
  virtual inline void setValue(int d, char **value);
private:
  char *myName;
  visStack<visCharP> myValue;
};

class visProperties : public visObject
{
public:
  virtual bool instanceOf(visClassId id) { 
    if (id == visPropertiesId) return true;
    return visObject::instanceOf(id);
  }
  visProperties();
  virtual ~visProperties();

  virtual bool allocate();
  virtual bool allocate(istream &in);
  virtual void deallocate();
  virtual bool setProperties(istream &in);
  virtual bool hasProperty(char *name);
  virtual bool getProperties(ostream &out);
  virtual visProperty getProperty(char *name);
  virtual void setProperty(visProperty &property);
  virtual void unsetProperty(char *name);
  virtual void unsetProperties();
private:
  visAVLTree<visProperty> myProperties;
};

template <class T> class visCoord
{
public:
  visCoord(int dim = 0, T *value = 0);
  visCoord(visCoord &c);
  virtual ~visCoord();
  T& operator[](unsigned int i);
  int operator<(const visCoord &x);
  int operator>(const visCoord &x);
  int operator<=(const visCoord &x);
  int operator>=(const visCoord &x);
  int operator==(const visCoord &x);
  void operator=(visCoord &x);
  inline int getDim();
  inline T getValue(unsigned int n = 0);
  inline void setValue(int d, T value);
  inline void setValue(T *value);
private:
  int myDim;
  T *myValue;
};

typedef visCoord<visSGTPOS> visSGPos;

//-----------------------------
// IAC modification
//-----------------------------
//typedef  visPosAVLTree;

typedef int visPosAVLTree;

//-----------------------------
// end of IAC modification
//-----------------------------

/* -----------------------------------------------------------------------------
   visCoord Methods
   ---------------------------------------------------------------------------*/

template <class T> visCoord<T>::visCoord(int dim, T *value) : myDim(dim)
{ 
  int i;
  if (myDim > 0) {
    myValue = new T[dim];
    if (value) {
      for (i = 0; i < myDim; i++) {
	myValue[i] = value[i];
      }
    }
  } else {
    myValue = NULL;
  }
};

template <class T> visCoord<T>::visCoord(visCoord &x) 
{ 
  int i;
  myDim = x.myDim;
  if (myDim > 0) {
    myValue = new T[myDim];
    if (x.myValue) {
      for (i = 0; i < myDim; i++) {
	myValue[i] = x.myValue[i];
      }
    }
  } else {
    myValue = NULL;
  }
};

template <class T> visCoord<T>::~visCoord() 
{
  if (myValue) { delete[] myValue; myValue = NULL; }
};

template <class T> T& visCoord<T>::operator[](unsigned int i)
{
  if (i >= myDim) {
    visMsgFatal << "Out of bounds in " << __FILE__ << ":" << __LINE__ 
		<< visMsgFatalEnd;
  }
  return myValue[i];
}

template <class T> int visCoord<T>::operator<(const visCoord &x) 
{ 
  int i;
  for (i = 0; i < myDim; i++) {
    if (myValue[i] < x.myValue[i]) {
      return true;
    } else if (myValue[i] > x.myValue[i]) {
      return false;
    }
  }
  return false;
};

template <class T> int visCoord<T>::operator>(const visCoord &x)
{ 
  int i;
  for (i = 0; i < myDim; i++) {
    if (myValue[i] > x.myValue[i]) {
      return true;
    } else if (myValue[i] < x.myValue[i]) {
      return false;
    }
  }
  return false;
};

template <class T> int visCoord<T>::operator<=(const visCoord &x) 
{ 
  int i;
  for (i = 0; i < myDim; i++) {
    if (myValue[i] < x.myValue[i]) {
      return true;
    } else if (myValue[i] > x.myValue[i]) {
      return false;
    }
  }
  return true;
};

template <class T> int visCoord<T>::operator>=(const visCoord &x) 
{ 
  int i;
  for (i = 0; i < myDim; i++) {
    if (myValue[i] > x.myValue[i]) {
      return true;
    } else if (myValue[i] < x.myValue[i]) {
      return false;
    }
  }
  return true;
};

template <class T> int visCoord<T>::operator==(const visCoord &x) 
{ 
  int i;
  for (i = 0; i < myDim; i++) {
    if (myValue[i] != x.myValue[i]) {
      return false;
    }
  }
  return true;
};

template <class T> void visCoord<T>::operator=(visCoord &x) 
{ 
  if (myValue) { delete[] myValue; myValue = NULL; }
  int i;
  myDim = x.myDim;
  if (myDim > 0) {
    myValue = new T[myDim];
    if (x.myValue) {
      for (i = 0; i < myDim; i++) {
	myValue[i] = x.myValue[i];
      }
    }
  } else {
    myValue = NULL;
  }
};

template <class T> int visCoord<T>::getDim() 
{ 
  return myDim; 
}

template <class T> T visCoord<T>::getValue(unsigned int n) 
{ 
  visAssert(n < myDim);
  return myValue[n]; 
};

template <class T> void visCoord<T>::setValue(int d, T value) 
{
  visAssert(d < myDim);
  myValue[d] = value;
};

template <class T> void visCoord<T>::setValue(T *value) 
{
  visAssert(value);
  int i;
  for (i = 0; i < myDim; i++) {
    myValue[i] = value[i];
  }
};


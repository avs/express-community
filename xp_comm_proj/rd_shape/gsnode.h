//
//  Include file for the XP_GIS_ESRI_Shape_c class, which provides utilities
//  to read an ESRI shape file.
//

/*
    Protected Constructors

      XP_GIS_TreeNode_c();
        The constructor.

      virtual ~XP_GIS_TreeNode_c();
        the destructor.

      XP_GIS_TreeNode_c(const XP_GIS_TreeNode_c &object);
        The copy constructor


    Protected Operators

      XP_GIS_TreeNode_c &operator=(const XP_GIS_Tree_x &object);
        The assignment operator assigns all values as necessary.  It 
        allocates space for a copy of the child list and then populates
        with data from object.

    Public Methods

      unsigned long InsertChild(XP_GIS_TreeNode_c &TreeNode);
        Method to insert a child.  Duplicate children are not allowed
        at a node.  If successful, return XP_GIS_OK.  Otherwise, returns
        one of the following:
          XP_GIS_ALLOC_ERROR
          XP_GIS_ERROR

      unsigned long RemoveChild(XP_GIS_TreeNode_c &TreeNode);
        Method to remove a child.  If successful, returns XP_GIS_OK.
        Otherwise, returns one of the following:
          XP_GIS_ERROR

      unsigned long InsertSibling(XP_GIS_TreeNode_c &TreeNode);
        Method to insert a sibling.  if successful, returns XP_GIS_OK.
        Otherwise, returns one of the following:
          XP_GIS_ALLOC_ERROR
          XP_GIS_ERROR
        
      unsigned long Reparent(XP_GIS_TreeNode_c &TreeNode);
        Method to reparent a node from it's current parent to TreeNode.
        If this method fails, the node is left parented to the original
        parent (if possible).

        If successful, returns XP_GIS_OK.  Otherwise, returns one of:
          XP_GIS_ALLOC_ERROR
          XP_GIS_ERROR

      XP_GIS_TreeNode_c &Parent();
        Method to get the parent.

      XP_GIS_TreeNode_c &Child(unsigned long ChildNumber);
        Method to get child number ChildNumber.

      unsigned long NumberOfChildren() const;
        Method to get the number of children.  This method returns the
        number of children.
*/



#ifndef XP_GIS_NODE_H
#define XP_GIS_NODE_H

#include <malloc.h>

#include "gserror.h"

class XP_GIS_TreeNode_c
{
  protected:

    XP_GIS_TreeNode_c();
    virtual ~XP_GIS_TreeNode_c();
    XP_GIS_TreeNode_c(const XP_GIS_TreeNode_c &object);
    XP_GIS_TreeNode_c &operator=(const XP_GIS_TreeNode_c &object);

  public:

    unsigned long InsertChild(XP_GIS_TreeNode_c &TreeNode);
    unsigned long RemoveChild(XP_GIS_TreeNode_c &TreeNode);
    unsigned long InsertSibling(XP_GIS_TreeNode_c &TreeNode);
    unsigned long Reparent(XP_GIS_TreeNode_c &TreeNode);
    XP_GIS_TreeNode_c &Parent();
    XP_GIS_TreeNode_c &Child(unsigned long ChildNumber);
    unsigned long NumberOfChildren() const {return _NumberOfChildren;}


  private:

    XP_GIS_TreeNode_c  *_Parent;
    unsigned long   _NumberOfChildren;
    unsigned long   _MaximumNumberOfChildren;
    XP_GIS_TreeNode_c **_Children;
};

#endif   // XP_GIS_NODE_H


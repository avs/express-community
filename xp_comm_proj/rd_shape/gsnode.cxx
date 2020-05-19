#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "gserror.h"

#include "gsnode.h"

//
//  The constructor.
//

XP_GIS_TreeNode_c::XP_GIS_TreeNode_c()
{
    _NumberOfChildren = 0;
    _MaximumNumberOfChildren = 0;
    _Parent = NULL;
    _Children = NULL;
}

//
//  The destructor.
//

XP_GIS_TreeNode_c::~XP_GIS_TreeNode_c()
{
    if (_Children != NULL)
    {
      free(_Children);
    }
}

//
//  The copy constructor.
//

XP_GIS_TreeNode_c::XP_GIS_TreeNode_c(const XP_GIS_TreeNode_c &object)
{
    *this = object;
}

//
//  The assignment operator.  This operator allocates space for a new
//  child list and populates it with the child list from the input object.
//

XP_GIS_TreeNode_c &XP_GIS_TreeNode_c::operator=(const XP_GIS_TreeNode_c &object)
{
    _Parent = object._Parent;
    _NumberOfChildren = object._NumberOfChildren;
    _MaximumNumberOfChildren = object._MaximumNumberOfChildren;
    _Children = (XP_GIS_TreeNode_c**) malloc(sizeof (XP_GIS_TreeNode_c *) *
                                         object._MaximumNumberOfChildren);
    if (_Children == NULL)  // on error, just reset # of children.
    {
      _NumberOfChildren = 0;
      _MaximumNumberOfChildren = 0;
    }
    else
    {
      memcpy(_Children,
             object._Children,
             sizeof(XP_GIS_TreeNode_c *) * _NumberOfChildren);
    }
    return *this;
}

//
//  Insert a child.  Note that duplicate children are not allowed.
//

unsigned long XP_GIS_TreeNode_c::InsertChild(XP_GIS_TreeNode_c &TreeNode)
{
    const unsigned long IncrementSize = 10;
    unsigned long ChildNumber;

    //
    //  If TreeNode is invalid, return an error.
    //

    if (&TreeNode == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  Check to see if the child is already on the list.
    //

    for (ChildNumber = 0; ChildNumber < _NumberOfChildren; ChildNumber++)
    {
      if (_Children[ChildNumber] == &TreeNode)
      {
        return XP_GIS_ERROR;     // node is already a child.
      }
    }

    //
    //  If necessary, reallocate the child list.
    //

    if (_MaximumNumberOfChildren == _NumberOfChildren)
    {
      _MaximumNumberOfChildren += IncrementSize;
      _Children = (XP_GIS_TreeNode_c**) realloc(_Children,
                                           sizeof (XP_GIS_TreeNode_c *) *
                                           _MaximumNumberOfChildren);
      if (_Children == NULL)
      {
        _MaximumNumberOfChildren -= IncrementSize;
        return XP_GIS_ALLOC_ERROR;
      }
    }

    //
    // Now, insert the child.
    //

    _Children[_NumberOfChildren++] = &TreeNode;

    //
    //  Update the child's parent pointer.
    //

    TreeNode._Parent = this;

    return XP_GIS_OK;
    
}

//
//  Remove the specified child.
//

unsigned long XP_GIS_TreeNode_c::RemoveChild(XP_GIS_TreeNode_c &TreeNode)
{
    unsigned long ChildNumber;

    //
    //  If TreeNode is invalid, return an error.
    //

    if (&TreeNode == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  Search the child list for a matching child.  If we find one
    //  remove it.  Otherwise return an error.  A child is removed
    //  by copying the rest of the list back one place and shortening
    //  the list by one.
    //

    for (ChildNumber = 0; ChildNumber < _NumberOfChildren; ChildNumber++)
    {
      if (_Children[ChildNumber] == &TreeNode)
      {
        for ( ; ChildNumber < _NumberOfChildren-1; ChildNumber++)
        {
          _Children[ChildNumber] = _Children[ChildNumber+1];
        }
        _NumberOfChildren--;
        return XP_GIS_OK; 
      }
    }
    return XP_GIS_ERROR;
}

//
//  Insert a sibling.  Simply insert it as a child of the parent.
//

unsigned long XP_GIS_TreeNode_c::InsertSibling(XP_GIS_TreeNode_c &TreeNode)
{
  return Parent().InsertChild(TreeNode);
}

//
//  Reparent the node so it is a child of the node passed in.
//

unsigned long XP_GIS_TreeNode_c::Reparent(XP_GIS_TreeNode_c &TreeNode)
{
    XP_GIS_TreeNode_c *OldParent;
    unsigned long ReturnValue;

    //
    //  If TreeNode is invalid, return an error.
    //

    if (&TreeNode == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  If the current parent and the new parent are the same, just
    //  return OK.
    //

    if(_Parent == &TreeNode)
    {
      return XP_GIS_OK;
    }

    //
    //  Save the old parent, so if we have a subsequent error we
    //  can hook it back up.
    //
 
    OldParent = _Parent;

    //
    //  Remove the reference to this child from it's current parent.
    //

    if ((ReturnValue = Parent().RemoveChild(*this)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Add this node to the new parent's list of children.
    //

    if ((ReturnValue = TreeNode.InsertChild(*this)) != XP_GIS_OK)
    {
      OldParent->InsertChild(*this);  // hook back up to the original parent
      return ReturnValue;
    }

    //
    //  Update the parent reference.
    //

    _Parent = &TreeNode;

    return XP_GIS_OK;
}

//
//  Get the parent
//

XP_GIS_TreeNode_c &XP_GIS_TreeNode_c::Parent()
{
    return *_Parent;
}

//
//  Get the specified child.
//

XP_GIS_TreeNode_c &XP_GIS_TreeNode_c::Child(unsigned long ChildNumber)
{

    //
    //  If ChildNumber is greater than the number of children, return
    //  *this to indicate an error.
    //

    if (ChildNumber >= _NumberOfChildren)
    {
      return *this;
    }

    return *(_Children[ChildNumber]);
}

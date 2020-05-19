#include <iostream.h>

//
//  This file contains the source code for the XP_GIS_PolygonTree_c class.
//  This class manages a tree of rings that comprise a polygon.
//
//

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

#include "gserror.h"

#include "gspgtree.h"

#include "gsif.h"


//
//  The default constructor.
//

XP_GIS_PolygonTree_c::XP_GIS_PolygonTree_c()
{
    _VertexList = NULL;
    _RingVertices   = NULL;
    _RootNode   = NULL;
    _Depth      = 0;
    _MaximumNumberOfVerticesInRing = 0;
}

//
//  The overloaded constructor.
//

XP_GIS_PolygonTree_c::XP_GIS_PolygonTree_c(double XMin,
                                          double YMin,
                                          double XMax,
                                          double YMax,
                                          unsigned long  NumberOfRings,
                                          unsigned long *RingIndex,
                                          unsigned long  NumberOfVertices,
                                          double        *XVertices,
                                          double        *YVertices)
{
    _VertexList = NULL;
    _RingVertices   = NULL;
    _RootNode   = NULL;
    _Depth      = 0;
    _MaximumNumberOfVerticesInRing = 0;

    //
    //  Create the VertexList object.
    //

    _VertexList = new XP_GIS_VertexList_c(NumberOfVertices,
                                         XVertices,
                                         YVertices);
    if (_VertexList != NULL)
    {

      //
      //  Build the tree
      //
  
      if (_BuildTree(XMin,YMin,XMax,YMax,
                     NumberOfRings,
                     RingIndex,
                     NumberOfVertices,
                     XVertices,
                     YVertices) != XP_GIS_OK)
      {
        // no-op for now
      }
    }
}

//
//  The destructor.
//

XP_GIS_PolygonTree_c::~XP_GIS_PolygonTree_c()
{
    if (_VertexList != NULL) delete _VertexList;
    if (_RootNode != NULL) _DestroyNode(*_RootNode);
    if (_RingVertices != NULL) free(_RingVertices);
}

//
//  The copy constructor.
//

XP_GIS_PolygonTree_c::XP_GIS_PolygonTree_c(const XP_GIS_PolygonTree_c &object)
{
    *this = object;
}

//
//  The assignment operator.  
//

XP_GIS_PolygonTree_c &XP_GIS_PolygonTree_c::operator=(
                                 const XP_GIS_PolygonTree_c &object)
{
    _VertexList = object._VertexList; // uses vertex list = operator
    _RootNode   = object._RootNode;   // uses polygon node = operator
    if (object._RingVertices != NULL)     // manually copy vertices
    {
      _RingVertices = (unsigned long *) malloc(sizeof (unsigned long) * 
                                       object._MaximumNumberOfVerticesInRing);
      if (_RingVertices == NULL)
      {
        _MaximumNumberOfVerticesInRing = 0;
      }
      else
      {
        memcpy(_RingVertices,
               object._RingVertices,
               _MaximumNumberOfVerticesInRing);
        _MaximumNumberOfVerticesInRing = object._MaximumNumberOfVerticesInRing;
      }
    }
    else
    {
     _RingVertices = NULL;
    }
    return *this;
}

//
//  Method to get the number of connections for a tree of rings.
//

unsigned long XP_GIS_PolygonTree_c::NumberOfConnections(
                              unsigned long *NumberOfConnections)
{
    _ResetDepth();
    return _GetNumberOfConnectionsAtNode(*_RootNode,
                                         NumberOfConnections);
}

//
//  Method to triangulate a tree of rings.
//

unsigned long XP_GIS_PolygonTree_c::Triangulate(
                              unsigned long *NumberOfConnections,
                              unsigned long *ConnectList)
{
    _ResetDepth();
    return _TriangulateNode(*_RootNode,
                            NumberOfConnections,
                            ConnectList);
}

//
//  Method to build a tree of rings.
//

unsigned long XP_GIS_PolygonTree_c::_BuildTree(
                                double         XMin,
                                double         YMin,
                                double         XMax,
                                double         YMax,
                                unsigned long  NumberOfRings,
                                unsigned long *RingIndex,
                                unsigned long  NumberOfVertices,
                                double        *XVertices,
                                double        *YVertices)
{
    XP_GIS_PolygonNode_c *RingNode;
    unsigned long RingNumber;
    unsigned long RingStart;
    unsigned long RingEnd;
    unsigned long NumberOfVerticesInRing;
    unsigned long VertexNumber;
    unsigned long VertexIndex;
 

    //
    //  If there is already a tree, destroy it.
    //
   
    if (_RootNode != NULL)
    {
      _DestroyNode(*_RootNode); 
      _RootNode = NULL;
    }

    //
    //  If there is already a vertex list, destroy it.
    //
   
    if (_VertexList != NULL)
    {
      delete _VertexList;
      _VertexList = NULL;
    }

    //
    //  Create the vertex list object
    //

    _VertexList = new XP_GIS_VertexList_c(NumberOfVertices,
                                         XVertices,
                                         YVertices);
    if (_VertexList == NULL)
    {
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  create root node.
    //

    _RootNode = new XP_GIS_PolygonNode_c();
    if (_RootNode == NULL)
    {
      delete _VertexList;
      return XP_GIS_ALLOC_ERROR;
    }
    _RootNode->SetBounds(-DBL_MAX,-DBL_MAX,DBL_MAX,DBL_MAX);
    _RootNode->VertexList(*_VertexList);

    //
    //  Allocate space for the indices to ring vertices.
    //

    _RingVertices = (unsigned long *) malloc(
                       sizeof(unsigned long) * NumberOfVertices);
    if (_RingVertices == NULL)
    {
      _DestroyNode(*_RootNode);
      if (_VertexList != NULL) delete _VertexList;
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  For each ring, create a node and insert it.
    //

    for (RingNumber = 0;
         RingNumber < NumberOfRings;
         RingNumber++)
    {
      //
      //  Figure out where the ring starts and ends in the ring index.
      //  Note that we want an open polygon (start and end are different).
      //  If this is a closed polygon, drop the last vertex.
      //

      RingStart = RingIndex[RingNumber];
      RingEnd   = (RingNumber < NumberOfRings - 1) ?
                      RingIndex[RingNumber + 1] - 1 :
                      NumberOfVertices - 1;
      if ((XVertices[RingStart] == XVertices[RingEnd]) &&
          (YVertices[RingStart] == YVertices[RingEnd]))
      {
        RingEnd--;
      }

      //
      //  Figure out the number of vertices in this ring. 
      //

      NumberOfVerticesInRing = (RingEnd - RingStart) + 1;
      
      //
      //  Swap the order of the vertex indices as we load the _RingVertex
      //  array.
      //

      for (VertexNumber = 0;
           VertexNumber < NumberOfVerticesInRing;
           VertexNumber++)
      {
        VertexIndex = RingStart + VertexNumber;
        _RingVertices[VertexIndex] = 
           ((RingStart + NumberOfVerticesInRing) - 1) - VertexNumber;
      }

      //
      //  Create the node.
      //

      RingNode = new XP_GIS_PolygonNode_c(*_VertexList,
                                          NumberOfVerticesInRing,
                                         &_RingVertices[RingStart]);
      if (RingNode == NULL)
      {
        _DestroyNode(*_RootNode);
        if (_VertexList != NULL) delete _VertexList;
        return XP_GIS_ALLOC_ERROR;
      }

      //
      //  Insert the node into the tree.
      //

      _ResetDepth();
      if (_InsertNode(*_RootNode,*RingNode) != XP_GIS_OK)
      {
        _DestroyNode(*_RootNode);
        if (_VertexList != NULL) delete _VertexList;
        return XP_GIS_ERROR;
      }
    }
    return XP_GIS_OK;
}

//
// Method to insert a node into a tree of rings.
//

unsigned long XP_GIS_PolygonTree_c::_InsertNode(
                                XP_GIS_PolygonNode_c &Node,
                                XP_GIS_PolygonNode_c &NodeToInsert)
{
    unsigned long ChildNumber;
    unsigned long ReturnValue;
    unsigned char ContainsChildren;
    XP_GIS_PolygonNode_c *ChildNode;

    //
    //  If the input nodes are null, return an error.
    //

    if ((&Node == NULL) || (&NodeToInsert == NULL))
    {
      return XP_GIS_ERROR;
    }

    //
    //  NodeToInsert could contain one or more of Node's children.
    //  If NodeToInsert contains any children of Node, reparent those
    //  children so they are children of NodeToInsert and reparent
    //  NodeToInsert so it is a child of Node.
    //

    ContainsChildren = XP_GIS_FALSE;
    for (ChildNumber = 0;
         ChildNumber < Node.NumberOfChildren();
         ChildNumber++)
    {
      ChildNode = (XP_GIS_PolygonNode_c*) &Node.Child(ChildNumber);
      if (NodeToInsert.Contains(*ChildNode) == XP_GIS_TRUE)
      {
        ContainsChildren = XP_GIS_TRUE;
        ReturnValue = ChildNode->Reparent(NodeToInsert);
        if (ReturnValue != XP_GIS_OK)
        {
          for ( ;
               ChildNumber >= 0;
               ChildNumber--)
          {
            ChildNode = (XP_GIS_PolygonNode_c*) &Node.Child(ChildNumber);
            ChildNode->Reparent(Node);
          }
          return ReturnValue;
        }
      }
    }
    if (ContainsChildren == XP_GIS_TRUE)
    {
      return Node.InsertChild(NodeToInsert);
    }

    //
    //  Otherwise, check to see if NodeToInsert is inside any of Node's
    //  children.  If it is it can be in exactly one child so recurse
    //  into that child.  If it is not, insert NodeToInsert as a child
    //   of Node.
    //


    for (ChildNumber = 0;
         ChildNumber < Node.NumberOfChildren();
         ChildNumber++)
    {
      ChildNode = (XP_GIS_PolygonNode_c*) &Node.Child(ChildNumber);
      if (NodeToInsert.Inside(*ChildNode) == XP_GIS_TRUE)
      {
        return _InsertNode(*ChildNode,NodeToInsert);
      }
    }
    return Node.InsertChild(NodeToInsert);
}

//
//  Method to destroy a node in a tree of rings.
//

unsigned long XP_GIS_PolygonTree_c::_DestroyNode(XP_GIS_PolygonNode_c &Node)
{
    unsigned long ChildNumber;
    unsigned long ReturnValue;

    //
    //  If the input node is null, just return OK.
    //

    if (&Node == NULL)
    {
      return XP_GIS_OK;
    }

    //
    //  Recursively call this method for each child.
    //

    for (ChildNumber = 0;
         ChildNumber < Node.NumberOfChildren();
         ChildNumber++)
    {
      ReturnValue = _DestroyNode(
                          (XP_GIS_PolygonNode_c&) Node.Child(ChildNumber));
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
    }

    //
    //  Delete the node that was passed in.
    //

    delete &Node;
    return XP_GIS_OK;
}

//
//  Method to get the number of connections in a node of rings.
//

unsigned long XP_GIS_PolygonTree_c::_GetNumberOfConnectionsAtNode(
                                XP_GIS_PolygonNode_c &Node,
                                unsigned long        *NumberOfConnections)
{
    unsigned long ChildNumber;
    unsigned long ReturnValue;
    unsigned long LocalNumberOfConnections;
    unsigned long TotalNumberOfVertices;


    //
    //  If the input node is null, just return OK.
    //

    if (&Node == NULL)
    {
      return XP_GIS_OK;
    }

    //
    //  If this is the root, initialize the number of connections
    //  to zero.
    //

    if (_Depth == 0)
    {
      *NumberOfConnections = 0;
    }

    //
    //  Recursively call this method for each child.
    //

    for (ChildNumber = 0;
         ChildNumber < Node.NumberOfChildren();
         ChildNumber++)
    {
      _Depth++;
      LocalNumberOfConnections = 0;
      ReturnValue = _GetNumberOfConnectionsAtNode(
                          (XP_GIS_PolygonNode_c&) Node.Child(ChildNumber),
                                                 &LocalNumberOfConnections);
      _Depth--;
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      *NumberOfConnections += LocalNumberOfConnections;
    }

    //
    //  Now accumulate number of connections at this node.
    //  We only do this if we are not at the root node (depth = 0)
    //  and if we are at an odd depth.  Note we don't want to recurse
    //  here.  We just want to go down one leve.
    //

    if ((_Depth%2) == 1)
    {
      TotalNumberOfVertices = (Node.NumberOfVertices());
      for (ChildNumber = 0;
           ChildNumber < Node.NumberOfChildren();
           ChildNumber++)
      {
        TotalNumberOfVertices += 
             ((XP_GIS_PolygonNode_c&) 
              (Node.Child(ChildNumber))).NumberOfVertices();
      }
      *NumberOfConnections += (((TotalNumberOfVertices - 2) + 
                                (2 * Node.NumberOfChildren())) * 3);
    }
    return XP_GIS_OK;
}

//
//  Method to triangulate a node of rings.
//

unsigned long XP_GIS_PolygonTree_c::_TriangulateNode(
                                XP_GIS_PolygonNode_c &Node,
                                unsigned long        *NumberOfConnections,
                                unsigned long        *ConnectList)
{
    unsigned long ChildNumber;
    unsigned long ReturnValue;
    unsigned long LocalNumberOfConnections;

    //
    //  If the input node is null, just return OK.
    //

    if (&Node == NULL)
    {
      return XP_GIS_OK;
    }

    //
    //  If this is the root, initialize the number of connections
    //  to zero.
    //

    if (_Depth == 0)
    {
      *NumberOfConnections = 0;
    }


    //
    //  Recursively call this method for each child.
    //

    for (ChildNumber = 0;
         ChildNumber < Node.NumberOfChildren();
         ChildNumber++)
    {
      _Depth++;
#ifdef DEBUG
cout << "_TriangulateNode: Depth = " << _Depth << " Child# = ";
cout << ChildNumber << endl;
#endif
      ReturnValue = _TriangulateNode(
                         ((XP_GIS_PolygonNode_c&) (Node.Child(ChildNumber))),
                         NumberOfConnections,
                        &ConnectList[*NumberOfConnections]);
      _Depth--;
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
    }

    //
    //  Now triangulate this node.
    //  We only do this if we are not at the root node (depth = 0)
    //  and if we are at an odd depth.
    //

    if ((_Depth%2) == 1)
    {
      ReturnValue = _TriangulatePolygon(Node,
                                       &LocalNumberOfConnections,
                                        ConnectList);
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      *NumberOfConnections += LocalNumberOfConnections;
    }
    return XP_GIS_OK;
}

//
//  Method to triangulate a polygon stored in a ring node.  The polygon
//  consists of the data at the Polygon node as well as any direct
//  children (if any).
//

unsigned long XP_GIS_PolygonTree_c::_TriangulatePolygon(
                                XP_GIS_PolygonNode_c &Polygon,
                                unsigned long        *NumberOfConnections,
                                unsigned long        *ConnectList)
{
    unsigned long         ContourNumber;
    unsigned long         NumberOfContours;
    unsigned long        *Contours;
    unsigned long         VertexNumber;
    unsigned long         TVertexNumber;
    unsigned long         ChildNumber;
    unsigned long         NumberOfVertices;
    unsigned long         TotalNumberOfVertices;
    unsigned long         NumberOfVerticesInPolygon;
    unsigned long        *VertexIndex;
    unsigned long         TriangleNumber;
    unsigned long         NumberOfTriangles;
    const unsigned long  *Vertices;
    int                   ReturnValue;
    int                  (*Triangles)[3];
    double               (*VertexList)[2];
    const double         *XVertices;
    const double         *YVertices;
    XP_GIS_PolygonNode_c *ChildPolygon;

    //
    //  Set the number of contours
    //

    NumberOfContours = 1 + Polygon.NumberOfChildren();

    //
    //  Allocate space for the contour array.
    //

    Contours = (unsigned long *) malloc(sizeof(unsigned long) * 
                                        NumberOfContours);
    if (Contours == NULL)
    {
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Figure out the number of vertices in this polygon.  The polygon
    //  consists of this node and immediate children.
    //

    NumberOfVerticesInPolygon = Polygon.NumberOfVertices();
    for (ChildNumber = 0;
         ChildNumber < Polygon.NumberOfChildren();
         ChildNumber++)
    {
      NumberOfVerticesInPolygon += ((XP_GIS_PolygonNode_c &)
                  Polygon.Child(ChildNumber)).NumberOfVertices();
    }

    //
    //  Allocate space for the vertex arrays.  We need one for the vertex
    //  index and one for the vertices.  Note that position 0 is reserved
    //  for the triangulation routine, so we need to allocate one more
    //  than the number of vertices in the polygon.
    //

    Polygon.VertexList().GetVertices(&TotalNumberOfVertices,
                                     &XVertices,
                                     &YVertices);
    VertexList = (double (*)[2]) malloc(sizeof(double) * 
                                    ((NumberOfVerticesInPolygon + 1) * 2));
    if (VertexList == NULL)
    {
      if (Contours != NULL) free(Contours);
      return XP_GIS_ALLOC_ERROR;
    }
    VertexIndex = (unsigned long *) malloc(sizeof(unsigned long) * 
                                           (NumberOfVerticesInPolygon + 1));
    if (VertexIndex == NULL)
    {
      if (Contours != NULL) free(Contours);
      if (VertexList != NULL) free(VertexList);
      return XP_GIS_ALLOC_ERROR;
    }
 
    //
    //  Construct the vertex list.  Each polygon object contains a list
    //  of vertex indexes.  These indexes point to the appropriate entry
    //  in the VertexList object.  This organization prevents us from
    //  copying vertices unecessarily.
    //
    //  For efficient processing we need to maintain an index from
    //  the vertex list we are creating from the triangulation routine
    //  back to the location in the VertexList object.  We will use
    //  this index to map the triangle corner vertex indexes returned
    //  from the triangulation routine back to vertex indexes from the
    //  VertexList object.
    //
    //  While we process each ring, populate the contour array with the
    //  number of vertices in each contour.
    //
    //

    Polygon.GetVertices(&NumberOfVertices,&Vertices);
    TVertexNumber = 1;
    for (VertexNumber = 0;
         VertexNumber < Polygon.NumberOfVertices();
         VertexNumber++)
    {
      VertexIndex[TVertexNumber] = Vertices[VertexNumber];
      VertexList[TVertexNumber][0] = XVertices[VertexIndex[TVertexNumber]];
      VertexList[TVertexNumber][1] = YVertices[VertexIndex[TVertexNumber]];
      TVertexNumber++;
    }
    Contours[0] = Polygon.NumberOfVertices();
    for (ContourNumber = 1;
         ContourNumber < NumberOfContours;
         ContourNumber++)
    {
      ChildPolygon = (XP_GIS_PolygonNode_c *) &(Polygon.Child(ContourNumber-1));
      if (ChildPolygon == NULL)
      {
        continue;
      }
      ChildPolygon->GetVertices(&NumberOfVertices,
                                &Vertices);
      Contours[ContourNumber] = NumberOfVertices;
      for (VertexNumber = 0;
           VertexNumber < NumberOfVertices;
           VertexNumber++)
      {
        VertexIndex[TVertexNumber] = Vertices[VertexNumber];
        VertexList[TVertexNumber][0] = XVertices[VertexIndex[TVertexNumber]];
        VertexList[TVertexNumber][1] = YVertices[VertexIndex[TVertexNumber]];
        TVertexNumber++;
      }
    }

    //
    //  Allocate space for the triangle array.
    //

    NumberOfTriangles = ((TVertexNumber - 1) - 2) + 
                         (2 * (NumberOfContours - 1));
    Triangles = (int (*)[3]) malloc(sizeof(int) * 
                                              (NumberOfTriangles * 3));
    if (Triangles == NULL)
    {
      if (Contours != NULL) free(Contours);
      if (VertexList != NULL) free(VertexList);
      if (VertexIndex != NULL) free(VertexIndex);
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Triangulate the polygon.
    //

    ReturnValue = triangulate_polygon((int)     NumberOfContours,
                                      (int *)   Contours,
                                                VertexList,
                                                Triangles);
    if (Contours != NULL) free(Contours);
    if (VertexList != NULL) free(VertexList);
    if (ReturnValue != 0)
    {
      if (VertexIndex != NULL) free(VertexIndex);
      if (Triangles != NULL) free(Triangles);
      return XP_GIS_ERROR;
    }

    //
    //  Now we need to map the triangle vertex indexes so that instead
    //  of offsets into the vertices array they point to the appropriate
    //  vertex in the VertexList object.  We do this as we copy the
    //  triangle vertex locations into the connect list.
    //

    *NumberOfConnections = 0;
    for (TriangleNumber = 0;
         TriangleNumber < NumberOfTriangles;
         TriangleNumber++)
    {
      ConnectList[(*NumberOfConnections)++] = 
                            VertexIndex[Triangles[TriangleNumber][0]];
      ConnectList[(*NumberOfConnections)++] = 
                            VertexIndex[Triangles[TriangleNumber][1]];
      ConnectList[(*NumberOfConnections)++] = 
                            VertexIndex[Triangles[TriangleNumber][2]];
    }
    if (VertexIndex != NULL) free(VertexIndex);
    if (Triangles != NULL) free(Triangles);

    return XP_GIS_OK;
}

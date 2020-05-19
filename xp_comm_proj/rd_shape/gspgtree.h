//
//  Include file for the XP_GIS_ESRI_Shape_c class, which provides utilities
//  to read an ESRI shape file.
//

/*

    Public Constructors
      XP_GIS_PolygonTree_c();
        The default constructor.

      XP_GIS_PolygonTree_c(double XMin,
                           double YMin,
                           double XMax,
                           double YMax,
                           unsigned long  NumberOfRings,
                           unsigned long *RingIndex,
                           unsigned long  NumberOfVertices,
                           double        *XVertices,
                           double        *YVertices);
        The overloaded constructor that creates the tree from the
        polygon description.  NumberOfRings is the number of rings
        in the polygon.  RingIndex contains an index into XVertices
        and YVertices for the start and end of each ring.  There are
        NumberOfRings * 2 entries in RingIndex.  NumberOfVertices is
        the number in XVertices and YVertices.

        The rings  should be arranged so the interior of the polygon is
        to the right when proceeding along the lines that form the ring.
        Outer rings are clockwise and inner rings are counterclockwise.

      virtual ~XP_GIS_PolygonTree_c();
        The destructor.

      XP_GIS_PolygonTree_c(const XP_GIS_PolygonTree_c &object);
        The copy constructor.

    Public Operators

      XP_GIS_PolygonTree_c &operator=(const XP_GIS_PolygonTree_c &object);
        The assignment operator.  This creates a copy of the tree.

    Public Methods

      unsigned long Triangulate(unsigned long *NumberOfConnections,
                                unsigned long *ConnectList);
        Method to triangulate a polygon that has been placed in the tree.
        If successful returns XP_GIS_OK.  Otherwise, returns XP_GIS_ERROR.

        NumberOfConnections is the number of connections for this set
        of rings (this polygon).  ConnectList is the connection list
        for this set of rings (polygon).  ConnectList contains indexes
        into arrays of vertices for the point that form the triangle.
        The number of triangles is NumberOfConnections / 3.

        It is expected that ConnectList will have been pre allocated 
        with sufficient space to hold all the connections.

      unsigned long NumberOfConnections(unsigned long *NumberOfConnections);
        Method to determine the number of connections.  This is rather
        expensive.  If successful, returns XP_GIS_OK.  Otherwise,
        returns XP_GIS_ERROR.

*/



#ifndef XP_GIS_POLYGON_TREE_H
#define XP_GIS_POLYGON_TREE_H

#include <malloc.h>

#include "gsconst.h"
#include "gserror.h"

#include "gspgnode.h"
#include "gsvrtlst.h"

class XP_GIS_PolygonTree_c
{

  public:

    XP_GIS_PolygonTree_c();
    XP_GIS_PolygonTree_c(double XMin,
                         double YMin,
                         double XMax,
                         double YMax,
                         unsigned long  NumberOfRings,
                         unsigned long *RingIndex,
                         unsigned long  NumberOfVertices,
                         double        *XVertices,
                         double        *YVertices);
    virtual ~XP_GIS_PolygonTree_c();
    XP_GIS_PolygonTree_c(const XP_GIS_PolygonTree_c &object);
    XP_GIS_PolygonTree_c &operator=(const XP_GIS_PolygonTree_c &object);

    unsigned long Triangulate(unsigned long *NumberOfConnections,
                              unsigned long *ConnectList);
    unsigned long NumberOfConnections(unsigned long *NumberOfConnections);

  private:
    void _ResetDepth() {_Depth = 0;}
    unsigned long _BuildTree(double         XMin,
                             double         YMin,
                             double         XMax,
                             double         YMax,
                             unsigned long  NumberOfRings,
                             unsigned long *RingIndex,
                             unsigned long  NumberOfVertices,
                             double        *XVertices,
                             double        *YVertices);
    unsigned long _InsertNode(XP_GIS_PolygonNode_c &TestNode,
                              XP_GIS_PolygonNode_c &RingNode);
    unsigned long _DestroyNode(XP_GIS_PolygonNode_c &Node);
    unsigned long _GetNumberOfConnectionsAtNode(
                          XP_GIS_PolygonNode_c &Polygon,
                          unsigned long        *NumberOfConnections);
    unsigned long _TriangulateNode(XP_GIS_PolygonNode_c &Polygon,
                                   unsigned long        *NumberOfConnections,
                                   unsigned long        *ConnectList);
    unsigned long _TriangulatePolygon(
                          XP_GIS_PolygonNode_c &Polygon,
                          unsigned long        *NumberOfConnections,
                          unsigned long        *ConnectList);

    XP_GIS_PolygonNode_c *_RootNode;     // root node of tree
    XP_GIS_VertexList_c  *_VertexList;   // object with actual vertices
    unsigned long         _Depth;        // how deep in the tree we are
    unsigned long        *_RingVertices; // list of vertex indices for 1 ring
    unsigned long         _MaximumNumberOfVerticesInRing;  // max verts in 1 ring

};

#endif   // XP_GIS_POLYGON_TREE_H

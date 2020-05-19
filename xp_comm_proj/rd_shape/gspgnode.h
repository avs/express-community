//
//  Include file for the XP_GIS_ESRI_Shape_c class, which provides utilities
//  to read an ESRI shape file.
//

/*

    Public Constructors

      XP_GIS_PolygonNode_c();
        The default constructor.


    XP_GIS_PolygonNode_c(XP_GIS_VertexList_c &VertList,
                         unsigned long NumberOfVertices = 0,
                         unsigned long const *Vertices = NULL,
                         double XMin = 0,
                         double YMin = 0,
                         double XMax = 0,
                         double YMax = 0);
        The overloaded constructor.  Optionally, the polygon bounds, 
        number of vertices, and vertex list can be provided.  The
        vertex list must be provided in this constructor.

      virtual ~XP_GIS_PolygonNode_c();
        The destructor.

      XP_GIS_PolygonNode_c(const XP_GIS_PolygonNode_c &object);
        The copy constructor


    Public Operators

      XP_GIS_PolygonNode_c &operator=(const XP_GIS_PolygonNode_c &object);
        The assignment operator assigns all values as necessary.  It 
        allocates space for a copy of the child list and then populates
        with data from object.

    Public Methods

      void SetBounds(double  XMin,
                     double  YMin,
                     double  XMax,
                     double  YMax);
        Method to set the polygon bounds.

      void GetBounds(double *XMin,
                     double *YMin,
                     double *XMax,
                     double *YMax) const;
        Method to get the polygon bounds.

      XP_GIS_VertexList_c &VertexList(XP_GIS_VertexList_c &VertexList);
        Method to set the vertex list object.

      XP_GIS_VertexList_c &VertexList() const;
        Method to get the vertex list object.

      unsigned long SetVertices(unsigned long        NumberOfVertices,
                                unsigned long const *Vertices);
        Method to set the polygon vertex list.  If successful, this 
        method returns XP_GIS_OK.  Otherwise it returns one of:
          XP_GIS_ALLOC_ERROR
          XP_GIS_ERROR

      unsigned long GetVertices(unsigned long        *NumberOfVertices,
                                unsigned long const **Vertices) const;
        Method to get the polygon vertex list.  If successful, this 
        method returns XP_GIS_OK.  Otherwise it returns one of:
          XP_GIS_ERROR

      unsigned long NumberOfVertices() const;
        Method to get the number of vertices in the polygon stored at
        this node.

      void SwapVertexOrder();
        Method to swap the order of the vertices in a polygon.

      unsigned long Inside(XP_GIS_PolygonNode_c &PolygonTreeNode);
        Method to determine if the polygon represented by this object
        is inside the polygon represented by PolygonTreeNode.  This
        method returns either XP_GIS_TRUE or XP_GIS_FALSE.

      unsigned long Contains(XP_GIS_PolygonNode_c &PolygonTreeNode);
        Method to determine if the polygon represented by this object
        contains the polygon represented by PolygonTreeNode.  This
        method returns either XP_GIS_TRUE or XP_GIS_FALSE.

*/



#ifndef XP_GIS_POLYGON_NODE_H
#define XP_GIS_POLYGON_NODE_H

#include <malloc.h>

#include "gsconst.h"
#include "gserror.h"

#include "gsnode.h"
#include "gsvrtlst.h"

class XP_GIS_PolygonNode_c : public XP_GIS_TreeNode_c
{

  public:

    XP_GIS_PolygonNode_c();
    XP_GIS_PolygonNode_c(XP_GIS_VertexList_c &VertList,
                         unsigned long NumberOfVertices = 0,
                         unsigned long const *Vertices = NULL,
                         double XMin = 0,
                         double YMin = 0,
                         double XMax = 0,
                         double YMax = 0);
    virtual ~XP_GIS_PolygonNode_c();
    XP_GIS_PolygonNode_c(const XP_GIS_PolygonNode_c &object);
    XP_GIS_PolygonNode_c &operator=(const XP_GIS_PolygonNode_c &object);

    void SetBounds(double  XMin,
                   double  YMin,
                   double  XMax,
                   double  YMax);
    void GetBounds(double *XMin,
                   double *YMin,
                   double *XMax,
                   double *YMax) const;
    XP_GIS_VertexList_c &VertexList(XP_GIS_VertexList_c &VertList)
                       {return *(_VertexList = &VertList);}
    XP_GIS_VertexList_c &VertexList() const {return *_VertexList;}
    unsigned long SetVertices(unsigned long        NumberOfVertices,
                              unsigned long const *Vertices);
    unsigned long GetVertices(unsigned long        *NumberOfVertices,
                              unsigned long const **Vertices) const;
    unsigned long NumberOfVertices() const;

    void SwapVertexOrder();

    unsigned long Inside(XP_GIS_PolygonNode_c &PolygonTreeNode);
    unsigned long Contains(XP_GIS_PolygonNode_c &PolygonTreeNode);

  private:

    //
    //  returns true/false.  True if polygon 1 is inside polygon 2
    //

    unsigned long _Inside(XP_GIS_PolygonNode_c &Polygon1,
                          XP_GIS_PolygonNode_c &Polygon2);
    unsigned long _ComputeBounds();
    unsigned long _PointInPolygon(double XPoint,
                                  double YPoint) const;

    XP_GIS_VertexList_c *_VertexList;
    unsigned long        _NumberOfVertices;
    unsigned long       *_Vertices;
    double               _XMin;
    double               _YMin;
    double               _XMax;
    double               _YMax;
};

#endif   // XP_GIS_POLYGON_NODE_H


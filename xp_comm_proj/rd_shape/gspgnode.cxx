#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

#include "gserror.h"

#include "gspgnode.h"

//
//  The default constructor.
//

XP_GIS_PolygonNode_c::XP_GIS_PolygonNode_c()
{
    _VertexList = NULL;
    _NumberOfVertices = 0;
    _Vertices = NULL;
}

//
//  The overloaded constructor.
//

XP_GIS_PolygonNode_c::XP_GIS_PolygonNode_c(XP_GIS_VertexList_c &VertList,
                                           unsigned long        NumberOfVertices,
                                           unsigned long const *Vertices,
                                           double               XMin,
                                           double               YMin,
                                           double               XMax,
                                           double               YMax)
{
    VertexList(VertList);
    _NumberOfVertices = 0;
    _Vertices = NULL;
    SetVertices(NumberOfVertices,Vertices);
    SetBounds(XMin,YMin,XMax,YMax);
}

//
//  The destructor.
//

XP_GIS_PolygonNode_c::~XP_GIS_PolygonNode_c()
{
    if (_Vertices!= NULL) free(_Vertices);
}

//
//  The copy constructor.
//

XP_GIS_PolygonNode_c::XP_GIS_PolygonNode_c(const XP_GIS_PolygonNode_c &object)
{
    *this = object;
}

//
//  The assignment operator.  This operator allocates space for a new
//  vertex list and populates it with the vertex list from the input object.
//

XP_GIS_PolygonNode_c &XP_GIS_PolygonNode_c::operator=(const XP_GIS_PolygonNode_c &object)
{
    VertexList(object.VertexList());
    _NumberOfVertices = 0;
    _Vertices = NULL;
    SetVertices(object._NumberOfVertices,
                object._Vertices);
    SetBounds(object._XMin,object._YMin,object._XMax,object._YMax);
    return *this;
}

void XP_GIS_PolygonNode_c::SetBounds(double  XMin,
                                     double  YMin,
                                     double  XMax,
                                     double  YMax)
{
    _XMin = XMin;
    _YMin = YMin;
    _XMax = XMax;
    _YMax = YMax;
}

void XP_GIS_PolygonNode_c::GetBounds(double *XMin,
                                     double *YMin,
                                     double *XMax,
                                     double *YMax) const
{
    *XMin = _XMin;
    *YMin = _YMin;
    *XMax = _XMax;
    *YMax = _YMax;
}

unsigned long XP_GIS_PolygonNode_c::SetVertices(
                            unsigned long        NumberOfVertices,
                            unsigned long const *Vertices)
{
    if (_Vertices != NULL) free(_Vertices);
    _NumberOfVertices = 0;

    if (NumberOfVertices > 0)
    {
      _Vertices = (unsigned long *) malloc(sizeof(unsigned long) * 
                                           NumberOfVertices);
      if (_Vertices == NULL)
      {
        _NumberOfVertices = 0;
        return XP_GIS_ALLOC_ERROR;
      }
      memcpy(_Vertices,Vertices,sizeof(unsigned long) * NumberOfVertices);
      _NumberOfVertices = NumberOfVertices;
    }
    return XP_GIS_OK;
}
unsigned long XP_GIS_PolygonNode_c::GetVertices(
                            unsigned long        *NumberOfVertices,
                            unsigned long const **Vertices) const
{
    *NumberOfVertices = _NumberOfVertices;
    *Vertices = _Vertices;
    return XP_GIS_OK;
}

unsigned long XP_GIS_PolygonNode_c::NumberOfVertices() const
{
    return _NumberOfVertices;
}

void XP_GIS_PolygonNode_c::SwapVertexOrder()
{
    unsigned long VertexNumber;
    unsigned long Vertex1;
    unsigned long Vertex2;
    unsigned long TempVal;

    //
    //  Loop through half the vertices and swap the vertices from
    //  the first half of the list with the vertex from the second
    //  half of the list.  Note that the loop terminator is the
    //  # of vertices divided by two.  We use integer division
    //  (with truncation) to ensure we don't do the middle vertex
    //  if the # of vertices is odd.
    //

    for (VertexNumber = 0;
         VertexNumber < _NumberOfVertices / 2;
         VertexNumber++)
    {
      Vertex1 = VertexNumber;
      Vertex2 = (_NumberOfVertices - 1) - VertexNumber;
      TempVal = _Vertices[Vertex1];
      _Vertices[Vertex1] = _Vertices[Vertex2];
      _Vertices[Vertex2] = TempVal;
    }
}

unsigned long XP_GIS_PolygonNode_c::Inside(
                            XP_GIS_PolygonNode_c &PolygonTreeNode)
{
    return _Inside(*this,PolygonTreeNode);
}
unsigned long XP_GIS_PolygonNode_c::Contains(
                            XP_GIS_PolygonNode_c &PolygonTreeNode)
{
    return _Inside(PolygonTreeNode,*this);
}

unsigned long XP_GIS_PolygonNode_c::_Inside(
                               XP_GIS_PolygonNode_c  &Polygon1,
                               XP_GIS_PolygonNode_c  &Polygon2) 
{
    unsigned long VertexNumber;

    //
    //  If the bounds have not been set yet compute them and set them.
    //

    if ((Polygon1._XMin == Polygon1._XMax) &&
        (Polygon1._YMin == Polygon1._YMax))
    {
      Polygon1._ComputeBounds();
    }

    if ((Polygon2._XMin == Polygon2._XMax) &&
        (Polygon2._YMin == Polygon2._YMax))
    {
      Polygon2._ComputeBounds();
    }

    //
    //  Now, do a simple bounds check on the two polygons.  If the bounding
    //  hulls do not overlap, then we can't have one cannot be inside
    //  the other.  In this case, return false.
    //

    if ((Polygon1._XMin > Polygon2._XMax) ||     // 1 above 2
        (Polygon1._XMax < Polygon2._XMin) ||     // 1 below 2
        (Polygon1._YMin > Polygon2._YMax) ||     // 1 right of 2
        (Polygon1._YMax < Polygon2._YMin))       // 1 left of 2
    {
      return XP_GIS_FALSE;
    }

    //
    //  OK, we could not do a simple rejection.  Now find a vertex from
    //  polygon 1 that is not coincident with a vertex from polygon 2.
    //  We will test to see if that vertex is inside polygon 2.  If we
    //  find that all of polygon 1's vertices are coincident with 
    //  polygon 2's vertices, assume the polygon is inside (return true).
    //

    for (VertexNumber = 0;
         VertexNumber < Polygon1._NumberOfVertices;
         VertexNumber++)
    {
      if (Polygon1._Vertices[VertexNumber] != 
          Polygon2._Vertices[VertexNumber])
      {
        break;
      }
    }
    if (VertexNumber == Polygon1._NumberOfVertices)
    {
      return XP_GIS_TRUE;
    }

    //
    //  Now use the point in polygon routine to see if the point from
    //  polygon 1 is inside polygon 2.
    //

    VertexNumber = Polygon1._Vertices[VertexNumber];
    return Polygon2._PointInPolygon(
                 Polygon1._VertexList->XVertices()[VertexNumber],
                 Polygon1._VertexList->YVertices()[VertexNumber]);
}
unsigned long XP_GIS_PolygonNode_c::_ComputeBounds()
{
    unsigned long NumberOfVertices;   // # in feature, not ring.
    unsigned long VertexNumber;
    const double *XVertices;
    const double *YVertices;
    double        XVertex;
    double        YVertex;

    //
    //  Get the pointers to the veryices
    //

    NumberOfVertices = _VertexList->NumberOfVertices();
    XVertices = _VertexList->XVertices();
    YVertices = _VertexList->YVertices();

    //
    //  Validate vertex info.
    //

    if (NumberOfVertices < _NumberOfVertices)
    {
      return XP_GIS_ERROR;
    }

    if (XVertices == NULL)
    {
      return XP_GIS_ERROR;
    }

    if (YVertices == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  Now compute and set the min and max values.
    //

    _XMin = DBL_MAX;
    _YMin = DBL_MAX;
    _XMax = -DBL_MAX;
    _YMax = -DBL_MAX;
    for (VertexNumber = 0;
         VertexNumber < _NumberOfVertices;
         VertexNumber++)
    {
      XVertex = XVertices[_Vertices[VertexNumber]];
      YVertex = YVertices[_Vertices[VertexNumber]];
      _XMin = MIN(XVertex,_XMin);
      _YMin = MIN(YVertex,_YMin);
      _XMax = MAX(XVertex,_XMax);
      _YMax = MAX(YVertex,_YMax);
    }
    return XP_GIS_OK;
}

//
//  Method to see if a point is inside a polygon.  The polygon can be
//  convext or concave and can be either clockwise or counterclockwise.
//  The polygon can be either open or closed.
//
//  A ray is projected from the locator point in the positive direction
//  along the X axis.  If this ray intersects the polygon outline an odd
//  number of times, then the point is inside the polygon.
//
//  Inputs:
//     VertexList        a vertex list object containing the vertex list
//                       for all rings in the feature.
//     PointNumber       the index to the point's vertices in VertexList
//     NumberOfVertices  # of vertices in polygon
//     VertexIndices     array of indices into VertexList for the
//                       polygon's vertices.
//
//  This function returns XP_GIS_TRUE if the point is inside or on the
//  border of the polygon.  Otherwise it returns XP_GIS_FALSE.
//
//  This method assumes that error checking has been performed before
//  it is invoked.  Specifically, it is assumed that PointNumber and
//  all elements of the VertexIndices array point to valid vertex
//  locations in VertexList.
//

unsigned long XP_GIS_PolygonNode_c::_PointInPolygon(double XPoint,
                                                    double YPoint) const
{
    int closed;
    int k;
    int l;
    unsigned long hit = 0;
    unsigned long num_edges;
    unsigned long i;
    unsigned long j;
    double slope;
    double xbeg;
    double ybeg;
    double xend;
    double yend;
    double x1;
    double y1;
    double x2;
    double y2;
    double yprev;
    double dy1;
    double dy2;
    double xmin;
    double xmax;
    double dx;
    double dy;
    double y_intersection;
    const double *XVertices;
    const double *YVertices;

    //
    //  Get pointers to the vertex arrays from the vertex list object.
    //

    XVertices = _VertexList->XVertices();
    YVertices = _VertexList->YVertices();

    //
    //  Validate the vertex arrays.
    //

    if (XVertices == NULL)
    {
      return XP_GIS_FALSE;
    }

    if (YVertices == NULL)
    {
      return XP_GIS_FALSE;
    }

    //
    // Determine if the polygon is closed.  It is closed if the first
    // vertex is duplicated at the end of the list.
    //

    closed = (XVertices[_Vertices[0]] == 
              XVertices[_Vertices[_NumberOfVertices - 1]] &&
              YVertices[_Vertices[0]] == 
              YVertices[_Vertices[_NumberOfVertices - 1]]);

    //
    // Loop through all edges in the polygon
    //

    num_edges = (closed ? (_NumberOfVertices - 1) : _NumberOfVertices);

    for (i = 0; i < num_edges; i++) 
    {

      //
      // Get the current edge definition 
      //

      if (! closed && i == num_edges - 1)
      {
        xbeg = XVertices[_Vertices[i]];
        ybeg = YVertices[_Vertices[i]];
        xend = XVertices[_Vertices[0]];
        yend = YVertices[_Vertices[0]];
      }
      else
      {
        xbeg = XVertices[_Vertices[i]];
        ybeg = YVertices[_Vertices[i]];
        xend = XVertices[_Vertices[i + 1]];
        yend = YVertices[_Vertices[i + 1]];
      }

      //
      // Classify the current edge as a vector from x1,y1 to x2,y2,
      // where y2 >= y1
      //

      if (yend <= ybeg)
      {
        x1 = xend;
        y1 = yend;
        x2 = xbeg;
        y2 = ybeg;
      }
      else
      {
        x1 = xbeg;
        y1 = ybeg;
        x2 = xend;
        y2 = yend;
      }

      //
      // There is definitely no crossing if the edge is entirely 
      // above or below the ray
      //

      if (y1 > YPoint || y2 < YPoint) continue;

      //
      // Determine the extreme X values
      //

      if (xend >= xbeg)
      {
        xmax = xend;
        xmin = xbeg;
      }
      else
      {
        xmax = xbeg;
        xmin = xend;
      }

      //
      // There is definitely no crossing if the edge is entirely 
      // to the left of the ray
      //

      if (xmax < XPoint) continue;

      //
      // Skip the current edge if it is horizontal, unless the locator
      // point is on the edge.  In that case, a direct hit is assured.
      //

      if (y2 == y1)
      {
        if (XPoint >= xmin && XPoint <= xmax)
        {
          return(1);
        }
        continue;
      }

      //
      // Determine if the current edge begins on the scan line
      // containing the ray
      //

      if (ybeg == YPoint) 
      {

        //
        // Skip the edge if it begins to the left of the ray
        //

        if (xbeg < XPoint) continue;

        //
        // If the current edge begins at the origin of the ray,
        // a polygon vertex was directly hit
        //

        if (xbeg == XPoint)
        {
          return(1);
        }

        //
        // If the current edge begins on the ray, we consider this
        // an intersection if and only if the two neighboring vertices
        // are on the same side of the ray.
        //
        // First, find the the Y coordinate of the previous vertex.
        // Note that special action must be taken if we are at the
        // first vertex.
        //

        if (i) 
        {
          k = i - 1;
        }
        else if (closed) 
        {
          k = _NumberOfVertices - 2;
        }
        else 
        {
          k = _NumberOfVertices - 1;
        }

        //
        // We need to make sure the previous point has a
        // different Y value - meaning the previous segment
        // is not a point. Otherwise, we might detect a hit
        // when there really isn't one.
        //
        // The loop is at max the number of edges. We need to
        // handle to case where we may wrap back around to the
        // end of the vert list - the case where we start out
        // with the Y values being the same.
        //

        k += _NumberOfVertices;
        for (j=0; j<num_edges; j++, k--)
        {
          l = k % _NumberOfVertices;
          if (YVertices[_Vertices[l]] != ybeg)
          {
            yprev = YVertices[_Vertices[l]];
            break;
          }
        }

        //
        // Find the Y delta for the previous and current edges
        //

        dy1 = ybeg - yprev;
        dy2 = yend - ybeg;

        //
        // If the deltas have opposite signs, both adjacent vertices are
        // on the same side of the ray.  Indicate a crossing by toggling
        // the state of the hit flag.
        //

        if ((dy1 <= 0 && dy2 > 0) || (dy1 > 0 && dy2 <= 0))
        {
          hit = (! hit);
        }
      }
      else if (xmin > XPoint || x2 == x1) 
      {

        //
        // If the edge is entirely to the right of the ray, or if it
        // is vertical, indicate a crossing.
        //

        hit = (! hit);
      }
      else
      {

        //
        // The edge ends to the right of the ray, but begins to the
        // left of it.  Determine the slope of the edge.
        //

        dx = x2 - x1;
        dy = y2 - y1;
        slope =  dy/ dx;

        //
        // Determine the Y coordinate of the intersection of the edge
        // with the vertical line passing through the locator point
        //

        y_intersection = y1 + (slope * (XPoint - x1));

        //
        // If the locator point is on the edge, a direct hit occurred
        //

        if (y_intersection == YPoint)
        {
          return(1);
        }

        //
        // Indicate a crossing if the locator point is above an
        // ascending edge or below a descending edge
        //

        if (y_intersection > YPoint)
        {
          if (slope < 0.0) hit = (! hit);
        }
        else if (slope > 0.0)
        {
          hit = (! hit);
        }
      }
    }

    //
    // Indicate if the polygon was hit
    //

    return(hit);
}

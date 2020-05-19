//
//  Include file for the XP_GIS_VertexList class, which provides utilities
//  to managing a vertex list.
//

/*

    Public Constructors

      XP_GIS_VertexList_c(unsigned long NumberOfVertices = 0,
                          double const *XVertices = NULL,
                          double const *YVertices = NULL);
        The constructor.

      virtual ~XP_GIS_VertexList_c();
        The destructor.

      XP_GIS_VertexList_c(const XP_GIS_VertexList_c &object);
        The copy constructor


    Public Operators

      XP_GIS_VertexList_c &operator=(const XP_GIS_VertexList_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods
      void SetVertices(unsigned long  NumberOfVertices,
                       double const  *XVertices,
                       double const  *YVertices);
        Method to set the vertices.  A copy of the vertices is NOT made.
        This method just assigns the pointers.  The vertices must reside
        in memory that persists as long as this object persists.

      void GetVertices(unsigned long  *NumberOfVertices,
                       double const  **XVertices,
                       double const  **YVertices) const;
        Method to get the vertices.

      unsigned long NumberOfVertices();
        Method to get the number of vertices in the vertex list.

      const double *XVertices() const;
        Method to get the X vertex list.

      const double *YVertices() const;
        Method to get the Y vertex list.

*/



#ifndef XP_GIS_VERTEX_LIST_H
#define XP_GIS_VERTEX_LIST_H

#include <malloc.h>

#include "gsconst.h"
#include "gserror.h"

class XP_GIS_VertexList_c
{

  public:

    XP_GIS_VertexList_c(unsigned long NumberOfVertices = 0,
                        double const *XVertices = NULL,
                        double const *YVertices = NULL);
    virtual ~XP_GIS_VertexList_c();
    XP_GIS_VertexList_c(const XP_GIS_VertexList_c &object);
    XP_GIS_VertexList_c &operator=(const XP_GIS_VertexList_c &object);

    void SetVertices(unsigned long  NumberOfVertices,
                     double const  *XVertices,
                     double const  *YVertices);
    void GetVertices(unsigned long  *NumberOfVertices,
                     double const  **XVertices,
                     double const  **YVertices) const;
    unsigned long NumberOfVertices() {return _NumberOfVertices;}
    const double *XVertices() const {return _XVertices;}
    const double *YVertices() const {return _YVertices;}

  private:

    unsigned long  _NumberOfVertices;
    double        *_XVertices;
    double        *_YVertices;
};

#endif   // XP_GIS_VERTEX_LIST_H


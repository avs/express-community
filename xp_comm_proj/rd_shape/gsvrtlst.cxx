#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "gserror.h"

#include "gsvrtlst.h"

//
//  The constructor.
//

XP_GIS_VertexList_c::XP_GIS_VertexList_c(unsigned long NumberOfVertices,
                                         double const   *XVertices,
                                         double const   *YVertices)
{
    SetVertices(NumberOfVertices,
                XVertices,
                YVertices);
}

//
//  The destructor.
//

XP_GIS_VertexList_c::~XP_GIS_VertexList_c()
{
}

//
//  The copy constructor.
//

XP_GIS_VertexList_c::XP_GIS_VertexList_c(const XP_GIS_VertexList_c &object)
{
    *this = object;
}

//
//  The assignment operator.
//

XP_GIS_VertexList_c &XP_GIS_VertexList_c::operator=(const XP_GIS_VertexList_c &object)
{
    SetVertices(object._NumberOfVertices,
                object._XVertices,
                object._YVertices);
    return *this;
}

//
//  Method to set the vertex list.  This method does not copy the list.
//  It only copies the pointer to the list.
//

void XP_GIS_VertexList_c::SetVertices(unsigned long  NumberOfVertices,
                                      double const  *XVertices,
                                      double const  *YVertices)
{
    _NumberOfVertices = NumberOfVertices;
    _XVertices = (double *) XVertices;
    _YVertices = (double *) YVertices;

}

//
//  Method to get the vertex list.
//

void XP_GIS_VertexList_c::GetVertices(unsigned long  *NumberOfVertices,
                                      double const  **XVertices,
                                      double const  **YVertices) const
{
    *NumberOfVertices = _NumberOfVertices;
    *XVertices = _XVertices;
    *YVertices = _YVertices;

}


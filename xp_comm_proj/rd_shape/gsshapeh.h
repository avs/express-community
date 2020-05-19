//
//  Include file for the XP_GIS_ESRI_ShapeHeader_c class, which provides
//  utilities to read and manipulate an ESRI shape file header.
//

/*

    Public Constructors

      XP_GIS_ESRI_ShapeHeader_c(const char *FileName=NULL);
        The constructor.

      virtual ~XP_GIS_ESRI_ShapeHeader_c();
        The destructor.

      XP_GIS_ESRI_ShapeHeader_c(const XP_GIS_ESRI_ShapeHeader_c &object);
        The copy constructor.


    Public Operators

      XP_GIS_ESRI_Shape_c &operator(const XP_GIS_ESRI_ShapeHeader_c &object);
        The assignment operator.


    Public Methods

      ifstream &FileStream(ifstream &Stream);
        Method to set the stream for the shape file.

      ifstream &FileStream() {return *_FileStream;}
        Method to get the stream for the shape file.

      const char *FileName(const char *Name);
        Method to set the name for the shape file.

      const char *FileName() {return _FileName;}
        Method to get the name for the shape file.

      long HeaderLength();
        Method to get the shape file header length in bytes.

      unsigned long FileCode() const;
        Method to return the value of the file code from the currently
        opened shape file.

      unsigned long FileLength() const;
        Method to return the value of the file length (in byes) from 
        the currently opened shape file.

      unsigned long Version() const;
        Method to return the value of the file version from the currently
        opened shape file.

      unsigned long ShapeType() const;
        Method to return the value of the shape type from the currently
        opened shape file.  0 = null, 1 = point, 3 = arc, 5 = polygon,
        and 8 = polypoint.

      double XMinimum() const;
        Method to return the value of the x min from the currently
        opened shape file.

      double YMinimum() const;
        Method to return the value of the y min from the currently
        opened shape file.

      double XMaximum() const;
        Method to return the value of the x max from the currently
        opened shape file.

      double YMaximum() const;
        Method to return the value of the y max from the currently
        opened shape file.

      unsigned long ReadHeader();
        Method to read the header from a .shp or .shx file.

      unsigned long ValidateFileHeader();
        Method to validate the shape file header.  Returns XP_GIS_OK
        if the header is valid.  Otherwise, it returns on of the error
        codes listed below:
          XP_GIS_BAD_MAGIC_NUMBER
          XP_GIS_BAD_VERSION
          XP_GIS_BAD_SHAPE_TYPE
          XP_GIS_BAD_BAD_BOUNDING_BOX
      
      void PrintFileHeader(ostream &PrintStream);
        Method to format and print the shape file header to an already-
        opened output stream.

*/



#ifndef XP_GIS_SHAPE_HEADER_H
#define XP_GIS_SHAPE_HEADER_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "gsesri.h"

class XP_GIS_ESRI_ShapeHeader_c
{
  public:
    XP_GIS_ESRI_ShapeHeader_c();
    virtual ~XP_GIS_ESRI_ShapeHeader_c();
    XP_GIS_ESRI_ShapeHeader_c(const XP_GIS_ESRI_ShapeHeader_c &object);
    XP_GIS_ESRI_ShapeHeader_c &operator=(
                          const XP_GIS_ESRI_ShapeHeader_c &object);

    ifstream &FileStream(ifstream &Stream);
    ifstream &FileStream() {return *_FileStream;}
    const char *FileName(const char *Name);
    const char *FileName() {return _FileName;}

    long HeaderLength();
    long FileCode() const {return _FileCode;}
    long FileLength() const {return _FileLength;}
    long Version() const {return _Version;}
    long ShapeType() const {return _ShapeType;}
    double XMinimum() const {return _XMin;}
    double YMinimum() const {return _YMin;}
    double XMaximum() const {return _XMax;}
    double YMaximum() const {return _YMax;}

    unsigned long ReadHeader();
    unsigned long ValidateFileHeader();
    unsigned long PrintFileHeader(ostream &PrintStream);

  private:

    ifstream *_FileStream;            // shape file stream
    char     *_FileName;              // shape file name
    long      _FileCode;              // magic number
    long      _FileLength;            // file length in bytes
    long      _Version;               // file version (only 1000 supported)
    long      _ShapeType;             // shape type (0 null, 1 point, 3 arc,
                                      // 5 polygon, 8 polypoint)
    double    _XMin;                  // bounding box x min
    double    _YMin;                  // bounding box x min
    double    _XMax;                  // bounding box x max
    double    _YMax;                  // bounding box y max
};

#endif   // XP_GIS_SHAPE_HEADER_H

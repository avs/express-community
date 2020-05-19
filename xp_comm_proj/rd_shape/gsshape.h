//
//  Include file for the XP_GIS_ESRI_Shape_c class, which provides utilities
//  to read an ESRI shape file.
//

/*

    Public Constructors

      XP_GIS_ESRI_Shape_c(const char *FileName=NULL);
        The constructor.

      virtual ~XP_GIS_ESRI_Shape_c();
        the destructor.

      XP_GIS_ESRI_Shape_c(const XP_GIS_ESRI_Shape_c &object);
        The copy constructor

    Public Operators

      XP_GIS_ESRI_Shape_c &operator=(const XP_GIS_ESRI_Shape_c &object);
        The assignment operator assigns all values as necessary.  It
        is implemented by invoking the FileName method.  Therefore, "this"
        closes any open files, opens the files in object, and sets up all
        its private properties.  Since this object opens all files read-only,
        there is no chance for conflict between open files.

    Public Methods

      const char *FileName(const char* FileName);
        Method to set the file name.  If there is already a file opened,
        this method closes the file before attempting to open the newly
        specified file.  Returns NULL on failure.  Otherwise returns the
        named of the newly opened file.

      const char *FileName() const;
        Method to return a pointer to the currently set file name.
        The name returned does not include the suffix necessary to
        access one of the three files.  Appending a suffix of .shp,
        .shx, and .dbf will create the names of the shape file, 
        index file, and dBASE file, respectively.

      const char *ShapeFileName(const char* ShapeFileName);
        Method to set the shape file name.  If there is already a file
        opened, this method closes the file before attempting to open
        the newly specified file.  Returns NULL on failure.  Otherwise
        returns the name of the newly opened file.

      const char *ShapeFileName() const;
        Method to return a pointer to the currently set shape file name.

      const char *IndexFileName(const char* ShapeFileName);
        Method to set the index file name.  If there is already a file
        opened, this method closes the file before attempting to open
        the newly specified file.  Returns NULL on failure.  Otherwise
        returns the name of the newly opened file.

      const char *IndexFileName() const {return _IndexFileName;}; 
        Method to return a pointer to the currently set index file name.

      unsigned char ShapeFileIsOpen() const {return _ShapeFileIsOpen;}
        Method to return value of true/false flag that indicates whether
        or not the shape file is open.  Return XP_GIS_TRUE if the file
        is open.  Otherwise return XP_GIS_FALSE.

      unsigned char IndexFileIsOpen() const {return _IndexFileIsOpen;}
        Method to return value of true/false flag that indicates whether
        or not the index file is open.  Return XP_GIS_TRUE if the file
        is open.  Otherwise return XP_GIS_FALSE.

      unsigned long GetArcShapeInfo(long *RecordNumber,
                                    long *NumberOfParts,
                                    long *NumberOfPoints);
        Method to get the number of parts and number of points for a
        give arc feature.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long GetPolygonShapeInfo(long *RecordNumber,
                                        long *NumberOfParts,
                                        long *NumberOfPoints);
        Method to get the number of parts and number of points for a
        give polygon feature.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long GetMultiPointShapeInfo(long *RecordNumber,
                                           long *NumberOfPoints);
        Method to get the number of points for a give multipoint feature.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long NumberOfDataRecords(unsigned long *NumberOfRecords);
        Method to get the number of data records in the shape file.
        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_NOT_IMPLEMENTED
          XP_GIS_ERROR

      unsigned long NumberOfParts(unsigned long *NumberOfParts);
        Method to get the total number of parts in the shape file.
        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_NOT_IMPLEMENTED
          XP_GIS_ERROR

      unsigned long NumberOfPoints(unsigned long *NumberOfPoints);
        Method to get the total number of points in the shape file.
        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_NOT_IMPLEMENTED
          XP_GIS_ERROR

      unsigned long SeekToFirstDataRecord();
        Method to seek to the first data record in the file.
        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_SEEK_ERROR
          XP_GIS_IO_ERROR

      unsigned long SeekToDataRecord(long RecordNumber);
        Method to seek to the start of data record number RecordNumber in
        the .shp file.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_SEEK_ERROR
          XP_GIS_ERROR

      unsigned long ReadNullShape();
        Method to read a null shape.  All it does is return with the
        appropriate code.  To find out the number of null features,
        use the NumberOfDataRecords method.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
 
      unsigned long ReadPointShape(long   *RecordNumber,
                                   double *PointX,
                                   double *PointY);
        Method to read a point shape.  RecordNumber is popluated with the
        record number in the shape file for this point.  This is the
        feature number.  PointX and PointY are populated with the point
        feature's x and y coordinates, respectively.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long ReadArcShape(long    *RecordNumber,
                                 double  *XMin,
                                 double  *YMin,
                                 double  *XMax,
                                 double  *YMax,
                                 long    *NumberOfParts,
                                 long    *MaximumNumberOfParts,
                                 long   **PartIndex,
                                 long    *NumberOfPoints,
                                 long    *MaximumNumberOfPoints,
                                 double **XArray,
                                 double **YArray);
        Method to read an arc shape.  RecordNumber is popluated with the
        record number in the shape file for this arc.  This is the
        feature number.  XMin, YMin, XMax, and YMax are populated with the
        corners of the bounding box around the arc.

        Each arc can consist of one or more polylines.  Each polyline
        is called a part.  NumberOfParts is populated with the number
        of polylines in the arc feature.  The coordinates for the
        arc feature are returned in XArray and YArray.  PartIndex is an
        array of indices into XArray/YArray, with one entry per part.
        Each entry in PartIndex is a zero-based index into XArray/YArray
        where the part starts.

        NumberOfPoints is populated with the number of points in each of 
        XArray and YArray.

        The number of parts and number of points are unknown until the 
        feature is read.  For better performance, it is undesirable to
        allocate and free PartIndex, XArray, and YArray for each call.
        Therefore, MaximumNumberOfParts contains the size of the PartIndex
        array.  This array can be re-sized by ReadArcShape as necessary.
        Likewise, MaximumNumberOfPoints contains the size of XArray and
        YArray.  These arrays can also be re-sized by ReadArcShape as
        necessary.

        If MaximumNumberOfParts and/or MaximumNumberOfPoints are zero,
        it is assumed that the corresponding array(s) have not yet been
        allocated, and they are then automatically allocated by this method.

        Note that PartIndex, XArray, and YArray are intended for the
        temporary storage of the returned values.  The values should be
        copied from these arrays to a more permanent location before the
        arrays are passed into a subsequent call to this method.

        After the last feature is read, these array should be freed.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long ReadPolygonShape(long    *RecordNumber,
                                     double  *XMin,
                                     double  *YMin,
                                     double  *XMax,
                                     double  *YMax,
                                     long    *NumberOfParts,
                                     long    *MaximumNumberOfParts,
                                     long   **PartIndex,
                                     long    *NumberOfPoints,
                                     long    *MaximumNumberOfPoints,
                                     double **XArray,
                                     double **YArray);
        Method to read a polygon shape.  RecordNumber is popluated with the
        record number in the shape file for this polygon.  This is the
        feature number.  XMin, YMin, XMax, and YMax are populated with the
        corners of the bounding box around the polygon.

        Each polygon can consist of one or more rings.  Each ring
        is called a part.  NumberOfParts is populated with the number
        of rings in the polygon feature.  The coordinates for the
        polygon feature are returned in XArray and YArray.  PartIndex is an
        array of indices into XArray/YArray, with one entry per part.
        Each entry in PartIndex is a zero-based index into XArray/YArray
        where the part starts.

        NumberOfPoints is populated with the number of points in each of 
        XArray and YArray.

        The number of parts and number of points are unknown until the 
        feature is read.  For better performance, it is undesirable to
        allocate and free PartIndex, XArray, and YArray for each call.
        Therefore, MaximumNumberOfParts contains the size of the PartIndex
        array.  This array can be re-sized by ReadPolygonShape as necessary.
        Likewise, MaximumNumberOfPoints contains the size of XArray and
        YArray.  These arrays can also be re-sized by ReadPolygonShape as
        necessary.

        If MaximumNumberOfParts and/or MaximumNumberOfPoints are zero,
        it is assumed that the corresponding array(s) have not yet been
        allocated, and they are then automatically allocated by this method.

        Note that PartIndex, XArray, and YArray are intended for the
        temporary storage of the returned values.  The values should be
        copied from these arrays to a more permanent location before the
        arrays are passed into a subsequent call to this method.

        After the last feature is read, these array should be freed.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      unsigned long ReadMultiPointShape(long    *RecordNumber,
                                        double  *XMin,
                                        double  *YMin,
                                        double  *XMax,
                                        double  *YMax,
                                        long    *NumberOfPoints,
                                        long    *MaximumNumberOfPoints,
                                        double **XArray,
                                        double **YArray);
        Method to read a multipoint shape.  RecordNumber is popluated with
        the record number in the shape file for this multipoint.  This is
        the feature number.  XMin, YMin, XMax, and YMax are populated with 
        the corners of the bounding box around the multipoint.

        Each multipoint is simply an ordered collection of points.
        NumberOfPoints is populated with the number of vertices in the
        multipoint feature.  The coordinates are returned in XArray
        and YArray.

        The number of points is unknown until the feature is read.  For
        better performance, it is undesirable to allocate and free XArray
        and YArray for each call.  Therefore, MaximumNumberOfPoints contains
        the size of XArray and YArray.  These arrays can be re-sized by
        ReadMultipointShape as necessary.

        If MaximumNumberOfParts and/or MaximumNumberOfPoints are zero,
        it is assumed that the corresponding array(s) have not yet been
        allocated, and they are then automatically allocated by this method.

        Note that XArray and YArray are intended for the temporary storage
        of the returned values.  The values should be copied from these
        arrays to a more permanent location before the arrays are passed
        into a subsequent call to this method.

        After the last feature is read, these array should be freed.

        If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
        the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_ERROR
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

*/



#ifndef XP_GIS_SHAPE_H
#define XP_GIS_SHAPE_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "gsconst.h"
#include "gsesri.h"
#include "gsshapeh.h"

class XP_GIS_ESRI_Shape_c
{
  public:
    XP_GIS_ESRI_Shape_c(const char *FileName=NULL);
    virtual ~XP_GIS_ESRI_Shape_c();
    XP_GIS_ESRI_Shape_c(const XP_GIS_ESRI_Shape_c &object);
    XP_GIS_ESRI_Shape_c &operator=(const XP_GIS_ESRI_Shape_c &object);

    const char *FileName(const char* FileName);
    const char *FileName() const {return _FileName;} 
    const char *ShapeFileName(const char* ShapeFileName);
    const char *ShapeFileName() const {return _ShapeFileName;} 
    const char *IndexFileName(const char* IndexFileName);
    const char *IndexFileName() const {return _IndexFileName;}
    unsigned char ShapeFileIsOpen() const {return _ShapeFileIsOpen;}
    unsigned char IndexFileIsOpen() const {return _IndexFileIsOpen;}

    XP_GIS_ESRI_ShapeHeader_c &ShapeHeader() {return _ShapeHeader;}
    XP_GIS_ESRI_ShapeHeader_c &IndexHeader() {return _IndexHeader;} 

    unsigned long GetArcShapeInfo(long *RecordNumber,
                                  long *NumberOfParts,
                                  long *NumberOfPoints);
    unsigned long GetPolygonShapeInfo(long *RecordNumber,
                                      long *NumberOfParts,
                                      long *NumberOfPoints);
    unsigned long GetMultiPointShapeInfo(long *RecordNumber,
                                         long *NumberOfPoints);
    unsigned long NumberOfDataRecords(unsigned long *NumberOfRecords);
    unsigned long NumberOfParts(unsigned long *NumberOfParts);
    unsigned long NumberOfPoints(unsigned long *NumberOfPoints);
    unsigned long SeekToFirstDataRecord();
    unsigned long SeekToDataRecord(long RecordNumber);
    unsigned long ReadNullShape();
    unsigned long ReadPointShape(long   *RecordNumber,
                                 double *PointX,
                                 double *PointY);
    unsigned long ReadArcShape(long    *RecordNumber,
                               double  *XMin,
                               double  *YMin,
                               double  *XMax,
                               double  *YMax,
                               long    *NumberOfParts,
                               long    *MaximumNumberOfParts,
                               long   **PartIndex,
                               long    *NumberOfPoints,
                               long    *MaximumNumberOfPoints,
                               double **XArray,
                               double **YArray);
    unsigned long ReadPolygonShape(long    *RecordNumber,
                                   double  *XMin,
                                   double  *YMin,
                                   double  *XMax,
                                   double  *YMax,
                                   long    *NumberOfParts,
                                   long    *MaximumNumberOfParts,
                                   long   **PartIndex,
                                   long    *NumberOfPoints,
                                   long    *MaximumNumberOfPoints,
                                   double **XArray,
                                   double **YArray);
    unsigned long ReadMultiPointShape(long    *RecordNumber,
                                      double  *XMin,
                                      double  *YMin,
                                      double  *XMax,
                                      double  *YMax,
                                      long    *NumberOfPoints,
                                      long    *MaximumNumberOfPoints,
                                      double **XArray,
                                      double **YArray);

  private:

    void CloseFiles();
    void CloseShapeFile();
    void CloseIndexFile();
    unsigned long ReadShapeRecordHeader(long *RecordNumber, long *RecordBytes);
    unsigned long ReadIndexRecord(long RecordNumber,
                                  long *OffsetOffset,
                                  long *OffsetLength);

    ifstream      _ShapeFileStream;
    ifstream      _IndexFileStream;

    unsigned char _ShapeFileIsOpen;  // t/f flag
    unsigned char _IndexFileIsOpen;  // t/f flag

    XP_GIS_ESRI_ShapeHeader_c _ShapeHeader;   // shape file header
    XP_GIS_ESRI_ShapeHeader_c _IndexHeader;   // index file header
    char  _FileName[_MAX_PATH];      // fully qualified file path w/o suffix
    char  _ShapeFileName[_MAX_PATH]; // fully qualified file path w/o suffix
    char  _IndexFileName[_MAX_PATH]; // fully qualified file path to index file
};

#endif   // XP_GIS_SHAPE_H


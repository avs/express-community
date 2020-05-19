//
//  Include file for the XP_GIS_DTED252_c class, which provides utilities
//  to read a DTED252 record from a DTED file.  The 252 record is the
//  record that contains the elevation data for a single column
//  of grid posts.  This is called a 252 record because the record sentinel
//  is a 1 byte field is a value of octal 252 (decimal 170).
//  record that contains elevation values for a single column of 
//  gridposts.
//

/*


    Public Constructors

      XP_GIS_DTED252_c(char          *LocalFileName = NULL,
                       unsigned short LocalNumberOfPoints = 1201);
        The constructor.  LocalFileName is the name of the DTED file.
        LocalNumberOfPoints is optional, and sets the number of points 
        in a column of data.  This is the number of grid posts along a 
        line of longitude (in the North/South direction) The number of
        points is stored in the uhl and dsi records in the DTED file.
        These values can be obtained via the XP_GIS_DTEDUHL_C and
        XP_GIS_DTEDDSI_C classes.

      virtual ~XP_GIS_DTED252_c();
        the destructor.

      XP_GIS_DTED252_c(const XP_GIS_DTED252_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DTED252_c &operator=(const XP_GIS_DTED252_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long Print(ostream &PrintStream) const;
        Method to print the header information from the 252 record.

      unsigned long Read(unsigned long RecordNumber,
                         ifstream     *FileStream);
        Method to read the 252 record.

      unsigned long FileName(char *LocalFileName);
        Method to set the file name in the 252 record.

      const char    *FileName() const
        Method to get the File name inthe 252 record.  This returns
        the file name set via the constructor of the overloaded
        File name method.

      unsigned long NumberOfPoints(unsigned short LocalNumberOfPoints);
        Method to set the number of points in the 252 record.

      unsigned short  NumberOfPoints() const;
        Method to get the number of points in the 252 record.  This
        returns the number of points set via the constructor or the
        overloaded NumberOfPoints method.

      unsigned char  RecognitionSentinel() const;
        Method to get the recognition sentinel.  This should always
        have a value of 252.

      unsigned long  DataBlockCount() const;
        Method to get the data block count value that is stored in the
        252 record.

      unsigned long  LongitudeCount() const;
        Method to get the longitude count stored in the 252 record.

      unsigned long  LatitudeCount()  const;
        Method to get the latitude count stored in the 252 record.

      const short   *DataArray() const;
        Method to get the array of elevation values for the 252 record.
        The data array contains elevation values for a single column,
        ordered from South to North.

      const unsigned char *RecordBuffer() const;
        Method to get the record buffer.

      long           Checksum() const;
        Method to get the checksum stored in the 252 record.

*/



#ifndef XP_GIS_DTED252_H
#define XP_GIS_DTED252_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#include "gserror.h"
#include "gsconst.h"

class XP_GIS_DTED252_c
{
  public:
    XP_GIS_DTED252_c(char          *LocalFileName = NULL,
                     unsigned short LocalNumberOfPoints = 1201);
    virtual ~XP_GIS_DTED252_c();
    XP_GIS_DTED252_c(const XP_GIS_DTED252_c &object);
    XP_GIS_DTED252_c &operator=(const XP_GIS_DTED252_c &object);

    unsigned long Print(ostream &PrintStream) const;

    unsigned long Read(unsigned long RecordNumber,
                       ifstream     *FileStream);
    unsigned long NumberOfPoints(unsigned short LocalNumberOfPoints);
    unsigned long FileName(char *LocalFileName);


    //
    //  Accesors to get values
    //

    const char    *FileName() const {return _FileName;}
    unsigned short NumberOfPoints() const {return _NumberOfPoints;}
    unsigned char  RecognitionSentinel()
                         const {return _RecognitionSentinel;}
    unsigned long  DataBlockCount() const {return _DataBlockCount;}
    unsigned long  LongitudeCount() const {return _LongitudeCount;}
    unsigned long  LatitudeCount()  const {return _LatitudeCount;}
    const short   *DataArray()      const {return _DataArray;}
    const unsigned char *RecordBuffer()   const {return _RecordBuffer;}
    long           Checksum()       const {return _Checksum;}

  private:

    unsigned long _FirstElevationRecordStart(ifstream      *FileStream,
                                             unsigned long *RecordStart);

    unsigned char  _RecognitionSentinel;   // should always be 252
    unsigned long  _DataBlockCount;
    unsigned long  _LongitudeCount;
    unsigned long  _LatitudeCount;
    short         *_DataArray;
    long           _Checksum;
    char           _FileName[_MAX_PATH];   // fully qualified file path
    unsigned short _NumberOfPoints;
    unsigned char *_RecordBuffer;
    unsigned long  _RecordLength;          // in bytes

};

#endif   // XP_GIS_DTED252_H


//
//  Include file for the XP_GIS_DTEDUHL_c class, which provides utilities
//  to read a DTED_UHL image from a DTED file.
//

/*

    Public Constructors

      XP_GIS_DTEDUHL_c();
        The constructor.

      virtual ~XP_GIS_DTEDUHL_c();
        the destructor.

      XP_GIS_DTEDUHL_c(const XP_GIS_DTEDUHL_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DTEDUHL_c &operator=(const XP_GIS_DTEDUHL_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long Print(ostream &PrintStream) const;
        Method to print a human-readable version of the UHL record.

      unsigned long Read(ifstream *FileStream);
        Method to read a UHL record from the DTED file specified by
        FileStream.

      const char     *RecognitionSentinel() const:
        Method to get the recognition sentinel field stored in the uhl record.

      double         OriginLatitude() const:
        Method to get the origin latitude field stored in the uhl record.
        The value is in decimal degrees.

      double         OriginLongitude() const:
        Method to get the origin longitude field stored in the uhl record.
        The value is in decimal degrees.

      float          LatitudeDataInterval() const:
        Method to get the latitude data interval field stored in the
        uhl record.

      float          LongitudeDataInterval() const:
        Method to get the longitude data interval field stored in the
        uhl record.

      unsigned short AbsoluteVerticalAccuracy() const:
        Method to get the absolute vertical accuracy field stored in the
        uhl record.  A value of USHRT_MAX means the field had a value of
        NA in the file.

      char           SecurityCode() const:
        Method to get the security code field stored in the uhl record.

      unsigned short UniqueReferece() const:
        Method to get the unique reference field stored in the uhl record.

      unsigned short NumberOfLongitudeLines() const:
        Method to get the number of longitude lines field stored in the
        uhl record.

      unsigned short NumberOfLatitudePoints() const:
        Method to get the number of latitude points field stored in the
        uhl record.


*/



#ifndef XP_GIS_DTED_UHL_H
#define XP_GIS_DTED_UHL_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#include "gserror.h"
#include "gsconst.h"

class XP_GIS_DTEDUHL_c
{
  public:
    XP_GIS_DTEDUHL_c();
    virtual ~XP_GIS_DTEDUHL_c();
    XP_GIS_DTEDUHL_c(const XP_GIS_DTEDUHL_c &object);
    XP_GIS_DTEDUHL_c &operator=(const XP_GIS_DTEDUHL_c &object);

    unsigned long Print(ostream &PrintStream) const;

    unsigned long Read(ifstream *FileStream);

    //
    //  Accesors to get values
    //

    const char     *RecognitionSentinel()
                         const {return _RecognitionSentinel;}
    double         OriginLatitude()
                         const {return _OriginLatitude;}
    double         OriginLongitude()
                         const {return _OriginLongitude;}
    float          LatitudeDataInterval()
                         const {return _LatitudeDataInterval;}
    float          LongitudeDataInterval()
                         const {return _LongitudeDataInterval;}
    unsigned short AbsoluteVerticalAccuracy()
                         const {return _AbsoluteVerticalAccuracy;}
    char           SecurityCode()
                         const {return _SecurityCode;}
    unsigned short UniqueReferece()
                         const {return _UniqueReference;}
    unsigned short NumberOfLongitudeLines()
                         const {return _NumberOfLongitudeLines;}
    unsigned short NumberOfLatitudePoints()
                         const {return _NumberOfLatitudePoints;}
    unsigned short MultipleAccuracy()
                         const {return _MultipleAccuracy;}

  private:

    unsigned long _ReadUHL(ifstream *FileStream);
    unsigned long _RecordStart(ifstream      *FileStream,
                               unsigned long *RecordStart);

    char           _RecognitionSentinel[4];
    double         _OriginLatitude;            // in degrees
    double         _OriginLongitude;           // in degrees
    float          _LatitudeDataInterval;      // in seconds
    float          _LongitudeDataInterval;     // in seconds
    unsigned short _AbsoluteVerticalAccuracy;  // USHRT_MAX means N/A
    char           _SecurityCode;              // T, S, C, U, or R
    unsigned short _UniqueReference;
    unsigned short _NumberOfLongitudeLines;
    unsigned short _NumberOfLatitudePoints;    // defaults to 1201 if missing
    unsigned short _MultipleAccuracy;
    
};

#endif   // XP_GIS_DTED_UHL_H


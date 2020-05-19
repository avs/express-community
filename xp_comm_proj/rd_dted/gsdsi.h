//
//  Include file for the XP_GIS_DTEDDSI_c class, which provides utilities
//  to read a DTED_DSI record from a DTED file.
//

/*

    Public Constructors

      XP_GIS_DTEDDSI_c();
        The constructor.

      virtual ~XP_GIS_DTEDDSI_c();
        the destructor.

      XP_GIS_DTEDDSI_c(const XP_GIS_DTEDDSI_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DTEDDSI_c &operator=(const XP_GIS_DTEDDSI_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods


      unsigned long Print(ostream &PrintStream) const;
        Method to print a human-readable version of the dsi header record.

      unsigned long Read(ifstream *FileStream);
        Method to read the dsi record from the DTED file specified by
        FileStream.

      const char     *RecognitionSentinel() const;
        Method to get the recognition sentinel field stored in the dsi record.

      char            SecurityCode() const;
        Method to get the security code field stored in the dsi record.

      const char     *SecurityControlMarkings() const;
        Method to get the security control markings field stored in the dsi record.

      const char     *SecurityHandlingDescription() const;
        Method to get the security handling description field stored in the dsi record.

      const char     *SeriesDesignator() const;
        Method to get the series designator field stored in the dsi record.

      const char     *UniqueReference() const;
        Method to get the unique reference field stored in the dsi record.

      unsigned short  EditionNumber() const;
        Method to get the edition number field stored in the dsi record.

      char            MatchMergeVersion() const;
        Method to get the match/merge versioni field stored in the dsi record.

      unsigned char   MaintenanceYear() const;
        Method to get the maintenance year field stored in the dsi record.

      unsigned char   MaintenanceMonth() const;
        Method to get the maintenance month field stored in the dsi record.

      unsigned char   MatchMergeYear() const;
        Method to get the match/merge year field stored in the dsi record.

      unsigned char   MatchMergeMonth() const;
        Method to get the match/merge month field stored in the dsi record.

      const char     *ProducerCode() const;
        Method to get the producer code field stored in the dsi record.

      const char     *StockNumber() const;
        Method to get the stock number field stored in the dsi record.

      unsigned char   AmendmentNumber() const;
        Method to get the amendment number field stored in the dsi record.

      unsigned char   SpecificationYear() const;
        Method to get the specification year field stored in the dsi record.

      unsigned char   SpecificationMonth() const;
        Method to get the specification month field stored in the dsi record.

      const char     *VerticalDatum() const;
        Method to get the vertical datum field stored in the dsi record.

      const char     *HorizontalDatum() const;
        Method to get the horizontal datum field stored in the dsi record.

      const char     *CollectionSystem() const;
        Method to get the collection system field stored in the dsi record.

      unsigned char   CompilationYear() const;
        Method to get the compilation year field stored in the dsi record.

      unsigned char   CompilationMonth() const;
        Method to get the compilation month field stored in the dsi record.

      double          OriginLatitude() const;
        Method to get the origin latitude field stored in the dsi record.
        The value is in decimal degrees.

      double          OriginLongitude() const;
        Method to get the origin longitude field stored in the dsi record.
        The value is in decimal degrees.

      double          SWLatitude() const;
        Method to get the South West latitude field stored in the dsi record.
        The value is in decimal degrees.

      double          SWLongitude() const;
        Method to get the South West longitude field stored in the dsi record.
        The value is in decimal degrees.

      double          NWLatitude() const;
        Method to get the North West latitude field stored in the dsi record.
        The value is in decimal degrees.

      double          NWLongitude() const;
        Method to get the North West longitude field stored in the dsi record.
        The value is in decimal degrees.

      double          NELatitude() const;
        Method to get the North East latitude field stored in the dsi record.
        The value is in decimal degrees.

      double          NELongitude() const;
        Method to get the North East longitude field stored in the dsi record.
        The value is in decimal degrees.

      double          SELatitude() const;
        Method to get the South East latitude field stored in the dsi record.
        The value is in decimal degrees.

      double          SELongitude() const;
        Method to get the South East longitude field stored in the dsi record.
        The value is in decimal degrees.

      double          Orientation() const;
        Method to get the orientation field stored in the dsi record.

      float           LatitudeInterval() const;
        Method to get the latitude interval field stored in the dsi record.
        The value is in arc seconds.

      float           LongitudeInterval() const;
        Method to get the longitude interval field stored in the dsi record.
        The value is in arc seconds.

      unsigned short  NumberOfLatitudeLines() const;
        Method to get the number of latitude lines field stored in the dsi record.

      unsigned short  NumberOfLongitudeLines() const;
        Method to get the number of longitude lines field stored in the dsi record.

      unsigned char   PartialCellIndicator() const;
        Method to get the partial cell indicator stored in the dsi record.

*/



#ifndef XP_GIS_DTED_DSI_H
#define XP_GIS_DTED_DSI_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#include "gserror.h"
#include "gsconst.h"

class XP_GIS_DTEDDSI_c
{
  public:
    XP_GIS_DTEDDSI_c();
    virtual ~XP_GIS_DTEDDSI_c();
    XP_GIS_DTEDDSI_c(const XP_GIS_DTEDDSI_c &object);
    XP_GIS_DTEDDSI_c &operator=(const XP_GIS_DTEDDSI_c &object);

    unsigned long Print(ostream &PrintStream) const;

    unsigned long Read(ifstream *FileStream);

    //
    //  Accesors to get values
    //

    const char     *RecognitionSentinel()
                         const {return _RecognitionSentinel;}
    char            SecurityCode()
                         const {return _SecurityCode;}
    const char     *SecurityControlMarkings()
                         const {return _SecurityControlMarkings;}
    const char     *SecurityHandlingDescription()
                         const {return _SecurityHandlingDescription;}
    const char     *SeriesDesignator()
                         const {return _SeriesDesignator;}
    const char     *UniqueReference()
                         const {return _UniqueReference;}
    unsigned short  EditionNumber()
                         const {return _EditionNumber;}
    char            MatchMergeVersion()
                         const {return _MatchMergeVersion;}
    unsigned char   MaintenanceYear()
                         const {return _MaintenanceYear;}
    unsigned char   MaintenanceMonth()
                         const {return _MaintenanceMonth;}
    unsigned char   MatchMergeYear()
                         const {return _MatchMergeYear;}
    unsigned char   MatchMergeMonth()
                         const {return _MatchMergeMonth;}
    const char     *ProducerCode()
                         const {return _ProducerCode;}
    const char     *StockNumber()
                         const {return _StockNumber;}
    unsigned char   AmendmentNumber()
                         const {return _AmendmentNumber;}
    unsigned char   SpecificationYear()
                         const {return _SpecificationYear;}
    unsigned char   SpecificationMonth()
                         const {return _SpecificationMonth;}
    const char     *VerticalDatum()
                         const {return _VerticalDatum;}
    const char     *HorizontalDatum()
                         const {return _HorizontalDatum;}
    const char     *CollectionSystem()
                         const {return _CollectionSystem;}
    unsigned char   CompilationYear()
                         const {return _CompilationYear;}
    unsigned char   CompilationMonth()
                         const {return _CompilationMonth;}
    double          OriginLatitude()
                         const {return _OriginLatitude;}
    double          OriginLongitude()
                         const {return _OriginLongitude;}
    double          SWLatitude()
                         const {return _SWLatitude;}
    double          SWLongitude()
                         const {return _SWLongitude;}
    double          NWLatitude()
                         const {return _NWLatitude;}
    double          NWLongitude()
                         const {return _NWLongitude;}
    double          NELatitude()
                         const {return _NELatitude;}
    double          NELongitude()
                         const {return _NELongitude;}
    double          SELatitude()
                         const {return _SELatitude;}
    double          SELongitude()
                         const {return _SELongitude;}
    double          Orientation()
                         const {return _Orientation;}
    float           LatitudeInterval()
                         const {return _LatitudeInterval;}
    float           LongitudeInterval()
                         const {return _LongitudeInterval;}
    unsigned short  NumberOfLatitudeLines()
                         const {return _NumberOfLatitudeLines;}
    unsigned short  NumberOfLongitudeLines()
                         const {return _NumberOfLongitudeLines;}
    unsigned char   PartialCellIndicator()
                         const {return _PartialCellIndicator;}

  private:

    unsigned long _ReadDSI(ifstream *FileStream);
    unsigned long _RecordStart(ifstream      *FileStream,
                               unsigned long *RecordStart);

    char           _RecognitionSentinel[4];
    char           _SecurityCode;
    char           _SecurityControlMarkings[3];
    char           _SecurityHandlingDescription[28];
    char           _SeriesDesignator[6];
    char           _UniqueReference[16];
    unsigned short _EditionNumber;
    char           _MatchMergeVersion;
    unsigned char  _MaintenanceYear;
    unsigned char  _MaintenanceMonth;
    unsigned char  _MatchMergeYear;
    unsigned char  _MatchMergeMonth;
    char           _ProducerCode[9];
    char           _StockNumber[10];
    unsigned char  _AmendmentNumber;
    unsigned char  _SpecificationYear;
    unsigned char  _SpecificationMonth;
    char           _VerticalDatum[4];
    char           _HorizontalDatum[6];
    char           _CollectionSystem[11];
    unsigned char  _CompilationYear;
    unsigned char  _CompilationMonth;
    double         _OriginLatitude;        // decimal degrees
    double         _OriginLongitude;       // decimal degrees
    double         _SWLatitude;            // decimal degrees
    double         _SWLongitude;           // decimal degrees
    double         _NWLatitude;            // decimal degrees
    double         _NWLongitude;           // decimal degrees
    double         _NELatitude;            // decimal degrees
    double         _NELongitude;           // decimal degrees
    double         _SELatitude;            // decimal degrees
    double         _SELongitude;           // decimal degrees
    double         _Orientation;
    float          _LatitudeInterval;      // arc seconds
    float          _LongitudeInterval;     // arc seconds
    unsigned short _NumberOfLatitudeLines;
    unsigned short _NumberOfLongitudeLines;
    unsigned char  _PartialCellIndicator;

};

#endif   // XP_GIS_DTED_DSI_H


//
//  This file contains the source for the XP_GIS_DTEDDSI_c class.
//  This class reads a DSI record from a DTED file.
//


#include <stdio.h>

#include "gsdsi.h"


//  Define the length of the DSI record

static const unsigned long DSIRecordLength = 291; // in bytes

//  Define the location of each field in the DSI record.

static const unsigned long RecognitionSentinelStart         = 0;
static const unsigned long RecognitionSentinelEnd           = 2;

static const unsigned long SecurityCodeStart                = 3;
static const unsigned long SecurityCodeEnd                  = 3;

static const unsigned long SecurityControlMarkingsStart     = 4;
static const unsigned long SecurityControlMarkingsEnd       = 5;

static const unsigned long SecurityHandlingDescriptionStart = 6;
static const unsigned long SecurityHandlingDescriptionEnd   = 32;

static const unsigned long SeriesDesignatorStart            = 58;
static const unsigned long SeriesDesignatorEnd              = 63;

static const unsigned long UniqueReferenceStart             = 64;
static const unsigned long UniqueReferenceEnd               = 78;

static const unsigned long EditionNumberStart               = 86;
static const unsigned long EditionNumberEnd                 = 88;

static const unsigned long MatchMergeVersionStart           = 89;
static const unsigned long MatchMergeVersionEnd             = 89;

static const unsigned long MaintenanceDateStart             = 90;
static const unsigned long MaintenanceDateEnd               = 93;

static const unsigned long MatchMergeDateStart              = 94;
static const unsigned long MatchMergeDateEnd                = 97;

static const unsigned long ProducerCodeStart                = 102;
static const unsigned long ProducerCodeEnd                  = 109;

static const unsigned long StockNumberStart                 = 125;
static const unsigned long StockNumberEnd                   = 134;

static const unsigned long AmendmentNumberStart             = 135;
static const unsigned long AmendmentNumberEnd               = 136;

static const unsigned long SpecificationDateStart           = 137;
static const unsigned long SpecificationDateEnd             = 140;

static const unsigned long VerticalDatumStart               = 141;
static const unsigned long VerticalDatumEnd                 = 143;

static const unsigned long HorizontalDatumStart             = 144;
static const unsigned long HorizontalDatumEnd               = 148;

static const unsigned long CollectionSystemStart            = 149;
static const unsigned long CollectionSystemEnd              = 158;

static const unsigned long CompilationDateStart             = 159;
static const unsigned long CompilationDateEnd               = 162;

static const unsigned long OriginLatitudeStart              = 185;
static const unsigned long OriginLatitudeEnd                = 193;

static const unsigned long OriginLongitudeStart             = 194;
static const unsigned long OriginLongitudeEnd               = 203;

static const unsigned long SWLatitudeStart                  = 204;
static const unsigned long SWLatitudeEnd                    = 210;

static const unsigned long SWLongitudeStart                 = 211;
static const unsigned long SWLongitudeEnd                   = 218;

static const unsigned long NWLatitudeStart                  = 219;
static const unsigned long NWLatitudeEnd                    = 225;

static const unsigned long NWLongitudeStart                 = 226;
static const unsigned long NWLongitudeEnd                   = 233;

static const unsigned long NELatitudeStart                  = 234;
static const unsigned long NELatitudeEnd                    = 240;

static const unsigned long NELongitudeStart                 = 241;
static const unsigned long NELongitudeEnd                   = 248;

static const unsigned long SELatitudeStart                  = 249;
static const unsigned long SELatitudeEnd                    = 255;

static const unsigned long SELongitudeStart                 = 256;
static const unsigned long SELongitudeEnd                   = 263;

static const unsigned long OrientationStart                 = 264;
static const unsigned long OrientationEnd                   = 272;

static const unsigned long LatitudeIntervalStart            = 273;
static const unsigned long LatitudeIntervalEnd              = 276;

static const unsigned long LongitudeIntervalStart           = 277;
static const unsigned long LongitudeIntervalEnd             = 280;

static const unsigned long NumberOfLatitudeLinesStart       = 281;
static const unsigned long NumberOfLatitudeLinesEnd         = 284;

static const unsigned long NumberOfLongitudeLinesStart      = 285;
static const unsigned long NumberOfLongitudeLinesEnd        = 288;

static const unsigned long PartialCellIndicatorStart        = 289;
static const unsigned long PartialCellIndicatorEnd          = 290;


//
//  Constructor.
//

XP_GIS_DTEDDSI_c::XP_GIS_DTEDDSI_c()
{
}

//
//  destructor.
//

XP_GIS_DTEDDSI_c::~XP_GIS_DTEDDSI_c()
{
}

//
//  Copy constructor.
//

XP_GIS_DTEDDSI_c::XP_GIS_DTEDDSI_c(const XP_GIS_DTEDDSI_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DTEDDSI_c &XP_GIS_DTEDDSI_c::operator=(
                      const XP_GIS_DTEDDSI_c &object)
{
    strcpy(_RecognitionSentinel,object._RecognitionSentinel);

    _SecurityCode = object._SecurityCode;
    strcpy(_SecurityControlMarkings,
                 object._SecurityControlMarkings);
    strcpy(_SecurityHandlingDescription,
                 object._SecurityHandlingDescription);
    strcpy(_SeriesDesignator,object._SeriesDesignator);
    strcpy(_UniqueReference,object._UniqueReference);
    _EditionNumber = object._EditionNumber;
    _MatchMergeVersion = object._MatchMergeVersion;
    _MaintenanceYear = object._MaintenanceYear;
    _MaintenanceMonth = object._MaintenanceMonth;
    _MatchMergeYear = object._MatchMergeYear;
    _MatchMergeMonth = object._MatchMergeMonth;
    strcpy(_ProducerCode,object._ProducerCode);
    strcpy(_StockNumber,object._StockNumber);
    _AmendmentNumber = object._AmendmentNumber;
    _SpecificationYear = object._SpecificationYear;
    _SpecificationMonth = object._SpecificationMonth;
    strcpy(_VerticalDatum,object._VerticalDatum);
    strcpy(_HorizontalDatum,object._HorizontalDatum);
    strcpy(_CollectionSystem,object._CollectionSystem);
    _CompilationYear = object._CompilationYear;
    _CompilationMonth = object._CompilationMonth;
    _OriginLatitude = object._OriginLatitude;
    _OriginLongitude = object._OriginLongitude;
    _SWLatitude = object._SWLatitude;
    _SWLongitude = object._SWLongitude;
    _NWLatitude = object._NWLatitude;
    _NWLongitude = object._NWLongitude;
    _NELatitude = object._NELatitude;
    _NELongitude = object._NELongitude;
    _SELatitude = object._SELatitude;
    _SELongitude = object._SELongitude;
    _Orientation = object._Orientation;
    _LatitudeInterval = object._LatitudeInterval;
    _LongitudeInterval = object._LongitudeInterval;
    _NumberOfLatitudeLines = object._NumberOfLatitudeLines;
    _NumberOfLongitudeLines = object._NumberOfLongitudeLines;
    _PartialCellIndicator = object._PartialCellIndicator;

    return *this;
}

//
//  Method to set the file stream.  When the file stream is set,
//  the DSI record is read from the file and the various properties
//  of the class populated with data from the record.
//

unsigned long XP_GIS_DTEDDSI_c::Read(ifstream *FileStream)
{

    //
    //  Validate the input.
    //

    if (FileStream == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Invoke the low-level routine to read the header.
    //

    return _ReadDSI(FileStream);
}

//
//  Method to print a human-readable version of the DSI record.
//  This is printed to PrintStream.
//

unsigned long XP_GIS_DTEDDSI_c::Print(ostream &PrintStream) const
{

    if (PrintStream == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    PrintStream << "DSI record contents:" << endl;

    PrintStream << "  Recognition Sentinel          = ";
    PrintStream << _RecognitionSentinel << endl;

    PrintStream << "  Security Code                 = ";
    PrintStream << _SecurityCode << endl;

    PrintStream << "  Security Control Markings     = ";
    PrintStream << _SecurityControlMarkings << endl;

    PrintStream << "  Security Handling Description = ";
    PrintStream << _SecurityHandlingDescription << endl;

    PrintStream << "  Series Designator             = ";
    PrintStream << _SeriesDesignator << endl;

    PrintStream << "  Edition Number                = ";
    PrintStream << _EditionNumber << endl;

    PrintStream << "  Match Merge Version           = ";
    PrintStream << _MatchMergeVersion << endl;

    PrintStream << "  Maintenance Year              = ";
    PrintStream << (short) _MaintenanceYear << endl;

    PrintStream << "  Maintenance Month             = ";
    PrintStream << (short) _MaintenanceMonth << endl;

    PrintStream << "  Match Merge Year              = ";
    PrintStream << (short) _MatchMergeYear << endl;

    PrintStream << "  Match Merge Month             = ";
    PrintStream << (short) _MatchMergeMonth << endl;

    PrintStream << "  Producer Code                 = ";
    PrintStream << _ProducerCode << endl;

    PrintStream << "  Stock Number                  = ";
    PrintStream << _StockNumber << endl;

    PrintStream << "  Amendment Number              = ";
    PrintStream << (short) _AmendmentNumber << endl;

    PrintStream << "  Specification Year            = ";
    PrintStream << (short) _SpecificationYear << endl;

    PrintStream << "  Specification Month           = ";
    PrintStream << (short) _SpecificationMonth << endl;

    PrintStream << "  Vertical Datum                = ";
    PrintStream << _VerticalDatum << endl;

    PrintStream << "  Horizontal Datum              = ";
    PrintStream << _HorizontalDatum << endl;

    PrintStream << "  Collection System             = ";
    PrintStream << _CollectionSystem << endl;

    PrintStream << "  Compilation Year              = ";
    PrintStream << (short) _CompilationYear << endl;

    PrintStream << "  Compilation Month             = ";
    PrintStream << (short) _CompilationMonth << endl;

    PrintStream << "  Origin Latitude               = ";
    PrintStream << _OriginLatitude << endl;

    PrintStream << "  Origin Longitude              = ";
    PrintStream << _OriginLongitude << endl;

    PrintStream << "  SW Latitude                   = ";
    PrintStream << _SWLatitude << endl;

    PrintStream << "  SW Longitude                  = ";
    PrintStream << _SWLongitude << endl;

    PrintStream << "  NW Latitude                   = ";
    PrintStream << _NWLatitude << endl;

    PrintStream << "  NW Longitude                  = ";
    PrintStream << _NWLongitude << endl;

    PrintStream << "  NE Latitude                   = ";
    PrintStream << _NELatitude << endl;

    PrintStream << "  NE Longitude                  = ";
    PrintStream << _NELongitude << endl;

    PrintStream << "  SE Latitude                   = ";
    PrintStream << _SELatitude << endl;

    PrintStream << "  SE Longitude                  = ";
    PrintStream << _SELongitude << endl;

    PrintStream << "  Orientation                   = ";
    PrintStream << _Orientation << endl;

    PrintStream << "  Latitude Interval             = ";
    PrintStream << _LatitudeInterval << endl;

    PrintStream << "  Longitude Interval            = ";
    PrintStream << _LongitudeInterval << endl;

    PrintStream << "  Number Of Latitude Lines      = ";
    PrintStream << _NumberOfLatitudeLines << endl;

    PrintStream << "  Number Of Longitude Lines     = ";
    PrintStream << _NumberOfLongitudeLines << endl;

    PrintStream << "  Partial Cell Indicator        = ";
    PrintStream << (short) _PartialCellIndicator << endl;

    return XP_GIS_OK;
}

//
//  Private method to read the DSI record.  This method validates the
//  file stream, reads the record, and parses out all the fields in
//  the DSI record.
//

unsigned long XP_GIS_DTEDDSI_c::_ReadDSI(ifstream *FileStream)
{
    unsigned long      RecordStart;
    unsigned long      ReturnValue;
    unsigned char      DSIBuffer[DSIRecordLength];
    char               StringBuffer[DSIRecordLength + 1];
    char               Hemisphere;  // N, n, S, s, W, w, E, or e
    unsigned long      Degrees;
    unsigned long      Minutes;
    unsigned long      Seconds;
    float              fSeconds;
    unsigned long      FieldLength;
    unsigned long      LatitudeInterval;
    unsigned long      LongitudeInterval;
    unsigned long      Year;
    unsigned long      Month;
    unsigned long      AmendmentNumber;
    unsigned long      PartialCellIndicator;

    //
    //  Make sure the file stream has been set.
    //

    if (FileStream == NULL)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Find the start of the DSI record
    //

    if ((ReturnValue = _RecordStart(FileStream,&RecordStart)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Seek to the start of the DSI record.
    //

    if (!FileStream->seekg(RecordStart,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  read the record header
    //

    if (!FileStream->read(DSIBuffer,
                          DSIRecordLength))
    {
      if (FileStream->eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (FileStream->gcount() != DSIRecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the recognition sentinel
    //

    FieldLength = (RecognitionSentinelEnd - RecognitionSentinelStart) + 1;
    memcpy(_RecognitionSentinel,&DSIBuffer[RecognitionSentinelStart],
           FieldLength);
    _RecognitionSentinel[FieldLength] = '\0';
    if (strcmp(_RecognitionSentinel,"DSI") != 0)
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }

    //
    //  Parse out the security code.
    //

    FieldLength = (SecurityCodeEnd - SecurityCodeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SecurityCodeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%1c",&_SecurityCode);

    //
    //  Parse out the security control markings.
    //

    FieldLength = (SecurityControlMarkingsEnd - 
                   SecurityControlMarkingsStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SecurityControlMarkingsStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2c",&_SecurityControlMarkings);
    _SecurityControlMarkings[2] = '\0';

    //
    //  Parse out the security handling description.
    //

    FieldLength = (SecurityHandlingDescriptionEnd - 
                   SecurityHandlingDescriptionStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SecurityHandlingDescriptionStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%27c",&_SecurityHandlingDescription);
    _SecurityHandlingDescription[27] = '\0';

    //
    //  Parse out the Series Designator.
    //

    FieldLength = (SeriesDesignatorEnd - SeriesDesignatorStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SeriesDesignatorStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%5c",&_SeriesDesignator);
    _SeriesDesignator[5] = '\0';

    //
    //  Parse out the unique reference (really a string).
    //

    FieldLength = (UniqueReferenceEnd - UniqueReferenceStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[UniqueReferenceStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%15c",&_UniqueReference);
    _UniqueReference[15] = '\0';

    //
    //  Parse out the edition number.
    //

    FieldLength = (EditionNumberEnd - EditionNumberStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[EditionNumberStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2hd",&_EditionNumber);

    //
    //  Parse out the match/merge version number
    //

    FieldLength = (MatchMergeVersionEnd - MatchMergeVersionStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[MatchMergeVersionStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%1c",&_MatchMergeVersion);

    //
    //  Parse out the maintenance year and month
    //

    FieldLength = (MaintenanceDateEnd - MaintenanceDateStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[MaintenanceDateStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d",&Year,&Month);
    _MaintenanceYear  = (unsigned char) Year;
    _MaintenanceMonth = (unsigned char) Month;

    //
    //  Parse out the match/merge year and month
    //

    FieldLength = (MatchMergeDateEnd - MatchMergeDateStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[MatchMergeDateStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d",&Year,&Month);
    _MatchMergeYear  = (unsigned char) Year;
    _MatchMergeMonth = (unsigned char) Month;

    //
    //  Parse out the producer code.
    //

    FieldLength = (ProducerCodeEnd - ProducerCodeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[ProducerCodeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%8c",&_ProducerCode);
    _ProducerCode[8] = '\0';

    //
    //  Parse out the stock number (really a string).
    //

    FieldLength = (StockNumberEnd - StockNumberStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[StockNumberStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%9c",&_StockNumber);
    _StockNumber[9] = '\0';

    //
    //  Parse out the amendment number (really a single character).
    //

    FieldLength = (AmendmentNumberEnd - AmendmentNumberStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[AmendmentNumberStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d",&AmendmentNumber);
    _AmendmentNumber = (unsigned char) AmendmentNumber;
    //
    //  Parse out the specification year and month
    //

    FieldLength = (SpecificationDateEnd - SpecificationDateStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SpecificationDateStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d",&Year, &Month);
    _SpecificationYear  = (unsigned char) Year;
    _SpecificationMonth = (unsigned char) Month;

    //
    //  Parse out the vertical datum.
    //

    FieldLength = (VerticalDatumEnd - VerticalDatumStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[VerticalDatumStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3c",&_VerticalDatum);
    _VerticalDatum[3] = '\0';

    //
    //  Parse out the horizontal datum.
    //

    FieldLength = (HorizontalDatumEnd - HorizontalDatumStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[HorizontalDatumStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%5c",&_HorizontalDatum);
    _HorizontalDatum[5] = '\0';

    //
    //  Parse out the collection system.
    //

    FieldLength = (CollectionSystemEnd - CollectionSystemStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[CollectionSystemStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%10c",&_CollectionSystem);
    _CollectionSystem[10] = '\0';

    //
    //  Parse out the compilation year and month
    //

    FieldLength = (CompilationDateEnd - CompilationDateStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[CompilationDateStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d",&Year, &Month);
    _CompilationYear  = (unsigned char) Year;
    _CompilationMonth = (unsigned char) Month;

    //
    //  Parse out the origin latitude into decimal degrees.
    //

    FieldLength = (OriginLatitudeEnd - OriginLatitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[OriginLatitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d%4f%1c",
           &Degrees,&Minutes,&fSeconds,&Hemisphere);
    _OriginLatitude = (double) Degrees;
    _OriginLatitude += (double) Minutes / 60.0;
    _OriginLatitude += (double) fSeconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _OriginLatitude = -_OriginLatitude;
    }

    //
    //  Parse out the origin longitude into decimal degrees.
    //

    FieldLength = (OriginLongitudeEnd - 
                   OriginLongitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[OriginLongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%4f%1c",
           &Degrees,&Minutes,&fSeconds,&Hemisphere);
    _OriginLongitude = (double) Degrees;
    _OriginLongitude += (double) Minutes / 60.0;
    _OriginLongitude += (double) fSeconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _OriginLongitude = -_OriginLongitude;
    }

    //
    //  Parse out the SW latitude into decimal degrees.
    //

    FieldLength = (SWLatitudeEnd - SWLatitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SWLatitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _SWLatitude = (double) Degrees;
    _SWLatitude += (double) Minutes / 60.0;
    _SWLatitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _SWLatitude = -_SWLatitude;
    }

    //
    //  Parse out the SW longitude into decimal degrees.
    //

    FieldLength = (SWLongitudeEnd - 
                   SWLongitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SWLongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _SWLongitude = (double) Degrees;
    _SWLongitude += (double) Minutes / 60.0;
    _SWLongitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _SWLongitude = -_SWLongitude;
    }

    //
    //  Parse out the NW latitude into decimal degrees.
    //

    FieldLength = (NWLatitudeEnd - NWLatitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NWLatitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _NWLatitude = (double) Degrees;
    _NWLatitude += (double) Minutes / 60.0;
    _NWLatitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _NWLatitude = -_NWLatitude;
    }

    //
    //  Parse out the NW longitude into decimal degrees.
    //

    FieldLength = (NWLongitudeEnd - 
                   NWLongitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NWLongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _NWLongitude = (double) Degrees;
    _NWLongitude += (double) Minutes / 60.0;
    _NWLongitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _NWLongitude = -_NWLongitude;
    }

    //
    //  Parse out the NE latitude into decimal degrees.
    //

    FieldLength = (NELatitudeEnd - NELatitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NELatitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _NELatitude = (double) Degrees;
    _NELatitude += (double) Minutes / 60.0;
    _NELatitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _NELatitude = -_NELatitude;
    }

    //
    //  Parse out the NE longitude into decimal degrees.
    //

    FieldLength = (NELongitudeEnd - 
                   NELongitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NELongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _NELongitude = (double) Degrees;
    _NELongitude += (double) Minutes / 60.0;
    _NELongitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _NELongitude = -_NELongitude;
    }

    //
    //  Parse out the SE latitude into decimal degrees.
    //

    FieldLength = (SELatitudeEnd - SELatitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SELatitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _SELatitude = (double) Degrees;
    _SELatitude += (double) Minutes / 60.0;
    _SELatitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _SELatitude = -_SELatitude;
    }

    //
    //  Parse out the SE longitude into decimal degrees.
    //

    FieldLength = (SELongitudeEnd - 
                   SELongitudeStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[SELongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",
           &Degrees,&Minutes,&Seconds,&Hemisphere);
    _SELongitude = (double) Degrees;
    _SELongitude += (double) Minutes / 60.0;
    _SELongitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _SELongitude = -_SELongitude;
    }

    //
    //  Parse out the orientation.
    //

    FieldLength = (OrientationEnd - 
                   OrientationStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[OrientationStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%4f",
           &Degrees,&Minutes,&fSeconds);
    _Orientation = (double) Degrees;
    _Orientation += (double) Minutes / 60.0;
    _Orientation += (double) fSeconds / (60.0 * 60.0);

    //
    //  Parse out the latitude data interval into seconds.
    //

    FieldLength = (LatitudeIntervalEnd - LatitudeIntervalStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[LatitudeIntervalStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%4d",&LatitudeInterval);
    _LatitudeInterval = LatitudeInterval / 10.0f;

    //
    //  Parse out the longitude data interval into seconds.
    //

    FieldLength = (LongitudeIntervalEnd - 
                   LongitudeIntervalStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[LongitudeIntervalStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%4d",&LongitudeInterval);
    _LongitudeInterval = LongitudeInterval / 10.0f;
    
    //
    //  Parse out the number of latitude lines.
    //

    FieldLength = (NumberOfLatitudeLinesEnd - NumberOfLatitudeLinesStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NumberOfLatitudeLinesStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%4hd",&_NumberOfLatitudeLines);

    //
    //  Parse out the number of longitude lines.
    //

    FieldLength = (NumberOfLongitudeLinesEnd - 
                   NumberOfLongitudeLinesStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[NumberOfLongitudeLinesStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%4hd",&_NumberOfLongitudeLines);

    //
    //  Parse out the partial cell indicator.
    //

    FieldLength = (PartialCellIndicatorEnd - PartialCellIndicatorStart) + 1;
    memcpy(StringBuffer,&DSIBuffer[PartialCellIndicatorStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%2d",&PartialCellIndicator);
    _PartialCellIndicator = (unsigned char) PartialCellIndicator;

    return XP_GIS_OK;
}

//
//  Method to compute the byte offset from the start of the file to the
//  start of the DSI record.  This value is different for tape-format
//  and CDROM-format files.  Tape format files have an extra 80 byte 
//  HDR record as the first record.  We can recognize this by looking
//  at the first 3 bytes of the file.  It they are "HDR", then this is
//  a tape-format file.  Otherwise, it should be a CDROM-format file
//  (this will be confirmed when reading the various header records by
//  looking at their sentinels).
//

unsigned long XP_GIS_DTEDDSI_c::_RecordStart(ifstream      *FileStream,
                                             unsigned long *RecordStart)
{

    const unsigned long HDRRecordLength = 80;
    char                RecordSentinel[4];

    //
    //  Seek to the start of the file.
    //

    if (!FileStream->seekg(0,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  read the record sentinel
    //

    if (!FileStream->read(RecordSentinel,3))
    {
      if (FileStream->eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (FileStream->gcount() != 3)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Check the sentinal to see if it is HDR.  If so, this file has
    //  the "tape" format, and there is a 80 byte HDR record we need
    //  to skip.  
    //

    RecordSentinel[3] = '\0';
    *RecordStart = 80;
    if (strcmp(RecordSentinel,"HDR") == 0)
    {
      *RecordStart += HDRRecordLength;
    }

    return XP_GIS_OK;
}

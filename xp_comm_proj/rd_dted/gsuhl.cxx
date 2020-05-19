//
//  This file contains the source for the XP_GIS_DTEDUHL_c class.
//  This class reads a UHL record from a DTED file.
//

#include <stdio.h>

#include "gsuhl.h"


//  Define the lengths of the UHL record

static const unsigned long UHLRecordLength = 57; // in bytes

//  Define the location of each field in the UHL record.

static const unsigned long RecognitionSentinelStart      = 0;
static const unsigned long RecognitionSentinelEnd        = 2;

static const unsigned long OriginLongitudeStart          = 4;
static const unsigned long OriginLongitudeEnd            = 11;

static const unsigned long OriginLatitudeStart           = 12;
static const unsigned long OriginLatitudeEnd             = 19;

static const unsigned long LongitudeDataIntervalStart    = 20;
static const unsigned long LongitudeDataIntervalEnd      = 23;

static const unsigned long LatitudeDataIntervalStart     = 24;
static const unsigned long LatitudeDataIntervalEnd       = 27;

static const unsigned long AbsoluteVerticalAccuracyStart = 28;
static const unsigned long AbsoluteVerticalAccuracyEnd   = 31;

static const unsigned long SecurityCodeStart             = 32;
static const unsigned long SecurityCodeEnd               = 34;

static const unsigned long UniqueReferenceStart          = 35;
static const unsigned long UniqueReferenceEnd            = 46;

static const unsigned long NumberOfLongitudeLinesStart   = 47;
static const unsigned long NumberOfLongitudeLinesEnd     = 50;

static const unsigned long NumberOfLatitudePointsStart   = 51;
static const unsigned long NumberOfLatitudePointsEnd     = 54;

static const unsigned long MultipleAccuracyStart         = 55;
static const unsigned long MultipleAccuracyEnd           = 56;

//
//  Constructor.
//

XP_GIS_DTEDUHL_c::XP_GIS_DTEDUHL_c()
{
}

//
//  destructor.
//

XP_GIS_DTEDUHL_c::~XP_GIS_DTEDUHL_c()
{
}

//
//  Copy constructor.
//

XP_GIS_DTEDUHL_c::XP_GIS_DTEDUHL_c(const XP_GIS_DTEDUHL_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DTEDUHL_c &XP_GIS_DTEDUHL_c::operator=(
                      const XP_GIS_DTEDUHL_c &object)
{
    strcpy(_RecognitionSentinel,object._RecognitionSentinel);
    _OriginLatitude           = object._OriginLatitude;
    _OriginLongitude          = object._OriginLongitude;
    _LatitudeDataInterval     = object._LatitudeDataInterval;
    _LongitudeDataInterval    = object._LongitudeDataInterval;
    _AbsoluteVerticalAccuracy = object._AbsoluteVerticalAccuracy;
    _SecurityCode             = object._SecurityCode;
    _UniqueReference          = object._UniqueReference;
    _NumberOfLongitudeLines   = object._NumberOfLongitudeLines;
    _NumberOfLatitudePoints   = object._NumberOfLatitudePoints;
    _MultipleAccuracy         = object._MultipleAccuracy;

    return *this;
}

//
//  Method to set the file stream.  When the file stream is set,
//  the UHL record is read from the file and the various properties
//  of the class populated with data from the record.
//

unsigned long XP_GIS_DTEDUHL_c::Read(ifstream *FileStream)
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

    return _ReadUHL(FileStream);
}

//
//  Method to print a human-readable version of the UHL record.
//  This is printed to PrintStream.
//

unsigned long XP_GIS_DTEDUHL_c::Print(ostream &PrintStream) const
{

    if (PrintStream == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    PrintStream << "UHL record contents:" << endl;

    PrintStream << "  Origin Longitude           = ";
    PrintStream << _OriginLongitude << endl;
    PrintStream << "  Origin Latitude            = ";
    PrintStream << _OriginLatitude << endl;
    PrintStream << "  Longitude Data Interval    = ";
    PrintStream << _LongitudeDataInterval << endl;
    PrintStream << "  Latitude Data Interval     = ";
    PrintStream << _LatitudeDataInterval << endl;

    PrintStream << "  Absolute Vertical Accuracy = ";
    if (_AbsoluteVerticalAccuracy < USHRT_MAX)
    {
      PrintStream << _AbsoluteVerticalAccuracy;
    }
    else
    {
      PrintStream << "NA";
    }
    PrintStream << endl;

    PrintStream << "  Security Code              = ";
    PrintStream << _SecurityCode << endl;

    PrintStream << "  Unique Reference           = ";
    PrintStream << _UniqueReference << endl;

    PrintStream << "  Number Of Longitude Lines  = ";
    PrintStream << _NumberOfLongitudeLines << endl;

    PrintStream << "  Number Of Latitude Points  = ";
    PrintStream << _NumberOfLatitudePoints << endl;

    PrintStream << "  Multiple Accuracy          = ";
    PrintStream << _MultipleAccuracy << endl;

    return XP_GIS_OK;
}

//
//  Private method to read the UHL record.  This method validates the
//  file stream, reads the record, and parses out all the fields in
//  the UHL record.
//

unsigned long XP_GIS_DTEDUHL_c::_ReadUHL(ifstream *FileStream)
{

    unsigned long      RecordStart;
    unsigned long      ReturnValue;
    unsigned char      UHLBuffer[UHLRecordLength];
    char               StringBuffer[UHLRecordLength + 1];
    char               Hemisphere;  // N, n, S, s, W, w, E, or e
    unsigned long      FieldLength;
    unsigned long      Degrees;
    unsigned long      Minutes;
    unsigned long      Seconds;
    unsigned long      LatitudeDataInterval;
    unsigned long      LongitudeDataInterval;

    //
    //  Make sure the file stream has been set.
    //

    if (FileStream == NULL)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Find the start of the UHL record
    //

    if ((ReturnValue = _RecordStart(FileStream,&RecordStart)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Seek to the start of the UHL record.
    //

    if (!FileStream->seekg(RecordStart,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  read the record header
    //

    if (!FileStream->read(UHLBuffer,
                          UHLRecordLength))
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
    if (FileStream->gcount() != UHLRecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the recognition sentinel
    //

    FieldLength = (RecognitionSentinelEnd - RecognitionSentinelStart) + 1;
    memcpy(_RecognitionSentinel,&UHLBuffer[RecognitionSentinelStart],
           FieldLength);
    _RecognitionSentinel[FieldLength] = '\0';

    if (strcmp(_RecognitionSentinel,"UHL") != 0)
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }

    //
    //  Parse out the origin longitude into decimal degrees.
    //

    FieldLength = (OriginLongitudeEnd - 
                   OriginLongitudeStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[OriginLongitudeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",&Degrees,&Minutes,&Seconds,&Hemisphere);
    _OriginLongitude = (double) Degrees;
    _OriginLongitude += (double) Minutes / 60.0;
    _OriginLongitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'W') || (Hemisphere == 'w'))
    {
      _OriginLongitude = -_OriginLongitude;
    }

    //
    //  Parse out the origin latitude into decimal degrees.
    //

    FieldLength = (OriginLatitudeEnd - 
                   OriginLatitudeStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[OriginLatitudeStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%3d%2d%2d%1c",&Degrees,&Minutes,&Seconds,&Hemisphere);
    _OriginLatitude = (double) Degrees;
    _OriginLatitude += (double) Minutes / 60.0;
    _OriginLatitude += (double) Seconds / (60.0 * 60.0);
    if ((Hemisphere == 'S') || (Hemisphere == 's'))
    {
      _OriginLatitude = -_OriginLatitude;
    }

    //
    //  Parse out the latitude data interval into seconds.
    //

    FieldLength = (LongitudeDataIntervalEnd - 
                   LongitudeDataIntervalStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[LongitudeDataIntervalStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%d",&LongitudeDataInterval);
    _LongitudeDataInterval = (float) LongitudeDataInterval / 10.0f;
    

    //
    //  Parse out the latitude data interval into seconds.
    //

    FieldLength = (LatitudeDataIntervalEnd - 
                   LatitudeDataIntervalStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[LatitudeDataIntervalStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%d",&LatitudeDataInterval);
    _LatitudeDataInterval = (float) LatitudeDataInterval / 10.0f;

    //
    //  Parse out the absolute vertical accuracy.  Use USHRT_MAX
    //  if value is "NA".
    //

    FieldLength = (AbsoluteVerticalAccuracyEnd - 
                   AbsoluteVerticalAccuracyStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[AbsoluteVerticalAccuracyStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strstr(StringBuffer,"NA") == NULL)
    {
      sscanf(StringBuffer,"%hd",&_AbsoluteVerticalAccuracy);
    }
    else
    {
      _AbsoluteVerticalAccuracy = USHRT_MAX;
    }

    //
    //  Parse out the security code.
    //

    FieldLength = (SecurityCodeEnd - 
                   SecurityCodeStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[SecurityCodeStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%c",&_SecurityCode);

    //
    //  Parse out the unique reference.  It is an optional field, so
    //  if blank, set to 0.
    //

    FieldLength = (UniqueReferenceEnd - 
                   UniqueReferenceStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[UniqueReferenceStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strcmp(StringBuffer,"            ") != 0)
    {
      sscanf(StringBuffer,"%hd",&_UniqueReference);
    }
    else
    {
      _UniqueReference = 0;
    }

    //
    //  Parse out the Number of longitude lines.
    //

    FieldLength = (NumberOfLongitudeLinesEnd - 
                   NumberOfLongitudeLinesStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[NumberOfLongitudeLinesStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%hd",&_NumberOfLongitudeLines);

    //
    //  Parse out the number of latitude points.  It is an optional field,
    //  so if blank, set to 1201.
    //

    FieldLength = (NumberOfLatitudePointsEnd - 
                   NumberOfLatitudePointsStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[NumberOfLatitudePointsStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strcmp(StringBuffer,"    ") != 0)
    {
      sscanf(StringBuffer,"%hd",&_NumberOfLatitudePoints);
    }
    else
    {
      _NumberOfLatitudePoints = 1201;
    }

    //
    //  Parse out the multiple accuracy.
    //

    FieldLength = (MultipleAccuracyEnd - 
                   MultipleAccuracyStart) + 1;
    memcpy(StringBuffer,&UHLBuffer[MultipleAccuracyStart],FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%hd",&_MultipleAccuracy);

    return XP_GIS_OK;
}

//
//  Method to compute the byte offset from the start of the file to the
//  start of the UHL record.  This value is different for tape-format
//  and CDROM-format files.  Tape format files have an extra 80 byte 
//  HDR record as the first record.  We can recognize this by looking
//  at the first 3 bytes of the file.  It they are "HDR", then this is
//  a tape-format file.  Otherwise, it should be a CDROM-format file
//  (this will be confirmed when reading the various header records by
//  looking at their sentinels).
//

unsigned long XP_GIS_DTEDUHL_c::_RecordStart(ifstream      *FileStream,
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
    *RecordStart = 0;
    if (strcmp(RecordSentinel,"HDR") == 0)
    {
      *RecordStart += HDRRecordLength;
    }

    return XP_GIS_OK;
}

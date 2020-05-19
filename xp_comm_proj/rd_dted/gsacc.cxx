//
//  This file contains the source for the XP_GIS_DTEDACC_c class.
//  This class reads a ACC record from a DTED file.
//

#include <stdio.h>

#include "gsacc.h"


//  Define the length of the ACC record.

static const unsigned long ACCRecordLength = 57; // in bytes

//  Define the location of each field in the ACC record.

static const unsigned long RecognitionSentinelStart         = 0;
static const unsigned long RecognitionSentinelEnd           = 2;

static const unsigned long AbsoluteHorizontalAccuracyStart  = 3;
static const unsigned long AbsoluteHorizontalAccuracyEnd    = 6;

static const unsigned long AbsoluteVerticalAccuracyStart    = 7;
static const unsigned long AbsoluteVerticalAccuracyEnd      = 10;

static const unsigned long RandomHorizontalAccuracyStart    = 11;
static const unsigned long RandomHorizontalAccuracyEnd      = 14;

static const unsigned long RandomVerticalAccuracyStart      = 15;
static const unsigned long RandomVerticalAccuracyEnd        = 18;

static const unsigned long MultipleAccuracyOutlineFlagStart = 55;
static const unsigned long MultipleAccuracyOutlineFlagEnd   = 56;

//
//  Constructor.
//

XP_GIS_DTEDACC_c::XP_GIS_DTEDACC_c()
{
}

//
//  destructor.
//

XP_GIS_DTEDACC_c::~XP_GIS_DTEDACC_c()
{
}

//
//  Copy constructor.
//

XP_GIS_DTEDACC_c::XP_GIS_DTEDACC_c(const XP_GIS_DTEDACC_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DTEDACC_c &XP_GIS_DTEDACC_c::operator=(
                      const XP_GIS_DTEDACC_c &object)
{
    strcpy(_RecognitionSentinel,object._RecognitionSentinel);
    _AbsoluteHorizontalAccuracy  = object._AbsoluteHorizontalAccuracy;
    _AbsoluteVerticalAccuracy    = object._AbsoluteVerticalAccuracy;
    _RandomHorizontalAccuracy    = object._RandomHorizontalAccuracy;
    _RandomVerticalAccuracy      = object._RandomVerticalAccuracy;
    _MultipleAccuracyOutlineFlag = object._MultipleAccuracyOutlineFlag;

    return *this;
}

//
//  Method to set the file stream.  When the file stream is set,
//  the ACC record is read from the file and the various properties
//  of the class populated with data from the record.
//

unsigned long XP_GIS_DTEDACC_c::Read(ifstream *FileStream)
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

    return _ReadACC(FileStream);
}

//
//  Method to print a human-readable version of the ACC record.
//  This is printed to PrintStream.
//

unsigned long XP_GIS_DTEDACC_c::Print(ostream &PrintStream) const
{

    if (PrintStream == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    PrintStream << "ACC record contents:" << endl;

    PrintStream << "  Absolute Horizontal Accuracy = ";
    if (_AbsoluteHorizontalAccuracy < USHRT_MAX)
    {
      PrintStream << _AbsoluteHorizontalAccuracy;
    }
    else
    {
      PrintStream << "NA";
    }
    PrintStream << endl;

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

    PrintStream << "  Random Horizontal Accuracy = ";
    if (_RandomHorizontalAccuracy < USHRT_MAX)
    {
      PrintStream << _RandomHorizontalAccuracy;
    }
    else
    {
      PrintStream << "NA";
    }
    PrintStream << endl;

    PrintStream << "  Random Vertical Accuracy = ";
    if (_RandomVerticalAccuracy < USHRT_MAX)
    {
      PrintStream << _RandomVerticalAccuracy;
    }
    else
    {
      PrintStream << "NA";
    }
    PrintStream << endl;

    PrintStream << "  Multiple Accuracy Outline Flag = ";
    PrintStream << _MultipleAccuracyOutlineFlag;
    PrintStream << endl;

    return XP_GIS_OK;
}

//
//  Private method to read the ACC record.  This method validates the
//  file stream, reads the record, and parses out all the fields in
//  the ACC record.
//

unsigned long XP_GIS_DTEDACC_c::_ReadACC(ifstream *FileStream)
{
    unsigned long      RecordStart;
    unsigned long      ReturnValue;
    unsigned char      ACCBuffer[ACCRecordLength];
    char               StringBuffer[ACCRecordLength + 1];
    unsigned long      FieldLength;

    //
    //  Make sure the file stream has been set.
    //

    if (FileStream == NULL)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Find the start of the ACC record
    //

    if ((ReturnValue = _RecordStart(FileStream,&RecordStart)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Seek to the start of the ACC record.
    //

    if (!FileStream->seekg(RecordStart,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the record header
    //

    if (!FileStream->read(ACCBuffer,
                          ACCRecordLength))
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
    if (FileStream->gcount() != ACCRecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the recognition sentinel
    //

    FieldLength = (RecognitionSentinelEnd - RecognitionSentinelStart) + 1;
    memcpy(_RecognitionSentinel,&ACCBuffer[RecognitionSentinelStart],
           FieldLength);
    _RecognitionSentinel[FieldLength] = '\0';
    if (strcmp(_RecognitionSentinel,"ACC") != 0)
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }

    //
    //  Parse out the absolute horizontal accuracy.  Use USHRT_MAX
    //  if value is "NA".
    //

    FieldLength = (AbsoluteHorizontalAccuracyEnd - 
                   AbsoluteHorizontalAccuracyStart) + 1;
    memcpy(StringBuffer,&ACCBuffer[AbsoluteHorizontalAccuracyStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strstr(StringBuffer,"NA") == NULL)
    {
      sscanf(StringBuffer,"%hd",&_AbsoluteHorizontalAccuracy);
    }
    else
    {
      _AbsoluteHorizontalAccuracy = USHRT_MAX;
    }

    //
    //  Parse out the absolute vertical accuracy.  Use USHRT_MAX
    //  if value is "NA".
    //

    FieldLength = (AbsoluteVerticalAccuracyEnd - 
                   AbsoluteVerticalAccuracyStart) + 1;
    memcpy(StringBuffer,&ACCBuffer[AbsoluteVerticalAccuracyStart],
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
    //  Parse out the random horizontal accuracy.  Use USHRT_MAX
    //  if value is "NA".
    //

    FieldLength = (RandomHorizontalAccuracyEnd - 
                   RandomHorizontalAccuracyStart) + 1;
    memcpy(StringBuffer,&ACCBuffer[RandomHorizontalAccuracyStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strstr(StringBuffer,"NA") == NULL)
    {
      sscanf(StringBuffer,"%hd",&_RandomHorizontalAccuracy);
    }
    else
    {
      _RandomHorizontalAccuracy = USHRT_MAX;
    }

    //
    //  Parse out the random vertical accuracy.  Use USHRT_MAX
    //  if value is "NA".
    //

    FieldLength = (RandomVerticalAccuracyEnd - 
                   RandomVerticalAccuracyStart) + 1;
    memcpy(StringBuffer,&ACCBuffer[RandomVerticalAccuracyStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    if (strstr(StringBuffer,"NA") == NULL)
    {
      sscanf(StringBuffer,"%hd",&_RandomVerticalAccuracy);
    }
    else
    {
      _RandomVerticalAccuracy = USHRT_MAX;
    }

    //
    //  Parse out the multiple accuracy outline flag
    //

    FieldLength = (MultipleAccuracyOutlineFlagEnd - 
                   MultipleAccuracyOutlineFlagStart) + 1;
    memcpy(StringBuffer,&ACCBuffer[MultipleAccuracyOutlineFlagStart],
           FieldLength);
    StringBuffer[FieldLength] = '\0';
    sscanf(StringBuffer,"%hd",&_MultipleAccuracyOutlineFlag);


    return XP_GIS_OK;

}

//
//  Method to compute the byte offset from the start of the file to the
//  start of the ACC record.  This value is different for tape-format
//  and CDROM-format files.  Tape format files have an extra 80 byte 
//  HDR record as the first record.  We can recognize this by looking
//  at the first 3 bytes of the file.  It they are "HDR", then this is
//  a tape-format file.  Otherwise, it should be a CDROM-format file
//  (this will be confirmed when reading the various header records by
//  looking at their sentinels).
//

unsigned long XP_GIS_DTEDACC_c::_RecordStart(ifstream      *FileStream,
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
    *RecordStart = 728;
    if (strcmp(RecordSentinel,"HDR") == 0)
    {
      *RecordStart += HDRRecordLength;
    }

    return XP_GIS_OK;
}

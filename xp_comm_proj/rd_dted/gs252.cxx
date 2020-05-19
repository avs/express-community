//
//  This file contains the source for the XP_GIS_DTED252_c class.
//  This class reads a 252 record from a DTED file.  The 252 record
//  is the record that contains the elevation data for a single column
//  of grid posts.  This is called a 252 record because the record sentinel
//  is a 1 byte field is a value of octal 252 (decimal 170).
//

#include <stdio.h>

#include "gs252.h"
#include "gsbyteu.h"



//  Define the starting byte within the header record.

static const unsigned long RecognitionSentinelStart      = 0;
static const unsigned long RecognitionSentinelEnd        = 0;

static const unsigned long DataBlockCountStart           = 1;
static const unsigned long DataBlockCountEnd             = 3;

static const unsigned long LongitudeCountStart           = 4;
static const unsigned long LongitudeCountEnd             = 5;

static const unsigned long LatitudeCountStart            = 6;
static const unsigned long LatitudeCountEnd              = 7;

static const unsigned long ElevationDataStart            = 8;

//
//  Create a static byte util object to use for byte swapping.
//

static GIS_ByteUtil_c ByteUtil;

//
//  Constructor.
//

XP_GIS_DTED252_c::XP_GIS_DTED252_c(char          *LocalFileName,
                                   unsigned short LocalNumberOfPoints)
 {

    //
    //  If the file name was provided, load it.
    //

    if (LocalFileName != NULL)
    {
      FileName(LocalFileName);
    }
    else
    {
      _FileName[0] = '\0';
    }


    //
    //  If the number of points was provided, load it.
    //

    _NumberOfPoints = 0;
    _RecordLength   = 0;
    _DataArray      = NULL;
    _RecordBuffer   = NULL;

    if (LocalNumberOfPoints != 0)
    {
      NumberOfPoints(LocalNumberOfPoints);
    }
}

//
//  destructor.
//

XP_GIS_DTED252_c::~XP_GIS_DTED252_c()
{
    if (_DataArray    != NULL) free (_DataArray);
    if (_RecordBuffer != NULL) free (_RecordBuffer);
}

//
//  Copy constructor.
//

XP_GIS_DTED252_c::XP_GIS_DTED252_c(const XP_GIS_DTED252_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DTED252_c &XP_GIS_DTED252_c::operator=(
                      const XP_GIS_DTED252_c &object)
{
    _RecognitionSentinel = object._RecognitionSentinel;
    _DataBlockCount = object._DataBlockCount;
    _LongitudeCount = object._LongitudeCount;
    _LatitudeCount = object._LatitudeCount;
    _Checksum = object._Checksum;

    _DataArray    = NULL;
    _RecordBuffer = NULL;
    strcpy(_FileName,object._FileName);
    NumberOfPoints(object._NumberOfPoints);
    if (_NumberOfPoints != 0)
    {
      memcpy(_DataArray,object._DataArray,_NumberOfPoints * 2);
      memcpy(_RecordBuffer,object._RecordBuffer,_RecordLength);
    }

    return *this;
}

//
//  Method to set the file name.  The file name is only used to see if we
//  are reading from a different file.  If so, then we need to check to
//  see if the file is a tape-format or CDROM-format file.  This check
//  is performed in the read method.
//

unsigned long XP_GIS_DTED252_c::FileName(char *LocalFileName)
{
    if (LocalFileName == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }
    strcpy(_FileName,LocalFileName);
    return XP_GIS_OK;
}

//
//  Method to set the number of points in a column of grid posts.
//  Based on the number of points, the record length is set annd the
//  data array and record buffer are allocated.
//

unsigned long XP_GIS_DTED252_c::NumberOfPoints(
                                  unsigned short LocalNumberOfPoints)
{

    //
    //  Validate the number of points
    //

    if ((LocalNumberOfPoints <= 0) ||
        (LocalNumberOfPoints > 1201))
    {
      return XP_GIS_BAD_VALUE;
    }
    _NumberOfPoints = LocalNumberOfPoints;

    //
    //  Calculate the record length;
    //
    
    _RecordLength = (LatitudeCountEnd + 1) +   // record header
                    (_NumberOfPoints * 2)  +   // elevation data
                    4;                         // checksum length

    //
    //  Allocate the data array.
    //

    if (_DataArray != NULL) free(_DataArray);
    _DataArray = (short *) malloc(sizeof (short) *
                                  _NumberOfPoints * 2);
    if (_DataArray == NULL)
    {
      _NumberOfPoints = 0;
      _RecordLength   = 0;
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Allocate the record buffer
    //

    if (_RecordBuffer != NULL) free(_RecordBuffer);
    _RecordBuffer = (unsigned char *) malloc(sizeof (unsigned char) * 
                                             _RecordLength);
    if (_RecordBuffer == NULL)
    {
      _NumberOfPoints = 0;
      _RecordLength   = 0;
      if (_DataArray != NULL) free (_DataArray); 
      return XP_GIS_ALLOC_ERROR;
    }
  return XP_GIS_OK;
}

//
//  Method to print a human-readable version of the data record's
//  header.  This is printed to PrintStream.
//

unsigned long XP_GIS_DTED252_c::Print(ostream &PrintStream) const
{

    if (PrintStream == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    PrintStream << "Elevation record contents:" << endl;

    PrintStream << "  Recognition Sentinel   = ";
    PrintStream << _RecognitionSentinel << endl;

    PrintStream << "  Data Block Count       = ";
    PrintStream << _DataBlockCount << endl;

    PrintStream << "  Longitude Count        = ";
    PrintStream << _LongitudeCount << endl;

    PrintStream << "  Latitude Count         = ";
    PrintStream << _LatitudeCount << endl;

    PrintStream << "  Checksum               = ";
    PrintStream << _Checksum << endl;

    return XP_GIS_OK;
}

//
//  Method to read a single data (252) record from the DTED file.
//  This method validates the inputs, reads the record, parses out
//  the record header, and then  populates the data array with the
//  record's elevation values.
//


unsigned long XP_GIS_DTED252_c::Read(unsigned long RecordNumber,
                                     ifstream     *FileStream)
{
    static unsigned long FirstElevationRecordStart = 0;
    static char          OldFileName[_MAX_PATH];
    unsigned long  ReturnValue;
    unsigned long  RecordStart;
    unsigned long  FieldLength;
    unsigned long  ChecksumStart;
    unsigned short PointNumber;
    unsigned long  Index;
    unsigned long  ByteNumber;
    long           ComputedSum;
    union _uData
    {
      long          LongData;
      short         ShortData[2];
      unsigned char uCharData[4];
    } uData;
    union _uElevation
    {
      short         ShortData;
      unsigned char uCharData[2];
    } uElevation;

    //
    //  Make sure the number of points has been set.
    //

    if ((_NumberOfPoints == 0) ||
        (_NumberOfPoints > 1201))
    {
      return XP_GIS_ERROR;
    }

    //
    //  Make sure the record buffer has been allocated.
    //

    if (_RecordBuffer == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  Make sure the data array has been allocated.
    //

    if (_DataArray == NULL)
    {
      return XP_GIS_ERROR;
    }

    //
    //  Make sure the file stream has been set.
    //

    if (FileStream == NULL)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Find the start of the first elevation record
    //
    //  If we have changed files, we need to recompute the first elevation
    //  record's starting point.  This is because it starts at different
    //  places for tape and cdrom format files.
    //

    if ((strcmp(OldFileName,_FileName) != 0) ||
        (FirstElevationRecordStart     == 0))
    {
      if ((ReturnValue = _FirstElevationRecordStart(FileStream,
                             &FirstElevationRecordStart)) != XP_GIS_OK)
      {
        return ReturnValue;
      }
      strcpy(OldFileName,_FileName);
    }

    //
    //  Seek to the start of the record (zero based).
    //

    RecordStart = FirstElevationRecordStart +
                  ((RecordNumber) * _RecordLength);

    if (!FileStream->seekg(RecordStart,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  read the record header
    //

    if (!FileStream->read(_RecordBuffer,
                          _RecordLength))
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
    if (FileStream->gcount() != (int) _RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the recognition sentinel
    //

    FieldLength = (RecognitionSentinelEnd - RecognitionSentinelStart) + 1;
    _RecognitionSentinel = _RecordBuffer[RecognitionSentinelStart];

    if (_RecognitionSentinel != 0252)  // octal 252 expected
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }

    //
    //  Parse out the data block count.
    //

    FieldLength = (DataBlockCountEnd - DataBlockCountStart) + 1;

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      uData.uCharData[0] = _RecordBuffer[DataBlockCountStart + 2];
      uData.uCharData[1] = _RecordBuffer[DataBlockCountStart + 1];
      uData.uCharData[2] = _RecordBuffer[DataBlockCountStart + 0];
    }
    else
    {
      uData.uCharData[0] = _RecordBuffer[DataBlockCountStart + 0];
      uData.uCharData[1] = _RecordBuffer[DataBlockCountStart + 1];
      uData.uCharData[2] = _RecordBuffer[DataBlockCountStart + 2];
    }
    uData.uCharData[3] = 0;

    _DataBlockCount = uData.LongData;

    //
    //  Parse out the longitude count.
    //

    FieldLength = (LongitudeCountEnd - LongitudeCountStart) + 1;

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes2(2,&_RecordBuffer[LongitudeCountStart]);
    }
    uData.uCharData[0] = _RecordBuffer[LongitudeCountStart + 0];
    uData.uCharData[1] = _RecordBuffer[LongitudeCountStart + 1];
    _LongitudeCount = uData.ShortData[0];

    //
    //  Parse out the latitude count.
    //

    FieldLength = (LatitudeCountEnd - LatitudeCountStart) + 1;

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes2(2,&_RecordBuffer[LatitudeCountStart]);
    }
    uData.uCharData[0] = _RecordBuffer[LatitudeCountStart + 0];
    uData.uCharData[1] = _RecordBuffer[LatitudeCountStart + 1];
    _LatitudeCount = uData.ShortData[0];

    //
    //  Parse out the elevation data.  Note that we may need to swap
    //  the bytes.  Also, the data values are stored in signed magnitude
    //  format.  To decode this format, we mask off the most significant
    //  bit and copy the rest to the data array.  Then, if the most
    //  significant bit is 1, we flip the sign of the value in the
    //  data array.
    //
    Index = ElevationDataStart;
    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
      {
        for (PointNumber = 0; PointNumber < _NumberOfPoints; PointNumber++)
          {
            uElevation.uCharData[1] = _RecordBuffer[Index++] & 0x7f;
            uElevation.uCharData[0] = _RecordBuffer[Index]; // 0111 1111
            
            _DataArray[PointNumber] = uElevation.ShortData;
            
            if ((_RecordBuffer[Index-1] & 0X80) != 0)   // 1000 0000
              {
                _DataArray[PointNumber] *= -1;
              }
            
            Index++;
          }
      }
    else
      {
        for (PointNumber = 0; PointNumber < _NumberOfPoints; PointNumber++)
          {
            uElevation.uCharData[0] = _RecordBuffer[Index++] & 0x7f; // 0111 1111
            uElevation.uCharData[1] = _RecordBuffer[Index];
            
            _DataArray[PointNumber] = uElevation.ShortData;
            
            if ((_RecordBuffer[Index-1] & 0X80) != 0)   // 1000 0000
              {
                _DataArray[PointNumber] *= -1;
              }
            
            Index++;
          }
      }
    

    //
    //  Parse out the checksum.
    //
    ChecksumStart = ElevationDataStart + (_NumberOfPoints * 2);
    FieldLength = 4;

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&_RecordBuffer[ChecksumStart]);
    }
    uData.uCharData[0] = _RecordBuffer[ChecksumStart + 0];
    uData.uCharData[1] = _RecordBuffer[ChecksumStart + 1];
    uData.uCharData[2] = _RecordBuffer[ChecksumStart + 2];
    uData.uCharData[3] = _RecordBuffer[ChecksumStart + 3];
    _Checksum = uData.LongData;

    //
    //  Is the checksum correct?
    //

    ComputedSum = 0;
    for (ByteNumber = 0; ByteNumber < _RecordLength - 4; ByteNumber++)
    {
      ComputedSum += _RecordBuffer[ByteNumber];
    }

    if (ComputedSum != _Checksum)
    {
      cout << "Checksum is incorrect for record " << RecordNumber;
      cout << " -- computed = " << ComputedSum;
      cout << " stored = " << _Checksum << endl;
      return XP_GIS_BAD_CHECKSUM;
    }

    return XP_GIS_OK;
}

//
//  Method to compute the byte offset from the start of the file to the
//  start of the first elevation data record (the first 252 record).
//  This value is different for tape-format and CDROM-format files.
//  Tape format files have an extra 80 byte HDR record as the first
//  record.  We can recognize this by looking at the first 3 bytes of the
//  file.  It they are "HDR", then this is a tape-format file.  Otherwise,
//  it should be a CDROM-format file (this will be confirmed when reading
//  the various header records by looking at their sentinels).
//

unsigned long XP_GIS_DTED252_c::_FirstElevationRecordStart(
                                             ifstream      *FileStream,
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
    *RecordStart = 3428;
    if (strcmp(RecordSentinel,"HDR") == 0)
    {
      *RecordStart += HDRRecordLength;
    }

    return XP_GIS_OK;
}

#include <stdio.h>

#ifdef MSDOS
#include <basetsd.h>
#endif

#include "gsbyteu.h"
#include "gsshape.h"

//  Define the lengths of the various fixed-length file items.

static const unsigned long IndexRecordLength           =   8; // in bytes
static const unsigned long RecordHeaderLength          =   8; // in bytes
static const unsigned long PointRecordLengthOriginal   =  20; // in bytes (original)
static const unsigned long PointRecordLength           =  64; // in bytes (from actual files)
static const unsigned long ArcPart1RecordLength        =  44; // in bytes
static const unsigned long PolygonPart1RecordLength    =  44; // in bytes
static const unsigned long MultiPointPart1RecordLength =  40; // in bytes

//  Define the starting byte within the index record.

static const unsigned long RecordOffsetStart     = 0;
static const unsigned long RecordLengthStart     = 4;

//  Define the starting byte within the record header for record header items.

static const unsigned long RecordNumberStart  = 0;
static const unsigned long ContentLengthStart = 4;

//  Define the starting byte within the point record for point items.

static const unsigned long PointXStart        = 4;
static const unsigned long PointYStart        = 12;

//  Define the starting byte within the arc record for point items.

static const unsigned long ArcXMinStart           = 4;
static const unsigned long ArcYMinStart           = 12;
static const unsigned long ArcXMaxStart           = 20;
static const unsigned long ArcYMaxStart           = 28;
static const unsigned long ArcNumberOfPartsStart  = 36;
static const unsigned long ArcNumberOfPointsStart = 40;

//  Define the starting byte within the polygon record for point items.

static const unsigned long PolygonXMinStart           = 4;
static const unsigned long PolygonYMinStart           = 12;
static const unsigned long PolygonXMaxStart           = 20;
static const unsigned long PolygonYMaxStart           = 28;
static const unsigned long PolygonNumberOfPartsStart  = 36;
static const unsigned long PolygonNumberOfPointsStart = 40;

//  Define the starting byte within the multipoint record for point items.

static const unsigned long MultiPointXMinStart           = 4;
static const unsigned long MultiPointYMinStart           = 12;
static const unsigned long MultiPointXMaxStart           = 20;
static const unsigned long MultiPointYMaxStart           = 28;
static const unsigned long MultiPointNumberOfPointsStart = 36;

//  Create a static byte util object to use for byte swapping.

static GIS_ByteUtil_c ByteUtil;

//
//  Constructor.
//

XP_GIS_ESRI_Shape_c::XP_GIS_ESRI_Shape_c(const char *BaseFileName)
{

    //
    //  Initialize strings to empty.
    //

    _FileName[0]      = '\0';
    _ShapeFileName[0] = '\0';
    _IndexFileName[0] = '\0';

    //
    //  Initialize file states to closed.
    //

    _ShapeFileIsOpen = XP_GIS_FALSE;
    _IndexFileIsOpen = XP_GIS_FALSE;

    //
    //  If the file name is null, then simply return.
    //  and return.
    //

    if (BaseFileName == NULL)
    {
      return;
    }

    //
    //  Otherwise, load the file name.
    //

    FileName(BaseFileName);
}

//
//  destructor.
//

XP_GIS_ESRI_Shape_c::~XP_GIS_ESRI_Shape_c()
{
    //
    //  Close open files.
    //

    CloseFiles();
}

//
//  Copy constructor.
//

XP_GIS_ESRI_Shape_c::XP_GIS_ESRI_Shape_c(const XP_GIS_ESRI_Shape_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_ESRI_Shape_c &XP_GIS_ESRI_Shape_c::operator=(
                      const XP_GIS_ESRI_Shape_c &object)
{

    //
    //  To set up the object properly, all we have to do is set the
    //  file name via the accessor.  This will set all the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    _ShapeFileIsOpen = XP_GIS_FALSE;
    _IndexFileIsOpen = XP_GIS_FALSE;
    FileName(object._FileName);

    return *this;
}

//
//  Method to set the file name.  If there is already a file opened,
//  this method closes the file before attempting to open the newly
//  specified file.  Returns NULL on failure.  Otherwise returns the
//  named of the newly opened file.
//

const char *XP_GIS_ESRI_Shape_c::FileName(const char* FileName) 
{
    char TempFileName[_MAX_PATH];  // temporary file name to use.

    //
    //  If a null is passed in, then close any open files.
    //

    if (FileName == NULL)
    {
      CloseFiles();
      return _FileName;
    }

    //
    //  If the name that was passed in has a suffix of .shp, .shx,
    //  or .dbf, then remove the suffix and use the result as the 
    //  base file name.  Otherwise, assume the name passed in is
    //  the base name for a set of three shape files.
    //

    if ((strcmp(&FileName[strlen(FileName)-4],".shp") == 0) ||
        (strcmp(&FileName[strlen(FileName)-4],".shx") == 0) ||
        (strcmp(&FileName[strlen(FileName)-4],".dbf") == 0))
    {
      strncpy(_FileName,FileName,strlen(FileName)-4);
      _FileName[strlen(FileName)-4] = '\0';
    }
    else
    {
      strcpy(_FileName,FileName);
    }
    

    strcpy(TempFileName,_FileName);
    strcat(TempFileName,".shp");
    if (ShapeFileName(TempFileName) == NULL)
    {
      CloseFiles();
      return NULL;
    }

    strcpy(TempFileName,_FileName);
    strcat(TempFileName,".shx");
    if (IndexFileName(TempFileName) == NULL)
    {
      CloseFiles();
      return NULL;
    }

    return _FileName;
    
}

void XP_GIS_ESRI_Shape_c::CloseFiles()
{

    //
    //  Set the base name to empty.
    //

    _FileName[0] = '\0';
    CloseShapeFile();
    CloseIndexFile();

    return;
}

void XP_GIS_ESRI_Shape_c::CloseShapeFile()
{

    //
    //  Close the shape file.
    //

    if (ShapeFileIsOpen() == XP_GIS_TRUE)
    {
      _ShapeFileIsOpen = XP_GIS_FALSE;
      _ShapeFileStream.close();
      _ShapeFileName[0] = '\0';
    }

    return;
}

void XP_GIS_ESRI_Shape_c::CloseIndexFile()
{

    //
    //  Close the index file.
    //

    if (IndexFileIsOpen() == XP_GIS_TRUE)
    {
      _IndexFileIsOpen = XP_GIS_FALSE;
      _IndexFileStream.close();
      _IndexFileName[0] = '\0';
    }

    return;
}

const char *XP_GIS_ESRI_Shape_c::ShapeFileName(const char* ShapeFileName) 
{

    //
    //  If there is already a shape file open, close it.
    //

    if (ShapeFileIsOpen() == XP_GIS_TRUE)
    {
      CloseShapeFile();
    }

    //
    //  If the file named passed in is NULL, simply return that.
    //

    if (ShapeFileName == NULL)
    {
      return NULL;
    }

    //
    //  Otherwise, attempt to open the requested file.  If not successful,
    //  clear the shape file name.  Otherwise, populate the file name and
    //  read the shape file header.
    //

#ifdef MSDOS
    _ShapeFileStream.open(ShapeFileName,ios::binary | ios::nocreate);
#else
    _ShapeFileStream.open(ShapeFileName,ios::nocreate);
#endif
    if (_ShapeFileStream.bad())
    {
      _ShapeFileName[0] = '\0';
      return NULL;
    }
    else
    {
      _ShapeFileIsOpen = XP_GIS_TRUE;
      strcpy(_ShapeFileName,ShapeFileName);
      _ShapeHeader.FileName(_ShapeFileName);
      _ShapeHeader.FileStream(_ShapeFileStream);
      if (_ShapeHeader.ReadHeader() != XP_GIS_OK)
      {
        CloseShapeFile();
        _ShapeFileName[0] = '\0';
        return NULL;     
      }
    }
    return _ShapeFileName;
}

const char *XP_GIS_ESRI_Shape_c::IndexFileName(const char* IndexFileName) 
{

    //
    //  If there is already a index file open, close it.
    //

    if (IndexFileIsOpen() == XP_GIS_TRUE)
    {
      CloseIndexFile();
    }

    //
    //  If the file named passed in is NULL, simply return that.
    //

    if (IndexFileName == NULL)
    {
      return NULL;
    }

    //
    //  Otherwise, attempt to open the requested file.  If not successful,
    //  clear the shape file name.  Otherwise, populate the file name.
    //

#ifdef MSDOS
    _IndexFileStream.open(IndexFileName,ios::binary | ios::nocreate);
#else
    _IndexFileStream.open(IndexFileName,ios::nocreate);
#endif
    if (_IndexFileStream.bad())
    {
      _IndexFileName[0] = '\0';
      return NULL;
    }
    else
    {
      _IndexFileIsOpen = XP_GIS_TRUE;
      strcpy(_IndexFileName,IndexFileName);
      _IndexHeader.FileName(_IndexFileName);
      _IndexHeader.FileStream(_IndexFileStream);
      if (_IndexHeader.ReadHeader() != XP_GIS_OK)
      {
        CloseIndexFile();
        return NULL;     
      }
    }
    return _IndexFileName;
}

//
//  Method to get the information about an ARC shape feature.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::GetArcShapeInfo(
                                  long *RecordNumber,
                                  long *NumberOfParts,
                                  long *NumberOfPoints)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[ArcPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= ArcPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the arc record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                          ArcPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != ArcPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the arc part 1 data
    //

    //
    //  Number of parts
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[ArcNumberOfPartsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[ArcNumberOfPartsStart],4);
    *NumberOfParts = (long)int32_val;

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[ArcNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[ArcNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    return XP_GIS_OK;
}

//
//  Method to get the information about a polygon shape feature.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::GetPolygonShapeInfo(
                                  long *RecordNumber,
                                  long *NumberOfParts,
                                  long *NumberOfPoints)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[PolygonPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the polygon record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                          PolygonPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != PolygonPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the polygon part 1 data
    //

    //
    //  Number of parts
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[PolygonNumberOfPartsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[PolygonNumberOfPartsStart],4);
    *NumberOfParts = (long)int32_val;

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[PolygonNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[PolygonNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    return XP_GIS_OK;
}

//
//  Method to get the information about a multipoint shape feature.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::GetMultiPointShapeInfo(
                                  long *RecordNumber,
                                  long *NumberOfPoints)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[MultiPointPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= MultiPointPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the multipoint record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                               MultiPointPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != MultiPointPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the multipoint part 1 data
    //

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[MultiPointNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[MultiPointNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    return XP_GIS_OK;
}

//
//  Method to get the number of data records in the shape file.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_NOT_IMPLEMENTED
//  XP_GIS_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::NumberOfDataRecords(
                                  unsigned long *NumberOfRecords)
{

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  The number of data records can be determined by looking at the
    //  index file.  The index file has a header followed by one fixed-
    //  length record per .shp data record.  The header contains a file
    //  length field we can use to figure out how many data records
    //  there are in the .shp file.
    //

    *NumberOfRecords = 
               (_IndexHeader.FileLength() - _IndexHeader.HeaderLength()) / 
               IndexRecordLength;

    return XP_GIS_OK;
}

//
//  Method to get the total number of parts in the shape file.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_NOT_IMPLEMENTED
//  XP_GIS_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::NumberOfParts(
                                         unsigned long *NumberOfParts)
{
    long          OriginalPosition;
    unsigned long ReturnValue;

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Find out where we are in the file so we can seek back there when
    //  we are finished.
    //

    OriginalPosition = _ShapeFileStream.tellg();

    //
    //  The way we compute the number of parts depends on the shape type.
    //  For null, point, and multipoint features, the number of
    //  parts is the same as the number of data records.
    //  For arc and polygon featues, the number of parts must be computed
    //  by looking at each feature.
    //

    switch (_ShapeHeader.ShapeType())
    {
      case XP_GIS_NULL_SHAPE:
      case XP_GIS_POINT_SHAPE:
      case XP_GIS_MULTIPOINT_SHAPE:
        ReturnValue = NumberOfDataRecords(NumberOfParts);
        break;
      case XP_GIS_ARC_SHAPE:
      case XP_GIS_POLYGON_SHAPE:
        {
          unsigned long NumberOfFeatures;
          long          FeatureNumber;
          long          RecordOffset;
          long          RecordLength;
#ifdef MSDOS
          UINT32 int32_val;
#else
          uint32_t int32_val;
#endif
          //long          LongValue;
          unsigned char IOBuffer[4];

          ReturnValue = NumberOfDataRecords(&NumberOfFeatures);
          if (ReturnValue != XP_GIS_OK)
          {
            break;
          }
          *NumberOfParts = 0;
          for (FeatureNumber = 1;
               FeatureNumber <= (long) NumberOfFeatures;
               FeatureNumber++)
          {

            //
            //  Find out where to seek to.
            //

            ReturnValue = ReadIndexRecord(FeatureNumber,
                                         &RecordOffset,
                                         &RecordLength);
            if (ReturnValue != XP_GIS_OK)
            {
              break;
            }
            RecordOffset += RecordHeaderLength;
            if (_ShapeHeader.ShapeType() == XP_GIS_ARC_SHAPE)
            { 
              if ((unsigned long) RecordLength <= ArcPart1RecordLength)
              {
                continue;     //  invalid record length...try next feature
              }
              RecordOffset += ArcNumberOfPartsStart; 
            }
            else if (_ShapeHeader.ShapeType() == XP_GIS_POLYGON_SHAPE)
            {
              if ((unsigned long) RecordLength <= PolygonPart1RecordLength)
              {
                continue;     //  invalid record length...try next feature
              }
              RecordOffset += PolygonNumberOfPartsStart; 
            }

            //
            //  Seek to the start of the number of parts field
            //  in the specified data record.    
            //

            if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
            {
              ReturnValue = XP_GIS_SEEK_ERROR;
              break;
            }

            //
            // Read the number of parts
            //

            if (!_ShapeFileStream.read(IOBuffer,4))
            {
              if (_ShapeFileStream.eof())
              {
                ReturnValue = XP_GIS_EOF;
                break;
              }
              else
              {
                ReturnValue = XP_GIS_READ_ERROR;
                break;
              }
            }
            if (_ShapeFileStream.gcount() != 4)
            {
              ReturnValue = XP_GIS_IO_ERROR;
              break;
            }

            //
            // Parse out the number of Parts
            //
           
            if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
            {
              ByteUtil.SwapBytes4(4,&IOBuffer);
            }
            memcpy(&int32_val,IOBuffer,4);
            (*NumberOfParts) += (long)int32_val;
          }
          break;
        }
      default:
        return XP_GIS_ERROR;
    }

    //
    //  Seek back to the place the file pointer was at when we
    //  entered this routine.
    //

    if (!_ShapeFileStream.seekg(OriginalPosition,ios::beg))
    {
      ReturnValue = XP_GIS_SEEK_ERROR;
    }
    return ReturnValue;
}

//
//  Method to get the total number of points in the shape file.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_NOT_IMPLEMENTED
//  XP_GIS_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::NumberOfPoints(
                                         unsigned long *NumberOfPoints)
{
    long          OriginalPosition;
    unsigned long ReturnValue;

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Find out where we are in the file so we can seek back there when
    //  we are finished.
    //

    OriginalPosition = _ShapeFileStream.tellg();

    //
    //  The way we compute the number of Points depends on the shape type.
    //  For null and point features, the number of points is the same as
    //  the number of data records.  For arc, polygon, and multipoint
    //  featues, the number of parts must be computed by looking at 
    //  each feature.
    //

    switch (_ShapeHeader.ShapeType())
    {
      case XP_GIS_NULL_SHAPE:
      case XP_GIS_POINT_SHAPE:
        ReturnValue = NumberOfDataRecords(NumberOfPoints);
        break;
      case XP_GIS_ARC_SHAPE:
      case XP_GIS_POLYGON_SHAPE:
      case XP_GIS_MULTIPOINT_SHAPE:
        {
          unsigned long NumberOfFeatures;
          long          FeatureNumber;
          long          RecordOffset;
          long          RecordLength;
#ifdef MSDOS
          UINT32 int32_val;
#else
          uint32_t int32_val;
#endif
          //long          LongValue;
          unsigned char IOBuffer[4];

          ReturnValue = NumberOfDataRecords(&NumberOfFeatures);
          if (ReturnValue != XP_GIS_OK)
          {
            break;
          }
          *NumberOfPoints = 0;
          for (FeatureNumber = 1;
               FeatureNumber <= (long) NumberOfFeatures;
               FeatureNumber++)
          {

            //
            //  Find out where to seek to.
            //

            ReturnValue = ReadIndexRecord(FeatureNumber,
                                         &RecordOffset,
                                         &RecordLength);
            if (ReturnValue != XP_GIS_OK)
            {
              break;
            }
            RecordOffset += RecordHeaderLength;
            if (_ShapeHeader.ShapeType() == XP_GIS_ARC_SHAPE)
            { 
              if ((unsigned long) RecordLength <= ArcPart1RecordLength)
              {
                continue;     //  invalid record length...try next feature
              }
              RecordOffset += ArcNumberOfPointsStart; 
            }
            else if (_ShapeHeader.ShapeType() == XP_GIS_POLYGON_SHAPE)
            {
              if ((unsigned long) RecordLength <= PolygonPart1RecordLength)
              {
                continue;     //  invalid record length...try next feature
              }
              RecordOffset += PolygonNumberOfPointsStart; 
            }
            else if (_ShapeHeader.ShapeType() == XP_GIS_MULTIPOINT_SHAPE)
            {
              if ((unsigned long) RecordLength <= MultiPointPart1RecordLength)
              {
                continue;     //  invalid record length...try next feature
              }
              RecordOffset += MultiPointNumberOfPointsStart; 
            }

            //
            //  Seek to the start of the number of points field
            //  in the specified data record.    
            //

            if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
            {
              ReturnValue = XP_GIS_SEEK_ERROR;
              break;
            }

            //
            // Read the number of points
            //

            if (!_ShapeFileStream.read(IOBuffer,4))
            {
              if (_ShapeFileStream.eof())
              {
                ReturnValue = XP_GIS_EOF;
                break;
              }
              else
              {
                ReturnValue = XP_GIS_READ_ERROR;
                break;
              }
            }
            if (_ShapeFileStream.gcount() != 4)
            {
              ReturnValue = XP_GIS_IO_ERROR;
              break;
            }

            //
            // Parse out the number of points
            //
           
            if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
            {
              ByteUtil.SwapBytes4(4,IOBuffer);
            }
            memcpy(&int32_val,IOBuffer,4);
            (*NumberOfPoints) += (long)int32_val;
          }
          break;
        }
      default:
        return XP_GIS_ERROR;
    }

    //
    //  Seek back to the place the file pointer was at when we
    //  entered this routine.
    //

    if (!_ShapeFileStream.seekg(OriginalPosition,ios::beg))
    {
      ReturnValue = XP_GIS_SEEK_ERROR;
    }
    return ReturnValue;
}

//
//  Method to seek to the first data record in the file.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::SeekToFirstDataRecord()
{

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    if (!_ShapeFileStream.seekg(_ShapeHeader.HeaderLength(),ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }
    return XP_GIS_OK;
}

//
//  Method to seek to the start of the RecordNumber'th data record in the
//  .shp file.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::SeekToDataRecord(long  RecordNumber)
                                   
{
    long          RecordOffset;
    long          RecordLength;


    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Find out where to seek to.
    //

    if (ReadIndexRecord(RecordNumber,
                       &RecordOffset,
                       &RecordLength) != XP_GIS_OK)
    {
      return XP_GIS_ERROR;
    }


    //
    //  Seek to the start of the specified data record.    
    //

    if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    return XP_GIS_OK;
}


//
//  Method to read a null shape.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//

unsigned long XP_GIS_ESRI_Shape_c::ReadNullShape()
{
    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    return XP_GIS_OK;
}

//
//  Method to read a point shape.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadPointShape(long   *RecordNumber,
                                                  double *PointX,
                                                  double *PointY)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[PointRecordLength];
    long RecordBytes;

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if (((unsigned long) RecordBytes != PointRecordLength) &&
        ((unsigned long) RecordBytes != PointRecordLengthOriginal))
    {
      return XP_GIS_ERROR;
    }

    //
    //  Now, read the point record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                               RecordBytes))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if ((_ShapeFileStream.gcount() != PointRecordLength) &&
        (_ShapeFileStream.gcount() != PointRecordLengthOriginal))
   {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the point data
    //

    //
    //  X coordinate
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PointXStart]);
    }
    memcpy(PointX,&RecordBuffer[PointXStart],8);

    //
    //  Y coordinate
    //
    
    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PointYStart]);
    }
    memcpy(PointY,&RecordBuffer[PointYStart],8);
    
    return XP_GIS_OK;
}

//
//  Method to read an arc shape.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadArcShape(long    *RecordNumber,
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
                                                double **YArray)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[ArcPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= ArcPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the arc record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                          ArcPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != ArcPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the arc part 1 data
    //

    //
    //  Bounding box X minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[ArcXMinStart]);
    }
    memcpy(XMin,&RecordBuffer[ArcXMinStart],8);

    //
    //  Bounding box Y minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[ArcYMinStart]);
    }
    memcpy(YMin,&RecordBuffer[ArcYMinStart],8);

    //
    //  Bounding box X maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[ArcXMaxStart]);
    }
    memcpy(XMax,&RecordBuffer[ArcXMaxStart],8);

    //
    //  Bounding box Y maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[ArcYMaxStart]);
    }
    memcpy(YMax,&RecordBuffer[ArcYMaxStart],8);

    //
    //  Number of parts
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[ArcNumberOfPartsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[ArcNumberOfPartsStart],4);
    *NumberOfParts = (long)int32_val;

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[ArcNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[ArcNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    //
    //  If necessary, reallocate the PointIndex array.
    //

    if (*NumberOfParts > *MaximumNumberOfParts)
    {
      *PartIndex = (long *) realloc(*PartIndex,*NumberOfParts * sizeof(long));
      if (*PartIndex == NULL)
      {
        return XP_GIS_ALLOC_ERROR;
      }
      *MaximumNumberOfParts = *NumberOfParts;
    }

    //
    //  If necessary, reallocate the X and Y coordinate array.
    //

    if (*NumberOfPoints > *MaximumNumberOfPoints)
    {
      *XArray = (double *) realloc(*XArray,*NumberOfPoints * sizeof(double));
      *YArray = (double *) realloc(*YArray,*NumberOfPoints * sizeof(double));
      if ((*XArray == NULL) || (*YArray == NULL))
      {
        return XP_GIS_ALLOC_ERROR;
      }
      *MaximumNumberOfPoints = *NumberOfPoints;
    }


    //
    //  If necessary, allocate temporary 32 bit PartIndex array so we can cast to the real long PartIndex.
    //
    if (sizeof(long) != 4) {
#ifdef MSDOS
       UINT32 *TempPartIndex = (UINT32 *)malloc(*NumberOfParts * sizeof(UINT32));
#else
       uint32_t *TempPartIndex = (uint32_t *)malloc(*NumberOfParts * sizeof(uint32_t));
#endif
       if (TempPartIndex == NULL)
       {
         return XP_GIS_ALLOC_ERROR;
       }

       //
       //  Read the PartIndex array
       //

       if (!_ShapeFileStream.read((unsigned char *)TempPartIndex,
                             *NumberOfParts * 4))
       {
         if (_ShapeFileStream.eof())
         {
           free(TempPartIndex);
           return XP_GIS_EOF;
         }
         else
         {
           free(TempPartIndex);
           return XP_GIS_READ_ERROR;
         }
       }
       if (_ShapeFileStream.gcount() != *NumberOfParts * 4)
       {
         free(TempPartIndex);
         return XP_GIS_IO_ERROR;
       }
       if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
       {
         ByteUtil.SwapBytes4(*NumberOfParts * 4,TempPartIndex);
       }

       // Cast the 32 bit temporary array to the real 64-bit array
       for (long PartNumber = 0; PartNumber < *NumberOfParts; PartNumber++) {
         (*PartIndex)[PartNumber] = (long)TempPartIndex[PartNumber];
       }

       //  free the temporary 32 bit PartIndex array.
       free(TempPartIndex);

    }
    else {
       //
       //  Read the PartIndex array
       //

       if (!_ShapeFileStream.read((unsigned char *)*PartIndex,
                             *NumberOfParts * 4))
       {
         if (_ShapeFileStream.eof())
         {
           return XP_GIS_EOF;
         }
         else
         {
           return XP_GIS_READ_ERROR;
         }
       }
       if (_ShapeFileStream.gcount() != *NumberOfParts * 4)
       {
         return XP_GIS_IO_ERROR;
       }
       if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
       {
         ByteUtil.SwapBytes4(*NumberOfParts * 4,*PartIndex);
       }
    }



    //
    //  Read the X and Y coordinate arrays
    //

    for (long PointNumber = 0; PointNumber < *NumberOfPoints; PointNumber++)
    {   
      if (!_ShapeFileStream.read((unsigned char *)&((*XArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }

      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {
        ByteUtil.SwapBytes8(sizeof(double),&((*XArray)[PointNumber]));
      }
    
      if (!_ShapeFileStream.read((unsigned char *)&((*YArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }
      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {
        ByteUtil.SwapBytes8(sizeof(double),&((*YArray)[PointNumber]));
      }
    }
    
    return XP_GIS_OK;
}

//
//  Method to read an polygon shape.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadPolygonShape(
                                          long    *RecordNumber,
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
                                          double **YArray)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[PolygonPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the polygon record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                          PolygonPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }

    if (_ShapeFileStream.gcount() != PolygonPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }
  
    //
    //  Parse out the polygon part 1 data
    //

    //
    //  Bounding box X minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PolygonXMinStart]);
    }
    memcpy(XMin,&RecordBuffer[PolygonXMinStart],8);

    //
    //  Bounding box Y minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PolygonYMinStart]);
    }
    memcpy(YMin,&RecordBuffer[PolygonYMinStart],8);

    //
    //  Bounding box X maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PolygonXMaxStart]);
    }
    memcpy(XMax,&RecordBuffer[PolygonXMaxStart],8);

    //
    //  Bounding box Y maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[PolygonYMaxStart]);
    }
    memcpy(YMax,&RecordBuffer[PolygonYMaxStart],8);

    //
    //  Number of parts
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[PolygonNumberOfPartsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[PolygonNumberOfPartsStart],4);
    *NumberOfParts = (long)int32_val;

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[PolygonNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[PolygonNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    //
    //  If necessary, reallocate the PartIndex array.
    //

    if (*NumberOfParts > *MaximumNumberOfParts)
    {
      *PartIndex = (long *) realloc(*PartIndex,*NumberOfParts * sizeof(long));
      if (*PartIndex == NULL)
      {
        return XP_GIS_ALLOC_ERROR;
      }
      *MaximumNumberOfParts = *NumberOfParts;
    }


    //
    //  If necessary, reallocate the X and Y coordinate array.
    //

    if (*NumberOfPoints > *MaximumNumberOfPoints)
    {
      *XArray = (double *) realloc(*XArray,*NumberOfPoints * sizeof(double));
      *YArray = (double *) realloc(*YArray,*NumberOfPoints * sizeof(double));
      if ((*XArray == NULL) || (*YArray == NULL))
      {
        return XP_GIS_ALLOC_ERROR;
      }
      *MaximumNumberOfPoints = *NumberOfPoints;
    }


    //
    //  If necessary, allocate temporary 32 bit PartIndex array so we can cast to the real long PartIndex.
    //
    if (sizeof(long) != 4) {
#ifdef MSDOS
       UINT32 *TempPartIndex = (UINT32 *)malloc(*NumberOfParts * sizeof(UINT32));
#else
       uint32_t *TempPartIndex = (uint32_t *)malloc(*NumberOfParts * sizeof(uint32_t));
#endif
       if (TempPartIndex == NULL)
       {
         return XP_GIS_ALLOC_ERROR;
       }

       //
       //  Read the PartIndex array
       //

       if (!_ShapeFileStream.read((unsigned char *)TempPartIndex,
                             *NumberOfParts * 4))
       {
         if (_ShapeFileStream.eof())
         {
           free(TempPartIndex);
           return XP_GIS_EOF;
         }
         else
         {
           free(TempPartIndex);
           return XP_GIS_READ_ERROR;
         }
       }
       if (_ShapeFileStream.gcount() != *NumberOfParts * 4)
       {
         free(TempPartIndex);
         return XP_GIS_IO_ERROR;
       }
       if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
       {
         ByteUtil.SwapBytes4(*NumberOfParts * 4,TempPartIndex);
       }

       // Cast the 32 bit temporary array to the real 64-bit array
       for (long PartNumber = 0; PartNumber < *NumberOfParts; PartNumber++) {
         (*PartIndex)[PartNumber] = (long)TempPartIndex[PartNumber];
       }

       //  free the temporary 32 bit PartIndex array.
       free(TempPartIndex);

    }
    else {
       //
       //  Read the PartIndex array
       //

       if (!_ShapeFileStream.read((unsigned char *)*PartIndex,
                             *NumberOfParts * 4))
       {
         if (_ShapeFileStream.eof())
         {
           return XP_GIS_EOF;
         }
         else
         {
           return XP_GIS_READ_ERROR;
         }
       }
       if (_ShapeFileStream.gcount() != *NumberOfParts * 4)
       {
         return XP_GIS_IO_ERROR;
       }
       if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
       {
         ByteUtil.SwapBytes4(*NumberOfParts * 4,*PartIndex);
       }
    }


    //
    //  Read the X and Y coordinate arrays
    //

    for (long PointNumber = 0; PointNumber < *NumberOfPoints; PointNumber++)
    {   
      if (!_ShapeFileStream.read((unsigned char *)&((*XArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }

      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {

        ByteUtil.SwapBytes8(sizeof(double),&((*XArray)[PointNumber]));
      }
    
      if (!_ShapeFileStream.read((unsigned char *)&((*YArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }
      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {
        ByteUtil.SwapBytes8(sizeof(double),&((*YArray)[PointNumber]));
      }
    }
    
    return XP_GIS_OK;
}

//
//  Method to read an multipoint shape.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_ERROR
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadMultiPointShape(
                                                long    *RecordNumber,
                                                double  *XMin,
                                                double  *YMin,
                                                double  *XMax,
                                                double  *YMax,
                                                long    *NumberOfPoints,
                                                long    *MaximumNumberOfPoints,
                                                double **XArray,
                                                double **YArray)
{
    unsigned long ReturnValue;
    unsigned char RecordBuffer[MultiPointPart1RecordLength];
    long RecordBytes;
    long RecordOffset;

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Read the record header
    //

    ReturnValue = ReadShapeRecordHeader(RecordNumber,&RecordBytes);
    if (ReturnValue == XP_GIS_EOF)
    {
      return XP_GIS_EOF;
    }
    else
    if (ReturnValue != XP_GIS_OK)
    {
      return ReturnValue;
    }
    if ((unsigned long) RecordBytes <= MultiPointPart1RecordLength)
    {
      //
      //  OK, we got a bad value.  There is a duplicate value in the
      //  index file.  Lets see if the one there make more sense.
      //

      ReturnValue = ReadIndexRecord(*RecordNumber,
                                    &RecordOffset,
                                    &RecordBytes);
      if (ReturnValue == XP_GIS_EOF)
      {
        return XP_GIS_EOF;
      }
      else
      if (ReturnValue != XP_GIS_OK)
      {
        return ReturnValue;
      }
      if ((unsigned long) RecordBytes <= PolygonPart1RecordLength)
      {
        //
        //  OK, we give up on this feature.  Just seek to the start of
        //  the next feature and return an error.
        //
        ReturnValue = ReadIndexRecord(*RecordNumber+1,
                                      &RecordOffset,
                                      &RecordBytes);
        if (ReturnValue == XP_GIS_EOF)
        {
          return XP_GIS_EOF;
        }
        else
        if (ReturnValue != XP_GIS_OK)
        {
          return ReturnValue;
        }
        if (!_ShapeFileStream.seekg(RecordOffset,ios::beg))
        {
          return XP_GIS_SEEK_ERROR;
        }
        return XP_GIS_ERROR;
      }
    }

    //
    //  Now, read part 1 of the multipoint record contents.
    //

    if (!_ShapeFileStream.read(RecordBuffer,
                               MultiPointPart1RecordLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != MultiPointPart1RecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the multipoint part 1 data
    //

    //
    //  Bounding box X minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[MultiPointXMinStart]);
    }
    memcpy(XMin,&RecordBuffer[MultiPointXMinStart],8);

    //
    //  Bounding box Y minimum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[MultiPointYMinStart]);
    }
    memcpy(YMin,&RecordBuffer[MultiPointYMinStart],8);

    //
    //  Bounding box X maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[MultiPointXMaxStart]);
    }
    memcpy(XMax,&RecordBuffer[MultiPointXMaxStart],8);

    //
    //  Bounding box Y maximum
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&RecordBuffer[MultiPointYMaxStart]);
    }
    memcpy(YMax,&RecordBuffer[MultiPointYMaxStart],8);

    //
    //  Number of Points
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[MultiPointNumberOfPointsStart]);
    }
    memcpy(&int32_val,&RecordBuffer[MultiPointNumberOfPointsStart],4);
    *NumberOfPoints = (long)int32_val;

    //
    //  If necessary, reallocate the X and Y coordinate array.
    //

    if (*NumberOfPoints > *MaximumNumberOfPoints)
    {
      *XArray = (double *) realloc(*XArray,*NumberOfPoints * sizeof(double));
      *YArray = (double *) realloc(*YArray,*NumberOfPoints * sizeof(double));
      if ((*XArray == NULL) || (*YArray == NULL))
      {
        return XP_GIS_ALLOC_ERROR;
      }
      *MaximumNumberOfPoints = *NumberOfPoints;
    }

    //
    //  Read the X and Y coordinate arrays
    //

    for (long PointNumber = 0; PointNumber < *NumberOfPoints; PointNumber++)
    {   
      if (!_ShapeFileStream.read((unsigned char *)&((*XArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }

      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {

        ByteUtil.SwapBytes8(sizeof(double),&((*XArray)[PointNumber]));
      }
    
      if (!_ShapeFileStream.read((unsigned char *)&((*YArray)[PointNumber]),8))
      {
        if (_ShapeFileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_ShapeFileStream.gcount() != 8)
      {
        return XP_GIS_IO_ERROR;
      }
      if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
      {
        ByteUtil.SwapBytes8(sizeof(double),&((*YArray)[PointNumber]));
      }
    }
    
    return XP_GIS_OK;
}

//
//  Method to read a record header
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadShapeRecordHeader(long *RecordNumber,
                                                         long *RecordBytes)
{
    //long          ContentLength;
    unsigned char HeaderBuffer[RecordHeaderLength];

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (ShapeFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  read the record header
    //

    if (!_ShapeFileStream.read(HeaderBuffer,
                          RecordHeaderLength))
    {
      if (_ShapeFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ShapeFileStream.gcount() != RecordHeaderLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the record number
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&HeaderBuffer[RecordNumberStart]);
    }
    memcpy(&int32_val,&HeaderBuffer[RecordNumberStart],4);
    *RecordNumber = (long)int32_val;

    //
    //  Parse out the record size.  It is stored in half words, so we must
    //  convert it to bytes.
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&HeaderBuffer[ContentLengthStart]);
    }
    memcpy(&int32_val,&HeaderBuffer[ContentLengthStart],4);
    *RecordBytes = 2 * (long)int32_val;  // convert to bytes

    return XP_GIS_OK;
}

//
//  Method to get the offset in bytes from the start of the .shp file
//  To the start of record number RecordNumber.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_ESRI_Shape_c::ReadIndexRecord(long  RecordNumber,
                                                   long *RecordOffset,
                                                   long *RecordLength)
{
    long          SeekOffset;
    unsigned char RecordBuffer[IndexRecordLength];

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

    if (IndexFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Seek to the start of the RecordNumber'th index record.
    //

    SeekOffset = _IndexHeader.HeaderLength() + 
                 ((RecordNumber-1) * IndexRecordLength); 
    if (!_IndexFileStream.seekg(SeekOffset,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the index record
    //

    if (!_IndexFileStream.read(RecordBuffer,IndexRecordLength))
    {
      if (_IndexFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_IndexFileStream.gcount() != IndexRecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the record offset in the .shp file.
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[RecordOffsetStart]);
    }
    memcpy(&int32_val,&RecordBuffer[RecordOffsetStart],4);
    *RecordOffset = 2 * (long)int32_val;  // convert from half words to bytes

    //
    //  Parse out the record length in the .shp file.
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&RecordBuffer[RecordLengthStart]);
    }
    memcpy(&int32_val,&RecordBuffer[RecordLengthStart],4);
    *RecordLength = 2 * (long)int32_val;  // convert from half words to bytes

    return XP_GIS_OK;
}


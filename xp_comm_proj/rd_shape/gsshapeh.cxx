//
//  This file contains the source code for the class to read an ESRI
//  shape file header.  A shape file is a collection of three files:
//     1.  a shape file with a suffix of .shp
//     2.  an index file with a suffix of .shx
//     3.  a dBASE file with a suffix of .dbf
//
//  The headers for the .shp and .shx files are identical.  This class
//  reads this header.
//

#ifdef MSDOS
#include <basetsd.h>
#endif

#include "gsbyteu.h"
#include "gsshape.h"

//  Define the lengths of the various fixed-length file items.

static const unsigned long FileHeaderLength      = 100; // in bytes

//  Define the starting byte within the header buffer for header items.

static const unsigned long FileCodeStart      =  0;
static const unsigned long FileLengthStart    = 24;
static const unsigned long VersionStart       = 28;
static const unsigned long ShapeTypeStart     = 32;
static const unsigned long XMinStart          = 36;
static const unsigned long YMinStart          = 44;
static const unsigned long XMaxStart          = 52;
static const unsigned long YMaxStart          = 60;

//  Create a static byte util object to use for byte swapping.

static GIS_ByteUtil_c ByteUtil;

//
//  Constructor.
//

XP_GIS_ESRI_ShapeHeader_c::XP_GIS_ESRI_ShapeHeader_c()
{
    _FileName = NULL;
    _FileStream = NULL;
}

//
//  destructor.
//

XP_GIS_ESRI_ShapeHeader_c::~XP_GIS_ESRI_ShapeHeader_c()
{
}

//
//  Copy constructor
//

XP_GIS_ESRI_ShapeHeader_c::XP_GIS_ESRI_ShapeHeader_c(
                               const XP_GIS_ESRI_ShapeHeader_c &object)
{
    *this = object;
}

//
//  Assignment operator
//

XP_GIS_ESRI_ShapeHeader_c &XP_GIS_ESRI_ShapeHeader_c::operator=(
                               const XP_GIS_ESRI_ShapeHeader_c &object)
{
    _FileStream = object._FileStream;
    _FileName   = object._FileName;
    _FileCode   = object._FileCode;
    _Version    = object._Version;
    _ShapeType  = object._ShapeType;
    _XMin       = object._XMin;
    _YMin       = object._YMin;
    _XMax       = object._XMax;
    _YMax       = object._YMax;

    return *this;
}


const char *XP_GIS_ESRI_ShapeHeader_c::FileName(const char *Name)
{
    return _FileName = (char *) Name;
}

ifstream &XP_GIS_ESRI_ShapeHeader_c::FileStream(ifstream &Stream)
{
    _FileStream = &Stream;
    return *_FileStream;
}

long XP_GIS_ESRI_ShapeHeader_c::HeaderLength()
{
    return (long) FileHeaderLength;
}

//
//  Method to read the shape file header
//

unsigned long XP_GIS_ESRI_ShapeHeader_c::ReadHeader()
{
    unsigned char FileHeader[FileHeaderLength];

#ifdef MSDOS
    UINT32 int32_val;
#else
    uint32_t int32_val;
#endif

    //
    //  Make sure the file is open
    //

#ifdef MSDOS
    if ((_FileStream) && (!_FileStream->is_open()))
    {
      return XP_GIS_NOT_OPEN;
    }
#endif

    //
    //  Read the header
    //

    if (!_FileStream->read(FileHeader,
                          FileHeaderLength))
    {
      if (_FileStream->eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_FileStream->gcount() != FileHeaderLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the file code
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&FileHeader[FileCodeStart]);
    }
    memcpy(&int32_val,&FileHeader[FileCodeStart],4);
    _FileCode = (long)int32_val;

    //
    //  Parse out the file length (in 16 bit units)
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::BIG_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&FileHeader[FileLengthStart]);
    }
    memcpy(&int32_val,&FileHeader[FileLengthStart],4);
    _FileLength = 2 * (long)int32_val;    // convert to bytes

    //
    //  Parse out the file version
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&FileHeader[VersionStart]);
    }
    memcpy(&int32_val,&FileHeader[VersionStart],4);
    _Version = (long)int32_val;

    //
    //  Parse ou the shape type
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(4,&FileHeader[ShapeTypeStart]);
    }
    memcpy(&int32_val,&FileHeader[ShapeTypeStart],4);
    _ShapeType = (long)int32_val;

    //
    //  Parse out the bounding box X min
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&FileHeader[XMinStart]);
    }
    memcpy(&_XMin,&FileHeader[XMinStart],8);

    //
    //  Parse out the bounding box Y min
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&FileHeader[YMinStart]);
    }
    memcpy(&_YMin,&FileHeader[YMinStart],8);

    //
    //  Parse out the bounding box X max
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&FileHeader[XMaxStart]);
    }
    memcpy(&_XMax,&FileHeader[XMaxStart],8);

    //
    //  Parse out the bounding box Y max
    //

    if (ByteUtil.ByteOrder() != GIS_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes8(8,&FileHeader[YMaxStart]);
    }
    memcpy(&_YMax,&FileHeader[YMaxStart],8);

    return XP_GIS_OK;
}

//
//  Method to validate the file header.  Returns XP_GIS_OK
//  if the header is valid.  Otherwise, it returns on of the error
//  codes listed below:
//
//    XP_GIS_BAD_MAGIC_NUMBER
//    XP_GIS_BAD_VERSION
//    XP_GIS_BAD_SHAPE_TYPE
//    XP_GIS_BAD_BAD_BOUNDING_BOX
//

unsigned long XP_GIS_ESRI_ShapeHeader_c::ValidateFileHeader()
{

    //
    //  If there is no file open, then there is no header to validate.
    //

#ifdef MSDOS
    if ((_FileStream) && (!_FileStream->is_open()))
    {
      return XP_GIS_NOT_OPEN;
    }
#endif

    //
    //  Make sure the magic number is OK 
    //

    if (_FileCode != 9994)
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }

    //
    //  Make sure the version is 1000 (the only one currently supported)
    //

    if (_Version != 1000)
    {
      return XP_GIS_BAD_VERSION;
    }

    //
    //  Make sure the shape type is valid
    //

    if ((_ShapeType != XP_GIS_NULL_SHAPE) &&
        (_ShapeType != XP_GIS_POINT_SHAPE) &&
        (_ShapeType != XP_GIS_ARC_SHAPE) &&
        (_ShapeType != XP_GIS_POLYGON_SHAPE) &&
        (_ShapeType != XP_GIS_MULTIPOINT_SHAPE))
    {
      return XP_GIS_BAD_SHAPE_TYPE;
    }

    //
    //  Make sure the bounding hull X min is smaller than the X max
    //

    if ((_XMin) >= (_XMax))
    {
      return XP_GIS_BAD_BOUNDING_BOX;
    }

    //
    //  Make sure the bounding hull Y min is smaller than the Y max
    //

    if ((_YMin) >= (_YMax))
    {
      return XP_GIS_BAD_BOUNDING_BOX;
    }

    return XP_GIS_OK;
}

//
//  Method to format and print the shape file header to and output stream.
//

unsigned long XP_GIS_ESRI_ShapeHeader_c::PrintFileHeader(ostream &PrintStream)
{

    //
    //  If there is no file open, then there is no header to print.
    //

#ifdef MSDOS
    if ((_FileStream) && (!_FileStream->is_open()))
    {
      return XP_GIS_NOT_OPEN;
    }
#endif

    PrintStream << "ShapeFileHeader";
    if (_FileName != NULL)
    {
       PrintStream << " for " << _FileName << endl;
    }
    else
    {
       PrintStream << ":" << endl;
    }
    PrintStream << "  FileCode   = " << _FileCode << endl;
    PrintStream << "  FileLength = " << _FileLength << endl;
    PrintStream << "  Version    = " << _Version << endl;
    PrintStream << "  Shape Type = " << _ShapeType << " (";
    switch (_ShapeType)
    {
      case XP_GIS_NULL_SHAPE:
        PrintStream << "null";
        break;
      case XP_GIS_POINT_SHAPE:
        PrintStream << "point";
        break;
      case XP_GIS_ARC_SHAPE:
        PrintStream << "arc";
        break;
      case XP_GIS_POLYGON_SHAPE:
        PrintStream << "polygon";
        break;
      case XP_GIS_MULTIPOINT_SHAPE:
        PrintStream << "polypoint";
        break;
      default:
        PrintStream << "invalid";
    }
    PrintStream << " shape)" << endl;
    PrintStream << "  XMin Type  = " << _XMin << endl;
    PrintStream << "  YMax Type  = " << _YMin << endl;
    PrintStream << "  XMax Type  = " << _XMax << endl;
    PrintStream << "  YMin Type  = " << _YMax << endl;

    return XP_GIS_OK;
}


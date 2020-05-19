//
//  This file contains the source code for the dBASE file header object.
//  This object provides utilities to read and manage a dBASE (dbf)
//  header record.
//

#include <stdio.h>

#ifdef MSDOS
#include <basetsd.h>
#endif

#include "dbfhdr.h"
#include "gsbyteu.h"

static DBF_ByteUtil_c ByteUtil;        // used to swap bytes

//
//  Define constant for the # of bytes in the first part of the
//  header
//

const unsigned long HeaderPart1BufferLength = 12;

//
//  Define constants for the start of header record items.
//

const unsigned long VersionStart      = 0;
const unsigned long DateStart         = 1;
const unsigned long NumberOfRowsStart = 4;
const unsigned long HeaderLengthStart = 8;
const unsigned long RowLengthStart    = 10;

//
//  Define constants for the lengths of header record items.
//

const unsigned long VersionSize      = 1;
const unsigned long DateSize         = 3;
const unsigned long NumberOfRowsSize = 4;
const unsigned long HeaderLengthSize = 2;
const unsigned long RowLengthSize    = 2;

//
//  The default constructor.  If this is used, then the FileName method
//  should be used to set the file name and the FileStream method MUST
//  be used to set the file stream.  The file must have already been
//  opened before this constructor is called.
//

XP_GIS_DBF_Header_c::XP_GIS_DBF_Header_c()
{
    _FileName = NULL;
    _FileStream = NULL;
}

//
//  The overloaded constructor.  This version sets the file name and
//  file stream.  The file must have already been opened before this
//  constructor is called.
//

XP_GIS_DBF_Header_c::XP_GIS_DBF_Header_c(const char     *DBFFileName,
                                         ifstream       &DBFFileStream)
{
    _FileName = NULL;
    _FileStream = NULL;

    FileName(DBFFileName);
    FileStream(DBFFileStream);
    ReadHeader();
}

//
//  The destructor.  When destroyed, this object does not close the file.
//

XP_GIS_DBF_Header_c::~XP_GIS_DBF_Header_c()
{
}

//
//  The copy constructor.
//

XP_GIS_DBF_Header_c::XP_GIS_DBF_Header_c(const XP_GIS_DBF_Header_c &object)
{
    *this = object;
}

//
//  The assignment operator.
//

XP_GIS_DBF_Header_c &XP_GIS_DBF_Header_c::operator=(
                                 const XP_GIS_DBF_Header_c &object)
{
    strcpy(_FileName,object.FileName());
    _FileStream   = object._FileStream;
    _Version      = object._Version;
    _HeaderLength = object._HeaderLength;
    _RowLength    = object._RowLength;
    _NumberOfRows = object._NumberOfRows;
    strcpy(_Date,object._Date);
    return *this;
}

//
//  Method to set the dBASE file name.
//

const char *XP_GIS_DBF_Header_c::FileName(const char *DBFFileName)
{
    _FileName = (char *) DBFFileName;
    return (_FileName);
}

//
//  Method to set the dBASE file stream.
//

ifstream &XP_GIS_DBF_Header_c::FileStream(ifstream &DBFFileStream)
{
    _FileStream = &DBFFileStream;
    return *_FileStream;
}
//
//  Method to read the dBASE file header.
//  If succesful, this method returns XP_GIS_OK.  Otherwise, it
//  returns one of the following:
//    XP_GIS_NOT_OPEN
//    XP_GIS_SEEK_ERROR
//    XP_GIS_EOF
//    XP_GIS_READ_ERROR
//    XP_GIS_IO_ERROR
//    XP_GIS_BAD_MAGIC_NUMBER
//

unsigned long XP_GIS_DBF_Header_c::ReadHeader()
{
    unsigned char HeaderPart1Buffer[HeaderPart1BufferLength];
    unsigned short TemporaryShort;

#ifdef MSDOS
    UINT32 TemporaryInt;
#else
    uint32_t TemporaryInt;
#endif

    //
    //  Make sure the file is open.
    //

#ifdef MSDOS
    if (!_FileStream->is_open())
    {
      return XP_GIS_NOT_OPEN;
    }
#endif

    //
    //  Seek to the start of the file.
    //

    if (!_FileStream->seekg(0,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the header.
    //

    if (!_FileStream->read((char*)HeaderPart1Buffer,HeaderPart1BufferLength))
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
    if (_FileStream->gcount() != HeaderPart1BufferLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the magic number
    //

    if (HeaderPart1Buffer[VersionStart] != 3)
    {
      return XP_GIS_BAD_MAGIC_NUMBER;
    }
    _Version = (unsigned long) HeaderPart1Buffer[VersionStart];

    //
    //  Parse out the date.
    //

    _Date[0] = '1';
    _Date[1] = '9';
    sprintf(&_Date[2],"%2.2d",(int) HeaderPart1Buffer[DateStart]);
    _Date[4]  = '/';          // separator
    sprintf(&_Date[5],"%2.2d",(int) HeaderPart1Buffer[DateStart+1]);
    _Date[7]  = '/';          // separator
    sprintf(&_Date[8],"%2.2d",(int) HeaderPart1Buffer[DateStart+2]);
    _Date[10] = '\0';         // terminator

    //
    //  Parse out the number of rows.
    //

    if (ByteUtil.ByteOrder() != DBF_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes4(NumberOfRowsSize, &HeaderPart1Buffer[NumberOfRowsStart]);
    }
    memcpy(&TemporaryInt, &HeaderPart1Buffer[NumberOfRowsStart], NumberOfRowsSize);
    _NumberOfRows = (unsigned long) TemporaryInt;

    //
    //  Parse out the number of bytes in the header
    //

    if (ByteUtil.ByteOrder() != DBF_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes2(HeaderLengthSize, &HeaderPart1Buffer[HeaderLengthStart]);
    }
    memcpy(&TemporaryShort, &HeaderPart1Buffer[HeaderLengthStart], HeaderLengthSize);
    _HeaderLength = (unsigned long) TemporaryShort;

    //
    //  Parse out the number of bytes in each row
    //

    if (ByteUtil.ByteOrder() != DBF_ByteUtil_c::LITTLE_ENDIAN)
    {
      ByteUtil.SwapBytes2(RowLengthSize, &HeaderPart1Buffer[RowLengthStart]);
    }
    memcpy(&TemporaryShort, &HeaderPart1Buffer[RowLengthStart], RowLengthSize);
    _RowLength = (unsigned long) TemporaryShort;

    return XP_GIS_OK;
}


//
//  Method to print the file header.
//

void XP_GIS_DBF_Header_c::PrintHeader(ostream &PrintStream) const
{
    PrintStream << "DBF file header for " << _FileName << endl;
    PrintStream << "  Version      = " << _Version << endl;
    PrintStream << "  Date         = " << _Date << endl;
    PrintStream << "  HeaderLength = " << _HeaderLength << endl;
    PrintStream << "  RowLength    = " << _RowLength << endl;
    PrintStream << "  NumberOfRows = " << _NumberOfRows << endl;
}


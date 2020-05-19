//
//  This file contains the source code for the dBASE field descriptor object.
//  This object provides utilities to read and manage a dBASE (.dbf)
//  field descriptor record.
//

#include "dbffldds.h"

//
//  Set up constants for where items start in record
//

static const int FieldNameStart         = 0;
static const int FieldTypeStart         = 11;
static const int FieldLengthStart       = 16;
static const int FieldDecimalCountStart = 17;

//
// Set up constants for item lengths in record
//

static const int FieldNameSize          = 11;

//
//  Set up the length of the field buffer
//

static const unsigned long FieldBufferLength = 32;


//
//  The default constructor.
//

XP_GIS_DBF_FieldDescriptor_c::XP_GIS_DBF_FieldDescriptor_c()
{
    _FieldLength       = 0;
    _FieldDecimalCount = 0;
    _FieldType         = XP_GIS_DBF_FieldDescriptor_c::CHARACTER;
    _Header            = NULL;
    _ColumnNumber      = 0;
}

//
//  The overloaded constructor.  It is recommended that this constructor
//  be used.  It sets the pointer to the header record object and the
//  column number.
//

XP_GIS_DBF_FieldDescriptor_c::XP_GIS_DBF_FieldDescriptor_c(
                                 const XP_GIS_DBF_Header_c *Header,
                                 unsigned long              ColumnNumber)
{
    _FieldLength       = 0;
    _FieldDecimalCount = 0;
    _FieldType         = XP_GIS_DBF_FieldDescriptor_c::CHARACTER;
    _Header            = (XP_GIS_DBF_Header_c*) Header;
    _ColumnNumber      = ColumnNumber;

    ReadFieldDescriptor();

}

//
//  The destructor.
//

XP_GIS_DBF_FieldDescriptor_c::~XP_GIS_DBF_FieldDescriptor_c()
{
}

//
//  The copy constructor
//

XP_GIS_DBF_FieldDescriptor_c::XP_GIS_DBF_FieldDescriptor_c(
                            const XP_GIS_DBF_FieldDescriptor_c &object)
{
    *this = object;
}

//
//  The assignment operator
//

XP_GIS_DBF_FieldDescriptor_c &XP_GIS_DBF_FieldDescriptor_c::operator=(
                            const XP_GIS_DBF_FieldDescriptor_c &object)
{
    _Header            = object._Header;
    _ColumnNumber      = object._ColumnNumber;
    _FieldLength       = object._FieldLength;
    _FieldDecimalCount = object._FieldDecimalCount;
    _FieldType         = object._FieldType;
    strcpy(_FieldName,object._FieldName);
    return *this;
}

//
//  Method to read the specified field descriptor for the specified field.
//  The field descriptor must be read before accessing any of the field values.
//
//  If successful, this method returns XP_GIS_OK.  Otherwise, it returns
//  one of the following:
//    XP_GIS_NOT_OPEN
//    XP_GIS_SEEK_ERROR
//    XP_GIS_EOF
//    XP_GIS_READ_ERROR
//    XP_GIS_IO_ERROR
//

unsigned long XP_GIS_DBF_FieldDescriptor_c::ReadFieldDescriptor()
{
    unsigned char FieldBuffer[FieldBufferLength];
    unsigned long StartPosition;

    //
    //  If the file is not open, return an error.
    //

#ifdef MSDOS
    if (!_Header->FileStream().is_open())
    {
      return XP_GIS_NOT_OPEN;
    }
#endif

    //
    //  Seek to the start of the field descriptor.
    //

    StartPosition = 32 +
                   ((_ColumnNumber-1) * FieldBufferLength);

    if (!_Header->FileStream().seekg(StartPosition,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the field descriptor.
    //

    if (!_Header->FileStream().read((char*)FieldBuffer,FieldBufferLength))
    {
      if (_Header->FileStream().eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_Header->FileStream().gcount() != (long) FieldBufferLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the field name
    //

    strncpy(_FieldName,(char *) &FieldBuffer[FieldNameStart],FieldNameSize);
    _FieldName[11] = '\0';

    //
    //  Parse out the field type
    //

    switch (FieldBuffer[FieldTypeStart])
    {
      case 'C':
        _FieldType = XP_GIS_DBF_FieldDescriptor_c::CHARACTER;
        break;
      case 'D':
        _FieldType = XP_GIS_DBF_FieldDescriptor_c::DATE;
        break;
      case 'L':
        _FieldType = XP_GIS_DBF_FieldDescriptor_c::LOGICAL;
        break;
      case 'N':
        _FieldType = XP_GIS_DBF_FieldDescriptor_c::NUMERIC;
        break;
      case 'M':
        _FieldType = XP_GIS_DBF_FieldDescriptor_c::MEMO;
        break;
      default:
        return XP_GIS_ERROR;
    }

    //
    //  parse out the field length
    //

    _FieldLength = (unsigned long) FieldBuffer[FieldLengthStart];

    //
    //  parse out the decimal count
    //

    _FieldDecimalCount = (unsigned long) FieldBuffer[FieldDecimalCountStart];
    return XP_GIS_OK;
}

//
//  Method to return the field descriptor length in bytes.
//

unsigned long XP_GIS_DBF_FieldDescriptor_c::DescriptorLength() const
{
    return FieldBufferLength;
}

//
//  Method to print the field descriptor.
//

void XP_GIS_DBF_FieldDescriptor_c::PrintDescriptor(ostream &PrintStream) const
{
    PrintStream << "Field descriptor for column " << _ColumnNumber;
    PrintStream << " of file " << _Header->FileName() << endl;
    PrintStream << "  FieldName = " << _FieldName << endl;
    PrintStream << "  FieldType = ";
    switch (_FieldType)
    {
      case CHARACTER:
        PrintStream << "CHARACTER";
        break;
      case DATE:
        PrintStream << "DATE";
        break;
      case LOGICAL:
        PrintStream << "LOGICAL";
        break;
      case NUMERIC:
        PrintStream << "NUMERIC";
        break;
      case MEMO:
        PrintStream << "MEMO";
        break;
      default:
        PrintStream << "invalid";
    }
    PrintStream << endl;
    PrintStream << "  FieldLength = " << _FieldLength << endl;
    PrintStream << "  FieldDecimalCount = " << _FieldDecimalCount << endl;
}


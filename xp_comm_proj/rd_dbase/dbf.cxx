//
//  This file contains the source code for the object that reads a dBASE
//  (.dbf) file.
//

#include <stdio.h>
#include <iostream>
#include <ios>
#include "gsbyteu.h"

#include "dbfhdr.h"
#include "dbffldds.h"
#include "dbf.h"

//  Define the lengths of the various fixed-length file items.

static const unsigned long IndexRecordLength           =   8; // in bytes

//  Define the starting byte within the file header

static const unsigned long PolygonXMinStart           = 4;
static const unsigned long PolygonYMinStart           = 12;

//  Create a static byte util object to use for byte swapping.

static DBF_ByteUtil_c ByteUtil;

//
//  Constructor.
//

XP_GIS_DBF_c::XP_GIS_DBF_c(const char *BaseFileName)
{

    //
    //  Initialize strings to empty. And the file to closed.
    //

    _FileName[0]      = '\0';
    _FileIsOpen       = XP_GIS_FALSE;

    //
    //  Initialize pointers to null.
    //

    _FieldDescriptor = NULL;
    _RowAddress      = NULL;
    _FieldAddress    = NULL;

    //
    //  If the file name is null, then simply return.
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

XP_GIS_DBF_c::~XP_GIS_DBF_c()
{
    //
    //  Close open files.
    //

    CloseFile();
}

//
//  Copy constructor.
//

XP_GIS_DBF_c::XP_GIS_DBF_c(const XP_GIS_DBF_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DBF_c &XP_GIS_DBF_c::operator=(
                      const XP_GIS_DBF_c &object)
{

    //
    //  To set up the object properly, all we have to do is set the
    //  file name via the accessor.  This will set all the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    FileName(object._FileName);

    return *this;
}

//
//  Method to set the file name.  If there is already a file opened,
//  this method closes the file before attempting to open the newly
//  specified file.  Returns NULL on failure.  Otherwise returns the
//  named of the newly opened file.
//

unsigned long XP_GIS_DBF_c::FileName(const char* FileName) 
{
    unsigned long ReturnValue;
    unsigned long ColumnNumber;
    unsigned long RowAddress;
    unsigned char FlagByte;

    //
    //  If a null is passed in, then close any open files.
    //
    if (FileName == NULL)
    {
      CloseFile();
      return XP_GIS_OK;
    }

    //
    //  Copy the file name to the private variable.  If necessary,
    //  append a .dbf suffix.
    //

    strcpy(_FileName,FileName);
    if (strcmp(&_FileName[strlen(FileName)-4],".dbf") != 0)
    {
      strcat(_FileName,".dbf");
    }

    //
    //  If there is already a file open, close it.
    //

    if (FileIsOpen() == XP_GIS_TRUE)
    {
      CloseFile();
    }

    //
    //  Otherwise, attempt to open the requested file.  If not successful,
    //  close the file name.  Otherwise, populate the file name and
    //  read the file header.
    //


	fstream fs(_FileName, ios::in);// attempt open for read
	if (fs)
	{
		fs.close();
#ifdef MSDOS
	    _FileStream.open(_FileName,ios::binary);
#else
	    _FileStream.open(_FileName);
#endif
	}
    if (_FileStream.bad())
    {
      _FileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }

    //
    //  Populate the header object with the file name and stream.
    //  Then read the header.
    //

    _Header.FileName(_FileName);
    _Header.FileStream(_FileStream);
    if ((ReturnValue = _Header.ReadHeader()) != XP_GIS_OK)
    {
      CloseFile();
      return ReturnValue;     
    }

    //
    //  Allocate the array of field descriptor object pointers.
    //

    _FieldDescriptor = (XP_GIS_DBF_FieldDescriptor_c **) malloc
         (sizeof (XP_GIS_DBF_FieldDescriptor_c*) * 
          _Header.NumberOfColumns());
    if (_FieldDescriptor == NULL)
    {
      CloseFile();
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Create the field descriptor objects and populate them.
    //

    for (ColumnNumber = 0; 
         ColumnNumber < _Header.NumberOfColumns();
         ColumnNumber++)
    {
      _FieldDescriptor[ColumnNumber] = new XP_GIS_DBF_FieldDescriptor_c(
                                            &_Header,
                                             ColumnNumber+1);
      if (_FieldDescriptor[ColumnNumber] == NULL)
      {
        CloseFile();
        return XP_GIS_ALLOC_ERROR;
      }
    }
    _NumberOfColumns = _Header.NumberOfColumns();

    //
    //  Now, build a list with one entry for each record.  The entry
    //  tells the address of each (not-yet-deleted) row.  Deletion is
    //  indicated by a byte in the file before the record.  The byte
    //  will be a space if the record has not been deleted, and an
    //  asterix if the record has been deleted.
    //

    _RowAddress= (unsigned long *) malloc(sizeof(unsigned long) * 
                                          _Header.NumberOfRows());
    if (_RowAddress == NULL)
    {
      CloseFile();
      return XP_GIS_ALLOC_ERROR;
    }
    _NumberOfRowsPresent = 0;
    for (unsigned long RowNumber = 1;
                       RowNumber <= _Header.NumberOfRows();
                       RowNumber++)
    {
      //
      //  Seek to the flag byte
      //

      RowAddress = _Header.HeaderLength() +
                   ((_Header.RowLength()) * (RowNumber - 1));
      if (!_FileStream.seekg(RowAddress,ios::beg))
      {
        CloseFile();
        return XP_GIS_SEEK_ERROR;
      }

      //
      //  The file now is open
      //
      _FileIsOpen = XP_GIS_TRUE;

      //
      //  Read the flag byte
      //

      if (!_FileStream.read((char*)&FlagByte,1))
      {
        if (_FileStream.eof())
        {
          return XP_GIS_EOF;
        }
        else
        {
          return XP_GIS_READ_ERROR;
        }
      }
      if (_FileStream.gcount() != 1)
      {
        return XP_GIS_IO_ERROR;
      }

      //
      //  Populate the flag array.  Assume the row has not been
      //  deleted unless the delete character (*) is present.
      //

      if (FlagByte != '*')
      {
        _RowAddress[_NumberOfRowsPresent++] = RowAddress;
      }
    }

    _FieldAddress = (unsigned long *) malloc(sizeof(unsigned long) * 
                                             _Header.NumberOfColumns());
    if (_FieldAddress == NULL)
    {
      CloseFile();
      return XP_GIS_ALLOC_ERROR;
    }
    _FieldAddress[0] = 1;
    for (ColumnNumber = 1;
         ColumnNumber < _Header.NumberOfColumns();
         ColumnNumber++)
    {
      _FieldAddress[ColumnNumber] = _FieldAddress[ColumnNumber-1] +
                       _FieldDescriptor[ColumnNumber]->FieldLength();
    }

    return XP_GIS_OK;
}

//
//  Method to close the file
//

void XP_GIS_DBF_c::CloseFile()
{
    //
    //  If the file is open, close the file and clear the name.
    //

    if (FileIsOpen() == XP_GIS_TRUE)
    {
      _FileStream.close();
      _FileName[0] = '\0';
      _FileIsOpen  = XP_GIS_FALSE;

      //
      //  We may have populated filed descriptor information.  If so,
      //  be sure the delete/free the appropriate stuff.
      //

      if (_FieldDescriptor != NULL)
      {
        for (unsigned long ColumnNumber = 0;
                           ColumnNumber < _Header.NumberOfColumns();
                           ColumnNumber++)
        {
          if (_FieldDescriptor[ColumnNumber] != NULL)
          {
            delete(_FieldDescriptor[ColumnNumber]);
          }
        }
        free(_FieldDescriptor);
      }

      //
      //  We may have allocated the row address array.  If so, free it.
      //

      if (_RowAddress != NULL)
      {
        free(_RowAddress);
      }

      //
      //  We may have allocated the field address array.  If so, free it.
      //

      if (_FieldAddress != NULL)
      {
        free(_FieldAddress);
      }
    }

    return;
}

//
//  Method to read a character string from the table
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//

unsigned long XP_GIS_DBF_c::ReadString(unsigned long RowNumber,
                                       unsigned long ColumnNumber,
                                       unsigned long StringLength,
                                       char         *String)
{
    unsigned long ReturnValue;

    //
    //  Make sure the string is big enough.
    //

    if (StringLength < _FieldDescriptor[ColumnNumber-1]->FieldLength() + 1)
    {
      return XP_GIS_BAD_VALUE;
    }

    if ((ReturnValue = LowLevelReadValue(RowNumber,
                                         ColumnNumber,
                                         StringLength,
                                         String)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Concatenate a string terminator.
    //

    String[_FieldDescriptor[ColumnNumber-1]->FieldLength()] = '\0';

    return XP_GIS_OK;
}

//
//  Method to read a date from the table
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//

unsigned long XP_GIS_DBF_c::ReadDate(unsigned long RowNumber,
                                     unsigned long ColumnNumber,
                                     unsigned long DateLength,
                                     char         *Date)
{
    unsigned long ReturnValue;

    //
    //  Make sure the string is big enough.
    //

    if ((DateLength < _FieldDescriptor[ColumnNumber-1]->FieldLength() + 3) ||
        (DateLength < 11))
    {
      return XP_GIS_BAD_VALUE;
    }

    if ((ReturnValue = LowLevelReadValue(RowNumber,
                                         ColumnNumber,
                                         DateLength,
                                         Date)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Insert the field separators (/ for now) and terminator.
    //  Note that we don't need to move the month field (first 4 bytes).
    //

    Date[10] = '\0';     // terminator
    Date[9]  = Date[7];  // day
    Date[8]  = Date[6];  // day
    Date[7]  = '/';      // separator
    Date[6]  = Date[5];  // month
    Date[5]  = Date[4];  // month
    Date[4]  = '/';      // separator

    return XP_GIS_OK;
}

//
//  Method to read an integer numeric value from the table. An integer
//  has a field type of numeric and a decimal count equal to 0.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//  XP_GIS_ALLOC_ERROR
//

unsigned long XP_GIS_DBF_c::ReadInteger(unsigned long RowNumber,
                                        unsigned long ColumnNumber,
                                        long         *Integer)
{
    unsigned long  ReturnValue;
    const unsigned long  BufferLength = 257;
    char                 Buffer[BufferLength];

    if ((ReturnValue = LowLevelReadValue(RowNumber,
                                         ColumnNumber,
                                         BufferLength,
                                         Buffer)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Concatenate a string terminator.
    //

    Buffer[_FieldDescriptor[ColumnNumber-1]->FieldLength()] = '\0';

    //
    //  Parse out the value
    //

    sscanf(Buffer,"%d",Integer);

    return XP_GIS_OK;
}

//
//  Method to read a float numeric value from the table.  A float
//  has a field type of numeric and a decimal count greater than 0.
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//  XP_GIS_ALLOC_ERROR
//

unsigned long XP_GIS_DBF_c::ReadFloat(unsigned long RowNumber,
                                      unsigned long ColumnNumber,
                                      float         *Float)
{
    unsigned long  ReturnValue;
    const unsigned long  BufferLength = 257;
    char                 Buffer[BufferLength];

    if ((ReturnValue = LowLevelReadValue(RowNumber,
                                         ColumnNumber,
                                         BufferLength,
                                         Buffer)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Concatenate a string terminator.
    //

    Buffer[_FieldDescriptor[ColumnNumber-1]->FieldLength()] = '\0';

    //
    //  Parse out the value
    //

    sscanf(Buffer,"%f",Float);

    return XP_GIS_OK;
}

//
//  Method to read a logical value from the table
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//

unsigned long XP_GIS_DBF_c::ReadLogical(unsigned long RowNumber,
                                        unsigned long ColumnNumber,
                                        char         *Character)
{
    unsigned long ReturnValue;

    if ((ReturnValue = LowLevelReadValue(RowNumber,
                                         ColumnNumber,
                                         1,
                                         Character)) != XP_GIS_OK)
    {
      return ReturnValue;
    }

    //
    //  Concatenate a string terminator.
    //

    Character[1] = '\0';

    return XP_GIS_OK;
}

//
//  Method to read a value from the table
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_EOF
//  XP_GIS_READ_ERROR
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//  XP_GIS_BAD_VALUE
//

unsigned long XP_GIS_DBF_c::LowLevelReadValue(unsigned long  RowNumber,
                                              unsigned long  ColumnNumber,
                                              unsigned long  BufferLength,
                                              char          *Buffer)
{
    unsigned long StartPosition;
    unsigned long k1;

    //
    //  Make sure the file is open
    //

    if (FileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Validate the row number.
    //

    if (RowNumber > _Header.NumberOfRows())
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Validate the column number.
    //

    if (ColumnNumber > _Header.NumberOfColumns())
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Make sure the buffer is big enough.
    //

    if (BufferLength < _FieldDescriptor[ColumnNumber-1]->FieldLength())
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Figure out where to seek to.  First, skip to the start
    //  of the desired row.  Then, skip over the flag byte at the
    //  start of the row.  Finally, skip to the start of the
    //  desired column.
    //  

    StartPosition = _RowAddress[RowNumber-1] + 1;
    for (k1 = 0;
         k1 < ColumnNumber-1;
         k1++)
    {
      StartPosition += _FieldDescriptor[k1]->FieldLength();
    }

    //
    //  Seek to the start position.
    //

    if (!_FileStream.seekg(StartPosition,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the value
    //

    if (!_FileStream.read(Buffer,
                          _FieldDescriptor[ColumnNumber-1]->FieldLength()))
    {
      if (_FileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_FileStream.gcount() != 
        (long) _FieldDescriptor[ColumnNumber-1]->FieldLength())
    {
      return XP_GIS_IO_ERROR;
    }

    return XP_GIS_OK;
}

//
//  Method to convert a column name to a column number.  Column numbers
//  range from 1 to n, where n is the number of columns in the file.
//  If successful, this method returns a value in the rante of 1 to n.
//  Othewise, it returns 0.
//

unsigned long XP_GIS_DBF_c::ColumnNameToNumber(const char *ColumnName)
{
    unsigned long ColumnNumber;

    //
    //  Make sure the file is open
    //
    if (FileIsOpen() == XP_GIS_FALSE)
    {
      return 0;
    }

    //
    //  Search the field descriptors for a matching name.  If we don't find
    //  a match, return 0.
    //

    for (ColumnNumber = 0; ColumnNumber < _NumberOfColumns; ColumnNumber++)
    {
      if (strcmp(_FieldDescriptor[ColumnNumber]->FieldName(),ColumnName) == 0)
      {
        return ColumnNumber + 1;
      }
    }
    return 0;
}

//
//  Method to convert a column number to a column name.  Column numbers
//  range from 1 to n, where n is the number of columns in the file.
//  If successful, this method returns the column name.  Otherwise, 
//  it returns NULL.
//

const char *XP_GIS_DBF_c::ColumnNumberToName(
                            unsigned long  ColumnNumber) const
{
    //
    //  Make sure the file is open
    //

    if (FileIsOpen() == XP_GIS_FALSE)
    {
      return NULL;
    }

    return _FieldDescriptor[ColumnNumber]->FieldName();
}

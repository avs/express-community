//
//  Include file for the XP_GIS_DBF_c class, which provides utilities
//  to read a dBASE (dbf) file.
//
//  To use this object, an instance must be created and a file name for
//  an existing dBASE file provided.  Currently, only version 3 files,
//  and only those files without memos, are supported.
//
//  The file name can be provided in the constructor or set with the
//  FileName method.  Once the file name has been specified, any of the
//  other methods can be used.
//

/*

    Public Constructors

      XP_GIS_DBF_c(const char *FileName=NULL);
        The constructor.

      virtual ~XP_GIS_DBF_c();
        the destructor.

      XP_GIS_DBF_c(const XP_GIS_DBF_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DBF_c &operator=(const XP_GIS_DBF_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long FileName(const char* FileName);
        Method to set the file name.  If there is already a file opened,
        this method closes the file before attempting to open the newly
        specified file.  Returns NULL on failure.  Otherwise returns the
        named of the newly opened file.

      const char *FileName() const;
        Method to return a pointer to the currently set file name.

      unsigned char FileIsOpen() const {return _FileIsOpen;}
        Method to get the flag that tells whether or not the dbf file is
        open.  Return XP_GIS_TRUE if the file is open.  Otherwise, it
        returns XP_GIS_FALSE.

      XP_GIS_DBF_Header_c &Header();
        Method to get the file header.

      XP_GIS_DBF_FieldDescriptor_c
                    &FieldDescriptor(unsigned long FieldNumber);
        Method to get the field descriptor given the field number.

      unsigned long NumberOfRows(unsigned long *NumberOfRows);
        Method to get the number of rows in the file.  This is the total 
        number of rows physically present in the file, including those
        flagged as deleted.

      unsigned long NumberOfColumns(unsigned long *NumberOfColumns);
        Method to get the number of columns in the file.

      unsigned long ColumnNameToNumber(char *ColumnName);
        Method to convert a column name to a column number.  Column numbers
        range from 1 to n, where n is the number of columns in the file.
        If successful, this method returns a value in the range of 1 to n.
        Otherwise, it returns 0.

      const char *ColumnNumberToName(unsigned long *ColumnNumber) const;
        Method to convert a column number to a column name.  Column numbers
        range from 1 to n, where n is the number of columns in the file.
        If successful, this method returns the column name.  Otherwise,
        it returns NULL.

      unsigned long ReadString(unsigned long RowNumber,
                               unsigned long ColumnNumber,
                               unsigned long StringLength,
                               char         *String);
        Method to read a string value given a row number and column number.
        String must be preallocated to a size sufficient to hold the
        string (with a \0 terminator).  StringLength is the size String
        has been preallocated with.

        If successful, this method return XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_VALUE

      unsigned long ReadDate(unsigned long RowNumber,
                             unsigned long ColumnNumber,
                             unsigned long DateLength,
                             char         *Date);
        Method to read a date value given a row number and column number.
        Date must be preallocated to a size sufficient to hold the
        date (with a \0 terminator).  DateLength is the size Date
        has been preallocated with.  Dates will never be greater than
        11 bytes long.

        If successful, this method return XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_VALUE

      unsigned long ReadInteger(unsigned long RowNumber,
                                unsigned long ColumnNumber,
                                long         *Integer);
        Method to read an integer value given a row number and column number.

        If successful, this method return XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_VALUE
          XP_GIS_ALLOC_ERROR

      unsigned long ReadFloat(unsigned long RowNumber,
                              unsigned long ColumnNumber,
                              float        *Integer);
        Method to read a float value given a row number and column number.

        If successful, this method return XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_VALUE
          XP_GIS_ALLOC_ERROR

      unsigned long ReadLogical(unsigned long RowNumber,
                                unsigned long ColumnNumber,
                                char         *Logical);
        Method to read a logical value given a row number and column number.

        If successful, this method return XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_VALUE

      unsigned long NumberOfRowsPresent() const;
        Method to geet the number of rows present in the file.  This is
        only the number of rows flagged as present.
*/



#ifndef XP_GIS_DBF_H
#define XP_GIS_DBF_H

#include <fstream>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "gsconst.h"
#include "dbfhdr.h"
#include "dbffldds.h"

class XP_GIS_DBF_c
{
  public:
    XP_GIS_DBF_c(const char *FileName=NULL);
    virtual ~XP_GIS_DBF_c();
    XP_GIS_DBF_c(const XP_GIS_DBF_c &object);
    XP_GIS_DBF_c &operator=(const XP_GIS_DBF_c &object);

    unsigned long FileName(const char* FileName);
    const char *FileName() const {return _FileName;} 

    unsigned char FileIsOpen() const {return _FileIsOpen;}

    XP_GIS_DBF_Header_c &Header() {return _Header;}
    XP_GIS_DBF_FieldDescriptor_c
                  &FieldDescriptor(unsigned long FieldNumber)
                            {return *_FieldDescriptor[FieldNumber-1];}

    unsigned long NumberOfRows(unsigned long *NumberOfRows);
    unsigned long NumberOfColumns(unsigned long *NumberOfColumns);
    unsigned long ColumnNameToNumber(const char *ColumnName);
    const char *ColumnNumberToName(unsigned long ColumnNumber) const;
    unsigned long ReadString(unsigned long RowNumber,
                             unsigned long ColumnNumber,
                             unsigned long StringLength,
                             char         *String);
    unsigned long ReadDate(unsigned long RowNumber,
                           unsigned long ColumnNumber,
                           unsigned long DateLength,
                           char         *Date);
    unsigned long ReadInteger(unsigned long RowNumber,
                              unsigned long ColumnNumber,
                              long         *Integer);
    unsigned long ReadFloat(unsigned long RowNumber,
                            unsigned long ColumnNumber,
                            float        *Integer);
    unsigned long ReadLogical(unsigned long RowNumber,
                              unsigned long ColumnNumber,
                              char         *Logical);
    unsigned long NumberOfRowsPresent() const {return _NumberOfRowsPresent;}

  private:

    unsigned long LowLevelReadValue(unsigned long  RowNumber,
                                    unsigned long  ColumnNumber,
                                    unsigned long  BufferLength,
                                    char          *Buffer);

    void CloseFile();
    ifstream      _FileStream;
    unsigned char _FileIsOpen;

    XP_GIS_DBF_Header_c _Header;        // file header
    XP_GIS_DBF_FieldDescriptor_c **_FieldDescriptor;   // field descriptor
    char          _FileName[_MAX_PATH]; // fully qualified file path
    unsigned long _NumberOfColumns;     // # of columns (fields) in table
    unsigned long *_RowAddress;         // address to start of each row
    unsigned long *_FieldAddress;       // rel address to start of each field
    unsigned long  _NumberOfRowsPresent;// # of undeleted rows.
};

#endif   // XP_GIS_DBF_H


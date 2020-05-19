//
//  Include file for the XP_GIS_DBF_FieldDescriptorr_c class, which provides
//  utilities to read and manage a dBASE (dbf) field descriptor record.
//
//  To use this object, a dBASE (.dbf) file must be opened.  Then, a
//  XP_GIS_DBF_Header_c object must be created for that file and the
//  header must be read.  Then, an instance of this object can be
//  created.
//  
//  This object can be created with the default constructor.  In this case,
//  The header and column number MUST be supplied using the accessor methods.
//  Alternatively, the overloaded constructor can be used.
//
//  Next, the ReadFieldDescritor method must be invoked to read the
//  descriptor.  After that, the accessors can be called.
//
//

/*

    Public Constructors

      XP_GIS_DBF_FieldDescriptor_c();
        The default constructor.

      XP_GIS_DBF_FieldDescriptor_c(const XP_GIS_DBF_Header_c *Header,
                                   unsigned long              ColumnNumber);
        Overloaded constructor to provide the header and set the column
        number.

      virtual ~XP_GIS_DBF_FieldDescriptor_c();
        The destructor.

      XP_GIS_DBF_FieldDescriptor_c(const XP_GIS_DBF_FieldDescriptor_c &object);
        The copy constructor

    Public Operators

      XP_GIS_DBF_FieldDescriptor_c &operator=(
                                   const XP_GIS_DBF_FieldDescriptor_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long ReadFieldDescriptor();
        Method to read the field descriptor for the specified field.
        The field descriptor must be read before accessing any of the
        field values.

        If successful, this method returns XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_SEEK_ERROR
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR

      const XP_GIS_DBF_Header_c *Header() const {return _Header;}
        Method to set the dBASE file header.  The header must be set
        before the field descriptor can be read.

      unsigned long        ColumnNumber() const {return _ColumnNumber;}
        Method to get the column number from the dBASE field descriptor.

      const char          *FieldName() const {return _FieldName;}
        Method to get the field name from the dBASE field descriptor.

      unsigned long        FieldLength() const {return _FieldLength;}
        Method to get the field length from the dBASE field descriptor.

      unsigned long        FieldDecimalCount() const {return _FieldDecimalCount;}
        Method to get the field decimal count from the dBASE field descriptor.

      XP_GIS_DBF_FieldType FieldType() const {return _FieldType;}
        Method to get the field type from the dBASE field descriptor.

      unsigned long        DescriptorLength() const;
        Method to get the field descriptor length.

      void                 PrintDescriptor(ostream &PrintStream) const;
        Method to print the field descriptor.

*/

#ifndef XP_GIS_DBF_FieldDescriptor_H
#define XP_GIS_DBF_FieldDescriptor_H

#include <fstream>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "dbfhdr.h"

class XP_GIS_DBF_FieldDescriptor_c
{
  public:

    enum XP_GIS_DBF_FieldType {CHARACTER,
                               DATE,
                               LOGICAL,
                               NUMERIC,
                               MEMO};

    XP_GIS_DBF_FieldDescriptor_c();
    XP_GIS_DBF_FieldDescriptor_c(const XP_GIS_DBF_Header_c *Header,
                                 unsigned long              ColumnNumber);
    virtual ~XP_GIS_DBF_FieldDescriptor_c();
    XP_GIS_DBF_FieldDescriptor_c(const XP_GIS_DBF_FieldDescriptor_c &object);
    XP_GIS_DBF_FieldDescriptor_c &operator=(
                                 const XP_GIS_DBF_FieldDescriptor_c &object);

    unsigned long ReadFieldDescriptor();
    const XP_GIS_DBF_Header_c *Header() const {return _Header;}
    unsigned long        ColumnNumber() const {return _ColumnNumber;}
    const char          *FieldName() const {return _FieldName;}
    unsigned long        FieldLength() const {return _FieldLength;}
    unsigned long        FieldDecimalCount() const {return _FieldDecimalCount;}
    XP_GIS_DBF_FieldType FieldType() const {return _FieldType;}
    unsigned long        DescriptorLength() const;
    void                 PrintDescriptor(ostream &PrintStream) const;

  private:

    XP_GIS_DBF_Header_c *_Header;
    unsigned long _ColumnNumber;

    char                 _FieldName[12];
    unsigned long        _FieldLength;
    unsigned long        _FieldDecimalCount;
    XP_GIS_DBF_FieldType _FieldType;
};

#endif   // XP_GIS_DBF_FieldDescriptor_H


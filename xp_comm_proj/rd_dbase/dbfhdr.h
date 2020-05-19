//
//  Include file for the XP_GIS_DBF_Header_c class, which provides
//  utilities to read and manage a dBASE (dbf) header record.
//
//  To use this object, a dBASE (.dbf) file must be opened.  The name
//  and file stream for that file should be passed to the constructor
//  of this object (although, optionally, the default constructor can
//  be used along with the FileName and FileStream methods).  Once
//  this is done, the ReadHeader method should be invoked.  After than,
//  the various accessors can be used to get the values form the header.
//

/*

    Public Constructors

      XP_GIS_DBF_Header_c();
        The default constructor.

      XP_GIS_DBF_Header_c(const char     *DBFFileName,
                          ifstream       &DBFFileStream);
        Overloaded constructor.  DBFFileName is the fully quealified name
        of a dBASE file.  DBFFileStream is the stream object for that file.

      virtual ~XP_GIS_DBF_Header_c();
        The destructor.

      XP_GIS_DBF_Header_c(const XP_GIS_DBF_Header_c &object);
        The copy constructor


    Public Operators
      XP_GIS_DBF_Header_c &operator=(const XP_GIS_DBF_Header_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      const char *FileName(const char *DBFFileName);
        Method to set the file name.  This method just sets a private
        character pointer to the pointer passed in.  This object does
        not use the file name, but it is sometimes useful to be able
        to get the file name from this object.  Therefore, it can
        (optionally) be set using this method (or via the overloaded 
        constructor).  This method returns a pointer to the file name
        passed in.

      const char *FileName() const;
        Method to get the file name.

      ifstream &FileStream(ifstream &DBFFileStream);
        Method to set the file stream for the dBASE file.  This method
        just sets a private stream pointer to the address of the stream
        object referenced passed in.  This method returns a reference
        to the file stream passed in.
 
      ifstream &FileStream();
        Method to get the file stream pointer for the dBASE file.

      unsigned long Version() const;
        Method to get the version of the header of the dBASE file.

      const char *  Date() const;
        Method to get the date from the header of the dBASE file.

      unsigned long HeaderLength() const;
        Method to get the header length in bytes from the header of 
        the dBASE file.

      unsigned long RowLength() const;
        Method to get the row length in bytes from the header of
        the dBASE file.

      unsigned long NumberOfRows() const;
        Method to get the number of rows from the header of the dBASE file.

      unsigned long NumberOfColumns() const;
        Method to get the number of columns from the header of the
        dBASE file.

      unsigned long ReadHeader();
        Method to read the header from the dBASE file.
        If successful, this method returns XP_GIS_OK.  Otherwise, it
        returns one of the following:
          XP_GIS_NOT_OPEN
          XP_GIS_SEEK_ERROR
          XP_GIS_EOF
          XP_GIS_READ_ERROR
          XP_GIS_IO_ERROR
          XP_GIS_BAD_MAGIC_NUMBER

      void PrintHeader(ostream &PrintStream) const;
        Method to print the header read from the dBASE file.
*/


#ifndef XP_GIS_DBF_HEADER_H
#define XP_GIS_DBF_HEADER_H

#include <fstream>
#include <ios>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;


#include "gserror.h"

class XP_GIS_DBF_Header_c
{
  public:

    XP_GIS_DBF_Header_c();
    XP_GIS_DBF_Header_c(const char     *DBFFileName,
                        ifstream       &DBFFileStream);
    virtual ~XP_GIS_DBF_Header_c();
    XP_GIS_DBF_Header_c(const XP_GIS_DBF_Header_c &object);
    XP_GIS_DBF_Header_c &operator=(const XP_GIS_DBF_Header_c &object);

    const char *FileName(const char *DBFFileName);
    const char *FileName() const {return _FileName;}

    ifstream &FileStream(ifstream &DBFFileStream);
    ifstream &FileStream() {return *_FileStream;}

    unsigned long Version() const {return _Version;}
    const char *  Date() const {return _Date;}
    unsigned long HeaderLength() const {return _HeaderLength;}
    unsigned long RowLength() const {return _RowLength;}
    unsigned long NumberOfRows() const {return _NumberOfRows;}
    unsigned long NumberOfColumns() const {return (_HeaderLength / 32) - 1;}

    unsigned long ReadHeader();
    void PrintHeader(ostream &PrintStream) const;

  private:

    char         *_FileName;
    ifstream     *_FileStream;
    unsigned long _Version;
    char          _Date[11];
    unsigned long _HeaderLength;
    unsigned long _RowLength;
    unsigned long _NumberOfRows;

};

#endif   // XP_GIS_DBF_HEADER_H


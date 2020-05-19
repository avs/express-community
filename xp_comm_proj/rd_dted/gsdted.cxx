//
//  This file contains the source code for the XP_GIS_DTED_c object.
//  This object reads data from a DTED file.
//

#include <stdio.h>

#include "gsdted.h"



//
//  Constructor.
//

XP_GIS_DTED_c::XP_GIS_DTED_c(const char *LocalFileName)
{

    //
    //  Initialize strings to empty.
    //

    _FileName[0] = '\0';

    //
    //  Initialize file state to closed.
    //

    _FileIsOpen = XP_GIS_FALSE;

    //
    //  If the file name is not null, then use it to
    //  set the object's file name.
    //

    if (LocalFileName != NULL)
    {
      FileName(LocalFileName);
    }

}

//
//  destructor.
//

XP_GIS_DTED_c::~XP_GIS_DTED_c()
{
    //
    //  Close open file.
    //

    _CloseFile();
}

//
//  Copy constructor.
//

XP_GIS_DTED_c::XP_GIS_DTED_c(const XP_GIS_DTED_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_DTED_c &XP_GIS_DTED_c::operator=(
                      const XP_GIS_DTED_c &object)
{

    //
    //  To set up the object properly, we have to set the
    //  file name via the accessor.  This will set the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    _FileIsOpen = XP_GIS_FALSE;
    FileName(object._FileName);

    return *this;
}

//
//  Method to set the file name.  If there is already a 
//  file opened, this method closes the file before attempting
//  to open the newly specified file.  Returns XP_GIS_OK if successful.
//  Otherwise, returns one of the following:
//
//    XP_GIS_OPEN_ERROR;
//

unsigned long XP_GIS_DTED_c::FileName(const char* LocalFileName) 
{
    unsigned long ReturnValue;


    //
    //  If a null is passed in, then close any open files.
    //

    if (LocalFileName == NULL)
    {
      _CloseFile();
      return XP_GIS_OK;
    }

    //
    //  If there is already a file open, close it.
    //

    if (_FileName[0] != '\0')
    {
      _CloseFile();
    }

    //
    //  Copy the file name that was passed in to the private variable.
    //  If does not have a suffix of .dt1, assume it is a base name, and
    //  add the suffix.
    //

    strcpy(_FileName,LocalFileName);
    if ((strcmp(&_FileName[strlen(_FileName)-4],".dt0") != 0) &&
        (strcmp(&_FileName[strlen(_FileName)-4],".dt1") != 0) &&
        (strcmp(&_FileName[strlen(_FileName)-4],".dt2") != 0)) 
    {
      strcat(_FileName,".dt1");
    }

#ifdef DEBUG
    printf("FileDTED:   Used Filename = %s\n", _FileName);
    fflush(stdout);
#endif

    //
    //  Now, attempt to open the requested file.  If not successful,
    //  clear the file name.  Otherwise, populate the file name and
    //  read the file header.
    //

#ifdef MSDOS
    _FileStream.open(_FileName,ios::binary | ios::nocreate);
#else
    _FileStream.open(_FileName,ios::nocreate);
#endif
    if (_FileStream == 0)
    {
      #ifdef DEBUG
          printf("FileDTED:   Error: Null Filestream\n");
          fflush(stdout);
      #endif

      _FileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }
    if (_FileStream.bad())
    {
      #ifdef DEBUG
          printf("FileDTED:   Error: Bad Filestream\n");
          fflush(stdout);
      #endif

      _FileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }
    else
    {
      #ifdef DEBUG
          printf("FileDTED:   Got Filestream\n");
          printf("FileDTED:   Reading UHL Data\n");
          fflush(stdout);
      #endif

      _FileIsOpen = XP_GIS_TRUE;
      if ((ReturnValue = _DTEDUHLData.Read(&_FileStream))!= XP_GIS_OK)
      {
        _CloseFile();
        return ReturnValue;     
      }
      #ifdef DEBUG
          printf("FileDTED:   Reading DSI Data\n");
          fflush(stdout);
      #endif
      if ((ReturnValue = _DTEDDSIData.Read(&_FileStream))!= XP_GIS_OK)
      {
        _CloseFile();
        return ReturnValue;     
      }
      #ifdef DEBUG
          printf("FileDTED:   Reading ACC Data\n");
          fflush(stdout);
      #endif
      if ((ReturnValue = _DTEDACCData.Read(&_FileStream))!= XP_GIS_OK)
      {
        _CloseFile();
        return ReturnValue;     
      }
      #ifdef DEBUG
          printf("FileDTED:   Finished Reading Data\n");
          fflush(stdout);
      #endif

      //
      //  We must set up the file name and number of points in the 252 object.
      //

      _DTED252Data.NumberOfPoints(_DTEDDSIData.NumberOfLatitudeLines());
      _DTED252Data.FileName(_FileName);

    }

    return XP_GIS_OK;
}

//
//  Private method to close the DTED file.
//

void XP_GIS_DTED_c::_CloseFile()
{

    //
    //  Close the file.
    //

    if (_FileIsOpen == XP_GIS_TRUE)
    {
      _FileIsOpen = XP_GIS_FALSE;
      _FileStream.close();
      _FileName[0] = '\0';
    }

    return;
}

//
//  Method to read the elevation data.  This version reads the entire
//  cell with no scaling.
//
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_DTED_c::ReadData(short *Buffer)
{
    unsigned long  RowNumber;
    unsigned long  ColumnNumber;
    const short   *DataArray;

    //
    //  Validate the inputs
    //

    if (Buffer == NULL)
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Make sure the file is open
    //

    if (_FileIsOpen == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Now read the data.  The data is column major.  We read each column
    //  and copy the column into the ouput buffer so it is row major.
    //  Also, the data in each columns goes from South to North.  We
    //  need to flip it so it goes from North to South.
    //

    for (ColumnNumber = 0; ColumnNumber < NumberOfColumns(); ColumnNumber++)
    {
      _DTED252Data.Read(ColumnNumber,&_FileStream);
      DataArray = _DTED252Data.DataArray();
      for (RowNumber = 0; RowNumber < NumberOfRows(); RowNumber++)
      {
        Buffer[(NumberOfColumns() * RowNumber) + ColumnNumber] = 
                    DataArray[(NumberOfRows() - RowNumber) - 1];
#ifdef DEBUG
        if ((DataArray[RowNumber] < 0) ||
            (DataArray[RowNumber] > 6000))
        {
          cout << "Invalid value for R,C " << RowNumber;
          cout << " " << ColumnNumber << " = " << DataArray[RowNumber];
          cout << endl;
        }
#endif
      }
    }
      
    return XP_GIS_OK;
}

//
//  Method to read the elevation data.  This version reads a chip out
//  of a single cell with an X and Y scale factor.
//
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_DTED_c::ReadData(unsigned long RowStart,
                                      unsigned long ColumnStart,
                                      unsigned long RowsInBuffer,
                                      unsigned long ColumnsInBuffer,
                                      float    XScaleFactor,
                                      float    YScaleFactor,
                                      short *Buffer)
{
    long            RowNumber;
    unsigned long   ColumnNumber;
    unsigned long   ColumnToRead;
    unsigned short *RowToUse;
    unsigned short *RowToUsePtr;
    unsigned long   ReturnValue;
    const short    *DataArray;
    float           XPosition;
    float           YPosition;

    //
    //  Validate the inputs
    //

    if ((XScaleFactor <= 0.0) ||
        (YScaleFactor <= 0.0))
    {
      return XP_GIS_BAD_VALUE;
    }

#ifdef DEBUG
    printf("FileDTED:   Rows Wanted = %f\n", (RowStart + (RowsInBuffer / YScaleFactor) - 1) );
    printf("FileDTED:   Rows Available = %d\n", NumberOfRows() );
    printf("FileDTED:   Columns Wanted = %f\n", (ColumnStart + (ColumnsInBuffer / XScaleFactor) - 1) );
    printf("FileDTED:   Columns Available = %d\n", NumberOfColumns() );
    fflush(stdout);
#endif

    if ((RowStart    + (RowsInBuffer    / YScaleFactor) - 1 > NumberOfRows()) ||
        (ColumnStart + (ColumnsInBuffer / XScaleFactor) - 1 > NumberOfColumns()))
    {
      return XP_GIS_BAD_VALUE;
    }

    //
    //  Make sure the file is open
    //

    if (_FileIsOpen == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Allocate space for array telling where we start each row.
    //

    RowToUse = (unsigned short*) malloc(sizeof(unsigned short) * 
                                        RowsInBuffer);
    if (RowToUse == NULL)
    {
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Build an array of where we start for each row.
    //  The data runs from South to North.  We need to flip
    //  the data so it runs North to South.  We do this by
    //  building this array in inverse order.
    //

    YPosition = (float) RowStart - 1;
    for (RowNumber = RowsInBuffer - 1; RowNumber >= 0; RowNumber--)
    {
      RowToUse[RowNumber] = (unsigned short) YPosition;
      YPosition += 1.0f / YScaleFactor;
    }

    //
    //  Now read the data.  The data is column major.  We read each column
    //  and copy the column into the ouput buffer so it is row major.
    //  Also, the data in each columns goes from South to North.  We
    //  need to flip it so it goes from North to South.
    //

    XPosition = (float) ColumnStart - 1;
    ColumnToRead = (unsigned short) XPosition;
    for (ColumnNumber = 0; ColumnNumber < ColumnsInBuffer; ColumnNumber++)
    {
      if ((ReturnValue = _DTED252Data.Read(ColumnToRead,&_FileStream)) != 
           XP_GIS_OK)
      {
        if (RowToUse != NULL) free(RowToUse);
        return ReturnValue;
      };
      DataArray = _DTED252Data.DataArray();
      RowToUsePtr = RowToUse;
      for (RowNumber = 0; RowNumber < (long) RowsInBuffer; RowNumber++)
      {
        Buffer[(ColumnsInBuffer * RowNumber) + ColumnNumber] = 
                                           DataArray[*RowToUsePtr++];
      }
      XPosition += 1.0f / XScaleFactor;
      ColumnToRead = (unsigned short) XPosition;
    }
      
    if (RowToUse != NULL) free(RowToUse);
    return XP_GIS_OK;
}

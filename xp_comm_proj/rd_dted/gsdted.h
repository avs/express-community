//
//  Include file for the XP_GIS_DTED_c class, which provides utilities
//  to read an data from a DTED file.
//

/*

    Public Constructors

      XP_GIS_DTED_c(const char *FileName=NULL);;
        The constructor.  The optional FileName argument is a pointer
        to a string containing the fully qualified path of the DTED
        file that should be opened.

      virtual ~XP_GIS_DTED_c();
        the destructor.

      XP_GIS_DTED_c(const XP_GIS_DTED_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DTED_c &operator=(const XP_GIS_DTED_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long   FileName(const char* LocalFileName);
        Method to set the file name.  If there is already a file opened,
        this method closes the file before attempting to open the newly
        specified file.  Returns NULL on failure.  Otherwise returns the
        named of the newly opened file.  When the file name is set, the
        file is opened and all the headers are read.  After the file
        name has been set, the various accessors to get information
        from the file can be called.

      const char     *FileName() const;
        Method to get the currently-set file name.

      unsigned short NumberOfRows() const;
        Method to get the number of rows of gridposts in the DTED cell.

      unsigned short NumberOfColumns() const;
        Method to get the number of columns of gridposts in the DTED cell.

      double SouthLatitude() const;
        Method to get the South latitude of the DTED cell in decimal degrees.
      double WestLongitude() const;
        Method to get the West longitude of the DTED cell in decimal degrees.
      double NorthLatitude() const;
        Method to get the North latitude of the DTED cell in decimal degrees.
      double EastLongitude() const;
        Method to get the East longitude of the DTED cell in decimal degrees.

      unsigned short LatitudeInterval() const;
        Method to get the latitude intervale between gridposts in seconds..
      unsigned short LongitudeInterval() const;
        Method to get the longitude intervale between gridposts in seconds..

      const XP_GIS_DTEDUHL_c &DTEDUHLData() const;
        Method to get the UHL header object.
      const XP_GIS_DTEDDSI_c &DTEDDSIData() const;
        Method to get the DSI header object.
      const XP_GIS_DTEDACC_c &DTEDACCData() const;
        Method to get the ACC header object.
      const XP_GIS_DTED252_c &DTED252Data() const;
        Method to get the 252 object.  This is the object with elevation data
        for a single column from the DTED cell.

      unsigned long ReadData(short *Buffer);
        Method to read the entire elevation contents of the DTED cell at
        full resolution.  Buffer must have been previously allocated to
        have sufficient space to store the entire DTED cell.  It must
        be allocated to a size of NumberOfRows() * NumberOfColumns() * 
        sizeof (short).  Buffer is populated with data in a row major
        fashion, starting in the upper left (North West) corner.

      unsigned long ReadData(unsigned long RowStart,
                             unsigned long ColumnStart,
                             unsigned long NumberOfRows,
                             unsigned long NumberOfColumns,
                             float XScaleFactor,
                             float YScaleFactor,
                             short *Buffer);
        Method to read a portion of the elevation contents of the DTED cell.
        RowStart and ColumnStart specify the row and column number to start 
        reading data.  Both RowStart and ColumnStart are 1 based.  

        XScaleFactor and YScaleFactor specify the X and Y scale factors at
        which the data will be read.  A factor of 1 will result in no
        scaling.  A factor greater than 1 will result in upsampling.  A
        facator of less than 1 will result in downsampling.  Downsampling
        is accomplished using decimation.

        NumberOfRows and NumberOfColumns specify the number of rows and
        columns that will be populated in Buffer.  If NumberOfRows and
        NumberOfColumns are set to zero, The number of rows and columns
        will be computed as the number necessary to hold the entire cell
        at the sampling rate specified by XScaleFactor and YScaleFactor.

        Buffer must have been previously allocated to have sufficient space
        to store the entire DTED cell.  Buffer is populated with data in
        a row major fashion, starting in the upper left (North West) corner.

*/



#ifndef XP_GIS_DTED_H
#define XP_GIS_DTED_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "gsconst.h"
#include "gsuhl.h"
#include "gsdsi.h"
#include "gsacc.h"
#include "gs252.h"

class XP_GIS_DTED_c
{
  public:
    XP_GIS_DTED_c(const char *FileName=NULL);
    virtual ~XP_GIS_DTED_c();
    XP_GIS_DTED_c(const XP_GIS_DTED_c &object);
    XP_GIS_DTED_c &operator=(const XP_GIS_DTED_c &object);

    unsigned long   FileName(const char* LocalFileName);
    const char     *FileName() const {return _FileName;} 

    unsigned short NumberOfRows()    
            const {return _DTEDDSIData.NumberOfLatitudeLines();}
    unsigned short NumberOfColumns()
            const {return _DTEDDSIData.NumberOfLongitudeLines();}

    double SouthLatitude() const {return _DTEDDSIData.SWLatitude();}
    double WestLongitude() const {return _DTEDDSIData.SWLongitude();}
    double NorthLatitude() const {return _DTEDDSIData.NELatitude();}
    double EastLongitude() const {return _DTEDDSIData.NELongitude();}

    unsigned short LatitudeInterval()
            const {return (unsigned short) _DTEDDSIData.LatitudeInterval();}
    unsigned short LongitudeInterval()
            const {return (unsigned short) _DTEDDSIData.LongitudeInterval();}

    const XP_GIS_DTEDUHL_c &DTEDUHLData() const {return _DTEDUHLData;}
    const XP_GIS_DTEDDSI_c &DTEDDSIData() const {return _DTEDDSIData;}
    const XP_GIS_DTEDACC_c &DTEDACCData() const {return _DTEDACCData;}
    const XP_GIS_DTED252_c &DTED252Data() const {return _DTED252Data;}

    unsigned long ReadData(short *Buffer);
    unsigned long ReadData(unsigned long RowStart,
                           unsigned long ColumnStart,
                           unsigned long NumberOfRows,
                           unsigned long NumberOfColumns,
                           float XScaleFactor,
                           float YScaleFactor,
                           short *Buffer);

  private:

    void _CloseFile();

    ifstream      _FileStream;

    XP_GIS_DTEDUHL_c _DTEDUHLData;   // uhl record information
    XP_GIS_DTEDDSI_c _DTEDDSIData;   // dsi record information
    XP_GIS_DTEDACC_c _DTEDACCData;   // acc record information
    XP_GIS_DTED252_c _DTED252Data;   // elevation record information

    char  _FileName[_MAX_PATH];      // fully qualified file path
    unsigned char _FileIsOpen;       // is the file open (T/F)
};

#endif   // XP_GIS_DTED_H


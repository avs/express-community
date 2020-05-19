//
//  Include file for gsgtiff for reading GeoTif files.
//

/*

    Public Constructors

      XP_GIS_GTIFF_c(const char *FileName=NULL);;
        The constructor.

      virtual ~XP_GIS_GTIFF_c();
        the destructor.

      XP_GIS_GTIFF_c(const XP_GIS_GTIFF_c &object);
        The copy constructor


    Public Operators

      XP_GIS_GTIFF_c &operator=(const XP_GIS_GTIFF_c &object);
        The assignment operator assigns all values as necessary.  It
        is implemented by invoking the FileName method.  Therefore, "this"
        closes any open files, opens the files in object, and sets up all
        its private properties.  Since this object opens all files read-only,
        there is no chance for conflict between open files.

    Public Methods

      unsigned long FileName(const char* FileName);
        Method to set the file name.  If there is already a file opened,
        this method closes the file before attempting to open the newly
        specified file.  Returns NULL on failure.  Otherwise returns the
        named of the newly opened file.

      int NumberOfColumns() const;
	Method that returns the number of columns in an image.

      int NumberOfRows() const;
	Method that returns the number of rows in an image.

      int DepthOfImage() const;
	Method that returns the depth of an image.

      unsigned long SWCorner(double *SWLongitude,double *SWLatitude) const;
	Method that returns SWCorner latitude and longitude of an GeoTiff 
        image.

      unsigned long PixelScale(double *SWLongitude,double *SWLatitude) const;
	Method that returns the pixel scale for latitude and longitude of an 
        GeoTiff image.

      unsigned long CheckImageSize(unsigned long RowStart,
                                   unsigned long ColumnStart,
                                   unsigned long RowsInBuffer,
                                   unsigned long ColumnsInBuffer) const;
        Method that checks the user request chip size against the size of the 
	image making sure the chip fits within the bounds of the image.  
        Returns XP_GIS_OK if the chip is within the bounds of the image.
        These are zero based images (start at 0,0)


      unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       unsigned char **Buffer) 
       This method creates the memory for a buffer and reads in the 
       data from the GeoTiff file. This is for 8 bit pixels with depth of 
       8 or 24 as received from the DepthOfImage method.  
       If successfull, returns XP_GIS_OK.  
        These are zero based images (start at 0,0)

      unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       short **Buffer) 
       This method creates the memory for a buffer and reads in the 
       data from the GeoTiff file. This is for 16 bit pixels with depth of 
       16 as received from the DepthOfImage method.  
        These are zero based images (start at 0,0)



      unsigned long XP_GIS_GTIFF_c::ReadChip(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       float **Buffer) 
       This method creates the memory for a buffer and reads in the 
       data from the GeoTiff file. This is for elevation data stored as
       32 bit float's.
        These are zero based images (start at 0,0)



      unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       unsigned char *Buffer)

        Method to read the geotiff data.  This method reads a chip of an 
        image If successfull, reutrns XP_GIS_OK. These are useful for 
        applications that supply the already allocated data structure.
        These are zero based images (start at 0,0)

      unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       short *Buffer)

        Method to read the geotiff data.  This method reads a chip of an 
        image If successfull, reutrns XP_GIS_OK. These are useful for 
        applications that supply the already allocated data structure.
        These are zero based images (start at 0,0)

        unsigned long XP_GIS_GTIFF_c::ReadData(unsigned long RowStart,
				       unsigned long ColumnStart,
                                       unsigned long RowsInBuffer,
                                       unsigned long ColumnsInBuffer,
				       float *Buffer)

        Method to read the geotiff data.  This method reads a chip of an 
        image If successfull, reutrns XP_GIS_OK. These are useful for 
        applications that supply the already allocated data structure.
        These are zero based images (start at 0,0)



*/



#ifndef XP_GIS_GTIFF_H
#define XP_GIS_GTIFF_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "geotiff.h"
#include "xtiffiop.h"

#ifndef XP_GIS_TRUE
#define XP_GIS_TRUE 1
#endif

#ifndef XP_GIS_FALSE
#define XP_GIS_FALSE 0
#endif

// from GeoTIFF library, for file version checking
enum {VERSION=0,MAJOR,MINOR};

class XP_GIS_GTIFF_c
{
  public:

    enum ReadWriteMode_e {GEOTIFF_READ, GEOTIFF_WRITE};
    enum Orientation_e {TW_ORIENTATION_TOPLEFT=0,TW_ORIENTATION_TOPRIGHT, 
                        TW_ORIENTATION_BOTRIGHT,TW_ORIENTATION_BOTLEFT, 
                        TW_ORIENTATION_LEFTTOP,TW_ORIENTATION_RIGHTTOP, 
                        TW_ORIENTATION_RIGHTBOT,TW_ORIENTATION_LEFTBOT};

    XP_GIS_GTIFF_c(const char *FileName = NULL);
    XP_GIS_GTIFF_c(const char *FileName, ReadWriteMode_e Mode);

    virtual ~XP_GIS_GTIFF_c();

    XP_GIS_GTIFF_c(const XP_GIS_GTIFF_c &object);
    XP_GIS_GTIFF_c &operator=(const XP_GIS_GTIFF_c &object);

    unsigned long   FileName(const char* LocalFileName);
    unsigned long   FileNameWrite(const char* LocalFileName);
    const char     *FileName() const {return _FileName;} 

    unsigned long   FileIsOpen(void) const;

    int NumberOfRows() const; 
    int NumberOfColumns() const;
    int DepthOfImage() const;
    Orientation_e GetOrientation(void);

    // set and get routines for tie points
    unsigned long NWCorner(double NWLongitude,double NWLatitude);
    unsigned long SWCorner(double *SWLongitude,double *SWLatitude) const; 

    // set and get routines for pixel scale
    unsigned long PixelScale(double PixelScaleLongitude,
                              double PixelScaleLatitiude); 
    unsigned long PixelScale(double *PixelScaleLongitude,
                              double *PixelScaleLatitiude) const; 

    double SouthLatitude() const {return(_SWLatitude);}
    double WestLongitude() const {return(_SWLongitude);}
    double NorthLatitude() const {return(_NELatitude);}
    double EastLongitude() const {return(_NELongitude);}

    unsigned long CheckImageSize(unsigned long RowStart,
                   unsigned long ColumnStart,
                   unsigned long RowsInBuffer,
                   unsigned long ColumnsInBuffer) const;

    unsigned long ReadChip(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  unsigned char **Buffer);

    unsigned long ReadChip(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  short **Buffer);

    unsigned long ReadChip(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  float **Buffer);

    unsigned long ReadData(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  unsigned char *Buffer);

    unsigned long ReadData(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  short *Buffer);

    unsigned long ReadData(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  float *Buffer);

  private:


    double _SWLatitude;
    double _SWLongitude;
    double _SELatitude;
    double _SELongitude;
    double _NWLatitude;
    double _NWLongitude;
    double _NELatitude;
    double _NELongitude;
    double _RasterSpacePosition[3];    
    double _LonLat[3];
    double _PixelScale[3];

    void _Initialization(const char *FileName, ReadWriteMode_e Mode);

    void _CloseFile();

    unsigned long _ReadDataUnTiled(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  unsigned char *Buffer);

    unsigned long _ReadDataUnTiled(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  short *Buffer);

    unsigned long _ReadDataUnTiled(unsigned long RowStart,
                                  unsigned long ColumnStart,
                                  unsigned long NumberOfRows,
                                  unsigned long NumberOfColumns,
                                  float *Buffer);

    TIFF *_tif;
    GTIF *_gtif;

    Orientation_e _Orientation;
    ifstream _FileStream;
    unsigned char _BufferAllocated;  // Has the buffer been allocated (T/F)
    unsigned char *_InternalBufferPointerChar;
    short *_InternalBufferPointerShort;
    float *_InternalBufferPointerFloat;

    char _FileName[1024];      // fully qualified file path
    unsigned char _FileIsOpen;       // is the file open (T/F)
    ReadWriteMode_e _FileMode;
};

#endif   // XP_GIS_GTIFF_H


//
//  Include file for the XP_GIS_Spot_c class, which provides utilities
//  to read a SPOT image from a file.
//

/*

    Public Constructors

      XP_GIS_Spot_c(const char *FileName=NULL);
        The constructor.

      virtual ~XP_GIS_Spot_c();
        the destructor.

      XP_GIS_Spot_c(const XP_GIS_Spot_c &object);
        The copy constructor


    Public Operators

      XP_GIS_Spot_c &operator=(const XP_GIS_Spot_c &object);
        The assignment operator assigns all values as necessary.  It
        is implemented by invoking the FileName method.  Therefore, "this"
        closes any open files, opens the files in object, and sets up all
        its private properties.  Since this object opens all files read-only,
        there is no chance for conflict between open files.

    Public Methods

      unsigned long PrintHeader(ostream &PrintStream);
        Method to print the file header.

      unsigned long ReadImage(unsigned long RowStart,
                              unsigned long ColumnStart,
                              unsigned long NumberOfRows,
                              unsigned long NumberOfColumns,
                              float         XScaleFactor,
                              float         YScaleFactor,
                              unsigned char *Buffer);

        Method to read an image.  If the image is multispectral, this
        method reads all bands and places them into Buffer.  If the
        image is panchromatic, it reads the one band in the image and
        places it into Buffer.   The value for each pixel in each band
        will have a range of 0 to 255.

        RowStart specifies the first row in the image to read.  ColumnStart
        specifies the first column in the image to read.  Both RowStart
        and ColumnStart are zero-based and start in the upper left corner
        of the image.

        NumberOfRows and NumberOfColumns specify the height and width,
        respectively, of the output buffer to be populated.  These numbers
        can be less than, equal to, or greater than the number of rows
        and columns in the image, depending on the values of the X and Y
        scaling factors.

        XScaleFactor and YScaleFactor specify the X and Y scale factors
        to be used when reading the image.  A scale factor less than
        1 downsamples the image in the specified dimension.  A scale
        factor greater than 1 upsamples the image in the specified
        dimension.

        As an example, consider a Level 1A panchromatic image.  This image
        will have 6000 rows and 6000 columns.  To build a 1000 row by 1000
        column output buffer of the entire image, we would use the following:
            RowStart        = 0
            ColumnStart     = 0
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0 / 6.0
            YScaleFactor    = 1.0 / 6.0

      To build a 1000 row by 1000 column output buffer of the lower right
      corner of the image at full resolution, we would use the following:
            RowStart        = 4999
            ColumnStart     = 4999
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0
            YScaleFactor    = 1.0

      unsigned long ReadBand(unsigned long BandNumber,
                             unsigned long RowStart,
                             unsigned long ColumnStart,
                             unsigned long NumberOfRows,
                             unsigned long NumberOfColumns,
                             float         XScaleFactor,
                             float         YScaleFactor,

        Method to read one band from an image.  If the image is multispectral,
        this method reads only the specified band and places it into Buffer.
        If the image is panchromatic, it reads the one band in the image and
        places it into Buffer.   The value for each pixel in the band
        will have a range of 0 to 255.

        BandNumber specifies the band to read.  It is one-based.

        RowStart specifies the first row in the image to read.  ColumnStart
        specifies the first column in the image to read.  Both RowStart
        and ColumnStart are zero-based and start in the upper left corner
        of the image.

        NumberOfRows and NumberOfColumns specify the height and width,
        respectively, of the output buffer to be populated.  These numbers
        can be less than, equal to, or greater than the number of rows
        and columns in the image, depending on the values of the X and Y
        scaling factors.

        XScaleFactor and YScaleFactor specify the X and Y scale factors
        to be used when reading the image.  A scale factor less than
        1 downsamples the image in the specified dimension.  A scale
        factor greater than 1 upsamples the image in the specified
        dimension.

        As an example, consider a Level 1A panchromatic image.  This image
        will have 6000 rows and 6000 columns.  To build a 1000 row by 1000
        column output buffer of the entire image, we would use the following:
            RowStart        = 0
            ColumnStart     = 0
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0 / 6.0
            YScaleFactor    = 1.0 / 6.0

      To build a 1000 row by 1000 column output buffer of the lower right
      corner of the image at full resolution, we would use the following:
            RowStart        = 4999
            ColumnStart     = 4999
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0
            YScaleFactor    = 1.0

        Method to read an image.  If the image is multispectral, this
        method reads all bands and places them into Buffer.  If the
        image is panchromatic, it reads the one band in the image and
        places it into Buffer.   The value for each pixel in each band
        will have a range of 0 to 255.

        RowStart specifies the first row in the image to read.  ColumnStart
        specifies the first column in the image to read.  Both RowStart
        and ColumnStart are zero-based and start in the upper left corner
        of the image.

        NumberOfRows and NumberOfColumns specify the height and width,
        respectively, of the output buffer to be populated.  These numbers
        can be less than, equal to, or greater than the number of rows
        and columns in the image, depending on the values of the X and Y
        scaling factors.

        XScaleFactor and YScaleFactor specify the X and Y scale factors
        to be used when reading the image.  A scale factor less than
        1 downsamples the image in the specified dimension.  A scale
        factor greater than 1 upsamples the image in the specified
        dimension.

        As an example, consider a Level 1A panchromatic image.  This image
        will have 6000 rows and 6000 columns.  To build a 1000 row by 1000
        column output buffer of the entire image, we would use the following:
            RowStart        = 0
            ColumnStart     = 0
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0 / 6.0
            YScaleFactor    = 1.0 / 6.0

      To build a 1000 row by 1000 column output buffer of the lower right
      corner of the image at full resolution, we would use the following:
            RowStart        = 4999
            ColumnStart     = 4999
            NumberOfRows    = 1000
            NumberOfColumns = 1000
            XScaleFactor    = 1.0
            YScaleFactor    = 1.0
                             unsigned char *Buffer);

      unsigned long HeaderFileName(const char* FileName);
        Method to set the name of the header file.  This method reads the
        header file and populates the private variables as appropriate.

      unsigned long ImageFileName(const char* FileName);
        Method to set the name of the image file.

      unsigned char HeaderFileIsOpen() const;
        Method to get the name of the header file.

      unsigned char ImageFileIsOpen() const;
        Method to get the name of the image file.


      The following accessors get the value of the various header elements

      double LLLatitude() const;
      double LLLongitude() const;
      double LRLatitude() const;
      double LRLongitude() const;
      double URLatitude() const;
      double URLongitude() const;
      double ULLatitude() const;
      double ULLongitude() const;
      double CenterLatitude() const;
      double CenterLongitude() const;
      double OrientationAngle() const;
      const char *IncidenceAngle() const;
      double SunAzimuthAngle() const;
      double SunElevationAngle() const;
      unsigned long Year() const;
      unsigned long Month() const;
      unsigned long Day() const;
      unsigned long Hour() const;
      unsigned long Minute() const;
      float Second() const;
      enum Mission_e Miss constion();
      enum SensorID_e SensorID const();
      enum SpectralMode_e SpectralM constode();
      unsigned long RevolutionWithinCycl conste();
      unsigned long MirrorStepValue() const;
      unsigned long Coding() const;
      const unsigned long *Gain() const;
      enum Interleaving_e Interleaving const();
      unsigned long NumberOfBands() const;
      const enum BandID_e *BandIDOrder() const;
      enum ProcessingLevel_e ProcessingLevel( const);
      unsigned long NumberOfRows() const;
      unsigned long NumberOfColumns() const;
      const char *HeaderFileName() const;
      const char *ImageFileName() const;

*/

#ifndef XP_GIS_SPOT_H
#define XP_GIS_SPOT_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"
#include "gsconst.h"

class XP_GIS_Spot_c
{
public:
   XP_GIS_Spot_c(const char *HeaderFileName=NULL,
                 const char *ImageFileName=NULL);
   virtual ~XP_GIS_Spot_c();
   XP_GIS_Spot_c(const XP_GIS_Spot_c &object);
   XP_GIS_Spot_c &operator=(const XP_GIS_Spot_c &object);

   unsigned long PrintHeader(ostream &PrintStream);

   //
   //  Methods to read the image
   //

   unsigned long ReadImage(unsigned long RowStart,
                           unsigned long ColumnStart,
                           unsigned long NumberOfRows,
                           unsigned long NumberOfColumns,
                           float         XScaleFactor,
                           float         YScaleFactor,
                           unsigned char *Buffer);
   unsigned long ReadBand(unsigned long BandNumber,
                          unsigned long RowStart,
                          unsigned long ColumnStart,
                          unsigned long NumberOfRows,
                          unsigned long NumberOfColumns,
                          float         XScaleFactor,
                          float         YScaleFactor,
                          unsigned char *Buffer);
   //
   //  Accesors to set values
   //

   unsigned long HeaderFileName(const char* FileName);
   unsigned long ImageFileName(const char* FileName);

   //
   //  Accessors to get file state
   //

   unsigned char HeaderFileIsOpen() const {return _HeaderFileIsOpen;}
   unsigned char ImageFileIsOpen() const {return _ImageFileIsOpen;}

   //
   //  Enumerations
   //

   enum Mission_e 
   {
      SPOT1,
      SPOT2,
      SPOT3,
      SPOT4,
      SPOT5,
      UNKNOWN_MISSION
   };

   enum SensorID_e
   {
      HRV1,
      HRV2,
      UNKNOWN_SENSOR_ID
   };

   enum Coding_e
   {
      DT,
      DPCM,
      E2,
      UNKNOWN_CODING
   };

   enum SpectralMode_e
   {
      PAN,
      XS,
      UNKNOWN_SPECTRAL_MODE
   };

   enum Interleaving_e
   {
      BIL,
      UNKNOWN_INTERLEAVING
   };

   enum BandID_e
   {
      XS1,
      XS2,
      XS3,
      PAN_BAND_ID,
      UNKNOWN_BAND_ID
   };

   enum ProcessingLevel_e
   {
      ONE_A,
      ONE_B,
      UNKNOWN_PROCESSING_LEVEL
   };

   //
   //  Accesors to get values
   //

   double LLLatitude()                   const {return _LLLatitude;}
   double LLLongitude()                  const {return _LLLongitude;}
   double LRLatitude()                   const {return _LRLatitude;}
   double LRLongitude()                  const {return _LRLongitude;}
   double URLatitude()                   const {return _URLatitude;}
   double URLongitude()                  const {return _URLongitude;}
   double ULLatitude()                   const {return _ULLatitude;}
   double ULLongitude()                  const {return _ULLongitude;}
   double CenterLatitude()               const {return _CenterLatitude;}
   double CenterLongitude()              const {return _CenterLongitude;}
   double OrientationAngle()             const {return _OrientationAngle;}
   const char *IncidenceAngle()          const {return _IncidenceAngle;}
   double SunAzimuthAngle()              const {return _SunAzimuthAngle;}
   double SunElevationAngle()            const {return _SunElevationAngle;}
   unsigned long Year()                  const {return _Year;}
   unsigned long Month()                 const {return _Month;}
   unsigned long Day()                   const {return _Day;}
   unsigned long Hour()                  const {return _Hour;}
   unsigned long Minute()                const {return _Minute;}
   float Second()                        const {return _Second;}
   enum Mission_e Mission()              const {return _Mission;}
   enum SensorID_e SensorID()            const {return _SensorID;}
   enum SpectralMode_e SpectralMode()    const {return _SpectralMode;}
   unsigned long RevolutionWithinCycle() const {return _RevolutionWithinCycle;}
   unsigned long MirrorStepValue()       const {return _MirrorStepValue;}
   unsigned long Coding()                const {return _Coding;}
   const unsigned long *Gain()           const {return _Gain;}
   enum Interleaving_e Interleaving()    const {return _Interleaving;}
   unsigned long NumberOfBands()         const {return _NumberOfBands;}
   const enum BandID_e *BandIDOrder()    const {return _BandIDOrder;}
   enum ProcessingLevel_e ProcessingLevel()  const {return _ProcessingLevel;}
   unsigned long NumberOfRows()          const {return _NumberOfRows;}
   unsigned long NumberOfColumns()       const {return _NumberOfColumns;}
   const char *HeaderFileName()          const {return _HeaderFileName;} 
   const char *ImageFileName()           const {return _ImageFileName;} 
private:
   void _CloseFiles();
   void _CloseHeaderFile();
   void _CloseImageFile();
   unsigned long _ReadFileHeader();
   unsigned long _ReadRow(unsigned long  RowNumber,
                          unsigned char *RowBuffer);
   double _ConvertLatitude(const char *LatitudeString) const;
   double _ConvertLongitude(const char *LongitudeString) const;

   ifstream      _HeaderFileStream;
   ifstream      _ImageFileStream;

   unsigned char _HeaderFileIsOpen;
   unsigned char _ImageFileIsOpen;

   double         _LLLatitude;
   double         _LLLongitude;
   double         _LRLatitude;
   double         _LRLongitude;
   double         _ULLatitude;
   double         _ULLongitude;
   double         _URLatitude;
   double         _URLongitude;
   double         _CenterLatitude;
   double         _CenterLongitude;
   double         _OrientationAngle;
   double         _SunAzimuthAngle;
   double         _SunElevationAngle;
   unsigned long  _RevolutionWithinCycle;
   unsigned long  _MirrorStepValue;
   unsigned long  _Gain[3];
   unsigned long  _NumberOfBands;
   unsigned long  _NumberOfRows;
   unsigned long  _NumberOfColumns;
   unsigned long  _Year;
   unsigned long  _Month;
   unsigned long  _Day;
   unsigned long  _Hour;
   unsigned long  _Minute;
   float          _Second;
   char           _HeaderFileName[_MAX_PATH];   // fully qualified file path
   char           _ImageFileName[_MAX_PATH];    // fully qualified file path
   char           _IncidenceAngle[17];
   enum Mission_e          _Mission;
   enum SensorID_e         _SensorID;
   enum Coding_e           _Coding;
   enum SpectralMode_e     _SpectralMode;
   enum Interleaving_e     _Interleaving;
   enum BandID_e           _BandIDOrder[3];
   enum ProcessingLevel_e  _ProcessingLevel;
};

#endif   // XP_GIS_SPOT_H

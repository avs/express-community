#include <stdio.h>

#include <avs/om.h>
#include <avs/gd_def.h>
#include <avs/f_utils.h>

#include "gsspot.hxx"


//  Define the lengths of the various fixed-length file items.

static const unsigned long PanImageRecordLength        = 8640; // in bytes
static const unsigned long XSImageRecordLength         = 5400; // in bytes
static const unsigned long HeaderHeaderRecordLength    = 8640; // in bytes
static const unsigned long LineMetadataLength          =   32; // in bytes

//  Define the starting byte within the header record.

static const unsigned long HeaderOffset = 3959;

static const unsigned long SceneLocationStart         = HeaderOffset + 117;
static const unsigned long SceneLocationEnd           = HeaderOffset + 436;

static const unsigned long OrientationAngleStart      = HeaderOffset + 437;
static const unsigned long OrientationAngleEnd        = HeaderOffset + 452;

static const unsigned long IncidenceAngleStart        = HeaderOffset + 453;
static const unsigned long IncidenceAngleEnd          = HeaderOffset + 468;

static const unsigned long SunAzimuthAngleStart       = HeaderOffset + 469;
static const unsigned long SunAzimuthAngleEnd         = HeaderOffset + 484;

static const unsigned long SunElevationAngleStart     = HeaderOffset + 485;
static const unsigned long SunElevationAngleEnd       = HeaderOffset + 500;

static const unsigned long DateTimeStart              = HeaderOffset + 581;
static const unsigned long DateTimeEnd                = HeaderOffset + 612;

static const unsigned long MissionStart               = HeaderOffset + 613;
static const unsigned long MissionEnd                 = HeaderOffset + 628;

static const unsigned long SensorIDStart              = HeaderOffset + 629;
static const unsigned long SensorIDEnd                = HeaderOffset + 644;

static const unsigned long SpectralModeStart          = HeaderOffset + 645;
static const unsigned long SpectralModeEnd            = HeaderOffset + 660;

static const unsigned long RevolutionWithinCycleStart = HeaderOffset + 661;
static const unsigned long RevolutionWithinCycleEnd   = HeaderOffset + 676;

static const unsigned long MirrorStepValueStart       = HeaderOffset + 677;
static const unsigned long MirrorStepValueEnd         = HeaderOffset + 692;

static const unsigned long CodingStart                = HeaderOffset + 693;
static const unsigned long CodingEnd                  = HeaderOffset + 710;

static const unsigned long GainNumberStart            = HeaderOffset + 725;
static const unsigned long GainNumberEnd              = HeaderOffset + 740;

static const unsigned long NumberOfColumnsStart       = HeaderOffset + 997;
static const unsigned long NumberOfColumnsEnd         = HeaderOffset + 1012;

static const unsigned long NumberOfRowsStart          = HeaderOffset + 1013;
static const unsigned long NumberOfRowsEnd            = HeaderOffset + 1028;

static const unsigned long InterleavingStart          = HeaderOffset + 1029;
static const unsigned long InterleavingEnd            = HeaderOffset + 1044;

static const unsigned long NumberOfBandsStart         = HeaderOffset + 1045;
static const unsigned long NumberOfBandsEnd           = HeaderOffset + 1060;

static const unsigned long BandIDOrderStart           = HeaderOffset + 1061;
static const unsigned long BandIDOrderEnd             = HeaderOffset + 1316;

static const unsigned long ProcessingLevelStart       = HeaderOffset + 1317;
static const unsigned long ProcessingLevelEnd         = HeaderOffset + 1332;

//
//  Constructor.
//

XP_GIS_Spot_c::XP_GIS_Spot_c(const char *LocalHeaderFileName,
                             const char *LocalImageFileName)
{

    //
    //  Initialize strings to empty.
    //

    _HeaderFileName[0] = '\0';
    _ImageFileName[0]  = '\0';
    _IncidenceAngle[0] = '\0';

    //
    //  Initialize the header and image file states to closed.
    //

    _HeaderFileIsOpen = XP_GIS_FALSE;
    _ImageFileIsOpen  = XP_GIS_FALSE;

    //
    //  If the header file name is not null, then use it to
    //  set the object's header file name.
    //

    if (LocalHeaderFileName != NULL)
    {
      HeaderFileName(LocalHeaderFileName);
    }

    //
    //  If the image file name is not null, then use it to
    //  set the object's image file name.
    //

    if (LocalImageFileName != NULL)
    {
      ImageFileName(LocalImageFileName);
    }

}

//
//  destructor.
//

XP_GIS_Spot_c::~XP_GIS_Spot_c()
{
    //
    //  Close open file.
    //

    _CloseFiles();
}

//
//  Copy constructor.
//

XP_GIS_Spot_c::XP_GIS_Spot_c(const XP_GIS_Spot_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

XP_GIS_Spot_c &XP_GIS_Spot_c::operator=(
                      const XP_GIS_Spot_c &object)
{

    //
    //  To set up the object properly, we have to set the
    //  file name via the accessor.  This will set the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    _HeaderFileIsOpen = XP_GIS_FALSE;
    _ImageFileIsOpen  = XP_GIS_FALSE;
    HeaderFileName(object._HeaderFileName);
    ImageFileName(object._ImageFileName);

    return *this;
}

//
//  Method to set the header file name.  If there is already a 
//  header file opened, this method closes the file before attempting
//  to open the newly specified file.  Returns XP_GIS_OK if successful.
//  Otherwise, returns one of the following:
//
//    XP_GIS_OPEN_ERROR;
//

unsigned long XP_GIS_Spot_c::HeaderFileName(const char* FileName) 
{
    unsigned long ReturnValue;


    //
    //  If a null is passed in, then close any open files.
    //

    if (FileName == NULL)
    {
      _CloseFiles();
      return XP_GIS_OK;
    }

    //
    //  Copy the file name that was passed in to the private variable.
    //  If does not have a suffix of .dat, assume it is a base name, and
    //  add the suffix.
    //

    strcpy(_HeaderFileName,FileName);
    if (strcmp(&FileName[strlen(FileName)-4],".dat") != 0) 
    {
      strcat(_HeaderFileName,".dat");
    }

    //
    //  Now, attempt to open the requested file.  If not successful,
    //  clear the file name.  Otherwise, populate the file name and
    //  read the file header.
    //

#ifdef MSDOS
    _HeaderFileStream.open(_HeaderFileName,ios::binary | ios::nocreate);
#else
    _HeaderFileStream.open(_HeaderFileName,ios::nocreate);
#endif
    if (_HeaderFileStream == 0)
    {
      _HeaderFileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }
    if (_HeaderFileStream.bad())
    {
      _HeaderFileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }
    else
    {
      _HeaderFileIsOpen = XP_GIS_TRUE;
      if ((ReturnValue = _ReadFileHeader()) != XP_GIS_OK)
      {
        _CloseHeaderFile();
        return ReturnValue;     
      }
    }
    return XP_GIS_OK;     
}

//
//  Method to set the image file name.  If there is already a 
//  image file opened, this method closes the file before attempting
//  to open the newly specified file.  Returns XP_GIS_OK if successful.
//  Otherwise, returns one of the following:
//
//    XP_GIS_OPEN_ERROR;
//

unsigned long XP_GIS_Spot_c::ImageFileName(const char* FileName) 
{


    //
    //  If a null is passed in, then close any open files.
    //

    if (FileName == NULL)
    {
      _CloseFiles();
      return XP_GIS_OK;
    }

    //
    //  Copy the file name that was passed in to the private variable.
    //  If does not have a suffix of .dat, assume it is a base name, and
    //  add the suffix.
    //

    strcpy(_ImageFileName,FileName);
    if (strcmp(&FileName[strlen(FileName)-4],".dat") != 0) 
    {
      strcat(_ImageFileName,".dat");
    }

    //
    //  Now, attempt to open the requested file.  If not successful,
    //  clear the file name.  Otherwise, populate the file name and
    //  read the file header.
    //

#ifdef MSDOS
    _ImageFileStream.open(_ImageFileName,ios::binary | ios::nocreate);
#else
    _ImageFileStream.open(_ImageFileName,ios::nocreate);
#endif
    if (_ImageFileStream == 0)
    {
      _ImageFileName[0] = '\0';
      return XP_GIS_OPEN_ERROR;
    }
    if (_ImageFileStream.bad())
    {
      return XP_GIS_OPEN_ERROR;
    }
    _ImageFileIsOpen = XP_GIS_TRUE;
    return XP_GIS_OK;
}

void XP_GIS_Spot_c::_CloseFiles()
{

    //
    //  Close the header file.
    //

    _CloseHeaderFile();
    _CloseImageFile();

    return;
}

void XP_GIS_Spot_c::_CloseHeaderFile()
{

    //
    //  Close the header file.
    //

    if (HeaderFileIsOpen() == XP_GIS_TRUE)
    {
      _HeaderFileIsOpen = XP_GIS_FALSE;
      _HeaderFileStream.close();
      _HeaderFileName[0] = '\0';
    }

    return;
}

void XP_GIS_Spot_c::_CloseImageFile()
{

    //
    //  Close the image file.
    //

    if (ImageFileIsOpen() == XP_GIS_TRUE)
    {
      _ImageFileIsOpen = XP_GIS_FALSE;
      _ImageFileStream.close();
      _ImageFileName[0] = '\0';
    }

    return;
}

unsigned long XP_GIS_Spot_c::_ReadFileHeader()
{
    unsigned char      HeaderBuffer[HeaderHeaderRecordLength];
    char               StringBuffer[HeaderHeaderRecordLength + 1];
    char               TempString[HeaderHeaderRecordLength + 1];
    unsigned long      BandNumber;
    unsigned long      StringOffset;
    unsigned long      ThousandSeconds;
    unsigned long      CharactersRead;
    int                TempInt;

    //
    //  Make sure the file is open
    //

    if (HeaderFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  read the record header
    //

    if (!_HeaderFileStream.read(HeaderBuffer,
                          HeaderHeaderRecordLength))
    {
      if (_HeaderFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_HeaderFileStream.gcount() != HeaderHeaderRecordLength)
    {
      return XP_GIS_IO_ERROR;
    }

    //
    //  Parse out the center and corner points
    //

    memcpy(StringBuffer,&HeaderBuffer[SceneLocationStart],
           (SceneLocationEnd - SceneLocationStart) + 1);
    StringBuffer[((SceneLocationEnd - SceneLocationStart) + 1)] = '\0';

    TempString[8] = '\0';
    StringOffset = 32;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _CenterLatitude = _ConvertLatitude(TempString);
    StringOffset += 16;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _CenterLongitude = _ConvertLongitude(TempString);

    StringOffset += 48;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _ULLatitude = _ConvertLatitude(TempString);
    StringOffset += 16;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _ULLongitude = _ConvertLongitude(TempString);

    StringOffset += 48;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _LLLatitude = _ConvertLatitude(TempString);
    StringOffset += 16;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _LLLongitude = _ConvertLongitude(TempString);

    StringOffset += 48;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _LRLatitude = _ConvertLatitude(TempString);
    StringOffset += 16;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _LRLongitude = _ConvertLongitude(TempString);

    StringOffset += 48;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _URLatitude = _ConvertLatitude(TempString);
    StringOffset += 16;
    memcpy(TempString,&StringBuffer[StringOffset],8);
    _URLongitude = _ConvertLongitude(TempString);
    
    //
    //  Parse out the orientation angle
    //

    memcpy(StringBuffer,&HeaderBuffer[OrientationAngleStart],
           (OrientationAngleEnd - OrientationAngleStart) + 1);
    StringBuffer[((OrientationAngleEnd - OrientationAngleStart) + 1)] = '\0';
    sscanf(StringBuffer,"%lf",&_OrientationAngle);

    //
    //  Parse out the incidence angle
    //

    memcpy(StringBuffer,&HeaderBuffer[IncidenceAngleStart],
           (IncidenceAngleEnd - IncidenceAngleStart) + 1);
    StringBuffer[((IncidenceAngleEnd - IncidenceAngleStart) + 1)] = '\0';
    _IncidenceAngle[0] = StringBuffer[0];
    sscanf(StringBuffer,"%1s %n",_IncidenceAngle,&CharactersRead);
    sscanf(&StringBuffer[CharactersRead],"%s",&_IncidenceAngle[1]);

    //
    //  Parse out the sun azimuth angle
    //

    memcpy(StringBuffer,&HeaderBuffer[SunAzimuthAngleStart],
           (SunAzimuthAngleEnd - SunAzimuthAngleStart) + 1);
    StringBuffer[((SunAzimuthAngleEnd - SunAzimuthAngleStart) + 1)] = '\0';
    sscanf(StringBuffer,"%lf",&_SunAzimuthAngle);

    //
    //  Parse out the sun elevation angle
    //

    memcpy(StringBuffer,&HeaderBuffer[SunElevationAngleStart],
           (SunElevationAngleEnd - SunElevationAngleStart) + 1);
    StringBuffer[((SunElevationAngleEnd - SunElevationAngleStart) + 1)] = '\0';
    sscanf(StringBuffer,"%lf",&_SunElevationAngle);

    //
    //  Parse out the date and time
    //

    memcpy(StringBuffer,&HeaderBuffer[DateTimeStart],
           (DateTimeEnd - DateTimeStart) + 1);
    StringBuffer[((DateTimeEnd - DateTimeStart) + 1)] = '\0';
    sscanf(StringBuffer,"%4d %2d %2d %2d %2d %5d",
           &_Year,
           &_Month,
           &_Day,
           &_Hour,
           &_Minute,
           &ThousandSeconds);
    _Second = (float) (ThousandSeconds / 1000.0f);

    //
    //  Parse out the mission
    //

    memcpy(StringBuffer,&HeaderBuffer[MissionStart],
           (MissionEnd - MissionStart) + 1);
    StringBuffer[((MissionEnd - MissionStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"SPOT1") == 0)
    {
      _Mission = XP_GIS_Spot_c::SPOT1;
    }
    else if (strcmp(TempString,"SPOT2") == 0)
    {
      _Mission = XP_GIS_Spot_c::SPOT2;
    }
    else if (strcmp(TempString,"SPOT3") == 0)
    {
      _Mission = XP_GIS_Spot_c::SPOT3;
    }
    else if (strcmp(TempString,"SPOT4") == 0)
    {
      _Mission = XP_GIS_Spot_c::SPOT4;
    }
    else if (strcmp(TempString,"SPOT5") == 0)
    {
      _Mission = XP_GIS_Spot_c::SPOT5;
    }
    else
    {
      _Mission = XP_GIS_Spot_c::UNKNOWN_MISSION;
    }

    //
    //  Parse out the sensor id
    //

    memcpy(StringBuffer,&HeaderBuffer[SensorIDStart],
           (SensorIDEnd - SensorIDStart) + 1);
    StringBuffer[((SensorIDEnd - SensorIDStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"HRV1") == 0)
    {
      _SensorID = XP_GIS_Spot_c::HRV1;
    }
    else if (strcmp(TempString,"HRV2") == 0)
    {
      _SensorID = XP_GIS_Spot_c::HRV2;
    }
    else
    {
      _SensorID = XP_GIS_Spot_c::UNKNOWN_SENSOR_ID;
    }

    //
    //  Parse out the spectral mode
    //

    memcpy(StringBuffer,&HeaderBuffer[SpectralModeStart],
           (SpectralModeEnd - SpectralModeStart) + 1);
    StringBuffer[((SpectralModeEnd - SpectralModeStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"XS") == 0)
    {
      _SpectralMode = XP_GIS_Spot_c::XS;
    }
    else if (strcmp(TempString,"PAN") == 0)
    {
      _SpectralMode = XP_GIS_Spot_c::PAN;
    }
    else
    {
      _SpectralMode = XP_GIS_Spot_c::UNKNOWN_SPECTRAL_MODE;
    }

    //
    //  Parse out the revolution number within the cycle 
    //

    memcpy(StringBuffer,&HeaderBuffer[RevolutionWithinCycleStart],
           (RevolutionWithinCycleEnd - RevolutionWithinCycleStart) + 1);
    StringBuffer[((RevolutionWithinCycleEnd - 
                  RevolutionWithinCycleStart) + 1)] = '\0';
    sscanf(StringBuffer,"%d",&_RevolutionWithinCycle);

    //
    //  Parse out the pointing mirror step value
    //

    memcpy(StringBuffer,&HeaderBuffer[MirrorStepValueStart],
           (MirrorStepValueEnd - MirrorStepValueStart) + 1);
    StringBuffer[((MirrorStepValueEnd - MirrorStepValueStart) + 1)] = '\0';
    sscanf(StringBuffer,"%d",&_MirrorStepValue);

    //
    //  Parse out the coding type
    //

    memcpy(StringBuffer,&HeaderBuffer[CodingStart],
           (CodingEnd - CodingStart) + 1);
    StringBuffer[((CodingEnd - CodingStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"DT") == 0)
    {
      _Coding = XP_GIS_Spot_c::DT;
    }
    else if (strcmp(TempString,"DPCM") == 0)
    {
      _Coding = XP_GIS_Spot_c::DPCM;
    }
    else if (strcmp(TempString,"E2") == 0)
    {
      _Coding = XP_GIS_Spot_c::E2;
    }
    else
    {
      _Coding = XP_GIS_Spot_c::UNKNOWN_CODING;
    }

    //
    //  Parse out the number of bands
    //

    memcpy(StringBuffer,&HeaderBuffer[NumberOfBandsStart],
           (NumberOfBandsEnd - NumberOfBandsStart) + 1);
    StringBuffer[((NumberOfBandsEnd - NumberOfBandsStart) + 1)] = '\0';
    sscanf(StringBuffer,"%d",&_NumberOfBands);

    //
    //  Parse out the gain number for each band
    //

    memcpy(StringBuffer,&HeaderBuffer[GainNumberStart],
           (GainNumberEnd - GainNumberStart) + 1);
    StringBuffer[((GainNumberEnd - GainNumberStart) + 1)] = '\0';
    CharactersRead = 0;
    for (BandNumber = 0;
         BandNumber < _NumberOfBands;
         BandNumber++)
    {
      sscanf(&StringBuffer[CharactersRead],"%d %n",
             &_Gain[BandNumber],
             &TempInt);
      CharactersRead += TempInt;
    }

    //
    //  Parse out the number of columns
    //

    memcpy(StringBuffer,&HeaderBuffer[NumberOfColumnsStart],
           (NumberOfColumnsEnd - NumberOfColumnsStart) + 1);
    StringBuffer[((NumberOfColumnsEnd - NumberOfColumnsStart) + 1)] = '\0';
    sscanf(StringBuffer,"%d",&_NumberOfColumns);


    //
    //  Parse out the number of rows
    //

    memcpy(StringBuffer,&HeaderBuffer[NumberOfRowsStart],
           (NumberOfRowsEnd - NumberOfRowsStart) + 1);
    StringBuffer[((NumberOfRowsEnd - NumberOfRowsStart) + 1)] = '\0';
    sscanf(StringBuffer,"%d",&_NumberOfRows);

    //
    //  Parse out the interleaving
    //

    memcpy(StringBuffer,&HeaderBuffer[InterleavingStart],
           (InterleavingEnd - InterleavingStart) + 1);
    StringBuffer[((InterleavingEnd - InterleavingStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"BIL") == 0)
    {
      _Interleaving = XP_GIS_Spot_c::BIL;
    }
    else
    {
      _Interleaving = XP_GIS_Spot_c::UNKNOWN_INTERLEAVING;
    }

    //
    //  Parse out the Band ID Order
    //

    memcpy(StringBuffer,&HeaderBuffer[BandIDOrderStart],
           (BandIDOrderEnd - BandIDOrderStart) + 1);
    StringBuffer[((BandIDOrderEnd - BandIDOrderStart) + 1)] = '\0';
    CharactersRead = 0;
    for (BandNumber = 0;
         BandNumber < _NumberOfBands;
         BandNumber++)
    {
      sscanf(&StringBuffer[CharactersRead],"%s %n",
              TempString,
             &TempInt);
      CharactersRead += TempInt;
      if (strcmp(TempString,"XS1") == 0)
      {
        _BandIDOrder[BandNumber] = XP_GIS_Spot_c::XS1;
      }
      else if (strcmp(TempString,"XS2") == 0)
      {
        _BandIDOrder[BandNumber] = XP_GIS_Spot_c::XS2;
      }
      else if (strcmp(TempString,"XS3") == 0)
      {
        _BandIDOrder[BandNumber] = XP_GIS_Spot_c::XS3;
      }
      else if (strcmp(TempString,"PAN") == 0)
      {
        _BandIDOrder[BandNumber] = XP_GIS_Spot_c::PAN_BAND_ID;
      }
      else
      {
        _BandIDOrder[BandNumber] = XP_GIS_Spot_c::UNKNOWN_BAND_ID;
      }
    }

    //
    //  Parse out the preprocessing level
    //

    memcpy(StringBuffer,&HeaderBuffer[ProcessingLevelStart],
           (ProcessingLevelEnd - ProcessingLevelStart) + 1);
    StringBuffer[((ProcessingLevelEnd - ProcessingLevelStart) + 1)] = '\0';
    sscanf(StringBuffer,"%s",TempString);
    if (strcmp(TempString,"1A") == 0)
    {
      _ProcessingLevel = XP_GIS_Spot_c::ONE_A;
    }
    else if (strcmp(TempString,"1B") == 0)
    {
      _ProcessingLevel = XP_GIS_Spot_c::ONE_B;
    }
    else
    {
      _ProcessingLevel = XP_GIS_Spot_c::UNKNOWN_PROCESSING_LEVEL;
    }

    return XP_GIS_OK;
}

//
//  Method to read the image
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_Spot_c::ReadImage(unsigned long RowStart,
                                       unsigned long ColumnStart,
                                       unsigned long NumberOfRows,
                                       unsigned long NumberOfColumns,
                                       float    XScaleFactor,
                                       float    YScaleFactor,
                                       unsigned char *Buffer)
{
    unsigned long RowNumber;
    unsigned long ColumnNumber;
    unsigned long BandNumber;
    unsigned long RowToRead;
    unsigned short *ColumnToUse;
    unsigned short *ColumnToUsePtr;
    unsigned long BandIndex[3]; 
    unsigned long BufferOffset;
    unsigned long ReturnValue;
    unsigned char *RowBuffer;   // longest row used by SPOT
    float         XPosition;
    float         YPosition;
    unsigned char *RowPtr;

    //
    //  Validate the inputs
    //

    if ((RowStart    + (NumberOfRows    * YScaleFactor) > _NumberOfRows) ||
        (ColumnStart + (NumberOfColumns * XScaleFactor) > _NumberOfColumns))
    {
      return XP_GIS_ERROR;
    }

    if ((XScaleFactor <= 0.0) ||
        (YScaleFactor <= 0.0))
    {
      return XP_GIS_ERROR;
    }

    //
    //  Make sure the file is open
    //

    if (ImageFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Allocate space for local arrays.
    //

    ColumnToUse = (unsigned short*) malloc(sizeof(unsigned short) *
                                           _NumberOfColumns);
    if (ColumnToUse == NULL)
    {
      return XP_GIS_ALLOC_ERROR;
    }

    RowBuffer = (unsigned char*) malloc(sizeof(unsigned char) *
                                            _NumberOfColumns);
    if (RowBuffer == NULL)
    {
      if (ColumnToUse != NULL) free(ColumnToUse);
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Build an array of where we start for each column
    //

    XPosition = (float) ColumnStart;
    for (ColumnNumber = 0;
         ColumnNumber < NumberOfColumns;
         ColumnNumber++)
    {
      XPosition += 1.0f / XScaleFactor;
      ColumnToUse[ColumnNumber] = (unsigned short) XPosition;
    }

    //
    //  Set up the band index based on the number of bands in the image.
    //

    if (_NumberOfBands == 1)
    {
      BandIndex[0] = 0;
    }
    else
    if (_NumberOfBands == 3)
    {
      BandIndex[0] = 2;    // red band
      BandIndex[1] = 1;    // green band
      BandIndex[2] = 0;    // blue band;
    }
    else      //  we only support 1 or 3 band images
    {
      return XP_GIS_ERROR;
    }

    //
    //  Process each row in the output buffer.  Note that we must
    //  move from the last row to the first row, to reverse the
    //  row order.  Also, if we are dealing with a multispectral
    //  image, make sure we load band 3 first (as red), band 2 second
    //  (as green), and band 1 last (as blue).
    //

    OMstatus_check (0, 0, 0);
    YPosition = (float) RowStart;
    RowToRead = RowStart * _NumberOfBands;
    for (RowNumber = 0;
         RowNumber < NumberOfRows;
         RowNumber++)
    {
      for (BandNumber = 0; BandNumber < _NumberOfBands; BandNumber++)
      {
        if ((ReturnValue = _ReadRow(RowToRead, RowBuffer)) != XP_GIS_OK)
        {
          if (RowBuffer   != NULL) free(RowBuffer);
          if (ColumnToUse != NULL) free(ColumnToUse);
          return ReturnValue;
        }
        ColumnToUsePtr = ColumnToUse;
        BufferOffset = (((NumberOfRows - RowNumber - 1) * 
                          NumberOfColumns) * _NumberOfBands) + 
                        BandIndex[BandNumber];
        RowPtr = &Buffer[BufferOffset];
        for (ColumnNumber = 0;
             ColumnNumber < NumberOfColumns;
             ColumnNumber++)
        {
          *RowPtr = RowBuffer[*ColumnToUsePtr++];
          RowPtr += _NumberOfBands;
        }
        RowToRead++;
      }
      YPosition += 1.0f / YScaleFactor;
      RowToRead = ((unsigned long) YPosition) * _NumberOfBands;

      OMstatus_check (int(100*(float(RowNumber+1)/float(NumberOfRows))), 0, 0);
    }
    OMstatus_check (100, 0, 0);

    if (RowBuffer   != NULL) free(RowBuffer);
    if (ColumnToUse != NULL) free(ColumnToUse);
    return XP_GIS_OK;
}

//
//  Method to read a single channel from an image
//  If successfull, reutrns XP_GIS_OK.  Otherwise, returns one of
//  the following:
//
//  XP_GIS_NOT_OPEN
//  XP_GIS_SEEK_ERROR
//  XP_GIS_IO_ERROR
//

unsigned long XP_GIS_Spot_c::ReadBand(unsigned long BandNumber,
                                      unsigned long RowStart,
                                      unsigned long ColumnStart,
                                      unsigned long NumberOfRows,
                                      unsigned long NumberOfColumns,
                                      float    XScaleFactor,
                                      float    YScaleFactor,
                                      unsigned char *Buffer)
{
    unsigned long RowNumber;
    unsigned long ColumnNumber;
    unsigned long RowToRead;
    unsigned short *ColumnToUse;
    unsigned short *ColumnToUsePtr;
    unsigned long ReturnValue;
    unsigned char *RowBuffer;   // longest row used by SPOT
    float         XPosition;
    float         YPosition;
    unsigned char *RowPtr;

    //
    //  Validate the inputs
    //

    if ((RowStart    + (NumberOfRows    * YScaleFactor) > _NumberOfRows) ||
        (ColumnStart + (NumberOfColumns * XScaleFactor) > _NumberOfColumns))
    {
      return XP_GIS_ERROR;
    }

    if ((XScaleFactor <= 0.0) ||
        (YScaleFactor <= 0.0))
    {
      return XP_GIS_ERROR;
    }

    //
    //  Make sure the file is open
    //

    if (ImageFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Allocate space for local arrays.
    //

    ColumnToUse = (unsigned short*) malloc(sizeof(unsigned short) *
                                           NumberOfColumns);
    if (ColumnToUse == NULL)
    {
      return XP_GIS_ALLOC_ERROR;
    }

    RowBuffer = (unsigned char*) malloc(sizeof(unsigned char) *
                                            _NumberOfColumns);
    if (RowBuffer == NULL)
    {
      if (ColumnToUse != NULL) free(ColumnToUse);
      return XP_GIS_ALLOC_ERROR;
    }

    //
    //  Build an array of where we start for each column
    //

    XPosition = (float) ColumnStart;
    for (ColumnNumber = 0;
         ColumnNumber < NumberOfColumns;
         ColumnNumber++)
    {
      XPosition += 1.0f / XScaleFactor;
      ColumnToUse[ColumnNumber] = (unsigned short) XPosition;
    }

    //
    //  Process each row in the output buffer
    //

    YPosition = (float) RowStart;
    RowToRead = (((unsigned long) YPosition) * _NumberOfBands) +
                (BandNumber - 1);
    for (RowNumber = 0;
         RowNumber < NumberOfRows;
         RowNumber++)
    {
      if ((ReturnValue = _ReadRow(RowToRead, RowBuffer)) != XP_GIS_OK)
      {
        if (RowBuffer   != NULL) free(RowBuffer);
        if (ColumnToUse != NULL) free(ColumnToUse);
        return ReturnValue;
      }
      ColumnToUsePtr = ColumnToUse;
      RowPtr = &Buffer[(NumberOfRows - RowNumber - 1) * NumberOfColumns];
      for (ColumnNumber = 0;
           ColumnNumber < NumberOfColumns;
           ColumnNumber++)
      {
        *RowPtr++ = RowBuffer[*ColumnToUsePtr++];
      }
      YPosition += 1.0f / YScaleFactor;
      RowToRead = (((unsigned long) YPosition) * _NumberOfBands) + 
                  (BandNumber - 1);
    }

    if (RowBuffer   != NULL) free(RowBuffer);
    if (ColumnToUse != NULL) free(ColumnToUse);
    return XP_GIS_OK;
}
unsigned long XP_GIS_Spot_c::_ReadRow(unsigned long  RowNumber,
                                      unsigned char *RowBuffer)
{
    unsigned long RowStartPosition;    //  offset to start of row in file.

    //
    //  Make sure the file is open
    //

    if (ImageFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    //
    //  Figure out where the data starts
    //

    if (_SpectralMode == PAN)
    {
      RowStartPosition = PanImageRecordLength        +        // header record
                         (PanImageRecordLength * RowNumber) + // previous rows
                         LineMetadataLength;                  // metadata
    }
    else if (_SpectralMode == XS)
    {
      RowStartPosition = XSImageRecordLength        +        // header record
                         (XSImageRecordLength * RowNumber) + // previous rows
                         LineMetadataLength;                 // metadata
    }
    else
    {
      return XP_GIS_ERROR;       // invalid spectral mode
    }

    //
    //  Seek to the start of the scan line.
    //

    if (!_ImageFileStream.seekg(RowStartPosition,ios::beg))
    {
      return XP_GIS_SEEK_ERROR;
    }

    //
    //  Read the row data.
    //

    if (!_ImageFileStream.read(RowBuffer,
                          _NumberOfColumns))
    {
      if (_ImageFileStream.eof())
      {
        return XP_GIS_EOF;
      }
      else
      {
        return XP_GIS_READ_ERROR;
      }
    }
    if (_ImageFileStream.gcount() != (int) _NumberOfColumns)
    {
      return XP_GIS_IO_ERROR;
    }
    return XP_GIS_OK;
}

double XP_GIS_Spot_c::_ConvertLatitude(const char *LatitudeString) const
{
    short Sign;
    long Degrees;
    long Minutes;
    long Seconds;
    double DecimalDegrees;

    if (LatitudeString == NULL)
    {
      return 0.0;
    }

    Sign = 1;
    if (LatitudeString[0] == 'S')
    {
      Sign = -1;
    }

    sscanf(&LatitudeString[1],"%3d",&Degrees);
    sscanf(&LatitudeString[4],"%2d",&Minutes);
    sscanf(&LatitudeString[6],"%2d",&Seconds);

    DecimalDegrees  = (double) Degrees;
    DecimalDegrees += (double) Minutes / 60.0;
    DecimalDegrees += (double) Seconds / 3600.0;
    DecimalDegrees *= (double) Sign;

    return DecimalDegrees;
}

double XP_GIS_Spot_c::_ConvertLongitude(const char *LongitudeString) const
{
    short Sign;
    long Degrees;
    long Minutes;
    long Seconds;
    double DecimalDegrees;

    if (LongitudeString == NULL)
    {
      return 0.0;
    }

    Sign = 1;
    if (LongitudeString[0] == 'W')
    {
      Sign = -1;
    }

    sscanf(&LongitudeString[1],"%3d",&Degrees);
    sscanf(&LongitudeString[4],"%2d",&Minutes);
    sscanf(&LongitudeString[6],"%2d",&Seconds);

    DecimalDegrees  = (double) Degrees;
    DecimalDegrees += (double) Minutes / 60.0;
    DecimalDegrees += (double) Seconds / 3600.0;
    DecimalDegrees *= (double) Sign;

    return DecimalDegrees;
}

unsigned long XP_GIS_Spot_c::PrintHeader(ostream &PrintStream)
{
    unsigned long BandNumber;

    //
    //  If there is no file open, then there is no header to print.
    //

    if (HeaderFileIsOpen() == XP_GIS_FALSE)
    {
      return XP_GIS_NOT_OPEN;
    }

    PrintStream << "Spot Image File Header";
    if (_HeaderFileName != NULL)
    {
       cout << " for " << _HeaderFileName << endl;
    }
    else
    {
      cout << ":" << endl;
    }
    PrintStream << "  Lower Left Lat/Long   = ";
    PrintStream << _LLLatitude << "  " << _LLLongitude << endl;
    PrintStream << "  Lower Right Lat/Long  = ";
    PrintStream << _LRLatitude << "  " << _LRLongitude << endl;
    PrintStream << "  Upper Left Lat/Long   = ";
    PrintStream << _ULLatitude << "  " << _ULLongitude << endl;
    PrintStream << "  Upper Right Lat/Long  = ";
    PrintStream << _URLatitude << "  " << _URLongitude << endl;
    PrintStream << "  Center Lat/Long       = ";
    PrintStream << _CenterLatitude << " ";
    PrintStream << _CenterLongitude << endl;
    PrintStream << "  Orientation Angle     = " << _OrientationAngle << endl;
    PrintStream << "  Incidence Angle       = " << _IncidenceAngle << endl;
    PrintStream << "  Sun Azimuth Angle     = " << _SunAzimuthAngle << endl;
    PrintStream << "  Sun Elevation Angle   = " << _SunElevationAngle << endl;
    PrintStream << "  Revolution with cycle = " << _RevolutionWithinCycle << endl;
    PrintStream << "  Mirror Step Value     = " << _MirrorStepValue << endl;
    PrintStream << "  Number Of Bands       = " << _NumberOfBands << endl;
    PrintStream << "  Number Of Rows        = " << _NumberOfRows << endl;
    PrintStream << "  Number Of Columns     = " << _NumberOfColumns << endl;
    PrintStream << "  Year                  = " << _Year << endl;
    PrintStream << "  Month                 = " << _Month << endl;
    PrintStream << "  Day                   = " << _Day << endl;
    PrintStream << "  Hour                  = " << _Hour << endl;
    PrintStream << "  Minute                = " << _Minute << endl;
    PrintStream << "  Second                = " << _Second << endl;
    for (BandNumber = 0;
         BandNumber < _NumberOfBands;
         BandNumber++)
    {
      PrintStream << "  Gain for Band " << BandNumber << "       = ";
      PrintStream << _Gain[BandNumber] << endl;
    }
    
    PrintStream << "  Mission               = ";
    switch (_Mission)
    {
      case SPOT1:
        PrintStream << "SPOT1";
        break;
      case SPOT2:
        PrintStream << "SPOT2";
        break;
      case SPOT3:
        PrintStream << "SPOT3";
        break;
      case SPOT4:
        PrintStream << "SPOT4";
        break;
      case SPOT5:
        PrintStream << "SPOT5";
        break;
      case UNKNOWN_MISSION:
        PrintStream << "UNKNOWN_MISSION";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;
    PrintStream << "  Sensor ID             = ";
    switch (_SensorID)
    {
      case HRV1:
        PrintStream << "HRV1";
        break;
      case HRV2:
        PrintStream << "HRV2";
        break;
      case UNKNOWN_SENSOR_ID:
        PrintStream << "UNKNOWN_SENSOR_ID";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;
    PrintStream << "  Coding                = ";
    switch (_Coding)
    {
      case DT:
        PrintStream << "DT";
        break;
      case DPCM:
        PrintStream << "DPCM";
        break;
      case E2:
        PrintStream << "E2";
        break;
      case UNKNOWN_CODING:
        PrintStream << "UNKNOWN_CODING";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;
    PrintStream << "  Spectral Mode         = ";
    switch (_SpectralMode)
    {
      case PAN:
        PrintStream << "PAN";
        break;
      case XS:
        PrintStream << "XS";
        break;
      case UNKNOWN_SPECTRAL_MODE:
        PrintStream << "UNKNOWN_SPECTRAL_MODE";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;
    PrintStream << "  Interleaving          = ";
    switch (_Interleaving)
    {
      case BIL:
        PrintStream << "BIL";
        break;
      case UNKNOWN_INTERLEAVING:
        PrintStream << "UNKNOWN_INTERLEAVING";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;
    for (BandNumber = 0;
         BandNumber < _NumberOfBands;
         BandNumber++)
    {
      PrintStream << "  Band ID for Band " << BandNumber << "    = ";
      switch (_BandIDOrder[BandNumber])
      {
        case XS1:
          PrintStream << "XS1";
          break;
        case XS2:
          PrintStream << "XS2";
          break;
        case XS3:
          PrintStream << "XS3";
          break;
        case PAN_BAND_ID:
          PrintStream << "PAN_BAND_ID";
          break;
        case UNKNOWN_BAND_ID:
          PrintStream << "UNKNOWN_BAND_ID";
          break;
        default:
          PrintStream << "bad value";
      };
      PrintStream << endl;
    }
    PrintStream << "  Processing level      = ";
    switch (_ProcessingLevel)
    {
      case ONE_A:
        PrintStream << "1A";
        break;
      case ONE_B:
        PrintStream << "1B";
        break;
      case UNKNOWN_PROCESSING_LEVEL:
        PrintStream << "UNKNOWN_PROCESSING_LEVEL";
        break;
      default:
        PrintStream << "bad value";
    };
    PrintStream << endl;

    return XP_GIS_OK;
}

// end of file


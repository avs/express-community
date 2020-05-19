#include <stdio.h>
#include <limits.h>
#include "METARSStation.h"
#include "GFC.h"

//
//  Constructor.
//

METARSStation_c::METARSStation_c(const char *LocalFileName)
{

    //
    //  Initialize strings to empty.
    //

    _FileName[0] = '\0';

    //
    //  Initialize file state to closed.
    //

    // Substituted zero for false here
    _FileIsOpen = XP_GIS_FALSE;

    //
    //  If the file name is not null, then use it to
    //  set the object's file name.
    //

    if (LocalFileName != NULL)
    {
      FileName(LocalFileName);
    }

    _NumberOfStations = 0;

}

//
//  destructor.
//

METARSStation_c::~METARSStation_c()
{
    //
    //  Close open file.
    //

    _CloseFile();
}

//
//  Copy constructor.
//

METARSStation_c::METARSStation_c(const METARSStation_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

METARSStation_c &METARSStation_c::operator=(
                      const METARSStation_c &object)
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

unsigned long METARSStation_c::FileName(const char* LocalFileName) 
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


    strcpy(_FileName,LocalFileName);

    _FileStream.open(_FileName,ios::nocreate,filebuf::openprot);
    
//#ifdef MSDOS
//    _FileStream.open(_FileName,ios::binary | ios::nocreate);
//#else
//    _FileStream.open(_FileName,ios::nocreate);
//#endif
    
    _FileIsOpen = XP_GIS_TRUE;


    return XP_GIS_OK;     
}

void METARSStation_c::_CloseFile()
{

    //
    //  Close the file.
    //

    if (_FileIsOpen == XP_GIS_TRUE)
    {

      _FileStream.close();
      _FileIsOpen = XP_GIS_FALSE;
      _FileName[0] = '\0';

    }

    return;
}



// Method to return the number of stations.
int METARSStation_c::NumberOfStations()
{

    return(_NumberOfStations);

}

// This method fills a field array with the information read from the Station
// METARS file.
void METARSStation_c::_FillFields(char* Buffer)
{

  char* PointerString; 

  PointerString = Buffer;
  while (*PointerString != '\0')
  {
    if (*PointerString == ';')
    {
      *PointerString = '\0';
    }
    PointerString++;
  }
  
  PointerString = Buffer;
  for (int i = 0;i < 14;i++)
  {
    _Fields[i] = strdup(PointerString);
    PointerString += strlen(PointerString) + 1;
  }

  

}

// Method to parse the information and store for later use.
unsigned long METARSStation_c::_ParseStations(char* Buffer)
{

    char *Tokens;
    int Index = 0;

    _FillFields(Buffer);
    
    strcpy(_Block,_Fields[Index++]);

    strcpy(_Station,_Fields[Index++]);

    strcpy(_ICAOLocation,_Fields[Index++]);

    _PlaceName = strdup(_Fields[Index++]);
    if (!(_PlaceName))
    {
      return XP_GIS_ERROR;
    }

    strcpy(_State,_Fields[Index++]);

    _Country = strdup(_Fields[Index++]);
    if (!(_Country))
    {
      return XP_GIS_ERROR;
    }

    sscanf(_Fields[Index++],"%d",&_WMORegion);

    strcpy(_StationLatitude,_Fields[Index++]);

    strcpy(_StationLongitude,_Fields[Index++]);

    strcpy(_StationUpperAirLatitude,_Fields[Index++]);

    strcpy(_StationUpperAirLongitude,_Fields[Index++]);

    sscanf(_Fields[Index++],"%f",&_StationElevation);

    sscanf(_Fields[Index++],"%f",&_StationUpperAirElevation);

    strcpy(_RBSN,_Fields[Index++]);

    for (int i = 0;i < 14;i++)
    {
      free(_Fields[i]);
    }

    return XP_GIS_OK;

}

//
// Method to read through the METARS Station file and find all the stations that 
// fall within the Lat and Lon specified.
//

unsigned long METARSStation_c::FindNumberOfStations(
                           double StartLatitude,
                           double StartLongitude,
                           double EndLatitude,
                           double EndLongitude)
{

    char Buffer[1024];
    double LatitudeDecimal;
    double LongitudeDecimal;
    int Degrees;
    int Minutes;
    int Seconds;
    double DoubleDegrees;
    double DoubleMinutes;
    double DoubleSeconds;
    char Direction[2]; 
    int Length; 
    unsigned long ErrReturn;
    char* PointerBuffer;

    _NumberOfStations = 0;

    while (_FileStream.getline(Buffer,1023,'\n'))
    {

      // Parse the each station and store data.
      if ((ErrReturn = _ParseStations(Buffer)) == XP_GIS_ERROR)
      {
        return XP_GIS_ERROR;
      }

      // Check for valid Latitude and convert to Decimal.
      if (_StationLatitude[0] != ' ')
      {
        if ((Length = strlen(_StationLatitude)) == 9)
        {
          sscanf(_StationLatitude,"%2d-%2d-%2d%s",&Degrees,&Minutes,&Seconds,Direction);
        }
        else 
        {
          sscanf(_StationLatitude,"%2d-%2d%s",&Degrees,&Minutes,Direction);
          Seconds = 0;
        }

        DoubleDegrees = (double)Degrees;
        DoubleMinutes = (double)Minutes;
        DoubleSeconds = (double)Seconds;

        if (!(strcmp(Direction,"S")))
        {
          DoubleDegrees *= -1;
        }

        LatitudeDecimal = 
            CGFCMath::ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees(
                                           DoubleDegrees,DoubleMinutes,DoubleSeconds);
      }

      // Check for valid Longitude and convert to Decimal.
      if (_StationLongitude[0] != ' ')
      {
        if ((Length = strlen(_StationLongitude)) == 10)
        {
          sscanf(_StationLongitude,"%3d-%2d-%2d%s",&Degrees,&Minutes,&Seconds,Direction);
        }
        else 
        {
          sscanf(_StationLongitude,"%3d-%2d%s",&Degrees,&Minutes,Direction);
          Seconds = 0;
        }

        DoubleDegrees = (double)Degrees;
        DoubleMinutes = (double)Minutes;
        DoubleSeconds = (double)Seconds;

        if (!(strcmp(Direction,"W")))
        {
          DoubleDegrees *= -1;
        }

        LongitudeDecimal = 
            CGFCMath::ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees(
                                           DoubleDegrees,DoubleMinutes,DoubleSeconds);
      }

      // Check to see if station falls within specified lat lon.
      if (((LongitudeDecimal < 0) && (LongitudeDecimal < StartLongitude) && 
          (LongitudeDecimal > EndLongitude)) || ((LongitudeDecimal >= 0) && 
          (LongitudeDecimal > StartLongitude) && (LongitudeDecimal < EndLongitude)))
      {

        if (((LatitudeDecimal < 0) && (LatitudeDecimal < StartLatitude) && 
            (LatitudeDecimal > EndLatitude)) || ((LatitudeDecimal >= 0) && 
            (LatitudeDecimal > StartLatitude) && (LatitudeDecimal < EndLatitude)))
        {
          if (strcmp(_ICAOLocation,"----"))
          {
          _NumberOfStations++;
          }
        }

      }


    }


    return XP_GIS_OK;

}



//
//  Method to read the METARS station data.  Return the station IDs and the 
//  station cooridnates Longitude, Latitude and Altitude (in meters).
//


unsigned long METARSStation_c::ReadData(
                                   double StartLatitude,
                                   double StartLongitude,
                                   double EndLatitude,
                                   double EndLongitude,
                                   char **StationIDs,
                                   float *OutCoordinates)
                                   
{


    char Buffer[1024];
    double LatitudeDecimal;
    double LongitudeDecimal;
    int Degrees;
    int Minutes;
    int Seconds;
    double DoubleDegrees;
    double DoubleMinutes;
    double DoubleSeconds;
    char Direction[2]; 
    int Length; 
    unsigned long ErrReturn;
    char* PointerBuffer;
    int CoordinateIndex;
    int Count;
    


    char* SaveFileName = strdup(_FileName);
    FileName(SaveFileName);

    CoordinateIndex = 0;
    Count = 0;

    while (_FileStream.getline(Buffer,1023,'\n'))
    {

      if ((ErrReturn = _ParseStations(Buffer)) == XP_GIS_ERROR)
      {
        return XP_GIS_ERROR;
      }

      // Check for valid Latitude and convert to Decimal.
      if (_StationLatitude[0] != ' ')
      {
        if ((Length = strlen(_StationLatitude)) == 9)
        {
          sscanf(_StationLatitude,"%2d-%2d-%2d%s",&Degrees,&Minutes,&Seconds,Direction);
        }
        else 
        {
          sscanf(_StationLatitude,"%2d-%2d%s",&Degrees,&Minutes,Direction);
          Seconds = 0;
        }

        DoubleDegrees = (double)Degrees;
        DoubleMinutes = (double)Minutes;
        DoubleSeconds = (double)Seconds;

        if (!(strcmp(Direction,"S")))
        {
          DoubleDegrees *= -1;
        }

        LatitudeDecimal = 
            CGFCMath::ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees(
                                           DoubleDegrees,DoubleMinutes,DoubleSeconds);
      }

      // Check for valid Longitude and convert to Decimal.
      if (_StationLongitude[0] != ' ')
      {
        if ((Length = strlen(_StationLongitude)) == 10)
        {
          sscanf(_StationLongitude,"%3d-%2d-%2d%s",&Degrees,&Minutes,&Seconds,Direction);
        }
        else 
        {
          sscanf(_StationLongitude,"%3d-%2d%s",&Degrees,&Minutes,Direction);
          Seconds = 0;
        }

        DoubleDegrees = (double)Degrees;
        DoubleMinutes = (double)Minutes;
        DoubleSeconds = (double)Seconds;

        if (!(strcmp(Direction,"W")))
        {
          DoubleDegrees *= -1;
        }

        LongitudeDecimal = 
            CGFCMath::ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees(
                                           DoubleDegrees,DoubleMinutes,DoubleSeconds);
      }

      // Check to see if the station falls within the lat lon and is a valid
      // station id. Store the station ID and the lon, lat, and elevation.
      if (((LongitudeDecimal < 0) && (LongitudeDecimal < StartLongitude) && 
          (LongitudeDecimal > EndLongitude)) || ((LongitudeDecimal >= 0) && 
          (LongitudeDecimal > StartLongitude) && (LongitudeDecimal < EndLongitude)))
      {

        if (((LatitudeDecimal < 0) && (LatitudeDecimal < StartLatitude) && 
            (LatitudeDecimal > EndLatitude)) || ((LatitudeDecimal >= 0) && 
            (LatitudeDecimal > StartLatitude) && (LatitudeDecimal < EndLatitude)))
        {
       
          if (strcmp(_ICAOLocation,"----"))
          {
            strcpy(StationIDs[Count++],_ICAOLocation);
            OutCoordinates[CoordinateIndex++] = LongitudeDecimal;
            OutCoordinates[CoordinateIndex++] = LatitudeDecimal;
            OutCoordinates[CoordinateIndex++] = _StationElevation;
          }

        }

      }

 
    }
    
    return XP_GIS_OK;

}



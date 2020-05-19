#include <stdio.h>
#include <limits.h>
#include "METARSStationChoosen.h"
#include "GFC.h"

//
//  Constructor.
//

METARSStationChoosen_c::METARSStationChoosen_c(const char *LocalFileName,
                                            const char *LocalFileNameChoosen)
{

    //
    //  Initialize strings to empty.
    //

    _FileName[0] = '\0';
    _FileNameChoosen[0] = '\0';

    //
    //  Initialize file state to closed.
    //

    // Substituted zero for false here
    _FileIsOpen = XP_GIS_FALSE;
    _FileIsOpenChoosen = XP_GIS_FALSE;

    //
    //  If the file name is not null, then use it to
    //  set the object's file name.
    //

    if (LocalFileName != NULL)
    {
      FileName(LocalFileName);
    }

    if (LocalFileNameChoosen != NULL)
    {
      FileNameChoosen(LocalFileNameChoosen);
    }

    _NumberOfStations = 0;

}

//
//  destructor.
//

METARSStationChoosen_c::~METARSStationChoosen_c()
{
    //
    //  Close open file.
    //

    _CloseFile();
}

//
//  Copy constructor.
//

METARSStationChoosen_c::METARSStationChoosen_c(const METARSStationChoosen_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

METARSStationChoosen_c &METARSStationChoosen_c::operator=(
                      const METARSStationChoosen_c &object)
{

    //
    //  To set up the object properly, we have to set the
    //  file name via the accessor.  This will set the private
    //  variables and open the files.  Since this object opens the files
    //  read-only, there should be no conflicts with open files.
    //

    _FileIsOpen = XP_GIS_FALSE;
    FileName(object._FileName);
    FileNameChoosen(object._FileNameChoosen);

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

unsigned long METARSStationChoosen_c::FileName(const char* LocalFileName) 
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

//
//  Method to set the file name.  If there is already a 
//  file opened, this method closes the file before attempting
//  to open the newly specified file.  Returns XP_GIS_OK if successful.
//  Otherwise, returns one of the following:
//
//    XP_GIS_OPEN_ERROR;
//

unsigned long METARSStationChoosen_c::FileNameChoosen(
                                         const char* LocalFileNameChoosen) 
{
    unsigned long ReturnValue;


    //
    //  If a null is passed in, then close any open files.
    //

    if (LocalFileNameChoosen == NULL)
    {
      _CloseFileChoosen();
      return XP_GIS_OK;
    }

    //
    //  If there is already a file open, close it.
    //

    if (_FileNameChoosen[0] != '\0')
    {
      _CloseFileChoosen();
    }


    strcpy(_FileNameChoosen,LocalFileNameChoosen);

    _FileStreamChoosen.open(_FileNameChoosen,ios::nocreate,filebuf::openprot);
    
//#ifdef MSDOS
//    _FileStream.open(_FileName,ios::binary | ios::nocreate);
//#else
//    _FileStream.open(_FileName,ios::nocreate);
//#endif
    
    _FileIsOpenChoosen = XP_GIS_TRUE;


    return XP_GIS_OK;     
}

void METARSStationChoosen_c::_CloseFile()
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

void METARSStationChoosen_c::_CloseFileChoosen()
{

    //
    //  Close the file.
    //

    if (_FileIsOpenChoosen == XP_GIS_TRUE)
    {

      _FileStreamChoosen.close();
      _FileIsOpenChoosen = XP_GIS_FALSE;
      _FileNameChoosen[0] = '\0';

    }

    return;
}




// Method to return the number of stations.
int METARSStationChoosen_c::NumberOfStations()
{

    return(_NumberOfStations);

}

// This method fills a field array with the information read from the Station
// METARS file.
void METARSStationChoosen_c::_FillFields(char* Buffer)
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
unsigned long METARSStationChoosen_c::_ParseStations(char* Buffer)
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

unsigned long METARSStationChoosen_c::FindNumberOfStations(void )
{

    char Buffer[1024];
    char BufferOne[1024];
    unsigned long ErrReturn;
    char *StationID;
    char *FoundStationTable;
    char DoneFlag;
    int TotalCount;
    int Count;

    TotalCount = 0;
    while (_FileStreamChoosen.getline(BufferOne,1023,'\n'))
    {
      TotalCount++; 
    }
    _FileStreamChoosen.close();
    _FileStreamChoosen.open(_FileNameChoosen,ios::nocreate,filebuf::openprot);

    FoundStationTable = (char *) malloc(sizeof(char) * TotalCount); 
    if (!(FoundStationTable))
    {
      return XP_GIS_ERROR;
    }
    for (Count = 0;Count < TotalCount;Count++)
    {
      FoundStationTable[Count] = XP_GIS_FALSE;
    }

    DoneFlag = XP_GIS_FALSE;
    _NumberOfStations = 0;

    while ((_FileStream.getline(Buffer,1023,'\n')) && (!(DoneFlag)))
    {

      // Parse each station and store data.
      if ((ErrReturn = _ParseStations(Buffer)) == XP_GIS_ERROR)
      {
        return XP_GIS_ERROR;
      }

      Count = 0;

      while (_FileStreamChoosen.getline(BufferOne,1023,'\n'))
      {

        if (FoundStationTable[Count] == XP_GIS_FALSE)
        {
          StationID = strtok(BufferOne,";");
 
          if (strcmp(_ICAOLocation,StationID))
          {
            if (strcmp(_ICAOLocation,"----"))
            {
              FoundStationTable[Count] = XP_GIS_TRUE;
              _NumberOfStations++;
            }

          }
        }
        Count++;
      }
      DoneFlag = XP_GIS_TRUE;
      for (Count = 0;Count < _NumberOfStations;Count++)
      {
        if (FoundStationTable[Count] == XP_GIS_FALSE)
        {
          DoneFlag = XP_GIS_FALSE;  
        }
      }
      _FileStreamChoosen.close();
      _FileStreamChoosen.open(_FileNameChoosen,ios::nocreate,filebuf::openprot);
    }

    _FileStream.close();
    _FileStreamChoosen.close();

    free(FoundStationTable);

    return XP_GIS_OK;

}



//
//  Method to read the METARS station data.  Return the station IDs and the 
//  station cooridnates Longitude, Latitude and Altitude (in meters).
//


unsigned long METARSStationChoosen_c::ReadData(
                                   char **StationIDs,
                                   char **StationNames,
                                   float *OutCoordinates)
                                   
{


    char Buffer[1024];
    char BufferOne[1024];
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
    int CoordinateIndex;
    char* InStationID;
    char* FoundInStation;
    char LocalStationID[5];
    char DoneFlag;
    char *FoundStationTable;
    int Count;
    int StationCount;
    int StationIndex;
    

    FoundStationTable = (char *) malloc(sizeof(char) * _NumberOfStations); 
    if (!(FoundStationTable))
    {
      return XP_GIS_ERROR;
    }
    for (Count = 0;Count < _NumberOfStations;Count++)
    {
      FoundStationTable[Count] = XP_GIS_FALSE;
    }
  
    _FileStream.open(_FileName,ios::nocreate,filebuf::openprot);
    _FileStreamChoosen.open(_FileNameChoosen,ios::nocreate,filebuf::openprot);

    StationIndex = 0;
    CoordinateIndex = 0;
    DoneFlag = XP_GIS_FALSE;

    while  ((_FileStream.getline(Buffer,1023,'\n')) && (!(DoneFlag)))
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

      StationCount = 0;
      while (_FileStreamChoosen.getline(BufferOne,1023,'\n'))
      {

        if (FoundStationTable[StationCount] == XP_GIS_FALSE)
        {            
            
          
          InStationID = strtok(BufferOne,";");
          strcpy(LocalStationID,InStationID);

          if (!(strcmp(_ICAOLocation,LocalStationID)))
          {
         

    
            // Alocating space for the station ids.
            StationIDs[StationIndex] = strdup(InStationID);
  
            InStationID = strtok(NULL,";");
  
            // Alocating space for the station names.
            StationNames[StationIndex++] = strdup(InStationID);
      
            // Check to see if the station falls within the lat lon and is a 
            // valid station id. Store the station ID and the lon, lat, and 
            // elevation.
            FoundStationTable[StationCount] = XP_GIS_TRUE;
            OutCoordinates[CoordinateIndex++] = LongitudeDecimal;
            OutCoordinates[CoordinateIndex++] = LatitudeDecimal;
            OutCoordinates[CoordinateIndex++] = _StationElevation;

          }
        }
        StationCount++;
      }
      DoneFlag = XP_GIS_TRUE;
      for (Count = 0;Count < _NumberOfStations;Count++)
      {
        if (FoundStationTable[Count] == XP_GIS_FALSE)
        {
          DoneFlag = XP_GIS_FALSE;  
        }
      }
      _FileStreamChoosen.close();
      _FileStreamChoosen.open(_FileNameChoosen,ios::nocreate,filebuf::openprot);
    }
 
    free(FoundStationTable);
    
    return XP_GIS_OK;

}



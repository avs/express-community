#include <stdio.h>
#include "METARS.h"

#define DEBUG

extern "C" int ReturnPtrDcdMETAR( char *inputstring,char *InternalStationID,float *InternalTemperature,float *InternalPressure);


//
//  Constructor.
//

METARS_c::METARS_c(const char *LocalFileName)
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

}

//
//  destructor.
//

METARS_c::~METARS_c()
{
    //
    //  Close open file.
    //

    _CloseFile();
}

//
//  Copy constructor.
//

METARS_c::METARS_c(const METARS_c &object)
{
    *this = object;
}

//
//  Assignment operator.
//

METARS_c &METARS_c::operator=(
                      const METARS_c &object)
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

unsigned long METARS_c::FileName(const char* LocalFileName) 
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

void METARS_c::_CloseFile()
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





//
//  Method to read the METARS data.  This method reads a line of METARS data and
//  passes it off to the METARS decoder. The station ids that it needs to match
//  are passed in along with places to store the Temperature and Pressure for
//  each station.  The METARS files are not in any order for stations and the 
//  station may not have reported for that hour so fill with NULL values.  
//
//  If successfull, reutrns XP_GIS_OK.  
//
//

unsigned long METARS_c::ReadData(char** InternalStationID,
                                 float *OutNodeDataTemperature,
                                 float *OutNodeDataPressure)
{


    char inputbuffer[2048];
    char totalbuffer[2048];
    char StationID[5];
    int ErrReturn;
    char DoneFlag;
    float InternalTemperature;
    float InternalPressure;
    char *SaveFilename = NULL;
    char *FoundTable = NULL;
    int Count;

    FoundTable = (char *)malloc(sizeof(char) * _NumberOfStations);
    if (!(FoundTable))
    {
      return XP_GIS_ERROR;
    }

    SaveFilename = strdup(_FileName);
    if (!(SaveFilename))
    {
      return XP_GIS_ERROR;
    }


    for (Count = 0;Count < _NumberOfStations;Count++)
    {
      FoundTable[Count] = XP_GIS_FALSE;
    }

    DoneFlag = XP_GIS_FALSE;
    while ((_FileStream.getline(inputbuffer,2047,'\n')) && (!(DoneFlag)))
    {

      totalbuffer[0] = '\0';
      while ((_FileStream.getline(inputbuffer,2047,'\n')) && 
                                             (strcmp(inputbuffer,"")))
      {

        strcat(totalbuffer,inputbuffer);
  
      }

      ErrReturn = ReturnPtrDcdMETAR(totalbuffer,StationID,
                       &InternalTemperature,&InternalPressure);

//#ifdef DEBUG
//      fprintf(stderr,"ReturnPtrDcdMETAR - StationID: %s Error: %d\n",StationID,ErrReturn);
//#endif

      // This Error comes about when a station can't be decoded.
      if (ErrReturn == 12)
      {
        // fprintf(stderr,"Error %d =\n",ErrReturn);
      }
      else if (ErrReturn != 0)
      {
        return XP_GIS_ERROR;
      }
      else
      {

        DoneFlag = XP_GIS_TRUE;
        for (Count = 0;Count < _NumberOfStations;Count++)
        {
          if (!(FoundTable[Count]))
          {
            DoneFlag = XP_GIS_FALSE;
     
            // When set the station has been found so set the Temperature and
            // Pressure for that station.
            if (!(strcmp(StationID,InternalStationID[Count])))
            {
#ifdef DEBUG
      fprintf(stderr,"ReturnPtrDcdMETAR - StationID: %s Temperature: %f Pressure: %f\n",StationID,InternalTemperature,InternalPressure);
#endif
              FoundTable[Count] = XP_GIS_TRUE;
              OutNodeDataTemperature[Count] = InternalTemperature;
              OutNodeDataPressure[Count] = InternalPressure;
            }
            else
            {
              OutNodeDataTemperature[Count] = 500.0;
              OutNodeDataPressure[Count] = 500.0;
            }
          }
        }
      }
    }
   
    return XP_GIS_OK;

}


// Method to set the number of stations.
void METARS_c::SetNumberOfStations(int InNumberOfStations)
{

  _NumberOfStations = InNumberOfStations;

}

// Method to return the number of stations.
int METARS_c::NumberOfStations(void)
{

  return _NumberOfStations;

}

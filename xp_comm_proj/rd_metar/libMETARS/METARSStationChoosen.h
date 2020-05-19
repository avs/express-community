//
//  Include file for metars for reading Metars files.
//

/*

    Public Constructors

      METARSStationChoosen_c(const char *FileName=NULL,
                             const char *FileNameChoosen=NULL);
        The constructor.

      virtual ~METARSStationChoosen_c();
        the destructor.

      METARSStationChoosen_c(const METARSStationChoosen_c &object);
        The copy constructor


    Public Operators

      METARSStationChoosen_c &operator=(const METARSStationChoosen_c &object);
        The assignment operator assigns all values as necessary.  It
        is implemented by invoking the FileName method.  Therefore, "this"
        closes any open files, opens the files in object, and sets up all
        its private properties.  Since this object opens all files read-only,
        there is no chance for conflict between open files.

    Public Methods

      const char *FileName(const char* FileName);
        Method to set the file name.  If there is already a file opened,
        this method closes the file before attempting to open the newly
        specified file.  Returns NULL on failure.  Otherwise returns the
        named of the newly opened file.

*/



#ifndef METARSSTATION_H
#define METARSSTATION_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "gserror.h"

#ifndef XP_GIS_TRUE
#define XP_GIS_TRUE 1
#endif

#ifndef XP_GIS_FALSE
#define XP_GIS_FALSE 0
#endif

// enum {VERSION=0,MAJOR,MINOR};


class METARSStationChoosen_c
{
  public:
    METARSStationChoosen_c(const char *FileName=NULL,
                           const char *FileNameChoosen=NULL);

    virtual ~METARSStationChoosen_c();
    METARSStationChoosen_c(const METARSStationChoosen_c &object);
    METARSStationChoosen_c &operator=(const METARSStationChoosen_c &object);

    unsigned long FileName(const char* LocalFileName);
    unsigned long FileNameChoosen(const char* LocalFileNameChoosen);

    const char     *FileName() const {return _FileName;}
 
    unsigned long ReadData(char **StationIDs,
                           char **StationNames,
                           float *OutCoordinates);
                           


    unsigned long FindNumberOfStations(void );

    int NumberOfStations(void);

  private:

    char _Block[3];
    char _Station[4];
    char _ICAOLocation[5];
    char* _PlaceName;           // Must be allocated
    char _State[3];
    char* _Country;             // Must be allocated
    int _WMORegion;
    char _StationLatitude[10];
    char _StationLongitude[11];
    char _StationUpperAirLatitude[10];
    char _StationUpperAirLongitude[11];
    float _StationElevation;
    float _StationUpperAirElevation;    
    char _RBSN[2];

    char* _Fields[14];
    
    void _FillFields(char* Buffer);
    unsigned long _ParseStations(char* Buffer);

    void _CloseFile();
    void _CloseFileChoosen();

    ifstream _FileStream;
    ifstream _FileStreamChoosen;

    char _FileName[1024];      // fully qualified file path
    char _FileNameChoosen[1024];      // fully qualified file path

    unsigned char _FileIsOpen;       // is the file open (T/F)
    unsigned char _FileIsOpenChoosen;       // is the file open (T/F)

    int _NumberOfStations;

};


#endif   // METARSSTATION_H


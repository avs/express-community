//
//  Include file for metars for reading Metars files.
//

/*

    Public Constructors

      METARSStation_c(const char *FileName=NULL);;
        The constructor.

      virtual ~METARSStation_c();
        the destructor.

      METARSStation_c(const METARSStation_c &object);
        The copy constructor


    Public Operators

      METARSStation_c &operator=(const METARSStation_c &object);
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


class METARSStation_c
{
  public:
    METARSStation_c(const char *FileName=NULL);
    virtual ~METARSStation_c();
    METARSStation_c(const METARSStation_c &object);
    METARSStation_c &operator=(const METARSStation_c &object);

    unsigned long   FileName(const char* LocalFileName);
    const char     *FileName() const {return _FileName;}
 
    unsigned long ReadData(double StartLatitude,
                           double StartLongitude,
                           double EndLatitude,
                           double EndLongitude,
                           char **StationIDs,
                           float *OutCoordinates);
                           


    unsigned long FindNumberOfStations(
                           double StartLatitude,
                           double StartLongitude,
                           double EndLatitude,
                           double EndLongitude);

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

    ifstream _FileStream;

    int _NumberOfStations;

    char _FileName[1024];      // fully qualified file path
    unsigned char _FileIsOpen;       // is the file open (T/F)
};


#endif   // METARSSTATION_H


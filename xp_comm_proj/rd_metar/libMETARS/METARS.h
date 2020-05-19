//
//  Include file for metars for reading Metars files.
//

/*

    Public Constructors

      METARS_c(const char *FileName=NULL);
        The constructor.

      virtual ~METARS_c();
        the destructor.

      METARS_c(const METARS_c &object);
        The copy constructor


    Public Operators

      METARS_c &operator=(const METARS_c &object);
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

    unsigned long ReadData(char** InternalStationID,
                           float *OutNodeDataTemperature,
                           float *OutNodeDatapressure);
      This method reads the data for the stations in the array and returns
      temperature and pressure for each station. 

    void SetNumberOfStations(int InNumberOfStations);
      This method sets the number of stations.

    int NumberOfStations(void);  
      This method returns the number of stations.

*/



#ifndef METARS_H
#define METARS_H

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


class METARS_c
{
  public:
    METARS_c(const char *FileName=NULL);
    virtual ~METARS_c();
    METARS_c(const METARS_c &object);
    METARS_c &operator=(const METARS_c &object);

    unsigned long   FileName(const char* LocalFileName);
    const char     *FileName() const {return _FileName;}

    unsigned long ReadData(char** InternalStationID,
                           float *out_node_data_temp,
                           float *out_node_data_pressure);

    void SetNumberOfStations(int InNumberOfStations);
    int NumberOfStations(void);

  private:


    int _NumberOfStations;
    void _CloseFile();

    ifstream _FileStream;

    char _FileName[1024];      // fully qualified file path
    unsigned char _FileIsOpen;       // is the file open (T/F)
};

#endif   // METARS_H


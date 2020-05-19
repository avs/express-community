
#include "avs/err.h"
#include "avs/gd_def.h"
#include "gen.h"

#define ERROR_MSG(a) ERRerror("read_Station::read", 1, ERR_ORIG, a)
	

int
ReadStations_ReadStatMods_read_Stations::read(OMevent_mask event_mask, int seq_num)
{
   // filename (OMXstr read req notify)
   // StartLatitude (OMXfloat read req notify)
   // StartLongitude (OMXfloat read req notify)
   // EndLatitude (OMXfloat read req notify)
   // EndLongitude (OMXfloat read req notify)
   // StationLocation (Mesh write)


    unsigned long ReturnValue;
    METARSStation_c *MetarsStationObject;

    char** LocalStationIDs;
    float* LocalStationPositions;
    int Count;

    int Interrupt;

    /***********************/
    /* Function's Body     */
    /***********************/

    OMstatus_check( 20, "METARS: Stations", &Interrupt );

    char * lookat = (char *)filename;

    // create an METARS object
    MetarsStationObject = new METARSStation_c((char *)filename);

    if (MetarsStationObject == NULL)
    {
      ERROR_MSG("read_Station: Error creating MetarsStationObject \n"); 
      delete MetarsStationObject;
      return OM_STAT_FAILURE;
    }

    if( filename.changed(seq_num) && (char *)filename &&
		    (strlen((char *)filename) > 0) )
    {
        MetarsStationObject->FileName((char *)filename);
    }

    

    float LocalStartLatitude = (float)StartLatitude;
    float LocalStartLongitude = (float)StartLongitude;
    float LocalEndLatitude = (float)EndLatitude;
    float LocalEndLongitude = (float)EndLongitude;

    ReturnValue = MetarsStationObject->FindNumberOfStations(
                     (double)LocalStartLatitude,
                     (double)LocalStartLongitude,
                     (double)LocalEndLatitude,
                     (double)LocalEndLongitude);

    NumberOfStations = MetarsStationObject->NumberOfStations(); 

    if ((int)NumberOfStations <= 0)
    {
      fprintf(stderr,"No Stations found \n");
      delete MetarsStationObject;
      return OM_STAT_FAILURE;
    }

    StationLocation.nnodes = (int)NumberOfStations;

    LocalStationPositions = 
       (float *)StationLocation.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

    if (!(LocalStationPositions))
    {
      ERROR_MSG("can't get coordinates values array for write" );
      delete MetarsStationObject;
      return OM_STAT_FAILURE;
    }

    LocalStationIDs = (char **) malloc(sizeof(char *) * 
                                   MetarsStationObject->NumberOfStations()); 
    for (Count = 0;Count < (int)NumberOfStations;Count++)
    {
      LocalStationIDs[Count] = (char *) malloc(sizeof(char) * 5);
    }

    OMstatus_check( 40, "METARS: Stations", &Interrupt );

    ReturnValue = MetarsStationObject->ReadData(
                               (double)LocalStartLatitude,
                               (double)LocalStartLongitude,
                               (double)LocalEndLatitude,
                               (double)LocalEndLongitude,
                               LocalStationIDs,LocalStationPositions);

    for (Count = 0;Count < (int)NumberOfStations;Count++)
    {
      StationID.set_str_array_val(Count,LocalStationIDs[Count]); 
    }


    delete MetarsStationObject;
    ARRfree((char *)LocalStationPositions);

    OMstatus_check( 90, "METARS: Stations", &Interrupt );

    // return 1 for success
    return OM_STAT_SUCCESS;
}


#include "src\ISWrapperMod_gen.hxx"

#include "isense.h"

ISD_TRACKER_HANDLE handle1;
BOOL verbose = FALSE;

int ISWrapperMod::OpenDevice(OMevent_mask event_mask, int seq_num)
{

	/***********************/
	/* Function's Body     */
	/***********************/

	handle1 = ISLIB_OpenTracker( NULL, 0, FALSE, verbose );   
	if (handle1) {
		DeviceState = (int)1;
	}
	else {
		DeviceState = (int)0;
	}

	printf("I'm in method: ISWrapperMod::OpenDevice, %d\n", (int)DeviceState);
	// return 1 for success
	return(1);
}

int ISWrapperMod::CloseDevice(OMevent_mask event_mask, int seq_num)
{

	/***********************/
	/* Function's Body     */
	/***********************/

	ISLIB_CloseTracker( handle1 );

	DeviceState = (int)0;

	printf("I'm in method: ISWrapperMod::CloseDevice, %d\n", (int)DeviceState);

	// return 1 for success
	return(1);
}

int ISWrapperMod::ShowStats(OMevent_mask event_mask, int seq_num)
{

	/***********************/
	/* Function's Body     */
	/***********************/
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ISWrapperMod::ShowStats\n");

	ISD_TRACKER_TYPE Tracker;
	ISD_STATION_CONFIG_TYPE Station;
	WORD i, numStations = 4;
	char buf[20];

	char   *systemType[3] = {"Unknown", "IS Precision Series", "InterTrax Series"};
	char   *modelName[5]  = {"Unknown", "IS-300 Series", "IS-600 Series", "IS-900 Series", "InterTrax 30"};

	if(ISLIB_GetTrackerState( handle1, &Tracker, TRUE ))
	{
		printf("\n********** InterSense Tracker Information ***********\n\n");

		printf("Type:     %s device on port %d\n", systemType[Tracker.TrackerType], Tracker.CommPort);
		printf("Model:    %s\n", modelName[Tracker.TrackerModel]);

		if(Tracker.TrackerType == ISD_PRECISION_SERIES)
		{
			if( Tracker.TrackerModel == ISD_IS600 || Tracker.TrackerModel == ISD_IS900)
			{
				numStations = ISD_MAX_STATIONS;
			}
			printf("\nStation\tTime\tState\tCube  Enhancement  Sensitivity  Prediction\n");

			for(i = 1; i <= numStations; i++)
			{
                printf("%d\t", i);

                if(ISLIB_GetStationState( handle1, &Station, i, FALSE ))
                {
                    printf("%s\t%s\t%s\t   %u\t\t%u\t   %u\n", 
                        Station.TimeStamped ? "ON" : "OFF", 
                        Station.State ? "ON" : "OFF", 
                        Station.InertiaCube == -1 ? "None" : itoa(Station.InertiaCube, buf, 10), 
                        Station.Enhancement, 
                        Station.Sensitivity, 
                        Station.Prediction);
                }
                else
                {
                    printf("ISLIB_GetStationState failed\n");
                    break;
                }
            }
            printf("\n");
        }
    }
    else
    {
        printf("ISLIB_GetTrackerState failed\n");
    }

	// return 1 for success
   return(1);
}

int ISWrapperMod::UpdateXform(OMevent_mask event_mask, int seq_num)
{
	// Xrot (OMXfloat write)
	// Yrot (OMXfloat write)
	// Zrot (OMXfloat write)
	// Xtrans (OMXfloat write)
	// Ytrans (OMXfloat write)
	// Ztrans (OMXfloat write)

	/***********************/
	/* Function's Body     */
	/***********************/

	ISD_DATA_TYPE            data1;
	ISD_STATION_CONFIG_TYPE  Station;
	ISD_TRACKER_TYPE         Tracker;

	int done = FALSE, station = 1;
	BOOL verbose = FALSE;

	/* Detect first tracker. If you have more than one InterSense device and would like to have
	a specific tracker, connected to a known port, initialized first, then enter the port 
	number instead of 0. Otherwise, tracker connected to the port with lower number is
	found first */

	if (DeviceState == 1) {
		ISLIB_GetTrackerData( handle1, &data1 );

		/* Get comm port statistics for display with tracker data */
		if(ISLIB_GetCommState( handle1, &Tracker ))
		{
			printf("%5.2fKbps %d Records/s %7.3f %7.3f %7.3f %7.2f %7.2f %7.2f\n", 
			Tracker.KBitsPerSec, Tracker.RecordsPerSec, 
			data1.Station[station-1].Position[0], 
			data1.Station[station-1].Position[1], 
			data1.Station[station-1].Position[2], 
			data1.Station[station-1].Orientation[0], 
			data1.Station[station-1].Orientation[1], 
			data1.Station[station-1].Orientation[2]);

			Xtrans = (float)data1.Station[station-1].Position[0];
			Ytrans = (float)data1.Station[station-1].Position[1];
			Ztrans = (float)data1.Station[station-1].Position[2];
			Xrot = (float)data1.Station[station-1].Orientation[0];
			Yrot = (float)data1.Station[station-1].Orientation[1];
			Zrot = (float)data1.Station[station-1].Orientation[2];
		}
	}

	// return 1 for success
	return(1);
}

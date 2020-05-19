/***********************************************************************
*
*       File Name:      isense.h
*       Description:    Header File for InterSense Tracker Driver
*       Created:        12/4/98
*       Author:         Yury Altshuler
*
*       Copyright:      InterSense 1998 - All rights Reserved.
*
*       Comments:       This dynamic link library is provided to simplify 
*                       communications with all models of InterSense tracking devices. 
*                       Currently, these include IS-300, IS-600, IS-900 and InterTrax.
*                       It can detect, configure, and get data from up to 4 trackers.
*
*                       DLL provides two methods of configuring the IS series trackers.
*                       You can use the provided function calls and hardcode the settings in  
*                       your application, or use the isenseX.ini files. Second method is
*                       strongly recommended, as it provides you with ability to change the
*                       configuration without recompiling your application. In the isenseX.ini
*                       file name X is a number, starting at 1, identifying one tracking system  
*                       in the order of initialization. Use included isense1.ini file as example. 
*                       Only enter the settings you want the program to change.
*
*                       InterTrax requires no configuration. All function calls and settings
*                       in the isenseX.ini files are ignored.
*                       
*                       
*                       Items not implemented in this release:

*                           ISLIB_OpenConfigInterface
*                           infoScreen
*
*
*                       
************************************************************************/
#ifndef _ISD_isenseh
#define _ISD_isenseh

#include <windows.h>

/* tracking system type */
#define ISD_NONE               0    /* none found, or can't identify */
#define ISD_PRECISION_SERIES   1    /* IS-300, IS-600 and IS-900 */
#define ISD_INTERTRAX_SERIES   2    /* InterTrax */

/* tracking system model */
#define ISD_UNKNOWN     0    
#define ISD_IS300       1    /* 3DOF system */
#define ISD_IS600       2    /* 6DOF system */
#define ISD_IS900       3    /* 6DOF system */
#define ISD_INTERTRAX   4    /* InterTrax */

/* for now limited to 8 */
#define ISD_MAX_STATIONS       8

/* assume no more than 4 rs232 ports */
#define ISD_MAX_TRACKERS       4 

/* number of supported joystick or stylus buttons */
#define ISD_MAX_BUTTONS        8

/* hardware is limited to 10 analog channels per station */
#define ISD_MAX_CHANNELS       10

/* orientation format */
#define ISD_EULER              1
#define ISD_QUATERNION         2


typedef int ISD_TRACKER_HANDLE;


typedef struct
{
    /* Following item are for information only and should not be changed */

    WORD   TrackerType;    /* IS Precision series or InterTrax. 
                              TrackerType can be: 
                              ISD_PRECISION_SERIES for IS-300, IS-600 and IS-900 model tracker, 
                              ISD_INTERTRAX_SERIES for InterTrax, or 
                              ISD_NONE if tracker is not initialized */

    WORD   TrackerModel;   /* ISD_UNKNOWN, ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX */
    
    WORD   CommPort;       /* Number of the rs232 port. Starts with 1. */

    /* Communications statistics. For information only. */

    WORD   RecordsPerSec;
    float  KBitsPerSec;    

    /* Following items are used to configure the tracker and can be set in
       the isenseX.ini file */

    WORD   SyncState;   /* 4 states: 0 - OFF, system is in free run 
                                     1 - ON, hardware genlock frequency is automatically determined
                                     2 - ON, hardware genlock frequency is specified by the user
                                     3 - ON, no hardware sygnal, lock to the user specified frequency */ 

    float  SyncRate;    /* Sync frequency - number of hardware sync signals per second, 
                           or, if SyncState is 3 - data record output frequency */

    WORD   SyncPhase;   

    WORD   wReserved1;  /* reserved for future use */
    WORD   wReserved2;
    float  fReserved1;
    float  fReserved2;

} ISD_TRACKER_TYPE;



/* ISD_STATION_CONFIG_TYPE can only be used with IS Precision Series tracking devices.
   If passed to ISD_SetStationState or ISD_GetStationState with InterTrax, FALSE is returned. */

typedef struct
{
    WORD    ID;             /* unique number identifying a station. It is the same as that 
                               passed to the ISD_SetStationState and ISD_GetStationState   
                               functions and can be 1 to ISD_MAX_STATIONS */

    BOOL    State;          /* TRUE if ON, FALSE if OFF */

    BOOL    Compass;        /* 0, 1 or 2 for OFF, PARTIAL and FULL. Older versions of tracker 
                               firmware supported only 0 and 1, which stood for ON or OFF. Please
                               use the new notation. This API will correctly interpret the settings.
                               Compass setting is ignored if station is configured for 
                               Fusion Mode operation. */

    LONG    InertiaCube;    /* InertiaCube associated with this station. If no InertiaCube is
                               assigned, this number is -1. Otherwise, it is a positive number
                               1 to 4 */

    WORD    Enhancement;    /* levels 0, 1, or 2 */
    WORD    Sensitivity;    /* levels 1 to 4 */
    WORD    Prediction;     /* 0 to 50 ms */
    WORD    AngleFormat;    /* ISD_EULER or ISD_QUATERNION */
    BOOL    TimeStamped;    /* TRUE if time stamp is requested */
    BOOL    GetButtons;     /* TRUE if joystick of stylus button state is requested */

    WORD    wReserved1;     /* reserved for future use */
    WORD    wReserved2;
    BOOL    bReserved1;
    BOOL    bReserved2;

} ISD_STATION_CONFIG_TYPE;


typedef struct
{
    float Orientation[4];   /* Supports both Euler and Quaternion formats */
    float Position[3];      /* Always in meters */
    float TimeStamp;        /* Seconds, reported only if requested */
    BOOL  ButtonState[ISD_MAX_BUTTONS];    /* Only if requested */

    /* Current hardware is limited to 10 channels, only 2 are used. 
       The only device using this is the IS-900 wand that has a built-in
       analog joystick. Channel 1 is x-axis rotation, channel 2 is y-axis
       rotation */

    short  AnalogData[ISD_MAX_CHANNELS]; 

} ISD_STATION_DATA_TYPE;


typedef struct
{
    ISD_STATION_DATA_TYPE Station[ISD_MAX_STATIONS];

} ISD_DATA_TYPE;


/* Returns -1 on failure. To detect tracker automatically specify 0 for commPort.
   hParent parameter to ISLIB_OpenTracker is optional and should only be used if 
   information screen or tracker configuration tools are to be used when available 
   in the future releases. If you would like a tracker initialization window to be 
   displayed, specify TRUE value for the infoScreen parameter. */

ISD_TRACKER_HANDLE ISLIB_OpenTracker( HWND hParent, DWORD commPort, 
                                      BOOL infoScreen, BOOL verbose );


/* This function call deinitializes the tracker, closes communications port and 
   frees the resources associated with this tracker. If 0 is passed, all currently
   open trackers are closed. When last tracker is closed, program frees the DLL. */

BOOL  ISLIB_CloseTracker( ISD_TRACKER_HANDLE );


/* Get general tracker information, such as type, model, port, etc.
   Also retrieves genlock synchronization configuration, if available. 
   See ISD_TRACKER_TYPE structure definition above for complete list of items */

BOOL  ISLIB_GetTrackerState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, BOOL verbose );


/* When used with IS Precision Series (IS-300, IS-600, IS-900) tracking devices 
   this function call will set genlock synchronization  parameters, all other fields 
   in the ISD_TRACKER_TYPE structure are for information purposes only */

BOOL  ISLIB_SetTrackerState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, BOOL verbose );


/* Get RecordsPerSec and KBitsPerSec without requesting genlock settings from the tracker.
   Use this instead of ISD_GetTrackerState to prevent your program from stalling while
   waiting for the tracker response. */

BOOL  ISLIB_GetCommState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE * );


/* Configure station as specified in the ISD_STATION_CONFIG_TYPE structure. Before 
   this function is called, all elements of the structure must be assigned a value. 
   stationNum is a number from 1 to ISD_MAX_STATIONS. Should only be used with
   IS Precision Series tracking devices, not valid for InterTrax.  */

BOOL  ISLIB_SetStationState( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                             WORD stationNum, BOOL verbose );


/* Fills the ISD_STATION_CONFIG_TYPE structure with current settings. Function
   requests configuration records from the tracker and waits for the response.
   If communications are interrupted, it will stall for several seconds while 
   attempting to recover the settings. Should only be used with IS Precision Series 
   tracking devices, not valid for InterTrax.
   stationNum is a number from 1 to ISD_MAX_STATIONS */

BOOL  ISLIB_GetStationState( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                             WORD stationNum, BOOL verbose );


/* Get data from all configured stations. Data is places in the ISD_DATA_TYPE
   structure. Orientation array may contain Euler angles or Quaternions, depending
   on the settings of the AngleFormat field of the ISD_STATION_CONFIG_TYPE structure.
   TimeStamp is only available if requested by setting TimeStamped field to TRUE. */

BOOL  ISLIB_GetTrackerData( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );


/* Send a configuration script to the tracker. Script must consist of valid commands as
   described in the interface protocol. Commands in the script should be terminated by
   the New Line character '\n'. Line Feed character '\r' is added by the function and is
   not required. */

BOOL  ISLIB_SendScript( ISD_TRACKER_HANDLE, char * );


/* Number of currently opened trackers is stored in the parameter passed to this
   functions */

BOOL  ISLIB_NumOpenTrackers( WORD * );


/* Open tracker configuration win32 interface. Provide tracker handle to configure one
   specific tracking device, or NULL to get all configured devices. 
   Not implemented in this release. */

HWND  ISLIB_OpenConfigWindow( ISD_TRACKER_HANDLE );
 
#endif

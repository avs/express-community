/***********************************************************************
*
*    File Name:      isense
*    Description:    DLL access point
*    Created:        12/7/98
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*                    
************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "isense.h"

static void printErrorMessage( DWORD error, LPCSTR lpProcName );
static FARPROC getProcAddress( HMODULE hModule, LPCSTR lpProcName );

typedef ISD_TRACKER_HANDLE (*ISLIB_OPEN_FN)          ( HWND, DWORD, BOOL, BOOL );
typedef BOOL               (*ISLIB_COMMAND_FN)       ( ISD_TRACKER_HANDLE );
typedef BOOL               (*ISLIB_COMM_STATE_FN)    ( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE * );
typedef BOOL               (*ISLIB_SYSTEM_STATE_FN)  ( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, BOOL );
typedef BOOL               (*ISLIB_STATION_STATE_FN) ( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, WORD, BOOL );
typedef BOOL               (*ISLIB_DATA_FN)          ( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );
typedef BOOL               (*ISLIB_SCRIPT_FN)        ( ISD_TRACKER_HANDLE, char * );
typedef BOOL               (*ISLIB_COUNT_FN)         ( WORD * );
typedef HWND               (*ISLIB_WINDOW_FN)        ( ISD_TRACKER_HANDLE );

/* dll function pointers */
ISLIB_OPEN_FN           _ISLIB_OpenTracker       = NULL;
ISLIB_COMMAND_FN        _ISLIB_CloseTracker      = NULL;
ISLIB_COMM_STATE_FN     _ISLIB_GetCommState      = NULL;
ISLIB_SYSTEM_STATE_FN   _ISLIB_GetTrackerState   = NULL;
ISLIB_SYSTEM_STATE_FN   _ISLIB_SetTrackerState   = NULL;
ISLIB_STATION_STATE_FN  _ISLIB_GetStationState   = NULL;
ISLIB_STATION_STATE_FN  _ISLIB_SetStationState   = NULL;
ISLIB_DATA_FN           _ISLIB_GetTrackerData    = NULL;      
ISLIB_SCRIPT_FN         _ISLIB_SendScript        = NULL;
ISLIB_COUNT_FN          _ISLIB_NumOpenTrackers   = NULL;
ISLIB_WINDOW_FN         _ISLIB_OpenConfigWindow  = NULL; 
 
/* dll handle */  
HINSTANCE ISLIB_lib = NULL;


/*****************************************************************************
*
*   functionName:   getProcAddress
*   Description:    loads specified dll routine
*   Created:        7/25/99
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static FARPROC getProcAddress( HMODULE hModule, LPCSTR lpProcName )
{
    FARPROC proc;

    proc = GetProcAddress( hModule, lpProcName );
    if(proc == NULL)
    {
        printf("Failed to load %s. Error code %d\n", lpProcName, GetLastError());
    }
    return proc;
}


/*****************************************************************************
*
*   functionName:   load_ISLIB
*   Description:    loads isense.dll
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static HINSTANCE load_ISLIB( void )
{
    HINSTANCE hLib;

    if(hLib = LoadLibrary ( "isense.dll" ))
    {
        _ISLIB_OpenTracker      = ( ISLIB_OPEN_FN )          getProcAddress( hLib, "ISD_OpenTracker" );
        _ISLIB_CloseTracker     = ( ISLIB_COMMAND_FN )       getProcAddress( hLib, "ISD_CloseTracker" );
        _ISLIB_GetCommState     = ( ISLIB_COMM_STATE_FN )    getProcAddress( hLib, "ISD_GetCommState" );
        _ISLIB_GetTrackerState  = ( ISLIB_SYSTEM_STATE_FN )  getProcAddress( hLib, "ISD_GetTrackerState" );
        _ISLIB_SetTrackerState  = ( ISLIB_SYSTEM_STATE_FN )  getProcAddress( hLib, "ISD_SetTrackerState" );
        _ISLIB_GetStationState  = ( ISLIB_STATION_STATE_FN ) getProcAddress( hLib, "ISD_GetStationState" );
        _ISLIB_SetStationState  = ( ISLIB_STATION_STATE_FN ) getProcAddress( hLib, "ISD_SetStationState" );
        _ISLIB_GetTrackerData   = ( ISLIB_DATA_FN )          getProcAddress( hLib, "ISD_GetTrackerData" );
        _ISLIB_SendScript       = ( ISLIB_SCRIPT_FN )        getProcAddress( hLib, "ISD_SendScript" );
        _ISLIB_NumOpenTrackers  = ( ISLIB_COUNT_FN )         getProcAddress( hLib, "ISD_NumOpenTrackers" );
        _ISLIB_OpenConfigWindow = ( ISLIB_WINDOW_FN )        getProcAddress( hLib, "ISD_OpenConfigWindow" );
    }

    if(hLib == NULL)
    {
        printErrorMessage(GetLastError(), "LoadLibrary");
    }

    return hLib;
}


/*****************************************************************************
*
*   functionName:   printErrorMessage
*   Description:    
*   Created:        7/25/99
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static void printErrorMessage( DWORD error, LPCSTR lpProcName )
{
    switch(error)
    {
        case ERROR_DLL_INIT_FAILED:
            printf("A DLL initialization routine failed.\n");
            break;
        case ERROR_INVALID_FUNCTION:
            printf("The function is incorrect.\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("The system cannot find the file specified.\n");
            break;
        case ERROR_PATH_NOT_FOUND:
            printf("The system cannot find the specified path.\n");
            break;
        case ERROR_TOO_MANY_OPEN_FILES:
            printf("The system cannot open the file.\n");
            break;
        case ERROR_ACCESS_DENIED:
            printf("Access is denied.\n");
            break;
        default:
            printf("%s failed. Error code %d\n", lpProcName, error); 
            break;
    }
}


/*****************************************************************************
*
*   functionName:   free_ISLIB
*   Description:    frees isense.dll
*   Created:        12/8/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static void free_ISLIB( HINSTANCE hLib )
{
    _ISLIB_OpenTracker      = NULL;
    _ISLIB_CloseTracker     = NULL;
    _ISLIB_GetCommState     = NULL;
    _ISLIB_GetTrackerState  = NULL;
    _ISLIB_SetTrackerState  = NULL;
    _ISLIB_GetStationState  = NULL;
    _ISLIB_SetStationState  = NULL;
    _ISLIB_GetTrackerData   = NULL;      
    _ISLIB_SendScript       = NULL;
    _ISLIB_NumOpenTrackers  = NULL;
    _ISLIB_OpenConfigWindow = NULL;

	FreeLibrary ( hLib );  /* free the dll */
}



/******************************************************************************/
ISD_TRACKER_HANDLE ISLIB_OpenTracker( HWND hParent, DWORD commPort, 
                                      BOOL infoScreen, BOOL verbose )
{
    if(!_ISLIB_OpenTracker) /* this will be NULL if dll not loaded */
    {
        ISLIB_lib = load_ISLIB();

        if(!ISLIB_lib)  /* failed to load dll */
        {
            return 0;
        }
    }
    return((*_ISLIB_OpenTracker)( hParent, commPort, infoScreen, verbose ));
}


/******************************************************************************/
BOOL  ISLIB_CloseTracker( ISD_TRACKER_HANDLE handle )
{
    BOOL ret;
    WORD num;

    if(_ISLIB_CloseTracker)
    {
        ret = (*_ISLIB_CloseTracker)( handle );

        /* if all trackers are closed the dll can be freed */
        if(ISLIB_NumOpenTrackers( &num ))
        {
            if(num == 0)
            {
                free_ISLIB( ISLIB_lib );
                ISLIB_lib = NULL;
            }
        }
        return ret;
    }
    return FALSE;
}


/******************************************************************************/
BOOL ISLIB_NumOpenTrackers( WORD *num )
{
    if(_ISLIB_NumOpenTrackers) 
    {
        return((*_ISLIB_NumOpenTrackers)( num ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetCommState( ISD_TRACKER_HANDLE handle, ISD_TRACKER_TYPE *Tracker )
{
    if(_ISLIB_GetCommState)
    {
        return((*_ISLIB_GetCommState)( handle, Tracker ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetTrackerState( ISD_TRACKER_HANDLE handle, 
                             ISD_TRACKER_TYPE *Tracker, BOOL verbose )
{
    if(_ISLIB_GetTrackerState)
    {
        return((*_ISLIB_GetTrackerState)( handle, Tracker, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SetTrackerState( ISD_TRACKER_HANDLE handle, 
                             ISD_TRACKER_TYPE *Tracker, BOOL verbose )
{
    if(_ISLIB_SetTrackerState)
    {
        return((*_ISLIB_SetTrackerState)( handle, Tracker, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SetStationState( ISD_TRACKER_HANDLE handle, 
                             ISD_STATION_CONFIG_TYPE *Station, WORD stationNum, BOOL verbose )
{
    if(_ISLIB_SetStationState)
    {
        return((*_ISLIB_SetStationState)( handle, Station, stationNum, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetStationState( ISD_TRACKER_HANDLE handle, 
                             ISD_STATION_CONFIG_TYPE *Station, WORD stationNum, BOOL verbose )
{
    if(_ISLIB_GetStationState)
    {
        return((*_ISLIB_GetStationState)( handle, Station, stationNum, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetTrackerData( ISD_TRACKER_HANDLE handle, ISD_DATA_TYPE *Data )
{
    if(_ISLIB_GetTrackerData)
    {
        return((*_ISLIB_GetTrackerData)( handle, Data ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SendScript( ISD_TRACKER_HANDLE handle, char *script )
{
    if(_ISLIB_SendScript)
    {
        return((*_ISLIB_SendScript)( handle, script ));
    }
    return FALSE;
}


/******************************************************************************/
HWND  ISLIB_OpenConfigWindow( ISD_TRACKER_HANDLE handle )
{
    if(_ISLIB_OpenConfigWindow)
    {
        return((*_ISLIB_OpenConfigWindow)( handle ));
    }
    return NULL;
}


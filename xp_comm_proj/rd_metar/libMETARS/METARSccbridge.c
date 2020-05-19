#include "local.h" 
#include "metar.h"     /* standard header file */

extern int DcdMETAR( char *string , Decoded_METAR *Mptr );

/* This function calls the METAR decoding function and returns temperature and
   pressure. The station id that needs to be matched is passed in also. */

int ReturnPtrDcdMETAR( char *InputString,char *LocalStationID,
             float* LocalTemperature,float* LocalPressure)
{

    Decoded_METAR *Mptr;
    int ErReturn;
    
    Mptr = (Decoded_METAR *)malloc(sizeof(Decoded_METAR));

    ErReturn = DcdMETAR( InputString, Mptr );
    if (ErReturn != 0) 
    { 
      *LocalTemperature = 500;
      *LocalPressure = 500;
      free(Mptr);
      return 0;
    }

    /* Station match.  Fill in the temperature and pressure and use null values if
       the stations have not reported. */

    strcpy(LocalStationID,Mptr->stnid);

    /* fprintf(stderr,"Found it %s \n ",Mptr->stnid); */ 
    if (Mptr->temp < MAXINT)
    {
      *LocalTemperature = (float)Mptr->temp;
    }
    else
    {
      *LocalTemperature = 500.0;
    }
    if (Mptr->Temp_2_tenths < (float)MAXINT)
    {
      *LocalTemperature = (float)Mptr->Temp_2_tenths;
    }
    else
    {
      *LocalTemperature = 500.0;
    }
    if (Mptr->inches_altstng < (double)MAXINT)
    {
      *LocalPressure = (float)Mptr->inches_altstng;
    }
    else
    {
      *LocalPressure = 500.0;
    }

    free(Mptr);

    return ErReturn;

}


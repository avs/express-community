/*
 * PR VCP - print a string to stdout - in the VCP window
 *
 * Synopsis           : prints a string parameter to stdout
 *
 * Author:              I. Curington, AVS Inc. 28 August 2000
 */

#include "gen.hxx"

#include <avs/om.h>
#include <avs/port.h>

#include <stdio.h>
#include <errno.h>


/**************************
 * START of Module        *
 **************************/

int
Instrumentation_InstrumentationMods_prvcp::update(OMevent_mask /*event_mask */, 
				   int /*seq_num */)
{
  // Reporting Section

  printf( "%s\n", (char *) message);

  return(1);
}

/* end */



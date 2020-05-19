/*
 * gtrace - gated ARR trace
 *
 * Synopsis           : turns the OM memory manager trace function on/off
 * Author:              I. Curington, AVS Inc. 29 August 2000
 */

#include "gen.hxx"

#include <avs/om.h>
#include <avs/port.h>
#include <avs/arr.h>

#include <stdio.h>
#include <errno.h>
#include <avs/timer.h>


/**************************
 * START of Module        *
 **************************/

int
Instrumentation_InstrumentationMods_gtrace::update(OMevent_mask /*event_mask */, 
				   int /*seq_num */)
{

  if ( mode == 1 )
       ARRset_trace ( NULL, 1 );
  else
       ARRset_trace ( NULL, 0 );

  return(1);
}

/* end */


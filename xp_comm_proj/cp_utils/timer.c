
/*
 * Copyright (C) Chris Pudney, The University of Western Australia, 1996.
 * All rights reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation only for the purposes of teaching and research is hereby
 * granted without fee, provided that the above copyright notice and this
 * permission notice appear in all copies of this software/documentation and
 * that you do not sell the software.  No commercial use or distribution
 * of the software is permitted without the consent of the copyright
 * owners.  Commercial licensing is available by contacting the author(s).
 *
 * THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
 * IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 *    TITLE: timer.c - timing functions
 *  CREATED: 96
 * MODIFIED: 17/10/96
 *   AUTHOR: Chris Pudney (cpudney@alphapharm.pharm.uwa.edu.au)
 *           Biomedical Confocal Microscopy Research Centre
 *           Department of Pharmacology
 *           The University of Western Australia
 *           Nedlands, WA 6907, Australia
 */


#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/times.h>

#include "timer.h"

void
time_check(void)
{
	static float	tick = (float)CLK_TCK;
	static clock_t utime = 0, stime = 0, cutime = 0, cstime = 0;
	static struct tms	buffer;

	(void)times(&buffer);
	(void)fprintf(stderr, "user=%f, %f\tsys=%f, %f\n",
		(buffer.tms_utime - utime) / tick,
		(buffer.tms_cutime - cutime) / tick,
		(buffer.tms_stime - stime) / tick,
		(buffer.tms_cstime - cstime) / tick);
	utime = buffer.tms_utime;
	stime = buffer.tms_stime;
	cutime = buffer.tms_cutime;
	cstime = buffer.tms_cstime;

} /* end time_check() */

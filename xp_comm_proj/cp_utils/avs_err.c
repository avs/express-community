
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
 *    TITLE: avs_err.c - Error messages for AVS
 *  CREATED: 15/05/95
 * MODIFIED: 15/10/96
 *   AUTHOR: Chris Pudney (cpudney@alphapharm.pharm.uwa.edu.au)
 *           Biomedical Confocal Microscopy Research Centre
 *           Department of Pharmacology
 *           The University of Western Australia
 *           Nedlands, WA 6907, Australia
 */


#include <string.h>


#define LEN	128


/* The AVS error string */
char	avsErrStr[LEN];


/*------------------------------------------------------------------------------
 * Sets the AVS error string.
 */

void
set_avs_err(char	*str)
{
	(void)strncpy(avsErrStr, str, LEN);

} /* end set_avs_err() */

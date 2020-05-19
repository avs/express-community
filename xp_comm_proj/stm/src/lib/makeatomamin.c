/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for makeatomamin.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"


/*  makeatomamin */

makeatomamin(aamin, natoms, atomamin)
    char      aamin[][6];
int       natoms;
int       atomamin[];
{

    register int i, j;
    int namins = sizeof(aminnames)/sizeof(char *);

    for (i = 0; i < natoms; i++) {
	for (j = 0; j < namins; j++)
	   /* Check name for equality (check first char for efficiency) */
	   if (*aamin[i] == *aminnames[j] && !strcmp(aamin[i],aminnames[j]))
	      break;
/*
	if (j == namins) AVSmessage(file_version, AVS_Warning, AVSmodule,
				    "Unrecognized aminno: %s\n",aamin[i]);
*/
	atomamin[i] = j;
    }
	return 1;
}


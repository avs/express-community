/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for makemainchain.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

/* makemainchain */

int makemainchain(atype, natoms, mainchain)
char      atype[][4];
int       natoms;
int       mainchain[];
{
/*
 * Generate into mainchain the indices of the mainchain atoms. Returns
 * number of mainchain atoms..
 *
 */

    register int i;
    register int nc;

	#define streq(a,b) (0==strcmp(a,b))
    nc = 0;
    for (i = 0; i < natoms; i++) {
		if (streq(atype[i], "N")
			|| streq(atype[i], "n")
			|| streq(atype[i], "C")
			|| streq(atype[i], "c")
			|| streq(atype[i], "CA")
			|| streq(atype[i], "ca"))
	    mainchain[nc++] = i;
	}
    return nc;
}


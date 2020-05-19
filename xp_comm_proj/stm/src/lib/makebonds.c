/*
  Created October 16, 1997 by Theresa Windus and Ken Flurchick.
  Taken from coord_to_geom.c to create a reusable library.
  The original code was written by Lee Bartolotti and Ken Flurchick.

  Basic library source for makebonds.
*/
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

makebonds(apos, atype, natoms, maxnbonds, bonds, units, bscale, hbscale, atomtype)
register float apos[][3];
char	atype[][4];
int		natoms;
int		units;
float	bscale;
float	hbscale;
int		maxnbonds;
int		atomtype[];
int		bonds[][2];
{
/*
 * Generate into bonds the indices into apos of neighbors. Returns number
 * of bonds.
 *
 * This version is a simple-minded distance check with special case code to
 * prevent hydrogen over-connectivity. Mike Pique
 */

  register int i, j;
  register int nbonds;
  register int nbonds_this_atom;
  register float dx, dy, dxy, dz;
  float    threshold0;

  nbonds = 0;
  for(i = natoms - 1; i > 0; i--) {
    nbonds_this_atom = 0;
    for(j = i - 1; j >= 0 && nbonds < maxnbonds; j--) {
      /*
       * The outer loop index 'i' is AFTER the inner loop 'j': 'i'
       * leads 'j' in the list: since hydrogens traditionally follow
       * the heavy atom they're bonded to, this makes it easy to quit
       * bonding to hydrogens after one bond is made by breaking out of
       * the 'j' loop when 'i' is a hydrogen and we make a bond to it.
       * Working backwards like this makes it easy to find the heavy
       * atom that came 'just before' the Hydrogen. mp
       * Base distance criteria on vdw...lb                                 
      */
      threshold0 = (radius[atomtype[i]] + radius[atomtype[j]])/2;
      if(units == BOHR) threshold0 = threshold0/0.52914;

      if(atype[i][0] == 'H' || atype[j][0] == 'H')
        threshold0 = hbscale * threshold0;
      else if(atype[i][0] == 'F' && atype[i][1] == 'e'){
        printf("in MakeBonds found Fe\n");
        threshold0 = bscale * threshold0/2.;
      } else
        threshold0 = bscale * threshold0;

      /* never bond hydrogens to each other... */
      if (atype[i][0] == 'H' && atype[j][0] == 'H') continue;

      dx = apos[i][0] - apos[j][0];
      if (dx < 0) dx = -dx;
      if (dx > threshold0) continue;
      dy = apos[i][1] - apos[j][1];
      if (dy < 0) dy = -dy;
      if (dy > threshold0) continue;
      if ((dxy = sqrt(dx*dx + dy*dy)) > threshold0) continue;

      dz = apos[i][2] - apos[j][2];
      if (dz < 0) dz = -dz;
      if (dz > threshold0) continue;
/*
      if (sqrt(dx*dx + dy*dy) > threshold0) continue;
*/
      if (sqrt(dxy*dxy + dz*dz) > threshold0) continue;

      bonds[nbonds][0] = j;
      bonds[nbonds][1] = i;

      nbonds++;
      nbonds_this_atom++;

      /* only one bond per hydrogen */
      /* if (atype[i][0] == 'H') break; */
    }
  }
  return nbonds;
}


/*
        Created October 16, 1997 by Theresa Windus and Ken Flurchick.
        Taken from coord_to_geom.c to create a reusable library.
        The original code was written by Lee Bartolotti and Ken Flurchick.

        Basic library source for makeatomtype.
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "geomcoord.h"

makeatomtype(atype, natoms, atomtype)
char      atype[][4];
int       natoms;
int       atomtype[];
{
/*
 * Generate into atomtype the integer 0..7 atom type Returns number of
 * atoms
 *
 */
    register int i;
    char      a, b;
    int       anum;

    for (i = 0; i < natoms; i++) {
		a = atype[i][0];
		if (islower(a)) a = toupper(a);
		switch (a) {
	  		case 'A':
				b = atype[i][1];
				if (islower(b)) b = toupper(b);
				if(b == 'C') anum = 89;
				else if(b == 'G') anum = 47;
				else if(b == 'L') anum = 13;
				else if(b == 'M') anum = 95;
				else if(b == 'R') anum = 18;
				else if(b == 'S') anum = 33;
				else if(b == 'T') anum = 85;
				else if(b == 'U') anum = 79;
				break;
			case 'B':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 56;
				else if(b == 'E') anum = 4;
				else if(b == 'I') anum = 83;
				else if(b == 'K') anum = 97;
				else if(b == 'R') anum = 35;
				else anum = 5;
				break;
			case 'C':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 20;
				else if(b == 'D') anum = 48;
				else if(b == 'E') anum = 58;
				else if(b == 'F') anum = 98;
				else if(b == 'L') anum = 17;
				else if(b == 'M') anum = 96;
				else if(b == 'O') anum = 27;
				else if(b == 'R') anum = 24;
				else if(b == 'S') anum = 55;
				else if(b == 'U') anum = 29;
				else anum = 6;
				break;
			case 'D':
				anum = 66;
				break;
			case 'E':
				b = atype[i][1];
				if (islower(b)) b = toupper(b);
				if(b == 'R') anum = 68;
				else if(b == 'S') anum = 99;
				else if(b == 'U') anum = 63;
				break;
			case 'F':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'E') anum = 26;
				else if(b == 'M') anum = 100;
				else if(b == 'R') anum = 87;
				else anum = 9;
				break;
			case 'G':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 31;
				else if(b == 'D') anum = 64;
				else if(b == 'E') anum = 32;
				break;
			case 'H':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'E') anum = 2;
				else if(b == 'F') anum = 72;
				else if(b == 'G') anum = 80;
				else if(b == 'O') anum = 67;
				else anum = 1;
				break;
			case 'I':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'N') anum = 49;
				else if(b == 'R') anum = 77;
				else anum = 53;
				break;
			case 'K':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'R') anum = 36;
				else anum = 19;
				break;
			case 'L':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 57;
				else if(b == 'I') anum = 3;
				else if(b == 'R') anum = 103;
				else if(b == 'U') anum = 71;
				break;
			case 'M':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'D') anum = 101;
				else if(b == 'G') anum = 12;
				else if(b == 'N') anum = 25;
				else if(b == 'O') anum = 42;
				break;
			case 'N':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 11;
				else if(b == 'B') anum = 41;
				else if(b == 'D') anum = 60;
				else if(b == 'E') anum = 10;
				else if(b == 'I') anum = 28;
				else if(b == 'O') anum = 102;
				else if(b == 'P') anum = 93;
				else anum = 7;
				break;
			case 'O':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'S') anum = 76;
				else anum = 8;
				break;
			case 'P':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 91;
				else if(b == 'B') anum = 82;
				else if(b == 'D') anum = 46;
				else if(b == 'M') anum = 61;
				else if(b == 'O') anum = 84;
				else if(b == 'R') anum = 59;
				else if(b == 'T') anum = 78;
				else if(b == 'U') anum = 94;
				else anum = 15;
				break;
			case 'R':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 88;
				else if(b == 'B') anum = 37;
				else if(b == 'E') anum = 75;
				else if(b == 'H') anum = 45;
				else if(b == 'N') anum = 86;
				else if(b == 'U') anum = 44;
				break;
			case 'S':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'B') anum = 51;
				else if(b == 'C') anum = 21;
				else if(b == 'E') anum = 34;
				else if(b == 'I') anum = 14;
				else if(b == 'M') anum = 62;
				else if(b == 'N') anum = 50;
				else if(b == 'R') anum = 38;
				else anum = 16;
				break;
			case 'T':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'A') anum = 73;
				else if(b == 'B') anum = 65;
				else if(b == 'C') anum = 43;
				else if(b == 'E') anum = 52;
				else if(b == 'H') anum = 90;
				else if(b == 'I') anum = 22;
				else if(b == 'L') anum = 81;
				else if(b == 'M') anum = 69;
				break;
			case 'U':
				anum = 92;
				break;
			case 'V':
				anum = 23;
				break;
			case 'W':
				anum = 74;
				break;
			case 'X':
				anum = 54;
				break;
			case 'Y':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'B') anum = 70;
				else anum = 39;
				break;
			case 'Z':
				b = atype[i][1];
				if (islower(b))b = toupper(b);
				if(b == 'N') anum = 30;
				else anum = 40;
				break;
			case ' ':
				anum=104;
				break;
			default:
				anum=6;
				break;
		}
		atomtype[i] = anum-1;
    }
    return (1);
}

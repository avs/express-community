#include "tridefs.h"

// compute barycentric coordinates of a point in a triangle
// return -1 if barycentric coordinates are all +ve, or otherwise
// the index of the most -ve coordinate
int
barycentric (float *p, float *p0, float *p1, float *p2)
{
	float b[3];
	int	i, j;

	b[1] = ((p0[1] - p2[1])*( p[0] - p2[0]) - (p0[0] - p2[0])*( p[1] - p2[1])) /
	       ((p1[0] - p2[0])*(p0[1] - p2[1]) - (p1[1] - p2[1])*(p0[0] - p2[0]));

	if ((p0[0] - p2[0]) != 0.0)
		b[0] = ((p[0] - p2[0]) - b[1] * (p1[0] - p2[0])) / (p0[0] - p2[0]);
	else
		b[0] = ((p[1] - p2[1]) - b[1] * (p1[1] - p2[1])) / (p0[1] - p2[1]);

	b[2] = 1.0 - b[1] - b[0];

	if (b[0] < 0.0 || b[1] < 0.0 || b[2] < 0.0) {

		i = (b[0] < b[1]) ? 0:1;
		j = (b[2] < b[i]) ? 2:i;
	
		return(j);
		
	} else return(-1);
}


/*
**	Catmull-Rom spline interpolation of an open or closed curve
*/

#include <stdlib.h>
#include <malloc.h>
#include "spline.h"

/*
**	Private routines
*/
static void compute_coeff(float *p, int n, float *c, int nspace, int xy);
static void complete_points(float *pt, float *cpt, int nspace, int npt);
static int  compute(int npt, int nspace, float *coeff, int n, float *out);

#ifdef TEST
#include <stdio.h>
#define MAXPOINTS	200
#define nspace		  2

/*
**  test program
*/
void main()
{
	int i, j;
	float pt[MAXPOINTS*nspace];
	FILE *fp;
	float *out;

	fp = fopen("o.dat", "r");
	if(fp == NULL) exit(0);

	for(i=0; i < MAXPOINTS; i++)
	{
		for(j=0; j < nspace; j++)
		{
			if(fscanf(fp, "%f", &pt[i*nspace+j]) != 1) goto end;
		}
	}
end: fclose(fp);
	out_points = nspace * (i * 10 + 1);
	out = (float *)malloc(out_npoints*sizeof(float));
	catmull_rom_spline(pt, nspace, i, 10, SPLINE_CLOSED, out);

	for(i=0; i < out_points; i++)
	{
		printf("%f ", out[i]);
		if((i % 2) == 1) printf("\n");
	}
	free(out);
}
#endif

/*
**  Interpolate the input points with a Catmull-Rom spline to a closed contour
**  passing through all the input points.
**
**  pt:		point coordinate array with format: X0, Y0, X1, Y1 (or X0, Y0, Z0, X1, Y1, Z1 ...)
**  nspace:	space dimensionality: must be 2 or 3
**  npoints:	number of input points. Must be >= 4
**  step:	number of points on each interpolated segment. Must be > 2
**  out:	array that will receive the output points: must be nspace*(npoints*step+1) floats
**  is_open:	if 1 the curve is open, otherwise must be closed
**
**  returns the number of output points on sucessful interpolation, 0 otherwise
*/
int catmull_rom_spline(float *pt, int nspace, int npoints, int step, int is_open, float *out)
{
	float *coeff;
	float *cpt;
	int i, out_size;

	/*
	**  check input parameters
	*/
	if(pt == NULL || out == NULL || npoints < 4 || nspace < 2 || nspace > 3 || step < 3) return 0;

	/*
	**  allocate temporary augmented points and coefficent arrays
	*/
	cpt = (float *)malloc(nspace*(npoints+3)*sizeof(float));
	coeff = (float *)malloc(4*nspace*(npoints+1)*sizeof(float));
	if(cpt == NULL || coeff == NULL) return 0;

	/*
	**  set boundary points
	*/
	complete_points(pt, cpt, nspace, npoints);

	/*
	**  for each dimension compute the spline coefficients
	*/
	for(i=0; i < nspace; i++)
		compute_coeff(cpt, npoints, coeff, nspace, i);

	/*
	**  using the spline compute the intermediate steps
	*/
	if(is_open) --npoints;
	out_size = compute(npoints, nspace, coeff, step, out);

	/*
	**  release temporary arrays and return success
	*/
	free(coeff);
	free(cpt);
	return out_size;
}

/*
**	Complete the point list to simplify the subsequent computations
**	Change the list:	P0 P1 ... Pn
**	Into:				Pn P0 P1 ... Pn P0 P1
*/
static void complete_points(float *pt, float *cpt, int nspace, int npt)
{
	int i, j;

	for(i=0; i < nspace; i++)
		cpt[i] = pt[(npt-1)*nspace+i];

	for(i=0; i < npt; i++)
		for(j=0; j < nspace; j++)
			cpt[i*nspace+j+nspace] = pt[i*nspace+j];

	for(i=0; i < 2; i++)
		for(j=0; j < nspace; j++)
			cpt[(npt+1+i)*nspace+j] = pt[i*nspace+j];
}

/*
**	Calculate coefficents
*/
static void compute_coeff(float *p, int n, float *c, int nspace, int xy)
{
	int i, row, col;
	float v[4];
	int m[4][4] = {
			{ 0,  2,  0,  0},
			{-1,  0,  1,  0},
			{ 2, -5,  4, -1},
			{-1,  3, -3,  1}
	};

	for(i=0; i <= n; i++)
	{
		v[0] = p[i*nspace+xy];
		v[1] = p[(i+1)*nspace+xy];
		v[2] = p[(i+2)*nspace+xy];
		v[3] = p[(i+3)*nspace+xy];

		for(row=0; row < 4; row++)
		{
			c[i*4*nspace+4*xy+row] = 0.;
			for(col=0; col < 4; col++)
			{
				c[i*4*nspace+4*xy+row] += (float)0.5*m[row][col]*v[col];
			}
		}
	}
}

int compute(int npt, int nspace, float *coeff, int n, float *out)
{
	int i, j, k;
	float d, delta;

	delta = (float)1./n;
	for(i=0; i < npt; i++)
	{
		d = 0.;
		for(k=0; k < n; k++)
		{
			
			for(j=0; j < nspace; j++)
			{
				out[i*n*nspace+k*nspace+j] =
					     coeff[i*4*nspace+4*j+0] +
					d * (coeff[i*4*nspace+4*j+1] +
					d * (coeff[i*4*nspace+4*j+2] +
					d *  coeff[i*4*nspace+4*j+3]));
			}
			d += delta;
		}
	}
	i = npt-1;
	for(j=0; j < nspace; j++)
	{
		out[npt*n*nspace+j] = 
			coeff[i*4*nspace+4*j+0] +
			coeff[i*4*nspace+4*j+1] + 
			coeff[i*4*nspace+4*j+2] +
			coeff[i*4*nspace+4*j+3];
	}

	return npt*n+1;
}

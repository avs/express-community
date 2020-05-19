
#include <math.h>
#include <stdlib.h>
#include "catmull.hxx"

CatmullRomSpline::CatmullRomSpline()
{
	m_outnpoints = 0;
	m_out = NULL;
}

CatmullRomSpline::~CatmullRomSpline()
{
	if (m_out!=NULL)
	   delete [] m_out;
}

//
//	Compute coefficents
//

float *CatmullRomSpline::compute_coeff(float *points, int npoints, int nspace,
                                       int dimension, int is_open, int *ncoeff,
                                       float s)
{
	int i, j, idx;
	int first, last, end;
	float v[4];

	//
	//  Consider closed or open lines to compute first and last segments
	//  Open:    P0   P0 P1 P2 .... Pn-3 Pn-2 Pn-1 Pn-1
	//  Closed:  Pn-1 P0 P1 P2 .....Pn-3 Pn-2 Pn-1 P0   P1
	//

	if(is_open)
	{
		first   = 0;
		last    = 1;
		end     = npoints - 3;
		*ncoeff = npoints - 1;
	}
	else
	{
		first   = npoints - 1;
		last    = npoints;
		end     = npoints - 2;
		*ncoeff = npoints;
	}

	//
	//	Allocate the coefficient vector for this dimension
	//
	
	float *c = new float[4*(*ncoeff)];

	//
	//	Now walk the points 4 by 4
	//
	
	for(i = -1; i <= end; i++)
	{
		//
		//  Load four adjacent points coords taking care of boundary
		//  constraints
		//

		for(j=0; j < 4; j++)
		{
			if (i+j < 0)
			   idx = first;
			else if(i+j >= npoints)
			   idx = i + j - last;
			else
			   idx = i + j;
		
			v[j] = points[idx*nspace+dimension];
		}

		//
		//	The spline is defined by the matrix:
		//				 	{ 0,    2,    0,  0}
		//			1/2 *	{ -s,   0,    s,  0}
		//					{ 2s, s-6, 6-2s, -s}
		//					{ -s, 4-s,  s-4,  s}
		//
		//	The smoothness parameter s varies between 1 (spline) to 0 
		//  (straight segments)
		//
		
		c[(i+1)*4+0] = v[1];
		c[(i+1)*4+1] = s*(v[2]-v[0])*0.5F;
		c[(i+1)*4+2] = s*(v[0]+0.5F*v[1]-v[2]-0.5F*v[3])+3.0F*(v[2]-v[1]);
		c[(i+1)*4+3] = s*(-v[0]-v[1]+v[2]+v[3])*0.5F+2.0F*(v[1]-v[2]);
	}

	return c;
}

//
//	Add to out the computed points
//

void CatmullRomSpline::compute_points(float **coeff, int ncoeff, 
                                      int nspace, int added_points)
{
	int i, j, k;
	float t, delta;

	//
	//	Equispaced param values
	//
	
	delta = 1.0F/(added_points+1);

	//
	//  For each segment compute the points in the open interval of the 
	//  parameter [0, 1)
	//
	
	for(i=0; i < ncoeff; i++)
	{
		t = 0.;
		for(k=0; k <= added_points; k++)
		{
			
			for(j=0; j < nspace; j++)
			{
				m_out[(i*(added_points+1)+k)*nspace+j] = coeff[j][i*4+0] + t * (coeff[j][i*4+1] + t * (coeff[j][i*4+2] + t *  coeff[j][i*4+3]));
			}
			t += delta;
		}
	}

	//
	//  Now add the last point (for parameter = 1 in the last 
	//  set of coefficients)
	//

	i = ncoeff-1;
	for(j=0; j < nspace; j++)
	{
		m_out[ncoeff*(added_points+1)*nspace+j] = coeff[j][i*4+0] 
		                                        + coeff[j][i*4+1] 
		                                        + coeff[j][i*4+2] 
		                                        + coeff[j][i*4+3];
	}
}


int CatmullRomSpline::ComputePoints(float *pt, int nspace, int npoints, 
                                    int added_points, int is_open, 
                                    float smoothness)
{
	int		 i;

	//
	//	Check input parameters
	//

	if(pt == NULL || npoints < ((is_open) ? 2 : 3) 
	              || nspace < 1 
				  || added_points < 0)
	{
		return 0;
	}

/*
	//  Check is essentially redundant as an extra node is generated anyway 
	//  if polyline is closed. Also causes floating point percision problems.
	//
	//  If last point is coincident with first one classify the line as 
	//  closed with one point less 
	//

	int coincident = 1;
	for(i=0; i < nspace; i++)
	{
		if(fabs(1.0 - pt[(npoints-1)*nspace + i]/pt[i]) > 0.001)
		{
			coincident = 0;
			break;
		}
	}
	if(coincident)
	{
		is_open = 0;
		--npoints;
	}
	if (npoints < 3)
	   return 0;
*/

	//
	//  For each dimension compute the spline coefficients
	//

	float **coeff = new float*[nspace];
	int     ncoeff;
	for(i=0; i < nspace; i++)
	{
		coeff[i] = compute_coeff(pt, npoints, nspace, i, is_open,
		                         &ncoeff, smoothness);
	}

	//
	//	Allocate the output array
	//

	m_outnpoints = (is_open) ? npoints + added_points * (npoints-1) 
	                         : npoints + added_points * npoints + 1;
		
	m_out = new float[m_outnpoints * nspace];

	//
	//	Now do the interpolation
	//

	compute_points(coeff, ncoeff, nspace, added_points);

	//
	//	Delete the temporary coefficient array
	//

	for(i=0; i < nspace; i++)
	   delete [] coeff[i];
	delete [] coeff;

	//
	//	Save useful values;
	//

	m_nspace = nspace;

	return m_outnpoints;
}

float *CatmullRomSpline::CopyResult(float *out)
{
	if(out != NULL)
	{
		for(int i=0; i < m_outnpoints*m_nspace; i++) out[i] = m_out[i];
		return out;
	}
	else
	{
		return m_out;
	}
}


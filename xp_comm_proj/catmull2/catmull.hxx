#ifndef AVS_CATMULL_H
#define AVS_CATMULL_H

//
//	Interpolate the input points with a Catmull-Rom spline to 
//	an open or closed contour passing through all the input points.
//

class CatmullRomSpline 
{

public:

	//
	//	Constructor, destructor and supporting functions
	//
	
	CatmullRomSpline();
	~CatmullRomSpline();

	//
	//	Compute the interpolating spline
	//
	//  pt:           point coordinate array with format: 
	//                      X0, Y0, X1, Y1 (or X0, Y0, Z0, X1, Y1, Z1 ...)
	//  nspace:       space dimensionality: must be > 0
	//  npoints:      number of input points. 
	//                Must be >= 2 (open) or >= 3 (close spline)
	//  added_points: number of points on each interpolated 
	//                segment between two input points. Must be >= 0
	//  is_open:      if 1 the curve is open, otherwise 
	//                if 0 must be closed (ie a segment is added 
	//                between the last and first input points).
	//	smoothness:   controls the degree of smoothness of the 
	//                Catmull-Rom spline (1: spline, 0: polyline)
	//
	//  returns the number of output points. Zero on error.
	//
	
	int ComputePoints(float *pt, int nspace, int npoints, int added_points,
	                  int is_open, float smoothness);

	//
	//	Copies the resulting output point array if not NULL. 
	//	Return a pointer to it or to the internal array.
	//
	
	float *CopyResult(float *out);

private:

	float *m_out;
	int    m_outnpoints;
	int    m_nspace;

	float *compute_coeff(float *points, int npoints, int nspace,
	                     int dimension, int is_open, int *ncoeff, 
	                     float smoothness);
	void   compute_points(float **coeff, int ncoeff, int nspace, 
	                      int added_points);

};

#endif

#ifndef TPSPLINE_H
#define TPSPLINE_H

class mgcThinPlateSpline
{
public:
	mgcThinPlateSpline (int _N, float* _x, float* _y, float* z,
		float smooth = 0);
	~mgcThinPlateSpline ();

	float operator() (float x0, float y0);

private:
	float D[3];
	int N;
	float *x, *y, *tmp, *C;
	float **mat, **T, **P, **Q;
    float xmin, xmax, xrange, ymin, ymax, yrange;

	int Allocate ();
	void Deallocate ();
	float LogSpecial (float t);
};

#endif

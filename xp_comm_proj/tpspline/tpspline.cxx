#include <math.h>
#include "solve.hxx"
#include "tpspline.hxx"

//---------------------------------------------------------------------------
int mgcThinPlateSpline::
Allocate ()
{
	int row;

	mat = new float*[N];
	if ( !mat )
		return 0;
	for (row = 0; row < N; row++) {
		mat[row] = new float[N];
		if ( !mat[row] )
			return 0;
	}

	T = new float*[N];
	if ( !T )
		return 0;
	for (row = 0; row < N; row++) {
		T[row] = new float[3];
		if ( !T[row] )
			return 0;
	}

	P = new float*[3];
	if ( !P )
		return 0;
	for (row = 0; row < 3; row++) {
		P[row] = new float[N];
		if ( !P[row] )
			return 0;
	}

	Q = new float*[3];
	if ( !Q )
		return 0;
	for (row = 0; row < 3; row++) {
		Q[row] = new float[3];
		if ( !Q[row] )
			return 0;
	}

	tmp = new float[N];
	if ( !tmp )
		return 0;

	return 1;
}
//---------------------------------------------------------------------------
void mgcThinPlateSpline::
Deallocate ()
{
	int row;

   // Check we have a valid number of points
	if (N < 3) return;

	if ( mat )
		for (row = 0; row < N; row++)
			if ( mat[row] )
				delete[] mat[row];
	delete[] mat;

	if ( T )
		for (row = 0; row < N; row++)
			if ( T[row] )
				delete[] T[row];
	delete[] T;

	if ( P )
		for (row = 0; row < 3; row++)
			if ( P[row] )
				delete[] P[row];
	delete[] P;

	if ( Q )
		for (row = 0; row < 3; row++)
			if ( Q[row] )
				delete[] Q[row];
	delete[] Q;

	delete[] tmp;
}
//---------------------------------------------------------------------------
mgcThinPlateSpline::
mgcThinPlateSpline (int _N, float* _x, float* _y, float* z, float smooth)
{
	int row, col, d;

	if ( (N = _N) < 3 )
		return;
	if ( (x = _x) == 0 )
		return;
	if ( (y = _y) == 0 )
    	return;

	// (x,y) data must be mapped to unit square
	xmin = x[0];
	xmax = x[0];
	for (d = 1; d < N; d++) {
		if ( x[d] < xmin )
			xmin = x[d];
		if ( x[d] > xmax )
			xmax = x[d];
	}
	xrange = xmax-xmin;
	for (d = 0; d < N; d++)
		x[d] = (x[d]-xmin)/xrange;

	ymin = y[0];
	ymax = y[0];
	for (d = 1; d < N; d++) {
		if ( y[d] < ymin )
			ymin = y[d];
		if ( y[d] > ymax )
			ymax = y[d];
	}
	yrange = ymax-ymin;
	for (d = 0; d < N; d++)
		y[d] = (y[d]-ymin)/yrange;

	mat = 0;
	T = 0;
	P = 0;
	Q = 0;
	tmp = 0;

	if ( !Allocate() ) {
		Deallocate();
		return;
	}

	// compute matrix E+smooth*I
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if ( row == col )
				mat[row][col] = smooth;
			else {
				float xdiff = x[row]-x[col];
				float ydiff = y[row]-y[col];
				mat[row][col] =
                    LogSpecial(float(sqrt(xdiff*xdiff+ydiff*ydiff)));
			}

	// compute matrix T
	for (row = 0; row < N; row++) {
		T[row][0] = 1;
		T[row][1] = x[row];
		T[row][2] = y[row];
	}

	// compute matrix (E+smooth*I)^{-1}
	if ( !mgcLinearSystem().Inverse(N,mat) ) {  // M <- Inverse(M)
		Deallocate();
		return;
	}

	// compute P = T^t (E+smooth*I)^{-1}  [3xN matrix]
	for (row = 0; row < 3; row++)
		for (col = 0; col < N; col++) {
			P[row][col] = 0;
			for (d = 0; d < N; d++)
				P[row][col] += T[d][row]*mat[d][col];
		}

	// compute Q = P T = T^t (E+smooth*I)^{-1} T  [3x3 matrix]
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++) {
			Q[row][col] = 0;
			for (d = 0; d < N; d++)
				Q[row][col] += P[row][d]*T[d][col];
		}

	// compute Q^{-1}
	if ( !mgcLinearSystem().Inverse(3,Q) ) {  // Q <- Inverse(Q)
		Deallocate();
		return;
	}

	// compute Pz
	for (row = 0; row < 3; row++) {
		tmp[row] = 0;
		for (d = 0; d < N; d++)
			tmp[row] += P[row][d]*z[d];
	}

	// compute D vector for smooth thin plate spline
	for (row = 0; row < 3; row++) {
		D[row] = 0;
		for (d = 0; d < 3; d++)
			D[row] += Q[row][d]*tmp[d];
	}

	// compute z-TD
	for (row = 0; row < N; row++) {
		tmp[row] = z[row];
		for (d = 0; d < 3; d++)
			tmp[row] -= T[row][d]*D[d];
	}

	// compute C vector for smooth thin plate spline
   C = 0;   // Clear array pointer first
	C = new float[N];
	if ( !C ) {
		Deallocate();
		return;
	}
	for (row = 0; row < N; row++) {
		C[row] = 0;
		for (d = 0; d < N; d++)
			C[row] += mat[row][d]*tmp[d];
	}
}
//---------------------------------------------------------------------------
mgcThinPlateSpline::
~mgcThinPlateSpline ()
{
	Deallocate();
	delete[] C;
}
//---------------------------------------------------------------------------
float mgcThinPlateSpline::
operator() (float x0, float y0)
{
	x0 = (x0-xmin)/xrange;
    y0 = (y0-ymin)/yrange;

	float result = D[0]+D[1]*x0+D[2]*y0;
	for (int d = 0; d < N; d++) {
		float xdiff = x0-x[d];
		float ydiff = y0-y[d];
		result += C[d]*LogSpecial(float(sqrt(xdiff*xdiff+ydiff*ydiff)));
	}
	return result;
}
//---------------------------------------------------------------------------
float mgcThinPlateSpline::
LogSpecial (float t)
{
	if ( t > 0 ) {
		float t2 = t*t;
		return float(t2*log(t2));
	}
	else
		return 0.0f;
}
//---------------------------------------------------------------------------

#ifdef TPSPLINE_TEST

#include <iostream.h>

int main ()
{
	cout.setf(ios::fixed);

	const int N = 9;
	int i, j;
	float x[N] = { 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0 };
	float y[N] = { 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0 };
	float z[N] = { 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0, 2.0, 3.0 };

	// no smoothing, exact interpolation at grid points
	mgcThinPlateSpline tps0(N,x,y,z);
	for (j = 0; j <= 6; j++) {
		for (i = 0; i <= 6; i++) {
			float result = tps0(i/6.0,j/6.0);
			cout << result << ' ';
		}
		cout << endl;
	}
	cout << endl;

	// small amount of smoothing
	mgcThinPlateSpline tps1(N,x,y,z,0.1);
	for (j = 0; j <= 6; j++) {
		for (i = 0; i <= 6; i++) {
			float result = tps1(i/6.0,j/6.0);
			cout << result << ' ';
		}
		cout << endl;
	}

	return 0;
}

#endif


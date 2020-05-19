#ifndef SOLVE_H
#define SOLVE_H

class mgcLinearSystem
{
public:
	mgcLinearSystem() {;}

	float** NewMatrix (int N);
	void DeleteMatrix (int N, float** A);
	float* NewVector (int N);
	void DeleteVector (int N, float* B);

	int Inverse (int N, float** A);
	// Input:
	//     A[N][N], entries are A[row][col]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     A[N][N], inverse matrix

	int Solve (int N, float** A, float* b);
	// Input:
	//     A[N][N] coefficient matrix, entries are A[row][col]
	//     b[N] vector, entries are b[row]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     A[N][N] is inverse matrix
	//     b[N] is solution x to Ax = b

	int SolveTri (int N, float* a, float* b, float* c, float* r, float* u);
	// Input:
	//     Matrix is tridiagonal.
	//     Lower diagonal a[N-1]
	//     Main  diagonal b[N]
	//     Upper diagonal c[N-1]
	//     Right-hand side r[N]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     u[N] is solution

	int SolveConstTri (int N, float a, float b, float c, float* r, float* u);
	// Input:
	//     Matrix is tridiagonal.
	//     Lower diagonal is constant, a
	//     Main  diagonal is constant, b
	//     Upper diagonal is constant, c
	//     Right-hand side r[N]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     u[N] is solution

	int SolveSymmetric (int N, float** A, float* b);
	// Input:
	//     A[N][N] symmetric coefficient matrix, entries are A[row][col]
	//     b[N] vector, entries are b[row]
	// Output:
	//     return value is TRUE if successful, FALSE if (nearly) singular
	//     decomposition A = L D L^t (diagonal terms of L are all 1)
	//         A[i][i] = entries of diagonal D
	//         A[i][j] for i > j = lower triangular part of L
	//     b[N] is solution to x to Ax = b

	int SymmetricInverse (int N, float** A, float** Ainv);
	// Input:
	//     A[N][N], entries are A[row][col]
	// Output:
	//     return value is TRUE if successful, FALSE if algorithm failed
	//     Ainv[N][N], inverse matrix
};

class mgcLinearSystemD
{
public:
	mgcLinearSystemD() {;}

	double** NewMatrix (int N);
	void DeleteMatrix (int N, double** A);
	double* NewVector (int N);
	void DeleteVector (int N, double* B);

	int Inverse (int N, double** A);
	// Input:
	//     A[N][N], entries are A[row][col]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     A[N][N], inverse matrix

	int Solve (int N, double** A, double* b);
	// Input:
	//     A[N][N] coefficient matrix, entries are A[row][col]
	//     b[N] vector, entries are b[row]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     A[N][N] is inverse matrix
	//     b[N] is solution x to Ax = b

	int SolveTri (int N, double* a, double* b, double* c, double* r, 
		double* u);
	// Input:
	//     Matrix is tridiagonal.
	//     Lower diagonal a[N-1]
	//     Main  diagonal b[N]
	//     Upper diagonal c[N-1]
	//     Right-hand side r[N]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     u[N] is solution

	int SolveConstTri (int N, double a, double b, double c, double* r, 
		double* u);
	// Input:
	//     Matrix is tridiagonal.
	//     Lower diagonal is constant, a
	//     Main  diagonal is constant, b
	//     Upper diagonal is constant, c
	//     Right-hand side r[N]
	// Output:
	//     return value is TRUE if successful, FALSE if pivoting failed
	//     u[N] is solution

	int SolveSymmetric (int N, double** A, double* b);
	// Input:
	//     A[N][N] symmetric coefficient matrix, entries are A[row][col]
	//     b[N] vector, entries are b[row]
	// Output:
	//     return value is TRUE if successful, FALSE if (nearly) singular
	//     decomposition A = L D L^t (diagonal terms of L are all 1)
	//         A[i][i] = entries of diagonal D
	//         A[i][j] for i > j = lower triangular part of L
	//     b[N] is solution to x to Ax = b

	int SymmetricInverse (int N, double** A, double** Ainv);
	// Input:
	//     A[N][N], entries are A[row][col]
	// Output:
	//     return value is TRUE if successful, FALSE if algorithm failed
	//     Ainv[N][N], inverse matrix
};

#endif


/*
**  Public interface to the Catmull-Rom spline interpolator
*/
extern int catmull_rom_spline(float *pt, int nspace, int npoints, int step, int is_open, float *out);

/*
**	Definition for the is_open parameter
*/
#define SPLINE_CLOSED	0
#define SPLINE_OPEN		1

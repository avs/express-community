//
//	Class that support optimized computations of:
//		position to slot
//		gaussian spread
//
#include "ComputeSlot.h"

ComputeSlot::ComputeSlot(int *slots, float corners[2][3])
{
	px = slots[0]/(corners[1][0] - corners[0][0]);
	py = slots[1]/(corners[1][1] - corners[0][1]);
	pz = slots[2]/(corners[1][2] - corners[0][2]);

	cx = corners[0][0];
	cy = corners[0][1];
	cz = corners[0][2];

	sxy = slots[0]*slots[1];

	sx = slots[0];
	sy = slots[1];
	sz = slots[2];

	// for the gaussian computation
	w = 0;
	kern = 0;
	half_kernel_len = 0;
}

ComputeSlot::~ComputeSlot()
{
	delete [] w;
}

int ComputeSlot::slot(float *pt) const
{
	//
	//	This is the routine before optimization
	//
//	int i = (int)((pt[0] - corners[0][0])/(corners[1][0] - corners[0][0])*slots[0]);
//	if(i < 0) i = 0;
//	else if(i >= slots[0]) i = slots[0]-1;
//	int j = (int)((pt[1] - corners[0][1])/(corners[1][1] - corners[0][1])*slots[1]);
//	if(j < 0) j = 0;
//	else if(j >= slots[1]) j = slots[1]-1;
//	int k = (int)((pt[2] - corners[0][2])/(corners[1][2] - corners[0][2])*slots[2]);
//	if(k < 0) k = 0;
//	else if(k >= slots[2]) k = slots[2]-1;
//	
//	return i + slots[0]*j + slots[0]*slots[1]*k;

	int i = (int)((pt[0] - cx)*px);
	if(i < 0) i = 0;
	else if(i >= sx) i = sx-1;

	int j = (int)((pt[1] - cy)*py);
	if(j < 0) j = 0;
	else if(j >= sy) j = sy-1;

	int k = (int)((pt[2] - cz)*pz);
	if(k < 0) k = 0;
	else if(k >= sz) k = sz-1;
	
	return i + sx*j + sxy*k;
}

int ComputeSlot::slot(float x, float y, float z) const
{
	int i = (int)((x - cx)*px);
	if(i < 0) i = 0;
	else if(i >= sx) i = sx-1;

	int j = (int)((y - cy)*py);
	if(j < 0) j = 0;
	else if(j >= sy) j = sy-1;

	int k = (int)((z - cz)*pz);
	if(k < 0) k = 0;
	else if(k >= sz) k = sz-1;
	
	return i + sx*j + sxy*k;
}

float ComputeSlot::weight(int di, int dj, int dk) const
{
	return w[di+half_kernel_len+kern*(dj+half_kernel_len)+kern*kern*(dk+half_kernel_len)];
}

float ComputeSlot::gauss(float x)
{
	if(x < -1.5 || x > 1.5) return 0.;
	if(x >= -0.5 && x < 0.5) return 0.75 - x*x;
	if(x < 0.) return 0.5 * (x+1.5) * (x+1.5);
	return 0.5 * (x-1.5) * (x-1.5);
}

void ComputeSlot::init_weights(float sigma, int kernel) 
{
	int i, j, k;

	kern = kernel;
	half_kernel_len = kernel/2;
	float mult = 1.5/(sigma*sigma*(half_kernel_len+1.));

	w = new float[kernel*kernel*kernel];

	for(i = 0; i < kernel; ++i)
	{
		float gx = gauss(mult*(i-half_kernel_len));

		for(j = 0; j < kernel; ++j)
		{
			float gy = gauss(mult*(j-half_kernel_len));

			for(k = 0; k < kernel; ++k)
			{
				float gz = gauss(mult*(k-half_kernel_len));

				w[i+kern*j+kern*kern*k] = gx*gy*gz;
			}
		}
	}

	// now normalize the result
	float norm = 0.;
	for(i=0; i < kern*kern*kern; ++i) norm += w[i];
	for(i=0; i < kern*kern*kern; ++i) w[i] /= norm;
}

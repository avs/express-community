//
//	Class that support optimized computations of:
//		position to slot
//		gaussian spread
//
class ComputeSlot
{
public:
	ComputeSlot(int *slots, float corners[2][3]);	// initialize
	~ComputeSlot();
	int slot(float *pt) const;						// position to slot
	int slot(float x, float y, float z) const;		// position to slot

	void init_weights(float sigma, int kernel);		// init gaussian spread function
	float weight(int di, int dj, int dk) const;		// return weight from index (displacement from current cell)

private:
	float px, py, pz;
	float cx, cy, cz;
	int sxy, sx, sy, sz;

	float gauss(float x); // compute approximate gaussian

	float *w;
	int kern;
	int half_kernel_len;
};

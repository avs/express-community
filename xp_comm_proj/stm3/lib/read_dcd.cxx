
#include "stm3.h"

static inline void REVERSE4BYTES(char *a)
{
        char x;
        x = a[0]; a[0] = a[3]; a[3] = x;
        x = a[1]; a[1] = a[2]; a[2] = x;
}

void read_dcd(FILE *fp, int num_atoms, int big_endian, float *X, float *Y, float *Z)
{
	int dummy[2];
	int i;

	fread(X, sizeof(float), num_atoms, fp);
	fread(dummy, sizeof(int), 2, fp);
	fread(Y, sizeof(float), num_atoms, fp);
	fread(dummy, sizeof(int), 2, fp);
	fread(Z, sizeof(float), num_atoms, fp);

	if(big_endian)
	{
		for(i=0; i < num_atoms; ++i)
		{
			REVERSE4BYTES((char *)&X[i]);
			REVERSE4BYTES((char *)&Y[i]);
			REVERSE4BYTES((char *)&Z[i]);
		}
	}
}

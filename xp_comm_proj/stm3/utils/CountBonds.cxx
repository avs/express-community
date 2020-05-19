
#include "CountBonds_gen.h"


int
STM3_UTILS_CountBonds::CountBonds(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// out (MolecularDataType write)
	int i;
	
	int *conn = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!conn) return 0;
	
	float *nbonds = (float *)out.charge_lst.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!nbonds)
	{
		ARRfree(conn);
		return 0;
	}

	for(i=0; i < in.num_atoms; ++i) nbonds[i] = 0.;

	for(i=0; i < in.num_bonds; ++i)
	{
		nbonds[conn[i*2+0]] += 1.;
		nbonds[conn[i*2+1]] += 1.;
	}

	ARRfree(nbonds);
	ARRfree(conn);
	
	// return 1 for success
	return 1;
}

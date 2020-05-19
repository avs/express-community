
#include "SubCell_gen.h"
#include <math.h>
#include <avs/mathutil.h>
#include "../utils/copy_mol.h"

// below this value a quantity will be considered 0
#define TOL 1e-6F

int
STM3_CRYSTAL_SubCell::SubCell(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// miller_planes (OMXint_array read notify)
	// out (MolecularDataType write)
	int i, j;

	// Get the Miller indices of the six planes  parallel to the unit_cell sides
	// A special convention: index 0 means infinite. The indices must be positive.
	int *miller_planes_arr = (int *)miller_planes.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!miller_planes_arr) return 0;

	// Sanity check of the indices
	for(i=0; i < 6; ++i) if(miller_planes_arr[i] < 0) miller_planes_arr[i] = 0;
	for(i=0; i < 5; i += 2)
	{
		if(miller_planes_arr[i] && !(miller_planes_arr[i+1] && (miller_planes_arr[i] >= miller_planes_arr[i+1])))
		{
			miller_planes_arr[i+1] = miller_planes_arr[i];
		}
	}
		
	// get the unit cell basis vectors
	float *basis = (float *)in.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!basis)
	{
		ARRfree(miller_planes_arr);
		return 0;
	}

	// if no unit cell data available, then exit
	if(basis[0] == 0.0F && basis[1] == 0.0F && basis[2] == 0.0F)
	{
		ARRfree(basis);
		ARRfree(miller_planes_arr);
		return 0;
	}
		
	// set the three basis vectors
	float *a = &basis[0];
	float *b = &basis[3];
	float *c = &basis[6];
	
	// compute the axis basis intersections 
	float bmin[9], bmax[9];
	for(i=0; i < 3; ++i)
	{
		bmin[0+i] = (miller_planes_arr[0]) ? a[i]/(float)miller_planes_arr[0] : 0.0F;
		bmin[3+i] = (miller_planes_arr[2]) ? b[i]/(float)miller_planes_arr[2] : 0.0F;
		bmin[6+i] = (miller_planes_arr[4]) ? c[i]/(float)miller_planes_arr[4] : 0.0F;
		
		bmax[0+i] = (miller_planes_arr[1]) ? a[i]/(float)miller_planes_arr[1] : 0.0F;
		bmax[3+i] = (miller_planes_arr[3]) ? b[i]/(float)miller_planes_arr[3] : 0.0F;
		bmax[6+i] = (miller_planes_arr[5]) ? c[i]/(float)miller_planes_arr[5] : 0.0F;
	}
	ARRfree(miller_planes_arr);

	// Compute the new unit cell
	float *angles_in_arr = (float *)in.unit_cell.angles.ret_array_ptr(OM_GET_ARRAY_RD);
	if(angles_in_arr)
	{
		float *angles_out_arr = (float *)out.unit_cell.angles.ret_array_ptr(OM_GET_ARRAY_WR);
		if(angles_out_arr)
		{
			for(i=0; i < 3; ++i) angles_out_arr[i] = angles_in_arr[i];
			
			ARRfree(angles_out_arr);
		}
		ARRfree(angles_in_arr);
	}
	
	float origin[3];
	origin[0] = origin[1] = origin[2] = 0.0F;
	float *origin_in_arr = (float *)in.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_RD);
	if(origin_in_arr)
	{
		float *origin_out_arr = (float *)out.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_WR);
		if(origin_out_arr)
		{
			for(i=0; i < 3; ++i) origin[i] = origin_in_arr[i];
			for(i=0; i < 3; ++i) origin_out_arr[i] = origin_in_arr[i] + bmin[0+i] + bmin[3+i] + bmin[6+i];
			
			ARRfree(origin_out_arr);
		}
		ARRfree(origin_in_arr);
	}
	
	float *side_vectors_out_arr = (float *)out.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_WR);
	if(side_vectors_out_arr)
	{
		for(i=0; i < 3; ++i) for(j=0; j < 3; ++j) side_vectors_out_arr[3*i+j] =  bmax[3*i+j] - bmin[3*i+j];

		ARRfree(side_vectors_out_arr);
	}
	
	// plane parameters
	float normal_a[3], normal_b[3], normal_c[3];
	float dist_a[2],   dist_b[2],   dist_c[2];
	
 	AVS_CROSS(b, c, normal_a);
	AVS_UNITIZE(normal_a, TOL);
	dist_a[0] = -AVS_DOT(normal_a, (&bmin[0]));
	dist_a[1] = -AVS_DOT(normal_a, (&bmax[0]));
		
 	AVS_CROSS(c, a, normal_b);
	AVS_UNITIZE(normal_b, TOL);
	dist_b[0] = -AVS_DOT(normal_b, (&bmin[3]));
	dist_b[1] = -AVS_DOT(normal_b, (&bmax[3]));
		
 	AVS_CROSS(a, b, normal_c);
	AVS_UNITIZE(normal_c, TOL);
	dist_c[0] = -AVS_DOT(normal_c, (&bmin[6]));
	dist_c[1] = -AVS_DOT(normal_c, (&bmax[6]));

	ARRfree(basis);

	// copy only the atoms included in the new cell
	float *xyz = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz) return 0;

	bool *included = new bool[in.num_atoms];
	
	float eps = epsilon;
	for(i=0; i < in.num_atoms; ++i)
	{
		float x = xyz[3*i+0] - origin[0];
		float y = xyz[3*i+1] - origin[1];
		float z = xyz[3*i+2] - origin[2];
#if 0		
		included[i] = ((normal_a[0]*x + normal_a[1]*y + normal_a[2]*z + dist_a[0]) >= 0) &&
					  ((normal_a[0]*x + normal_a[1]*y + normal_a[2]*z + dist_a[1]) <= 0) &&
					  ((normal_b[0]*x + normal_b[1]*y + normal_b[2]*z + dist_b[0]) >= 0) &&
					  ((normal_b[0]*x + normal_b[1]*y + normal_b[2]*z + dist_b[1]) <= 0) &&
					  ((normal_c[0]*x + normal_c[1]*y + normal_c[2]*z + dist_c[0]) >= 0) &&
					  ((normal_c[0]*x + normal_c[1]*y + normal_c[2]*z + dist_c[1]) <= 0);
#endif		
		included[i] = ((normal_a[0]*x + normal_a[1]*y + normal_a[2]*z + dist_a[0]) >= -eps) &&
					  ((normal_a[0]*x + normal_a[1]*y + normal_a[2]*z + dist_a[1]) <=  eps) &&
					  ((normal_b[0]*x + normal_b[1]*y + normal_b[2]*z + dist_b[0]) >= -eps) &&
					  ((normal_b[0]*x + normal_b[1]*y + normal_b[2]*z + dist_b[1]) <=  eps) &&
					  ((normal_c[0]*x + normal_c[1]*y + normal_c[2]*z + dist_c[0]) >= -eps) &&
					  ((normal_c[0]*x + normal_c[1]*y + normal_c[2]*z + dist_c[1]) <=  eps);
	}
	
	// now copy the atoms
	copy_partial_molecule(in, out, included);

	// finish
	delete [] included;
	return 1;
}

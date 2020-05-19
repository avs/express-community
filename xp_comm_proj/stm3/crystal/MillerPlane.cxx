
#include "MillerPlane_gen.h"
#include <math.h>
#include <avs/mathutil.h>
#include "../utils/copy_mol.h"

// below this value a quantity will be considered 0
#define TOL 1e-6F

int
STM3_CRYSTAL_MillerPlane::ComputePlane(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// n1 (OMXint read notify)
	// n2 (OMXint read notify)
	// n3 (OMXint read notify)
	// side (OMXenum read notify)
	// offset (OMXfloat read notify)
	// scale_u (OMXfloat read notify)
	// scale_v (OMXfloat read notify)
	// vertices (OMXfloat_array write)
	// out (MolecularDataType write)
	int i;
	float va[3], vb[3];
	
	// decision table
	int type = 0;
	if((int)n1 != 0) type += 1;
	if((int)n2 != 0) type += 2;
	if((int)n3 != 0) type += 4;

	// shortcut
	if(type == 0) return 0;
	
	// get the unit cell basis vectors
	float *b_arr = (float *)in.unit_cell.side_vectors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!b_arr) return 0;
	float basis[9];
	memcpy(basis, b_arr, 9 * sizeof(float));
	ARRfree(b_arr);
	
	// if no unit cell data available, then exit
	if(basis[0] == 0.0F && basis[1] == 0.0F && basis[2] == 0.0F) return 0;
	
	// set the three basis vectors
	float *a = &basis[0];
	float *b = &basis[3];
	float *c = &basis[6];
	
	// compute the axis basis intersections (bi left undefined if plane parallel to that axis)
	float bi[9];
	for(i=0; i < 3; ++i)
	{
		if((int)n1) bi[0+i] = basis[0+i]/(float)n1;
		if((int)n2) bi[3+i] = basis[3+i]/(float)n2;
		if((int)n3) bi[6+i] = basis[6+i]/(float)n3;
	}

	float normal[3], dist = 0.0F, incr = 1.0F;
	switch(type)
	{
	case 0:		// (000)
		return 0;

	case 1:		// (n00)
 		AVS_CROSS(b, c, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[0]));
		incr = AVS_LENGTH(a);
		break;

	case 2:		// (0n0)
 		AVS_CROSS(c, a, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[3]));
		incr = AVS_LENGTH(b);
		break;

	case 3:		// (nn0)
		for(i=0; i < 3; ++i)
		{
			va[i] = basis[6+i];
			vb[i] = bi[3+i] - bi[0+i];
		}
 		AVS_CROSS(vb, va, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[0]));
		for(i=0; i < 3; ++i) va[i] = basis[0+i] + basis[3+i];
		incr = AVS_LENGTH(va);
		break;

	case 4:		// (00n)
 		AVS_CROSS(a, b, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[6]));
		incr = AVS_LENGTH(c);
		break;

	case 5:		// (n0n)
		for(i=0; i < 3; ++i)
		{
			va[i] = basis[3+i];
			vb[i] = bi[6+i] - bi[0+i];
		}
 		AVS_CROSS(va, vb, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[0]));
		for(i=0; i < 3; ++i) va[i] = basis[0+i] + basis[6+i];
		incr = AVS_LENGTH(va);
		break;

	case 6:		// (0nn)
		for(i=0; i < 3; ++i)
		{
			va[i] = basis[0+i];
			vb[i] = bi[3+i] - bi[6+i];
		}
 		AVS_CROSS(va, vb, normal);
		AVS_UNITIZE(normal, TOL);
		dist = AVS_DOT(normal, (&bi[3]));
		for(i=0; i < 3; ++i) va[i] = basis[3+i] + basis[6+i];
		incr = AVS_LENGTH(va);
		break;

	case 7:		// (nnn)
		for(i=0; i < 3; ++i)
		{
			va[i] = bi[6+i] - bi[0+i];
			vb[i] = bi[3+i] - bi[0+i];
		}
 		AVS_CROSS(vb, va, normal);
		AVS_UNITIZE(normal, TOL);

		dist = AVS_DOT(normal, (&bi[0]));
		for(i=0; i < 3; ++i) va[i] = basis[0+i] + basis[3+i] + basis[6+i];
		incr = AVS_LENGTH(va);
		break;
	};

	// compute the offset
	dist = -dist - offset * incr;

	// now put the plane
	float *vertices_arr = (float *)vertices.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!vertices_arr) return 0;
	
	float *p0 = &vertices_arr[0];
	float *p1 = &vertices_arr[3];
	float *p2 = &vertices_arr[6];
	float *p3 = &vertices_arr[9];

	// compute four points on the plane
	switch(type)
	{
	case 0:		// (000)
		return 0;

	case 1:		// (n00)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[0+i];
			p1[i] = p0[i] + b[i] * scale_u;
			p2[i] = p1[i] + c[i] * scale_v;
			p3[i] = p0[i] + c[i] * scale_v;
		}
		break;

	case 2:		// (0n0)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[3+i];
			p1[i] = p0[i] + a[i] * scale_u;
			p2[i] = p1[i] + c[i] * scale_v;
			p3[i] = p0[i] + c[i] * scale_v;
		}
		break;

	case 3:		// (nn0)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[0+i] - (bi[3+i] - bi[0+i]) * (scale_u - 1.);
			p1[i] = bi[3+i] + (bi[3+i] - bi[0+i]) * (scale_u - 1.);
			p2[i] = p1[i] + c[i] * scale_v;
			p3[i] = p0[i] + c[i] * scale_v;
		}
		break;

	case 4:		// (00n)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[6+i];
			p1[i] = p0[i] + a[i] * scale_u;
			p2[i] = p1[i] + b[i] * scale_v;
			p3[i] = p0[i] + b[i] * scale_v;
		}
		break;

	case 5:		// (n0n)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[6+i] - (bi[6+i] - bi[0+i]) * (scale_u - 1.);
			p1[i] = bi[0+i] + (bi[6+i] - bi[0+i]) * (scale_u - 1.);
			p2[i] = p1[i] + b[i] * scale_v;
			p3[i] = p0[i] + b[i] * scale_v;
		}
		break;

	case 6:		// (0nn)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[3+i] - (bi[6+i] - bi[3+i]) * (scale_u - 1.);
			p1[i] = bi[6+i] + (bi[6+i] - bi[3+i]) * (scale_u - 1.);
			p2[i] = p1[i] + a[i] * scale_v;
			p3[i] = p0[i] + a[i] * scale_v;
		}
		break;

	case 7:		// (nnn)
		for(i=0; i < 3; ++i)
		{
			p0[i] = bi[0+i];
			p1[i] = bi[3+i] + (bi[3+i] - bi[0+i]) * (scale_u - 1.);
			p2[i] = p1[i];
			p3[i] = bi[6+i] + (bi[6+i] - bi[0+i]) * (scale_v - 1.);
		}
		break;
	};

	// add the unit cell origin
	float *origin_arr = (float *)in.unit_cell.origin.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!origin_arr)
	{
		ARRfree(vertices_arr);
		return 0;
	}
	
	for(i=0; i < 3; ++i)
	{
		vertices_arr[0+i] += origin_arr[i] + normal[i] * offset * incr;
		vertices_arr[3+i] += origin_arr[i] + normal[i] * offset * incr;
		vertices_arr[6+i] += origin_arr[i] + normal[i] * offset * incr;
		vertices_arr[9+i] += origin_arr[i] + normal[i] * offset * incr;
	}
	ARRfree(vertices_arr);
	
	// copy only the atoms above or below the plane
	bool *included = new bool[in.num_atoms];
	
	float *xyz = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz)
	{
		delete [] included;
		ARRfree(origin_arr);
		return 0;
	}
	
	if((int)side)
	{
		for(i=0; i < in.num_atoms; ++i)
		{
			included[i] = (normal[0]*(xyz[3*i+0]-origin_arr[0])+normal[1]*(xyz[3*i+1]-origin_arr[1])+normal[2]*(xyz[3*i+2]-origin_arr[2])+dist) >= 0;
		}
	}
	else
	{
		for(i=0; i < in.num_atoms; ++i)
		{
			included[i] = (normal[0]*(xyz[3*i+0]-origin_arr[0])+normal[1]*(xyz[3*i+1]-origin_arr[1])+normal[2]*(xyz[3*i+2]-origin_arr[2])+dist) <= 0;
		}
	}
	
	// now copy the atoms
	copy_partial_molecule(in, out, included);
	
	// release everything
	delete [] included;
	ARRfree(origin_arr);
	ARRfree(xyz);
	return 1;
}

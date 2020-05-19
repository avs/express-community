
#include "SummaryResidueNames_gen.h"

void compute_centroid(float *in_coords, int npt, float *out_coords)
{
	float x = 0.;
	float y = 0.;
	float z = 0.;
	
	for(int i=0; i < npt; ++i)
	{
		x += in_coords[i*3+0];
		y += in_coords[i*3+1];
		z += in_coords[i*3+2];
	}	

	out_coords[0] = x/npt;
	out_coords[1] = y/npt;
	out_coords[2] = z/npt;
}

int
STM3_BASE_SummaryResidueNames::SummaryResidueNames(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// summarize (OMXenum read req notify)
	// coords (OMXfloat_array write)
	// names (OMXstr_array write)
	int i;
	int d[2];
	char s[256];

	// do nothing if not asked to summarize
	if((int)summarize == 0) return 0;

	//OMparse_buffer(this->obj_id(OM_OBJ_RW), "coords=>;", 0);
	//OMparse_buffer(this->obj_id(OM_OBJ_RW), "names=>;", 0);

	// count the residues
	char last_residue[256], *curr_name;
	last_residue[0] = '\0';
	int nr = 0;
	for(i = 0; i < in.num_atoms; ++i)
	{
		curr_name = in.residue_name.ret_str_array_val(i, s);
		if(strcmp(curr_name, last_residue))
		{			
			strcpy(last_residue, curr_name);
			++nr;
		}
	}	

	// initialize the output dimensions
	names.set_array_size(nr);
	d[1] = nr;
	d[0] = 3;
	OMset_array_dims(coords.obj_id(OM_OBJ_RW), 2, d);

	// compute the new values		
	float *coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!coords_arr) return 0;

	float *xyz_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!xyz_arr)
	{
		ARRfree(coords_arr);
		return 0;
	}

	int r = 1;
	int start_curr_id = 0;

	strcpy(last_residue, in.residue_name.ret_str_array_val(0, s));
	names.set_str_array_val(0, last_residue);

	for(i=1; i < in.num_atoms; ++i)
	{
		curr_name = in.residue_name.ret_str_array_val(i, s);
		if(strcmp(curr_name, last_residue))
		{			
			strcpy(last_residue, curr_name);
			names.set_str_array_val(r, curr_name);

			compute_centroid(&xyz_arr[start_curr_id*3], i - start_curr_id, &coords_arr[(r-1)*3]);
			++r;

			start_curr_id = i;
		}
	}
	compute_centroid(&xyz_arr[start_curr_id*3], i - start_curr_id, &coords_arr[(r-1)*3]);

	ARRfree(coords_arr);
	ARRfree(xyz_arr);

	// return 1 for success
	return 1;
}

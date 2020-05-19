
#include "NCACTube_gen.h"


int
STM3_TUBES_NCACTube::FindAtoms(OMevent_mask event_mask, int seq_num)
{
	// molecule (MolecularDataType read req notify)
	// tube_coordinates (OMXfloat_array write)
	int i;
	char name[256];
	char *curr_name;
	int d[2];

	// set to zero points found
	d[1] = 0;
	d[0] = 3;
	OMset_array_dims(tube_coordinates.obj_id(OM_OBJ_RW), 2, d);
	
	// count the number of interesting atoms
	int TubeCoords_size = 0;
	for(i=0; i < molecule.num_atoms; i++)
	{
   		curr_name = molecule.atom_name.ret_str_array_val(i, name);
      	if(!strcmp(curr_name, "N") && strcmp(curr_name, "OXT"))
		{
   			for(; i < molecule.num_atoms; i++)
   			{
				curr_name = molecule.atom_name.ret_str_array_val(i, name);
        		if(!strcmp(curr_name, "CA")) break;
        	}
			if(i == molecule.num_atoms) break;
   			for(; i < molecule.num_atoms; i++)
   			{
				curr_name = molecule.atom_name.ret_str_array_val(i, name);
        		if(!strcmp(curr_name, "C")) break;
        	}

        	TubeCoords_size += 3;
      	}
   	}
	
	// none found   
	if(TubeCoords_size == 0) return 0;
  	
	// get the atoms coordinates
   	float *AllCoords = (float *)molecule.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!AllCoords) return 0;
	
 	// setup the output array
	d[1] = TubeCoords_size/3;
	d[0] = 3;
	OMset_array_dims(tube_coordinates.obj_id(OM_OBJ_RW), 2, d);
	float *tube_coordinates_arr = (float *)tube_coordinates.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!tube_coordinates_arr)
	{
		ARRfree(AllCoords);
		return 0;
	}
	
	// fill in array of tube coords
	int j = 0;
   	for(i=0; i < molecule.num_atoms; i++)
   	{
   		curr_name = molecule.atom_name.ret_str_array_val(i, name);
      	if(!strcmp(curr_name, "N") && strcmp(name, "OXT"))
		{
   			for(; i < molecule.num_atoms; i++)
   			{
				curr_name = molecule.atom_name.ret_str_array_val(i, name);
        		if(!strcmp(curr_name, "CA")) break;
        	}
			if(i == molecule.num_atoms) break;
			
        	tube_coordinates_arr[j++] = AllCoords[3*i+0];
        	tube_coordinates_arr[j++] = AllCoords[3*i+1];
        	tube_coordinates_arr[j++] = AllCoords[3*i+2];
			
		   	for(; i < molecule.num_atoms; i++)
   			{
				curr_name = molecule.atom_name.ret_str_array_val(i, name);
        		if(!strcmp(curr_name, "C")) break;
        	}

        	TubeCoords_size += 3;
      	}
   	}

	ARRfree(AllCoords);
	ARRfree(tube_coordinates_arr);
   
	return 1;
}


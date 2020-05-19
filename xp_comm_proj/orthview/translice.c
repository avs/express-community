
#include "gen.h"


int
TransSlice_Mesh(OMobj_id Mesh_Slice_id, OMevent_mask event_mask, int seq_num)
{
	/************************/
	/*	 Declare variables  */
	/************************/
	double plane;
	int  axis;
	OMobj_id _3D_Coords_id;
	int  Dimension = 0;
	int  xy_out_size = 0;
	float *xy_out = NULL; 
	int  x;
	int  xyz_Coords_size = 0;
	float *xyz_Coords = NULL;
	int  count = 0;
	int dec;
	float value_mod;
	float plane_mod;
	int tmp=1;

	/**************************/
	/*	 Get input values	  */
	/**************************/
	
	
	
	if (OMget_name_real_val(Mesh_Slice_id, OMstr_to_name("plane"), &plane) != 1) 
		plane = 0.0;

	if (OMget_name_int_val(Mesh_Slice_id, OMstr_to_name("axis"), &axis) != 1) 
		axis = 0;
		
	if (OMget_name_int_val(Mesh_Slice_id, OMstr_to_name("dec"), &dec) != 1) 
		dec = 0;
	
	_3D_Coords_id = OMfind_subobj(Mesh_Slice_id, OMstr_to_name("_3D_Coords"),OM_OBJ_RW);
	if (OMis_null_obj(_3D_Coords_id)) return(0);
	
	xyz_Coords = (float *)OMret_name_array_ptr(_3D_Coords_id, OMstr_to_name("xyz_Coords"), OM_GET_ARRAY_RW, &xyz_Coords_size, NULL);

	/***********************/
	/* Function's Body	  */
	/***********************/

	/* Setting tmp (Value to cut the float-vals */
	x=1;
	while(x<=dec)
	{
		tmp=tmp*10;
		x++;
	}

	/* count number of coordinates in selected slice */
	plane_mod=(plane*tmp);


	for (x=0 ; x < xyz_Coords_size/3 ; x++)
	  {
	  	value_mod=(xyz_Coords[x*3+axis]*tmp);
		 if ((value_mod-plane_mod)>-1 && (value_mod-plane_mod)<1) Dimension++;
	  }
	
	OMset_name_int_val(Mesh_Slice_id, OMstr_to_name("Dimension"), Dimension);
	
	xy_out = (float *)OMret_name_array_ptr(Mesh_Slice_id, OMstr_to_name("xy_out"), OM_GET_ARRAY_WR, &xy_out_size, NULL);
	
	for (x=0 ; x < xyz_Coords_size/3 ; x++)
	  {
		 value_mod=(xyz_Coords[x*3+axis]*tmp);
		
		 if ((value_mod-plane_mod)>-1 && (value_mod-plane_mod)<1)
			{
			  if (axis == 0)		 /* Sagital */
				 {
					xy_out[count*2] = xyz_Coords[x*3+1];
					xy_out[count*2+1] = xyz_Coords[x*3+2];
				 }
			  else if (axis == 1) /* Coronal */
				 {
					xy_out[count*2] = xyz_Coords[x*3];
					xy_out[count*2+1] = xyz_Coords[x*3+2];
				 }
			  else if (axis ==2)	 /* Transaxial */
				 {
					xy_out[count*2] = xyz_Coords[x*3];
					xy_out[count*2+1] = xyz_Coords[x*3+1];
				 }
			  count++;
			}
	  }
	if (xyz_Coords != NULL) 
	  ARRfree((char *)xyz_Coords);

	if (xy_out != NULL) 
	  ARRfree((char *)xy_out);
	return(1);
}

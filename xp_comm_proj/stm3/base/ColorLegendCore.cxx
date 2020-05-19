
#include "ColorLegendCore_gen.h"
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <set>
#include "../lib/stm3.h"
#include "mol_type.h"
using namespace std;

int
STM3_BASE_ColorLegendCore::ColorLegendCore(OMevent_mask event_mask, int seq_num)
{
	// in (DisplayParams read req notify)
	// x (OMXfloat_array read req notify)
	// y (OMXfloat_array read req notify)
	// z (OMXfloat read req notify)
	// fld (Mesh+Node_Data write)
	int natoms;
	set<int> unique_z;
	int *atom_z_arr;
	int i;
	
	// If atoms are not colored by atom type
	switch((int)in.atoms_representation)
	{
	case STICK:
	case LICORICE_STICK:
		// If the atoms have the default color, no legend is show
		// FALLTROUGH
	case BALL_AND_STICK_CHARGE:
	case CPK_CHARGE:
	case LICORICE_CHARGE:
		// If atoms colored by charge for now show nothing
		fld.nnodes = 0;
		labels.set_array_size(0);
		labels_pt.nnodes = 0;
		return 1;

	case COLORED_RESIDUE:
		// Find the list of unique residue names
		for(i=0; i < in.num_atoms; ++i)
		{
			char tmp[50];
			int rn = res_number(in.residue_name.ret_str_array_val(i, tmp));
			unique_z.insert(rn);
		}
		break;

	case BALL_AND_STICK:
	case CPK:
	case COLORED_STICK:
	case LICORICE:
	default:
		// Find the list of unique atom types
		atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD, &natoms);
		if(!atom_z_arr) return 0;
		for(i=0; i < natoms; ++i) unique_z.insert(atom_z_arr[i]);
		ARRfree(atom_z_arr);
		break;
	}
	
	// Set the output sizes
	fld.nnodes = unique_z.size();
	labels.set_array_size(unique_z.size());
	labels_pt.nnodes = unique_z.size();
	
	// Get the limits for the legend
	float xmin = -.9F;
	//float xmax = -.8F;
	float *x_arr = (float *)x.ret_array_ptr(OM_GET_ARRAY_RD);
	if(x_arr)
	{
		xmin = x_arr[0];
		//xmax = x_arr[1];
		ARRfree(x_arr);
	}
	float ymin = -.9F;
	float ymax =  .9F;
	float *y_arr = (float *)y.ret_array_ptr(OM_GET_ARRAY_RD);
	if(y_arr)
	{
		ymin = y_arr[0];
		ymax = y_arr[1];
		ARRfree(y_arr);
	}
	float zval = z;
	float ydelta = (unique_z.size() > 1) ? (ymax - ymin)/(unique_z.size()-1) : 0;
	
	// Setup the output spheres
	float *fld_coordinates = (float *)fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!fld_coordinates) return 0;
	float *labels_coordinates = (float *)labels_pt.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if(!labels_coordinates)
	{
		ARRfree(fld_coordinates);
		return 0;
	}
	float *rgb_arr = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(!rgb_arr)
	{
		ARRfree(fld_coordinates);
		ARRfree(labels_coordinates);
		return 0;
	}
	float *radius_arr = (float *)fld.node_data[1].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(!radius_arr)
	{
		ARRfree(fld_coordinates);
		ARRfree(labels_coordinates);
		ARRfree(rgb_arr);
		return 0;
	}
	
	set<int>::iterator iu;
	for(iu = unique_z.begin(), i=0; iu != unique_z.end(); iu++, ++i)
	{
		fld_coordinates[3*i+0] = xmin;
		fld_coordinates[3*i+1] = ymax - i * ydelta;
		fld_coordinates[3*i+2] = zval;
		
		labels_coordinates[3*i+0] = xmin + label_offset;
		labels_coordinates[3*i+1] = ymax - i * ydelta;
		labels_coordinates[3*i+2] = zval;
	}
	
	if((int)in.atoms_representation == COLORED_RESIDUE)
	{
		for(iu = unique_z.begin(), i=0; iu != unique_z.end(); iu++, ++i)
		{
			labels.set_str_array_val(i, res_name(*iu));

			radius_arr[i] = 1.5F * scale;

			res_to_color(res_name(*iu), &rgb_arr[3*i]);
		}
	}
	else
	{
		for(iu = unique_z.begin(), i=0; iu != unique_z.end(); iu++, ++i)
		{
			labels.set_str_array_val(i, atom_properties[*iu].atom_name);

			radius_arr[i] = atom_properties[*iu].rvdw * scale;

			rgb_arr[3*i+0] = atom_properties[*iu].color[0];
			rgb_arr[3*i+1] = atom_properties[*iu].color[1];
			rgb_arr[3*i+2] = atom_properties[*iu].color[2];
		}
	}

	ARRfree(fld_coordinates);
	ARRfree(labels_coordinates);
	ARRfree(rgb_arr);
	ARRfree(radius_arr);
	
	return 1;
}

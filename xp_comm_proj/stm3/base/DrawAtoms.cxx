
#include "DrawAtoms_gen.h"
#include "../lib/stm3.h"
#include "mol_type.h"
#include <float.h>

// Defined by DrawPOV.cxx
extern void value_to_color(float *node_data, int nnodes, float nd_min, float nd_max, float *rgb);

int
STM3_BASE_DrawAtoms::DrawAtoms(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read req notify)
	// representation (OMXint read req notify)
	// normal_scale (OMXfloat read req notify)
	// cpk_scale (OMXfloat read req notify)
	// licorice_scale (OMXfloat read req notify)
	// fld (Mesh+Node_Data write)
	int i;
	int *bonds, *count_bonds;
	
	// No atoms will be computed with those representations
	if((int)representation == COLORED_RESIDUE || (int)representation == STICK)
	{
		fld.nnodes = 0;
		return 1;
	}
		
	// Prepare the output field
	fld.nnodes = in.num_atoms;
	
	int *atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!atom_z_arr) return 0;
	
	float *fld_color = (float *)fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(!fld_color)
	{
		ARRfree(atom_z_arr);
		return 0;
	}
	
	float *fld_radius = (float *)fld.node_data[1].values.ret_array_ptr(OM_GET_ARRAY_WR);
    if(!fld_radius)
	{
		ARRfree(atom_z_arr);
		ARRfree(fld_color);
		return 0;
	}
	
	// get the default colors for the uniform color mode
	float default_r = 0.5F, default_g = 1.0F, default_b = 0.5F;
	float *default_colors_arr = (float *)default_colors.ret_array_ptr(OM_GET_ARRAY_RD);
	if(default_colors_arr)
	{
		default_r = default_colors_arr[0];
		default_g = default_colors_arr[1];
		default_b = default_colors_arr[2];
		
		ARRfree(default_colors_arr);
	}
	
	// If requested access the charge field computing max and min values
	float *charge_arr = 0;
	float cmin = FLT_MAX;
	float cmax = FLT_MIN;
	switch((int)representation)
	{
	case BALL_AND_STICK_CHARGE:
	case CPK_CHARGE:
	case LICORICE_CHARGE:
		charge_arr = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!charge_arr)
		{
			ARRfree(atom_z_arr);
			ARRfree(fld_color);
			ARRfree(fld_radius);
			return 0;
		}
		if((int)use_manual_limits)
		{
			float *charge_manual_limits_arr = (float *)charge_manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
			if(charge_manual_limits_arr)
			{
				cmin = charge_manual_limits_arr[0];
				cmax = charge_manual_limits_arr[1];
				ARRfree(charge_manual_limits_arr);
			}
			else
			{
				for(i=0; i < in.num_atoms; ++i)
				{
					if(charge_arr[i] > cmax)      cmax = charge_arr[i];
					else if(charge_arr[i] < cmin) cmin = charge_arr[i];
				}
			}
		}
		else
		{
			for(i=0; i < in.num_atoms; ++i)
			{
				if(charge_arr[i] > cmax)      cmax = charge_arr[i];
				else if(charge_arr[i] < cmin) cmin = charge_arr[i];
			}
			float *charge_manual_limits_arr = (float *)charge_manual_limits.ret_array_ptr(OM_GET_ARRAY_RW);
			if(charge_manual_limits_arr)
			{
				if(charge_manual_limits_arr[0] > cmin) charge_manual_limits_arr[0] = cmin; else cmin = charge_manual_limits_arr[0];
				if(charge_manual_limits_arr[1] < cmax) charge_manual_limits_arr[1] = cmax; else cmax = charge_manual_limits_arr[1];
				ARRfree(charge_manual_limits_arr);
			}
		}
	};
		
	switch((int)representation)
	{
	default:
	case BALL_AND_STICK:
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = atom_properties[atom_z_arr[i]].rvdw * normal_scale;
			fld_color[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			fld_color[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			fld_color[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		break;
		
	case CPK:
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = atom_properties[atom_z_arr[i]].rvdw * cpk_scale;
			fld_color[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			fld_color[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			fld_color[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		break;
		
	case COLORED_STICK:
		bonds = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		count_bonds = new int[in.num_atoms];
		for(i=0; i < in.num_atoms; ++i) count_bonds[i] = 0;
		if(bonds) for(i=0; i < 2*in.num_bonds; ++i) ++count_bonds[bonds[i]];
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = (count_bonds[i]) ? licorice_scale : 0.0F;
			fld_color[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			fld_color[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			fld_color[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		if(bonds) ARRfree(bonds);
		delete [] count_bonds;
		break;

	case COLORED_RESIDUE:
	case STICK:
		// not reached, but anyway...
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = 0.0F;
			fld_color[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			fld_color[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			fld_color[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		break;
		
	case LICORICE:
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = licorice_scale;
			fld_color[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			fld_color[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			fld_color[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		break;

	case LICORICE_STICK:
		for(i=0; i < in.num_atoms; ++i)
		{
			fld_radius[i]    = licorice_scale;
			fld_color[3*i+0] = default_r;
			fld_color[3*i+1] = default_g;
			fld_color[3*i+2] = default_b;
		}
		break;
	
	case BALL_AND_STICK_CHARGE:
		value_to_color(charge_arr, in.num_atoms, cmin, cmax, fld_color);
		for(i=0; i < in.num_atoms; ++i) fld_radius[i] = atom_properties[atom_z_arr[i]].rvdw * normal_scale;
		break;
		
	case CPK_CHARGE:
		value_to_color(charge_arr, in.num_atoms, cmin, cmax, fld_color);
		for(i=0; i < in.num_atoms; ++i) fld_radius[i] = atom_properties[atom_z_arr[i]].rvdw * cpk_scale;
		break;
		
	case LICORICE_CHARGE:
		value_to_color(charge_arr, in.num_atoms, cmin, cmax, fld_color);
		for(i=0; i < in.num_atoms; ++i) fld_radius[i] = licorice_scale;
		break;		
	}
	
	if(charge_arr) ARRfree(charge_arr);
	ARRfree(fld_color);
	ARRfree(fld_radius);
	ARRfree(atom_z_arr);

	return 1;
}


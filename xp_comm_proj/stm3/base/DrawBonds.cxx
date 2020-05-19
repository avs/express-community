
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#define __USE_STRING_INLINES
#endif
#include <string.h>

// the following enables the inline function sincosf
#ifdef __linux__
#define FOUND_SINCOS
#define __USE_GNU
#endif
#include <math.h>

#include <avs/math.h>
#include <avs/gd_def.h>

#define TWOPI     2*M_PI
#define VISMINVAL 1.0e-6

#include "DrawBonds_gen.h"
#include "../lib/stm3.h"
#include "mol_type.h"

// Defined by DrawPOV.cxx
extern void value_to_color(float *node_data, int nnodes, float nd_min, float nd_max, float *rgb);

static void VISline_2_tube(float in[2][3], float radius, int res, float *out);

int
STM3_BASE_DrawBonds::DrawBonds(OMevent_mask event_mask, int seq_num)
{
	// coords (OMXfloat_array read notify)
	int coords_size;
	float *coords_arr;

	// connect (OMXint_array read notify)
	int connect_size;
	int *connect_arr;

	// rgb (OMXfloat_array read notify)
	int rgb_size;

	// atom_names (OMXstr_array read notify)
	// lineflag (OMXint read notify)
	// facets (OMXint read notify)
	// bondradius (OMXfloat read notify)
	// out (Mesh+Node_Data write)
	int i, n;
	int *atom_z_arr;
	
	// set a null output just in case
	out.nnodes = 0;
	out.ncell_sets = 0;

	// no atoms or CPK representation (bonds not needed) so draw nothing
	int natoms = in.num_atoms;
    if(natoms <= 0 || (int)representation == CPK || (int)representation == CPK_CHARGE) return 1;
	
	// sanity check
	if(facets < 3) facets = 3;
	if(bondradius <= 0.) bondradius = 0.1;

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
	
	// access the input data
	coords_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!coords_arr) return 0;

	connect_arr = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD, &connect_size);
	if(!connect_arr)
	{
		ARRfree(coords_arr);
		return 0;
	}
	
	// If requested access the charge field computing max and min values
	float *charge_arr = 0;
	float cmin = 0;
	float cmax = 0;
	switch((int)representation)
	{
	case BALL_AND_STICK_CHARGE:
	case CPK_CHARGE:
	case LICORICE_CHARGE:
		charge_arr = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!charge_arr)
		{
			ARRfree(coords_arr);
			ARRfree(connect_arr);
			return 0;
		}
		float *charge_manual_limits_arr = (float *)charge_manual_limits.ret_array_ptr(OM_GET_ARRAY_RD);
		if(charge_manual_limits_arr)
		{
			cmin = charge_manual_limits_arr[0];
			cmax = charge_manual_limits_arr[1];
			ARRfree(charge_manual_limits_arr);
		}
	};

	// create the color array
	rgb_size = 3 * natoms;
	float *rgb_arr = (float *)ARRalloc(NULL, DTYPE_FLOAT, rgb_size, NULL);
	
	switch((int)representation)
	{
	default:
	case BALL_AND_STICK:
	case LICORICE:
	case COLORED_STICK:
		atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!atom_z_arr)
		{
			ARRfree(connect_arr);
			ARRfree(coords_arr);
			return 0;
		}
		for(i=0; i < natoms; ++i)
		{
			rgb_arr[3*i+0] = atom_properties[atom_z_arr[i]].color[0];
			rgb_arr[3*i+1] = atom_properties[atom_z_arr[i]].color[1];
			rgb_arr[3*i+2] = atom_properties[atom_z_arr[i]].color[2];
		}
		ARRfree(atom_z_arr);
		break;

	case CPK:
		// not reached, but keep it anyway
		ARRfree(connect_arr);
		ARRfree(coords_arr);
		ARRfree(rgb_arr);
		return 1;
		break;

	case COLORED_RESIDUE:
		for(i=0; i < natoms; ++i)
		{
			char tmp[50];
			res_to_color(in.residue_name.ret_str_array_val(i, tmp), &rgb_arr[3*i]);
		}
		break;
		
	case STICK:
	case LICORICE_STICK:
		for(i=0; i < natoms; ++i)
		{
			rgb_arr[3*i+0] = default_r;
			rgb_arr[3*i+1] = default_g;
			rgb_arr[3*i+2] = default_b;
		}
		break;
	
	case BALL_AND_STICK_CHARGE:
	case CPK_CHARGE:
	case LICORICE_CHARGE:
		value_to_color(charge_arr, natoms, cmin, cmax, rgb_arr);
		break;		
	}
	if(charge_arr) ARRfree(charge_arr);
	
	// get here if needed the bond type array so can be adjusted for splitting
	int *bond_type_arr = NULL;
	if((int)lineflag == LINEFLAG_ALL_H || (int)lineflag == LINEFLAG_H_BONDS)
	{
		bond_type_arr = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!bond_type_arr)
		{
			ARRfree(connect_arr);
			ARRfree(coords_arr);
			ARRfree(rgb_arr);
			return 0;
		}
	}
	
	// if the bonds must be split colored, adjust the input arrays adding two fake atoms at the middle of the bonds
	if((int)split && (int)representation != STICK && (int)representation != LICORICE_STICK && connect_size != 0)
	{
		int nbonds = connect_size / 2;
		
		// add atoms
		int fnatoms = natoms + 2 * nbonds;
		float *fcoords_arr = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3 * fnatoms, NULL);
		memcpy(fcoords_arr, coords_arr, sizeof(float) * 3 * natoms);
		for(i=0, n=3*natoms; i < nbonds; ++i)
		{
			// compute the middle coordinates and add two fake atoms
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			fcoords_arr[n++] = (coords_arr[3*a1+0] + coords_arr[3*a2+0]) / 2.0F;
			fcoords_arr[n++] = (coords_arr[3*a1+1] + coords_arr[3*a2+1]) / 2.0F;
			fcoords_arr[n++] = (coords_arr[3*a1+2] + coords_arr[3*a2+2]) / 2.0F;
			
			fcoords_arr[n++] = (coords_arr[3*a1+0] + coords_arr[3*a2+0]) / 2.0F;
			fcoords_arr[n++] = (coords_arr[3*a1+1] + coords_arr[3*a2+1]) / 2.0F;
			fcoords_arr[n++] = (coords_arr[3*a1+2] + coords_arr[3*a2+2]) / 2.0F;
		}
		
		// change colors
		float *frgb_arr = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3 * fnatoms, NULL);
		memcpy(frgb_arr, rgb_arr, sizeof(float) * 3 * natoms);
		for(i=0, n=3*natoms; i < nbonds; ++i)
		{
			// distribute the real atoms colors to the two fake atoms
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			frgb_arr[n++] = rgb_arr[3*a1+0];
			frgb_arr[n++] = rgb_arr[3*a1+1];
			frgb_arr[n++] = rgb_arr[3*a1+2];
			
			frgb_arr[n++] = rgb_arr[3*a2+0];
			frgb_arr[n++] = rgb_arr[3*a2+1];
			frgb_arr[n++] = rgb_arr[3*a2+2];
		}
		
		// add bonds
		int fnbonds = 2 * nbonds;
		int *fconnect_arr = (int *)ARRalloc(NULL, DTYPE_INT, 2 * fnbonds, NULL);
		int fa = natoms;
		for(i=n=0; i < nbonds; ++i)
		{
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			fconnect_arr[n++] = a1;
			fconnect_arr[n++] = fa++;

			fconnect_arr[n++] = fa++;
			fconnect_arr[n++] = a2;
		}
		
		// switch the two sets of arrays
		ARRfree(coords_arr);
		coords_arr = fcoords_arr;
		natoms = fnatoms;
		
		ARRfree(rgb_arr);
		rgb_arr = frgb_arr;
		rgb_size = 3 * fnatoms;
		
		ARRfree(connect_arr);
		connect_arr = fconnect_arr;
		connect_size = 2 * fnbonds;
		
		if(bond_type_arr)
		{
			// duplicate bond types (if needed)
			int *fbond_type_arr = (int *)ARRalloc(NULL, DTYPE_INT, fnbonds, NULL);
			for(i=n=0; i < nbonds; ++i)
			{
				fbond_type_arr[n++] = bond_type_arr[i];
				fbond_type_arr[n++] = bond_type_arr[i];
			}
			ARRfree(bond_type_arr);
			bond_type_arr = fbond_type_arr;
		}
	}
	
   	OMobj_id out_id = out.obj_id(OM_OBJ_RW);
   	OMobj_id cell_set;

	// lineflag sets the type of resulting bonds:
	//		0:	create lines
	//		1:	create tubes
	//		2:	line for H-bonds and regular H atoms bounds, tube otherwise
	//		3:	line for H-bonds only, tube otherwise
	if((int)lineflag == LINEFLAG_ALL_H || (int)lineflag == LINEFLAG_H_BONDS)
	{
		int  i, j, r, l, o, b, e, p, bg;
		
		// total bounds
		int n_bonds = connect_size / 2;
		
		// compute number of H-bonds and regular H bounds
		int n_h_bonds = 0;
		
		for(i=0; i < n_bonds; i++)
		{
			if((bond_type_arr[i] == H_BOND || bond_type_arr[i] == SPECIAL_BOND) || (bond_type_arr[i] == H_NORMAL_BOND && (int)lineflag == LINEFLAG_ALL_H))
			{
				++n_h_bonds;
			}
		}

		out.nnodes =  2 * (n_bonds-n_h_bonds) * facets + 2 * n_h_bonds;
		
		// create the appropriate cell_sets. First the tubes then the lines
		FLDadd_cell_set(out_id, "Quad");
		FLDget_cell_set(out_id, 0, &cell_set);
		FLDset_ncells(cell_set, (n_bonds-n_h_bonds) * facets);
		if(n_h_bonds)
		{
			FLDadd_cell_set(out_id, "Line");
			out.cell_set[1].ncells = n_h_bonds;
		}
		
		// create the tubes
   		float *out_coords = NULL; 
   		int *out_connect = NULL; 

		rgb_size = out.nnodes * 3;
		float* out_rgb = (float *)ARRalloc(NULL, DTYPE_FLOAT, rgb_size, NULL);
		FLDget_coord(out_id, &out_coords, &coords_size, OM_GET_ARRAY_WR);
		if(!out_coords)
		{
			ARRfree(coords_arr);
			ARRfree(connect_arr);
			ARRfree(rgb_arr);
			ARRfree(bond_type_arr);
			printf("\nerror writing coords");
			return 0;
		}
		FLDget_node_connect(cell_set, &out_connect, &connect_size, OM_GET_ARRAY_WR);
#if 0
		if(!out_connect)
		{
			ARRfree(coords_arr);
			ARRfree(connect_arr);
			ARRfree(rgb_arr);
			ARRfree(bond_type_arr);
			printf("\nerror reading conn");
			return 0;
		}
#endif		 
		l = o = r = p = 0;
		for(i=0; i < n_bonds; i++)
		{
			// verify if it is an H-bond
			if((bond_type_arr[i] == H_BOND || bond_type_arr[i] == SPECIAL_BOND) || (bond_type_arr[i] == H_NORMAL_BOND && (int)lineflag == LINEFLAG_ALL_H))
			{
				continue;
			}
			
			b = connect_arr[i*2+0];
			e = connect_arr[i*2+1];
			float pt[2][3];
			memcpy(pt, &coords_arr[3*b], sizeof(float) * 3);
			memcpy(&pt[1][0], &coords_arr[3*e], sizeof(float) * 3);

			bg = o;
       		for(j=0; j < facets; j++)
			{
				memcpy(&out_rgb[r], &rgb_arr[3*b], sizeof(float) * 3); r+=3;
				memcpy(&out_rgb[r], &rgb_arr[3*e], sizeof(float) * 3); r+=3;

				out_connect[o] = l/2+0;
				o++; l++;
				out_connect[o] = l/2+1;
				o++; l++;
				if (j != (facets-1))
				{
					out_connect[o] = out_connect[o-1]+2;
					o++; l++;
					out_connect[o] = out_connect[o-1]-1;
					o++; l++;
            	}
            	else
				{
               		out_connect[o] = out_connect[bg+1];
               		o++; l++;
               		out_connect[o] = out_connect[bg];
               		o++; l++;
            	}
       		}

       		VISline_2_tube(pt, bondradius, facets, &out_coords[6 * facets * p]);
			p++;
     	}
     	if(out_connect) ARRfree(out_connect);
		
		// now the line cell set
		if(n_h_bonds)
		{
			out_connect = (int *)out.cell_set[1].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
			if(out_connect)
			{
				l = o = 0;
				for(i=0; i < n_bonds; i++)
				{
					// verify if it is an H-bond
					if((bond_type_arr[i] == H_BOND || bond_type_arr[i] == SPECIAL_BOND) || (bond_type_arr[i] == H_NORMAL_BOND && (int)lineflag == LINEFLAG_ALL_H))
					{
						b = connect_arr[i*2+0];
						e = connect_arr[i*2+1];

						// copy coordinates
						memcpy(&out_coords[6 * facets * p + 6*l], &coords_arr[3*b], sizeof(float) * 3);
						memcpy(&out_coords[6 * facets * p + 6*l + 3], &coords_arr[3*e], sizeof(float) * 3);

						// copy value
						memcpy(&out_rgb[r], &rgb_arr[3*b], sizeof(float) * 3); r+=3;
						memcpy(&out_rgb[r], &rgb_arr[3*e], sizeof(float) * 3); r+=3;

						// add connectivity
						out_connect[o] = 2 * facets * p + 2*l; o++;
						out_connect[o] = 2 * facets * p + 2*l + 1; o++;
						l++;
					}
				}
				ARRfree(out_connect);
			}
		}
		
     	FLDset_node_data(out_id, 0, (char *)out_rgb, DTYPE_FLOAT, rgb_size, OM_SET_ARRAY_FREE);
		ARRfree(bond_type_arr);
     	ARRfree(out_coords);
	}
	else if((int)lineflag == LINEFLAG_TUBE)
	{
		int ntubes = connect_size / 2;
		int nnodes = 2 * ntubes * facets;
   		float *out_coords = NULL; 
   		int *out_connect = NULL; 

		FLDadd_cell_set(out_id, "Quad");
		FLDget_cell_set(out_id, 0, &cell_set);
		FLDset_ncells(cell_set, ntubes * facets);
		FLDset_nnodes(out_id, nnodes);
		rgb_size = 3 * nnodes;
		float* out_rgb = (float *)ARRalloc(NULL, DTYPE_FLOAT, rgb_size, NULL);
		FLDget_coord(out_id, &out_coords, &coords_size, OM_GET_ARRAY_WR);
		FLDget_node_connect(cell_set, &out_connect, &connect_size, OM_GET_ARRAY_WR);

		int  i, j, r, l, o, b, e, bg;
		l = o = r = 0;
		for(i=0; i < ntubes; i++)
		{
			b = connect_arr[i*2+0];
			e = connect_arr[i*2+1];
			float pt[2][3];
			memcpy(pt, &coords_arr[3*b], sizeof(float) * 3);
			memcpy(&pt[1][0], &coords_arr[3*e], sizeof(float) * 3);
			
			bg = o;
       		for(j=0; j < facets; j++)
			{
				memcpy(&out_rgb[r], &rgb_arr[3*b], sizeof(float) * 3); r+=3;
				memcpy(&out_rgb[r], &rgb_arr[3*e], sizeof(float) * 3); r+=3;

				out_connect[o] = l/2+0;
				o++; l++;
				out_connect[o] = l/2+1;
				o++; l++;
				if (j != (facets-1))
				{
					out_connect[o] = out_connect[o-1]+2;
					o++; l++;
					out_connect[o] = out_connect[o-1]-1;
					o++; l++;
            	}
            	else
				{
               		out_connect[o] = out_connect[bg+1];
               		o++; l++;
               		out_connect[o] = out_connect[bg];
               		o++; l++;
            	}
       		}

       		VISline_2_tube(pt, bondradius, facets, &out_coords[6 * facets * i]);
     	}
     	FLDset_node_data(out_id, 0, (char *)out_rgb, DTYPE_FLOAT, rgb_size, OM_SET_ARRAY_FREE);
		
     	if(out_coords != NULL) ARRfree(out_coords);
     	if(out_connect != NULL)  ARRfree(out_connect);
	}
	else // LINEFLAG_LINE
	{
		out.nnodes = natoms;
		
		FLDadd_cell_set(out_id, "Line");
		FLDget_cell_set(out_id, 0, &cell_set);
		FLDset_ncells(cell_set, connect_size/2);
		FLDset_node_connect(cell_set, connect_arr, connect_size, OM_SET_ARRAY_STATIC);
		FLDset_coord(out_id, coords_arr, natoms*3, OM_SET_ARRAY_STATIC);
		FLDset_node_data(out_id, 0, (char *)rgb_arr, DTYPE_FLOAT, rgb_size, OM_SET_ARRAY_STATIC);
	}

	ARRfree(coords_arr);
	ARRfree(connect_arr);
	ARRfree(rgb_arr);
	return 1;
}

/* ===========================================================================
   Calculates the matrix to rotate a line to the same orientation as two points
   in 3D space
   Inputs:
      begin[3]  : (float*) the begin point (X,Y,Z)
      end[3]    : (float*) the end point (X,Y,Z)
   Outputs:
      mat[4][4] : (float*) the rotation matrix (quaternion)
   Return value:
      none
*/
static void VISgen_rot_matrix3D(float *begin, float *end, float mat[4][4])
{
   float cy, cz, dx, dy, dz, hxy, hxyz, sy, sz;
   //float dp1[3], dp2[3];

   // fill ry and rz as identity matrices
   float ry[4][4] = {
      {1.0, 0.0, 0.0, 0.0},
      {0.0, 1.0, 0.0, 0.0},
      {0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0}
   };
   float rz[4][4] = {
      {1.0, 0.0, 0.0, 0.0},
      {0.0, 1.0, 0.0, 0.0},
      {0.0, 0.0, 1.0, 0.0},
      {0.0, 0.0, 0.0, 1.0}
    };

   // set origin
   //dp1[0] = dp1[1] = dp1[2] = 0.;
   
   // set point
   dx = end[0] - begin[0];
   dy = end[1] - begin[1];
   dz = end[2] - begin[2];

   // calculate lengths
   //dp2[0] = dx;
   //dp2[1] = dy;
   //dp2[2] = 0.;
   hxy = sqrt(dx * dx + dy * dy);
   //dp2[2] = dz;
   hxyz = sqrt(dx * dx + dy * dy + dz * dz);

   // Y rotation first
   if(hxyz > VISMINVAL)
   {
      sy =  dz/hxyz;
      cy = hxy/hxyz;
   }
   else
   {
      sy = 0.;
      cy = 1.;
   }
   
   // then Z rotation
   if(hxy > VISMINVAL)
   {
      sz = dy/hxy;
      cz = dx/hxy;
   }
   else
   {
      sz = 0.;
      if(dx > 0)
         cz =  1.;
      else
         cz = -1.;
   }
   
   // fill ry - i runs from LHS to RHS, j from T to B
   ry[0][0] =  cy;
   ry[2][0] =  sy;
   ry[0][2] = -sy;
   ry[2][2] =  cy;
  
   // fill rz - i runs from LHS to RHS, j from T to B
   rz[0][0] =  cz;
   rz[1][0] =  sz;
   rz[0][1] = -sz;
   rz[1][1] =  cz;

   // now concatenate the matrices
   MATmat4_multiply(rz, ry, mat);
}

/* ===========================================================================
   Rotates the point in 3D space using the matrix
   Inputs:
      pt[3]     : (float*) the point (X,Y,Z) to be rotated
      mat[4][4] : (float*) the rotation matrix (quaternion)
   Outputs:
      pt[3] : the rotated point
   Return value:
      none
*/
static inline void VISdo_rot_point3D(float pt[3], float mat[4][4])
{
   float rpt[4];

   // generate the point in homogeneous coords then multiply by the rotation matrix
	rpt[0] = mat[0][0]*pt[0] + mat[0][1]*pt[1] + mat[0][2]*pt[2] + mat[0][3];
 	rpt[1] = mat[1][0]*pt[0] + mat[1][1]*pt[1] + mat[1][2]*pt[2] + mat[1][3];
	rpt[2] = mat[2][0]*pt[0] + mat[2][1]*pt[1] + mat[2][2]*pt[2] + mat[2][3];
	//rpt[3] = mat[3][0]*pt[0] + mat[3][1]*pt[1] + mat[3][2]*pt[2] + mat[3][3];
  
   pt[0] = rpt[0];
   pt[1] = rpt[1];
   pt[2] = rpt[2];
}

/* ===========================================================================
   Generates a tube around the line defined by two points.
   Inputs:
      in[2][3] : (float) begin and end points
      radii[2] : (float) the radii at the begin and end points
      res      : (int) the number of segments in the tube
   Outputs:
      out[res*2][3] : (float) the nodes defining the tube
   Return value:
      none
*/
static void VISline_2_tube(float in[2][3], float radius, int res, float *out) {
   int k, r;
   float dp1[3], dp2[3], len, mat[4][4]; 

   // calculate length of this segment
   dp1[0] = in[0][0];
   dp1[1] = in[0][1];
   dp1[2] = in[0][2];
   dp2[0] = in[1][0];
   dp2[1] = in[1][1];
   dp2[2] = in[1][2];
   len = sqrt((dp2[0] - dp1[0])*(dp2[0] - dp1[0]) +
              (dp2[1] - dp1[1])*(dp2[1] - dp1[1]) +
              (dp2[2] - dp1[2])*(dp2[2] - dp1[2]));

   // generate rotation matrix
   VISgen_rot_matrix3D(dp1, dp2, mat);

   // do revolution, rotation & translation
   for (k = r = 0; r < res; r++)
   {
	  float s, c;
#ifdef FOUND_SINCOS
	  sincosf(TWOPI*(float)r/(float)res, &s, &c);
#else
	  s = sin(TWOPI*(float)r/(float)res);
	  c = cos(TWOPI*(float)r/(float)res);
#endif   
      dp1[0] = 0.;
      dp1[1] = radius * s;
      dp1[2] = radius * c;
      dp2[0] = len;
      dp2[1] = radius * s;
      dp2[2] = radius * c;

      // rotate points
      VISdo_rot_point3D(dp1, mat);
      VISdo_rot_point3D(dp2, mat);
	  
      // translate points
      dp1[0] += in[0][0];
      dp1[1] += in[0][1];
      dp1[2] += in[0][2];
      dp2[0] += in[0][0];
      dp2[1] += in[0][1];
      dp2[2] += in[0][2];
	  
      // set out
      memcpy(&out[k], dp1, sizeof(float) * 3); k+=3;
      //out[k++] = dp1[0]; 
      //out[k++] = dp1[1];
      //out[k++] = dp1[2];
      memcpy(&out[k], dp2, sizeof(float) * 3); k+=3;
      //out[k++] = dp2[0];
      //out[k++] = dp2[1];
      //out[k++] = dp2[2];
   }
}

//
//   Author: I. Curington 8 July, 1999
//   Advanced Visual Systems, Inc.
//   ianc@avs.com
//
 
#include "WritePOV_gen.h"
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <avs/f_utils.h>
#include "../base/mol_type.h"
#include "../lib/stm3.h"
#include <avs/mathutil.h>
#include <avs/gd_def.h>
#include "avs/math.h"

void write_smooth_cylinder(FILE *fp, float *xyz1, float *xyz2, float radius, float r1, float g1, float b1, float r2, float g2, float b2)
{
	float v[3];
	v[0] = xyz2[0] - xyz1[0];
	v[1] = xyz2[1] - xyz1[1];
	v[2] = xyz2[2] - xyz1[2];
	double length = AVS_LENGTH(v);
	v[0] /= length;
	v[1] /= length;
	v[2] /= length;

	double dist = AVS_DOT(v, xyz1);
	double phase = -dist/length;

	fprintf(fp, "    cylinder {\n");
	fprintf(fp, "        <%.3f, %.3f %.3f>", xyz1[0], xyz1[1], xyz1[2]);
	fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz2[0], xyz2[1], xyz2[2], radius);
	fprintf(fp, "        pigment { gradient <%.3f, %.3f, %.3f> scale %.4lf phase %.4lf\n", v[0], v[1], v[2], length, phase);  
    fprintf(fp, "                  color_map {\n                    [0.0 rgb <%.3f, %.3f, %.3f>]\n", r1, g1, b1);
    fprintf(fp, "                    [1.0 rgb <%.3f, %.3f, %.3f>]\n                  }\n        }\n    }\n", r2, g2, b2);
}

void write_camera_and_lights(FILE *fp, float cx, float cy, float cz, float cameraz, float fov, float zoffset)
{

	fprintf(fp, "//## Start of Sample Camera & Light Data\n\n");
	fprintf(fp, "#global_settings { assumed_gamma 2.2 }\n");
	fprintf(fp, "#include \"colors.inc\"\n");
	fprintf(fp, "camera {\n");
	fprintf(fp, "    location <%.3f, %.3f, %.3f>\n", cx, cy, cameraz);
	fprintf(fp, "    look_at <%.3f, %.3f, %.3f>\n", cx, cy, cz);
	fprintf(fp, "    up <1, 0, 0>\n");
	fprintf(fp, "    angle %.3f\n", fov);
	if(zoffset >= 0.001) fprintf(fp, "    translate <0.000, 0.000, %.3f> // undo the bug fixing offset\n", -zoffset);
	fprintf(fp, "}\n");
	fprintf(fp, "light_source { <%.2f, %.2f, %.2f> color rgb < 0.7, 0.7, 0.7 > }\n", cx-50, cy+50, cz-70-zoffset);
	fprintf(fp, "light_source { <%.2f, %.2f, %.2f> color rgb < 0.7, 0.7, 0.7 > }\n", cx+50, cy-50, cz-70-zoffset);
	fprintf(fp, "//## End of Sample Camera & Light Data\n\n");
}

void write_default_color(FILE *fp, OMXfloat_array &default_colors, int finish, float default_transparency)
{
	fprintf(fp, "#declare trans = %.3f;\n", default_transparency);

	float *default_colors_arr = (float *)default_colors.ret_array_ptr(OM_GET_ARRAY_RD);
	fprintf(fp, "#default {\n");
	fprintf(fp, "    texture {\n");
	if(default_colors_arr)
	{
		fprintf(fp, "        pigment { color rgbt <%.3f, %.3f, %.3f, 0.0> }\n", default_colors_arr[0], default_colors_arr[1], default_colors_arr[2]);
	
		ARRfree(default_colors_arr);
	}
	else
	{
		fprintf(fp, "        pigment { color rgbt <0.5, 0.5, 0.7, 0.0> }\n");
	}

	// get the default finish
	switch(finish)
	{
	default:
	case 0:
		fprintf(fp, "        finish { diffuse 0.3 specular 0.3 ambient 0.3 roughness 0.007 }\n");
		break;
	case 1:
		fprintf(fp, "        finish { phong 0.8 }\n");
		break;
	case 2:
		fprintf(fp, "        finish { ambient 0.2 diffuse 0.7 reflection 0.0 brilliance 1.0 phong 0.3 phong_size 50 specular 0.0 }\n");
		break;
	};
	
	fprintf(fp, "    }\n}\n\n");
}

void write_background(FILE *fp, int background_type)
{
	switch(background_type)
	{
	default:
	case 0:
		fprintf(fp, "background { color SkyBlue }\n\n");
		break;

	case 1:
		fprintf(fp, "sky_sphere {\n\tpigment {\n");
        fprintf(fp, "\t\tbozo\n");
        fprintf(fp, "\t\tcolor_map\n");
        fprintf(fp, "\t\t{\n");
        fprintf(fp, "\t\t   [ 0.0 rgbft <0.666667, 0.666667, 1.0, 0.0, 0.0> ]\n");
        fprintf(fp, "\t\t   [ 0.1 rgbft <0.666667, 0.666667, 1.0, 0.0, 0.0> ]\n");
        fprintf(fp, "\t\t   [ 1.0 rgbft <1.0, 1.0, 1.0, 0.0, 0.0> ]\n");
        fprintf(fp, "\t\t}\n");
        fprintf(fp, "\t\tturbulence 0.5\n");
        fprintf(fp, "\t\toctaves 3\n");
        fprintf(fp, "\t\tlambda 6.0\n");
        fprintf(fp, "\t\tramp_wave \n");  
        fprintf(fp, "\t\tscale .05\n\t}\n}\n\n");
		break;
	};
}

static void convertHSVtoRGB(float *hsv, float *rgb)
{
   float h,s,v,r,g,b;

   h = hsv[0];
   s = hsv[1];
   v = hsv[2];
   
   // to silence the compiler
   r = g = b = 0.0;
   
   if(v <= 0.0)
   {
      r = g = b = 0.0;
   }
   else if(s <= 0.0)
   {
      r = g = b = v;
   }
   else
   {
      int ai;
      float f, p, q, t;

      h *= 6.0;
      if(h >= 6.0)
         h = 0.0;

      ai = (int)h;
      f = h - ai;
      p = v * (1.0 - s);
      q = v * (1.0 - (s * f));
      t = v * (1.0 - (s * (1.0-f)));

      switch(ai)
      {
         case 0:
            r=v; g=t; b=p;	break;

         case 1:
            r=q; g=v; b=p;	break;

         case 2:
            r=p; g=v; b=t;	break;

         case 3:
            r=p; g=q; b=v;	break;

         case 4:
            r=t; g=p; b=v;	break;

         case 5:
            r=v; g=p; b=q;	break;
      }
   }


   // Clamp values to 0..1 range to ensure that rounding errors don't cause problems.
   if(r<0.0) rgb[0] = 0.0;
   else if(r>1.0) rgb[0] = 1.0;
   else rgb[0] = r;

   if(g<0.0) rgb[1] = 0.0;
   else if(g>1.0) rgb[1] = 1.0;
   else rgb[1] = g;

   if(b<0.0) rgb[2] = 0.0;
   else if(b>1.0) rgb[2] = 1.0;
   else rgb[2] = b;
}

// apply a rainbow colormap
void value_to_color(float *node_data, int nnodes, float nd_min, float nd_max, float *rgb)
{
	float hsv[3];
	hsv[1] = 1.0;
	hsv[2] = 1.0;
	float width = nd_max - nd_min;
	
	for(int i=0; i < nnodes; ++i)
	{
		float value = node_data[i];
		
		// Clamp the value
		if(value < nd_min)      value = nd_min;
		else if(value > nd_max) value = nd_max;
		
		hsv[0] = 0.6667 * (1.0 - (value - nd_min) / width);

		convertHSVtoRGB(hsv, &rgb[3*i]);
	}
}

// Check if two points are coincident
static inline bool DEGENERATE(float *p1, float *p2, float eps)
{
	float delta = p1[0] - p2[0];
	if(delta < eps && delta > -eps)
	{
		delta = p1[1] - p2[1];
		if(delta < eps && delta > -eps)
		{
			delta = p1[2] - p2[2];
			if(delta < eps && delta > -eps) return true;
		}
	}
	return false;
}

int
STM3_POV_WritePovCore::WritePov(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read)
	// filename (OMXstr read req notify)
	// err (OMXint write)
	// err_str (OMXstr write)
	FILE *fp;
	int i;
	float r, g, b, radius;
	const float fov = 45.; // camera field of view
	int *bonds, *count_bonds;
	
	// initialize error output
	err = 0;
	err_str = "";
	
	// check if there is anything to be output
	int natoms = in.num_atoms;
	int nfld   = fld.ret_array_size();
    if(natoms <= 0 && nfld <= 0) return 0;
	
	// check if filename available
	if(!filename.valid_obj() || (char *)filename == NULL || *(char *)filename == '\0') return 0;

	// animation support
	char *fn;
	char fn_str[256];
	if((int)enable_animation && step.valid_obj())
	{
		char tmp[256];
		strcpy(tmp, filename);
		char *s = strrchr(tmp, '.');
		if(s) *s = '\0';
		sprintf(fn_str, "%s%05d.pov", tmp, (int)step);
		fn = fn_str;
	}
	else
	{
		fn = filename;
	}

	// create the POV-Ray file and write an header
	if((fp = (FILE *)FILEfopen(fn, "w")) == (FILE *) NULL )
	{
		err = 1;
		err_str = "could not open pov file for write";
		return 1;
	}     
	fprintf(fp, "// AVS/Express generated POV File for STM3 toolkit\n");
	fprintf(fp, "//         http://www.avs.com\n");
	fprintf(fp, "// POV raytracer version 3.5 object description file\n");
	fprintf(fp, "//         http://www.povray.org\n");
	fprintf(fp, "// module version 1.1, October 9, 2003\n");

	// compute limits for the molecule
	int   *atom_z_arr = 0;
	float *coords_arr = 0;
	float *xyz        = 0;
	float cx = 0.0F;
	float cy = 0.0F;
	float cz = 0.0F;
	float xmin = FLT_MAX;
	float ymin = FLT_MAX;
	float zmin = FLT_MAX;
	float xmax = FLT_MIN;
	float ymax = FLT_MIN;
	float zmax = FLT_MIN;
	float zoffset = 0.0F;
	if(natoms > 0)
	{
		// access data
		atom_z_arr = (int *)in.atom_z.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!atom_z_arr)
		{
			fclose(fp);
			err = 1;
			err_str = "could not get atoms Z data";
			return 1;
		}
		coords_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!coords_arr)
		{
			fclose(fp);
			ARRfree(atom_z_arr);
			err = 1;
			err_str = "could not get atoms coords data";
			return 1;
		}

		// convert the molecule coordinates, compute the baricenter and limits
		xyz = new float[3*natoms];
		for(i=0; i < natoms; ++i)
		{
			// convert the axis order
			xyz[3*i+0] = -coords_arr[3*i+0];
			xyz[3*i+1] =  coords_arr[3*i+1];
			xyz[3*i+2] = -coords_arr[3*i+2];

			if(xyz[3*i+0] < xmin) xmin = xyz[3*i+0];
			if(xyz[3*i+1] < ymin) ymin = xyz[3*i+1];
			if(xyz[3*i+2] < zmin) zmin = xyz[3*i+2];

			if(xyz[3*i+0] > xmax) xmax = xyz[3*i+0];
			if(xyz[3*i+1] > ymax) ymax = xyz[3*i+1];
			if(xyz[3*i+2] > zmax) zmax = xyz[3*i+2];
		}
		// seems a bug in the cylinder object, strange twist if z is negative
		if(zmin < 0)
		{
			zoffset = -zmin;
			zmin = 0.0F;
			zmax += zoffset;

		}
		for(i=0; i < natoms; ++i)
		{
			// make all the z coordinates positive
			xyz[3*i+2] += zoffset;

			// compute the barycenter
			cx += xyz[3*i+0];
			cy += xyz[3*i+1];
			cz += xyz[3*i+2];
		}
		cx /= natoms;
		cy /= natoms;
		cz /= natoms;
	}

	// adjust for the fields
	if(nfld > 0)
	{
		float cxf = 0.;
		float cyf = 0.;
		float czf = 0.;
		
		for(i=0; i < nfld; ++i)
		{
			// get the transformation for the field
			float *xfm = (float *)fld[i].xform.mat.ret_array_ptr(OM_GET_ARRAY_RD);
			if(!xfm)
			{
				xfm = (float *)ARRalloc(NULL, DTYPE_FLOAT, 16, NULL);
				memset(xfm, 0, 16*sizeof(float));
				xfm[0] = xfm[5] = xfm[10] = xfm[15] = 1.0F;
			}
			float *xlate = (float *)fld[i].xform.xlate.ret_array_ptr(OM_GET_ARRAY_RD);
			if(!xlate)
			{
				xlate = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3, NULL);
				memset(xlate, 0, 3*sizeof(float));
			}
		
			float *rcmin = (float *)fld[i].coordinates.min_vec.ret_array_ptr(OM_GET_ARRAY_RD);
			if(!rcmin) continue;
			float *rcmax = (float *)fld[i].coordinates.max_vec.ret_array_ptr(OM_GET_ARRAY_RD);
			if(!rcmax)
			{
				ARRfree(rcmin);
				continue;
			}
			
			// take care of the xform of the field (important for Gaussian cube isosurfaces!)
			float cmin[3], cmax[3];
			memcpy(cmin, rcmin, 3*sizeof(float));
			memcpy(cmax, rcmax, 3*sizeof(float));
			ARRfree(rcmin);
			ARRfree(rcmax);
			MATvec3_mat4_multiply(cmin, (Matr4 *)xfm);
			VEC_ADD(cmin, cmin, xlate);
			MATvec3_mat4_multiply(cmax, (Matr4 *)xfm);
			VEC_ADD(cmax, cmax, xlate);
						
			if(-cmax[0] < xmin) xmin = -cmax[0];
			if(cmin[1]  < ymin) ymin =  cmin[1];
			if(-cmax[2] < zmin) zmin = -cmax[2];
			if(-cmin[0] > xmax) xmax = -cmin[0];
			if(cmax[1]  > ymax) ymax =  cmax[1];
			if(-cmin[2] > zmax) zmax = -cmin[2];
			
			cxf -= (cmin[0]+cmax[0])/2.;
			cyf += (cmin[1]+cmax[1])/2.;
			czf -= (cmin[2]+cmax[2])/2.;
		}
		
		if(natoms > 0)
		{
			cx = (cx + cxf/nfld)/2;
			cy = (cy + cyf/nfld)/2;
			cz = (cz + czf/nfld)/2;
		}
		else
		{
			cx = cxf/nfld;
			cy = cyf/nfld;
			cz = czf/nfld;
		}
	}

	// try to find a position for the camera
	// you will likely want to change these by hand once the file is written
	float deltax = xmax - xmin;
	float deltay = ymax - ymin;
	if(deltax < 0) deltax = -deltax;
	if(deltay < 0) deltay = -deltay;
	float delta  = ((deltax > deltay) ? deltax : deltay) / (2*tan(fov/2*M_PI/180.));
	if(delta == 0.0F) delta = 10.0F;
	
	// write the camera and lights part
	write_camera_and_lights(fp, cx, cy, cz, zmin-delta, fov, zoffset);

	// setup the default color
	write_default_color(fp, display_params.default_colors, (int)finish, (float)transparency);

	// write the background: solid or a sky sphere
	write_background(fp, (int)background_type);
	
	// output any field
	if(nfld > 0) output_fields(fp, (float)display_params.licorice_scale);

	// if no atom data finish the work here
	if(natoms <= 0)
	{
		fclose(fp);
		return 1;
	}

	float *charge_arr = 0;
	float *rgb = 0;
	switch((int)display_params.atoms_representation)
	{
	case BALL_AND_STICK_CHARGE:
	case CPK_CHARGE:
	case LICORICE_CHARGE:
		charge_arr = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		rgb = new float[3*natoms];
		break;
	};

	// output atoms
	fprintf(fp, "union {\n    // Atoms\n");
	switch((int)display_params.atoms_representation)
	{
	default:
	case BALL_AND_STICK:
		for(i=0; i < natoms; ++i)
		{
			radius = atom_properties[atom_z_arr[i]].rvdw * display_params.normal_scale;
			r = atom_properties[atom_z_arr[i]].color[0];
			g = atom_properties[atom_z_arr[i]].color[1];
			b = atom_properties[atom_z_arr[i]].color[2];

			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, r, g, b);
		}
		break;
		
	case CPK:
		for(i=0; i < natoms; ++i)
		{
			radius = atom_properties[atom_z_arr[i]].rvdw * display_params.cpk_scale;
			r = atom_properties[atom_z_arr[i]].color[0];
			g = atom_properties[atom_z_arr[i]].color[1];
			b = atom_properties[atom_z_arr[i]].color[2];

			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, r, g, b);
		}
		break;
		
	case COLORED_STICK:
		bonds = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		count_bonds = new int[in.num_atoms];
		for(i=0; i < natoms; ++i) count_bonds[i] = 0;
		if(bonds) for(i=0; i < 2*in.num_bonds; ++i) ++count_bonds[bonds[i]];
		radius = display_params.licorice_scale;
		for(i=0; i < natoms; ++i)
		{
			if(count_bonds[i])
			{
				r = atom_properties[atom_z_arr[i]].color[0];
				g = atom_properties[atom_z_arr[i]].color[1];
				b = atom_properties[atom_z_arr[i]].color[2];

				fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
							xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, r, g, b);
			}
		}
		if(bonds) ARRfree(bonds);
		delete [] count_bonds;
		break;
		
	case COLORED_RESIDUE:
	case STICK:
		break;
		
	case LICORICE:
		radius = display_params.licorice_scale;
		for(i=0; i < natoms; ++i)
		{
			r = atom_properties[atom_z_arr[i]].color[0];
			g = atom_properties[atom_z_arr[i]].color[1];
			b = atom_properties[atom_z_arr[i]].color[2];

			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, r, g, b);
		}
		break;

	case LICORICE_STICK:
		radius = display_params.licorice_scale;
		for(i=0; i < natoms; ++i)
		{
			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius);
		}
		break;
	
	case BALL_AND_STICK_CHARGE:
		if(charge_arr) value_to_color(charge_arr, natoms, display_params.charge_min, display_params.charge_max, rgb);
		else memset(rgb, 0, 3*natoms*sizeof(float));
		for(i=0; i < natoms; ++i)
		{
			radius = atom_properties[atom_z_arr[i]].rvdw * display_params.normal_scale;
			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, rgb[3*i+0], rgb[3*i+1], rgb[3*i+2]);
		}
		break;
		
	case CPK_CHARGE:
		if(charge_arr) value_to_color(charge_arr, natoms, display_params.charge_min, display_params.charge_max, rgb);
		else memset(rgb, 0, 3*natoms*sizeof(float));
		for(i=0; i < natoms; ++i)
		{
			radius = atom_properties[atom_z_arr[i]].rvdw * display_params.cpk_scale;
			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, rgb[3*i+0], rgb[3*i+1], rgb[3*i+2]);
		}
		break;
		
	case LICORICE_CHARGE:
		if(charge_arr) value_to_color(charge_arr, natoms, display_params.charge_min, display_params.charge_max, rgb);
		else memset(rgb, 0, 3*natoms*sizeof(float));
		radius = display_params.licorice_scale;
		for(i=0; i < natoms; ++i)
		{
			fprintf(fp, "    sphere {\n        <%.3f, %.3f, %.3f>\n        %.3f\n        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n",
						xyz[3*i+0], xyz[3*i+1], xyz[3*i+2], radius, rgb[3*i+0], rgb[3*i+1], rgb[3*i+2]);
		}
		break;		
	}
	
	// bonds	
	int *connect_arr = (int *)in.connect_lst.ret_array_ptr(OM_GET_ARRAY_RD);

	// get here if needed the bond type array so can be adjusted for splitting
	int *bond_type_arr = NULL;
	if(connect_arr && ((int)display_params.bonds_representation == LINEFLAG_ALL_H || (int)display_params.bonds_representation == LINEFLAG_H_BONDS))
	{
		bond_type_arr = (int *)in.bond_type_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!bond_type_arr)
		{
			ARRfree(atom_z_arr);
			ARRfree(connect_arr);
			ARRfree(coords_arr);
			if(charge_arr) ARRfree(charge_arr);
			delete [] xyz;
			fclose(fp);
			err = 1;
			err_str = "could not get bond type data";
			return 1;
		}
	}
	
	fprintf(fp, "\n    // Bonds\n");
	if(connect_arr) switch((int)display_params.atoms_representation)
	{
	default:
	case BALL_AND_STICK:
	case LICORICE:
	case COLORED_STICK:
		for(i=0; i < in.num_bonds; ++i)
		{
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			// check for degenerate bonds (can happen with symmetries)
			if(DEGENERATE(&xyz[3*a1], &xyz[3*a2], 1e-5f)) continue;
			   
			float r1 = atom_properties[atom_z_arr[a1]].color[0];
			float g1 = atom_properties[atom_z_arr[a1]].color[1];
			float b1 = atom_properties[atom_z_arr[a1]].color[2];
			float r2 = atom_properties[atom_z_arr[a2]].color[0];
			float g2 = atom_properties[atom_z_arr[a2]].color[1];
			float b2 = atom_properties[atom_z_arr[a2]].color[2];

			float radius = (float)display_params.licorice_scale;
			if(bond_type_arr && 
			   (bond_type_arr[i] == H_BOND || bond_type_arr[i] == SPECIAL_BOND ||
			   (bond_type_arr[i] == H_NORMAL_BOND && (int)display_params.bonds_representation == LINEFLAG_ALL_H)))
			{
				radius /= 5.;
				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <.7, .7, .7> }\n    }\n");
			}
			else if(r1 == r2 && g1 == g2 && b1 == b2)
			{
				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r1, g1, b1);
			}
			else if((int)display_params.bonds_split_color)
			{
				float xc = (xyz[3*a1+0] + xyz[3*a2+0])/2.0F;
				float yc = (xyz[3*a1+1] + xyz[3*a2+1])/2.0F;
				float zc = (xyz[3*a1+2] + xyz[3*a2+2])/2.0F;

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xc, yc, zc, radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r1, g1, b1);

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xc, yc, zc);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r2, g2, b2);
			}
			else
			{
				write_smooth_cylinder(fp, &xyz[3*a1], &xyz[3*a2], radius, r1, g1, b1, r2, g2, b2);
			}
		}
		break;

	case CPK:
	case CPK_CHARGE:
		break;

	case COLORED_RESIDUE:
		for(i=0; i < in.num_bonds; ++i)
		{
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			// check for degenerate bonds (can happen with symmetries)
			if(DEGENERATE(&xyz[3*a1], &xyz[3*a2], 1e-5f)) continue;
			   
			float c1[3], c2[3];
			char tmp[50];
			res_to_color(in.residue_name.ret_str_array_val(a1, tmp), c1);
			res_to_color(in.residue_name.ret_str_array_val(a2, tmp), c2);

			if(c1[0] == c2[0] && c1[1] == c2[1] && c1[2] == c2[2])
			{
				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], (float)display_params.licorice_scale);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", c1[0], c1[1], c1[2]);
			}
			else if((int)display_params.bonds_split_color)
			{
				float xc = (xyz[3*a1+0] + xyz[3*a2+0])/2.0F;
				float yc = (xyz[3*a1+1] + xyz[3*a2+1])/2.0F;
				float zc = (xyz[3*a1+2] + xyz[3*a2+2])/2.0F;

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xc, yc, zc, (float)display_params.licorice_scale);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", c1[0], c1[1], c1[2]);

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xc, yc, zc);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], (float)display_params.licorice_scale);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", c2[0], c2[1], c2[2]);
			}
			else
			{
				write_smooth_cylinder(fp, &xyz[3*a1], &xyz[3*a2], (float)display_params.licorice_scale, c1[0], c1[1], c1[2], c2[0], c2[1], c2[2]);
			}
		}
		break;
		
	case STICK:
	case LICORICE_STICK:
		for(i=0; i < in.num_bonds; ++i)
		{
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			// check for degenerate bonds (can happen with symmetries)
			if(DEGENERATE(&xyz[3*a1], &xyz[3*a2], 1e-5f)) continue;
			   
			fprintf(fp, "    cylinder {\n");
			fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
			fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], (float)display_params.licorice_scale);
			fprintf(fp, "    }\n");
		}
		break;
		
	case BALL_AND_STICK_CHARGE:
	case LICORICE_CHARGE:
		for(i=0; i < in.num_bonds; ++i)
		{
			int a1 = connect_arr[2*i+0];
			int a2 = connect_arr[2*i+1];
			
			// check for degenerate bonds (can happen with symmetries)
			if(DEGENERATE(&xyz[3*a1], &xyz[3*a2], 1e-5f)) continue;
			   
			float r1 = rgb[3*a1+0];
			float g1 = rgb[3*a1+1];
			float b1 = rgb[3*a1+2];
			float r2 = rgb[3*a2+0];
			float g2 = rgb[3*a2+1];
			float b2 = rgb[3*a2+2];

			float radius = (float)display_params.licorice_scale;
			if(bond_type_arr && 
			   (bond_type_arr[i] == H_BOND || bond_type_arr[i] == SPECIAL_BOND ||
			   (bond_type_arr[i] == H_NORMAL_BOND && (int)display_params.bonds_representation == LINEFLAG_ALL_H)))
			{
				radius /= 5.;
				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <.7, .7, .7> }\n    }\n");
			}
			else if(r1 == r2 && g1 == g2 && b1 == b2)
			{
				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r1, g1, b1);
			}
			else if((int)display_params.bonds_split_color)
			{
				float xc = (xyz[3*a1+0] + xyz[3*a2+0])/2.0F;
				float yc = (xyz[3*a1+1] + xyz[3*a2+1])/2.0F;
				float zc = (xyz[3*a1+2] + xyz[3*a2+2])/2.0F;

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xyz[3*a1+0], xyz[3*a1+1], xyz[3*a1+2]);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xc, yc, zc, radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r1, g1, b1);

				fprintf(fp, "    cylinder {\n");
				fprintf(fp, "        <%.3f, %.3f %.3f>", xc, yc, zc);
				fprintf(fp, " <%.3f, %.3f %.3f> %.3f\n", xyz[3*a2+0], xyz[3*a2+1], xyz[3*a2+2], radius);
				fprintf(fp, "        pigment { color rgb <%.3f, %.3f, %.3f> }\n    }\n", r2, g2, b2);
			}
			else
			{
				write_smooth_cylinder(fp, &xyz[3*a1], &xyz[3*a2], radius, r1, g1, b1, r2, g2, b2);
			}
		}
		break;		
	}
	if(zoffset >= 0.001) fprintf(fp, "\n    // undo the bug fixing offset\n    translate <0.000, 0.000, %.3f>\n", -zoffset);
	fprintf(fp, "}\n\n");
	
	// finish
	if(bond_type_arr) ARRfree(bond_type_arr);
	ARRfree(atom_z_arr);
	ARRfree(coords_arr);
	if(connect_arr) ARRfree(connect_arr);
	if(charge_arr) ARRfree(charge_arr);
	delete [] xyz;
	delete [] rgb;
	fclose(fp);
	return 1;
}

#define CELL_TRI         4
#define CELL_QUAD        5
#define CELL_POLYTRI    10
#define CELL_POLYLINE    3
#define CELL_LINE        2
#define CELL_HEX		 6
#define CELL_POINT		 1

#define CELL_TRI_N    1004
#define CELL_TRI_C    2004
#define CELL_TRI_NC   3004

#define CELL_QUAD_N   1005
#define CELL_QUAD_C   2005
#define CELL_QUAD_NC  3005

#define CELL_POINT_C  2001

void STM3_POV_WritePovCore::output_fields(FILE *outfile, float radius)
{
	int   npolys;
	int   p;
	int   i, j, k;
	float *in_coords;
	float raw[24], nraw[24];
	int   nnodes;
	int   nspace;
	int   cs, cell_type[100];
	float nd_min = 0, nd_max = 0, *node_data, *normals, *rgb;
	int   *in_ncl = NULL;
	float *xfm, *xlate;
	int   nf;
	bool  smooth_triangle_header_done = false;

	fprintf(outfile,"#declare line_thickness = %.3f;\n", radius);
	fprintf(outfile,"#declare sphere_radius  = %.3f;\n\n", radius);

	//  Loop over all input fields
	for(nf=0; nf < fld.ret_array_size(); ++nf) 
	{
		nnodes = fld[nf].nnodes;
		nspace = fld[nf].nspace;

		// check we have found some facets
		if(nnodes == 0) continue;

		// get coords array
		in_coords = (float*)fld[nf].coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
		if(in_coords == NULL)
		{
			err = 1;
			err_str = "write pov: problem assigning coordinates array";
			fprintf(outfile," *** FILE WRITE STOPPED EARLY, COORD ACCESS ERROR! ***\n");
			continue;
		}

		// check if normals, color or scalar node_data are available
		normals = rgb = node_data = 0;
		for(i=0; i < fld[nf].nnode_data; ++i)
		{
			if(fld[nf].node_data[i].id == GD_NORMAL_DATA_ID)
			{
				if(!normals)
				{
					normals = (float *)fld[nf].node_data[i].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, DTYPE_FLOAT);
				}
			}
			else if(fld[nf].node_data[i].id == GD_COLOR_DATA_ID)
			{
				if(!rgb)
				{
					rgb = (float *)fld[nf].node_data[i].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, DTYPE_FLOAT);
				}
			}
			else if(fld[nf].node_data[i].veclen == 1)
			{
				if(!node_data)
				{
					node_data = (float *)fld[nf].node_data[i].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, DTYPE_FLOAT);

					nd_min = (float)fld[nf].node_data[i].min;
					nd_max = (float)fld[nf].node_data[i].max;
				}
			}
		}

		// create a rgb scale
		if(!rgb && node_data)
		{
			rgb = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3*nnodes, NULL);
			if(rgb) value_to_color(node_data, nnodes, nd_min, nd_max, rgb);
		}
		
		// get the transformation for the field
		xfm   = (float*)fld[nf].xform.mat.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!xfm)
		{
			xfm = (float *)ARRalloc(NULL, DTYPE_FLOAT, 16, NULL);
			memset(xfm, 0, 16*sizeof(float));
			xfm[0] = xfm[5] = xfm[10] = xfm[15] = 1.0F;
		}
		xlate = (float*)fld[nf].xform.xlate.ret_array_ptr(OM_GET_ARRAY_RD);
		if(!xlate)
		{
			xlate = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3, NULL);
			memset(xlate, 0, 3*sizeof(float));
		}

		// supports only cell_sets of specific types,
		// so scan structure for exact matches.
		// after that, we can simply check the cell_type.
		cs = -1;
		for(i=0; i< fld[nf].ncell_sets; i++)
		{
			// check for cell_set of type "Tri",
			if(	fld[nf].cell_set[i].ncells > 0        &&
				fld[nf].cell_set[i].cell_ndim   == 2  &&
				fld[nf].cell_set[i].cell_nnodes == 3  &&
				fld[nf].cell_set[i].cell_order  == 1  &&
				fld[nf].cell_set[i].poly_flag   == 0 )
			{
				cell_type[i] = CELL_TRI;
				cs = i;

			// check for cell_set of type "Polytri",
			} else if(fld[nf].cell_set[i].ncells   > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 2  &&
				  fld[nf].cell_set[i].cell_nnodes == 3  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 1 )
			{
				cell_type[i] = CELL_POLYTRI;
				cs = i;

			// check for cell_set of type "Polyline",
			} else if ( fld[nf].cell_set[i].ncells > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 1  &&
				  fld[nf].cell_set[i].cell_nnodes == 2  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 1 )
			{
				cell_type[i] = CELL_POLYLINE;
				cs = i;

			// check for cell_set of type "Quad",
			} else if ( fld[nf].cell_set[i].ncells > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 2  &&
				  fld[nf].cell_set[i].cell_nnodes == 4  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 0 )
			{
				cell_type[i] = CELL_QUAD;
				cs = i;

			// check for cell_set of type "Line",
			} else if ( fld[nf].cell_set[i].ncells > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 1  &&
				  fld[nf].cell_set[i].cell_nnodes == 2  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 0 )
			{
				cell_type[i] = CELL_LINE;
				cs = i;

			// check for cell_set of type "Hex",
			} else if ( fld[nf].cell_set[i].ncells > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 3  &&
				  fld[nf].cell_set[i].cell_nnodes == 8  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 0 )
			{
				cell_type[i] = CELL_HEX;
				cs = i;

			// check for cell_set of type "Point",
			} else if ( fld[nf].cell_set[i].ncells > 0  &&
				  fld[nf].cell_set[i].cell_ndim   == 0  &&
				  fld[nf].cell_set[i].cell_nnodes == 1  &&
				  fld[nf].cell_set[i].cell_order  == 1  &&
				  fld[nf].cell_set[i].poly_flag   == 0 )
			{
				cell_type[i] = CELL_POINT;
				cs = i;
				
			// skip to next, no type recognized
			} else {
				cell_type[i] = -1;
			}
		}

		if(cs == -1) return;

		// adjust the cell type to use normals and/or color
		bool need_smooth_triangle_header = false;
		for(cs=0; cs < fld[nf].ncell_sets; cs++)
		{
			if(cell_type[cs] == CELL_TRI)
			{
				if(normals && rgb) {cell_type[cs] = CELL_TRI_NC; need_smooth_triangle_header = true;}
				else if(normals)   {cell_type[cs] = CELL_TRI_N;}
				else if(rgb)       {cell_type[cs] = CELL_TRI_C; need_smooth_triangle_header = true;}
			}
			else if(cell_type[cs] == CELL_QUAD)
			{
				if(normals && rgb) {cell_type[cs] = CELL_QUAD_NC; need_smooth_triangle_header = true;}
				else if(normals)   {cell_type[cs] = CELL_QUAD_N;}
				else if(rgb)       {cell_type[cs] = CELL_QUAD_C; need_smooth_triangle_header = true;}
			}
			else if(cell_type[cs] == CELL_POINT)
			{
				if(rgb)       	   {cell_type[cs] = CELL_POINT_C;}
			}
		}

		// Output the trimap header.
		// Here is the original comment
		//
		/*************************************************************************
        		TRIANGLE MAPPING MACRO FILE FOR PERSISTENCE OF VISION 3.1
		**************************************************************************

		Created by Chris Colefax, 1 June 1999;
		   Updated 12 September 1999: renamed macros, improved memory use,
    		  added full mesh {} support.

		*************************************************************************/
		if(need_smooth_triangle_header && !smooth_triangle_header_done)
		{
			// COMMON TRANSFORMATION MACRO (REQUIRED FOR BOTH MAPPING METHODS)
			fprintf(outfile,"#macro tri_matrix() #local NX = P2-P1; #local NY = P3-P1; #local NZ = vcross(NX, NY); matrix <NX.x, NX.y, NX.z, NY.x, NY.y, NY.z, NZ.x, NZ.y, NZ.z, P1.x, P1.y, P1.z> #end\n");

			// COLOURED VERTEX MACROS
			fprintf(outfile,"#macro get_cv_texture () texture { pigment {\n");
			fprintf(outfile,"   average pigment_map {[1 gradient x color_map {[0 rgb 0] [1 C2*3]}] [1 gradient y color_map {[0 rgb 0] [1 C3*3]}] [1 gradient z color_map {[0 rgb 0] [1 C1*3]}]}\n");
			fprintf(outfile,"   matrix <1.01, 0, 1, 0, 1.01, 1, 0, 0, 1, -.002, -.002, -1> tri_matrix() }} #end\n\n");

			fprintf(outfile,"#macro cv_triangle (P1, C1, P2, C2, P3, C3) #local T = get_cv_texture () triangle {P1, P2, P3 texture {T}} #end\n");
			fprintf(outfile,"#macro cv_smooth_triangle (P1, N1, C1, P2, N2, C2, P3, N3, C3) #local T = get_cv_texture () smooth_triangle {P1, N1, P2, N2, P3, N3 texture {T}} #end\n\n");

			smooth_triangle_header_done = true;
		}

		// now that we have the data structure set up, write the file contents
		fprintf(outfile,"// Field %d\nunion {\n", nf);

		// loop over all cell sets, processing for output
		for(cs=0; cs < fld[nf].ncell_sets; cs++)
		{
			npolys = fld[nf].cell_set[cs].ncells;

			// get node connect list
			if(cell_type[cs] == CELL_POLYTRI || cell_type[cs] == CELL_POLYLINE)
				in_ncl = (int*)fld[nf].cell_set[cs].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
			else
				in_ncl = (int*)fld[nf].cell_set[cs].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);

			if(in_ncl == NULL)
			{
				err = 1;
				err_str = "write_pov: problem allocating node connect list";
				continue;
			}

			// process each cell type in turn:
			switch(cell_type[cs])
			{
			case CELL_TRI: // triangle
				for(i = 0; i < npolys; i++)
				{
					int k1 = in_ncl[i*3+0];
					int k2 = in_ncl[i*3+1];
					int k3 = in_ncl[i*3+2];

					// if the triangle is degenerate skip it
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k2*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k3*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k3*nspace], &in_coords[k2*nspace], 1e-3F)) continue;

					fprintf(outfile,"triangle {\n");
					for(j=0; j < 3; j++)
					{
						k = in_ncl[i*3+j];

						raw[0] = in_coords[k*nspace+0];
						raw[1] = in_coords[k*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
						VEC_ADD(raw, raw, xlate);
						fprintf(outfile,"\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);
					}
					fprintf(outfile, "}\n");
				}
				fprintf(outfile,"// end of tri cell set\n\n");
				break;

			case CELL_TRI_N: // triangle plus normals
				for(i = 0; i < npolys; i++)
				{
					int k1 = in_ncl[i*3+0];
					int k2 = in_ncl[i*3+1];
					int k3 = in_ncl[i*3+2];

					// if the triangle is degenerate skip it
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k2*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k3*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k3*nspace], &in_coords[k2*nspace], 1e-3F)) continue;

					fprintf(outfile,"smooth_triangle {\n");
					for(j=0; j < 3; j++)
					{
						k = in_ncl[i*3+j];

						raw[0] = in_coords[k*nspace+0];
						raw[1] = in_coords[k*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
						VEC_ADD(raw, raw, xlate);
						fprintf(outfile, "\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);

						raw[0] = normals[k*nspace+0];
						raw[1] = normals[k*nspace+1];
						raw[2] = (nspace == 3) ? normals[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
						fprintf(outfile, "\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);
					}
					fprintf(outfile, "}\n");
				}
				fprintf(outfile,"// end of tri cell set\n\n");
				break;

			case CELL_TRI_C: // triangle plus color
				for(i = 0; i < npolys; i++)
				{
					int k1 = in_ncl[i*3+0];
					int k2 = in_ncl[i*3+1];
					int k3 = in_ncl[i*3+2];

					// if the triangle is degenerate skip it
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k2*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k3*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k3*nspace], &in_coords[k2*nspace], 1e-3F)) continue;

					// if the three colors are equal output a normal triangle
					if(rgb[3*k1+0] == rgb[3*k2+0] && rgb[3*k1+0] == rgb[3*k3+0] &&
					   rgb[3*k1+1] == rgb[3*k2+1] && rgb[3*k1+1] == rgb[3*k3+1] &&
					   rgb[3*k1+2] == rgb[3*k2+2] && rgb[3*k1+2] == rgb[3*k3+2])
					{
      					fprintf(outfile,"triangle {\n");
						for(j=0; j < 3; j++)
						{
							k = in_ncl[i*3+j];

							raw[0] = in_coords[k*nspace+0];
							raw[1] = in_coords[k*nspace+1];
							raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							VEC_ADD(raw, raw, xlate);
							fprintf(outfile,"\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);
						}
						fprintf(outfile,"\tpigment { color rgbt < %.5f, %.5f, %.5f, trans > }\n", rgb[3*k+0], rgb[3*k+1], rgb[3*k+2]);
						fprintf(outfile, "}\n");
					}
					else
					{
      					fprintf(outfile,"cv_triangle (");
						for(j=0; j < 3; j++)
						{
							k = in_ncl[i*3+j];

							raw[0] = in_coords[k*nspace+0];
							raw[1] = in_coords[k*nspace+1];
							raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							VEC_ADD(raw, raw, xlate);
							fprintf(outfile,"< %.5f, %.5f, %.5f >, ", -raw[0], raw[1], -raw[2]);

							fprintf(outfile,"rgbt < %.5f, %.5f, %.5f, trans >", rgb[3*k+0], rgb[3*k+1], rgb[3*k+2]);

							if(j < 2) fprintf(outfile, ", ");
						}
						fprintf(outfile, ")\n");
					}
				}
				fprintf(outfile,"// end of tri cell set\n\n");
				break;

			case CELL_TRI_NC: // triangles with normals and color
				for(i = 0; i < npolys; i++)
				{
					int k1 = in_ncl[i*3+0];
					int k2 = in_ncl[i*3+1];
					int k3 = in_ncl[i*3+2];

					// if the triangle is degenerate skip it
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k2*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k1*nspace], &in_coords[k3*nspace], 1e-3F)) continue;
					if(DEGENERATE(&in_coords[k3*nspace], &in_coords[k2*nspace], 1e-3F)) continue;

					// if the three colors are equal output a normal smooth triangle
					if(rgb[3*k1+0] == rgb[3*k2+0] && rgb[3*k1+0] == rgb[3*k3+0] &&
					   rgb[3*k1+1] == rgb[3*k2+1] && rgb[3*k1+1] == rgb[3*k3+1] &&
					   rgb[3*k1+2] == rgb[3*k2+2] && rgb[3*k1+2] == rgb[3*k3+2])
					{
      					fprintf(outfile,"smooth_triangle {\n");
						for(j=0; j < 3; j++)
						{
							k = in_ncl[i*3+j];

							raw[0] = in_coords[k*nspace+0];
							raw[1] = in_coords[k*nspace+1];
							raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							VEC_ADD(raw, raw, xlate);
							fprintf(outfile, "\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);

							raw[0] = normals[k*nspace+0];
							raw[1] = normals[k*nspace+1];
							raw[2] = (nspace == 3) ? normals[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							fprintf(outfile, "\t< %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);
						}
						fprintf(outfile,"\tpigment { color rgbt < %.3f, %.3f, %.3f, trans > }\n", rgb[3*k+0], rgb[3*k+1], rgb[3*k+2]);
						fprintf(outfile, "}\n");
					}
					else
					{
      					// cv_smooth_triangle (P1, N1, C1, P2, N2, C2, P3, N3, C3)
      					fprintf(outfile,"cv_smooth_triangle (");
						for(j=0; j < 3; j++)
						{
							k = in_ncl[i*3+j];

							raw[0] = in_coords[k*nspace+0];
							raw[1] = in_coords[k*nspace+1];
							raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							VEC_ADD(raw, raw, xlate);
							fprintf(outfile, "< %.5f, %.5f, %.5f >, ", -raw[0], raw[1], -raw[2]);

							raw[0] = normals[k*nspace+0];
							raw[1] = normals[k*nspace+1];
							raw[2] = (nspace == 3) ? normals[k*nspace+2] : 0.0;
							MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
							fprintf(outfile, "< %.5f, %.5f, %.5f >, ", -raw[0], raw[1], -raw[2]);

							fprintf(outfile,"rgbt < %.5f, %.5f, %.5f, trans >", rgb[3*k+0], rgb[3*k+1], rgb[3*k+2]);

							if(j < 2) fprintf(outfile, ", ");
						}
						fprintf(outfile, ")\n");
					}
				}
				fprintf(outfile,"// end of tri cell set\n\n");
				break;

			case CELL_QUAD:

				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
					for(j=0; j < 4; j++)
					{
						k = in_ncl[i*4+j];
						raw[3*j+0] = in_coords[k*nspace+0];
						raw[3*j+1] = in_coords[k*nspace+1];
						raw[3*j+2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&raw[3*j], (Matr4 *)xfm);
						VEC_ADD((&raw[3*j]), (&raw[3*j]), xlate);
						raw[3*j+0] = -raw[3*j+0];
						raw[3*j+2] = -raw[3*j+2];
					}

					fprintf(outfile,"triangle {");
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[0], raw[1], raw[2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[3], raw[4], raw[5]);
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[6], raw[7], raw[8]);
					fprintf(outfile," }\n");
					fprintf(outfile,"triangle {");
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[0], raw[1],  raw[2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[6], raw[7],  raw[8]);
						fprintf(outfile," < %.3f, %.3f, %.3f >", raw[9], raw[10], raw[11]);
					fprintf(outfile," }\n");
					
				} // end of primitive loop
				fprintf(outfile,"// end of quad cell set\n\n");
				break;

			case CELL_QUAD_N:

				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
					for(j=0; j < 4; j++)
					{
						k = in_ncl[i*4+j];
						raw[3*j+0] = in_coords[k*nspace+0];
						raw[3*j+1] = in_coords[k*nspace+1];
						raw[3*j+2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&raw[3*j], (Matr4 *)xfm);
						VEC_ADD((&raw[3*j]), (&raw[3*j]), xlate);
						raw[3*j+0] = -raw[3*j+0];
						raw[3*j+2] = -raw[3*j+2];
						
						nraw[3*j+0] = normals[k*nspace+0];
						nraw[3*j+1] = normals[k*nspace+1];
						nraw[3*j+2] = (nspace == 3) ? normals[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&nraw[3*j], (Matr4 *)xfm);
						nraw[3*j+0] = -nraw[3*j+0];
						nraw[3*j+2] = -nraw[3*j+2];
					}

					fprintf(outfile,"smooth_triangle {");
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[0], raw[1], raw[2], nraw[0], nraw[1], nraw[2]);
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[3], raw[4], raw[5], nraw[3], nraw[4], nraw[5]);
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[6], raw[7], raw[8], nraw[6], nraw[7], nraw[8]);
					fprintf(outfile," }\n");
					fprintf(outfile,"smooth_triangle {");
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[0], raw[1],  raw[2],  nraw[0], nraw[1],  nraw[2]);
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[6], raw[7],  raw[8],  nraw[6], nraw[7],  nraw[8]);
						fprintf(outfile," < %.3f, %.3f, %.3f > < %.3f, %.3f, %.3f >", raw[9], raw[10], raw[11], nraw[9], nraw[10], nraw[11]);
					fprintf(outfile," }\n");
					
				} // end of primitive loop
				fprintf(outfile,"// end of quad cell set\n\n");
				break;

			case CELL_QUAD_C:

				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
					int h[4];
					for(j=0; j < 4; j++)
					{
						h[j] = k = in_ncl[i*4+j];
						raw[3*j+0] = in_coords[k*nspace+0];
						raw[3*j+1] = in_coords[k*nspace+1];
						raw[3*j+2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&raw[3*j], (Matr4 *)xfm);
						VEC_ADD((&raw[3*j]), (&raw[3*j]), xlate);
						raw[3*j+0] = -raw[3*j+0];
						raw[3*j+2] = -raw[3*j+2];
					}
					
					fprintf(outfile,"cv_triangle (");
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[0], raw[1], raw[2], rgb[3*h[0]+0], rgb[3*h[0]+1], rgb[3*h[0]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[3], raw[4], raw[5], rgb[3*h[1]+0], rgb[3*h[1]+1], rgb[3*h[1]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >",  raw[6], raw[7], raw[8], rgb[3*h[2]+0], rgb[3*h[2]+1], rgb[3*h[2]+2]);
					fprintf(outfile," )\n");
					fprintf(outfile,"cv_triangle (");
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[0], raw[1],  raw[2],  rgb[3*h[0]+0], rgb[3*h[0]+1], rgb[3*h[0]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[6], raw[7],  raw[8],  rgb[3*h[2]+0], rgb[3*h[2]+1], rgb[3*h[2]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >",  raw[9], raw[10], raw[11], rgb[3*h[3]+0], rgb[3*h[3]+1], rgb[3*h[3]+2]);
					fprintf(outfile," )\n");
					
				} // end of primitive loop
				fprintf(outfile,"// end of quad cell set\n\n");
				break;

			case CELL_QUAD_NC:

				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
					int h[4];
					for(j=0; j < 4; j++)
					{
						h[j] = k = in_ncl[i*4+j];
						raw[3*j+0] = in_coords[k*nspace+0];
						raw[3*j+1] = in_coords[k*nspace+1];
						raw[3*j+2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&raw[3*j], (Matr4 *)xfm);
						VEC_ADD((&raw[3*j]), (&raw[3*j]), xlate);
						raw[3*j+0] = -raw[3*j+0];
						raw[3*j+2] = -raw[3*j+2];
						
						nraw[3*j+0] = normals[k*nspace+0];
						nraw[3*j+1] = normals[k*nspace+1];
						nraw[3*j+2] = (nspace == 3) ? normals[k*nspace+2] : 0.0;
						MATvec3_mat4_multiply(&nraw[3*j], (Matr4 *)xfm);
						nraw[3*j+0] = -nraw[3*j+0];
						nraw[3*j+2] = -nraw[3*j+2];
					}
					
					fprintf(outfile,"cv_smooth_triangle (");
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[0], raw[1], raw[2], nraw[0], nraw[1], nraw[2], rgb[3*h[0]+0], rgb[3*h[0]+1], rgb[3*h[0]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[3], raw[4], raw[5], nraw[3], nraw[4], nraw[5], rgb[3*h[1]+0], rgb[3*h[1]+1], rgb[3*h[1]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >",  raw[6], raw[7], raw[8], nraw[6], nraw[7], nraw[8], rgb[3*h[2]+0], rgb[3*h[2]+1], rgb[3*h[2]+2]);
					fprintf(outfile," )\n");
					fprintf(outfile,"cv_smooth_triangle (");
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[0], raw[1],  raw[2],  nraw[0],  nraw[1],  nraw[2], rgb[3*h[0]+0], rgb[3*h[0]+1], rgb[3*h[0]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >,", raw[6], raw[7],  raw[8],  nraw[6],  nraw[7],  nraw[8], rgb[3*h[2]+0], rgb[3*h[2]+1], rgb[3*h[2]+2]);
						fprintf(outfile," < %.3f, %.3f, %.3f >, < %.3f, %.3f, %.3f >, rgbt < %.5f, %.5f, %.5f, trans >",  raw[9], raw[10], nraw[9], nraw[10], nraw[11], raw[11], rgb[3*h[3]+0], rgb[3*h[3]+1], rgb[3*h[3]+2]);
					fprintf(outfile," )\n");
					
				} // end of primitive loop
				fprintf(outfile,"// end of quad cell set\n\n");
				break;

			case CELL_POLYLINE:

				npolys = fld[nf].cell_set[cs].npolys;

				// loop over geometric primitives within this cell set
				for(p = 0; p < npolys; p++)
				{
					fprintf(outfile,"// start of polyline cell set\n");

					for (i=in_ncl[p*2]; i <= (in_ncl[p*2+1])-1; i++)
					{
						/* starting end of cylinder */
						raw[0] = in_coords[i*nspace+0];
						raw[1] = in_coords[i*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[i*nspace+2] : 0.0;
						MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
						VEC_ADD(raw, raw, xlate);

						fprintf(outfile,"cylinder {\n");

						fprintf(outfile,"\t< %f, %f, %f >,", -raw[0], raw[1], -raw[2]);

						/* the other end of cylinder */
						raw[0] = in_coords[(i+1)*nspace+0];
						raw[1] = in_coords[(i+1)*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[(i+1)*nspace+2] : 0.0;
						MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
						VEC_ADD(raw, raw, xlate);

						fprintf(outfile," < %.3f, %.3f, %.3f >\n", -raw[0], raw[1], -raw[2]);

						fprintf(outfile,"\tline_thickness\n}\n");
					} // end of vertex loop
				} // end of primitive loop
				fprintf(outfile,"// end polyline\n");
				break;

    		case CELL_POLYTRI:

            	npolys = fld[nf].cell_set[cs].npolys;

            	// loop over geometric primitives within this cell set
            	for (p = 0; p < npolys; p++)
            	{
					fprintf(outfile,"// start of polytri cell set\n");

					for (i=in_ncl[p*2]; i<=(in_ncl[p*2+1])-2; i++)
					{
    					/* first vertex of triangle */
    					raw[0] = in_coords[i*nspace+0];
    					raw[1] = in_coords[i*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[i*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile,"triangle {\n");

    					fprintf(outfile,"\t< %f, %f, %f >,", -raw[0], raw[1], -raw[2]);

    					/* middle point */
    					raw[0] = in_coords[(i+1)*nspace+0];
    					raw[1] = in_coords[(i+1)*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[(i+1)*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile," < %f, %f, %f >,", -raw[0], raw[1], -raw[2]);

    					/* last point of triangle */
    					raw[0] = in_coords[(i+2)*nspace+0];
    					raw[1] = in_coords[(i+2)*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[(i+2)*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile," < %f, %f, %f >\n", -raw[0], raw[1], -raw[2]);

    					fprintf(outfile,"}\n");

					} // end of vertex loop
            	} // end of primitive loop
            	fprintf(outfile,"// end polytri\n");
            	break;

    		case CELL_LINE:

				// loop over geometric primitives within this cell set
				for (i = 0; i < npolys; i++)
				{
    				fprintf(outfile,"// start of disjoint line cell set\n");

    				j = in_ncl[i*2];    // pull connectivity
    				k = in_ncl[i*2+1];

    				/* starting end of cylinder */
    				raw[0] = in_coords[j*nspace+0];
    				raw[1] = in_coords[j*nspace+1];
					raw[2] = (nspace == 3) ? in_coords[j*nspace+2] : 0.0;
    				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    				VEC_ADD(raw, raw, xlate);

    				fprintf(outfile,"cylinder {\n");

    				fprintf(outfile,"\t< %f, %f, %f >,", -raw[0], raw[1], -raw[2]);

    				/* the other end of cylinder */
    				raw[0] = in_coords[(k)*nspace+0];
    				raw[1] = in_coords[(k)*nspace+1];
					raw[2] = (nspace == 3) ? in_coords[k*nspace+2] : 0.0;
    				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    				VEC_ADD(raw, raw, xlate);

    				fprintf(outfile," < %f, %f, %f >\n", -raw[0], raw[1], -raw[2]);

    				fprintf(outfile,"\tline_thickness\n");

    				fprintf(outfile,"}\n");

				} // end of primitive loop

				fprintf(outfile,"// end line cell set\n");
				break;

			case CELL_HEX:

				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
					int s[12][2] = {{0,1},
									{1,2},
									{2,3},
									{3,0},
									{4,5},
									{5,6},
									{6,7},
									{7,4},
									{0,4},
									{1,5},
									{2,6},
									{3,7}};

					for(j=0; j < 12; ++j)
					{
						int k1 = i*8+s[j][0];
						int k2 = i*8+s[j][1];
						
    					fprintf(outfile,"cylinder {\n");
						
    					/* starting end of cylinder */
    					raw[0] = in_coords[k1*nspace+0];
    					raw[1] = in_coords[k1*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[k1*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile,"\t< %f, %f, %f >,", -raw[0], raw[1], -raw[2]);

    					/* the other end of cylinder */
    					raw[0] = in_coords[k2*nspace+0];
    					raw[1] = in_coords[k2*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[k2*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile," < %f, %f, %f >\n", -raw[0], raw[1], -raw[2]);

    					fprintf(outfile,"\tline_thickness\n");
						
						if(rgb) fprintf(outfile,"\tpigment { color rgbt < %.3f, %.3f, %.3f, trans > }\n", rgb[3*k1+0], rgb[3*k1+1], rgb[3*k1+2]);

    					fprintf(outfile,"}\n");
					}
					
					// spheres on the vertices
					for(j=0; j < 8; ++j)
					{
						int k1 = i*8+j;
						
    					/* the other end of cylinder */
    					raw[0] = in_coords[k1*nspace+0];
    					raw[1] = in_coords[k1*nspace+1];
						raw[2] = (nspace == 3) ? in_coords[k1*nspace+2] : 0.0;
    					MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    					VEC_ADD(raw, raw, xlate);

    					fprintf(outfile, "sphere {\n\t<%.3f, %.3f, %.3f>\n\tsphere_radius\n",
							 -raw[0], raw[1], -raw[2]);
							 
						if(rgb) fprintf(outfile,"\tpigment { color rgbt < %.3f, %.3f, %.3f, trans > }\n", rgb[3*k1+0], rgb[3*k1+1], rgb[3*k1+2]);

    					fprintf(outfile,"}\n");
					}
				} // end of primitive loop
				fprintf(outfile,"// end of hex cell set border\n\n");
				break;
				
			case CELL_POINT:
				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
    				/* the other end of cylinder */
    				raw[0] = in_coords[i*nspace+0];
    				raw[1] = in_coords[i*nspace+1];
					raw[2] = (nspace == 3) ? in_coords[i*nspace+2] : 0.0;
    				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    				VEC_ADD(raw, raw, xlate);

    				fprintf(outfile, "    sphere {\n        <%.3f, %.3f, %.3f>\n        sphere_radius\n    }\n",
						 -raw[0], raw[1], -raw[2]);
				}
            	break;

			case CELL_POINT_C:
				// loop over geometric primitives within this cell set
				for(i = 0; i < npolys; i++)
				{
    				/* the other end of cylinder */
    				raw[0] = in_coords[i*nspace+0];
    				raw[1] = in_coords[i*nspace+1];
					raw[2] = (nspace == 3) ? in_coords[i*nspace+2] : 0.0;
    				MATvec3_mat4_multiply(raw, (Matr4 *)xfm);
    				VEC_ADD(raw, raw, xlate);

    				fprintf(outfile, "    sphere {\n        <%.3f, %.3f, %.3f>\n        sphere_radius\n", -raw[0], raw[1], -raw[2]);
					fprintf(outfile,"\tpigment { color rgbt < %.3f, %.3f, %.3f, trans > }\n", rgb[3*i+0], rgb[3*i+1], rgb[3*i+2]);
    				fprintf(outfile, "    }\n");
				}
            	break;

    		default:
            	break;
				
			} // end of switch

			fprintf(outfile,"// end of cell set\n");

    	} // end of cell set loop

		// free arrays, communicating changes to OM
		ARRfree((char*)in_coords);
		ARRfree((char*)xfm);
		ARRfree((char*)xlate);
		ARRfree((char*)in_ncl);
		if(node_data) ARRfree((char*)node_data);
		if(normals) ARRfree((char*)normals);
		if(rgb) ARRfree((char*)rgb);

		fprintf(outfile,"} // end of field union\n");

	} // end of ARR field loop
}

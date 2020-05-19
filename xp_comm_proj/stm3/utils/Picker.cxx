
#include "Picker_gen.h"
#include <math.h>
#include <avs/mathutil.h>

// b is the vertex and the other two define the angle to be measured
static double valence_angle(float *a, float *b, float *c)
{
	double v0, v1, v2;
	double w0, w1, w2;
	double dot_product, lv2, lw2;

	v0 = a[0] - b[0];
	w0 = c[0] - b[0];
	v1 = a[1] - b[1];
	w1 = c[1] - b[1];
	v2 = a[2] - b[2];
	w2 = c[2] - b[2];

	dot_product = v0*w0 + v1*w1 + v2*w2;
	lv2 = v0*v0 + v1*v1 + v2*v2;
	lw2 = w0*w0 + w1*w1 + w2*w2;

	return acos(dot_product/sqrt(lv2*lw2))*180./M_PI;
}

// torsion angle is the angle between the planes defined by a, m1, m2 and m1, m2, b
static double torsion_angle(float *a, float *m1, float *m2, float *b)
{
	float v1a[3], v12[3], v21[3], v2b[3];
	
	// vectors
	for(int i=0; i < 3; ++i)
	{
		v1a[i] = a[i]-m1[i];
		v2b[i] = b[i]-m2[i];
		v12[i] = m2[i]-m1[i];
		v21[i] = -v12[i];
	}

	// find normal to the plane defined by v1a and v12
	float n1[3];
	AVS_CROSS(v1a, v12, n1);

	// find normal to the plane defined by v21 and v2b
	float n2[3];
	AVS_CROSS(v21, v2b, n2);

	double dot_product = AVS_DOT(n1, n2);
	double l1 = AVS_LENGTH(n1);
	double l2 = AVS_LENGTH(n2);

	return acos(dot_product/(l1*l2))*180./M_PI;
}

// Beware! Here I don't test for special cases!
void add_angle_indicator(float *pt, int additional_points, float angle)
{
	int i, j, k;
	float v[3], w[3];
	
	// unitary vectors from &pt[0] and &pt[6]
	for(i=0; i < 3; ++i)
	{
		v[i] = pt[i]-pt[3+i];
		w[i] = pt[6+i]-pt[3+i];
	}
	AVS_UNITIZE(v, 1e-8);
	AVS_UNITIZE(w, 1e-8);

	// find normal to plane containing the two sides
	float n[3];
	AVS_CROSS(v, w, n);

	// complete the axis triad
	float t[3];
	AVS_CROSS(v, n, t);
	AVS_UNITIZE(t, 1e-8);

	// create the rotation matrix
	float R[3][3];
	for(i=0; i < 3; ++i)
	{
		R[i][0] = t[i]; 
		R[i][1] = v[i]; 
		R[i][2] = n[i];
	}

	float vect[3];
	vect[2] = 0.F;
	for(i=0; i < additional_points; ++i)
	{
		float delta = (float)i/(additional_points-1.F) * (angle/180.F*M_PI);
		
		vect[0] = -(float)sin(delta);
		vect[1] =  (float)cos(delta);

		for(j=0; j < 3; ++j)
		{
			float res = 0.F;
			for(k=0; k < 3; ++k) res += R[j][k] * vect[k];
			pt[9+3*i+j] = res+pt[3+j];
		}
	}
}

//
//	The operations
//
enum {
        PICK_DISABLED,
        PICK_ATOM,
        PICK_DISTANCE,
        PICK_ANGLE,
        PICK_TORSION
} PickOpType;

int
STM3_UTILS_Picker::DoPicking(OMevent_mask event_mask, int seq_num)
{
	// in (MolecularDataType read)
	// mode (OMXenum read req notify)
	// pick_info (GDpick_info read req notify)
	// label_pt (OMXfloat_array write)
	// label (OMXstr write)
	// line_size (OMXint write)
	// line_pt (OMXfloat_array write)
	// nprev_atoms (OMXint write)
	// prev_atoms (OMXint_array write)
	char msg[128];

	// changing mode means reset everything
	if(mode.changed(seq_num) || (int)mode == PICK_DISABLED || pick_info.npicked < 1)
	{
		label = " ";
		extended_label = " ";
		line_size = 0;
		nprev_atoms = nlast_atoms = 0;
		OMparse_buffer(this->obj_id(OM_OBJ_RW), "label_pt=>;", 0);
		OMparse_buffer(this->obj_id(OM_OBJ_RW), "line_conn=>;", 0);
		return 1;
	}
	
	// get the atom index
	int curr_atom = pick_info.pick_data[0].verti;
	
	// get atom coordinates
	float *points_arr = (float *)in.xyz_lst.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!points_arr) return 0;

	// output the current atom info if mode == PICK_ATOM or during multi point pick
	if((int)mode == PICK_ATOM || (int)nprev_atoms < (int)mode-1)
	{
		float *label_pt_arr = (float *)label_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(label_pt_arr)
		{
			label_pt_arr[0] = points_arr[curr_atom*3+0];
			label_pt_arr[1] = points_arr[curr_atom*3+1];
			label_pt_arr[2] = points_arr[curr_atom*3+2];
			ARRfree(label_pt_arr);
		}
		char sa[128], *atom_name;
		char sr[128], *res_name;
		atom_name = in.atom_name.ret_str_array_val(curr_atom, sa);
		res_name  = in.residue_name.ret_str_array_val(curr_atom, sr);
		
		float chrg = 0.0F;
		float *charge_arr = (float *)in.charge_lst.ret_array_ptr(OM_GET_ARRAY_RD);
		if(charge_arr)
		{
			chrg = charge_arr[curr_atom];
			ARRfree(charge_arr);
		}
		sprintf(msg, "%s - %d\n(%.2f %.2f %.2f)\n%s\ncharge: %.2f",
					atom_name, curr_atom+1, points_arr[curr_atom*3+0], points_arr[curr_atom*3+1], points_arr[curr_atom*3+2], res_name, chrg);
		label = msg;
		sprintf(msg, "atom: %s\natom number: %d\ncoords: (%.5f %.5f %.5f)\nresidue: %s\ncharge: %.5f",
					atom_name, curr_atom+1, points_arr[curr_atom*3+0], points_arr[curr_atom*3+1], points_arr[curr_atom*3+2], res_name, chrg);
		extended_label = msg;
	}

	// if mode == PICK_ATOM nothing else to do
	if((int)mode == PICK_ATOM)
	{
		ARRfree(points_arr);
		return 1;
	}

	// save the atom index and update the counters of saved atoms index (nprev_atoms) and (nlast_atoms) 
	int *ppt_arr = (int *)prev_atoms.ret_array_ptr(OM_GET_ARRAY_RW);
	if(!ppt_arr)
	{
		ARRfree(points_arr);
		return 0;
	}
	int idx = nprev_atoms;
	ppt_arr[idx] = curr_atom;
	nprev_atoms = nlast_atoms = idx+1;

	// start with no line enabled
	line_size = 0;
    line_conn.set_array_size(0);
	OMparse_buffer(this->obj_id(OM_OBJ_RW), "line_conn=>;", 0);
	
	// after two clicks measure distance
	if((int)mode == PICK_DISTANCE && (int)nprev_atoms == 2)
	{
		// compute distance
		int prev_atom = ppt_arr[0];
		float dx = points_arr[curr_atom*3+0] - points_arr[prev_atom*3+0];
		float dy = points_arr[curr_atom*3+1] - points_arr[prev_atom*3+1];
		float dz = points_arr[curr_atom*3+2] - points_arr[prev_atom*3+2];
		
		double dist = sqrt(dx*dx+dy*dy+dz*dz);
		
		// position label
		float *label_pt_arr = (float *)label_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(label_pt_arr)
		{
			label_pt_arr[0] = (points_arr[curr_atom*3+0] + points_arr[prev_atom*3+0])/2.F;
			label_pt_arr[1] = (points_arr[curr_atom*3+1] + points_arr[prev_atom*3+1])/2.F;
			label_pt_arr[2] = (points_arr[curr_atom*3+2] + points_arr[prev_atom*3+2])/2.F;
			ARRfree(label_pt_arr);
		}

		sprintf(msg, "lenght: %.2lf", dist);
		label = msg;
		sprintf(msg, "lenght: %.5lf", dist);
		extended_label = msg;
		
		// prepare the measurement line
		line_size = 2;
		float *line_pt_arr = (float *)line_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_pt_arr)
		{
			line_pt_arr[0] = points_arr[prev_atom*3+0];
			line_pt_arr[1] = points_arr[prev_atom*3+1];
			line_pt_arr[2] = points_arr[prev_atom*3+2];
			line_pt_arr[3] = points_arr[curr_atom*3+0];
			line_pt_arr[4] = points_arr[curr_atom*3+1];
			line_pt_arr[5] = points_arr[curr_atom*3+2];
			ARRfree(line_pt_arr);
		}

        line_conn.set_array_size(2);
		int *line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_conn_arr)
		{
			line_conn_arr[0] = 0;
			line_conn_arr[1] = 1;
			
			ARRfree(line_conn_arr);
		}
		
		// reset the number of saved atoms so next pick can restart
		nprev_atoms = 0;
	}
	// picking on 2 points during angle measurement draw the first side of the angle
	else if(((int)mode == PICK_ANGLE || (int)mode == PICK_TORSION) && (int)nprev_atoms == 2)
	{		
		int prev_atom = ppt_arr[0];
		line_size = 2;
		float *line_pt_arr = (float *)line_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_pt_arr)
		{
			line_pt_arr[0] = points_arr[prev_atom*3+0];
			line_pt_arr[1] = points_arr[prev_atom*3+1];
			line_pt_arr[2] = points_arr[prev_atom*3+2];
			line_pt_arr[3] = points_arr[curr_atom*3+0];
			line_pt_arr[4] = points_arr[curr_atom*3+1];
			line_pt_arr[5] = points_arr[curr_atom*3+2];
			ARRfree(line_pt_arr);
		}
        line_conn.set_array_size(2);
		int *line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_conn_arr)
		{
			line_conn_arr[0] = 0;
			line_conn_arr[1] = 1;
			
			ARRfree(line_conn_arr);
		}
	}
	// picking the third point during angle measurement draw the second side of the angle and compute the angle
	else if((int)mode == PICK_ANGLE && (int)nprev_atoms == 3)
	{
		int start_atom = ppt_arr[0];
		int mid_atom   = ppt_arr[1];
		
		// compute angle
		double angle = valence_angle(&points_arr[start_atom*3], &points_arr[mid_atom*3], &points_arr[curr_atom*3]);
		
		int additional_points = (int)((angle/10.)+0.5);
		if(additional_points < 2) additional_points = 2;
		
		line_size = 3+additional_points;
		float *line_pt_arr = (float *)line_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_pt_arr)
		{
			line_pt_arr[0] = points_arr[start_atom*3+0];
			line_pt_arr[1] = points_arr[start_atom*3+1];
			line_pt_arr[2] = points_arr[start_atom*3+2];
			line_pt_arr[3] = points_arr[mid_atom*3+0];
			line_pt_arr[4] = points_arr[mid_atom*3+1];
			line_pt_arr[5] = points_arr[mid_atom*3+2];
			line_pt_arr[6] = points_arr[curr_atom*3+0];
			line_pt_arr[7] = points_arr[curr_atom*3+1];
			line_pt_arr[8] = points_arr[curr_atom*3+2];
			add_angle_indicator(line_pt_arr, additional_points, (float)angle);
			ARRfree(line_pt_arr);
		}
		
        line_conn.set_array_size(4);
		int *line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_conn_arr)
		{
			line_conn_arr[0] = 0;
			line_conn_arr[1] = 2;
			line_conn_arr[2] = 3;
			line_conn_arr[3] = 3+additional_points-1;
			
			ARRfree(line_conn_arr);
		}

		// position the label
		sprintf(msg, "angle: %.2lf", angle);
		label = msg;
		sprintf(msg, "angle: %.5lf", angle);
		extended_label = msg;
		
		float *label_pt_arr = (float *)label_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(label_pt_arr)
		{
			label_pt_arr[0] = line_pt_arr[9+3*(additional_points/2)+0];
			label_pt_arr[1] = line_pt_arr[9+3*(additional_points/2)+1];
			label_pt_arr[2] = line_pt_arr[9+3*(additional_points/2)+2];
			ARRfree(label_pt_arr);
		}
		
		nprev_atoms = 0;
	}
	// picking the third point during angle measurement draw the second side of the angle and compute the angle
	else if((int)mode == PICK_TORSION && (int)nprev_atoms == 3)
	{
		int start_atom = ppt_arr[0];
		int mid_atom   = ppt_arr[1];
		
		line_size = 3;
		float *line_pt_arr = (float *)line_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_pt_arr)
		{
			line_pt_arr[0] = points_arr[start_atom*3+0];
			line_pt_arr[1] = points_arr[start_atom*3+1];
			line_pt_arr[2] = points_arr[start_atom*3+2];
			line_pt_arr[3] = points_arr[mid_atom*3+0];
			line_pt_arr[4] = points_arr[mid_atom*3+1];
			line_pt_arr[5] = points_arr[mid_atom*3+2];
			line_pt_arr[6] = points_arr[curr_atom*3+0];
			line_pt_arr[7] = points_arr[curr_atom*3+1];
			line_pt_arr[8] = points_arr[curr_atom*3+2];
			ARRfree(line_pt_arr);
		}
        line_conn.set_array_size(2);
		int *line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_conn_arr)
		{
			line_conn_arr[0] = 0;
			line_conn_arr[1] = 2;
			
			ARRfree(line_conn_arr);
		}
	}
	// picking the third point during angle measurement draw the second side of the angle and compute the angle
	else if((int)mode == PICK_TORSION && (int)nprev_atoms == 4)
	{	
		int start_atom = ppt_arr[0];
		int mid1_atom  = ppt_arr[1];
		int mid2_atom  = ppt_arr[2];
		
		// compute the torsion angle
		double angle = torsion_angle(&points_arr[start_atom*3], &points_arr[mid1_atom*3], &points_arr[mid2_atom*3], &points_arr[curr_atom*3]);
		
		// position the label
		sprintf(msg, "torsion angle: %.2lf", angle);
		label = msg;
		sprintf(msg, "torsion angle: %.5lf", angle);
		extended_label = msg;
		
		float *label_pt_arr = (float *)label_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(label_pt_arr)
		{
			label_pt_arr[0] = (points_arr[mid1_atom*3+0] + points_arr[mid2_atom*3+0])/2.0F;
			label_pt_arr[1] = (points_arr[mid1_atom*3+1] + points_arr[mid2_atom*3+1])/2.0F;
			label_pt_arr[2] = (points_arr[mid1_atom*3+2] + points_arr[mid2_atom*3+2])/2.0F;
			ARRfree(label_pt_arr);
		}
		
		line_size = 4;
		float *line_pt_arr = (float *)line_pt.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_pt_arr)
		{
			line_pt_arr[0]  = points_arr[start_atom*3+0];
			line_pt_arr[1]  = points_arr[start_atom*3+1];
			line_pt_arr[2]  = points_arr[start_atom*3+2];
			line_pt_arr[3]  = points_arr[mid1_atom*3+0];
			line_pt_arr[4]  = points_arr[mid1_atom*3+1];
			line_pt_arr[5]  = points_arr[mid1_atom*3+2];
			line_pt_arr[6]  = points_arr[mid2_atom*3+0];
			line_pt_arr[7]  = points_arr[mid2_atom*3+1];
			line_pt_arr[8]  = points_arr[mid2_atom*3+2];
			line_pt_arr[9]  = points_arr[curr_atom*3+0];
			line_pt_arr[10] = points_arr[curr_atom*3+1];
			line_pt_arr[11] = points_arr[curr_atom*3+2];
			ARRfree(line_pt_arr);
		}
        line_conn.set_array_size(2);
		int *line_conn_arr = (int *)line_conn.ret_array_ptr(OM_GET_ARRAY_WR);
		if(line_conn_arr)
		{
			line_conn_arr[0] = 0;
			line_conn_arr[1] = 3;
			
			ARRfree(line_conn_arr);
		}
		nprev_atoms = 0;
	}

	// return 1 for success
	ARRfree(ppt_arr);
	ARRfree(points_arr);
	return 1;
}

/*
** Module: DVcombine_data_to_HSV
** Author: Mark Lambert
** Date: 1/11/99
** Description: 
**    Module used to map upto 3 data values to Hue, Saturation and Value/Light values 
**    and then convert to RGB values for use in Express.
*/

#include "xp_comm_proj/combdata/gen.hxx"

/*
** Function for converting from HSV to RGB
*/
void hsv_to_rgb(float h,float s,float v,float *r,float *g,float *b){ 
	int	i;
	float	f, p, q, t;
	
	if(v == 0.0){
		*r = 0.0;
		*g = 0.0;
		*b = 0.0;
		return;
	}
	if(s == 0.0){
		*r = v;
		*g = v;
		*b = v;
	}
	
	h *= 6.0;
	if(h >= 6.0)
		h = 0.0;
	
	i = h;
	f = h - i;
	
	p = v*(1.0-s);
	q = v*(1.0-s*f);
	t = v*(1.0-s*(1.0-f));
	
	switch(i)
	{ 
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
	
	/* values should be in the range 0.0 to 0.999999... */
	if (*r < 0.0)
		*r = 0.0;
	else if (*r > 0.999999)
		*r = 0.999999;
	if (*g < 0.0)
		*g = 0.0;
	else if (*g > 0.999999)
		*g = 0.999999;
	if (*b < 0.0)
		*b = 0.0;
	else if (*b > 0.999999)
		*b = 0.999999;
	
	return;
}

/*
** Main update function
*/
int
CombineData_CombineDataMods_DVcombine_data_to_HSV::update(OMevent_mask event_mask, int seq_num)
{
	// data0 (OMXfloat_array read req notify)
	int data0_size;
	float *data0_arr;
	
	// data1 (OMXfloat_array read req notify)
	int data1_size;
	float *data1_arr;
	
	// data2 (OMXfloat_array read req notify)
	int data2_size;
	float *data2_arr;
	
	// Nin (OMXint read)
	// max0 (OMXfloat read)
	// min0 (OMXfloat read)
	// max1 (OMXfloat read)
	// min1 (OMXfloat read)
	// r (OMXfloat_array write)
	int r_size;
	float *r_arr;
	
	// g (OMXfloat_array write)
	int g_size;
	float *g_arr;
	
	// b (OMXfloat_array write)
	int b_size;
	float *b_arr;
		
	/***********************/
	/* Function's Body     */
	/***********************/
	int i;
	float rval = 0;
	float gval = 0;
	float bval = 0;
	float data0val = 0;
	float data1val = 0;
	float data2val = 0;
	
	//local copies of module parameter values, reduces OM access overhead
	float min0val = min0;
	float min1val = min1;
	float min2val = min2;
	
	float max0val = max0;
	float max1val = max1;
	float max2val = max2;
	
	int const0flag = constant0on;
	int const1flag = constant1on;
	int const2flag = constant2on;
	
	float const0val = constant0val;
	float const1val = constant1val;
	float const2val = constant2val;
	
	int invert0flag = invert0on;
	int invert1flag = invert1on;
	int invert2flag = invert2on;
	
	// copies of input and output data arrays
	
	data0_arr = (float *)data0.ret_array_ptr(OM_GET_ARRAY_RD,&data0_size);
	data1_arr = (float *)data1.ret_array_ptr(OM_GET_ARRAY_RD,&data1_size);
	data2_arr = (float *)data2.ret_array_ptr(OM_GET_ARRAY_RD,&data2_size);
	
	r_arr = (float *)r.ret_array_ptr(OM_GET_ARRAY_WR,&r_size);
	g_arr = (float *)g.ret_array_ptr(OM_GET_ARRAY_WR,&g_size);
	b_arr = (float *)b.ret_array_ptr(OM_GET_ARRAY_WR,&b_size);
	
	//normalise data values and then calculate r,g,b
	for (i= 0; i< Nin; i++){
		if (const0flag) // if using a constant value (i.e. not the data values)
			data0val= const0val;
		else if (invert0flag) // if inverting data values
			data0val= 1 - (data0_arr[i] - min0val) / (max0val - min0val);
		else // if using normalised data values
			data0val= (data0_arr[i] - min0val) / (max0val - min0val);
		
		if (const1flag) // if using a constant value (i.e. not the data values)
			data1val= const1val;
		else if (invert1flag) // if inverting data values
			data1val= 1 - (data1_arr[i] - min1val) / (max1val - min1val);
		else // if using normalised data values
			data1val= (data1_arr[i] - min1val) / (max1val - min1val);
		
		if (const2flag) // if using a constant value (i.e. not the data values)
			data2val= const2val;
		else if (invert2flag) // if inverting data values
			data2val= 1 - (data2_arr[i] - min2val) / (max2val - min2val);
		else // if using normalised data values
			data2val= (data2_arr[i] - min2val) / (max2val - min2val);
		
		hsv_to_rgb(data0val,data1val,data2val,&rval,&gval,&bval);
		r_arr[i] = rval;
		g_arr[i] = gval;
		b_arr[i] = bval;
	}
	
	// free data arrays
	if (data0_arr) {
		ARRfree(data0_arr);
	}
	if (data1_arr) {
		ARRfree(data1_arr);
	}
	if (data2_arr) {
		ARRfree(data2_arr);
	}
	if (r_arr) {
		ARRfree(r_arr);
	}
	if (g_arr) {
		ARRfree(g_arr);
	}
	if (b_arr) {
		ARRfree(b_arr);
	}

	// return 1 for success
	return(1);
}


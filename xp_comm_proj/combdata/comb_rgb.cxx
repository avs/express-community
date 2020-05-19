/*
** Module: DVcombine_data_to_RGB
** Author: Mark Lambert
** Date: 2/11/99
** Description: 
**    Module used to map upto 3 data values to Red, Green and Blue channels
*/

#include "xp_comm_proj/combdata/gen.hxx"

int
CombineData_CombineDataMods_DVcombine_data_to_RGB::update(OMevent_mask event_mask, int seq_num)
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
   // min0 (OMXfloat read)
   // min1 (OMXfloat read)
   // min2 (OMXfloat read)
   // max0 (OMXfloat read)
   // max1 (OMXfloat read)
   // max2 (OMXfloat read)
   // constant0on (OMXint read req notify)
   // constant1on (OMXint read req notify)
   // constant2on (OMXint read req notify)
   // constant0val (OMXfloat read req notify)
   // constant1val (OMXfloat read req notify)
   // constant2val (OMXfloat read req notify)
   // invert0on (OMXint read req notify)
   // invert1on (OMXint read req notify)
   // invert2on (OMXint read req notify)

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
	
	//local copies of module parameter values, reduces OM access overhead

	float data0val = 0;
	float data1val = 0;
	float data2val = 0;
	
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
		
		r_arr[i] = data0val;
		g_arr[i] = data1val;
		b_arr[i] = data2val;
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


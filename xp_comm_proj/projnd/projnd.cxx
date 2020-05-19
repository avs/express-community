
#include "gen.h"
#include <string.h>
#include <math.h>

//#define DEBUG

static float xformY[16] = {1.0, 0.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0,-1.0, 0.0, 0.0,
                           0.0, 0.0, 0.0, 1.0};

static float xformX[16] = {0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           1.0, 0.0, 0.0, 0.0,
                           0.0, 0.0, 0.0, 1.0};

static float xformZ[16] = {1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0};

static float *xform[]= {xformX, xformY, xformZ};

int
ProjectNodeData_ProjectNodeDataCore::update(OMevent_mask event_mask, int seq_num)
{
	// in (Mesh_Unif+Node_Data read notify)
	// axis (OMXint read notify)
	// method (OMXenum read notify) {
	//		0	"Maximum",
	//		1	"Depth", 
	//		2	"Depth+Maximum",
	//		3	"Mean",
	//		4	"Sum",
	//		5	"Magnitude",
	// }
	// out (Mesh_Unif+Node_Data write)

	int i,j;
	int *in_dims, *out_dims;
	float *in_points, *out_points, *out_xform_mat;
	int data_comp, in_data_size;
	
	double *d_node_data_out,*d_node_data_in;
	char *in_node_data=NULL, *out_node_data=NULL;
	int  out_data_size, veclen;
	int idx[3],idy[3];
	long index, in_index, out_index=0;   
	int *depth_val = NULL;
		
	/***********************/
	/* Function's Body	  */
	/***********************/
	#ifdef DEBUG
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ProjectNodeData_ProjectNodeDataCore::update\n");
	#endif

	#ifdef DEBUG
		fprintf(stderr,"method: %s\n", (char*) method );
	#endif
	
	if (!(char*) method) {
		ERRverror("",ERR_NO_HEADER | ERR_PRINT,"ProjectNodeData: No Mapping Mode Specified!\n");
		#ifdef DEBUG
			fprintf(stderr,"no method specified: %s\n", (char*) method );
		#endif
		return 0;
	}
	 
	out.ndim = ((int) in.ndim) - 1;
	out.nspace = ((int) in.nspace) - 1; 
	out.npoints = 2;
	
	in_dims = (int *)in.dims.ret_array_ptr(OM_GET_ARRAY_RD);
	out_dims = (int *)out.dims.ret_array_ptr(OM_GET_ARRAY_WR);

	in_points = (float *)in.points.ret_array_ptr(OM_GET_ARRAY_RD);
	out_points = (float *)out.points.ret_array_ptr(OM_GET_ARRAY_WR);
	out_xform_mat = (float *)out.xform.mat.ret_array_ptr(OM_GET_ARRAY_WR);

	memcpy(out_xform_mat,xform[(int)axis],16*sizeof(float));
	
	for (i=0, j=0; i<(int) in.ndim; i++) {
		// take out the dimension for the given projection axis
		if ( i == (int) axis) continue; 
		out_dims[j] = ((int) in_dims[i]);
		out_points[j] = ((double) in_points[i]);
		out_points[((int)out.nspace)+j] = ((double) in_points[((int)in.nspace)+i]);
		j++;
	}; 

	out.nnode_data=(int)in.nnode_data;

	for (data_comp = 0; data_comp < in.nnode_data; data_comp++) { 
		veclen=(int)in.node_data[data_comp].veclen;
		out.node_data[data_comp].veclen=in.node_data[data_comp].veclen;
		out.node_data[data_comp].labels=in.node_data[data_comp].labels;
		out.node_data[data_comp].units=in.node_data[data_comp].units;

		in_node_data = (char *)in.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_RD, DTYPE_DOUBLE,&in_data_size);
		if(in_node_data)
		{
			// set pointer to output array
			out_node_data = (char *)out.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_WR, DTYPE_DOUBLE,&out_data_size);
			#ifdef DEBUG
				fprintf(stderr,"out_data_size: %i\n",out_data_size);
			#endif
			d_node_data_in=(double *)in_node_data;
			d_node_data_out=(double *)out_node_data;
			if (((int)method)== 2) //	Depth+Maximum
			{
					// create array for depth values
					depth_val=(int*) malloc(out_dims[0]*out_dims[1]*sizeof(int));
					memset(depth_val,0,out_dims[0]*out_dims[1]*sizeof(int));
			}

			// initialize out_data component
			for (i=0;i<out_dims[0]*out_dims[1];i++){
				d_node_data_out[i]= 0.0;
			}

			// loop over the whole in_data component
			for (idx[0]=0;idx[0]<in_dims[0];idx[0]++) {
				for (idx[1]=0;idx[1]<in_dims[1];idx[1]++) {
					for (idx[2]=0;idx[2]<in_dims[2];idx[2]++) {
						in_index = (idx[2] * in_dims[1] + idx[1]) * in_dims[0] + idx[0];
						switch ((int)axis) {
							case 0:	
								out_index = idx[2]*in_dims[1]+idx[1];
								break;						
							case 1:
								out_index = idx[2]*in_dims[0]+idx[0];
								break;						
							case 2:
								out_index = idx[1]*in_dims[0]+idx[0];
								break;						
						}
						switch ((int)method) {
							case 0:	//	Maximum
								d_node_data_out[out_index] = (d_node_data_out[out_index]<d_node_data_in[in_index])?d_node_data_in[in_index]:d_node_data_out[out_index];
								break; 
							case 1:	//	Depth
								idy[0]=idx[0];
								idy[1]=idx[1];
								idy[2]=idx[2];
								idy[axis]=(int)d_node_data_out[out_index];
								index = (idy[2] * in_dims[1] + idy[1]) * in_dims[0] + idy[0];							
								d_node_data_out[out_index] = (d_node_data_in[index]<d_node_data_in[in_index])?idx[axis]:d_node_data_out[out_index];
								break; 
							case 2:	//	Depth+Maximum
								idy[0]=idx[0];
								idy[1]=idx[1];
								idy[2]=idx[2];
								idy[axis]=depth_val[out_index];
								index = (idy[2] * in_dims[1] + idy[1]) * in_dims[0] + idy[0];							
								if (d_node_data_in[index]<d_node_data_in[in_index]){
									d_node_data_out[out_index] = d_node_data_in[in_index]*idx[axis];
									depth_val[out_index]=idx[axis];
								};
								break; 
							case 3:	//	Mean
								d_node_data_out[out_index] += d_node_data_in[in_index]/in_dims[axis];
								break; 						
							case 4:	//	Sum
								d_node_data_out[out_index] += d_node_data_in[in_index];
								break; 
							case 5: // Magnitude
								d_node_data_out[out_index] += (d_node_data_in[in_index]*d_node_data_in[in_index]);
								break;
						}
					}
				}
			}
			// Postprocessing out_data component
			if (((int)method)==5) {
				for (i=0;i<out_dims[0]*out_dims[1];i++){
					d_node_data_out[i]=sqrt(d_node_data_out[i]);
				}
			}
		}
		if (in_node_data)
			ARRfree(in_node_data);
		if (out_node_data)
			ARRfree(out_node_data);
	}
	
	/***********************/
	/* Function's Cleanup  */
	/***********************/

	if (in_dims)
		ARRfree(in_dims);
	if (in_points)
		ARRfree(in_points);

	if (out_dims)
		ARRfree(out_dims);
	if (out_points)
		ARRfree(out_points);		
	if (out_xform_mat)
		ARRfree(out_xform_mat);
		
	if (depth_val)
		free(depth_val);

	// return 1 for success
	return(1);
}

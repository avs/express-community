
#include <math.h>
#include "gen.hxx"

#define DEBUG

//
//           CatmullSplineFldCore code
//
// appends src_arr to dest_arr and sets dest_size
// if dest_arr is null, allocate it
//

void Catmull_2_Catmull_2Mods_CatmullSplineFldCore::append_array(float*& dest_arr,
                                                            int& dest_size,
                                                            float* src_arr, 
                                                            int src_size, 
                                                            int nspace) 
{
	int temp_size = dest_size + src_size;
	float* temp_arr = (float*)malloc(temp_size*sizeof(float)*nspace);

	if (dest_arr) 
	{
		memcpy (temp_arr, dest_arr, dest_size*sizeof(float)*nspace);
		free(dest_arr);
	}

	if (src_arr) 
	{
		memcpy (temp_arr+(dest_size*nspace), src_arr,
		        src_size*sizeof(float)*nspace);
	}

	dest_arr = temp_arr;
	dest_size = temp_size;
}


//
// return 1 if pt1 and pt2 are closer than epsilon distance
//

int Catmull_2_Catmull_2Mods_CatmullSplineFldCore::cmp_coords(float* pt1, 
                                                         float* pt2,
                                                         int nspace, 
                                                         float epsilon) 
{
	int i;
	float temp, result=0;

	for (i=0; i<nspace; i++) 
	{
		temp = pt2[i] - pt1[i];
		result += temp*temp;
	}

	result = sqrt(result);
	if ( (result > (-epsilon)) && (result < epsilon) )
		return 1;
	else
		return 0;
}

//
// Creates a mask array that can be later used to filter out members of src_arr
//   each point is compared with the following
//   if the distance is below threshold the mask element is set
//

void Catmull_2_Catmull_2Mods_CatmullSplineFldCore::gen_filter_mask(int*& mask_arr, 
                                                               float* src_arr, 
                                                               int src_size, 
                                                               int nspace, 
                                                               float epsilon) 
{
	if ((src_size==0) || (src_arr==NULL)) 
	{
		mask_arr = NULL;
	}

	mask_arr = (int*)malloc(sizeof(int)*src_size); // allocate the Mask Array
	
	if (src_size==1) 
	{
		mask_arr[0]=1;
		return;
	}

	int* mask_ref = mask_arr;	
	
	mask_ref[0] = 1; // keep first point
	
	mask_ref += 1; // move mask_ref forward
	
	float* curr_src_pt = src_arr;
	float* next_src_pt = NULL;

	for (int i=0; i<(src_size-1); i++) 
	{
		next_src_pt = curr_src_pt + nspace;
		
        // if point are far apart, keep next_src_point else mask it

		mask_ref[0] = !cmp_coords(curr_src_pt, next_src_pt, nspace, epsilon);

		mask_ref += 1; //move mask_ref forward
		
		curr_src_pt = next_src_pt;
	}
}

//
// Filters src_arr into dest_arr by applying a mask array
//   the mask array is generated by gen_filter_mask
//

void Catmull_2_Catmull_2Mods_CatmullSplineFldCore::apply_filter_mask
			(float*& dest_arr, int& dest_size, 
			 float* src_arr, int src_size, 
			 int* mask_arr, int nspace) 
{

	if ((src_size==0) || (src_arr==NULL)) 
	{
		dest_size = 0;
		dest_arr = NULL;
	}
	
	// worst case,  keep all points

	dest_arr = (float*)malloc(sizeof(float)*src_size*nspace); 
	float* dest_ref = dest_arr;	

	dest_size = 0;

	float* src_ref = src_arr;	
	int* mask_ref = mask_arr;	

	for (int i=0; i<src_size; i++) 
	{
		if (mask_ref[0]) 
		{
			//copy the current element
			memcpy(dest_ref, src_ref, sizeof(float)*nspace);
			dest_size++;
			dest_ref += nspace;	//move dest_ref forward
		}
		src_ref += nspace;   //move src_ref forward
		mask_ref+=1;         //move mask_ref forward
	}
}

//
// generates a dest_arr of node data by interpolating values based on 
// the src_arr assumes that there is a correspondance between in nodes
// and out nodes

// *    *    *    *    *     : src_arr, * are input nodes
// *++++*++++*++++*++++*++++ : dest_arr, where input nodes are *, + are 
//                             added points (i.e. 4)
// node_data values of * are copied, node_data values of + are interpolated
//

void Catmull_2_Catmull_2Mods_CatmullSplineFldCore::interpolate_node_data 
				(float*& dest_arr, int& dest_size, float* src_arr,
				 int src_size, int added_points, int veclen, int is_open) 
{
	dest_size = src_size + added_points*(src_size - 1);
	if (!is_open)
	   dest_size += added_points + 1;

	dest_arr = (float*)malloc(sizeof(float)*veclen*dest_size);	

	float* curr_node = src_arr;
	float* next_node = NULL;
	float* dest_node = dest_arr;
	float start_val, delta_val;

	int npoints = added_points + 2;
	float deltaX = 1.0f/(float)(npoints-1);
	
	for (int i=0; i<(src_size-1); i++) 
	{
		next_node = curr_node + veclen;
		start_val = curr_node[0];
		delta_val = next_node[0] - start_val;

		for (int j=0; j<npoints; j++) 
		{
			dest_node[0] = start_val + delta_val*(float)j*deltaX;
			dest_node += veclen; //forward to the next point
		}

		// move dest_node back to inital node of next line segement
		dest_node -= veclen;
		curr_node += veclen; // increment node pointer
	}

	// If polyline is closed complete interpolation of last nodes
	if (!is_open) 
	{
		curr_node = src_arr + veclen*(src_size-1); // last but one element
		next_node = src_arr;			  // last element
		start_val = curr_node[0];
		delta_val = next_node[0] - start_val;
		for (int j=0; j<npoints; j++) 
		{
			dest_node[0] = start_val + delta_val*(float)j*deltaX;
			dest_node += veclen;
		}
	}

}

int Catmull_2_Catmull_2Mods_CatmullSplineFldCore::update(OMevent_mask event_mask,
                                                     int seq_num)
{
	// in_fld (Mesh+Node_Data read req notify)
   
	int In_NNodes = (int)in_fld.nnodes;
	int In_NNodeData = 0;
	int In_NCellSet = (int)in_fld.ncell_sets;
	int NSpace=(int)in_fld.nspace;
	out.nnodes=0;
	out.nnode_data=0;
	out.nspace=NSpace;
	out.ncell_sets=In_NCellSet;

	if (In_NCellSet==0)
		return(1);

	int UseND = 0;

	if ((in_fld.nnode_data.valid_obj()) && (in_fld.nnode_data > 0))
	{
		UseND = 1;
		In_NNodeData = (int)in_fld.nnode_data;
	}

	int NodeDataType=0;
	int NodeDataVecLen=0;
   
	float *In_NodeData = NULL;
	if (UseND) 
	{
		FLDget_node_data_type (in_fld.obj_id(),0,&NodeDataType);
		NodeDataVecLen = (int)in_fld.node_data[0].veclen;
		UseND = (NodeDataType==4) && (NodeDataVecLen==1);
		
		if (!(NodeDataType==4))
			printf("Warning Node Data Type should be float (%d)!\n",
			       NodeDataType);
				   
		if (!(NodeDataVecLen==1))
			printf("Warning Node Data VecLen should be 1 (%d)!\n",
					NodeDataVecLen);
	}
	
	float *In_Coords = (float *)in_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
   
	int In_NPolys = (int)in_fld.cell_set[0].npolys;
	int *In_PolyConnect = (int *)in_fld.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
   
	out.cell_set[0].npolys = In_NPolys;
	int *Out_PolyConnect = (int *)out.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

	if (UseND) 
	{
		In_NodeData = (float*)in_fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);
	}
   
	float Smoothness = (float)smoothness;
	int PointsAdded = (int)points_added;
	int SplineType = (int)spline_type;	//not yet used
	float Epsilon = (float)DistanceEps;

	int II=0;
	int In_StartNode=0;
	int In_EndNode=0;
	int In_NPoints=0;
	int IsOpen = 0;
	float In_StartCoords[3];
	float In_EndCoords[3];
	float* In_CatMullCoords=NULL;
	int res=0;
	int Out_NthNodeSize=0;
	int Out_NodeSize=0;
	int Out_NodeDataSize=0;
	int Out_NthNodeDataSize=0;
	float* Out_NthCoords=NULL;
	float* Out_Coords=NULL;

	float* In_NthNodeData=NULL;
	float* Out_NthNodeData=NULL;
	float* Out_NodeData=NULL;

	char msg[255];
   
	CatmullRomSpline *crs;   
	for (II=0;II<In_NPolys;II++) 
	{
		//get the start and end node of this polyline
		In_StartNode = In_PolyConnect[II*2+0];
		In_EndNode   = In_PolyConnect[II*2+1];
		In_NPoints   = In_EndNode - In_StartNode + 1;
	
		// check if the polyline is closed or not
		
		memcpy(In_StartCoords,In_Coords+(In_StartNode*NSpace),
		       sizeof(float)*NSpace);
			   
		memcpy(In_EndCoords,In_Coords+(In_EndNode*NSpace),
		       sizeof(float)*NSpace);
			   
		IsOpen = !cmp_coords(In_StartCoords, In_EndCoords, NSpace, Epsilon);
		
		if (!IsOpen) 
		{ 
			// don't use the last point (is similar to the first)
			In_EndNode--;
			In_NPoints--;
		}
		
		// get the coordinates of this polyline
		
		In_CatMullCoords = (float*) malloc(In_NPoints*sizeof(float)*NSpace);
		
		memcpy(In_CatMullCoords,In_Coords+(In_StartNode*NSpace),
		       In_NPoints*sizeof(float)*NSpace);
			   
		// get the node data of this polyline
	
		In_NthNodeData = (float*) malloc(In_NPoints*sizeof(float)*NodeDataVecLen);

		memcpy(In_NthNodeData,In_NodeData+(In_StartNode*NodeDataVecLen),
		       In_NPoints*sizeof(float)*NodeDataVecLen);
			   
		// filter the coordinates in order to have far points 
		// (with distance > Epsilon) to obtain better splines

		float* In_CatMullCoords_Filtered=NULL;
		float* In_NthNodeData_Filtered=NULL;
		int* In_NthMask=NULL;	
		int In_NPoints_Filtered=0;
	
		// find the mask

		gen_filter_mask(In_NthMask, In_CatMullCoords, In_NPoints, 
		                NSpace, Epsilon);

		// apply the mask to the coordinates

		apply_filter_mask(In_CatMullCoords_Filtered, In_NPoints_Filtered,
		                  In_CatMullCoords, In_NPoints, In_NthMask, NSpace);
	
		if (UseND) 
		{
	    	// apply the mask to the node_data

	    	apply_filter_mask(In_NthNodeData_Filtered, In_NPoints_Filtered,
			                  In_NthNodeData, In_NPoints, In_NthMask,
			                  NodeDataVecLen);

			if (In_NthNodeData) free(In_NthNodeData);
		}
		
		if (In_NthMask)
			free(In_NthMask);

		if (In_CatMullCoords)
			free(In_CatMullCoords);

		// call the Spline generation on filtered points

		crs = (CatmullRomSpline*) new CatmullRomSpline();
		res = crs->ComputePoints (In_CatMullCoords_Filtered, NSpace, 
		                          In_NPoints_Filtered, PointsAdded,
		                          IsOpen, (float)smoothness);

		// copy this chunk of points into the Nth-coords array

		Out_NthNodeSize = res;
		Out_NthCoords = (float*) malloc(Out_NthNodeSize*sizeof(float)*NSpace);

		if (Out_NthCoords) 
			crs->CopyResult(Out_NthCoords);	

		Out_PolyConnect[II*2+0]=Out_NodeSize;

		if (UseND) 
		{
		
	    	// this brings in the Filtered Node Data and interpolate them to
			// obtain the chunk of node data for this chunk of filtered points
			
			interpolate_node_data(Out_NthNodeData, Out_NthNodeDataSize,
			                      In_NthNodeData_Filtered, In_NPoints_Filtered,
			                      PointsAdded, NodeDataVecLen, IsOpen );

			// should be that Out_NthNodeSize <= Out_NthNodeDataSize
			if (Out_NthNodeDataSize < Out_NthNodeSize) 
			{
				printf("Warning: Interpolation produced less node data (%d) then expected (%d)\n", Out_NthNodeDataSize, Out_NthNodeSize);
	    	}

			// don't need them anymore
			if (In_NthNodeData_Filtered) free (In_NthNodeData_Filtered);
		}	
	
		//this will append this chunk of points to the master coords array, 
		//update also Out_NodeSize, reallocate the Out_Coords

		append_array(Out_Coords, Out_NodeSize, Out_NthCoords, 
		             Out_NthNodeSize, NSpace);
					 
		Out_PolyConnect[II*2+1]=Out_NodeSize-1;

		if (UseND) 
		{
		
	    	// this will append this chunk of node_data to the master node_data 
			// array, update also Out_NodeDataSize, reallocate the Out_NodeData

			append_array(Out_NodeData, Out_NodeDataSize, Out_NthNodeData, 
			             Out_NthNodeSize, NodeDataVecLen);

			if(Out_NthNodeData) free(Out_NthNodeData);
		}
		
		if(In_CatMullCoords_Filtered) free(In_CatMullCoords_Filtered);
		if(Out_NthCoords) free(Out_NthCoords);
		if (crs) delete crs;

	}
	if (In_PolyConnect) ARRfree(In_PolyConnect);
	if (In_Coords) ARRfree(In_Coords);

	out.nnodes = Out_NodeSize;

	float *Out_Coords_arr = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   
	memcpy(Out_Coords_arr,Out_Coords,Out_NodeSize*sizeof(float)*NSpace);
	if (Out_Coords) free(Out_Coords);

	if (Out_Coords_arr) ARRfree(Out_Coords_arr);
	if (Out_PolyConnect) ARRfree(Out_PolyConnect);
   
	if (UseND) 
	{
		out.nnode_data=1;

		// NodeDataType should be 4 (float)

		FLDset_node_data_type (out.obj_id(),0,NodeDataType);	

		out.node_data[0].veclen=NodeDataVecLen;			//should be 1
		if (In_NodeData) ARRfree(In_NodeData);
		
		// should be that Out_NodeDataSize == Out_NodeSize

		if (Out_NodeDataSize != Out_NodeSize) 
		{
			printf("Warning: Out_NodeDataSize (%d) different from Out_NodeSize(%d)\n",Out_NodeDataSize,Out_NodeSize);
		}

		float* Out_NodeData_arr = (float*) out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);

		memcpy(Out_NodeData_arr,Out_NodeData,
		       Out_NodeDataSize*sizeof(float)*NodeDataVecLen);

		if (Out_NodeData) 
			free(Out_NodeData);

		if (Out_NodeData_arr) 
			ARRfree(Out_NodeData_arr);
	} else {
		out.nnode_data=0;
	}
   
	// return 1 for success
	return(1);
}


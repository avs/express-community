
#include <malloc.h>
#include "gen.hxx"

//#define GFDEBUG
//#define GFDEBUG2

#define COPY_DATA(T) {\
	T*  in_fld_node_data=(T*)in_fld.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD);\
	if (!in_fld_node_data){\
		ERRverror("IsoObjectLabel",ERR_ERROR,"could not get node_data from in_field\n");\
		return(0);\
	}\
	T* out_fld_node_data=(T*)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR);\
	if (!out_fld_node_data){\
		ERRverror("IsoObjectLabel",ERR_ERROR,"could not get pointer for out_field node_data\n");\
		return(0);\
	}\
	if (lineIndep>0){\
		for (outNode=0, inNode=0; outNode<numLabels; outNode++, inNode+=Nnodes_in/numLabels) {\
			memcpy(out_fld_coordinates+(outNode*NSpace),in_fld_coordinates+(inNode*NSpace),sizeof(float)*NSpace);\
			memcpy(out_fld_node_data  +(outNode*veclen),in_fld_node_data  +(inNode*veclen),sizeof(T    )*veclen);\
		}\
	}\
	else{\
		outNode=0;\
		for(inCellSet=0; inCellSet<numInCellSets; inCellSet+=contourFreq){\
			numCellsInCellSet=(int)in_fld.cell_set[inCellSet].ncells;\
			numCellsInCellSet-=numCellsInCellSet%nodeFreq;\
			in_fld_node_connect_list = (int *)in_fld.cell_set[inCellSet].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);\
			for(inCell=0; inCell<numCellsInCellSet; inCell+=nodeFreq){\
				inNode=in_fld_node_connect_list[2*inCell];\
				memcpy(out_fld_coordinates+(outNode*NSpace),in_fld_coordinates+(inNode*NSpace),sizeof(float)*NSpace);\
				memcpy(out_fld_node_data  +(outNode*veclen),in_fld_node_data  +(inNode*veclen),sizeof(T    )*veclen);\
				outNode++;\
			}\
		}\
	}\
	if ( in_fld_node_data) ARRfree( in_fld_node_data);\
	if (out_fld_node_data) ARRfree(out_fld_node_data);\
}

int
IsoObjectLabel_IsoObjectLabelMods_IsoObjectLabelCore::update(OMevent_mask event_mask, int seq_num)
{
	// Active 
	if ((int)Active==0) return(1);

    // Parameters of IsoObjectLabelCore module
	int contourFreq=(int)ContourFrequency;
	int nodeFreq=(int)NodeFrequency;
	int lineIndep=(int)LineIndependent;
	int numLabels=(int)NumberOfLabels;

	// Parameters to be obtained from the in_field
	int NSpace;
	int nodeType;
	int Nnodes_in;
	float *in_fld_coordinates=NULL;
	int *in_fld_node_connect_list=NULL;

	// Parameters to setup out_field
	int veclen=1;
	int Nnodes_out=0; // to be calculated
	float *out_fld_coordinates=NULL;

	// Variables for calculating out_field parameters
	int numInCellSets;
	int inCellSet;
	int numCellsInCellSet;
	int inCell;
	int inNode;
	int outNode;

	// Count variables
	int i,j;

	// If in_field empty setup empty out_field and return
	Nnodes_in=(int)in_fld.nnodes;
#ifdef GFDEBUG
	printf("Nnodes_in: %d\n",Nnodes_in);
#endif
    if (Nnodes_in==0) {
		out.nnodes=0;
		out.ncell_sets=0;
		out.nnode_data=0;
		return(0);
	}
	if (((int)in_fld.nnode_data==0)) {
		out.nnodes=0;
		out.ncell_sets=0;
		out.nnode_data=0;
		return(0);
	}

	// Get in_field parameters
	NSpace = (int)in_fld.nspace;
	FLDget_node_data_type (in_fld.obj_id(),0,&nodeType);

	// Set out_field parameters (Mesh+Node_Data write)
	out.nspace = NSpace;
	out.ncell_sets=0;
	out.nnode_data=1;
	out.node_data[0].veclen=veclen;
	FLDset_node_data_type (out.obj_id(),0,nodeType);

	// Get coords and node_data[0] from in_field
	in_fld_coordinates = (float *)in_fld.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
	if(!in_fld_coordinates){
		ERRverror("IsoObjectLabel",ERR_ERROR,"could not get coordinates from in_field\n");
		return(0);
	}

	// Check if Isolines has only one node_data (scalar), float
	if (!((int)in_fld.nnode_data==1)) 
		ERRverror("IsoObjectLabel",ERR_WARNING,"in_field has more than one node_data\n");
	if (!((int)in_fld.node_data[0].veclen==1))
		ERRverror("IsoObjectLabel",ERR_WARNING,"veclen of in_field node_data should be 1\n");

	// get number of nodes in out_field 
	if (lineIndep>0){
		if(numLabels<0)numLabels=0;
		Nnodes_out = numLabels;
	}
	else{
		if(contourFreq<=0)contourFreq=1;
		if(nodeFreq<=0)nodeFreq=1;
		numInCellSets = in_fld.ncell_sets;
		numInCellSets-=numInCellSets%contourFreq;
		for(inCellSet=0; inCellSet<numInCellSets; inCellSet+=contourFreq){
			numCellsInCellSet=(int)in_fld.cell_set[inCellSet].ncells;
			numCellsInCellSet-=numCellsInCellSet%nodeFreq;
			for(inCell=0; inCell<numCellsInCellSet; inCell+=nodeFreq)Nnodes_out++;
		}
	}

#ifdef GFDEBUG
	printf("NSpace: %d\n",NSpace);
	printf("Nnodes_out: %d\n",Nnodes_out);
	printf("NodeType: %d\n",nodeType);
#endif

	out.nnodes = (int)Nnodes_out;

	// User does not want any labels 
	if (out.nnodes==0)return(1);

#ifdef GFDEBUG
	printf("Start copy of input to output\n");
#endif

	// get pointer for out coordinates array
	out_fld_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
	if (!out_fld_coordinates){
		ERRverror("IsoObjectLabel",ERR_ERROR,"could not get coordinates pointer from out_field\n");
		return(0);
	}

	switch(nodeType){

	case DTYPE_BYTE:
		COPY_DATA(char);
		break;
	case DTYPE_SHORT:
		COPY_DATA(short);
		break;
	case DTYPE_INT:
		COPY_DATA(int);
		break;
	case DTYPE_FLOAT:
		COPY_DATA(float);
		break;
	case DTYPE_DOUBLE:
		COPY_DATA(double);
		break;
	}
	
	if ( in_fld_coordinates) ARRfree( in_fld_coordinates);
	if (out_fld_coordinates) ARRfree(out_fld_coordinates);

	// return 1 for success
	return(1);
}


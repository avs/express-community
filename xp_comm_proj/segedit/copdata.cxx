
#include "gen_copy.h"
#define COPY_DATA(T) \
{ \
	T * idata = (T *) input_node_data; \
	T * odata = (T *) output_node_data; \
	memcpy(odata,idata,input_data_size*sizeof(T));\
}

int
SegmentationEditor_SegmentationEditorMods_copy_node_data::update(OMevent_mask event_mask, int seq_num)
{
	// input (Node_Data read notify)
	// output (Node_Data write)
	output.nnodes=(int)input.nnodes;
	output.nnode_data=(int)input.nnode_data;
	
	int  data_comp;
	int  output_data_size, input_data_size, input_data_type;
	char *input_node_data,*output_node_data;
	for (data_comp = 0; data_comp < (int)input.nnode_data; data_comp++) { 
		output.node_data[data_comp].veclen=(int)input.node_data[data_comp].veclen;
		// input.node_data[input_data_comp].values (char [])
		input_node_data = (char *)input.node_data[data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &input_data_size, &input_data_type);				
		output_node_data = (char*)output.node_data[data_comp].values.ret_typed_array_ptr(OM_GET_ARRAY_WR, input_data_type, &output_data_size);
		switch (input_data_type)
		{
			case OM_TYPE_BYTE:
				COPY_DATA(unsigned char);
			break;
			case OM_TYPE_CHAR:
				COPY_DATA(char);
			break;
			case OM_TYPE_SHORT:
				COPY_DATA(short);
			break;
			case OM_TYPE_INT:
				COPY_DATA(int);
			break;
			case OM_TYPE_FLOAT:
				COPY_DATA(float);
			break;
			case OM_TYPE_DOUBLE:
				COPY_DATA(double);
		};

		if (input_node_data)
			ARRfree(input_node_data);
		if (output_node_data)
			ARRfree(output_node_data);
	}

	/***********************/
	/* Function's Body     */
	/***********************/
//	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor_SegmentationEditorMods_demux::update\n");

	// return 1 for success
	return(1);
}

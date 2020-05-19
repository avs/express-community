
flibrary ExtractNodeDataMods<build_dir="xp_comm_proj/extrnode",out_hdr_file="gen.h"> 
{

	group+OPort ExtractNodeDataParams{
		int node_index[];
		int nnode_data;
		int offset;
		int dims[];
		int index;
		float out;
	};

	module Extract_Node_Data<src_file="extrnode.c"> {

		Mesh_Unif+Node_Data+IPort2 &input_data;
		ExtractNodeDataParams+IPort2 &params;
		int+IPort2 node_index[] => params.node_index;
		int+IPort2 offset => params.offset;
		omethod+notify_inst+req Update(
			.node_index+read+notify+req,
			.offset+read+notify+req,
			.input_data+read+notify+req,
			.output+write
		) = "ExtractNodeData";

		int num_curves => array_size(node_index);
		int num_points => input_data.nnode_data;

		float+OPort2 output[num_curves][num_points];
	};
};

flibrary DownSampleMods <build_dir="xp_comm_proj/resample",
                         cxx_name="",
                         out_hdr_file="gen.h",
                         out_src_file="gen.cxx"> {

	group DownSampleParams<NEportLevels={1,1}> {
		int &output_size<NEportLevels={2,1}>[3];
		int ndim<NEportLevels=1>;
		int dims<NEportLevels=1>[];
	};
						 
	module downsample <src_file="downsample.cxx",cxx_hdr_files="fld/Xfld.h"> {
		Mesh_Unif+Node_Data &in_field<NEportLevels={2,0}>;
		DownSampleParams+IPort2 &params<NEportLevels={2,0}>;
		int output_mesh_size<export=2>[3] => .params.output_size;
		Mesh_Unif+Node_Data out_field<NEportLevels={0,2}>;
		cxxmethod+req resample(
			in_field+read+notify+req,
			output_mesh_size+read+notify+req,
			out_field+write,done+write
		);
	};

};

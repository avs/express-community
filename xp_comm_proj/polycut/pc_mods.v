flibrary CutPolylinesMods <build_dir="xp_comm_proj/polycut",
						   process="express",
						   out_hdr_file="gen.h",
						   out_src_file="gen.cxx",
						   cxx_hdr_files="fld/Xfld.h",
						   libdeps="FLD",
						   cxx_name=""> {

	module CutPolylinesCore <src_file="polycut.cxx">{
		float level<NEportLevels={2,0}>;
		Node_Data &in_dists<NEportLevels={2,0}>;
		Mesh &in_poly<NEportLevels={2,0}>;
		Mesh out <NEportLevels={0,2}> {
			ncell_sets = 1;
			Cell_Set+Polyline cell_set[ncell_sets]{
				ncells => <-.nnodes;
			};
		};
		cxxmethod+notify_inst+req update (
			level+read+notify+req,
			in_dists+read+notify+req,
			in_poly+read+notify+req,
			out+write
		);
	 };


	 module CutPolylinesTest <src_file="polytest.cxx">{
		int value<NEportLevels={2,0}>;
		ilink in_poly<NEportLevels={2,0}>;
		ilink in_field<NEportLevels={2,0}>;
		cxxmethod+notify_inst+req update (
			value+read+notify+req,
			in_poly+read+notify+req,
			in_field+read+notify+req,
			out+write 
		);
		olink out;
	 };

};


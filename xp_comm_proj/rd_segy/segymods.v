
flibrary ReadSegyMods <build_dir = "xp_comm_proj/rd_segy",
						cxx_src_files = "segy_debug.cxx",
						cxx_hdr_files="fld/Xfld.h",
							  out_src_file = "gen.cxx",
							  out_hdr_file = "gen.hxx">
{
	group+OPort ReadSegyParams {
		string+Port2 filename;
		int+Port2 timescale;
		int+Port2 normalize;
	};

	module ReadSegyCore<src_file="rd_segy.cxx"> {
		string+IPort2 FileName;
		int+IPort2 normalize;
		int+IPort2 timescale;
		cxxmethod+req update (
			FileName+read+notify+req,
			normalize+read+notify+req,
			timescale+read+notify+req,
			out+write	
		);
		Mesh_Unif+Node_Data+OPort2 out;
	};

};


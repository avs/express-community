flibrary FilterNodeMods <
				build_dir="xp_comm_proj/fnc",
				hdr_dirs="xp_comm_proj/fnc",
				out_hdr_file="gen.h",
				out_src_file="gen.cxx",
				cxx_hdr_files="fld/Xfld.h",			
				libdeps="FLD"
					>
{
	module FilterNodeConnectCore <src_file="fnc.cxx"> {
		Field &in<NEportLevels={2,0}>;
		int active<NEportLevels={2,0}>;
		Field out<NEportLevels={0,2}>;
		cxxmethod+notify_inst+req update (
			in+read+notify+req,
			active+read+notify+req,
			out+write+nosave
		);
	};

};

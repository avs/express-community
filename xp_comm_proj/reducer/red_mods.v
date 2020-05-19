flibrary ReducerMods <build_dir="xp_comm_proj/reducer",
		  			 out_hdr_file="gen.h",
		  			 out_src_file="gen.cxx"> 
{
	group+OPort ReducerParams 
	{
		int+Port2 factor;
	};
	
	module ReducerCore<src_file="reducer.cxx">
	{
		float+IPort2 in[];
		ReducerParams+IPort2 &params;
		cxxmethod+notify_inst+req downsizer(
			factor+read+notify+req,
			in+read+notify+req,
			nout+read+write,
			out+read+write);
		int factor => params.factor;
		int nout;
		float+OPort2 out[.nout];
	};
};

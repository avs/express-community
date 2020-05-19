
flibrary ReadVis5DMods<
	cxx_name="",
	build_dir="xp_comm_proj/rd_vis5d">
{
	group+OPort ReadVis5DParams
	{
		string+Port2 filename;
		int+Port2 load_all_steps;
		int+Port2 time_step;
		int+Port2 show_title; 	
		string+Port2 time_steps[];
		string+Port2 var_names[];
		string+Port2 var_info[];
		string+Port2 status;
		int+Port2 wind_components[3];
	};

	module ReadVis5DCore <
		src_file="RdVis5D.cxx",
		out_hdr_file="RdV5Dgen.h",
		out_src_file="RdV5Dgen.cxx",
		c_src_files="binio.c",
		cxx_src_files="v5d.cxx",
		cxx_hdr_files="fld/Xfld.h RdVis5D.h",
		cxx_members= <"
			private:
				char* compute_grid(ReaderStatus *s, float *zscale);
				int   read_all_steps(ReaderStatus *s);
				int   get_one_step(ReaderStatus *s, int time);
		">
	> {
		cxxmethod+notify_inst+req ReadVis5Dfile<weight=1,status=1>(
			.filename+read+notify+req,
			.load_all_steps+read+notify,
			.fld+write,
			.vars2d+write,
			.time_steps+write,
			.var_names+write,
			.var_info+write,
			.time_step+write,
			.status+write,
			.wind_components+write,
			.local_ptr+read+write
		);

		cxxmethod+req SelectTimeStep<weight=2,status=1>(
			.time_step+read+notify+req,
			.wind_components+read+notify,
			.fld+write,
			.vars2d+write,
			.status+write,
			.local_ptr+read
		);

		cxxmethod+notify_deinst OnDeinstance(
			.local_ptr+read+write
		);

		string+IPort2 filename;
		boolean+IPort2 load_all_steps;
		int+IPort2 time_step;
		string+OPort2 time_steps[];
		string+OPort2 var_names[];
		string+OPort2 var_info[];
		string+OPort2 status;
		Field_Struct+Float+OPort2 fld;
		Field_Unif+Float+OPort2 vars2d;
		int+OPort2 wind_components[3];
		ptr+nosave local_ptr<NEvisible=0> = 0;
	};

	group+OPort ReadVis5DTopographyParams
	{
		string+Port2 topofile;
		int+Port2 subsampling;
		int+Port2 show_topo;
		float+Port2 min_height; 
	};

	module ReadTopoCore<
		src_file="ReadTopo.cxx",
		out_hdr_file="RdTopgen.h",
		out_src_file="RdTopgen.cxx",
		c_src_files="binio.c",
		cxx_src_files="v5d.cxx",
		cxx_hdr_files="fld/Xfld.h"
	> {
		cxxmethod+notify_inst+req ComputeTopography<weight=2,status=1>(
			.filename+read+notify+req,
			.topofile+notify,
			.subsampling+read+notify,
			.min_height+read+notify,
			.topo_params+read,
			.topo_size+read,
			.topo_grid+read,
			.fld+write
		);

		cxxmethod+notify_inst+req OnInstance<weight=1>(
			.topofile+read+notify,
			.topo_params+write,
			.topo_size+write,
			.topo_grid+write
		);

		string+IPort2 filename;
		string+IPort2 topofile;
		int+IPort2 subsampling;
		float+IPort2 min_height;
		Field_Struct+Float+OPort2 fld;

		float		topo_params<NEvisible=0>[4];
		int  		topo_size<NEvisible=0>[2];
		short  		topo_grid<NEvisible=0>[prod(topo_size)];
	};
};

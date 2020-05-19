//avs@iavsc.org, www.iavsx.org

flibrary RegionGrowMods <build_dir="xp_comm_proj/region",
						 out_hdr_file="gen.h",
						 out_src_file="gen.cxx",
						 cxx_hdr_files="fld/Xfld.h">
{
	//define the params block group
	
	group+OPort RegionGrowParams 
	{
		int+Port2 use_diagonals;
		int+Port2 dims[];
		int+Port2 out_val;
		string+Port2 out_label;
		int+Port2 set_null_value;
		int+Port2 invert_region;
		int+Port2 and_precedence;
		int+Port2 combine_vec[];
		int+Port2 combine_and_or[];
		string+Port2 vec_comp[];
		int+Port2 incremental[];
		int+Port2 seed[];
		float+Port2 min_vec[];
		float+Port2 max_vec[];
		float+Port2 delta[];
		string+Port2 nd_labels[];
	};
	

	module RegionGrowCore <src_file="reg_grow.cxx", 
						   cxx_hdr_files="grow.h",
						   cxx_src_files="grow.cxx"> 
	{
		int+IPort2 dims[];
		Node_Data+IPort2 &in;
	    int+IPort2 seed[];
		int+IPort2 combine_and_or[];
	    double+IPort2 delta[];
	    int+IPort2 use_diagonals;
		int+IPort2 set_null_value;
		int+IPort2 and_precedence;
		int+IPort2 invert_region;
	    int+IPort2 out_val;
		string+IPort2 out_label;
		int incremental[];
		int trigger;
			
		cxxmethod+notify_inst+req update(
			.trigger+notify,
			.dims+read+notify+req,
			.in+read+notify+req,
			.seed+read+notify+req,
			.combine_and_or+read+notify+req,
			.delta+read+notify+req,
			.use_diagonals+read+notify+req,
			.incremental+read+notify+req,
			.out_val+read+notify+req,
			.out_label+read+notify+req,
			.set_null_value+read+notify+req,
			.and_precedence+read+notify+req,
			.invert_region+read+notify+req,
			.out+write
		);
			
	    Node_Data+OPort2 out; 
	};
	
};

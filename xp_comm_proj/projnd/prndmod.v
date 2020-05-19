
flibrary ProjectNodeDataMods <cxx_name="",build_dir="xp_comm_proj/projnd",
						 out_hdr_file="gen.h",
						 out_src_file="gen.cxx",
						 cxx_hdr_files="fld/Xfld.h",
						 libdeps="FLD">
{
	
	enum ProjectNodeDataMethods {
		choices = {
			"Maximum",
			"Depth",
			"Depth+Maximum",
			"Mean",
			"Sum",
			"Magnitude"
		};
	};

	group+OPort ProjectNodeDataParams {
		int+Port2 axis;
		ProjectNodeDataMethods+Port2 method;
	};

	module ProjectNodeDataCore <src_file="projnd.cxx">{
		Mesh_Unif+Node_Data+IPort2 &in;
		int+IPort2 axis;
		ProjectNodeDataMethods+IPort2 method; 
		cxxmethod update (
			.in+read+notify+req,
			.axis+read+notify+req,
			.method+read+notify+req,
			.out+write		
		);
		Mesh_Unif+Node_Data+OPort2 out;
	};

};

flibrary RevolveMods <build_dir="xp_comm_proj/revolve", out_hdr_file="gen.h", out_src_file="gen.cxx"> 
{

	group+OPort RevolveParams 
	{
		int+Port2 resolution;
		boolean+Port2 smooth;
	};

	// module: revolve
	// purpose: surface of revolution module
	
	module revolve<src_file="revolve.cxx",cxx_hdr_files="fld/Xfld.h"> {
		float+IPort2 profile [][2];
		int+IPort2 resolution => .params.resolution;
		boolean+IPort2 smooth => .params.smooth;
		RevolveParams+IPort2 &params;

		cxxmethod+notify_inst+req generate(
			resolution+read+notify+req,
			smooth+read+notify+req,
			profile+read+notify+req,
			surface+write
		);

		Mesh+Quad+Node_Data+OPort2 surface<NEcolor2=65280> {
			node_data {
				float values[nvals][veclen];
				float+opt null_value;
				float min;
				float max;
				float min_vec[veclen];
				float max_vec[veclen];
			};
		};
	};
};



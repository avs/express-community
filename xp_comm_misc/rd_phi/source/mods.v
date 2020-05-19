library rp_mods <build_dir="source", process="express"> {

	module read_phoenics <f_src_files = "readp.f neplib.f"> {
		// fire module
		int trigger<NEportLevels={2,0}>;
		// default filenames
		string phi_name<NEportLevels={2,0}> = "phida";
		string xyz_name<NEportLevels={2,0}> = "xyzda";
		string pat_name<NEportLevels={2,0}> = "patgeo";
		// files ascii or direct access (default) ?
		int phi_bin<NEportLevels={2,0}> = 1;
		int xyz_bin<NEportLevels={2,0}> = 1;
		// name of problem (output)
		string title<NEportLevels={0,2}>;
		// output message
		string message<NEportLevels={0,2}>;
		// output field, all data is float
		Field out_fld<NEportLevels={0,2}> {
			node_data {
				veclen = 1;
				values+float;
				min+float;
				max+float;
				min_vec+float;
				max_vec+float;
				null_value+float;
			};
			cell_set {
				cell_data {
					veclen = 1;
					values+float;
					min+float;
					max+float;
					min_vec+float;
					max_vec+float;
					null_value+float;
				};
			};
		};	

		fmethod+req update(
			trigger+notify+req,
			phi_name+read+req,
			xyz_name+read,
			pat_name+read,
			phi_bin+read+req,
			xyz_bin+read+req,
			title+write,
			message+write,
			out_fld+write
		) = "readp_f";

	};


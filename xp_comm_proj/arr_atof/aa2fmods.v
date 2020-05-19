
flibrary ArrayAToFMods <build_dir = "xp_comm_proj/arr_atof",
                        cxx_name = "",
	 	        out_hdr_file = "gen.h",
		        out_src_file = "gen.cxx"> {

  // define the low-level module

  module ArrayAToF <src_file = "arr_atof.cxx"> {
    string+IPort2 strings_in[];
    float+OPort2 floats_out[];

    cxxmethod+req+notify_inst update(
      .strings_in  +notify+read+req,
      .floats_out  +write
    );
  };
};

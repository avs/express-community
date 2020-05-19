
flibrary MakeArrayMods <build_dir="xp_comm_proj/mk_array",
		        out_hdr_file="gen.h",
		        out_src_file="gen.cxx"> {

  // define the parameter block group

  // define the low-level module

  module MakeArray <src_file="mk_array.c"> {
    omethod+notify_inst update() = "mk_array";
  };
};


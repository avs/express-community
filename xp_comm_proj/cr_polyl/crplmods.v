
flibrary CreatePolyListMods <build_dir = "xp_comm_proj/cr_polyl",
		             out_hdr_file = "gen.h",
		             out_src_file = "gen.cxx"> {

  // define the low-level module

  module CreatePolyList <src_file = "cr_polyl.cxx"> {
    int+IPort2 id_array[];
    int+OPort2 num_polys;
    int+OPort2 poly_connect_list[]; 

    cxxmethod+notify_inst+req update(
      .id_array+read+notify+req,
      .num_polys+write,
      .poly_connect_list+write
    );
  };
};

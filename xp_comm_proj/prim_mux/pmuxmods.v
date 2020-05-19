
flibrary PrimitiveMUXMods <build_dir = "xp_comm_proj/prim_mux",
                           cxx_name = "",
                           out_hdr_file = "gen.h",
                           out_src_file = "gen.cxx"> {
    
  // define the low-level module
    
  module modPMUX <src_file = "prim_mux.cxx"> {
    group+IPort2 &input[];
    prim+OPort2 output <val_state=2>;
    int+OPort2 selectedItem <val_state=2>; 

    cxxmethod+req update (
      .input+read+notify+req,
      .output+write+nonotify,
      .selectedItem+write+nonotify 
    );
  };
};

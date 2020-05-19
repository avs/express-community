
flibrary RadioBoxMods <build_dir = "xp_comm_proj/radiobox",
		       out_hdr_file = "gen.h",
		       out_src_file = "gen.cxx"> {

  // define the low-level module

  module RadioBox <src_file = "radiobox.cxx"> {

    int+IPort2 active = 1;
    group+IPort2 &set[];
    int+OPort2 selectedItem = 0; 
    int bufferItem <NEvisible=0> = 0; 
    int+OPort2 previousItem = 0; 

    cxxmethod+notify_inst+req update (
      .active+read+req,
      .set+read+write+notify+req,
      .selectedItem+read+write+notify, 
      .bufferItem+read+write,
      .previousItem+write
    );
  };
};

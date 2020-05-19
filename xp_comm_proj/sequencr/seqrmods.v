
flibrary SequencerMods <build_dir = "xp_comm_proj/sequencr",
                       cxx_name = "",
		       out_hdr_file = "gen.h",
		       out_src_file = "gen.cxx"> {

  // define the low-level module

  module modSequencer <src_file = "sequencr.cxx"> {

    prim+IPort2 trigger;
    int+OPort2 seq_1;
    int+OPort2 seq_2;
    int+OPort2 seq_3;

    cxxmethod+req update (
      .trigger+notify,
      .seq_1+write,
      .seq_2+write,
      .seq_3+write
    );
  };
};


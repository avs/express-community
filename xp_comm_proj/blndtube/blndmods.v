
flibrary BlendyTubesMods <build_dir="xp_comm_proj/blndtube",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.hxx"> {

  // parameter block

  group+OPort BlendyTubesParams {
    float+Port2   scale;
    boolean+Port2 offsetScale;
    boolean+Port2 invertScale;
    int+Port2     facets;
  };



  // module

  module BlendyTubesCore <src_file="blndtube.cxx",
                          libdeps="FLD",
                          cxx_hdr_files="fld/Xfld.h">
  {
    Mesh+Node_Data+read+notify+req+IPort2 &lines_in;

    BlendyTubesParams+IPort2 &params;

    float+read+notify+req scale         => .params.scale;
    boolean+read+notify+req offsetScale => .params.offsetScale;
    boolean+read+notify+req invertScale => .params.invertScale;
    int+read+notify+req facets          => .params.facets;

    cxxmethod+notify_inst+req update;

    Mesh+Node_Data+write+OPort2 tubes_out;
  };


};


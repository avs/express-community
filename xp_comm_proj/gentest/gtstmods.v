
flibrary GenerateTestDataMods<build_dir="xp_comm_proj/gentest",
  		          out_hdr_file="gen.h",
		          out_src_file="gen.cxx"> {

  // define the parameter block group
  group+OPort GenerateTestDataParams {
    int+Port2 pattern;
    int+Port2 width;
    int+Port2 height;
    int+Port2 num_frames;
   };

  module Generate_Test_Data<src_file="gentest.cxx",
                   cxx_hdr_files="fld/Xfld.h"> {
    cxxmethod+notify_inst+req GenerateTestData(.pattern+read+notify+req,
                                          .width+read+notify+req,
                                          .height+read+notify+req,
                                          .num_frames+read+notify+req,
                                          .output+write
                                          );

    GenerateTestDataParams+IPort2 &Params;

    int pattern=>Params.pattern;
    int width=>Params.width;
    int height=>Params.height;
    int num_frames=>Params.num_frames;
    Mesh_Unif+Node_Data+Byte+OPort2 output;
  };
};

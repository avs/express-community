
flibrary ReadSLPSTLMods {

  // define the parameter block group

  group ReadSLPSTLParams <NEportLevels={0,1}> {
    string filename <NEportLevels=2>;
    int err <NEportLevels=2> = 0;
    string err_str <NEportLevels=2> = "";
  };


  // define the low-level module

  module ReadSLPSTLFile < build_dir="xp_comm_proj/rd_slpst",
                          src_file="rd_slp.cxx",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.h",
                    		  libdeps="FLD",
			                 cxx_hdr_files="fld/Xfld.h"> {

      ReadSLPSTLParams &ReadSLPSTLParams <NEportLevels={2,0}>;

      cxxmethod+req upd<status=1> (
         filename+read+notify+req,
         out+write,
         .err+write,
         .err_str+write
      );


      string filename => ReadSLPSTLParams.filename;
      int err => ReadSLPSTLParams.err;
      string err_str => ReadSLPSTLParams.err_str;

      Field out <export=2,NEportLevels={0,2}>;
  };

};

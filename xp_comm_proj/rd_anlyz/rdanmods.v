flibrary ReadAnalyzeMods <build_dir="xp_comm_proj/rd_anlyz",
                      process="express",
                      out_hdr_file="gen.h",
                      out_src_file="gen.cxx"> {
 
  // define ReadAnalyzeFile

     module ReadAnalyzeFile {
         string filename_in<NEportLevels={2,0}>;
         Mesh_Unif+Node_Data out<NEportLevels={1,2}>;
      
         omethod+notify_inst+req update<src_file="rd_anlyz.c">(
            filename_in+read+notify+req, out+write
         ) = "read_analyze_update";
      };

};


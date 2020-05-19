
flibrary WriteVGFMods {

   module WriteVGFCore< src_file="wr_vgf.cxx",
                        cxx_hdr_files="fld/Xfld.h",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.h",
                        build_dir="xp_comm_proj/wr_vgf">
   {
      cxxmethod+req upd<status=1>(
         filename+read+notify+req,
         in+read,
         top_name+read,
         nobjs+read,
         .err+write,
         .err_str+write);

      string filename<export=2,NEportLevels={2,0}>;
      Mesh+Node_Data &in<export=2,NEportLevels={2,0}>[];
      string top_name<export=2,NEportLevels={2,0}> = "default_avs_field";
      int nobjs<export=2,NEportLevels={2,0}>;

      int err<export=2,NEportLevels={0,2}> = 0;
      string err_str<export=2,NEportLevels={0,2}> = "";
   };

};


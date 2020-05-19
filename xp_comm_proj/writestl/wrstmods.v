
flibrary WriteSTLMods {

   module write_stl< build_dir="xp_comm_proj/writestl",
                     src_file="writestl.cxx",
                     out_hdr_file="gen.hxx",
                     out_src_file="gen.cxx",
                     libdeps="FLD",
                     cxx_hdr_files="fld/Xfld.h">
   {
      cxxmethod+req update<status=1>(
         filename+read+req,
         trigger+read+req+notify,
         in+read,
         .err+write,
         .err_str+write);

      string filename<export=2,NEportLevels={2,0}>;
      int trigger<export=2,NEportLevels={2,0}>;

      Mesh &in<export=2,NEportLevels={2,0}>;
      int err<export=2,NEportLevels={0,2}> = 0;
      string err_str<export=2,NEportLevels={0,2}> = "";
   };

};


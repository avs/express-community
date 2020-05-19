
flibrary DisplaySplitFilenameMods <build_dir="xp_comm_proj/dispfile",
                                   out_src_file="gen.cxx",
                                   out_hdr_file="gen.h">
{

   module DSFCore <src_file="dispfile.cxx"> {
      string+IPort2  FullFileName;

      cxxmethod update (
         .FullFileName+read+notify,
         .FileName+write,
         .Path+write,
         .ValidName+write
      );

      string+OPort2  FileName;
      string+OPort2  Path;
      boolean+OPort2 ValidName;
   };

};


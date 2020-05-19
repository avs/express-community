
flibrary GetFilesMods <build_dir="xp_comm_proj/getfiles",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.hxx"> {

   module GetFilesCore <src_file="getfiles.cxx"> {
      string+IPort2 dir<export=2>;
      string+IPort2 ext<export=2>;

      cxxmethod+req+notify_inst update (
         .dir+req+read+notify,
         .ext+req+read+notify,
         .nFiles+write,
         .files+write
      );

      int nFiles;
      string+OPort2 files<export=2>[.nFiles];
   };

};


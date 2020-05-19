
flibrary ReadDatamineMods <build_dir="xp_comm_proj/rd_dmine",
                           out_src_file="gen.cxx",
                           out_hdr_file="gen.hxx">
{

   group+OPort ReadDatamineParams {
      string+Port2  dir;
      string+Port2  filenames[];
      boolean+Port2 model_as_point;
      float+Port2   model_scale;
      int+Port2     err;
      string+Port2  err_str;
   };


   module ReadDatamineCore <src_file="rd_dmine.cxx",
                            cxx_src_files="dmfile.cxx",
                            cxx_hdr_files="fld/Xfld.h",
                            libdeps="FLD">
   {
      ReadDatamineParams+IPort2 &params;

      string  dir => params.dir;
      string  filenames[] => params.filenames;
      boolean model_as_point => params.model_as_point;
      float   model_scale => params.model_scale;

      cxxmethod+req+notify_inst update (
         .dir+read+notify+req,
         .filenames+read+notify+req,
         .model_as_point+read+notify+req,
         .model_scale+read+notify+req,
         .nFlds+write,
         .fld+write,
         .err+write,
         .err_str+write
      );

      int    err => params.err;
      string err_str => params.err_str;

      int nFlds;
      Field+OPort2 fld[nFlds];
   };

};


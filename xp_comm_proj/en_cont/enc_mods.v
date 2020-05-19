
flibrary EnhanceContrastMods <build_dir="xp_comm_proj/en_cont",
                              out_src_file="gen",
                              out_hdr_file="gen.hxx">
{

   group+OPort EnhanceContrastParams {
      float+Port2 level;
   };


   module EnhanceContrastCore <src_file="en_cont.cxx",
                               c_src_files="clahe.c"> {
      float+IPort2 level;
      int+IPort2   in_dims[];
      byte+IPort2  img[prod(in_dims)];

      cxxmethod+req Enhance (
         .level+read+notify+req,
         .in_dims+read+notify+req,
         .img+read+notify+req,
         .out+write
      );

      byte+OPort2  out[prod(in_dims)];
   };

};


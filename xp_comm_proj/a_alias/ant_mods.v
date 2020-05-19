
flibrary AntiAliasMods <build_dir="xp_comm_proj/a_alias",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.h">
{

   module AntiAliasCore <src_file="a_alias.cxx">
   {
      byte+IPort2 input[][4];
      int+IPort2  input_dims[2];

      cxxmethod+req update (
         input+read+notify+req,
         input_dims+read+notify+req,
         output+write,
         outx+write,
         outy+write
      );

      int+OPort  outx;
      int+OPort  outy;
      int+OPort2 output_dims[2] => {outx,outy};
      byte+OPort2 output[outx*outy][4];
   };

};


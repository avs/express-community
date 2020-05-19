
flibrary GammaCorrectMods <build_dir="xp_comm_proj/gamma",
                           out_src_file="gen.cxx",
                           out_hdr_file="gen.hxx">
{

   group+OPort GammaCorrectParams {
      float+Port2 gamma_value;
      int+Port2 method;
   };


   module GammaCorrectCore<src_file="gamma.cxx">
   {
      byte+IPort2 input[][4];

      GammaCorrectParams+IPort2 &params;

      float+IPort gamma_value => params.gamma_value;
      int+IPort   method => params.method;

      cxxmethod+req update(
         .input+read+notify+req,
         .gamma_value+read+notify+req,
         .method+read+notify+req,
         .out_size+write,
         .output+write
      );

      int out_size;
      byte+OPort2 output[out_size][4];
   };

};


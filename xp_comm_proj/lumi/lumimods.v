
flibrary LuminanceMods <build_dir="xp_comm_proj/lumi",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.hxx">
{

   module LuminanceCore <src_file="lumi.cxx">
   {
      byte+IPort2 input[][4];

      cxxmethod+req update (
         .input+read+notify+req,
         .out_size+write,
         .output+write
      );

      int out_size;
      byte+OPort2 output[out_size][1];
   };

};


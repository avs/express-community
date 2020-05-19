
flibrary ImageFilterMods <build_dir="xp_comm_proj/i_filter",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.hxx">
{

   group+OPort ImageFilterParams {
      int+Port2   filterOp;
      float+Port2 alphaConst;
      float+Port2 redConst;
      float+Port2 greenConst;
      float+Port2 blueConst;
   };


   module ImageFilterCore <src_file="i_filter.cxx">
   {
      byte+IPort2 input[][4];

      ImageFilterParams+IPort2 &params;

      cxxmethod+req update (
         .input+read+notify+req,
         .params.filterOp+read+notify+req,
         .params.alphaConst+read+notify+req,
         .params.redConst+read+notify+req,
         .params.greenConst+read+notify+req,
         .params.blueConst+read+notify+req,
         .out_size+write,
         .output+write
      );

      int out_size;
      byte+OPort2 output[out_size][4];
   };

};


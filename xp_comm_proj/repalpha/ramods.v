
flibrary ReplaceAlphaMods <build_dir="xp_comm_proj/repalpha",
                           out_src_file="gen.cxx",
                           out_hdr_file="gen.h">
{

   module ReplaceAlphaCore<src_file="repalpha.cxx"> {

      byte+IPort2 input_image<export=2>[][4];
      byte+IPort2 input_alpha<export=2>[];

      cxxmethod+req update(
         input_image+read+notify+req,
         input_alpha+read+notify+req,
         out_size+write,
         output+write
      );

      int out_size;
      byte+OPort2 output[out_size][4];
   };

};


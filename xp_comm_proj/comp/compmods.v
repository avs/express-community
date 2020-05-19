
flibrary CompositeMods <build_dir="xp_comm_proj/comp",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.h">
{


   module CompositeCore<src_file="comp.cxx"> {

      byte+IPort2 input_foreground<export=2>[][4];
      byte+IPort2 input_background<export=2>[][4];

      cxxmethod+req update(
         .input_foreground+read+notify+req,
         .input_background+read+notify+req,
         .out_size+write,
         .output+write
      );

      int out_size;
      byte+OPort2 output<NEportLevels={0,2}>[out_size][4];
   };


};


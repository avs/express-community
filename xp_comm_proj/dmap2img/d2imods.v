
flibrary Dmap2ImageMods <build_dir="xp_comm_proj/dmap2img",
                         out_src_file="gen.cxx",
                         out_hdr_file="gen.hxx">
{

   group+OPort Dmap2ImageParams {
      int+Port2 X_Resolution;
      int+Port2 Y_Resolution;
      boolean+Port2 Mode;
   };

   module Dmap2ImageCore <src_file="dmap2img.cxx"> {

      Dmap2ImageParams+IPort2 &params;
      DMAP.DatamapTempl+IPort2 &dmap;

      int X_Resolution<export=2> => params.X_Resolution;
      int Y_Resolution<export=2> => params.Y_Resolution;
      boolean Mode<export=2>     => params.Mode;

      cxxmethod+req+notify_inst update (
         X_Resolution+read+notify+req,
         Y_Resolution+read+notify+req,
         Mode+read+notify+req,
         dmap+read+notify,
         output+write,
         output_dims+write
      );

      int+OPort2  output_dims[2];
      byte+OPort2 output[(output_dims[0]*output_dims[1])][4];
   };

};


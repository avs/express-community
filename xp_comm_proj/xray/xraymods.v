
flibrary XrayMods <build_dir="xp_comm_proj/xray",
                   out_src_file="gen.cxx",
                   out_hdr_file="gen.h">
{

   group+OPort XrayParams {
      int+Port2 axis;
      int+Port2 mode;
   };


   module XrayCore<src_file="xray.cxx">
   {
      byte+IPort2 input[];
      int+IPort2  dims[3];

      XrayParams+IPort2 &XrayParams;

      int axis => XrayParams.axis;
      int mode => XrayParams.mode;

      cxxmethod+req x_ray_update (
         input+read+notify+req,
         dims+read+notify+req,
         axis+read+notify+req,
         mode+read+notify+req,
         nx+write,
         ny+write,
         output+write
      );

      int+OPort2 nx;
      int+OPort2 ny;
      int+OPort2 output[.nx][.ny];
   };

};


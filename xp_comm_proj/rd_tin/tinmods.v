
flibrary ReadTinMods <build_dir="xp_comm_proj/rd_tin",
                      out_hdr_file="gen.h",
                      out_src_file="gen.cxx">
{
   group+OPort ReadTinParams {
      string+Port2 filename;
      float+Port2 ZScale = 1;
      int+Port2 ZeroElevations = 0;
   };      
   
   module ReadTinCore <src_file="rd_tin.c"> {
      string+IPort2  filename;
      float+IPort2   ZScale;
      boolean+IPort2 ZeroElevations;

      omethod+notify_inst+req update (
         .filename+read+notify+req,
         .ZScale+read+notify+req,
         .ZeroElevations+read+notify+req,
         .Geometry+write
      ) = "ReadTinCore_Update";

      Mesh+OPort2 Geometry;
   };
};



flibrary ChromaDepthMods <build_dir="xp_comm_proj/chromad",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.hxx">
{

   module ChromaDepthCore <src_file="chromad.cxx">
   {
      byte+IPort2  framebuffer<export=2>[][4];
      float+IPort2 zbuffer<export=2>[][1];
      float+IPort2 mix<export=2> = 1.0;

      cxxmethod+req update (
         .framebuffer+read+notify+req,
         .zbuffer+read+notify+req,
         .mix+read+notify+req,
         .out_size+write,
         .output+write
      );

      int out_size = 0;
      byte+OPort2 output[out_size][4];
   };

   group+OPort buffer_struct {
      Mesh_Unif+Dim2+Space2+Node_Data+Vector4+nosave framebuffer;
      Mesh_Unif+Dim2+Space2+Node_Data+Scalar+nosave zbuffer;
   };

};


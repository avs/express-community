
flibrary ChromaDepthMacs <compile_subs=0> {

   macro chroma_depth_buffer {
      XP_COMM_PROJ.ChromaDepth.ChromaDepthMods.buffer_struct+IPort2 &in;

      Mesh_Unif+Dim2+Space2+Node_Data+Scalar+nosave+Port &zbuffer => in.zbuffer;
      Mesh_Unif+Dim2+Space2+Node_Data+Vector4+nosave+Port &image => in.framebuffer;

      int+Port &dims[] => image.dims;

      XP_COMM_PROJ.ChromaDepth.ChromaDepthMods.ChromaDepthCore ChromaDepthCore {
         &framebuffer => <-.image.node_data[0].values;
         &zbuffer => <-.zbuffer.node_data[0].values;
      };

      int+Port valid => (ChromaDepthCore.out_size > 0);
      int+Port new_dims[2] => {dims[0]*valid, dims[1]*valid};

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.new_dims;
         };
         data {
            in_data => <-.<-.ChromaDepthCore.output;
         };
      };

      olink chromadepth_image => .image_field_argb.out;
      olink chromadepth_obj   => .image_field_argb.obj;
   };


   GDM.OutputField chroma_depth {
      output_field {
         Output {
            buffers+OPort3;
            flip = 1;
            output+OPort3;
         };
         View {
            mode = "Automatic";
         };
         UImod_panel {
            title = "chroma depth";
         };
         UIslider {
            value = 800;
         };
         UIslider#1 {
            value = 800;
         };
         UIslider UIslider#2 {
            parent => <-.UImod_panel;
            title => "Luminance Mixing";
            y => <-.UIslider#1.y + <-.UIslider#1.height + 10;
            min = 0.;
            max = 1.;
            value+OPort3 = 1.;
            mode = "real";
         };
         UItoggle {
            y => <-.UIslider#2.y + <-.UIslider#2.height + 10;
         };
         DataObject {
            obj+OPort3;
         };
      };

      XP_COMM_PROJ.ChromaDepth.ChromaDepthMacs.chroma_depth_buffer chroma_depth_buffer {
         in => <-.output_field.Output.buffers;
         ChromaDepthCore {
            mix+IPort3 => <-.<-.output_field.UIslider#2.value;
         };
      };

      olink chromadepth_image => chroma_depth_buffer.chromadepth_image;
      olink chromadepth_obj   => chroma_depth_buffer.chromadepth_obj;
   };


};


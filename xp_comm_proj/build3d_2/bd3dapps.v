
flibrary Build3D_2_Apps<compile_subs=0> {

   APPS.SingleWindowApp Build3D_2_Eg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.orthoslice.out_obj,
                  <-.<-.<-.bounds.out_obj
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };

      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.build3d_2 build3d_2 {
         params {
            filename_head = "$XP_PATH<1>/xp_comm_proj/build3d_2/dense/dense";
            filename_tail = ".tif";
            min_length = 0;
            start = 0;
            end = 29;
            out_format = 5;
         };
      };

      MODS.downsize downsize {
         in_field => <-.build3d_2.out_fld;
         DownsizeParam {
            factor0 = 4.;
            factor1 = 4.;
            factor2 = 1.;
         };
      };
      MODS.scale scale {
         in_field => <-.downsize.out_fld;
         ScaleParam {
            scale_z = 5.;
         };
      };

      MODS.bounds bounds {
         in_field => <-.scale.out_fld;
      };

      MODS.orthoslice orthoslice {
         in_field => <-.scale.out_fld;
         OrthoSliceParam {
            axis = 2;
         };
      };
   };



   Build3D_2_Eg AdjustSliceSpacingEg {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.orthoslice.out_obj,
                  <-.<-.<-.Axis3D.out_obj
               };
            };
         };
      };

      
      build3d_2 {
         params {
            end = 5;
         };
      };

      float+OPort slice_position[] = {0, 5, 6, 15, 20, 22};

      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.adjust_slice_spacing adjust_slice_spacing {
         in_fld => <-.build3d_2.out_fld;
         slice_position => <-.slice_position;
      };

      downsize {
         in_field => <-.adjust_slice_spacing.out_fld;
      };
      -bounds;

      GEOMS.Axis3D Axis3D {
         in_field => <-.scale.out_fld;
         x_axis_param {
            end = 512.;
            step = 128.;
            ndig = 0;
            off_anno = 0.;
            axis_name = "";
         };
         y_axis_param {
            end = 512.;
            step = 128.;
            ndig = 0;
            off_anno => 0.;
            axis_name = "";
         };
         z_axis_param {
            end = 24.;
            step = 4.;
            minor_ticks = 0;
            ndig = 0;
            off_anno => 4.;
            axis_name = "Z";
         };
      };
   };



   APPS.SingleWindowApp NodeARGBDataEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.node_2_argb_data.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.build3d_2 build3d_2 {
         params {
            filename_head = "$XP_PATH<1>/xp_comm_proj/build3d_2/cube/cube";
            filename_tail = ".bmp";
            start = 0;
            end = 3;
            out_format = 0;
         };
      };

      MODS.bounds bounds {
         in_field => <-.build3d_2.out_fld;
      };

      MODS.orthoslice orthoslice {
         in_field => <-.build3d_2.out_fld;
         OrthoSliceParam {
            axis = 1;
            plane = 2;
         };
      };

      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.node_2_argb_data node_2_argb_data {
         in_fld => <-.orthoslice.out_fld;
      };
   };



   APPS.SingleWindowApp NodeColorDataEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.node_2_color_data_iso.out_obj,
                  <-.<-.<-.node_2_color_data_ortho.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.build3d_2 build3d_2 {
         params {
            filename_head = "$XP_PATH<1>/xp_comm_proj/build3d_2/cube/cube";
            filename_tail = ".bmp";
            start = 0;
            end = 3;
            out_format = 6;
         };
      };

      MODS.extract_scalar extract_scalar {
         in_field => <-.build3d_2.out_fld;
         ExtractScalarParam {
            component = 1;
         };
      };
      MODS.combine_comp combine_comp {
         in_field1 => <-.extract_scalar.out_fld;
         in_field2 => <-.build3d_2.out_fld;
      };


      MODS.orthoslice orthoslice {
         in_field => <-.build3d_2.out_fld;
         OrthoSliceParam {
            axis = 1;
            plane = 1;
         };
      };

      MODS.bounds bounds {
         in_field => <-.extract_scalar.out_fld;
      };

      MODS.isosurface isosurface {
         in_field => <-.combine_comp.out_fld;
         IsoParam {
            iso_level = 0.99;
            map_component = {1};
         };
      };

      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.node_2_color_data node_2_color_data_iso {
         in_fld => <-.isosurface.out_fld;
      };

      XP_COMM_PROJ.Build3D_2.Build3D_2_Macs.node_2_color_data node_2_color_data_ortho {
         in_fld => <-.orthoslice.out_fld;
      };

   };


};


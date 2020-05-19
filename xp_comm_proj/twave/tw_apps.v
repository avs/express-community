
flibrary TextureWaveApps <compile_subs=0> {

   APPS.SingleWindowApp TextureWaveEg1 {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.extract_scalar.out_obj
               };
            };
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/marble.x";
            };
         };
      };

      XP_COMM_PROJ.TextureWave.TextureWaveMacs.texture_wave texture_wave {
         in_image => <-.Read_Image.field;
      };
      MODS.extract_scalar extract_scalar {
         in_field => <-.texture_wave.out_image;
      };

   };



   XP_COMM_PROJ.TextureWave.TextureWaveApps.TextureWaveEg1 TextureWaveEg2 {
      Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.General_Graph.GraphViewport.obj
               };
               Xform {
                  mat = {
                     0.937019,0.,0.,0.,
                     0.,0.937019,0.,0.,
                     0.,0.,0.937019,0.,
                     0.,0.,0.,1.
                  };
               };
            };
         };
      };

      texture_wave {
         params {
            freq_value = 2.0;
            wave_mode = 2;
         };
      };
      MODS.orthoslice orthoslice {
         in_field => <-.extract_scalar.out_fld;
         OrthoSliceParam {
            plane => 200;
         };
      };
      FLD_MAP.extract_data_array extract_data_array {
         in => <-.orthoslice.out_fld;
      };

      AGHLM.General_Graph General_Graph {
         ValuesY1 => <-.extract_data_array.data;
         Graph#1 {
            Gparams {
               Graph_params1 {
                  graphtype = 4;
               };
            };
         };
         GraphWorld {
            World_params {
               X_min = 0.;
               X_max = 400.;
               Y_min = 0.;
               Y_max = 255.;
            };
         };
         GraphLegend {
            Legend_params {
               Legend_on_off = 0;
            };
         };
         XYAxis {
            Y_Axis_params {
               Steps_for_Major = 50;
               Steps_for_Minor = 1;
               Y_Axis_text = "Alpha Value";
            };
            X_Axis_params {
               Steps_for_Major = 100;
               Steps_for_Minor = 3;
               X_Axis_text = "Y Position";
            };
         };
      };

   };



   APPS.SingleWindowApp TextureWaveEg3 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.texture_wave_field.out_obj
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/helens2.fld";
         };
      };

      MODS.crop crop_fld {
         in_field => <-.Read_Field.field;
         CropParam {
            min = {29,112};
            max = {290,449};
         };
      };
      MODS.surf_plot surf_plot {
         in_field => <-.crop_fld.out_fld;
         SurfPlotParam {
            scale = 0.06;
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
            };
         };
      };
      MODS.crop crop_image {
         in_field => <-.Read_Image.field;
         CropParam {
            min = {67,159};
            max = {267,287};
         };
      };

      XP_COMM_PROJ.TextureWave.TextureWaveMacs.texture_wave_field texture_wave_field {
         in_fld => <-.surf_plot.out_fld;
         in_image => <-.crop_image.out_fld;
         texture_wave {
            params {
               wave_mode = 4;
               numsteps = 20;
            };
         };
         Scalar2Texture {
            data_obj {
               Texture {
                  filter = "Point";    // Fastest filtering. Use bilinear or trilinear for higher quality
               };
            };
         };
      };

   };



   APPS.SingleWindowApp TextureWaveEg4 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.texture_wave_field.out_obj
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/MicroAVS/demo_data/room/room.fld";
         };
      };
      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/MicroAVS/demo_data/room/room.geo";
         };
      };

      MODS.crop_box crop_box {
         in_field => <-.Read_Field.field;
      };

      GEOMS.FPlane FPlane {
         in => <-.crop_box.out_fld;
         dim1 = 10;
         dim2 = 15;
         plane {
            xform {
               xlate = {0.214426,0.228686,1.07295};
               mat = {
                  0.841965,0.,0.,0.,
                  0.,0.841965,0.,0.,
                  0.,0.,0.841965,0.,
                  0.,0.,0.,1.
               };
            };
         };
         probe_ui {
            probe_edit {
               GDxform_editor {
                  z_trans = 1.;
                  abs_z_trans = 1.;
               };
            };
         };
      };


      MODS.combine_vect combine_vect {
         in_field => <-.crop_box.out_fld;
      };
      MODS.streamlines streamlines {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane.out_fld;
         StreamParam {
            order = 3;
            nseg = 3;
            max_seg = 400;
            min_vel = 0.02;
            rib_width = 0.03;
            ribbons = 1;
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<1>/xp_comm_proj/twave/twave1.tif";
            };
         };
      };

      XP_COMM_PROJ.TextureWave.TextureWaveMacs.texture_wave_field texture_wave_field {
         in_fld => <-.streamlines.out_fld;
         in_image => <-.Read_Image.field;
         texture_wave {
            params {
               wave_mode = 1;
               numsteps = 20;
            };
         };
         Scalar2Texture {
            data_obj {
               Texture {
                  filter = "Point";    // Fastest filtering. Use bilinear or trilinear for higher quality
               };
            };
         };
      };

   };


};


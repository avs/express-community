
flibrary StreamTimeApps {

   // example app - normal usage

   APPS.SingleWindowApp StreamTimeEg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.stream_time.out_obj
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
         dim1 = 4;
         dim2 = 3;
         plane {
            xform {
               mat = {
                  0.841965,0.,0.,0.,
                  0.,0.841965,0.,0.,
                  0.,0.,0.841965,0.,
                  0.,0.,0.,1.
               };
               xlate = {0.214426,0.228686,1.07295};
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
            min_vel = 0.05;
            rib_width = 0.03;
         };
      };


      XP_COMM_PROJ.StreamTime.StreamTimeMacs.stream_time stream_time {
         in_fld => <-.streamlines.out_fld;
         StreamTimeParams {
            normalise = 1;
            radius_scale = 0.02;
         };
      };
   };



   APPS.SingleWindowApp StreamTimeEg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_edges.out_obj,
                  <-.<-.<-.stream_time.out_obj
               };
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
         };
      };

      MODS.combine_vect combine_vect {
         in_field => <-.Read_UCD.field;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      GEOMS.FPlane FPlane {
         in => <-.Read_UCD.field;
         dim1 = 10;
         dim2 = 14;
      };
      MODS.external_edges external_edges {
         in_field => <-.Read_UCD.field;
      };

      MODS.streamlines streamlines {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane.out_fld;
         StreamParam {
            min_vel = 0.;
         };
      };

      XP_COMM_PROJ.StreamTime.StreamTimeMacs.stream_time stream_time {
         in_fld => <-.streamlines.out_fld;
         StreamTimeParams {
            normalise = 1;
            radius_scale = 0.05;
         };
      };
   };



   StreamTimeEg1 StreamTimeEg3 {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.isoline.out_obj
               };
            };
         };
      };


      stream_time {
         StreamTimeParams {
            normalise = 1;
            radius_scale = 0.02;
            resolution = 8;
         };
      };

      MODS.Loop Loop {
         start => <-.stream_time.out_fld.node_data[0].min;
         end   => <-.stream_time.out_fld.node_data[0].max;
         incr = 30.;
         done = 1;
      };

      MODS.isoline isoline {
         in_field => <-.stream_time.out_fld;
         IsoParam {
            ncontours = 1;
            level_min<NEportLevels={3,2},export=3> => <-.<-.Loop.count;
            //level_max => 10404.33;
         };
      };
   };



   StreamTimeEg1 StreamTimeEg4 {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.threshold.out_obj
               };
            };
         };
      };


      stream_time {
         StreamTimeParams {
            normalise = 1;
            radius_scale = 0.02;
            resolution = 8;
         };
      };

      MODS.Loop Loop {
         start => <-.stream_time.out_fld.node_data[0].min;
         end   => <-.stream_time.out_fld.node_data[0].max;
         incr = 100.;
         done = 1;
      };

      MODS.threshold threshold {
         in_field => <-.stream_time.out_fld;
         ThreshParam {
            below = 0;
            above = 1;
            max_value<NEportLevels={3,2}> => <-.<-.Loop.count;
         };
      };
   };



   StreamTimeEg1 StreamTimeEg5 {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.texture_wave.out_obj
               };
            };
         };
      };


      stream_time {
         StreamTimeParams {
            normalise = 1;
            radius_scale = 0.04;
            resolution = 4;
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<1>/xp_comm_proj/twave/twave1.tif";
            };
         };
      };

      XP_COMM_PROJ.TextureWave.TextureWaveMacs.texture_wave_field texture_wave {
         in_image => <-.Read_Image.field;
         in_fld => <-.stream_time.out_fld;
         texture_wave {
            params {
               wave_mode = 2;
               contrast = 0.99;
               numsteps = 20;
               cycle = 0;
            };
         };
      };
   };


};


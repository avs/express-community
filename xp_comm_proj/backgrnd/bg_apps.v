
flibrary BackgroundApps <compile_subs=0> {

   APPS.SingleWindowApp BackgroundEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.background_generator.out_obj
               };
            };
            View {
               View {
                  back_col = {0.3, 0.5, 0.35};
               };
            };
         };
      };


      XP_COMM_PROJ.Background.BackgroundMacs.background_generator background_generator {
         params {
            Upper_Left_Hue = 0.67;
            Upper_Left_Sat = 1.0;
            Upper_Left_Value = 1.0;
            Upper_Right_Hue = 0.67;
            Upper_Right_Sat = 1.0;
            Upper_Right_Value = 1.0;
            Lower_Left_Hue = 0.0;
            Lower_Left_Sat = 1.0;
            Lower_Left_Value = 0.0;
            Lower_Right_Hue = 0.0;
            Lower_Right_Sat = 1.0;
            Lower_Right_Value = 0.0;
            X_Resolution = 128;
            Y_Resolution = 128;
            Dither = 0;
         };
      };

   };



   APPS.SingleWindowApp TexturedBackgroundEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.textured_background.out_obj
               };
            };
         };
      };


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };

      XP_COMM_PROJ.Background.BackgroundMacs.background_generator background_generator {
         params {
            Upper_Left_Hue = 0.63;
            Upper_Left_Sat = 1.;
            Upper_Left_Value = 1.;
            Upper_Right_Hue = 0.63;
            Upper_Right_Sat = 1.;
            Upper_Right_Value = 1.;
            Lower_Left_Hue = 0.63;
            Lower_Left_Sat = 0.3;
            Lower_Left_Value = 1.;
            Lower_Right_Hue = 0.63;
            Lower_Right_Sat = 0.3;
            Lower_Right_Value = 1.;
            X_Resolution = 128;
            Y_Resolution = 128;
            Dither = 0;
         };
      };

      XP_COMM_PROJ.Background.BackgroundMacs.textured_background textured_background {
         texture_in => <-.background_generator.out_fld;
      };

   };



   APPS.SingleWindowApp ColouredBackgroundEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom,
                  <-.<-.<-.coloured_background.out_obj
               };
            };
         };
      };

      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };

      XP_COMM_PROJ.Background.BackgroundMacs.coloured_background coloured_background;
   };


};


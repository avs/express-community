
flibrary ThermometerApps <compile_subs=0> {

   APPS.SingleWindowApp ThermometerEg {
      GDM.Uviewer Uviewer {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.isosurface.out_obj
               };
            };
            Top2D {
               child_objs => {
                  <-.<-.<-.Thermometer2D.out_obj,<-.<-.<-.Bargraph2D.out_obj
               };
            };
         };
      };

      MODS.Loop Loop {
         cycle = 2;
         start = 0.0;
         end = 1.0;
         incr = 0.05;
      };


      XP_COMM_PROJ.Thermometer.ThermometerMacs.Thermometer2D Thermometer2D {
         params {
            x_pos = 4.;
            y_pos = -4.;
            radius = 0.3;
            length = 3.5;
            background_col = {1.0, 1.0, 1.0};
            foreground_col = {1.0, 0.0, 0.0};
         };

         value => <-.Loop.count;
      };

      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.isosurface isosurface {
         in_field => <-.Read_Field.field;
         IsoParam {
            iso_level<NEportLevels={2,3}>;
         };
      };


      XP_COMM_PROJ.Thermometer.ThermometerMacs.Bargraph2D Bargraph2D {
         params {
            x_pos = -4.;
            y_pos = -4.;
            radius = 0.2;
            length = 3.5;
            background_col = {1.0, 1.0, 1.0};
            foreground_col = {0.0, 1.0, 0.0};
         };

         value => (<-.isosurface.IsoParam.iso_level / 255);
      };

   };

};


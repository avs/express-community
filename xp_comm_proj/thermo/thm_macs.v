
flibrary ThermometerMacs <compile_subs=0> {

   macro Thermometer2D {
      float value<NEportLevels={2,1}> = 0.0;

      XP_COMM_PROJ.Thermometer.ThermometerMods.ThermometerParams params {
         x_pos = 0.;
         y_pos = 0.;
         radius = 0.3;
         length = 4.;
         background_col = {1.0, 1.0, 1.0};
         foreground_col = {1.0, 0.0, 0.0};
      };

      XP_COMM_PROJ.Thermometer.ThermometerMods.ThermometerCircle Min_circle {
         radius => (<-.params.radius/3.0);
         x_pos  => <-.params.x_pos;
         y_pos  => <-.params.y_pos + <-.params.radius + <-.params.length;
         DataObject {
            Props {
               col => <-.<-.<-.params.background_col;
            };
            Obj {
               name => name_of(<-.<-.<-);
            };
         };
      };
      XP_COMM_PROJ.Thermometer.ThermometerMods.ThermometerCircle Max_circle {
         radius => <-.params.radius;
         x_pos  => <-.params.x_pos;
         y_pos  => <-.params.y_pos;
         DataObject {
            Props {
               col => <-.<-.<-.params.foreground_col;
            };
            Obj {
               name=> name_of(<-.<-.<-);
            };
         };
      };
      XP_COMM_PROJ.Thermometer.ThermometerMods.ThermometerRectangle Total_rectangle {
         x_pos  => <-.params.x_pos;
         y_pos  => <-.params.y_pos;
         height => <-.params.radius + <-.params.length;
         width  => <-.params.radius * (2.0/3.0);
         DataObject {
            Props {
               col => <-.<-.<-.params.background_col;
            };
            Obj {
               name=> name_of(<-.<-.<-);
            };
         };
      };
      XP_COMM_PROJ.Thermometer.ThermometerMods.ThermometerRectangle Partial_rectangle {
         x_pos  => <-.params.x_pos;
         y_pos  => <-.params.y_pos;
         height => <-.params.radius + (<-.params.length * <-.value);
         width  => <-.Total_rectangle.width;
         DataObject {
            Props {
               col => <-.<-.<-.params.foreground_col;
            };
            Obj {
               name=> name_of(<-.<-.<-);
            };
         };
      };

      GDM.GroupObject Thermo2D {
         child_objs => {
            <-.Min_circle.out_obj,
            <-.Max_circle.out_obj,
            <-.Total_rectangle.out_obj,
            <-.Partial_rectangle.out_obj
         };
         Top {
            name=> name_of(<-.<-.<-);
         };
      };
      olink out_obj => .Thermo2D.obj;
   };


   Thermometer2D Bargraph2D {
      params {
         radius = 0.2;
      };
      Min_circle {
         radius => <-.params.radius;
         y_pos =>  <-.params.y_pos + <-.params.length;
      };
      Total_rectangle {
         height => <-.params.length;
         width  => (<-.params.radius * 2);
      };
      Partial_rectangle {
         height => (<-.params.length * <-.value);
      };
   };

};


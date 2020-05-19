Applications.ModuleStack
   {

GEOMS.Block Block<NEx=198.,NEy=132.>;
GEOMS.Axis3D Axis3D<NEx=198.,NEy=198.> {
   in_field => <-.city_plot.out_fld;
   UIpanel<NEdisplayMode="open",NEx=286.,NEy=88.> {
      option {
         set = 1;
      };
      parent<NEdisplayMode="open"> => Applications.assist0.assist1.frames[1].br;
   };
   Axis3DUI {
      x_start_typein {
         x = 0;
      };
      x_end_typein {
         x = 0;
      };
      x_origin_typein {
         x = 0;
      };
      x_step_typein {
         x = 0;
      };
      x_offset_typein {
         x = 0;
      };
      y_start_typein {
         x = 0;
      };
      y_end_typein {
         x = 0;
      };
      y_origin_typein {
         x = 0;
      };
      y_step_typein {
         x = 0;
      };
      y_offset_typein {
         x = 0;
      };
      z_start_typein {
         x = 0;
      };
      z_end_typein {
         x = 0;
      };
      z_origin_typein {
         x = 0;
      };
      z_step_typein {
         x = 0;
      };
      z_offset_typein {
         x = 0;
      };
      UIradioBoxLabel_mode1 {
         label_cmd {
            cmd[];
         };
      };
      UIradioBoxLabel_mode2 {
         label_cmd {
            cmd[];
         };
      };
   };
};
   $link Viewer3D.objs_in Axis3D.out_obj
   };
!assist0.assist1.frames[1]{old=1;};


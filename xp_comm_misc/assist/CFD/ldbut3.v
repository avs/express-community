
Applications.ModuleStack{

MODS.Readers.Read_UCD begstream<NEx=429.,NEy=132.> {
   read_ucd_ui {
      file_browser<NEx=11.,NEy=55.> {
         filename = "$ASSIST_HOME/cfd/data/begstrea.inp";
      };

   };
};
MODS.Mappers.advector advector<NEx=451.,NEy=231.> {
   in_field => <-.Read_UCD.field;
   in_probe => <-.begstream.field;
   in_glyph => <-.Arrow2.out_fld;
   UIpanel.visible=>1;
   AdvectParam {
      count = 56.;
      normalize = 1.;
   };
   AdvectUI {
      norm_toggle<NEx=308.,NEy=253.> {
         set = 1;
      };
      end_slider<NEx=330.,NEy=209.> {
         max<NEdisplayMode="open"> = 10000.;
      };
      time_typein {
         x = 0;
         height = 40;
      };
   };
   AdvectUI {
      norm_toggle<NEx=308.,NEy=253.> {
         set<NEdisplayMode="open">;
      };
      UIpanel {
         width = 150;
      };
      UIradioBoxLabel_mode {
         UIradioBox {
            y = 616;
            parent => ModuleStackUI.mod_panel;
            x = 60;
         };
      };
      scale_slider {
         y = 10;
         parent => assist0.assist1.frames[2].br;
         x = 10;
         max=0.5;
         min=0.01;
         increment=0.01;
         decimalPoints=3;
         value=0.1;
      };
      start_slider {
         y = scale_slider.y;
         parent => assist0.assist1.frames[2].br;
         x = scale_slider.x+160;
      };
      incr_slider {
         y = scale_slider.y;
         parent => assist0.assist1.frames[2].br;
         x = scale_slider.x+320;
      };
      run_toggle {
         parent => assist0.assist1.frames[2].br;
         y = scale_slider.y+110;
         x = scale_slider.x;
      };
      reset_toggle {
         parent => assist0.assist1.frames[2].br;
         y = scale_slider.y+110;
         x = scale_slider.x+160;
      };
      cycle_toggle {
         parent => assist0.assist1.frames[2].br;
         y = scale_slider.y+110;
         x = scale_slider.x+320;
      };
      end_slider<NEx=330.,NEy=209.> {
         max<NEdisplayMode="open">;
         parent => assist0.assist1.frames[2].br;
         max=300;
         value=100;
         y = scale_slider.y+40;
         x = scale_slider.x+160;
      };
      time_typein {
         label {
            parent => assist0.assist1.frames[2].br;
            y = scale_slider.y+60;
            x = scale_slider.x+0;
         };
         field {
            parent => assist0.assist1.frames[2].br;
            x = scale_slider.x+60;
            y = scale_slider.y+60;
         };
      };


   };
  
};
GEOMS.Arrow2 Arrow2<NEx=484.,NEy=187.>;

$link Viewer3D.objs_in advector.out_obj
};
!assist0.assist1.frames[2]{old=1;};

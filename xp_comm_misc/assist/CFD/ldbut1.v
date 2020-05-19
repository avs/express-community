Applications.ModuleStack{
MODS.Mappers.streamlines streamlines<NEx=429.,NEy=132.> {
   in_field => <-.Read_UCD.field;
   in_probe => <-.begstream.field;
   StreamUI {
      UIpanel.visible=>1;
      vel_slider<NEx=22.,NEy=407.>;
      ribbon_width_slider{
         min = 0.;
         max = 0.05;
         value = 0.01;
         decimalPoints=4;
         increment<NEdisplayMode="open"> = 0.001;
         parent => assist0.assist1.frames[0].br;
         x = 10;
	 y = 10;

      };
      UIpanel<NEx=11.,NEy=143.> {
         width = 150;
      };

      nseg_slider {
         parent => assist0.assist1.frames[0].br;
         y = ribbon_width_slider.y;
         x = ribbon_width_slider.x+160;
      };
      max_seg_slider {
         y = ribbon_width_slider.y+60;
         parent => assist0.assist1.frames[0].br;
         x = ribbon_width_slider.x+160;
      };
      ribbon_toggle {
         y = ribbon_width_slider.y+15;
         parent => assist0.assist1.frames[0].br;
         x = ribbon_width_slider.x+320;
      };


   };
};
MODS.Readers.Read_UCD begstream<NEx=429.,NEy=77.> {
   read_ucd_ui {
      file_browser<NEdisplayMode="open",NEx=11.,NEy=55.> {
         filename<NEdisplayMode="open"> = "$ASSIST_HOME/cfd/data/begstrea.inp";
      };
   };

};
$link Viewer3D.objs_in streamlines.out_obj
};
!assist0.assist1.frames[0]{old=1;};


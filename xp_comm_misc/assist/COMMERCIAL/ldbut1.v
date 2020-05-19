Applications.ModuleStack{

$push
MODS.Mappers.city_plot city_plot<NEx=44.,NEy=165.> {
   UIpanel.visible=>1;
   in_field => <-.Read_Field.field;
   CityUI {
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         width = 150;
      };
      scale_slider {
        min=0.01;
        max=1.;
 	parent => assist0.assist1.frames[0].br;
         y = 20;
         x = 10;
      };
      scale_x_slider {
        min=0.01;
        max=1.;
 	parent => assist0.assist1.frames[0].br;
         y = scale_slider.y;
         x = scale_slider.x+160;
      };
      scale_y_slider {
        min=0.01;
        max=1.;
 	parent => assist0.assist1.frames[0].br;
         y = scale_slider.y+60;
         x = scale_slider.x+160;
      };
   };
};
$pop
$link Viewer3D.objs_in city_plot.out_obj
};
!assist0.assist1.frames[0]{old=1;};


Applications.ModuleStack{

MODS.Mappers.ribbons_plot ribbons_plot<NEx=352.,NEy=165.> {
   in_field => <-.Read_Field.field;
   SurfRibUI {
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         visible=1;
         width = 120;
      };
      UIradioBoxLabel_dir {
         UIradioBox {
            y = 24;
            parent => assist0.assist1.frames[2].br;
            x = 16;
         };
         UIlabel {
            y = 4;
            parent => assist0.assist1.frames[2].br;
            x = 4;
         };
      };
      UIradioBoxLabel_height {
         UIradioBox {
            y = 24;
            parent =>assist0.assist1.frames[2].br ;
            x = 152;
         };
         UIlabel {
            y = 4;
            parent => assist0.assist1.frames[2].br;
            x = 144;
         };
      };
      UIradioBoxLabel_map {
         UIradioBox {
            y = 24;
            parent => assist0.assist1.frames[2].br;
            x = 328;
         };
         UIlabel {
            y = 4;
            parent => assist0.assist1.frames[2].br;
            x = 276;
         };
      };
      scale_slider {
         parent => assist0.assist1.frames[2].br;
         increment=0.1;
         min=0;
         max=1.5;
         value=0.5;
         y = 68;
         x = 16;
      };
      offset_slider {
         parent => assist0.assist1.frames[2].br;
         y = 68;
         x = 284;
         min=0;
         max=10;
         value=3;
      };
   };
};
$link Viewer3D.objs_in ribbons_plot.out_obj
};
!assist0.assist1.frames[2]{old=1;};


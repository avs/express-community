Applications.ModuleStack
   {
   mapper_2D
      {
      MODS.Mappers.city_plot city_plot<NEx=200.,NEy=60.>
         {
         in_field => <-.in_field;
         out_obj<NEportLevels={1,3}>;
         CityUI
            {
            UIradioBoxLabel_glyph
               {
               UIradioBox 
                  {
                  visible=1;
                  parent => assist0.assist1.frames[1].br;
                  y =25;
                  x = 140;
                  };
               UIlabel
                  {
                  visible=1;
                  parent => assist0.assist1.frames[1].br;
                  y = 4;
                  x = 140;
                  
                  };
               };
            UIradioBoxLabel_map
               {
               UIradioBox 
                  {
                  visible=1;
                  parent => assist0.assist1.frames[1].br;
                  y =25;
                  x = 280;
                  };
               UIlabel
                  {
                  visible=1;
                  parent => assist0.assist1.frames[1].br;
                  y = 4;
                  x = 280;
                  };
               };
            scale_slider
               {
               parent => assist0.assist1.frames[1].br;
               x = 4;
               y = 4;
               width = 140;
               max => cache(10/<-.<-.in_field.node_data[<-.<-.CityParam.glyph_comp].max);
               };
            };
         };
     };
$push
$link Viewer3D.objs_in mapper_2D.city_plot.out_obj
$pop
   };
!assist0.assist1.frames[1]{old=1;};

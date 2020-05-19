Applications.ModuleStack
   {
   mapper_2D
      {
      MODS.Mappers.surf_plot surf_plot<NEx=50.,NEy=60.>
         {
         in_field => <-.in_field;
         out_obj<NEportLevels={1,3}>;
         SurfPlotUI
            {
            UIradioBoxLabel
               {
               UIradioBox 
                  {
                  visible=1;
                  parent => assist0.assist1.frames[2].br;
                  y =25;
                  x = 140;
                  };
               UIlabel
                  {
                  visible=1;
                  parent => assist0.assist1.frames[2].br;
                  y = 4;
                  x = 140;
                  };
               };
            scale_slider
               {
               parent => assist0.assist1.frames[2].br;
               x = 4;
               y = 4;
               width = 140;
               min=>0;
               max => cache(20/<-.<-.in_field.node_data[<-.param.component].max);
               };
            };
         };
     };
$push
$link Viewer3D.objs_in mapper_2D.surf_plot.out_obj
$pop
   };
!assist0.assist1.frames[2]{old=1;};


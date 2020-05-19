Applications.ModuleStack
   {
   mapper_2D
      {
      MODS.Mappers.isoline isoline<NEx=350.,NEy=60.> 
         {
         in_field => <-.in_field;
         out_obj<NEportLevels={1,3}>;
         IsoParam 
            {
            level_min => cache(in_field.node_data[contour_comp].min);
            level_max => cache(in_field.node_data[contour_comp].max);
            };
         IsoLineUI 
            {
            UIpanel.width = 136;
            UIradioBoxLabel 
                {
                x = 272;
                y=0;
                UIpanel.parent=>assist0.assist1.frames[0].br;
                visible=1;
                };
            nleveles 
               {
               parent => assist0.assist1.frames[0].br;
               y = 0;
               x = 0;
               };
            iso_level_min  
               {
               parent => assist0.assist1.frames[0].br;
               y = 0;
               x = 136;
               };
            iso_level_max   
               {
               parent => assist0.assist1.frames[0].br;
               x = 136;
               y = 75;
               };
            };
         };
      };

$push
$link Viewer3D.objs_in mapper_2D.isoline.out_obj
$pop
   };
!assist0.assist1.frames[0]{old=1;};

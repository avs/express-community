Applications.ModuleStack
   {
   mapper_3D
      {
      macro field_to_byte<NEx=50.,NEy=55.> 
         {
         link in_field<NEportLevels={2,1},NEx=11.,NEy=22.> => <-.in_field;
         byte byte<NEportLevels=1,NEx=11.,NEy=66.>[][] => 
           255 * (in_field.node_data[0].values - in_field.node_data[0].min) / 
                 (in_field.node_data[0].max - in_field.node_data[0].min);
         int dims<NEportLevels=1,NEx=11.,NEy=110.>[] => in_field.dims;
         FLD_MAP.Field_Mappers.uniform_scalar_field 
           uniform_scalar_field<NEx=11.,NEy=154.> 
             {
             mesh 
                {
                in_dims => <-.<-.dims;
                };
             data 
                {
                in_data => <-.<-.byte;
                };
             out<NEportLevels={0,2}>;
             };
          link out_field<NEportLevels={1,2}>=>uniform_scalar_field.out;
          };
      MODS.Mappers.volume_render volume_render<NEx=50.,NEy=100.> 
         {
         in_mesh => <-.field_to_byte.out_field;
         DataObject.obj<NEportLevels={1,4}>;
         alpha_frame 
            {
            parent => assist0.assist1.frames[2].br;
            y = 28;
            height = 90;
            x = 220;
            width = 128;
            };
         UIlabel.width=125;
         UIslider#1 
            {
            parent => assist0.assist1.frames[2].br;
	    x=0;
            y=0;
            };
         UIslider#2
            {
            parent => assist0.assist1.frames[2].br;
	    x=0;
            y=75;
            };
         };
     };

$push
$link Viewer3D.objs_in mapper_3D.volume_render.DataObject.obj
$pop
};
!assist0.assist1.frames[2]{old=1;};

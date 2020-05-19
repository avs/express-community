Applications.ModuleStack
   {
   mapper_3D
      {
      MODS.Mappers.orthoslice orthoslice<NEx=200.,NEy=60.>
         {
         in_field => <-.in_field;
         out_obj<NEportLevels={1,3}>;
         OrthoSliceUI 
            {
            axis_slider.parent => assist0.assist1.frames[1].br;
            plane_slider.parent => assist0.assist1.frames[1].br;
            };
         };
     };
$push
$link Viewer3D.objs_in mapper_3D.orthoslice.out_obj
$pop
   };
!assist0.assist1.frames[1]{old=1;};

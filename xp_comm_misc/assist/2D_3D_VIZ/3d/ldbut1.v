Applications.ModuleStack
   {
   mapper_3D
      {
      UIslider transp_slider<NEx=220.,NEy=11.> 
         {
         parent => assist0.assist1.frames[0].br;
         value = 0.1;
         x = 50;
         y = 55;
         min = 0.;
         max = 1.;
         title="isosurface opacity";
      };
      MODS.Mappers.isosurface isosurface<NEx=350.,NEy=60.> 
         {
         in_field => <-.in_field;
         out_obj<NEportLevels={1,3}>;
         IsoUI.UIiso_level
            {
            parent => assist0.assist1.frames[0].br;
            x=0;
            y=0;
            };
         obj.Props.trans => <-.<-.<-.transp_slider.value;
         };
      };

$push
$link Viewer3D.objs_in mapper_3D.isosurface.out_obj
$pop
   };
!assist0.assist1.frames[0]{old=1;};

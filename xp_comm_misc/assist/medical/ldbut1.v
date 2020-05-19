assist0.assist1
   {
   UIslider transp_slider<NEx=396.,NEy=220.> 
      {
      parent => frames[0].br;
      value = 0.1;
      x = 6;
      y = 60;
      min = 0.;
      max = 1.;
      width = 120;
      value=0.4;
      title="isosurface opacity";
      };
      !frames[0]{old=1;};
   };
Applications.ModuleStack
   {
   MODS.Mappers.isosurface isosurface<NEx=363.,NEy=132.> 
      {
      in_field => <-.Read_Field.field;
      IsoUI.UIiso_level
          {
          parent => assist0.assist1.frames[0].br;
          x=3;
          y=3;
          value=-700;
          title="isosurface value";
          };
      obj.Props.trans => assist0.assist1.transp_slider.value;
      };

$push
$link Viewer3D.objs_in isosurface.out_obj
$pop
   };

!assist0.assist1.frames[0]{old=1;};

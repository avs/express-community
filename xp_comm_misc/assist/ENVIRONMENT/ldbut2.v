assist0.assist1
   {
   UIslider transp_slider<NEx=396.,NEy=220.> 
      {
      parent => frames[1].br;
      value = 0.8;
      x = 20;
      y = 55;
      min = 0.;
      max = 1.;
      width=140;
      title="isosurface opacity";
      };
   };
Applications.ModuleStack
   {
   MODS.Mappers.isosurface isosurface<NEx=66.,NEy=154.> 
      {
      in_field => <-.Read_Field.field;
      IsoUI
          {
          UIiso_level
             {
             parent => assist0.assist1.frames[1].br;
             x=20;
             y=0;
             value=0.006;
             increment=0.001;
             decimalPoints=4;
             width=140;
             };

          UIoptionBoxLabel 
            {
            UIoptionBox 
              {
              y = 20;
              parent => assist0.assist1.frames[1].br;
              x = 320;
              visible=>1;
              };
            UIlabel  
              {
              parent => assist0.assist1.frames[1].br;
              x = 320;
              visible=>1;
              };
           };
         UIradioBoxLabel  
            {
            UIradioBox  
               {
               y = 20;
               parent => assist0.assist1.frames[1].br;
               x = 180;
              visible=>1;
               };
            UIlabel  
               {
               parent => assist0.assist1.frames[1].br;
               x = 180;
              visible=>1;
               };
            };

          };
      obj.Props.trans => assist0.assist1.transp_slider.value;
      };

$push
$link stdlink isosurface.out_obj
$pop
   };
!assist0.assist1.frames[1]{old=1;};

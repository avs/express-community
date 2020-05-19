Applications.ModuleStack{
MODS.Filters.crop crop<NEx=187.,NEy=132.> 
   {
   in_field => <-.Read_Field.field;
      panel {
         width = 120;visible=>1;
      };
   CropUI {
      min0_slider {
         parent => assist0.assist1.frames[2].br;
         value=15;
         x = 156;
      };
      max0_slider {
         parent => assist0.assist1.frames[2].br;
         value=25;
         y = 56;
         x = 156;
      };
      min1_slider {
         parent => assist0.assist1.frames[2].br;
         value=18;
         y = 0;
         x = 276;
      };
      max1_slider {
         parent => assist0.assist1.frames[2].br;
         value=28;
         y = 56;
         x = 276;
      };
      min2_slider {
         parent => assist0.assist1.frames[2].br;
         value=4;
         y = 0;
         x = 396;
      };
      max2_slider {
         parent => assist0.assist1.frames[2].br;
         value=14;
         y = 56;
         x = 396;
      };
   };
};
MODS.Mappers.isovolume cropped_iso<NEx=165.,NEy=264.> {
   UIpanel.visible=>1;
   in_field => <-.crop.out_fld;
   IsoVolUI {
      cut_level {
         parent => assist0.assist1.frames[2].br;
         value=400;
         y = 0;
         width = 152;
      };
      above_toggle {
         parent => assist0.assist1.frames[2].br;
         y = 72;
      };
   };
   obj.Modes.normals=2;
};
$push
$link Viewer3D.objs_in cropped_iso.out_obj
$pop
};
!assist0.assist1.frames[2]{old=1;};

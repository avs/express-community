Applications.ModuleStack
   {
   MODS.Mappers.tile_volume tile_volume<NEx=308.,NEy=44.> 
      {
      FieldIn => <-.Read_Field.field;
      UIslider 
         {
         parent => assist0.assist1.frames[4].br;
         width=150;
         };
      UIslider#1  
         {
         parent => assist0.assist1.frames[4].br;
         y = 68;
         value=6;
         max=10;
         };
      UIslider#2 
         {
         parent => assist0.assist1.frames[4].br;
         y = 68;
         x = 160;
         value=5;
         max=10;
         };
      UIslider#3 
         {
         parent => assist0.assist1.frames[4].br;
         y = 4;
         x = 160;
         value=1;
         max=5;
         };
      };
   Viewers.Viewer2D Viewer2D<NEx=77.,NEy=341.> 
      {
      objs_in => <-.tile_volume.DataObject.obj;
      ViewUI.Shell 
         {
         x = 652;
         y = 0;
         };
      };
   };
!assist0.assist1.frames[4]{old=1;};

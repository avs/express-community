ModuleStack
   {
   macro mapper_3D<NEx=50.,NEy=350.,NEwidth=500,NEheight=200> 
      {
      link in_field<NEportLevels={2,1},NEx=44.,NEy=11.> => <-.Read_Field.field;
      };
   };
assist0.assist1
   {
   my_local_dir=>my_dir+Applications.ModuleStack.field_dim_change.ndir;
   button_labels={"isosurface","slice","volumetry"};
   butlabels={"isosurface","slice","volumetry"};
   start={0,1,0};
   };


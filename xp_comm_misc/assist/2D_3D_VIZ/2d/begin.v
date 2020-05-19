ModuleStack
   {
   macro mapper_2D<NEx=66.,NEy=33.> 
      {
      link in_field<NEportLevels={2,1},NEx=44.,NEy=11.> => <-.Read_Field.field;
      };
   };
assist0.assist1
   {
   my_local_dir=>my_dir+Applications.ModuleStack.field_dim_change.ndir;
   button_labels={"isolines","city plot","surface plot"};
   butlabels={"isolines","city plot","surface plot"};
   start={1,0,0};
   };

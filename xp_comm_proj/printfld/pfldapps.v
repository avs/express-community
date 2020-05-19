XP_COMM_PROJ.PrintField.PrintFieldMacs.PrintFieldEg PrintFieldEg {
   Uviewer3D<NEx=627.,NEy=187.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice.out_obj};
            Xform {
               ocenter = {31.5,31.5,31.5};
               dcenter = {4.,4.,4.};
               mat = {
                  0.0660427,0.0152152,-0.0477044,0.,0.0124391,0.0714989,0.0400252,
0.,0.0485024,-0.0390544,0.0546911,0.,0.,0.,0.,1.
               };
               xlate = {-4.,-4.,-4.};
            };
         };
         View {
            View {
               renderer => "Software";
            };
         };
      };
   };
   Read_Field {
      read_field_ui {
         file_browser {
            x = 496;
            y = 290;
            width = 304;
            height = 388;
            dirMaskCache = "/usr/local/bin/express51/data/field/*";
         };
         filename = "/usr/local/bin/express51/data/field/hydrogen.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
   };
   print_field<NEx=242.,NEy=286.> {
      PrintFieldParams {
         filename = "/home/ts23/tst.txt";
         htmlout = 0;
      };
      PrintFieldUI {
         file_browser {
            x = 556;
            y = 304;
            width = 300;
            height = 388;
            ok = 1;
            dirMaskCache = "/home/ts23/*";
         };
      };
   };
   MODS.bounds bounds<NEx=539.,NEy=99.> {
      in_field => <-.Read_Field.field;
   };
   MODS.orthoslice orthoslice<NEx=396.,NEy=121.> {
      in_field => <-.Read_Field.field;
   };
};

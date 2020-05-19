
flibrary GetFilesApps<compile_subs=0> {

   APPS.ModuleStack GetFilesEg1 {

      XP_COMM_PROJ.GetFiles.GetFilesMacs.Multiple_File_Select Multiple_File_Select {
         UImod_panel {
            option {
               set = 1;
            };
         };
         MultiFileShell {
            dirText {
               text = "$XP_PATH<1>";
            };
            extText {
               text = "*";
            };
         };
      };

      string+Port selected_files[] => .Multiple_File_Select.filenames_with_dir;
      string+Port highlighted_file => .Multiple_File_Select.selected_filename;

   };


   APPS.SingleWindowApp GetFilesEg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom
               };
            };
         };
      };

      XP_COMM_PROJ.GetFiles.GetFilesMacs.Multiple_File_Select Multiple_File_Select {
         MultiFileShell {
            dirText {
#ifdef MSDOS
               text = "$XP_PATH<0>\\data\\geom";
#else
               text = "$XP_PATH<0>/data/geom";
#endif
            };
            extText {
               text = "geo*";
            };
         };
      };

      MODS.Read_Geom Read_Geom {
         ACread_geom {
            filename<NEportLevels={3,0}> => <-.<-.Multiple_File_Select.selected_filename;
         };
      };
   };


};


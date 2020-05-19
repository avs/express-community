flibrary SeededThreshold_Apps<compile_subs=0>
{
   APPS.SingleWindowApp SeededThresholdEg {
      MODS.Read_Field Read_Field {
         read_field_ui {
            file_browser {
               x = 556;
               y = 299;
               width = 300;
               height = 400;
               ok = 1;
               dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
            };
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };
      XP_COMM_PROJ.SeededThreshold.SeededThreshold_Macs.SeededThreshold SeededThreshold {
         in_fld => <-.Read_Field.field;
         params {
            min = 20.;
            max = 105.;
            seed_x = 32;
            seed_y = 31;
            seed_z = 27;
         };
      };
      MODS.isovolume isovolume {
         in_field => <-.SeededThreshold.out_fld;
      };
      MODS.cut_plane cut_plane {
         in_field => <-.isovolume.out_fld;
		 CutParam {
		    above = 0;
		 };
      };
	  MODS.bounds bounds {
      	 in_field => <-.Read_Field.field;
	  };
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.cut_plane.out_obj_cut,<-.<-.<-.bounds.out_obj};
            };
         };
      };
   };
};

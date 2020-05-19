
flibrary ReadDatamineApps<compile_subs=0> {

   APPS.SingleWindowApp ReadDatamineEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.extract_component.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };


      XP_COMM_PROJ.ReadDatamine.ReadDatamineMacs.read_datamine read_datamine {
	 params {
	    dir = "$XP_PATH<1>/xp_comm_proj/rd_dmine/data";
	    filenames[4] = {"contours.dm","points.dm","topopt.dm","topotr.dm"};
	 };
	 ReadDatamineUI {
	    UIfilenamesText {
	       text = "\"contours.dm\",  \"points.dm\",\"topopt.dm\",\"topotr.dm\"";
	    };
	 };
      };

      XP_COMM_PROJ.ReadDatamine.ReadDatamineMacs.extract_field extract_field {
         fldSlider {
            value = 3;
         };
         flds => <-.read_datamine.out_flds;
      };
      MODS.extract_component extract_component {
         in_field => <-.extract_field.out_fld;
         ExtrCompParam {
            component = 2;
         };
      };

   };


};


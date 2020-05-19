
flibrary WiggleApps <compile_subs=0>{

   APPS.SingleWindowApp WiggleEg<NEdisplayMode="maximized"> {
	  GDM.Uviewer2D Uviewer2D {
    	 Scene {
        	Top {
               child_objs => {
            	  <-.<-.<-.reset_xform.out_obj,<-.<-.<-.Wiggle.out_obj};
        	};
    	 };
	  };
	  MODS.Read_Field Read_Field{
    	 read_field_ui {
        	file_browser {
               ok = 1;
        	};
        	filename = "$XP_PATH<0>/data/field/lobster.fld";
    	 };
      };

	  MODS.orthoslice orthoslice {
    	 in_field => <-.Read_Field.field;
    	 OrthoSliceParam {
        	plane => 80;
    	 };
	  };
	  MODS.downsize downsize {
    	 in_field => <-.orthoslice.out_fld;
    	 DownsizeParam {
        	factor0 = 2.;
        	factor1 = 1.;
    	 };
	  };
	  XP_COMM_PROJ.Wiggle.WiggleMacs.Wiggle Wiggle {
    	 in_fld => <-.downsize.out_fld;
    	 params {
        	mode = 0;
        	scale = 0.05;
    	 };
	  };
      MODS.reset_xform reset_xform {
         in_field => <-.downsize.out_fld;
      };
   };
};

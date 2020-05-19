
flibrary ReadGeoTiffApps{
	APPS.SingleWindowApp ReadGeoTiff_Eg<NEdisplayMode="maximized"> {
	   UI {
		  shell {
			 x = 154;
			 y = 116;
		  };
	   };
	   GDM.Uviewer2D Uviewer2D {
		  Scene {
			 Top {
				child_objs => {
				   <-.<-.<-.mirror.out_obj};
			 };
		  };
		  Scene_Editor {
			 View_Editor {
				InfoPanel {
				   curobj_sel {
					  width = 238;
					  height = 245;
					  x = 0;
					  y = 0;
				   };
				};
			 };
		  };
	   };
	   XP_COMM_PROJ.ReadGeoTiff.ReadGeoTiffMacs.ReadGeoTiff ReadGeoTiff {
    	  ReadGeoTiffUI {
        	 FileDialog {
        		ok = 1;
        		dirMaskCache = "xp_comm_proj/ReadGeoTiff/*.tif";
        	 };
        	 RowColOption {
        		set = 1;
        	 };
    	  };
    	  ReadGeoTiffParams {
        	 Filename = "xp_comm_proj/ReadGeoTiff/bogota.tif";
    	  };
	   };
	   MODS.mirror mirror {
		  in_field => <-.ReadGeoTiff.out;
		  DVmirror {
			 DVcopy_xform_field {
				out_xform {
				   xform {
					  dcenter = {0.,0.,0.};
					  ocenter = {0.,0.,0.};
				   };
				};
			 };
		  };
	   };
	};
};

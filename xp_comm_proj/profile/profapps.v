flibrary ProfileApps {

	// ProfileEG_line
	// views the line object

	APPS.SingleWindowApp ProfileEg_line {
   		UI {
      		Modules {
         		IUI {
            		optionList {
               			selectedItem = 0;
            		};
         		};
      		};
   		};
   		GDM.Uviewer2D Uviewer2D {
      		Scene {
         		Top {
            		child_objs => {<-.<-.<-.Profile.line_obj};
            		Xform {
               			ocenter = {60.,99.5,0.};
               			dcenter = {2.41206,4.,0.};
	            	};
    	     	};
        	 	View {
            		View {
            	   		trigger = 1;
            		};
         		};
      		};
   		};
   		XP_COMM_PROJ.Profile.ProfileMacs.Profile Profile {
      		params {
         		x = 31;
	         	y = 7;
    	     	state = 3;
        	 	mode = 2;
      		};
   		};
	};

	// ProfileEg_map
	// views the map object

	APPS.SingleWindowApp ProfileEg_img {
   		UI {
      		Modules {
         		IUI {
            		optionList {
               			selectedItem = 0;
            		};
         		};
      		};
   		};
   		GDM.Uviewer2D Uviewer2D {
      		Scene {
         		Top {
            		child_objs => {<-.<-.<-.Profile.img_obj};
            		Xform {
               			ocenter = {99.5,99.5,0.};
               			dcenter = {4.,4.,0.};
            		};
         		};
         		View {
            		View {
               			trigger = 1;
            		};
         		};
      		};
   		};
   		XP_COMM_PROJ.Profile.ProfileMacs.Profile Profile {
      		params {
         		x = 31;
         		y = 7;
         		state = 3;
         		mode = 2;
      		};
   		};
	};
};

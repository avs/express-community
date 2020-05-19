
flibrary WritePsMacs <compile_subs=0> {

  macro WritePsUI {

	UImod_panel panel {
       title = "Write Postscript";
       message = "Select Write Postscript control panel.";
       parent<NEportLevels={4,0}>;
	};
	UIlabel PS_Filename {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "Write Postscript File Name";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
	};
	UItext file_name {
	    parent => panel;
	    y => PS_Filename.y + PS_Filename.height + 5;
	    text => <-.filename;
	    width = 170;
            showLastPosition = 1;
	};
	UIbutton visible {
	    parent => panel;
	    x => file_name.x + file_name.width + 5;
	    y => file_name.y;
	    width = 75;
	    height => <-.file_name.height;
	    label = "Browse...";
	};
	
	UIfileSB file_browser {
	    GMOD.copy_on_change copy_on_change {
               trigger => <-.<-.visible.do; 
               input => <-.<-.visible.do;
               output => <-.visible;
	    };
	    title = "Write Postscript Filename";
	    searchPattern = "/tmp/*";
	    filename => <-.filename;
	};

	string+OPort2 filename = "/tmp/avs-000.ps";
  };


  macro writePostscript {

     ilink image_in;

     XP_COMM_PROJ.WritePostscript.WritePsMods.WritePsCore WritePsCore {
        file_name => <-.WritePsUI.filename;
        image_in => <-.image_in;
     };

     WritePsUI WritePsUI;
  };


  APPS.SingleWindowApp WritePostscriptEg {
     MODS.Read_Geom Read_Geom {
        read_geom_ui {
           filename = "$XP_PATH<0>/data/geom/face-sm.geo";
        };
     };

     GDM.Uviewer3D Uviewer3D {
        Scene {
           Top {
              child_objs => {<-.<-.<-.Read_Geom.geom};
           };
        };
     };

     GDM.OutputField OutputField {
        view_in => <-.Uviewer3D.Scene_Selector.curr_view;
     };

     XP_COMM_PROJ.WritePostscript.WritePsMacs.writePostscript writePostscript {
        image_in => <-.OutputField.output_field.Output.output;
     };

  };

}; // end of library


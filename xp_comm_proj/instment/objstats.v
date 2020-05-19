/*
-* Synopsis           : Reports object counts to a log file on trigger
-*/

macro obj_stats_ui {

        XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

	UImod_panel panel {
            title => name_of(<-.<-.<-,1);
	    message = "Object Statistics control panel.";
	    parent<NEportLevels={4,0}>;
	};
	UIlabel TIME_Filename {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "object stats log File Name";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
	};
	UItext file_name {
	    parent => <-.panel;
	    y => TIME_Filename.y + TIME_Filename.height + 5;
	    text => <-.instr_params.timerFile;
	    width = 170;
            showLastPosition = 1;
	};
	UIbutton visible {
	    parent => <-.panel;
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
	    title = "Object Stats Log Filename";
	    searchPattern = "/tmp/*";
	    filename => <-.instr_params.timerFile;
	};


        UIbutton active {
            parent => <-.panel;
	    y => <-.visible.y + <-.visible.height + 20;
            width = 250;
            label = "Write Current Object Stats";
            do => <-.instr_params.timerOn;
        };

};

macro obj_stats_noui {

   string Message => name_of(<-,1); 

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.obj_stats obj_stats{
      filename+nres => <-.instr_params.timerFile;
      message => Message;
      Trigger => <-.instr_params.timerOn;
      depth = 2;
   };
};

macro object_stats {
      XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params instr_params;

      obj_stats_ui obj_stats_ui {
         instr_params => <-.instr_params;
      };
      obj_stats_noui obj_stats_noui {
         Message => name_of(<-.<-,1); 
         instr_params => <-.instr_params;
         obj_stats {
            depth = 3;
         };
      };
};


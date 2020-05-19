/*
-* Synopsis           : Starts a timer on trigger, prints elapsed time when
-*                      activity has ceased (system returns to idle state)
-*/

macro timeActivity_ui {

        XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

	UImod_panel panel {
            title => name_of(<-.<-.<-,1);
	    message = "Select timeActivity control panel.";
	    parent<NEportLevels={4,0}>;
	};
	UIlabel TIME_Filename {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "timeActivity log File Name";
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
	    title = "timeActivity Log Filename";
	    searchPattern = "/tmp/*";
	    filename => <-.instr_params.timerFile;
	};
        UItoggle active {
            parent => <-.panel;
	    y => <-.visible.y + <-.visible.height + 20;
            width = 170;
            label = "Enable Timer";
            set => <-.instr_params.timerOn;
        };


};

macro timeActivity_noui {
   prim Trigger<NEportLevels={2,1}>;

   string Message => name_of(<-,1);

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

   GMOD.loop timeLoop{
      start_val = 0;
      count = 0;
      end_val = 0.9;
      incr = 1;
   };
   XP_COMM_PROJ.Instrumentation.InstrumentationMods.usage usage{
      init<weight=0>;
      output<weight=0>;
      filename+nres => <-.instr_params.timerFile;
      message => Message;
   };
   
   GMOD.parse_v startTime{
      relative => <-;
      trigger => <-.Trigger;
      active+nres => <-.instr_params.timerOn;
      v_commands => "usage.startTrigger = 1;
                     timeLoop.reset = 1; timeLoop.run = 1;";
      on_inst = 0;
   };
   GMOD.parse_v endTime{
      relative => <-;
      trigger => timeLoop.count;
      active => (timeLoop.count > 0);
      v_commands => "usage.outTrigger = 1;";
      on_inst = 0;
   };
};


   macro time_activity {
      ilink Trigger;

      XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params instr_params;

      timeActivity_ui timeActivity_ui {
         instr_params => <-.instr_params;
      };
      timeActivity_noui timeActivity_noui {
         Trigger => <-.Trigger;
         Message => name_of(<-.<-,1);
         instr_params => <-.instr_params;
      };
   };


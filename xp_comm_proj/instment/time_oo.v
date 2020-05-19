/*
-* Synopsis           : Starts a timer on trigger, prints elapsed time when
-*                      a second trigger fires
-*/
macro time_on_off_ui {

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

	UImod_panel panel {
       title => name_of(<-.<-.<-,1);
	    message = "Select time on_off control panel.";
	    parent<NEportLevels={4,0}>;
	};
	UIlabel TIME_Filename {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "time on_off log File Name";
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
	    title = "time on_off Log Filename";
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


macro time_on_off_noui{
   ilink TriggerOn;
   ilink TriggerOff;
   string Message => name_of(<-,1);

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>; 

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.usage usage{
      init<weight=0>;
      output<weight=0>;
      filename+nres => <-.instr_params.timerFile;
      message => Message;
   };
   GMOD.parse_v startTime{
      relative => <-;
      trigger => TriggerOn;
      active+nres => <-.instr_params.timerOn;
      v_commands => "usage.startTrigger = 1;";      
      on_inst = 0;
   };
   GMOD.parse_v endTime{
      relative => <-;
      trigger => TriggerOff;
      active => <-.instr_params.timerOn;
      v_commands => "usage.outTrigger = 1;";
      on_inst = 0;
   };
};


macro time_on_off {
      ilink Trigger_On;
      ilink Trigger_Off;

      XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params instr_params;

      time_on_off_ui time_on_off_ui {
         instr_params => <-.instr_params;
      };
      time_on_off_noui time_on_off_noui {
         Message => name_of(<-.<-,1);
         TriggerOn => <-.Trigger_On;
         TriggerOff => <-.Trigger_Off;
         instr_params => <-.instr_params;
      };
};

/*
end
-*/


/*
-* Gated Verbose
-*
-* Synopsis           : Turns on Verbose Functions at the VCP on initial input trigger,
-*                    : then turns is off once
-*                      activity has ceased (system returns to idle state)
-*/

macro gated_verbose_ui {

        XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

	UImod_panel panel {
            title => name_of(<-.<-.<-,1);
	    message = "Select Gated Verbose control panel.";
	    parent<NEportLevels={4,0}>;
	};
	UIlabel GVlabel {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "Gated Verbose Functions";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
	};
        UItoggle active {
            parent => <-.panel;
	    y => <-.GVlabel.y + <-.GVlabel.height + 20;
            width = 250;
            label = "Enable Gated Verbose Trigger";
            set => <-.instr_params.timerOn;
        };


};

macro gated_verbose_noui {

   prim Trigger<NEportLevels={2,1}>;

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

   GMOD.loop timeLoop{
      start_val = 0;
      count = 0;
      end_val = 0.9;
      incr = 1;
   };

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.prvcp prvcp {
      Trigger =>  message;
      int index = 0;
      message => switch(index, "**** Gated Verbose BEGIN ****", "**** Gated Verbose END ****");
   };
   
   GMOD.parse_v startTime{
      relative => <-;
      trigger => <-.Trigger;
      active+nres => <-.instr_params.timerOn;
      v_commands => "prvcp.index=1; NetworkEditor.optionsMenu.verboseItem.option.set = 1; timeLoop.reset = 1; timeLoop.run = 1;";
      on_inst = 0;
   };
   GMOD.parse_v endTime{
      relative => <-;
      trigger => timeLoop.count;
      active => (timeLoop.count > 0);
      v_commands => "NetworkEditor.optionsMenu.verboseItem.option.set = 0;prvcp.index=2;";
      on_inst = 0;
   };
};

macro gated_verbose {
      ilink Trigger;

      XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params instr_params;

      gated_verbose_ui gated_verbose_ui {
         instr_params => <-.instr_params;
      };
      gated_verbose_noui gated_verbose_noui {
         Trigger => <-.Trigger;
         instr_params => <-.instr_params;
      };
};


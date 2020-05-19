/*
-* Gated Trace
-*
-* Synopsis           : Turns on ARR trace at the VCP on initial input trigger,
-*                    : then turns is off once
-*                      activity has ceased (system returns to idle state)
-*/

macro gated_trace_ui {

	XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params &instr_params<NEportLevels={2,1}>;

	UImod_panel panel {
	    title => name_of(<-.<-.<-,1);
	    message = "Select Gated Trace control panel.";
	    parent<NEportLevels={4,0}>;
	};
	UIlabel GTlabel {
	    parent => <-.panel;
	    y = 0;
	    width => parent.width;
	    alignment = 0;
            label = "Gated ARR Trace";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
	};
        UItoggle active {
            parent => <-.panel;
	    y => <-.GTlabel.y + <-.GTlabel.height + 20;
            width = 250;
            label = "Enable Gated ARR Trace Trigger";
            set => <-.instr_params.timerOn;
        };


};

macro gated_trace_noui {

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
      message => switch(index, "**** Gated Trace BEGIN ****","**** Gated Trace END ****");
   };

   XP_COMM_PROJ.Instrumentation.InstrumentationMods.gtrace gtrace {
      mode = 0;
   };
   
   GMOD.parse_v startTime{
      relative => <-;
      trigger => <-.Trigger;
      active+nres => <-.instr_params.timerOn;
      v_commands => "prvcp.index=1; gtrace.mode = 1;
                     timeLoop.reset = 1; timeLoop.run = 1;";
      
      on_inst = 0;
   };
   GMOD.parse_v endTime{
      relative => <-;
      trigger => timeLoop.count;
      active => (timeLoop.count > 0);
      v_commands => "gtrace.mode = 0;prvcp.index=2;";
      on_inst = 0;
   };
};


macro gated_trace {
      ilink Trigger;

      XP_COMM_PROJ.Instrumentation.InstrumentationMods.instr_params instr_params;

      gated_trace_ui gated_trace_ui {
         instr_params => <-.instr_params;
      };
      gated_trace_noui gated_trace_noui {
         Trigger => <-.Trigger;
         instr_params => <-.instr_params;
      };
};


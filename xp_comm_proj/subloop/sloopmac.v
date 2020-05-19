
flibrary SubLoopMacs {

	macro SubLoopUI {
		XP_COMM_PROJ.SubLoop.SubLoopMods.SubLoopParams+IPort2 &params;
		UImod_panel UIpanel{
			title=> name_of(<-.<-.<-,1);
		};
		UItoggle run_toggle {
			y = 0;
			width = 125;
			parent => <-.UIpanel;
			label => "Run";
			set<NEportLevels={2,0}> => <-.params.run;
		};
		UItoggle run_back_toggle {
			x = 125;
			y = 0;
			width = 125;
			parent => <-.UIpanel;
			label => "Run Backwards";
			set<NEportLevels={2,0}> => <-.params.run_back;
		};
		UItoggle step_toggle {
			y => ((<-.run_toggle.y + <-.run_toggle.height) + 4);
			width = 125;
			parent => <-.UIpanel;
			label => "Step";
			set<NEportLevels={2,0}> => <-.params.step;
		};
		UItoggle step_back_toggle {
			x = 125;
			y => ((<-.run_toggle.y + <-.run_toggle.height) + 4);
			width = 125;
			parent => <-.UIpanel;
			label => "Step Backwards";
			set<NEportLevels={2,0}> => <-.params.step_back;
		};
		UItoggle reset_toggle {
			y => ((<-.step_toggle.y + <-.step_toggle.height) + 4);
			width = 125;
			parent => <-.UIpanel;
			label => "Reset";
			set<NEportLevels={2,0}> => <-.params.reset;
		};
		UItoggle reset_back_toggle {
			x = 125;
			y => ((<-.step_toggle.y + <-.step_toggle.height) + 4);
			width = 125;
			parent => <-.UIpanel;
			label => "Reset Back";
			set<NEportLevels={2,0}> => <-.params.reset_back;
		};
		UIoption Once {
			set = 1;
		};
		UIoption Cycle;
		UIoption Bounce;
		UIoptionMenu cycle_toggle {
			x = 0;
			y => ((<-.reset_toggle.y + <-.reset_toggle.height) + 4);
			width => <-.UIpanel.width;
			parent => <-.UIpanel;

			label => "Cycle Options";
			cmdList => {<-.Once,<-.Cycle,
				<-.Bounce};
			selectedItem => <-.params.cycle;
		};
		UIfieldTypein start_typein {
			UIparent=> <-.UIpanel;
			flabel => "Start Value";
			fval<NEportLevels={2,0}> => <-.params.start;
			panel {
				width => <-.<-.UIpanel.width;
			};
			x = 0;
			y => ((<-.cycle_toggle.y + <-.cycle_toggle.height) + 4);
		};
		UIfieldTypein end_typein {
			UIparent=> <-.UIpanel;
			flabel => "End Value";
			fval<NEportLevels={2,0}> => <-.params.end;
			panel {
				width => <-.<-.UIpanel.width;
			};
			x = 0;
			y => ((<-.start_typein.y + <-.start_typein.height) + 4);
		};
		UIfieldTypein incr_typein {
			UIparent=> <-.UIpanel;
			flabel => "Increment";
			fval<NEportLevels={2,0}> => <-.params.incr;
			panel {
				width => <-.<-.UIpanel.width;
			};
			x = 0;
			y => ((<-.end_typein.y + <-.end_typein.height) + 4);
		};
		UIfieldTypein count_typein {
			UIparent=> <-.UIpanel;
			flabel => "Loop count";
			fval<NEportLevels={2,0}> => <-.params.count;
			panel {
				width => <-.<-.UIpanel.width;
			};
			x = 0;
			y => ((<-.incr_typein.y + <-.incr_typein.height) + 4);
		};
	};

	macro SubLoopFunc {
		XP_COMM_PROJ.SubLoop.SubLoopMods.SubLoopParams+IPort2 &params;
		ilink trigger;
		XP_COMM_PROJ.SubLoop.SubLoopMods.SubLoopCore SubLoopCore {
			trigger => <-.trigger;
			reset => <-.params.reset;
			reset_back => <-.params.reset_back;
			step => <-.params.step;
			step_back => <-.params.step_back;
			start => <-.params.start;
			end => <-.params.end;
			incr => <-.params.incr;
			run => <-.params.run;
			run_back => <-.params.run_back;
			count => <-.params.count;
			cycle => <-.params.cycle;
		};
		olink count => SubLoopCore.count;
		olink done => SubLoopCore.done;
	};
	
	macro SubLoop {
		ilink trigger;
		int+IPort2 run = 0;
		int+IPort2 run_back = 0;
		int+IPort2 step = 0;
		int+IPort2 step_back = 0;
		int+IPort2 reset;
		int+IPort2 reset_back = 0;
		int+IPort2 cycle = 0;
		float+IPort2 start = 0;
		float+IPort2 end = 0;
		float+IPort2 incr = 0;
		XP_COMM_PROJ.SubLoop.SubLoopMods.SubLoopParams params {
			run => <-.run;
			run_back => <-.run_back;
			step => <-.step;
			step_back => <-.step_back;
			reset => <-.reset;
			reset_back => <-.reset_back;
			cycle => <-.cycle;
			start => <-.start;
			end => <-.end;
			incr => <-.incr;
			count=0;
		};		
		XP_COMM_PROJ.SubLoop.SubLoopMacs.SubLoopUI SubLoopUI {
			params => <-.params;
		};
		XP_COMM_PROJ.SubLoop.SubLoopMacs.SubLoopFunc SubLoopFunc {
			params => <-.params;
			trigger => <-.trigger;
		};
		olink count => SubLoopFunc.count;		
		olink done => SubLoopFunc.done;
	};
};

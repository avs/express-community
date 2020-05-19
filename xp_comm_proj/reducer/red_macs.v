flibrary ReducerMacs 
{
	macro ReducerFunc {
		ilink in;
		XP_COMM_PROJ.Reducer.ReducerMods.ReducerParams+IPort2 &params;
		XP_COMM_PROJ.Reducer.ReducerMods.ReducerCore ReducerCore {
			in => {<-.in};
			params => <-.params;
		};
		olink out => .ReducerCore.out;
	};

	macro ReducerUI {
		XP_COMM_PROJ.Reducer.ReducerMods.ReducerParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIlabel Factor {
			parent => <-.UImod_panel;
			label => "Factor ";
			active = 1;
			width => parent.clientWidth/2;
		};
		UIfield UIfield {
			parent => <-.UImod_panel;
			x => <-.Factor.x + <-.Factor.width;
			y => <-.Factor.y;
			active = 1;
			value => <-.params.factor;
			mode = "integer";
		};
	};

	macro Reducer {
		ilink in;
		XP_COMM_PROJ.Reducer.ReducerMods.ReducerParams params;
		XP_COMM_PROJ.Reducer.ReducerMacs.ReducerFunc ReducerFunc {
			in => <-.in;
			params => <-.params;
		};
		XP_COMM_PROJ.Reducer.ReducerMacs.ReducerUI ReducerUI {
			params => <-.params;
		};
		olink out => .ReducerFunc.out;
	};

};

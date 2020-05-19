

flibrary DashDialApps <compile_subs=0> {
	//
	// dashboard array system
	//
	macro DashDialArrayEg {
		int+Port num_dashes = 10;
		int+Port spacing	  = 210;
		string+Port titles [num_dashes] => {
			"Temperature",
			"Engine Speed",
			"Fuel Level",
			"Vacume",
			"Efficiency",
			"Fuel Rate",
			"Load Factor",
			"Noise Level",
			"Stability",
			"Weight"
		};
		GMOD.loop loop {
			reset = 0;
			run	=> <-.UIoption.set;
			cycle	  = 1;
			start_val = 0.0;
			end_val = 100.0;
			incr = 0.1;
		};
		float+Port t => loop.count ;
		float+Port values[num_dashes] => {
			((t - 50) ),
			((150 - t)*2.0),
			(t ),
			(rand(4.0)-2.0),
			(sin(t) * 1.5),
			(sin(t * 9.0 + 2.0) * 1.5),
			(cos(t * 2.0 + 2.0) * 1.0),
			(cos(t  + 3.0) * 2.0),
			(cos(t * 3.0) * 1.0),
			(cos(t * 9.0) * 2.0)
		};
		// define array of dashboards
		//
		// position formula based on 4 per row
		//
		XP_COMM_PROJ.DashDial.DashDialMacs.DashDial dash_dial_array[ num_dashes ] {
			int	 index => index_of (dash_dial_array);
			step_value => <-.values[index];
			UIpanel {
				x => ( index & 3 ) * <-.<-.spacing;
				y => ( index / 4) * <-.<-.spacing;
			};
			title => <-.titles[index];
			parent => <-.UIshell;
		};
		UIshell UIshell {
			width = 1000;
			height = 700;
			title = "Dash-Board Statistics Summary";
			menu => <-.UIcmdList;
		};
		Commands.UIcmdList UIcmdList {
			cmdList => {<-.UIcmdList#1};
		};
		Commands.UIcmdList UIcmdList#1 {
			cmdList => {<-.UIoption};
			label = "Controls";
		};
		Commands.UIoption UIoption {
			label = "Run";
		};
		GMOD.copy_on_change copy_val {
			trigger => <-.UIoption.do;
			input = 10;
			output => <-.num_dashes;
		};
	};
};

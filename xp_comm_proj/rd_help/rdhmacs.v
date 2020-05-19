
library ReadHelpMacs { 
	
	//Macros

	// The following is the UI Macro

	macro ReadHelpUI{
		XP_COMM_PROJ.ReadHelp.ReadHelpMods.ReadHelpParams+IPort2 &params;

		UIshell Help_Dialog {
            width = 500;
            height = 300;
			visible=> <-.params.active;
			showStatusBar = 0;
		};

		UIscrolledWindow UIscrolledWindow {
			x = 0;
			y = 0;
			width => parent.clientWidth;
			height => (parent.clientHeight - 40);
			parent => <-.Help_Dialog;
		};

		UItext UItext {
			y = 0;
			parent => <-.UIscrolledWindow;
			text => <-.params.text;
			rows => <-.params.rows;
			columns => <-.params.cols;
			width => parent.clientWidth;
			height => parent.clientHeight;
			resizeToText = 1;
			multiLine = 1;
			outputOnly = 1;
		};

		UIbutton Dismiss {
			x => ((parent.clientWidth - .width) / 2);
			y => ((parent.clientHeight - .height) - 5);
			parent => <-.Help_Dialog;
			do => (!<-.params.active);
		};

	};

	// The following is the Functional Macro

	macro ReadHelpFunc {
		XP_COMM_PROJ.ReadHelpMods.ReadHelpParams+IPort2 &params;

		XP_COMM_PROJ.ReadHelpMods.ReadHelpCore ReadHelpCore {
			params => <-.params;
		}; 
		olink text => ReadHelpCore.text;
	};


	//The following is the User Macro 

	macro ReadHelp {
		ilink filename;
		int+IPort2 active;
		ilink width;

		XP_COMM_PROJ.ReadHelpParams params{
			filename => <-.filename;
			active => <-.active;
			width => <-.width;
		};

		XP_COMM_PROJ.ReadHelpUI ReadHelpUI{
			params => <-.params;
		};
		XP_COMM_PROJ.ReadHelpFunc ReadHelpFunc{
			params => <-.params;
		};
		olink text => .ReadHelpFunc.text;

	}; 
};   




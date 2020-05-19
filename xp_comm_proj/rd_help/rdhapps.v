
library ReadHelpApps{
	APPS.DefaultApplication ReadHelpEg {

		UIshell UIshell;
		
		UIfileDialog UIfileDialog{
			visible => <-.browse.do;
			filename = "DISCLAIMER";
			width = 300;
			height = 388;
			ok = 1;
			dirMaskCache = "*";
			cancel = 1;
		};

		UItext UItext{
			parent => <-.UIshell;
			width = 300;
			text => <-.UIfileDialog.filename;
		}; 

		UIbutton browse{ 
			parent => <-.UIshell;
			y = 38; 
			width = 300;
			height = 30;
			ReadHelpParams;
		};	

		UItoggle show_help_Dialog {
			parent => <-.UIshell;
			width = 300;
			do = 3;
			y = 72;
		};   

		UIfield width{
			parent => <-.UIshell; 
			value = 60.;
			mode = "integer";
			y = 98;
		};

		XP_COMM_PROJ.ReadHelpMacs.ReadHelp ReadHelp{
			filename => <-.UIfileDialog.filename;
			active => <-.show_help_Dialog.set;
			width => <-.width.value;
		};
	};
};


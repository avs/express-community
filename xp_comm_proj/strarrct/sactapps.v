
flibrary StringArrayConcatenateApps {
	APPS.MultiWindowApp StringArrayConcatenateEg<NEdisplayMode="maximized"> {
		string+Port in_strings<NEwidth=400.,NEdisplayMode="open">[] = {"a","b","c"};
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMacs.StringArrayConcatenate StringArrayConcatenate {
			in_strings => <-.in_strings;
			params {
				leader = "test = ";
				separator = " + ";
				trailer = ";";
			};
		};
		string+Port string<NEwidth=400.,NEdisplayMode="open"> => .StringArrayConcatenate.out_string;
	};
};




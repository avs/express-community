
flibrary StringArrayConcatenateMacs {

	// UI Macros
	macro StringArrayConcatenateUI {
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMods.StringArrayConcatenateParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);			
		};
		UIlabel Leader_String {
			parent => <-.UImod_panel;
			y = 0;
			width => parent.clientWidth;
		};
		UItext Leader {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			y => <-.Leader_String.y + <-.Leader_String.height+10;
			text => <-.params.leader;
			updateMode = -1;
		};
		UIlabel Separator_String {
			parent => <-.UImod_panel;
			y => <-.Leader.y + <-.Leader.height+10;
			width => parent.clientWidth;
		};
		UItext Separator {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			y => <-.Separator_String.y + <-.Separator_String.height+10;
			text => <-.params.separator;
			updateMode = -1;
		};
		UIlabel Trailer_String {
			parent => <-.UImod_panel;
			y => <-.Separator.y + <-.Separator.height+10;
			width => parent.clientWidth;
		};
		UItext Trailer {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			y => <-.Trailer_String.y + <-.Trailer_String.height+10;
			text => <-.params.trailer;
			updateMode = -1;
		};
		UIbutton Generate_String {
			parent => <-.UImod_panel;
			width => parent.clientWidth;
			y => <-.Trailer.y + <-.Trailer.height+10;
			do => <-.params.trigger;		
		};
	};

	// Functional macro
  
	macro StringArrayConcatenateFunc<NEx=275.,NEy=143.> {
		ilink in_strings; 
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMods.StringArrayConcatenateParams+IPort2 &params;
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMods.StringArrayConcatenateCore StringArrayConcatenateCore
		{
			trigger => <-.params.trigger;
			input_strings => <-.in_strings;
			leader_string => <-.params.leader;
			separator_string => <-.params.separator;
			trailer_string => <-.params.trailer;		
		};
		olink out_string => StringArrayConcatenateCore.output_string;
		olink done => StringArrayConcatenateCore.done;		
	};

	// User Macros (Functional and UI)
	macro StringArrayConcatenate {
		ilink in_strings; 
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMods.StringArrayConcatenateParams params
		{
			leader = "";
			separator = "";
			trailer = "";
			trigger = 0;
		};
		
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMacs.StringArrayConcatenateUI StringArrayConcatenateUI
		{
			params => <-.params;		
		};
		XP_COMM_PROJ.StringArrayConcatenate.StringArrayConcatenateMacs.StringArrayConcatenateFunc StringArrayConcatenateFunc
		{
			in_strings => <-.in_strings;
			params => <-.params;		
		};
		olink out_string => StringArrayConcatenateFunc.out_string;
		olink done => StringArrayConcatenateFunc.done;		
	};

};




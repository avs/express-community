
flibrary FindStringInArrayMacs
{
	//FindStringInArrayFunc
	//Functional Macro

	macro FindStringInArrayFunc
	{
		mlink in<NEportLevels={2,1}>;
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMods.FindStringInArrayParams &params<NEportLevels={2,0}>;
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMods.FindStringInArrayCore FindStringInArrayCore
		{
			active => <-.params.active;
			search_string => <-.params.search_string;
			strings => <-.in;
			ignore_whitespace => <-.params.ignore_whitespace;
		};
		olink out => FindStringInArrayCore.selectedItem;
	};
	
	//FindStringInArrayUI
	//UI Macro
	//UImod_panel containing a label, a text box and a toggle button
	
	macro FindStringInArrayUI
	{
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMods.FindStringInArrayParams &params<NEportLevels={2,0}>;
		UImod_panel UImod_panel
		{
			title => "Find String in Array";
		};
		UIlabel UIlabel
		{
			parent => <-.UImod_panel;
			label => "Search string";
			width => parent.clientWidth;
		};
		UItext UItext
		{
			parent => <-.UImod_panel;
			text => <-.params.search_string;
			y => <-.UIlabel.y+<-.UILabel.height;
			width => parent.clientWidth;
		};
		UItoggle UItoggle
		{
			parent => <-.UImod_panel;
			label => "Ignore white spaces";
			set => <-.params.ignore_whitespace;
			y => <-.UItext.y+<-.UItext.height+10;
			width => parent.clientWidth;
		};
	};

	//FindStringInArray
	//User Macro (Functional and UI macros)

	macro FindStringInArray
	{
		mlink in<NEportLevels={2,1}>;
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMods.FindStringInArrayParams params
		{
			active = 1;
			ignore_whitespace = 0;
		};
	
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMacs.FindStringInArrayFunc FindStringInArrayFunc
		{
			params => <-.params;
			in => <-.in;
		};
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMacs.FindStringInArrayUI FindStringInArrayUI
		{
			params => <-.params;
		};
		olink out => FindStringInArrayFunc.out;
	};
	
	APPS.SingleWindowApp FindStringInArrayEg
	{
		macro Input
		{
			string string<NEportLevels={1,2}>[] = {"zero","one","two","three","four"};
			UImod_panel Input;
			UIlabel UIlabel
			{
				parent => <-.Input;
				label = "Array String List";
				width => parent.clientWidth;
			};
			UIlist UIlist
			{
				parent => <-.Input;
				strings => <-.string;
				width => parent.clientWidth;
				y => <-.UIlabel.y+<-.UIlabel.heigth;
				active = 1;
			};
		};
		XP_COMM_PROJ.FindStringInArray.FindStringInArrayMacs.FindStringInArray FindStringInArray
		{
			in => <-.Input.string;
		};
		macro Output
		{
			int int<NEportLevels={2,1}> => <-.FindStringInArray.out;
			UImod_panel Output;
			UIlabel UIlabel
			{
				parent => <-.Output;
				label = "Array Element Number";
				width => parent.clientWidth;
			};
			UIfield UIfield
			{
				parent => <-.Output;
				value => <-.int;
				y => <-.UILabel.y+<-.UILabel.height;
				width => parent.clientWidth;
				mode = "integer";
			};
		};
	};
};

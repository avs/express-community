
flibrary ExtractNodeDataMacs<compile_subs=0> {
	macro StringListFunc
	{
		string+IPort2 in_list[];
		ilink item;
		XP_COMM_PROJ.StringList.StringListMods.StringListParams+IPort2 &StringListParams;
		XP_COMM_PROJ.StringList.StringListMods.StringListCore StringListCore
		{
			list => <-.in_list;
			item => StringListFunc.item;
			params => <-.StringListParams;
		};
		olink out_list => StringListCore.list;
	};

	macro UIStringList {
		ilink visible=1;
		ilink parent;
		string+IPort2 strings[];
		UIpanel UIpanel {
			visible => <-.visible;
			parent => <-.parent;
			width => parent.clientWidth;
			height => <-.UItext.y + <-.UItext.height + 10;

		};
		UIlabel UIlabel {
			parent => <-.UIpanel;
			label => name_of(<-.<-,1);
			y = 0;
			width => parent.clientWidth;
		};
		UIlist UIlist {
			parent => <-.UIpanel;
			strings => <-.StringListFunc.out_list;
			y => <-.UIlabel.y + <-.UIlabel.height + 5;
			selectedItem => <-.StringListParams.index;
			width => parent.clientWidth;
		};
		XP_COMM_PROJ.StringList.StringListMods.StringListParams StringListParams;
		UIbutton Add {
			parent => <-.UIpanel;
			do => <-.StringListParams.append;
			y => <-.UIlist.y + <-.UIlist.height + 5;
			width => parent.clientWidth / 2 - 5 ;
		};
		UIbutton Delete {
			parent => <-.UIpanel;
			do => <-.StringListParams.delete;
			x => <-.Add.x + Add.width + 10;
			y => <-.Add.y;
			width => parent.clientWidth / 2 - 5 ;
		};
		UItext UItext {
			parent => <-.UIpanel;
			y => <-.Add.y + <-.Add.height + 5;
			updateMode = 2;
			width => parent.clientWidth;
		};
		XP_COMM_PROJ.StringList.StringListMacs.StringListFunc StringListFunc {
			in_list => <-.strings;
			item => <-.UItext.text;
			StringListParams => <-.StringListParams;
		};
		olink selectedText => UIlist.selectedText;
		olink selectedItem => UIlist.selectedItem;
	};
	
	macro StringListEg<NEdisplayMode="maximized"> {
		UIshell StringListEg;
		UIframe UIframe {
			parent => <-.StringListEg;
			width => parent.clientWidth-10;
			height = 200; 
			x = 5; 
			y = 5;
		};
		XP_COMM_PROJ.StringList.StringListMacs.UIStringList UIStringList {
			parent => <-.UIframe;
		};
		UIlabel Selected_Text {
			parent => <-.StringListEg;
			y => <-.UIframe.y + <-.UIframe.height + 10;
			width => parent.clientWidth/2;
		};
		UItext Selected {
			parent => <-.StringListEg;
			y => <-.Selected_Text.y;
			x => <-.Selected_Text.x + <-.Selected_Text.width;
			width => parent.clientWidth/2 - 5;
			text => <-.UIStringList.selectedText;
		};
		UIlabel Selected_Item {
			parent => <-.StringListEg;
			y => <-.Selected_Text.y + <-.Selected_Text.height + 10;
			width => parent.clientWidth /2 ;
		};
		UIfield Item {
			parent => <-.StringListEg;
			y => <-.Selected_Item.y;
			x => <-.Selected_Item.x + <-.Selected_Text.width;
			width => parent.clientWidth/2 - 5;
			value => <-.UIStringList.selectedItem;
		};
	};
};

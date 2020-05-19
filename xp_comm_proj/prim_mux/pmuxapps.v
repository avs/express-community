flibrary PrimitiveMUXMods {
	macro PrimitiveMUXEg<NEdisplayMode="maximized"> {
		UIshell UIshell {
			x = 7;
			y = 371;
			title => name_of(<-.<-,1);
			height => <-.Output_Frame.y + <-.Output_Frame.height + 10;
			showStatusBar = 0;
		};
		UIlabel Input {
			parent => <-.UIshell;
			width => parent.clientWidth;			
			y = 5;
		};
		UIframe Input_Frame {
			y = 29;
			width => parent.clientWidth - 10;
			height => <-.set_to_1.y + <-.set_to_1.height + 10;
			parent => <-.UIshell;
			x = 5;
		};
		UIslider float_value {
			y = 0;
			width => parent.clientWidth;
			parent => <-.Input_Frame;
			value = 69.69;
		};
		UIlabel string_value {
			parent => <-.Input_Frame;
			height => <-.UItext.height;
			y => <-.float_value.y + <-.float_value.height + 5;
			width => parent.clientWidth / 3 - 5;
			alignment = "left";
		};
		UItext UItext {
			x => <-.string_value.x + <-.string_value.width + 5;
			y => <-.string_value.y;
			width => parent.clientWidth * 2/3;
			parent => <-.Input_Frame;
			text = "test";
		};
		UIslider Integer_Value {
			y => <-.UItext.y + <-.UItext.height + 5;
			width => parent.clientWidth;
			parent => <-.Input_Frame;
			value = 33.;
			mode = "integer";
		};
		UIbutton set_to_1 {
			y => <-.Integer_Value.y + <-.Integer_Value.height + 5;
			width => parent.clientWidth;
			parent => <-.Input_Frame;
		};
		XP_COMM_PROJ.PrimitiveMUX.PrimitiveMUXMods.modPMUX modPMUX {
			input => {
				<-.float_value.value,<-.UItext.text,<-.Integer_Value.value,
				<-.set_to_1.do};
		};
		UIlabel Output {
			y => <-.Input_Frame.y + <-.Input_Frame.height + 5;
			width => parent.clientWidth;
			parent => <-.UIshell;
		};
		UIframe Output_Frame {
			y => <-.Output.y + <-.Output.height + 5;
			width => parent.clientWidth - 10;
			height => <-.selected_item.y + <-.selected_item.height + 10;
			parent => <-.UIshell;
			x = 5;
		};
		UIlabel value {
			parent => <-.Output_Frame;
			height => <-.UItext#1.height;
			y => 5;
			width => parent.clientWidth / 3 - 5;
			alignment = "left";
		};
		UItext UItext#1 {
			x => <-.value.x + <-.value.width + 5;
			y => <-.value.y;
			width => parent.clientWidth * 2/3;
			parent => <-.Output_Frame;
			text => <-.modPMUX.output;
		};
		UIlabel selected_item {
			parent => <-.Output_Frame;
			height => <-.UItext#2.height;
			y => <-.value.y + <-.value.height + 5;
			width => parent.clientWidth / 3 - 5;
			alignment = "left";
		};
		UItext UItext#2 {
			x => <-.selected_item.x + <-.selected_item.width + 5;
			y => <-.selected_item.y;
			width => parent.clientWidth * 2/3;
			parent => <-.Output_Frame;
			text => <-.modPMUX.selectedItem;
		};
	};
};

// gam_macs.v
// by James S. Perrin Time-stamp: <Monday 10/09/01 16:03:51 zzcgujp>
library GroupArrManMacs {

	macro GroupArrayManagerFunc {
		ilink in;
		group+OPort farm;
		XP_COMM_PROJ.GroupArrMan.GroupArrManMods.grouparrmanParams+IPort2 & params;
		XP_COMM_PROJ.GroupArrMan.GroupArrManMods.group_array_manager group_array_manager {
			in => <-.in;
			selected => <-.params.selected;
			other => <-.params.other;
			add => <-.params.add;
			replace => <-.params.replace;
			swap => <-.params.swap;
			delete => <-.params.delete;
			nGroups => <-.params.nGroups;
			farm => <-.farm;
		};
		olink out_arr => group_array_manager.out;
		olink out => group_array_manager.out[params.selected];
	};
	
	macro GroupArrayManagerUI {
		XP_COMM_PROJ.GroupArrMan.grouparrmanParams+IPort2 &params;
		UImod_panel Group_Array_Manager {
			option {
				set = 1;
			};
		};
		UIbutton Add {
			parent => <-.Group_Array_Manager;
			do+Port2 => <-.params.add;
			width => parent.clientWidth/2 - 5;
			y = 10;
		};
		UIbutton Replace {
			parent => <-.Group_Array_Manager;
			do+Port2 => <-.params.replace;
			width => parent.clientWidth/2 - 5;
			x=> <-.Add.x + <-.Add.width + 10;
			y=> <-.Add.y;
		};
		UIbutton Swap {
			do+Port2 => <-.params.swap;
			parent => <-.Group_Array_Manager;
			width => parent.clientWidth/2 - 5;
			x => Add.x;
			y => Add.y + Add.height + 10;
		};		
		UIbutton Delete {
			do+Port2 => <-.params.delete;
			parent => <-.Group_Array_Manager;
			width => parent.clientWidth/2 - 5;
			x=> <-.Swap.x + <-.Swap.width + 10;
			y=> <-.Swap.y;
		};
		UIslider Selected {
			parent => <-.Group_Array_Manager;
			value+Port2 => <-.params.selected;
			max+IPort2 => (<-.params.nGroups - 1);
			min = 0.;
			mode = "integer";
			width => parent.clientWidth;
			y => <-.Swap.y + <-.Swap.height + 10;
		};
		UIslider Other {
			parent => <-.Group_Array_Manager;
			value => <-.params.other;
			max+IPort2 => (<-.params.nGroups - 1);
			min = 0.;
			mode = "integer";
			width => parent.clientWidth;
			y => <-.Selected.y + <-.Selected.height + 10;
		};
	};
	macro GroupArrayManager {
		ilink in;
		XP_COMM_PROJ.GroupArrMan.GroupArrManMods.grouparrmanParams params
		{
			nGroups = 0;
		};
		XP_COMM_PROJ.GroupArrMan.GroupArrManMacs.GroupArrayManagerUI GroupArrayManagerUI 
		{
			params => <-.params;
		};
		XP_COMM_PROJ.GroupArrMan.GroupArrManMacs.GroupArrayManagerFunc GroupArrayManagerFunc 
		{
			in => <-.in;
			params => <-.params;
		};
		olink out_arr => GroupArrayManagerFunc.out_arr;
		olink out => GroupArrayManagerFunc.out;
		GDM.DataObject DataObject {
			in => <-.out;
		};
		olink obj => DataObject.obj;
	};

};

flibrary ReadMETARSMacs {	


	// ReadMETARSUI
	// UI Macro
	macro ReadMETARSUI {
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIlabel file_title {
			parent => <-.UImod_panel;
			label => "METARS file:";
			y = 0;
			alignment = "left";
		};
		UItext selected_file {
			parent => <-.UImod_panel;
			text<NEportLevels={2,2}> => <-.UIfileDialog.filename;
			y => <-.file_title.y + <-.file_title.height;
			width => parent.clientWidth - <-.browse_btn.width;
		};
		UIbutton browse_btn {
			parent => <-.UImod_panel;
			y => selected_file.y;
			label => "Browse";
			message = "Select the Vis5D file to be read";
			x => parent.clientWidth - width;
			width = 65;
			height => selected_file.height;
		};

		UIfileDialog UIfileDialog {
			visible => <-.browse_btn.do;
			title => "Select METARS file to read";
			searchPattern = "./*.txt";
			filename => <-.params.filename;
		};
	};
	
	macro ReadMETARSFunc {
		ilink StationLocation;
		ilink StationID;
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSParams+IPort2 &params;
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSCore ReadMETARSCore
		{
			filename => <-.params.filename;
			StationLocation => <-.StationLocation;
			StationID => <-.StationID;
		};
		olink out => ReadMETARSCore.out;
	};
	
	macro ReadMETARS {
		ilink StationLocation;
		ilink StationID;
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMods.ReadMETARSParams params;
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMacs.ReadMETARSUI ReadMETARSUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ReadMETARS.ReadMETARSMacs.ReadMETARSFunc ReadMETARSFunc {
			StationLocation => <-.StationLocation;
			StationID => <-.StationID;
			params => <-.params;
		};
		olink out => ReadMETARSFunc.out;
		GDM.DataObject DataObject {
			in => <-.ReadMETARSFunc.out;
		};
		olink obj => DataObject.obj;
	};
	
};

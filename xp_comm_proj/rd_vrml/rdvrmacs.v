// Macro-Library of ReadVRML-project

flibrary ReadVRMLMacs 
{
	macro  ReadVRMLUI {
		XP_COMM_PROJ.ReadVRML.ReadVRMLMods.ReadVRMLParams+IPort2 & params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UItext filename {
			parent => <-.UImod_panel;
			text => <-.params.filename;
			y = 0;
			width => ((2 * parent.clientWidth) / 3);
		};
		UIbutton browse {
			parent => <-.UImod_panel;
			y => <-.filename.y;
			height => <-.filename.height;
			x => (<-.filename.x + filename.width);
			width => (parent.clientWidth / 3);
		};
		UIfileDialog UIfileDialog {
			visible => <-.browse.do;
			filename => <-.params.filename;
			dirMaskCache = "*.wrl";
		};
		UIslider Subdivision {
			parent => <-.UImod_panel;
			value => <-.params.Subdivision;
			y => ((<-.filename.y + <-.filename.height) + 15);
			min = 3;
			width => parent.clientWidth;
			mode = "integer";
		};
	};

	macro  ReadVRMLFunc {
		XP_COMM_PROJ.ReadVRML.ReadVRMLMods.ReadVRMLParams+IPort2 &params;
		XP_COMM_PROJ.ReadVRML.ReadVRMLMods.ReadVRMLCore ReadVRMLCore {
			Filename => <-.params.filename;
			Subdivision => <-.params.Subdivision;
		};
		GDM.DataObject DataObject {
			in => <-.ReadVRMLCore.out;
		};
		olink out => ReadVRMLCore.out;
		olink obj => DataObject.obj;
	};
	
	macro  ReadVRML {
		XP_COMM_PROJ.ReadVRML.ReadVRMLMods.ReadVRMLParams params
		{
			Subdivision = 20;
		};
		XP_COMM_PROJ.ReadVRML.ReadVRMLMacs.ReadVRMLUI ReadVRMLUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ReadVRML.ReadVRMLMacs.ReadVRMLFunc ReadVRMLFunc {
			params => <-.params;
		};
		olink out => ReadVRMLFunc.out;
		olink obj => ReadVRMLFunc.obj;
	};
};

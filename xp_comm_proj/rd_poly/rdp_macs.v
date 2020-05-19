flibrary ReadPolygonMacs {

	macro ReadPolygonUI {
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMods.ReadPolygonParams+IPort2 &params;
		UIlabel File {
			x = 0;
			y = 0;
			width = 75;
			parent => <-.UImod_panel;
			label => "Polygon file";
			alignment = "left";
		};
		UIbutton Browse {
			x => ((.parent.clientWidth - .width) - 2);
			y => <-.Filename.y;
			width => 70;
			height => <-.Filename.height;
			parent => <-.UImod_panel;
		};
		UIfileDialog FileDialog {
			visible => <-.Browse.do;
			filename => <-.Filename.text;
		};
		UItext Filename {
			x => <-.File.x;
			y => ((<-.File.y + <-.File.height) + 10);
			width => (( <-.Browse.x - <-.File.x ) - 2 );
			parent => <-.UImod_panel;
			text => <-.params.filename;
		};
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
	};

	macro ReadPolygonFunc {
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMods.ReadPolygonParams+IPort2 &params;
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMods.ReadPolygonCore ReadPolygonCore {
			params => <-.params;
		};
		olink out => ReadPolygonCore.out;
	};
	
	macro ReadPolygon {
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMods.ReadPolygonParams params;
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMacs.ReadPolygonUI ReadPolygonUI {
			params => <-.params;	
		};
		XP_COMM_PROJ.ReadPolygon.ReadPolygonMacs.ReadPolygonFunc ReadPolygonFunc {
			params => <-.params;
		};
		olink out => ReadPolygonFunc.out;
		GDM.DataObject DataObject {
			in => <-.ReadPolygonFunc.out;
		};
		olink obj => DataObject.obj;
	};
};
	

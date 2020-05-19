
flibrary ReadSegyMacs <compile_subs=0> { 

	macro ReadSegyUI {
		
		XP_COMM_PROJ.ReadSegy.ReadSegyMods.ReadSegyParams+IPort2 &params;
		
		UImod_panel ReadSegyPanel {
			parent<NEportLevels={4,0}>;
			title => name_of(<-.<-.<-,1);
		};
		UIlabel FilenameLabel {
			x = 10;
			y = 0;
			parent => <-.ReadSegyPanel;
			alignment = "left";
			label = "SegY file to read:";
		};
		UItext FilenameText {
			x = 0;
			y => <-.FilenameLabel.y + <-.FilenameLabel.height + 5;
			width => parent.clientWidth * (2./3.) - 5;
			parent => <-.ReadSegyPanel;
			text => <-.params.filename;
		};
		UIbutton BrowseButton {
			x => <-.FilenameText.x + <-.FilenameText.width + 5;
			y => <-.FilenameText.y;
			width => parent.clientWidth * (1./3.) - 5;
			label = "Browse...";
			parent => <-.ReadSegyPanel;
		};
		UIfileDialog SegyFileDialog {
			visible => <-.BrowseButton.do;
			filename => <-.params.filename;
			searchPattern = "*.segy";
		};
		UItoggle normalize {
			parent => <-.ReadSegyPanel;
			width => parent.clientWidth;
			y => <-.FilenameText.y+<-.FilenameText.height+10;
			set => <-.params.normalize;		
		};
		UIslider timescale {
			parent => <-.ReadSegyPanel;
			active => (! <-.params.normalize);
			width => parent.clientWidth;
			y => <-.normalize.y+<-.normalize.height+10;
			mode="integer";
			value=> <-.params.timescale;
			max=10;						
		};
	};


	macro ReadSegyFunc {
		XP_COMM_PROJ.ReadSegy.ReadSegyMods.ReadSegyParams+IPort2 &params;
		
		XP_COMM_PROJ.ReadSegy.ReadSegyMods.ReadSegyCore ReadSegyCore {
			FileName => <-.params.filename;
			timescale => <-.params.timescale;
			normalize => <-.params.normalize;
		};
		GDM.DataObject DataObject{
			in => <-.ReadSegyCore.out;
		};
		olink out => .ReadSegyCore.out;
		olink obj => .DataObject.obj;
	};


	macro ReadSegY {
		XP_COMM_PROJ.ReadSegy.ReadSegyMods.ReadSegyParams params {
			normalize = 0;
			timescale = 2;
		};
		
		XP_COMM_PROJ.ReadSegy.ReadSegyMacs.ReadSegyUI ReadSegyUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ReadSegy.ReadSegyMacs.ReadSegyFunc ReadSegyFunc {
			params => <-.params;
		};

		olink out => .ReadSegyFunc.out;
		olink obj => .ReadSegyFunc.obj;
	};

};


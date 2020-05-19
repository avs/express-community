flibrary ImageCompareMacs<compile_subs=0> {

	macro ImageCompareUI {
		XP_COMM_PROJ.ImageCompare.ImageCompareMods.ImageCompareParams+IPort2 &params;

		UImod_panel ImagePanel {
			title => "ImageCompare";
		};

		UIslider ValueSlider {
			parent => <-.ImagePanel;
			value => <-.params.Value;
			title => "Position Value";
			width => parent.clientWidth;
			max = 1.;
			min = 0.;
		};

		UIradioBoxLabel Compare_Type {
			parent => <-.ImagePanel;
			labels => {
				"Vertical","Horizontal",
				"Diagonal","Solid",
				"Circle", "Checkerboard"
			};
			selectedItem => <-.params.Mode;
			width => parent.clientWidth;
			y => <-.ValueSlider.y + <-.ValueSlider.height + 10;
		};

		UItoggle SwitchToggle {
			parent => <-.ImagePanel;
			label => "Switch datasets";
			set => <-.params.Switch;
			y => <-.Compare_Type.y + <-.Compare_Type.height + 10;
			width => parent.clientWidth;
		};
	};

	macro ImageCompareFunc {

		ilink inImage1;
		ilink inDatamap1;
		ilink inImage2;
		ilink inDatamap2;
		XP_COMM_PROJ.ImageCompare.ImageCompareMods.ImageCompareParams+IPort2 &params;

		XP_COMM_PROJ.ImageCompare.ImageCompareMods.ImageCompareCore ImageCompareCore {
			In_Data_1 => <-.inImage1;
			Datamap_1 => <-.inDatamap1;
			In_Data_2 => <-.inImage2;
			Datamap_2 => <-.inDatamap2;
			params => <-.params;
		};

		olink out => .ImageCompareCore.Out_Data;
	};

	macro ImageCompare {

		ilink inImage1;
		ilink inDatamap1;
		ilink inImage2;
		ilink inDatamap2;

		XP_COMM_PROJ.ImageCompare.ImageCompareMods.ImageCompareParams params;
		
		XP_COMM_PROJ.ImageCompare.ImageCompareMacs.ImageCompareFunc ImageCompareFunc {
			inImage1 => <-.inImage1;
			inDatamap1 => <-.inDatamap1;
			inImage2 => <-.inImage2;
			inDatamap2 => <-.inDatamap2;
			params => <-.params;
		};

		XP_COMM_PROJ.ImageCompare.ImageCompareMacs.ImageCompareUI ImageCompareUI {
			params => <-.params;
		};

		olink out => .ImageCompareFunc.out;

		GDM.DataObject DataObject {
			in => <-.ImageCompareFunc.out;
		};

		olink obj => .DataObject.obj;
	};


};

flibrary CoordinatesSliceMacs <compile_subs=0> {

	//CoordinatesSliceFunc - Functional Macro

	macro CoordinatesSliceFunc {
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.CoordinatesSliceParams+IPort2 &params;
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.Coords_3D+IPort2 &Coords_3D;
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.CoordinatesSliceCore CoordinatesSliceCore {
			plane => <-.params.plane;
			axis => <-.params.axis;
			dec => <-.params.dec;
			_3D_Coords => <-.Coords_3D;
		};
		olink xy_out => CoordinatesSliceCore.xy_out;
	};

	//CoordinatesSliceUI - User Interface

	macro CoordinatesSliceUI {
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.CoordinatesSliceParams+IPort2 &params;

		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIslider Axis {
			parent => <-.UImod_panel;
			value => <-.params.axis;
			y = 10;
			min = 0.;
			max = 2.;
			width => parent.clientWidth;
			decimalPoints = 0;
			increment = 1.;
		};
		UIslider Plane_value {
			parent => <-.UImod_panel;
			y => <-.Axis.y + <-.Axis.height + 10;
			max => <-.params.max;
			value => <-.params.plane;
			min => <-.params.min;	  
			width => parent.clientWidth;
			decimalPoints => <-.params.dec;
		};
	};

	//CoordinatesSlice - User Macro

	macro CoordinatesSlice {
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.CoordinatesSliceParams params {
			axis = 0;
			dec = 2;
			plane = 0;
			maxvals => max_array(<-.Coords_3D.xyz_Coords);
			minvals => min_array(<-.Coords_3D.xyz_Coords);
			max => maxvals[axis];
			min => minvals[axis];
		};
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMods.Coords_3D+IPort2 &Coords_3D;
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMacs.CoordinatesSliceFunc CoordinatesSliceFunc {
			params => <-.params;
			Coords_3D => <-.Coords_3D;
		};
		XP_COMM_PROJ.CoordinatesSlice.CoordinatesSliceMacs.CoordinatesSliceUI CoordinatesSliceUI {
			params => <-.params;
		};
		olink xy_out => CoordinatesSliceFunc.xy_out;
	};

};

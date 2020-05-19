
flibrary BodyOfRevolutionMacs {

	// UI Macros
	macro BodyOfRevolutionUI {
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMods.BodyOfRevolutionParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIslider Subdivision {
			parent => <-.UImod_panel;
			value => <-.params.num_thetas;
			y = 0;
			width => parent.clientWidth;
			mode = "integer";
		};
		UItoggle Nose_cap<NEx=253.,NEy=286.> {
			parent => <-.UImod_panel;
			set => <-.params.nose_cap_on;
			width => parent.clientWidth;
			y => <-.Subdivision.y + <-.Subdivision.height+10;
		};
		UIlabel Density<NEx=253.,NEy=341.> {
			parent => <-.UImod_panel;
			width => parent.clientWidth/2 - 5;
			y => <-.Nose_cap.y+Nose_cap.height + 10;
			alignment = "left";
		};
		UIfield dens<NEx=253.,NEy=385.> {
			parent => <-.UImod_panel;
			value => <-.params.density;
			width => parent.clientWidth/2 - 5;
			y => <-.Density.y;
			x => <-.Density.x + Density.width + 10;
			format = "mixed";
			nullString = "0.0";
		};
		UIlabel Mass<NEx=253.,NEy=429.> {
			parent => <-.UImod_panel;
			y => <-.Density.y+<-.Density.height+20;
			width => parent.clientWidth/2 - 5;
			alignment = "left";
		};
		UIfield massout<NEx=253.,NEy=473.> {
			parent => <-.UImod_panel;
			value => <-.params.mass;
			width => parent.clientWidth/2 - 5;
			y => <-.Mass.y;
			x => <-.Mass.x+<-.Mass.width + 10;
			outputOnly = 1;
			format = "mixed";
			nullString = "0.0";
		};
	};

	// Functional macros (Field I/O)
	macro BodyOfRevolutionFunc {
		ilink r_outside;
		ilink z_outside;
		ilink r_inside;
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMods.BodyOfRevolutionParams+IPort2 &params;
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMods.BodyOfRevolutionCore BodyOfRevolutionCore<NEx=220.,NEy=132.> {
			params => <-.params;
			r_outside => <-.r_outside;
			z_outside => <-.z_outside;
			r_inside => <-.r_inside;
		};
		olink mass => BodyOfRevolutionCore.mass;
		olink out => BodyOfRevolutionCore.out;
	};

	// User Macros (Functional and UI)

	macro BodyOfRevolution {
		ilink r_outside;
		ilink z_outside;
		ilink r_inside;
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMods.BodyOfRevolutionParams params {
			active = 1;
			nose_cap_on = 0;
			num_thetas = 20;
			density = 1;
			mass = 0;
		};
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMacs.BodyOfRevolutionUI BodyOfRevolutionUI {
			params => <-.params;
		};
		XP_COMM_PROJ.BodyOfRevolution.BodyOfRevolutionMacs.BodyOfRevolutionFunc BodyOfRevolutionFunc {
			r_outside => <-.r_outside;
			z_outside => <-.z_outside;
			r_inside => <-.r_inside;
			params => <-.params;
		};
		GDM.DataObject DataObject {
			in => <-.BodyOfRevolutionFunc.out;
		};
		olink mass => .BodyOfRevolutionFunc.mass;
		olink out => BodyOfRevolutionFunc.out;
		olink obj => DataObject.obj;
	};

};


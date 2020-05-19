
flibrary TubesMacs <compile_subs=0> {

macro TubesFunc {
	XP_COMM_PROJ.Tubes.TubesMods.TubesParams+IPort2 &TubesParams;

	ilink lines;
	ilink data;

	MODS.interp_data DVinterp_data {
		UIpanel InterpUI;
		in_field	=> <-.data;
		in_probe	=> <-.lines;
		DVinterpolate_data {
			comps	=> <-.<-.TubesParams.col_scale_comp;
		};
	};

	DVM.DVswitch input_switch {
		in => {	<-.lines,
			<-.DVinterp_data.out_fld
		};
		index => (is_valid(<-.data) & (!<-.TubesParams.activate_simpletube));
	};

	XP_COMM_PROJ.Tubes.TubesMods.tube tube {
		in			=> <-.input_switch.out;
		axis			=> <-.TubesParams.axis;
		xform			=> <-.TubesParams.do_transform;
		zscale			=> <-.TubesParams.z_scale;
		resolution		=> <-.TubesParams.facets;
		normalise		=> <-.TubesParams.normalise;
		scale			=> <-.TubesParams.scale;
		node_cell		=> <-.TubesParams.node_cell;
		node_data_comp	=> <-.TubesParams.node_data_comp;
		cell_data_comp	=> <-.TubesParams.cell_data_comp;
		col_data_comp	=> <-.TubesParams.col_data_comp;
	};

	XP_COMM_PROJ.Tubes.TubesMods.TubesParams blendyParams {
		axis				=> <-.TubesParams.axis;
		do_transform		=> <-.TubesParams.do_transform;
		z_scale				=> <-.TubesParams.z_scale;
		facets				=> <-.TubesParams.facets;
		normalise			=> <-.TubesParams.normalise;
		scale				=> <-.scaleSwitch.out;
		node_cell			=> <-.TubesParams.node_cell;
		node_data_comp		=> <-.TubesParams.node_data_comp;
		cell_data_comp		=> <-.TubesParams.cell_data_comp;
		col_data_comp		=> <-.TubesParams.col_data_comp;
		offsetScale			=> <-.TubesParams.offsetScale;
		invertScale			=> <-.TubesParams.invertScale;
		col_scale_comp[2]	=> <-.TubesParams.col_scale_comp;
		only_tube_allowed	=> <-.TubesParams.only_tube_allowed;
		activate_simpletube	=> <-.TubesParams.activate_simpletube;
		use_blendy_scaling	=> <-.TubesParams.use_blendy_scaling;
	};

	XP_COMM_PROJ.Tubes.TubesMods.BlendyTubesCore BlendyTubesCore {
		lines_in	=> <-.input_switch.out;
		params		=> <-.blendyParams;
	};

	DVM.DVswitch output_switch {
		in	=> {
			<-.BlendyTubesCore.tubes_out,
			<-.tube.out
		};
		index	=> <-.TubesParams.only_tube_allowed;
	};

	//GMOD.instancer instancer_Blendy {
	//	Value => (!<-.TubesParams.only_tube_allowed);
	//	Group => <-.BlendyTubesCore;
	//};
	//GMOD.instancer instancer_tube {
	//	Value => <-.TubesParams.only_tube_allowed;
	//	Group => <-.tube;
	//};

	GMOD.copy_on_change copy_on_change {
		input	=> ((<-.lines.nspace != 3) | (<-.lines.cell_set[0].poly_flag == 0));
		output	=> <-.TubesParams.only_tube_allowed;
	};

	GDM.DataObject DataObject {
		in => <-.output_switch.out;
	};

	olink out_fld => .output_switch.out;
	olink out_obj => .DataObject.obj;

	float+Port finalScale;

	DVM.DVswitch scaleSwitch {
		float &in[]	=> {
			<-.TubesParams.scale,
			<-.finalScale
		};
		index		=> (is_valid(<-.data) & <-.TubesParams.use_blendy_scaling);
		float &out	=> in[index];
	};

	//DVM.DVswitch parse_vSwitch {
	//	int in[]	=> {0, 1};
	//	index		=> (is_valid(<-.data) & <-.TubesParams.use_blendy_scaling);
	//	int out		=> in[index];
	//};

	int+Port initialScale	= 50;
	float+Port coordSize	=> sqrt(((pow((data.coordinates.max_vec[0] - data.coordinates.min_vec[0]),2) +
								pow((data.coordinates.max_vec[1] - data.coordinates.min_vec[1]),2)) +
								pow((data.coordinates.max_vec[2] - data.coordinates.min_vec[2]),2)));

	GMOD.parse_v parse_v {
		v_commands	= "finalScale = TubesParams.scale * (coordSize / (data.node_data[TubesParams.col_scale_comp[1]].max - data.node_data[TubesParams.col_scale_comp[1]].min)) / initialScale";
		no_errors	= 1;
		relative	=> <-;
		//active		=> <-.parse_vSwitch.out;
		active		=> (is_valid(<-.data) & <-.TubesParams.use_blendy_scaling);
		trigger		=> <-.TubesParams.scale;
	};
};

macro TubesUI {
	XP_COMM_PROJ.Tubes.TubesMods.TubesParams+IPort2 &TubesParams;
	UImod_panel tubes;
	ilink in;

	macro TubeUI {
		XP_COMM_PROJ.Tubes.TubesMods.TubesParams+IPort2 &TubesParams => <-.TubesParams;
		UIpanel tube {
			y				= 0;
			visible			=> <-.TubesParams.only_tube_allowed;
			parent			=> <-.<-.tubes;
			width			= 250;
			height			= 1500;
			clientWidth		= 250;
			clientHeight	= 1500;
		};

		UIslider axisSlider {
			y = 8;
			width			=> <-.w;
			parent			=> <-.tube;
			min				= 0.;
			max				= 2.;
			value			=> <-.TubesParams.axis;
			mode			= "integer";
			decimalPoints	= 0;
			immediateMode	= 0;
			title			=> "2D projection axis";
		};
		UItoggle xformToggle {
			y		=> ((<-.axisSlider.y + <-.axisSlider.height) + <-.axisSlider.y);
			width	=> <-.w;
			parent	=> <-.tube;
			label	=> "Do xform on 2D datasets";
			set		=> <-.TubesParams.do_transform;
		};

		int+Port w	=> tube.clientWidth;

		UIlabel zsLabel {
			y			=> ((<-.xformToggle.y + <-.xformToggle.height) + <-.axisSlider.y);
			width		= 120;
			parent		=> <-.tube;
			label		=> "Z Scale for 2D transform";
			alignment	= "left";
		};
		UIfield zsField {
			x		=> (<-.zsLabel.x + <-.zsLabel.width);
			y		=> (<-.zsLabel.y - (.height / 5));
			width	=> (<-.w - .x);
			parent	=> <-.tube;
			value	=> <-.TubesParams.z_scale;
		};
		UIslider resSlider {
			y				=> ((<-.zsField.y + <-.zsField.height) + <-.axisSlider.y);
			parent			=> <-.tube;
			min				= 3.;
			max				= 36.;
			value			=> <-.TubesParams.facets;
			mode			= "integer";
			decimalPoints	= 0;
			immediateMode	= 0;
			title			=> "Number of facets on tubes";
		};
		UItoggle normToggle {
			y		=> ((<-.resSlider.y + <-.resSlider.height) + <-.axisSlider.y);
			width	=> <-.w;
			parent	=> <-.tube;
			label	=> "Normalise tubes to Radius Scale";
			set		=> <-.TubesParams.normalise;
		};
		UIlabel scaleLabel {
			y			=> ((<-.normToggle.y + <-.normToggle.height) + <-.axisSlider.y);
			width		= 120;
			parent		=> <-.tube;
			label		=> "Radius Scale";
			alignment	= "left";
		};
		UIfield scaleField {
			x				=> (<-.scaleLabel.x + <-.scaleLabel.width);
			y				=> (<-.scaleLabel.y - (.height / 5));
			width			=> (<-.w - .x);
			parent			=> <-.tube;
			value			=> <-.TubesParams.scale;
			decimalPoints	= 6;
		};
		UItoggle ncToggle {
			y		=> ((<-.scaleField.y + <-.scaleField.height) + <-.axisSlider.y);
			width	=> <-.w;
			parent	=> <-.tube;
			label	=> "Select node data (off) or cell data (on)";
			set		=> <-.TubesParams.node_cell;
		};
		UIlabel ndLabel {
			y			=> ((<-.ncToggle.y + <-.ncToggle.height) + <-.axisSlider.y);
			width		= 120;
			active		=> (!<-.ncToggle.set);
			parent		=> <-.tube;
			label		=> "Node data component";
			alignment	= "left";
		};
		UIlabel cdLabel {
			y			=> ((<-.ndField.y + <-.ndField.height) + <-.axisSlider.y);
			width		= 120;
			active		=> (!<-.ndLabel.active);
			parent		=> <-.tube;
			label		=> "Cell data component";
			alignment	= "left";
		};
		UIfield ndField {
			x				=> (<-.ndLabel.x + <-.ndLabel.width);
			y				=> (<-.ndLabel.y - (.height / 5));
			width			=> (<-.w - .x);
			active			=> <-.ndLabel.active;
			parent			=> <-.tube;
			min				= 0.;
			value			=> <-.TubesParams.node_data_comp;
			mode			= "integer";
			decimalPoints	= 0;
		};
		UIfield cdField {
			x				=> (<-.colLabel.x + <-.colLabel.width);
			y				=> (<-.cdLabel.y - (.height / 5));
			width			=> (<-.w - .x);
			active			=> <-.cdLabel.active;
			parent			=> <-.tube;
			min				= 0.;
			value			=> <-.TubesParams.cell_data_comp;
			mode			= "integer";
			decimalPoints	= 0;
		};
		UIlabel colLabel {
			y			=> ((<-.cdField.y + <-.cdField.height) + <-.axisSlider.y);
			width		= 120;
			parent		=> <-.tube;
			label		=> "Colour component";
			alignment	= "left";
		};
		UIfield colField {
			x				=> (<-.colLabel.x + <-.colLabel.width);
			y				=> (<-.colLabel.y - (.height / 5));
			width			=> (<-.w - .x);
			parent			=> <-.tube;
			min				= 0.;
			value			=> <-.TubesParams.col_data_comp;
			mode			= "integer";
			decimalPoints	= 0;
		};
	};

	macro BlendyTubesUI {
		Mesh+Node_Data+IPort2 &in => <-.in;

		XP_COMM_PROJ.Tubes.TubesMods.TubesParams+IPort2 &TubesParams => <-.TubesParams;

		UIpanel blendy_tubes {
			visible	=> (!<-.TubesParams.only_tube_allowed);
			y		= 0;
			width	= 250;
			height	= 1500;
			parent	=> <-.<-.tubes;
		};

		UItoggle Use_simpletubes {
			y		= 0;
			parent	=> <-.blendy_tubes;
			width	=> parent.width;
			label	=> "use simpletubes";
			set		=> <-.TubesParams.activate_simpletube;
			visible	=> is_valid(<-.in);
		};
		UIslider Facets {
			x				=> <-.Use_simpletubes.x;
			y				=> (<-.Use_simpletubes.y + ((<-.Use_simpletubes.height + 4) *
								<-.Use_simpletubes.visible));
			parent			=> <-.blendy_tubes;
			min				= 3.;
			max				= 32.;
			value			=> TubesParams.facets;
			mode			= "integer";
			decimalPoints	= 0;
			increment		= 3.;
		};
		UItoggle Use_blendy_scale {
			x		=> <-.Facets.x;
			y		=> ((<-.Facets.y + <-.Facets.height) + 20);
			parent	=> <-.blendy_tubes;
			width	=> parent.width;
			label	=> "use BlendyTubes scaling";
			set		=> <-.TubesParams.use_blendy_scaling;
			visible	=> is_valid(<-.in);
		};
		UIslider Scale {
			x				=> <-. Use_blendy_scale.x;
			y				=> (<-.Use_blendy_scale.y + ((<-.Use_blendy_scale.height + 4) *
								<-.Use_blendy_scale.visible));
			parent			=> <-.blendy_tubes;
			min				= 0.;
			max				= 10.;
			value			=> <-.TubesParams.scale;
			mode			= "real";
			decimalPoints	= 2;
		};
		UItoggle Offset_Scale {
			x		=> <-.Scale.x;
			y		=> ((<-.Scale.y + <-.Scale.height) + 20);
			width	=> (((parent.width - .x) / 2) - 2);
			parent	=> <-.blendy_tubes;
			set		=> <-.TubesParams.offsetScale;
		};
		UItoggle Invert_Scale {
			x		=> ((<-.Offset_Scale.x + <-.Offset_Scale.width) + 4);
			y		=> <-.Offset_Scale.y;
			width	=> <-.Offset_Scale.width;
			parent	=> <-.blendy_tubes;
			set		=> <-.TubesParams.invertScale;
		};

		macro node_data_labels {
			group labelfield[in.nnode_data] {
				string &label => in.node_data[index_of(labelfield)].labels;
			};
			string+Port &labels[]	=> labelfield.label;
			ilink in				=> <-.in;
			omlink out				=> labels;
		};

		UIradioBoxLabel Scale_Component {
			parent			=> <-.blendy_tubes;
			labels			=> <-.node_data_labels.out;
			selectedItem	=> <-.TubesParams.col_scale_comp[1];
			x				=> <-.Offset_Scale.x;
			y				=> ((<-.Offset_Scale.y + <-.Offset_Scale.height) + 20);
			UIpanel {
				visible => (is_valid(<-.<-.in) & (!<-.<-.Use_simpletubes.set));
			};
		};
		UIradioBoxLabel Colour_Component {
			parent			=> <-.blendy_tubes;
			labels			=> <-.node_data_labels.out;
			selectedItem	=> <-.TubesParams.col_scale_comp[0];
			x				=> <-.Scale_Component.x;
			y				=> ((<-.Scale_Component.y + <-.Scale_Component.height) + 20);
			UIpanel {
				visible => (is_valid(<-.<-.in) & (!<-.<-.Use_simpletubes.set));
			};
		};
	};
};

macro Tubes {
	XP_COMM_PROJ.Tubes.TubesMods.TubesParams TubesParams {
		axis				= 0;
		do_transform		= 0;
		z_scale				= 1;
		facets				= 3;
		normalise			= 0;
		scale				= 1;
		node_cell			= 0;
		node_data_comp		= 0;
		cell_data_comp		= 0;
		col_data_comp		= 0;
		offsetScale			= 0;
		invertScale			= 0;
		col_scale_comp		= {4, 0};
		only_tube_allowed	= 0;
		activate_simpletube	= 0;
		use_blendy_scaling	= 1;
	};
	XP_COMM_PROJ.Tubes.TubesMacs.TubesUI TubesUI {
		TubesParams	=> <-.TubesParams;
		in			=> <-.data;
	};

	ilink lines;
	ilink data;

	XP_COMM_PROJ.Tubes.TubesMacs.TubesFunc TubesFunc {
		lines		=> <-.lines;
		data		=> <-.data;
		TubesParams	=> <-.TubesParams;
	};

	olink out_fld => .TubesFunc.out_fld;
	olink out_obj => .TubesFunc.out_obj;
};

// example app - normal usage
APPS.SingleWindowApp TubesEg1 {

	GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {<-.<-.<-.bounds.out_obj,<-.<-.<-.tubes.out_obj};
			};
		};
	};

	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "$XP_PATH<0>/data/field/hydrogen.fld";
		};
	};

	MODS.bounds bounds {
		in_field => <-.Read_Field.field;
	};

	MODS.orthoslice orthoslice {
		in_field => <-.Read_Field.field;
		OrthoSliceParam {
			axis = 2;
		};
	};

	MODS.isoline isoline {
		in_field => <-.orthoslice.out_fld;
	};

	XP_COMM_PROJ.Tubes.TubesMacs.Tubes tubes {
		TubesUI {
			TubesParams {
				axis			= 2;
				do_transform	= 1;
				facets			= 4;
				normalise		= 1;
				scale			= 0.2;
			};
		};
		lines => <-.isoline.out_fld;
	};

};

// example to show Z scaling for slice plane
APPS.SingleWindowApp TubesEg2 {
	GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {<-.<-.<-.tubes.out_obj,<-.<-.<-.bounds.out_obj};
			};
		};
	};

	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "$XP_PATH<0>/data/field/hydrogen.fld";
		};
	};

	MODS.bounds bounds {
		in_field => <-.Read_Field.field;
	};

	MODS.slice_plane slice_plane {
		in_field => <-.Read_Field.field;
		plane {
			xform {
				xlate	= {0.,0.,60.};
				mat		= {
					0.5,0.,-0.866025,0.,0.224144,0.965926,0.12941,
					0.,0.836516,-0.258819,0.482963,0.,0.,0.,0.,1.
				};
			};
		};
		SliceUI {
			plane_xform {
				GDxform_editor {
					x_rot		= 15.;
					y_rot		= 60.;
					z_trans		= 60.;
					abs_x_rot	= 15.;
					abs_y_rot	= 60.;
					abs_z_trans	= 60.;
				};
				XformEditorUI {
					trans_shell {
						ok = 1;
					};
				};
			};
		};
	};

	MODS.isoline isoline {
		in_field => <-.slice_plane.out_fld;
		IsoParam {
			level_min => 0.;
			level_max => 255.;
		};
	};

	XP_COMM_PROJ.Tubes.TubesMacs.Tubes tubes {
		TubesUI {
			TubesParams {
				axis			= 2;
				do_transform	= 1;
				z_scale			= 64.;
				scale			= 0.0001;
			};
		};
		lines => <-.isoline.out_fld;
	};
};

// example application
APPS.SingleWindowApp SimpleTubesEg {
	GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {
					<-.<-.<-.bounds.out_obj,
					<-.<-.<-.Line3D.out_obj,
					<-.<-.<-.tubes.out_obj
				};
				Xform {
					mat		= {
						0.320307,0.0733338,-0.148807,0.,
						-0.0235839,0.340404,0.116991,0.,
						0.164211,-0.0941555,0.307063,0.,
						0.,0.,0.,1.
					};
					xlate	= {-1.18071,-1.50195,-1.04656};
					center	= {3.27323,4.16378,2.90132};
				};
			};
			Camera {
				Camera {
					perspec		= 1;
					front		= 1.;
					auto_norm	= "None";
				};
			};
		};
	};

	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "$XP_ROOT/data/field/bluntfin.fld";
		};
	};

	MODS.bounds bounds {
		in_field => <-.Read_Field.field;
	};

	MODS.combine_vect combine_vect {
		in_field => <-.Read_Field.field;
		CombineVectParam {
			components = {1,2,3};
		};
	};

	GEOMS.Line3D Line3D {
		line3 {
			xform {
				mat		= {
					2.11102,-0.0167101,-5.29822,0.,
					4.76114,-2.49615,1.9049,0.,
					-2.32444,-5.12804,-0.90997,0.,
					0.,0.,0.,1.
				};
				xlate	= {-6.95756,0.929703,5.48915};
			};
	 };
	};

	MODS.streamlines streamlines {
		in_field => <-.combine_vect.out_fld;
		in_probe => <-.Line3D.out_fld;
	};

	XP_COMM_PROJ.Tubes.TubesMacs.Tubes tubes {
		lines => <-.streamlines.out_fld;

		TubesParams {
			scale	= 0.3;
			facets	= 6;
		};
	};

};

// example application
APPS.SingleWindowApp BlendyTubesEg {

	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "$XP_ROOT/data/field/bluntfin.fld";
		};
	};

	MODS.bounds bounds {
		in_field => <-.Read_Field.field;
	};

	MODS.combine_vect combine_vect {
		in_field => <-.Read_Field.field;
		CombineVectParam {
			components = {1,2,3};
		};
	};

	GEOMS.Line3D Line3D {
		line3 {
			xform {
				mat		= {
					2.11102,-0.0167101,-5.29822,0.,
					4.76114,-2.49615,1.9049,0.,
					-2.32444,-5.12804,-0.90997,0.,
					0.,0.,0.,1.
				};
				xlate	= {-6.95756,0.929703,5.48915};
			};
	 };
	};

	MODS.streamlines streamlines {
		in_field => <-.combine_vect.out_fld;
		in_probe => <-.Line3D.out_fld;
	};

	XP_COMM_PROJ.Tubes.TubesMacs.Tubes tubes {
		lines	=> <-.streamlines.out_fld;
		data	=> <-.Read_Field.field;

		TubesParams {
			scale		= 1.50;
			facets		= 6;
			offsetScale	= 1;
		};
	};

	GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {
					<-.<-.<-.bounds.out_obj,
					<-.<-.<-.Line3D.out_obj,
					<-.<-.<-.tubes.out_obj
				};
				Xform {
					mat		= {
						0.320307,0.0733338,-0.148807,0.,
						-0.0235839,0.340404,0.116991,0.,
						0.164211,-0.0941555,0.307063,0.,
						0.,0.,0.,1.
					};
					xlate	= {-1.18071,-1.50195,-1.04656};
					center	= {3.27323,4.16378,2.90132};
				};
			};
			Camera {
				Camera {
					perspec		= 1;
					front		= 1.;
					auto_norm	= "None";
				};
			};
		};
	};

};

// example application
APPS.SingleWindowApp EdgesTubesEg {
	GDM.Uviewer3D Uviewer3D {
		Scene {
			Top {
				child_objs => {<-.<-.<-.tubes.out_obj,<-.<-.<-.bounds.out_obj};
			};
		};
	};

	MODS.Read_Field Read_Field {
		read_field_ui {
			filename = "$XP_PATH<0>/data/field/hydrogen.fld";
		};
	};

	MODS.bounds bounds {
		in_field => <-.Read_Field.field;
	};

	MODS.isosurface isosurface {
		in_field => <-.Read_Field.field;
	};
	
	MODS.edges edges {
		in_field => <-.isosurface.out_fld;
	};
	
	XP_COMM_PROJ.Tubes.TubesMacs.Tubes tubes {
		TubesUI {
			TubesParams {
				axis			= 2;
				do_transform	= 1;
				z_scale			= 64.;
				scale			= 0.0025;
			};
			UImod_panel {
				option {
					set = 1;
				};
			};
		};
		lines => <-.edges.out_fld;
	};
};

};

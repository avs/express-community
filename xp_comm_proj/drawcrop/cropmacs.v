flibrary DrawCropMacs //<user_library=0> 
{
//*******list of the functional macros used in this module
	  //box
	  //ContinuousSketch
	  //macroAXIS
	  //macroAXIS2
	  //parameters
	  //macroUI
	  //macroDrawCrop
	  //DrawCrop

//***********************************************************************

	macro DrawCropBox {
		GEOMS.Cross2D Cross2D {
			cross {
				coordinates {
					values<NEportLevels={5,0}>;
				};
			};
			Cross2D {
				out_obj<NEportLevels={1,3}>;
			};
		};
		GEOMS.Cross2D Cross2D#1 {
			cross {
				coordinates {
					values<NEportLevels={5,0}>;
				};
			};
			Cross2D#1 {
				out_obj<NEportLevels={1,3}>;
			};
		};
	};

//***********************************************************************


	macro ContSketchViz{
		ilink+nres in_view;
		ilink+nres in_obj;
		GDM.ContinuousSketch ContinuousSketch {
			cont_sketch {
				UImod_panel {
					visible = 0;
					option {
						set = 0;
						title = "Blank";
						active = 0;
					};
					active = 0;
				};
				GDroi2d_cont {
					option = "Box";
					immed = "On Button Up";
					mode = "Clear";
				};
			};
			view_in => <-.in_view;
			obj_in => <-.in_obj;
		};
		olink+nres min_vec => ContinuousSketch.cont_sketch.GDmap2d_ss.out_mesh.coordinates.min_vec; 
		olink+nres max_vec => ContinuousSketch.cont_sketch.GDmap2d_ss.out_mesh.coordinates.max_vec; 
	};


//***********************************************************************

	macro DrawCropAXIS {
		group &params<NEportLevels={2,1}>;
		DVM.DVorthoslice_unif DVorthoslice_unif {
			in<NEportLevels={3,0}>;
		};
		MODS.xform_to_plane xform_to_plane {
			in_field => <-.DVorthoslice_unif.out;
			in_plane => <-.DVorthoslice_unif.out;
		};
		DVM.DVscale DVscale {
			in => <-.xform_to_plane.out_fld;
		};
		GDM.DataObject DataObject {
			in => <-.DVscale.out;
			obj<NEportLevels={1,3}>;
		};
	};

//***********************************************************************

	macro DrawCropAXIS2 {
		group &params<NEportLevels={2,1}>;
		DVM.DVorthoslice_unif DVorthoslice_unif {
			in<NEportLevels={3,0}>;
		};
		MODS.xform_to_plane xform_to_plane {
			in_field => <-.DVorthoslice_unif.out;
			in_plane => <-.DVorthoslice_unif.out;
		};
		GDM.DataObject DataObject {
			in => <-.xform_to_plane.out_fld;
			obj<NEportLevels={1,3}>;
		};
	};

//***********************************************************************

	group+OPort parameters {
		int+Port axis2;
		int+Port axis1;
		int+Port axis0;
		float+Port zscale;
	};



//************************************************************************

	macro select_scene {
		ilink top;
		ilink right;
		ilink front;
		ilink _3D;
		ilink parent;
		UIoption sel_top {
			set => <-.top;
		};
		UIoption sel_front {
			set => <-.front;
		};
		UIoption sel_right {
			set => <-.right;
		};
		UIoption sel_3D {
			set => <-._3D;
		};
		UIradioBox UIradioBox {
			visible = 0;
			parent => <-.parent;
			cmdList => {<-.sel_top,<-.sel_front,
				<-.sel_right,<-.sel_3D};
		};
		olink selection => UIradioBox.selectedItem;
	};


	macro DrawCropUI {
		ilink+nres infield;
		parameters &params<NEportLevels={2,1}>;

		UImod_panel UImod_panel {
			option {
				set = 1;
			};
			height = 984;
			title = "3D DrawCrop";
		};

		UIpanel ortho_panel{
			y=0;
			parent => <-.UImod_panel;
			width => parent.clientWidth; 
			height => ( 
				<-.UIlabelORTHO.height + 
				<-.AXIS2_PLANE.height +
				<-.AXIS1_PLANE.height +
				<-.AXIS0_PLANE.height +
				<-.UIlabelSCALE.height +
				<-.UIsliderSCALE.height + 40
			);
		};

		UIlabel UIlabelORTHO {
			parent => <-.ortho_panel;
			label => "Orthoslice controls";
			y = 0;
			width => parent.clientWidth;
			message = "Orthoslice controls";
		};

		UIslider AXIS2_PLANE {
			parent => <-.ortho_panel;
			y => <-.UIlabelORTHO.y + <-.UIlabelORTHO.height;
			width => parent.clientWidth;
			max+nres => (<-.infield.dims[2] - 1);
			min = 0.;
			decimalPoints = 0;
			title => "Orthoslice of principal axis";
			value => <-.params.axis2;
		};
		UIslider AXIS1_PLANE {
			parent => <-.ortho_panel;
			y => <-.AXIS2_PLANE.y + <-.AXIS2_PLANE.height+10;
			max+nres => (<-.infield.dims[1] - 1);
			min = 0.;
			decimalPoints = 0;
			title => "Orthoslice of secondary axis";
			width => parent.clientWidth;
			value => <-.params.axis1;
		};
		UIslider AXIS0_PLANE {
			parent => <-.ortho_panel;
			y => <-.AXIS1_PLANE.y + <-.AXIS1_PLANE.height+10;
			max+nres => (<-.infield.dims[0] - 1);
			width => parent.clientWidth;
			min = 0.;
			decimalPoints = 0;
			title => "Orthoslice of tertiary axis";
			value => <-.params.axis0;
		};

		UIlabel UIlabelSCALE {
			parent => <-.ortho_panel;
			y => <-.AXIS0_PLANE.y + <-.AXIS0_PLANE.height+10;
			label => "Z Axis Scale Value";
			width => parent.clientWidth;
		};
		UIslider UIsliderSCALE {
			parent => <-.ortho_panel;
			y => <-.UIlabelSCALE.y + <-.UIlabelSCALE.height;
			width => parent.clientWidth;
			max = 10.0;
			min = 1.0;
			value+nres => (<-.params.zscale);
			title => "Scale value";
		};


		UIframe UIframe {
			parent => <-.UImod_panel;
			y => <-.ortho_panel.y + <-.ortho_panel.height;
			width => parent.clientWidth;
			height => (
				<-.UIlabel.height + 
				<-.RESTART.height +
				<-.OK_AXIS2.height +
				<-.OK_AXIS1.height +
				<-.OK_AXIS0.height +
				<-.ACCEPT_CROP_VALUES.height + 15				
			);
		};
		UIlabel UIlabel {
			parent => <-.UIframe;
			width => parent.clientWidth;
			label => "Crop Controls";
			y = 0;
		};
		UItoggle OK_AXIS2 {
			parent => <-.UIframe;
			y => <-.UIlabel.y + <-.UIlabel.height+5;
			width => parent.clientWidth;
			label => "Crop for principal axis";
		};
		UItoggle OK_AXIS1 {
			parent => <-.UIframe;
			y => <-.OK_AXIS2.y + <-.OK_AXIS2.height;
			label => "Crop for secondary axis";
			width => parent.clientWidth;
		};
		UItoggle OK_AXIS0 {
			parent => <-.UIframe;
			y => <-.OK_AXIS1.y + <-.OK_AXIS1.height;
			label => "Crop for tertiary axis";
			width => parent.clientWidth;
		};
		UItoggle ACCEPT_CROP_VALUES {
			parent => <-.UIframe;
			label => "Accept crop values";
			width => parent.clientWidth;
			y => <-.OK_AXIS0.y + <-.OK_AXIS0.height;
		};
		UItoggle RESTART {
			parent => <-.UIframe;
			y => <-.ACCEPT_CROP_VALUES.y + <-.ACCEPT_CROP_VALUES.height+5;
			label => "RESET parameters";
			width => parent.clientWidth;
		};

		XP_COMM_PROJ.DrawCrop.DrawCropMods.crop_controls crop_controls {
			dims_fld+nres => <-.infield.dims;
			restart => <-.RESTART.set;
			OK_axis2 => <-.OK_AXIS2.set;
			OK_axis1 => <-.OK_AXIS1.set;
			OK_axis0 => <-.OK_AXIS0.set;
			all_OK => <-.ACCEPT_CROP_VALUES.set;
			set_coord<NEportLevels={0,3}>;
		};
	};

	
	GDM.Iscene2D Iscene2D_select {
		int+OPort2 clicked => View.View.mouse_events.buttonMask;
	};

	DrawCropUI DrawCropViewUI {
		MODS.orthoslice orthoslice_X {
			in_field => <-.infield;
			OrthoSliceParam {
				axis = 0;
				plane => <-.<-.params.axis0;
			};
			int OrthoSliceUI;
		};
		MODS.orthoslice orthoslice_Y {
			in_field => <-.infield;
			OrthoSliceParam {
				axis = 1;
				plane => <-.<-.params.axis1;
			};
			int OrthoSliceUI;
		};
		MODS.orthoslice orthoslice_Z {
			in_field => <-.infield;
			OrthoSliceParam {
				axis = 2;
				plane => <-.<-.params.axis2;
			};
			int OrthoSliceUI;
		};

		UIpanel ViewPanel {
			parent => UImod_panel;
			x = 0;
			y = 0;
			width => parent.clientWidth;
			height => width;
		};
		int size => min_array({ViewPanel.clientWidth,ViewPanel.clientHeight});

		UIframe top {
			parent => <-.ViewPanel;
			x = 0;
			y = 0;
			width => (<-.size / 2);
			height => .width;
		};
		UIframe _3D {
			parent => <-.ViewPanel;
			y => <-.top.y;
			x => (<-.top.x + <-.top.width);
			width => (<-.size / 2);
			height => .width;
		};
		UIframe front {
			parent => <-.ViewPanel;
			y => (<-.top.y + <-.top.height);
			x => <-.top.x;
			width => (<-.size / 2);
			height => .width;
		};
		UIframe right {
			parent => <-.ViewPanel;
			x => (<-.top.x + <-.top.width);
			y => (<-.top.y + <-.top.height);
			width => (<-.size / 2);
			height => .width;
		};
		MODS.reset_xform reset_xform_x {
			in_field => <-.orthoslice_X.out_fld;
		};
		MODS.reset_xform reset_xform_y {
			in_field => <-.orthoslice_Y.out_fld;
		};
		Iscene2D_select Iscene2D_top {
			View {
				ViewUI {
					ViewWindow {
						parent => <-.<-.<-.<-.top;
					};
				};
			};
			Top {
				child_objs => {<-.<-.reset_xform_y.out_obj};
			};
		};
		Iscene2D_select  Iscene2D_right {
			View {
				ViewUI {
					ViewWindow {
						parent => <-.<-.<-.<-.right;
					};
				};
			};
			Top {
				child_objs => {<-.<-.reset_xform_x.out_obj};
			};
		};
		Iscene2D_select  Iscene2D_front {
			View {
				ViewUI {
					ViewWindow {
						parent => <-.<-.<-.<-.front;
					};
				};
			};
			Top {
				child_objs => {<-.<-.orthoslice_Z.out_obj};
			};
		};
		GDM.Iscene3D Iscene3D {
			ilink xform;
			View {
				ViewUI {
					ViewWindow {
						parent => <-.<-.<-.<-._3D;
					};
				};
			};
			Top {
				child_objs<NEportLevels={4,1}>;
				Top{
					xform+nres => <-.<-.xform;
				};
				Camera {
					Camera<NEdisplayMode="open"> {
						xform<NEdisplayMode="open">;
						from = {-0.,-0.,12.};
					};
				};
			};
			int+OPort2 clicked => View.View.mouse_events.buttonMask;
		};
		select_scene select_scene {
			right => <-.Iscene2D_right.clicked;
			front => <-.Iscene2D_front.clicked;
			top => <-.Iscene2D_top.clicked;
			_3D => <-.Iscene3D.clicked;
			parent => <-.UImod_panel;
		};
		ortho_panel{
			y => <-.ViewPanel.y + <-.ViewPanel.height + 5;
		};	

	};


//**********************************************************************

	macro DrawCropFunc {

		ilink+nres fieldin;
		group &params<NEportLevels={2,1}>;
		ilink+nres link;

		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS2 DrawCropAXIS2 {
			&params<NEportLevels={2,1}> => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 2;
			};
		};
		DataObject {
			obj<NEportLevels={1,4}>;
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS DrawCropAXIS1 {
			&params<NEportLevels={2,1}> => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 1;
			};
			DataObject {
				obj<NEportLevels={1,4}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS DrawCropAXIS {
			&params<NEportLevels={2,1}> => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 0;
			};
			DataObject {
				obj<NEportLevels={1,4}>;
			};
		};

		int+Port+nres int => Uviewer2D.Scene.View.View.selected;
		int+Port+nres int#1 => Uviewer2D.Scene#1.View.View.selected;
		int+Port+nres int#2 => Uviewer2D.Scene#2.View.View.selected;
		int+Port+nres int#3 => (int#1 + (int#2 * 2));

		GMOD.instancer instancer {
			Value => ((<-.int + <-.int#1) + <-.int#2);
			Group => <-.ContSketchViz;
		};
			
		

		DVM.DVswitch DVswitch {
			in => {
				<-.DrawCropAXIS2.DataObject.obj,<-.DrawCropAXIS1.DataObject.obj,
				<-.DrawCropAXIS.DataObject.obj};
			index => <-.int#3;
		};

		XP_COMM_PROJ.DrawCrop.DrawCropMods.cropdata cropdata {
			dims_in+nres => <-.<-.fieldin.dims;
			min_in => <-.ContSketchViz.min_vec;
			max_in => <-.ContSketchViz.max_vec;
		};

		XP_COMM_PROJ.DrawCrop.DrawCropMods.crop_coordinate crop_coordinate {
			set_coord => <-.cropdata.reset_coord;
			dims_in+nres => <-.<-.fieldin.dims;
			min_vec_axis2 => <-.cropdata.min_axis2;
			min_vec_axis1 => <-.cropdata.min_axis1;
			min_vec_axis0 => <-.cropdata.min_axis0;
			max_vec_axis2 => <-.cropdata.max_axis2;
			max_vec_axis1 => <-.cropdata.max_axis1;
			max_vec_axis0 => <-.cropdata.max_axis0;
			mins<NEportLevels={1,3}>;
			maxs<NEportLevels={1,3}>;
		};



		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropBox DrawCropBox2 {
			Cross2D {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate1_axis2;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis2;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropBox DrawCropBox1 {
			Cross2D {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate1_axis1;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis1;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropBox DrawCropBox{
			Cross2D {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate1_axis0;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis0;
					};
				};
				out_obj<NEportLevels={1,4}>;
			};
		};


		XP_COMM_PROJ.DrawCrop.DrawCropMacs.ContSketchViz ContSketchViz {
			in_view => <-.link;
			in_obj => <-.DVswitch.out;					 
			
		};
	};


//**************************************************************************

	

	macro DrawCrop {

		int+Port+nres Test2D => Uviewer2D.Scene.Top.instancer.active;
		GMOD.instancer instancer {
			Value => <-.Test2D;
			Group => <-;
		};
		int+Port+nres Test3D => Uviewer3D.Scene.Top.instancer.active;
		GMOD.print_error print_error {
			error+nres+notify =>	 Uviewer3D.Scene.Top.instancer.active;
			error_source = "DrawCrop";
			error_message = "You must use the Uviewer2D module for this application and the first three Scenes must connected to the first three outputs respectively. If you need a 3D viewer you can create a 3D Scene through the Editors.View menu";
			on_inst+nres+notify => Uviewer3D.Scene.Top.instancer.active;
		};
		int+Port+nres TestU => Uviewer.Scene.Top.instancer.active;
		GMOD.print_error print_error#1 {
			error => <-.TestU;
			error_source = "DrawCrop";
			error_message = "You must use the Uviewer2D module for this application. If you need a 3D viewer you can create a 3D Scene through the Editors.View menu";
		};

		ilink+nres fieldin;
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.parameters parameters {
			axis2+nres => (<-.fieldin.dims[2] / 2);
			axis1+nres => (<-.fieldin.dims[1] / 2);
			axis0+nres => (<-.fieldin.dims[0] / 2);
			zscale = 1.;
		};

		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropUI DrawCropUI {
			params => <-.parameters;
			infield => <-.fieldin;
			AXIS0_PLANE {
				value+nres => <-.params.axis0;
			};
			AXIS2_PLANE {
				value+nres => <-.params.axis2;
			};
			AXIS1_PLANE {
				value+nres => <-.params.axis1;
			};
		};

		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropFunc DrawCropFunc {
			fieldin => <-.fieldin;
			params => <-.parameters;
			link<NEportLevels={3,1}>;
			DrawCropAXIS2 {
				DVorthoslice_unif {
					plane+nres => <-.params.axis2;
				};
			};
			DrawCropAXIS1 {
				DVorthoslice_unif {
					plane+nres => <-.params.axis1;
				};
				DVscale {
					scale_y+nres => <-.params.zscale;
				};
			};
			DrawCropAXIS {
				DVorthoslice_unif {
					plane+nres => <-.params.axis0;
				};
				DVscale {
					scale_y+nres => <-.params.zscale;
					out;
				};
			};
			cropdata {
				set_coord => <-.<-.DrawCropUI.crop_controls.set_coord;
				which_view => <-.int#3;
			};

			crop_coordinate {
				dims_in+nres => <-.fieldin.dims;
				z_scale+nres => <-.params.zscale;
			};
		};

		GDM.GroupObject Principal {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS2.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox2.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox2.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};
		GDM.GroupObject Secondary {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS1.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox1.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox1.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};
		GDM.GroupObject Tertiary {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};

		DVM.DVcrop_unif DVcrop_unif {
			in => <-.fieldin;
			min => <-.DrawCropFunc.crop_coordinate.mins;
			max => <-.DrawCropFunc.crop_coordinate.maxs;
		};
		DVM.DVscale DVscale {
			in => <-.DVcrop_unif.out;
			scale_z => <-.parameters.zscale;
		};		 
		GDM.DataObject	DataObject {
			in => <-.DVscale.out;
		};

		olink+nres _3DFld => DVscale.out;
		olink+nres _3DDataObject => DataObject.obj;
	};

	macro DrawCropView {
	
		int+nres Test2D => Uviewer2D.Scene.Top.instancer.active;
		ilink+nres fieldin;
		ilink curr_view;
		Xform+nres &Xform;
		GDM.GroupObject in_3D {
			child_objs<NEportLevels={3,1}>;
		};
		GMOD.instancer instancer {
			Value => <-.Test2D;
			Group => <-;
		};
		int+nres Test3D<NEportLevels=1> => Uviewer3D.Scene.Top.instancer.active;
		GMOD.print_error print_error {
			error+notify+nres => Uviewer3D.Scene.Top.instancer.active;
			error_source = "DrawCrop";
			error_message = "You must use the Uviewer2D module for this application and the first three Scenes must connected to the first three outputs respectively. If you need a 3D viewer you can create a 3D Scene through the Editors.View menu";
			on_inst+notify+nres => Uviewer3D.Scene.Top.instancer.active;
		};
		int+nres TestU<NEportLevels=1> => Uviewer.Scene.Top.instancer.active;
		GMOD.print_error print_error#1 {
			error => <-.TestU;
			error_source = "DrawCrop";
			error_message = "You must use the Uviewer2D module for this application. If you need a 3D viewer you can create a 3D Scene through the Editors.View menu";
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.parameters parameters {
			axis2+nres => (<-.fieldin.dims[2] / 2);
			axis1+nres => (<-.fieldin.dims[1] / 2);
			axis0+nres => (<-.fieldin.dims[0] / 2);
			zscale = 1.;
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropFunc DrawCropFunc {
			fieldin => <-.fieldin;
			params => <-.parameters;
			link<NEportLevels={2,1}> => <-.curr_view;
			DrawCropAXIS2 {
				DVorthoslice_unif {
					plane+nres => <-.params.axis2;
				};
			};
			DrawCropAXIS1 {
				DVorthoslice_unif {
					plane+nres => <-.params.axis1;
				};
				DVscale {
					scale_y+nres => <-.params.zscale;
				};
			};
			DrawCropAXIS {
				DVorthoslice_unif {
					plane+nres => <-.params.axis0;
				};
				DVscale {
					scale_y+nres => <-.params.zscale;
				};
			};
			cropdata {
				which_view => <-.int#3;
			};
			crop_coordinate {
				dims_in => <-.fieldin.dims;
				z_scale+nres => <-.params.zscale;
			};
		};
		GDM.GroupObject Principal {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS2.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox2.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox2.Cross2D#1.out_obj};
			obj<NEportLevels={1,3}>;
		};
		GDM.GroupObject Secondary {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS1.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox1.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox1.Cross2D#1.out_obj};
			obj<NEportLevels={1,3}>;
		};
		GDM.GroupObject Tertiary {
			child_objs => {
				<-.DrawCropFunc.DrawCropAXIS.DataObject.obj,
				<-.DrawCropFunc.DrawCropBox.Cross2D.out_obj,
				<-.DrawCropFunc.DrawCropBox.Cross2D#1.out_obj};
			obj<NEportLevels={1,3}>;
		};
		DVM.DVcrop_unif DVcrop_unif {
			in => <-.fieldin;
			min => <-.DrawCropFunc.crop_coordinate.mins;
			max => <-.DrawCropFunc.crop_coordinate.maxs;
		};
		DVM.DVscale DVscale {
			in => <-.DVcrop_unif.out;
			scale_z => <-.parameters.zscale;
		};
		GDM.DataObject DataObject {
			in => <-.DVscale.out;
		};
		olink+nres _3DFld => DVscale.out;
		olink+nres _3DDataObject => DataObject.obj;
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropViewUI DrawCropViewUI {
			infield => <-.fieldin;
			params => <-.parameters;
			Iscene3D {
				xform => <-.<-.Xform;
				Top {
					child_objs => {<-.<-.<-.in_3D.obj};
				};
			};
		};
	};




};

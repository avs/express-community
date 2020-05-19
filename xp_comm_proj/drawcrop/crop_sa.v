flibrary DrawCropStandAlone {
//*******list of the functional macros used in this module
 
	  //ContinuousSketch
	  //SA_DC_macro
	  //StandAloneDrawCrop_Eg

//***********************************************************************

	GDM.ContinuousSketch ContinuousSketch {
		macro cont_sketch<module_stack_menu=1,NEhelpTopic="old_ContinuousSketch"> {
			ilink+nres rv<NEcolor0=65280,NEportLevels={3,1}>;
			GDview_templ &view<NEportLevels={2,1}> => <-.view_in;
			UItwoPoint UItwoPoint {
				view => <-.rv;
				startEvent = "<Btn3Down>";
				runEvent = "<Btn3Motion>";
				stopEvent = "<Btn3Up>";
			};
			GDroi2d_cont GDroi2d_cont {
				state => <-.UItwoPoint.state;
				redraw+nres => view.trigger;
				view_in => view;
				x => <-.UItwoPoint.x;
				y => <-.UItwoPoint.y;
				clear = 1;
				done = 1;
				immed = "On Button Up";
				option = "Box";
			};
			GDmap2d_ss GDmap2d_ss {
				in_buf => <-.GDroi2d_cont.out_buf;
				view_in => <-.view;
				cam_in => view.picked_camera;
				obj_in+nres<NEportLevels={3,0}> => <-.<-.obj_in;
				out_mesh<NEportLevels={0,4}> {
					coordinates {
						min_vec<NEportLevels={0,6}>;
						max_vec<NEportLevels={0,6}>;
					};
				};
			};
		};
	};



//***********************************************************************



  //SA_DC_macro

	macro SA_DC_macro {
		ilink+nres fieldin;
		group &params<NEportLevels={2,1}>;
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS2 DrawCropAXIS2 {
			params => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 2;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS DrawCropAXIS1 {
			params => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 1;
			};
			DataObject {
				obj<NEportLevels={1,3}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropAXIS DrawCropAXIS {
			params => <-.params;
			DVorthoslice_unif {
				in => <-.<-.fieldin;
				axis = 0;
			};
			DataObject {
				obj<NEportLevels={1,3}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropStandAlone.ContinuousSketch ContinuousSketch {
			view_in<NEportLevels={3,1}>;
			obj_in => <-.DrawCropAXIS.DataObject.obj;
			cont_sketch {
				rv<NEportLevels={4,1}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropStandAlone.ContinuousSketch ContinuousSketch#1 {
			view_in<NEportLevels={3,1}>;
			obj_in => <-.DrawCropAXIS1.DataObject.obj;
			cont_sketch {
				rv<NEportLevels={4,1}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropStandAlone.ContinuousSketch ContinuousSketch#2 {
			view_in<NEportLevels={3,1}>;
			obj_in => <-.DrawCropAXIS2.DataObject.obj;
			cont_sketch {
				rv<NEportLevels={4,1}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMods.crop_mouse crop_mouse {
			dims_in+nres => <-.fieldin.dims;
			set_coord<NEportLevels={3,1}>;
			min_vec_axis2_cs => <-.ContinuousSketch#2.cont_sketch.GDmap2d_ss.out_mesh.coordinates.min_vec;
			min_vec_axis1_cs => <-.ContinuousSketch#1.cont_sketch.GDmap2d_ss.out_mesh.coordinates.min_vec;
			min_vec_axis0_cs => <-.ContinuousSketch.cont_sketch.GDmap2d_ss.out_mesh.coordinates.min_vec;
			max_vec_axis2_cs => <-.ContinuousSketch#2.cont_sketch.GDmap2d_ss.out_mesh.coordinates.max_vec;
			max_vec_axis1_cs => <-.ContinuousSketch#1.cont_sketch.GDmap2d_ss.out_mesh.coordinates.max_vec;
			max_vec_axis0_cs => <-.ContinuousSketch.cont_sketch.GDmap2d_ss.out_mesh.coordinates.max_vec;
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMods.crop_coordinate crop_coordinate {
			set_coord => <-.crop_mouse.reset_coord;
			dims_in+nres => <-.fieldin.dims;
			z_scale+nres => <-.params.z_scale;
			min_vec_axis2 => <-.crop_mouse.min_vec_axis2;
			min_vec_axis1 => <-.crop_mouse.min_vec_axis1;
			min_vec_axis0 => <-.crop_mouse.min_vec_axis0;
			max_vec_axis2 => <-.crop_mouse.max_vec_axis2;
			max_vec_axis1 => <-.crop_mouse.max_vec_axis1;
			max_vec_axis0 => <-.crop_mouse.max_vec_axis0;
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
				out_obj<NEportLevels={1,3}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis2;
					};
				};
				out_obj<NEportLevels={1,3}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropBox DrawCropBox1 {
			Cross2D {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate1_axis1;
					};
				};
				out_obj<NEportLevels={1,3}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis1;
					};
				};
				out_obj<NEportLevels={1,3}>;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropBox DrawCropBox {
			Cross2D {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate1_axis0;
					};
				};
				out_obj<NEportLevels={1,3}>;
			};
			Cross2D#1 {
				cross {
					coordinates {
						values => <-.<-.<-.<-.crop_coordinate.xlate2_axis0;
					};
				};
				out_obj<NEportLevels={1,3}>;
			};
		};
		GroupObject GroupObjectAXIS2 {
			child_objs => {
				<-.DrawCropAXIS2.DataObject.obj,
				<-.DrawCropBox2.Cross2D.out_obj,
				<-.DrawCropBox2.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};
		GroupObject GroupObjectAXIS1 {
			child_objs => {
				<-.DrawCropAXIS1.DataObject.obj,
				<-.DrawCropBox1.Cross2D.out_obj,
				<-.DrawCropBox1.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};
		GroupObject GroupObjectAXIS0 {
			child_objs => {
				<-.DrawCropAXIS.DataObject.obj,
				<-.DrawCropBox.Cross2D.out_obj,
				<-.DrawCropBox.Cross2D#1.out_obj
			};
			obj<NEportLevels={1,3}>;
		};
	};

//***********************************************************************

	APPS.MultiWindowApp StandAloneDrawCrop_Eg {
		XP_COMM_PROJ.DrawCrop.DrawCropStandAlone.SA_DC_macro SA_DC_macro {
			fieldin => <-.link_field;
			params => <-.parameters;
			ContinuousSketch {
				view_in => <-.<-.Iscene2D#2.View.View;
				cont_sketch {
					rv => <-.<-.<-.Iscene2D#2.View.ViewUI.ViewWindow;
				};
			};
			ContinuousSketch#1 {
				view_in => <-.<-.Iscene2D#1.View.View;
				cont_sketch {
					rv => <-.<-.<-.Iscene2D#1.View.ViewUI.ViewWindow;
				};
			};
			ContinuousSketch#2 {
				view_in => <-.<-.Iscene2D.View.View;
				cont_sketch {
					rv => <-.<-.<-.Iscene2D.View.ViewUI.ViewWindow;
				};
			};
			crop_mouse {
				set_coord => <-.<-.DrawCropUI.crop_controls.set_coord;
			};
			crop_coordinate {
				z_scale => <-.params.zscale;
			};
			DrawCropAXIS2 {
				DVorthoslice_unif {
					plane => <-.params.axis2;
				};
				xform_to_plane {
					DVxform_to_plane {
						DVcopy_xform_field {
							out_xform {
								xform {
									dcenter = {0.,0.,0.};
									ocenter = {0.,0.,0.};
								};
							};
						};
					};
				};
			};
			DrawCropAXIS1 {
				DVorthoslice_unif {
					plane => <-.params.axis1;
				};
				xform_to_plane {
					DVxform_to_plane {
						DVcopy_xform_field {
							out_xform {
								xform {
									dcenter = {0.,0.,0.};
									ocenter = {0.,0.,0.};
								};
							};
						};
					};
				};
			};
			DrawCropAXIS {
				DVorthoslice_unif {
					plane => <-.params.axis0;
				};
				xform_to_plane {
					DVxform_to_plane {
						DVcopy_xform_field {
							out_xform {
								xform {
									dcenter = {0.,0.,0.};
									ocenter = {0.,0.,0.};
								};
							};
						};
					};
				};
			};
		};
		GDM.Iscene2D Iscene2D {
			Top {
				child_objs => {
					<-.<-.SA_DC_macro.GroupObjectAXIS2.obj
				};
				Xform {
					ocenter = {59.5,59.5,0.};
					dcenter = {-51.5,-51.5,0.};
				};
			};
			View {
				View {
					trigger = 1;
				};
				ViewUI {
					ViewWindow<NEx=22.,NEy=110.,NEportLevels={0,4}> {
						parent => <-.<-.<-.<-.UIshell;
					};
				};
			};
		};
		UIshell UIshell {
			x = 82;
			y = 90;
		};
		UIshell UIshell#1 {
			x = 82;
			y = 411;
		};
		GDM.Iscene2D Iscene2D#1 {
			View {
				View {
					trigger = 1;
				};
				ViewUI {
					ViewWindow<NEportLevels={0,4}> {
						parent => <-.<-.<-.<-.UIshell#1;
					};
				};
			};
			Top {
				child_objs => {
					<-.<-.SA_DC_macro.GroupObjectAXIS1.obj
				};
				Xform {
					ocenter = {59.5,16.5,0.};
					dcenter = {-51.5,-14.2815,0.};
				};
			};
		};
		GDM.Iscene2D Iscene2D#2 {
			View {
				View {
					trigger = 1;
				};
				ViewUI {
					ViewWindow<NEportLevels={0,4}> {
						parent => <-.<-.<-.<-.UIshell#2;
					};
				};
			};
			Top {
				child_objs => {
					<-.<-.SA_DC_macro.GroupObjectAXIS0.obj};
				Xform {
					ocenter = {59.5,16.5,0.};
					dcenter = {-51.5,-14.2815,0.};
				};
			};
		};
		UIshell UIshell#2 {
			x = 150;
			y = 251;
		};
		UIshell UIshell#3;
		DVcrop_unif DVcrop_unif {
			min => <-.SA_DC_macro.crop_coordinate.mins;
			in => <-.link_field;
			max => <-.SA_DC_macro.crop_coordinate.maxs;
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.DrawCropUI DrawCropUI {
			infield => <-.link_field;
			params => <-.parameters;
			AXIS2_PLANE {
				value => <-.params.axis2;
			};
			AXIS1_PLANE {
				value => <-.params.axis1;
			};
			AXIS0_PLANE {
				value => <-.params.axis0;
			};
		};
		XP_COMM_PROJ.DrawCrop.DrawCropMacs.parameters parameters {
			axis2 => (<-.link_field.dims[2] / 2);
			axis1 => (<-.link_field.dims[1] / 2);
			axis0 => (<-.link_field.dims[0] / 2);
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
				set = 1;
					};
					height = 984;
				};
				file_browser {
					x = 490;
					y = 267;
					width = 300;
					height = 390;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		link link_field => .Read_Field.field;
		GDM.Iscene3D Iscene3D {
			Top {
				child_objs => {
					<-.<-.isosurface.out_obj};
			};
			View {
				ViewUI {
					ViewWindow {
						parent => <-.<-.<-.<-.UIshell#3;
					};
				};
			};
		};
		MODS.isosurface isosurface {
			in_field => <-.DVcrop_unif.out;
			DVcell_data_labels {
				labels[];
			};
			UIpanel {
				option {
					set = 1;
				};
			};
		};
	};
};

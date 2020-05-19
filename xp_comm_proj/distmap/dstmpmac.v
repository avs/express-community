
flibrary DistanceMapMacs <compile_subs=0> {

	macro DistanceMapSegmentationUI {
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.distmapParams+OPort &distmap_params;
		UImod_panel DistanceMapSegmentation{
#ifndef MSDOS
			width => parent.clientWidth-8;
#endif
		};
		UIframe SliceFrame {
			y = 0;
			width => parent.clientWidth;
			parent => <-.DistanceMapSegmentation;
			height => <-.UIradioBoxLabel.height+5;
		};
		UIslider slice {
			x => parent.clientWidth/3;
			y => 5;
			width => (2 * parent.clientWidth/3) - 5;
			parent => <-.SliceFrame;
			max+nres => (<-.distmap_params.pt_params.dims[<-.distmap_params.pt_params.axis] - 1);
			value => <-.distmap_params.pt_params.plane;
			mode = "integer";
		};
		string+Port labels[] = {"x","y","z"};
		UIradioBoxLabel UIradioBoxLabel{
			parent => <-.SliceFrame;
			labels => <-.labels;
			selectedItem => <-.distmap_params.pt_params.axis;
			width => (.parent.clientWidth/3) - 5;
			UIlabel UIlabel;
			UIradioBox {
				x = 5;
				y => 0;
			};
		};
		UIframe UIframe {
			y => <-.SliceFrame.y +  <-.SliceFrame.height+5;
			width => parent.clientWidth;
			parent => <-.DistanceMapSegmentation;
			height => <-.tolerance.y + <-.tolerance.height+5;
		};
		UIbutton init_mask {
			x => 5;
			y => <-.Seg_modes.y;
			width => (parent.clientWidth/2) - 10;
			height => (<-.Seg_modes.height/3);
			parent => <-.UIframe;
			do => <-.distmap_params.fill_params.init;
		};
		UIbutton reset {
			x => 5;
			y => <-.init_point0.y + <-.init_point0.height;
			width =>  (parent.clientWidth/2) - 10;
			height => (<-.Seg_modes.height/3);
			parent => <-.UIframe;
			do => <-.distmap_params.fill_params.reset;
		};
		UIbutton init_point0 {
			x => 5;
			y => <-.init_mask.y + <-.init_mask.height;
			width =>  (parent.clientWidth/2) - 10;
			height => (<-.Seg_modes.height/3);
			parent => <-.UIframe;
			label => "init points";
			do => <-.distmap_params.pt_init_params.init_points0;
		};
		UIslider mask_number {
			x = 5;
			y => <-.Seg_modes.y + <-.Seg_modes.height + 5;
			width => parent.clientWidth-10;
			parent => <-.UIframe;
			min = 1.;
			max = 100.;
			value => <-.distmap_params.fill_params.mask_num;
			mode = "integer";
		};
		UIslider tolerance {
			x = 5;
			y => <-.mask_number.y + <-.mask_number.height + 5;
			width => parent.clientWidth-10;
			parent => <-.UIframe;
			min = 1.;
			max = 1000.;
			value => <-.distmap_params.fill_params.tolerance;
			mode = "integer";
		};
		string+Port seg_modes[] => {"replace","add","intersect","subtract"};
		UIradioBoxLabel Seg_modes {
			parent => <-.UIframe;
			labels => <-.seg_modes;
			selectedItem => <-.distmap_params.fill_params.fill_mode;
			width => (parent.clientWidth/2)-10;
			x => (parent.clientWidth/2+5);
			y = 5;
			UIlabel UIlabel;
			UIradioBox {
				y => 0;
			};
		};
		UIframe UIframe#1 {
			y => <-.UIframe.y +  <-.UIframe.height+5;
			width => parent.clientWidth;
			parent => <-.DistanceMapSegmentation;
			height => <-.smooth_out.y + <-.smooth_out.height + 10;
		};
		UIlabel Pick_Value {
			y = 5;
			x = 5;
			width => (2*parent.clientWidth/3)-10;
			parent => <-.UIframe#1;
		};
		UIfield UIfield {
			y = 5;
			x => (2*parent.clientWidth/3) + 5;
			width => (parent.clientWidth/3)-10;
			parent => <-.UIframe#1;
			value => <-.distmap_params.fill_params.fill_val;
		};
		UIslider cutoff {
			x = 5;
			y => <-.UIfield.y + <-.UIfield.height + 5;
			parent => <-.UIframe#1;
			min = 0.;
			max = 255.;
			width => parent.clientWidth-10;
			value => <-.distmap_params.out_params.t1;
			mode = "integer";
		};
		UIslider smooth {
			x = 5;
			y => <-.cutoff.y + <-.cutoff.height + 5;
			parent => <-.UIframe#1;
			width => parent.clientWidth-10;
			min = 0.;
			max = 255.;
			value => <-.distmap_params.out_params.t2;
			mode = "integer";
		};
		UIbutton do {
			width => (parent.clientWidth/3)-10;
			height => <-.smooth_out.height;
			x => (2*parent.clientWidth/3) + 5;
			y => <-.smooth.y + <-.smooth.height + 5;
			parent => <-.UIframe#1;
			do => <-.distmap_params.out_params.do;
		};
		string+Port smooth_modes[] => {"no smoothing","smooth diam 3","smooth diam 5"};
		UIradioBoxLabel smooth_out {
			width => (2*parent.clientWidth/3)-10;
			x = 5;
			y => <-.do.y;
			parent => <-.UIframe#1;
			selectedItem => <-.distmap_params.out_params.smooth;
			labels => <-.smooth_modes;
			UIlabel UIlabel;
			UIradioBox {
				y => 0;
			};
		};
	};

	macro distance_map_slice {
		Mesh_Unif+Node_Data+IPort2 &in;
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.pt_params+IPort2 &pt_params;
		DVM.DVorthoslice_unif slicer {
			in => <-.in;
			axis => <-.pt_params.axis;
			plane => <-.pt_params.plane;
		};
		FLD_MAP.uniform_scalar_field slice {
			mesh {
				in_dims => <-.<-.slicer.out.dims;
			};
			data {
				in_data => <-.<-.slicer.out.node_data[0].values;
			};
		};
		prim+OPort2 val => slicer.out.node_data[0].values[slice.obj.pick_info.pick_data[0].verti][0];
		GMOD.copy_on_change fill_val {
			input => <-.val;
		};
		olink out => slice.out;
		olink obj => slice.obj;
	};

	macro DistanceMapSegmentationFunc {
		Mesh_Unif+Node_Data+IPort2 &in_volume {
			ndim = 3;
			nnode_data = 1;
			node_data {
				veclen = 1;
				byte values[.nvals][.veclen];
				byte+opt null_value;
				byte min;
				byte max;
				byte min_vec[.veclen];
				byte max_vec[.veclen];
			};
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.distmapParams+OPort &distmap_params;
		int+OPort init_point0 [100][3] = init_array(300,-1,-1);		
		XP_COMM_PROJ.DistanceMap.DistanceMapMods.init_points init_points {
			in_buf => <-.in_volume;
			par => <-.distmap_params.pt_init_params;
			init_point0 => <-.init_point0;
			init_point1 => <-.init_point0;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMods.reset_all reset_all {
			in_buf => <-.in_volume;
			par => <-.distmap_params;
			mask => <-.mask_fld.mask_array;
			init_point0 => <-.init_point0;
			init_point1 => <-.init_point0;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMods.init_mask init_mask {
			par => <-.distmap_params;
			mask => <-.mask_fld.mask_array;
		};
		macro mask_fld {
			Mesh_Unif+Node_Data+IPort2 &in => <-.in_volume;
			XP_COMM_PROJ.DistanceMap.DistanceMapMods.create_outm create_outm {
				in => <-.in;
			};
			FLD_MAP.uniform_scalar_field uniform_scalar_field {
				mesh {
					in_dims => <-.<-.in.dims;
				};
				data {
					in_data => <-.<-.create_outm.mask_array;
				};
			};
			olink mask_array => create_outm.mask_array;
			olink out => uniform_scalar_field.out;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMods.dis_fill dis_fill {
			par => <-.distmap_params.fill_params;
			mask => <-.mask_fld.mask_array;
			in => <-.in_volume;
			init_point0 => <-.init_point0;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMods.create_outf create_outf {
			par => <-.distmap_params.out_params;
			in => <-.in_volume;
			mask_fld => <-.dis_fill.out;
			mask_array => <-.mask_fld.mask_array;
		};
		olink out_field => create_outf.out;
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.distance_map_slice data_slice {
			in => <-.in_volume;
			pt_params => <-.distmap_params.pt_params;
			DVM.DVorthoslice_unif slicer {
				in => <-.in;
				axis => <-.pt_params.axis;
				plane => <-.pt_params.plane;
				fill_val {
					output => <-.<-.distmap_params.fill_params.fill_val;
				};
			};
			XP_COMM_PROJ.DistanceMap.DistanceMapMods.add_points add_points {
				par => <-.pt_params;
				switch = 0;
				init_point0 => <-.<-.init_point0;
				init_point1 => <-.<-.init_point0;
			};
			slice {
				DataObject {
					Obj {
						xform_mode = "Parent";
					};
				};
			};
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.distance_map_slice fill_slice {
			in => <-.dis_fill.out;
			pt_params => <-.distmap_params.pt_params;
		};
		olink fill_obj => fill_slice.obj;
		GMOD.instancer instancer {
			Value => is_valid(dis_fill.out.node_data[0].max);
			Group => isoline;
		};
		MODS.isoline isoline {
		    macro UIpanel;
			macro IsoLineUI;
			DVisoline {
				component = 0;
				level => {<-.<-.distmap_params.out_params.t1};
			};
			in_field => <-.fill_slice.out;
			IsoParam.color = 0;
			instancer.Value => is_valid(in_field.node_data[contour_comp].min);
			instancer.Group => IsoParam;
		};
		GDM.DataObjectLite DataObjectLite {
			in => <-.isoline.out_fld;
			Obj {
				xform_mode = "Parent";
			};
		};
		GDM.GroupObject GroupObject {
			child_objs => {<-.data_slice.obj,
				<-.DataObjectLite.obj};
		};
		olink data_obj => GroupObject.obj;
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.distance_map_slice res_slice {
			in => <-.create_outf.out;
			pt_params => <-.distmap_params.pt_params;
		};
		olink res_obj => res_slice.obj;
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.distance_map_slice mask_slice {
			in => <-.mask_fld.out;
			pt_params => <-.distmap_params.pt_params;
		};
		olink mask_obj => mask_slice.obj;
	};		
	
	macro DistanceMapSegmentation {
		Mesh_Unif+Node_Data+IPort2 &in_volume {
			ndim = 3;
			nnode_data = 1;
			node_data {
				veclen = 1;
				byte values[.nvals][.veclen];
				byte+opt null_value;
				byte min;
				byte max;
				byte min_vec[.veclen];
				byte max_vec[.veclen];
			};
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.distmapParams distmap_params {
			fill_params.tolerance = 100;
			fill_params.margin_width = 1;
			fill_params.init = 0;
			fill_params.reset = 0;
			fill_params.fill_mode = 0;
			fill_params.fill_val = 0;
			fill_params.mask_num = 1;
			out_params.t1 = 100;
			out_params.mask_num => <-.fill_params.mask_num;
			out_params.init => <-.fill_params.fill_mode;
			out_params.smooth = 1;
			pt_params.point => <-.<-.DistanceMapSegmentationFunc.data_slice.slice.DataObject.PickInfo.pick_data[0].vert;
			pt_params.axis = 2;
			pt_params.dims => <-.<-.in_volume.dims;
			pt_params.plane => (<-.<-.in_volume.dims[axis] / 2);
			pt_init_params.init_points0 = 0;
			pt_init_params.init_points1 = 0;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.DistanceMapSegmentationFunc DistanceMapSegmentationFunc {	
			in_volume => <-.in_volume;
            distmap_params => <-.distmap_params;
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.DistanceMapSegmentationUI DistanceMapSegmentationUI {	
			distmap_params => <-.distmap_params;
		};
		olink out_field => DistanceMapSegmentationFunc.out_field;
		olink fill_obj => DistanceMapSegmentationFunc.fill_obj;
		olink data_obj => DistanceMapSegmentationFunc.data_obj;
		olink res_obj => DistanceMapSegmentationFunc.res_obj;
		olink mask_obj => DistanceMapSegmentationFunc.mask_obj;
	};

	macro DistanceMapSegmentationDialog {
		ilink in;
		int+IPort2 visible;
		macro UI<hconnect=2>{
			AU.AUtoolbar toolbar<hconnect=2> {
				x = 0;
				y = 0;
				height = 27;
				parent => <-.toolframe;
				tip_placement = 0;
			};
			UIshell DistanceSegEditor {
				x = 6;
				y = 215;
				visible => <-.<-.visible;
                title => name_of(<-.<-.<-,1);
				width = 1206;
				height = 819;
			};
			UIscrolledWindow UIscrolledWindow {
				parent => <-.UIframe;
				virtualWidth => switch(((300 < .clientWidth) + 1),300,.clientWidth);
				virtualHeight => 1500;
				width => parent.clientWidth;
				height => parent.clientHeight;
			};
			UIframe outputframe {
				parent => <-.DistanceSegEditor;
				y => (<-.toolframe.y + <-.toolframe.height);
				width => (parent.clientWidth - x);
				height => (parent.clientHeight - .y);
				x => <-.toolframe.x;
			};
			UIframe UIframe {
				parent => <-.DistanceSegEditor;
				y => outputframe.y;
				width => (parent.clientWidth / 3);
				height => (parent.clientHeight - .y);
			};
			UIframe toolframe {
				parent => <-.DistanceSegEditor;
				y = 0;
				x => (<-.UIframe.x + <-.UIframe.width);
				width => (parent.clientWidth - x);
				height = 33;
			};
			UIframe titleframe {
				parent => <-.DistanceSegEditor;
				y = 0;
				width => <-.UIframe.width;
				height => <-.toolframe.height;
			};
			UIlabel UIlabel {
				parent => <-.titleframe;
				label => <-.<-.Uviewer2D.scene_labels[<-.<-.Uviewer2D.selectded_view];
				y => (parent.clientHeight - height);
				width => parent.clientWidth;
			};
		};
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.DistanceMapSegmentation DistanceMapSegmentation {
			in_volume => <-.in;
			distmap_params.fill_params {  
				reset=0;
				tolerance = 280;
			};
			distmap_params.out_params {
				t1 = 158;
				smooth = 0;
			};
			DistanceMapSegmentationUI {
				UIframe DistanceMapSegmentation {
					parent => <-.<-.<-.UI.UIscrolledWindow;
#ifdef MSDOS
					width => parent.virtualWidth-15;
#else 
					width => parent.virtualWidth;
#endif
					height => 1500;
				};
			};
			MODS.bounds bounds {
				macro UIpanel{
				  int visible=1;
				};
				in_field => <-.out_field;
			};
			MODS.volume_render volume_render {
			    macro UImod_panel;
				DataObject.Modes.mode => {0,0,0,switch(((<-.<-.<-.<-.Uviewer2D.Scene#1.View.View.renderer==0)+1),2,3),0};
				in_mesh => <-.out_field;
				DataObject {
					VolRenderDatamap {
						DataRange = {
							{
								selectAlphaRange=0,,,,,,,
							},
						};
						DatamapValue = {,
							{v1=0.06},
							{v1=0.09},
							{v1=0.36}
						};
					};
					Props {
						fat_ray = 0;
					};
				};
			};
		};
		GDM.Uviewer2D Uviewer2D {
			int selectded_view => Scene.View.View.selected+2*Scene#1.View.View.selected+3*Scene#2.View.View.selected+4*Scene#3.View.View.selected+5*Scene#4.View.View.selected;
			string scene_labels[6] =>{"DistanceMapSegmentation","Input","Scene","Fill","Mask","Result"}; 
			GDM.Uscene2D Scene {
				Top {
					child_objs => {
						<-.<-.<-.DistanceMapSegmentation.data_obj};
				};
				View {
					ViewUI {
						ViewPanel {
							 UI{ 
						 		panel {
									width => ((parent.width <+> dw)*2 / 3);
									parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
									height => ((parent.height <+> dh) / 2);
								};
							};
						};
					};
				};
			};
			Scene_Selector {
				input_views => {<-.Scene.View.View,<-.Scene#1.View.View,<-.Scene#2.View.View,
				  <-.Scene#3.View.View,<-.Scene#4.View.View};
			};
			GDM.Uscene3D Scene#1 {
				Top {
					child_objs => {
						<-.<-.<-.DistanceMapSegmentation.volume_render.out_obj,
						<-.<-.<-.DistanceMapSegmentation.bounds.out_obj
					};
				};
				View {
					View {
					    //renderer => <-.<-.<-.<-.DistanceMapSegmentation.volume_render.datamap_ui.Dmap2ImageLegend.Scene.View.View.renderer;
					};
					ViewUI {
						ViewPanel {
							UI {
								panel {
								    y => ((parent.height <+> dh) / 2);
									parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
									width => ((parent.width <+> dw)*2 / 3);
									height => ((parent.height <+> dh) / 2);
								};
							};
						};
					};
				};
			};
			GDM.Uscene2D Scene#2 {
				Top {
					child_objs => {
						<-.<-.<-.DistanceMapSegmentation.fill_obj};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								panel {
									x => ((parent.width <+> dw)*2 / 3);
									width => ((parent.width <+> dw) / 3);
									height => ((parent.height <+> dh) / 3);
									parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
								};
							};
						};
					};
				};
			};
			GDM.Uscene2D Scene#3 {
				Top {
					child_objs => {
						<-.<-.<-.DistanceMapSegmentation.mask_obj};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								panel {
									x => ((parent.width <+> dw)*2 / 3);
									y => ((parent.height <+> dh) / 3); 
									width => ((parent.width <+> dw) / 3);
									height => ((parent.height <+> dh) / 3);
									parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
								};
							};
						};
					};
				};
			};
			GDM.Uscene2D Scene#4 {
				Top {
					child_objs => {
						<-.<-.<-.DistanceMapSegmentation.res_obj};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								panel {
									x => ((parent.width <+> dw)*2 / 3);
									y => ((parent.height <+> dh)*2 / 3);
									width => ((parent.width <+> dw) / 3);
									height => ((parent.height <+> dh) / 3);
									parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
								};
							};
						};
					};
				};
			};
		};
		olink out => DistanceMapSegmentation.out_field;
		olink obj => DistanceMapSegmentation.volume_render.out_obj;
	};

};

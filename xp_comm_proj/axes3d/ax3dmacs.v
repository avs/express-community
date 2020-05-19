flibrary Axes3DMacs <compile_subs=0> {
	macro Axes3DUI {
		int+Port ind[] => {
			1,2,0,2,0,1
		};
		string+Port axe_names[] => {"x axis","y axis",
			"z axis"};
		string+Port plane_names[] => {"xy plane",
			"xz plane","yz plane"};
		Root.UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
			option {
				set = 1;
			};
		};
		int+Port h = 55;
		int+Port a => ((.h / 2) - 5);
		UIframe gen_frame {
			y = 0;
			width => parent.width;
			parent => <-.UImod_panel;
			height => ((5 * <-.h) + 10);
		};
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_mod_params+IPort2 &params;
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_geom_params+IPort2 &geom_params;
		UItoggle auto_grid {
			parent => <-.gen_frame;
			width = 70;
			y = 0;
		};
		UIslider label_density {
			parent => <-.gen_frame;
			value => <-.params.n_labels;
			y = 0;
			x = 70;
			width => ((parent.width - .x) - 7);
			visible => <-.auto_grid.set;
			min = 5.;
			max = 100.;
			mode = "integer";
		};
		UIpanel grid_panel {
			parent => <-.gen_frame;
			y = 0;
			x = 70;
			width => ((parent.width - .x) - 7);
			height => <-.h;
			visible => (1 - <-.auto_grid.set);
		};
		UIslider label_height {
			parent => <-.gen_frame;
			value => <-.geom_params.h;
			y => (4 * <-.h);
			min = 5.;
			max = 100.;
			mode = "integer";
		};
		macro axis_params[3] {
			int+Port k => index_of(<-.axis_params);
			UIlabel gridlabel {
				parent => <-.<-.grid_panel;
				label => <-.<-.axe_names[<-.k];
				y = 0;
				width => (parent.width / 3);
				x => (<-.k * .width);
			};
			UIfield UIfield {
				parent => <-.<-.grid_panel;
				y = 24;
				width => ((parent.width / 3) - 2);
				x => (<-.k * <-.gridlabel.width);
				value => <-.<-.params.intervals[<-.k];
			};
			UIframe axis_frame {
				parent+IPort3 => <-.<-.UImod_panel;
				x => (<-.k * 83);
				y => (<-.<-.gen_frame.height + 5);
				width = 80;
				height => (14 * <-.<-.a);
			};
			XP_COMM_PROJ.Axes3D.Axes3DMods.axes_geom_params+IPort2 &geom_params => <-.geom_params;
			XP_COMM_PROJ.Axes3D.Axes3DMods.axes_mod_params+IPort2 &params => <-.params;
			macro axe_pos {
				ilink parent => <-.axis_frame;
				string+OPort labels[] => {
					("in " + <-.<-.plane_names[ind[(2 * <-.k)]]),
					("in " + <-.<-.plane_names[ind[((2 * <-.k) + 1)]])};
				int+IPort2 selectedItem => <-.params.pos[(2 * <-.k)];
				string title => <-.<-.axe_names[<-.k];
				int visible = 1;
				int active = 1;
				int x = 0;
				int y = 0;
				int width = 80;
				int height => (.UIradioBox.y + .UIradioBox.height);
				Root.label_cmd label_cmd {
					labels => <-.labels;
					cmd+OPort2 [2] {
						active => <-.<-.active;
					};
					inItem => <-.selectedItem;
				};
				UIpanel UIpanel {
					x => <-.x;
					y => <-.y;
					width => <-.width;
					height => <-.height;
					parent => <-.parent;
				};
				UIlabel UIlabel {
					y = 0;
					width => <-.UIpanel.width;
					active => <-.active;
					parent => <-.UIpanel;
					label => <-.title;
					alignment = "center";
				};
				UIradioBox UIradioBox {
					y => ((<-.UIlabel.y + <-.UIlabel.height) + 4);
					width => <-.UIpanel.width;
					&visible => <-.visible;
					parent => <-.UIpanel;
					cmdList => <-.label_cmd.cmd;
					selectedItem => <-.label_cmd.outItem;
				};
			};
			macro axe_pos1 {
				ilink parent => <-.axis_frame;
				string+Port labels[] => {"bottom","top"};
				int+IPort2 selectedItem => <-.params.pos[((2 * <-.k) + 1)];
				int visible = 1;
				int active = 1;
				int x = 0;
				int y => (<-.axe_pos.height + 5);
				int width = 80;
				int height => (.UIradioBox.y + .UIradioBox.height);
				Root.label_cmd+OPort label_cmd {
					labels => <-.labels;
					cmd+OPort2 [2] {
						active => <-.<-.active;
					};
					inItem => <-.selectedItem;
				};
				UIpanel UIpanel {
					x => <-.x;
					y => <-.y;
					width => <-.width;
					height => <-.height;
					parent => <-.parent;
				};
				UIradioBox UIradioBox {
					y = 0;
					width => <-.UIpanel.width;
					&visible => <-.visible;
					parent => <-.UIpanel;
					cmdList => <-.label_cmd.cmd;
					selectedItem => <-.label_cmd.outItem;
				};
			};
			int+Port l => <-.ind[(2 * .k)];
			int+Port m => <-.ind[((2 * .k) + 1)];
			UIslider pos {
				y => ((<-.k + 1) * <-.<-.h);
				width = 160;
				parent => <-.link;
				min = 0;
				max => <-.params.num_labels[(2 - <-.k)] - 1;
				value => <-.params.ixyz[(2 - <-.k)];
				mode = "integer";
				title => (<-.<-.plane_names[<-.k] + " position");
				message => (<-.<-.plane_names[<-.k] + " position");
			};
			UItoggle plane {
				x = 170;
				y => (((<-.k + 1) * <-.<-.h) + (<-.<-.a / 2));
				parent => <-.link;
				set => <-.geom_params.planes_vis[(2 - <-.k)];
			};
			UItoggle grid {
				x = 170;
				y => (((<-.k + 1) * <-.<-.h) + (<-.<-.a * 1.5));
				parent => <-.link;
				set => <-.params.frames[(<-.k)];
				active => <-.plane.set;
			};
			ilink link => <-.gen_frame;
			UItext text {
				y => (<-.label.y + <-.label.height);
				width => parent.width;
				parent => <-.axis_frame;
				text => <-.geom_params.labels[<-.k];
			};
			UIlabel label {
				y = 129;
				width => parent.width;
				parent => <-.axis_frame;
				label => (<-.<-.axe_names[<-.k] + " label");
			};
			macro x_tb {
				ilink parent => <-.axis_frame;
				string+Port labels [] => {"at bottom",
					"at top"};
				int+IPort2 selectedItem => <-.params.label_pos[<-.k];
				int visible = 1;
				int active = 1;
				int y => ((<-.text.y + <-.text.height) + 5);
				int width = 80;
				int height => .UIradioBox#1.height;
				Root.label_cmd+OPort label_cmd {
					labels => <-.labels;
					cmd+OPort2 [2] {
						active => <-.<-.active;
					};
					inItem => <-.selectedItem;
				};
				UIradioBox UIradioBox {
					y => (<-.y + <-.height);
					width => <-.width;
					&visible => <-.visible;
					active = 1;
					parent => <-.parent;
					cmdList => <-.label_cmd.cmd;
					selectedItem => <-.label_cmd.outItem;
				};
				string+OPort labels#1 [] => {"left","centered","right"};
				int+IPort2 selectedItem#1 => <-.geom_params.labels_align[<-.k];
				Root.label_cmd label_cmd#1 {
					labels => <-.labels#1;
					cmd+OPort2 [3];
					inItem => <-.selectedItem#1;
				};
				UIradioBox UIradioBox#1 {
					width => <-.width;
					y => <-.y;
					&visible;
					active = 1;
					parent => <-.parent;
					cmdList => <-.label_cmd#1.cmd;
					selectedItem => <-.label_cmd#1.outItem;
				};
			};
		};
	};

	macro Axes3DFunc {
		Grid+IPort2 &infld;
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_mod_params+IPort &params {
			n_labels = 15;
			min_extent => <-.infld.coordinates.min_vec;
			max_extent => <-.infld.coordinates.max_vec;
			intervals = {0.,0.,0.};
			ixyz = {0,50,0};
			frames = {0,0,0};
			pos = {1,0,1,1,1,1};
			label_pos = {1,1,1};
		};
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_geom_params& geom_params {
			h = 15;
			labels = {"x","y","z"};
			planes_vis = {1,1,1};
			labels_align = {2,2,0,0,0,0};
		};
		XP_COMM_PROJ.Axes3D.Axes3DMods.Axes3DCore Axes3DCore {
			params => <-.params;
			xy_mesh+OPort2;
		};
		macro planes[3] {
			int k => index_of(<-.planes);
			GDM.DataObject plane {
				in+IPort3 => <-.<-.Axes3DCore.planes[<-.k];
				Obj {
					visible => <-.<-.<-.geom_params.planes_vis[<-.<-.k];
					name => ("plane" + <-.<-.k);
					xform_mode = "Parent";
				};
			};
			GDM.DataObject frame {
				in+IPort3 => <-.<-.Axes3DCore.planes[(<-.k + 3)];
				Modes {
					mode = {0,2,1,0,0};
				};
				Obj {
					visible => <-.<-.<-.geom_params.planes_vis[<-.<-.k];
					name => ("frame" + <-.<-.k);
					xform_mode = "Parent";
				};
			};
		};
		macro+OPort axes_geom[3] {
			int k => index_of(<-.axes_geom);
			XP_COMM_PROJ.Axes3D.Axes3DMods.axis_data+IPort2 &axis_pars => <-.Axes3DCore.axes_data[.k];
			Mesh+Port &in_mesh => axis_pars.labels;
			string in_text[] => str_format(axis_pars.labels_f,axis_pars.labels_v);
			XP_COMM_PROJ.Axes3D.Axes3DMods.TextVals TextValues {
				align_horiz => <-.<-.geom_params.labels_align[<-.k];
				text_values => <-.in_text;
			};
			Grid+Xform+OPort &TextField => merge(.TextValues,.in_mesh,,,,,,,,,,);
			GDM.DataObject axis_obj {
				in => <-.TextField;
				Props {
					font => <-.<-.<-.geom_params.font;
					trans = 0.;
				};
				Obj {
					name => ("axis" + <-.<-.k);
					xform_mode = "Parent";
				};
			};
			macro TextString3D {
				float coord[] => <-.<-.Axes3DCore.axe_labels_loc[<-.k][];
				XP_COMM_PROJ.Axes3D.Axes3DMods.TextVals text {
					int+IPort2 align_horiz => (2 - <-.<-.<-.geom_params.labels_align[((2 * <-.<-.k) + 1)]);
					string+IPort2 str => <-.<-.<-.geom_params.labels[<-.<-.k];
					int nspace = 3;
					float position[.nspace] => <-.coord;
					float+write min_vec[.nspace];
					float+write max_vec[.nspace];
				};
				GDM.DefaultObject+OPort2 DefaultObject {
					input => <-.text;
					props => <-.DefaultProps;
					name => name_of(<-.<-);
					pick_info => <-.DefaultPickInfo;
					xform_mode = "Parent";
				};
				DefaultProps DefaultProps {
					font+IPort2 => <-.<-.<-.geom_params.l_font;
					inherit = 0;
				};
				GDpick_info DefaultPickInfo;
			};
			GDM.GroupObject full_obj {
				child_objs => {<-.axis_obj.obj,<-.TextString3D.DefaultObject};
				Top {
					xform_mode = "Parent";
				};
			};
			olink out_obj => .full_obj.obj;
		};
		GDM.GroupObject planes_obj {
			child_objs => {
				<-.planes[0].plane.obj,<-.planes[1].plane.obj,
				<-.planes[2].plane.obj};
			Props {
				line_style = "Dotted";
			};
			Modes {
				mode = {0,2,1,0,0};
			};
			Top {
				xform_mode = "Parent";
			};
		};
		GDM.GroupObject frames_obj {
			child_objs => {
				<-.planes[0].frame.obj,<-.planes[1].frame.obj,
				<-.planes[2].frame.obj};
			Modes {
				mode = {0,2,1,0,0};
			};
			Top {
				xform_mode = "Parent";
			};
		};
		GDM.GroupObject axes_obj {
			child_objs => {
				<-.axes_geom[0].out_obj,<-.axes_geom[1].out_obj,
				<-.axes_geom[2].out_obj};
			Top {
				xform_mode = "Parent";
			};
		};
		GDM.GroupObject GroupObject {
			child_objs => {<-.planes_obj.obj,
				<-.frames_obj.obj,<-.axes_obj.obj};
			Top {
				xform_mode = "Parent";
			};
			obj+OPort3;
		};
		GMOD.instancer instancer {
			Value => (<-.Axes3DCore.axe_labels_loc[0][0] != -9999);
			Group => <-.axes_geom;
		};
	};
	macro Axes3D {
		Grid+IPort2 &infld;
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_geom_params+OPort geom_params {
			h = 15;
			labels = {"x","y","z"};
			planes_vis = {1,1,1};
			labels_align = {2,2,0,0,0,0};
		};
		XP_COMM_PROJ.Axes3D.Axes3DMods.axes_mod_params+OPort params {
			n_labels = 15;
			min_extent => <-.infld.coordinates.min_vec;
			max_extent => <-.infld.coordinates.max_vec;
			min_vals => <-.infld.coordinates.min_vec;
			max_vals => <-.infld.coordinates.max_vec;
			intervals = {0.,0.,0.};
			ixyz = {0,50,0};
			frames = {0,0,0};
			pos = {1,0,1,1,1,1};
			label_pos = {1,1,1};
			num_labels = {50,50,50};
		};
		XP_COMM_PROJ.Axes3D.Axes3DMacs.Axes3DUI Axes3DUI {
			params => <-.params;
			geom_params => <-.geom_params;
		};
		XP_COMM_PROJ.Axes3D.Axes3DMacs.Axes3DFunc Axes3DFunc {
			infld => <-.infld;
			params+IPort2 => <-.params;
			geom_params+IPort2 => <-.geom_params;
		};
		olink obj => Axes3DFunc.GroupObject.obj;
	};
};


flibrary OrthogonalViewerMacs {
// Macro transslice
	macro transslice{
		Mesh_Unif+IPort2 &plane;

		group+nres+IPort2 &in_coords{
			int axis;
			float min_axis;
			float max_axis;
			float min_axis0;
			float max_axis0;
			float min_axis1;
			float max_axis1;
			float min_axis2;
			float max_axis2;
			float points_xy [2][2];
			float points_yz [2][2];
			float points_xz [2][2];
			float dist;
			};

		Field+IPort2  &in;
		int+IPort2 data_range_size = 255;

		macro gr_Gen {
			Mesh+Node_Data+IPort2 &in => <-.DVfld_match.links.link2;

			float offset_arr[3] => {minX,minY,minZ};
			float+OPort2 offset => offset_arr[<-.in_coords.axis];
		
			float &minX => in.coordinates.min_vec[0];
			float &maxX => in.coordinates.max_vec[0];
			float &minY => in.coordinates.min_vec[1];
			float &maxY => in.coordinates.max_vec[1];
			float &minZ => switch(is_valid(in.coordinates.min_vec[2]),in.coordinates.min_vec[2]);
			float &maxZ => switch(is_valid(in.coordinates.max_vec[2]),in.coordinates.max_vec[2]);

			XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.TransSliceCute TransSliceCute {
				in => <-.in;
				plane => <-.offset;
				axis => <-.<-.in_coords.axis;
				dec => 1;
				Mesh { 
			  	&xform => <-.<-.<-.xform_Gen;
				};
			};

			olink out => TransSliceCute.out;
		};

		group gr_Unif {
			Mesh_Unif+Node_Data+IPort2 &in => <-.DVfld_match.links.link0;

			int npoints => 2;
			float &points_arr [3][2][2]=> {<-.in_coords.points_yz,<-.in_coords.points_xz,<-.in_coords.points_xy};
			float &points[2][2] => points_arr[<-.in_coords.axis];

			Mesh_Unif Mesh {
				nnodes => in.nnodes;
				nspace => in.nspace;
				ndim => in.ndim;
				dims => in.dims;
				npoints => <-.npoints;
				points => switch(is_valid(<-.points),<-.points);
				grid_type => in.grid_type;
				ncell_sets => in.ncell_sets;
				&cell_set => in.cell_set;
				&xform => <-.<-.xform_Unif;
			};
		
			group tmp [in.nnode_data]{
				Data_Array &field => in.node_data[index_of(<-.tmp)];
			};

			Node_Data ND {
				nnodes => in.nnodes;
				nnode_data => in.nnode_data;
				node_data => tmp.field;
			};

			FLD_MAP.combine_mesh_data CMD1 {
				in_mesh => <-.Mesh;
				in_nd => <-.ND;
			};
			olink out => CMD1.out;
		};

		group gr_Rect {
			int npoints => 2;
			float &points_arr [3][2][2]=> {<-.in_coords.points_yz,<-.in_coords.points_xz,<-.in_coords.points_xy};
			float &points[2][2] => points_arr[<-.in_coords.axis];
		
			Mesh_Rect+Node_Data+IPort2 &in => <-.DVfld_match.links.link1;
			Mesh_Rect Mesh {
				nnodes => in.nnodes;
				nspace => in.nspace;
				ndim => in.ndim;
				dims => in.dims;
				npoints => <-.npoints;
				points => switch(is_valid(<-.points),<-.points);
				grid_type => in.grid_type;
				ncell_sets => in.ncell_sets;
				&cell_set => in.cell_set;
				&xform => <-.<-.xform_Rect;
			};
		
			group tmp [in.nnode_data]{
				Data_Array &field => in.node_data[index_of(<-.tmp)];
			};

			Node_Data ND {
				nnodes => in.nnodes;
				nnode_data => in.nnode_data;
				node_data => tmp.field;
			};

			FLD_MAP.combine_mesh_data CMD1 {
				in_mesh => <-.Mesh;
				in_nd => <-.ND;
			};
			olink out => CMD1.out;
		};

		group gr [3] => {gr_Unif,gr_Rect,gr_Gen};
		int axis => .in_coords.axis;

		// z-plane
		GDIF.DefaultXform xform_Gen_xy	{
			xlate => {0.,0.,0.1};
		};
		// y-plane
		GDIF.DefaultXform xform_Gen_xz	{
			mat => {   1.,0.,0.,0.,0.,-1,0.,0.,0.,0.,1,0.,0.,0.,0.,1.};
			xlate => {0,<-.plane.points[1][1],0.1};
		};
		// x-plane
		GDIF.DefaultXform xform_Gen_yz	{
			mat => { 0.,1.,0.,0.,-1.,0.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.};
			xlate => {<-.plane.points[1][0],0,0.1};
		};

		// z-plane
		GDIF.DefaultXform xform_Unif_xy	{
		};
		// y-plane
		GDIF.DefaultXform xform_Unif_xz	{
			mat => {
				0,-1,0,0,
				1,0,0,0,
				0,0,1,0,
				0,0,0,1
			};
			xlate => {0,<-.plane.points[1][1],0};
		};
		// x-plane
		GDIF.DefaultXform xform_Unif_yz	{
			mat => {
				0,1,0,0,
				-1,0,0,0,
				0,0,1,0,
				0,0,0,1
			};
			xlate => {<-.plane.points[1][0],0,0};
		};
	
		GDIF.DefaultXform xform_arr_Unif [3]=> {xform_Unif_yz,xform_Unif_xz,xform_Unif_xy};
		GDIF.DefaultXform xform_arr_Gen [3]=> {xform_Gen_yz,xform_Gen_xz,xform_Gen_xy};
	
		GDIF.DefaultXform &xform_Unif	=> xform_arr_Unif[axis];
		GDIF.DefaultXform &xform_Rect	=> xform_arr_Unif[axis];
		GDIF.DefaultXform &xform_Gen	=> xform_arr_Gen[axis];

		DVfld_match DVfld_match {
			in => <-.in;
		};

		DVswitch DVswitch{
			in => {<-.gr_Unif.out, <-.gr_Rect.out,<-.gr_Gen.out};
			index => DVfld_match.index;
		};

		DataObjectNoTexture obj {
	        	in => DVswitch.out;
        		Obj {
	            		name => name_of(<-.<-.<-);
            			xform_mode = GD_XFORM_MODE_PARENT;
				Datamap {
						int range_size => <-.<-.data_range_size;
						DataRange {
							size => <-.range_size;
						};
						int range_min => DataRange.minimum;
						int range_max => DataRange.maximum;
					};
        		};
    		};
		Mesh_Unif bounds_plane{
	  		dims = {2,2};
   			ndim = 2;
   			points => <-.plane.points;
   			nspace = 2;
		};
	
		Mesh_Unif+OPort2 &out_plane => switch((array_size(in) && is_valid(plane)),bounds_plane);
	    	olink out_fld<export_all=2> => DVswitch.out;
	    	olink out_obj => obj.obj;
	};
// Macro transslice_ARR
	macro transslice_ARR {
		group+nres+IPort2 &in_coords [] {
			int axis;
			float min_axis;
			float max_axis;
			float min_axis0;
			float max_axis0;
			float min_axis1;
			float max_axis1;
			float min_axis2;
			float max_axis2;
			float points_xy [2][2];
			float points_yz [2][2];
			float points_xz [2][2];
			float dist;
			};

		Mesh_Unif+IPort2 &plane;
		Field+IPort2 &in[];
		int+IPort2 data_range_size = 255;
		transslice convert[array_size(in)]
		{
			in => <-.in[index_of(convert)];
			in_coords => <-.in_coords[index_of(convert)];
			plane => <-.plane;
			data_range_size=<-.data_range_size;
		};
		// output
		DataObjects objs {
			in_fields => convert.out_fld;
			dos.Obj.xform_mode = GD_XFORM_MODE_PARENT;
			dos {
				Obj {
					name+nres => name_of(<-.<-.<-.<-)+index_of(dos);
					Datamap {
						int range_size => <-.<-.<-.data_range_size;
						DataRange {
							size => <-.range_size;
						} = {
							{
									size=><-.range_size
							}
						};
						int range_min => DataRange.minimum;
						int range_max => DataRange.maximum;
					};
				};
			};
		};
	
		GroupObject slice_obj {
			Top.name => name_of(<-.<-.<-);
			child_objs => objs.dos.obj;
			Top.xform_mode = GD_XFORM_MODE_PARENT;
			Top.alt_xform+nres => <-.plane.xform;
		};
	
		Mesh_Unif bounds_plane{
	  		dims = {2,2};
   			ndim = 2;
   			points => <-.plane.points;
   			nspace = 2;
		};

		Mesh_Unif+OPort2 &out_plane => switch((array_size(in) && is_valid(plane)),bounds_plane);

		mlink+OPort2 out_fld => convert.out_fld;
    		olink out_obj => slice_obj.obj;
	};
// Macro get_position
	macro get_position {
		Mesh+IPort2 &Mesh;
		Mesh+IPort2 &obj_in;
		
		float+nres min_x => obj_in.coordinates.min_vec[0];
   		float+nres max_x => obj_in.coordinates.max_vec[0];
		float+nres min_y => obj_in.coordinates.min_vec[1];
   		float+nres max_y => obj_in.coordinates.max_vec[1];
   		float+nres point_x => Mesh.coordinates.values[0][0];
   		float+nres point_y => Mesh.coordinates.values[0][1];
		int x => switch((point_x > min_x) + 1,min_x,switch((point_x < max_x) + 1,max_x,point_x));
        	int y => switch((point_y > min_y) + 1,min_y,switch((point_y < max_y) + 1,max_y,point_y));
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_x {
			in => <-.x;
			out => <-.x_out;
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_y {
			in => <-.y;
			out => <-.y_out;
		};
		int+OPort2 x_out;
		int+OPort2 y_out;
	};
// Macro ChangeDatarange
	macro ChangeDatarange {
		ilink view;
		int+IPort2 DataRangeSize;
		int+IPort2 min;
		int+IPort2 max;
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify {
			in => <-.UItwoPoint.y;
			min => <-.min;
			max => <-.max;
			val => <-.DataRangeSize;
			reset => (<-.UItwoPoint.state == 1);
		};
		UItwoPoint UItwoPoint {
			view+nres => <-.view.render_view;
			startEvent = "<Btn2Down>";
			runEvent = "<Btn2Motion>";
			stopEvent = "<Btn2Up>";
		};
	};
// Macro ChangeZoom
	macro ChangeZoom {
		ilink view;
		int+IPort2 Zoom;
		int+IPort2 min = 16;
		int+IPort2 max = 2000;
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify {
			in => <-.UItwoPoint.y;
			min => <-.min;
			max => <-.max;
			val => <-.Zoom;
			reset => (<-.UItwoPoint.state == 1);
		};
		UItwoPoint UItwoPoint {
			view+nres => <-.view.render_view;
			startEvent = "<Btn3Down>";
			runEvent = "<Btn3Motion>";
			stopEvent = "<Btn3Up>";
		};
	};
// Macro DrawCross2D
	macro DrawCross2D {
		Mesh_Unif+IPort2 &in;

		int+IPort2 x;
		int+IPort2 y;
		
		Mesh_Unif line {
			nspace = 1;
			dims = {4};
			ndim = 1;
			points+nres => {in.points[0][1],in.points[1][1]};
			xform {
				mat = {
					0,1.,0.,0.,
					-1.,0,0.,0.,
					0.,0.,1.,0.,
					0.,0.,0.,1.
				};
				xlate+nres => {	x,0.,0.};
			};
		};
		Mesh_Unif line2 {
			nspace = 1;
			ndim = 1;
			dims = {4};
			points+nres => {in.points[0][0],in.points[1][0]};
			xform {
				xlate+nres => {0,y,0};
			};
		};
		GDM.DataObject obj {
			in => <-.line;
			Obj {
				name => name_of(<-.<-.<-);
			};
			obj+OPort3;
		};
		GDM.DataObject obj2 {
			in => <-.line2;
			Obj {
				name => name_of(<-.<-.<-);
			};
			obj+OPort3;
		};
 	};
// Macro Ortho Scene	
	macro OrthoScene {
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.OrthogonalViewerParams+IPort2 &params;
		Field+nres &MeshIn<NEportLevels={2,1}>[];
		int+nres axis<NEportLevels={2,1}>;
		ilink ViewPanel;
		int+OPort DataRangeSize => switch((params.useGlobal + 1),params.DataRangeSize[axis],params.globalDataRangeSize);
        	int+OPort Zoom => switch((params.useGlobal + 1),params.zoom[axis],params.globalZoom);
		
		group ViewFrameParams {
			int+nres height => parent.height;
			int+nres width  => parent.width;
			int+nres x => parent.x;
			int+nres y => parent.y;
			ilink parent=> <-.ViewPanel;
		};

		GDM.Uscene2D Uscene2D {
			Top {
				child_objs => { <-.<-.transslice_ARR.out_obj,<-.<-.bounds.out_obj
						,<-.<-.DrawCross2D.obj.obj,<-.<-.DrawCross2D.obj2.obj
				};
			};
			Camera {
				Camera {
					norm_scale => <-.<-.<-.Zoom*0.05;
				};
			};
			View {
				macro ViewUI {
					UIrenderView ViewWindow {
						y = 0;
						parent => <-.ViewFrame;
						width => (parent.width <+> .dw);
						height => (parent.height <+> .dh);
						color {
							backgroundColor = "black/\*";
						};
						int+nres dw => (parent.clientWidth - parent.width);
						int+nres dh => (parent.clientHeight - parent.height);
					};
					GMOD.instancer instancer {
						Value => <-.<-.View.video;
						Group => <-.FullViewWindow;
					};
					macro FullViewWindow {
						UIshell FullShell {
							x = 0;
							y = 0;
							width => <-.<-.<-.View.full_view_width;
							height => <-.<-.<-.View.full_view_height;
							showStatusBar = 0;
						};
						UIpanel FullPanel {
							x => parent.x;
							parent => <-.FullShell;
							y => parent.y;
							width => parent.width;
							height => parent.height;
						};
						UIrenderView FullWindow {
							x => parent.x;
							parent => <-.FullPanel;
							y => (parent.height / 2);
							width => parent.width;
							height => (parent.height / 2);
						};
					};
					UImouseEvents ViewEvents {
						view => <-.RendView;
					};
					UItwoPoint PickInteractor {
						view => <-.RendView;
						startEvent = "Control<Btn1Down>";
						runEvent<NEdisplayMode="open">;
						stopEvent = "Control<Btn1Up>";
					};
					UIonePoint VideoOffControl {
						view => <-.RendView;
						runEvent = "Shift<Btn1Down>";
					};
					GMOD.parse_v ViewSelect {
						v_commands = "View.selected = 1;";
						trigger => (<-.ViewEvents.buttonType | ViewPanel.UI.panel.visible);
						sync = 1;
						active => ;
						on_inst = 0;
						relative => <-;
						parse_v_relative;
					};
					link RendView => switch((<-.View.video + 1),.ViewWindow,.FullViewWindow.FullWindow);
					UIframe ViewFrame {
						x => <-.<-.<-.<-.ViewFrameParams.x;
						y => <-.<-.<-.<-.ViewFrameParams.y;
						width => <-.<-.<-.<-.ViewFrameParams.width;
						height => <-.<-.<-.<-.ViewFrameParams.height;
						parent => <-.<-.<-.<-.ViewFrameParams.parent;
						color {
							backgroundColor => switch(<-.<-.<-.View.selected,"red");
						};
						shadowType = "shadow_etched_in";
						shadowThickness = 3;
					};
				};
			};
		};
		
		XP_COMM_PROJ.slice_orthoplane_ARR.slice_orthoplane_ARRmacs.slice_orthoplane_ARR_params SliceParams{
        		components[array_size(<-.MeshIn)] { component = { 0 }; };
	        	cell_datas[array_size(<-.MeshIn)] { cell_data = { 0 }; };
			axis => <-.axis;
			dist => <-.params.plane[axis];
		};

		XP_COMM_PROJ.slice_orthoplane_ARR.slice_orthoplane_ARRmacs.slice_orthoplane_ARR_func orthoplane{
			in_field => <-.MeshIn;
			data_range_size =><-.DataRangeSize;
			Params => <-.SliceParams;
		};

		transslice_ARR transslice_ARR {
			in => <-.orthoplane.out_fld;
			plane => <-.orthoplane.out_plane;
			in_coords => <-.orthoplane.out_coords;
			data_range_size =><-.DataRangeSize;
		};

		GDM.DrawCursor2D DrawCursor2D {
			view_in => <-.Uscene2D.View.View;
			obj_in => <-.bounds.out_obj;
			draw_cursor {
				UIpanel UImod_panel;
				UItwoPoint {
					startEvent = "<Btn1Down>";
					runEvent = "<Btn1Motion>";
					stopEvent = "<Btn1Up>";
				};
				GDdraw2d_cursor {
					immed = 0;
					mode= "Append";
				};
 			};
		};

		get_position get_position {
			Mesh => <-.DrawCursor2D.draw_cursor.GDdraw2d_cursor.out_mesh;
			obj_in => <-.transslice_ARR.out_plane;
		};

		MODS.bounds bounds {
			in_field => <-.transslice_ARR.out_plane;
			UIpanel UIpanel;
		};

		MODS.bounds bounds_all {
			in_field => <-.orthoplane.out_plane;
			UIpanel UIpanel;
		};
		
		GDM.DefaultObject+OPort2 OrthogonalSlice {
			objects => {<-.orthoplane.out_obj,<-.bounds_all.out_obj};
			name = "OrthogonalSlice";
			xform_mode = "Parent";
		};
		DrawCross2D DrawCross2D {
			in => <-.transslice_ARR.out_plane;
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.ChangeDatarange ChangeDatarange {
			view => <-.Uscene2D.View.View;
			DataRangeSize => <-.DataRangeSize;
			//min => <-.orthoplane.out_obj.Datamap.range_min;
			//max => <-.orthoplane.out_obj.Datamap.range_max;
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.ChangeZoom ChangeZoom {
			view => <-.Uscene2D.View.View;
			Zoom => <-.Zoom;
		};
	};

// Macro globalUI
	macro globalUI {
		ilink parent;
		int y;
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.OrthogonalViewerParams+IPort2 &params;
		UIpanel UIpanel {
			visible => <-.params.useGlobal;
			parent+IPort3 => <-.parent;
			y => <-.y;
			width => parent.clientWidth;
			height => <-.Data_Range_Size.y+<-.Data_Range_Size.height+10;
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIslider Zoom {
			parent => <-.UIframe;
			value = 0.8;
			y = 0;
			width => parent.clientWidth;
			min = 0.8;
			max = 100.;
		};
		GMOD.copy_on_change copy_on_change_zoom_slider {
			input => (<-.params.globalZoom * 0.05);			
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_zoom_slider {
			in => <-.copy_on_change_zoom_slider.output;
			out => <-.Zoom.value;
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_zoom_value {
			in => (<-.Zoom.value / 0.05);
			out => <-.params.globalZoom;
		};
		GMOD.copy_on_change copy_on_change_zoom_axial {
			input => <-.copy_on_modify_zoom_value.out;
			output => <-.params.zoom[2];
		};
		GMOD.copy_on_change copy_on_change_zoom_coronal {
			input => <-.copy_on_modify_zoom_value.out;
			output => <-.params.zoom[1];
		};
		GMOD.copy_on_change copy_on_change_zoom_sagittal {
			input => <-.copy_on_modify_zoom_value.out;
			output => <-.params.zoom[0];
		};
		UIslider Data_Range_Size {
			parent => <-.UIframe;
			value => <-.params.globalDataRangeSize;
			y => <-.Zoom.y+<-.Zoom.height;
			width => parent.clientWidth;
			min = 0;
			mode = "integer";
			max = 255;
		};
		GMOD.copy_on_change copy_on_change_range_axial {
			input => <-.params.globalDataRangeSize;
			output => <-.params.DataRangeSize[2];
		};
		GMOD.copy_on_change copy_on_change_range_coronal {
			input => <-.params.globalDataRangeSize;
			output => <-.params.DataRangeSize[1];
		};
		GMOD.copy_on_change copy_on_change_range_sagittal {
			input => <-.params.globalDataRangeSize;
			output => <-.params.DataRangeSize[0];
		};
	};
// Macro LocalUI
	macro localUI {
		ilink parent;
		int y;
		int axis;
		string label;
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.OrthogonalViewerParams+IPort2 &params;
		UIpanel UIpanel {
			visible => (! <-.params.useGlobal);
			parent+IPort3 => <-.parent;
			y => <-.y;
			width => parent.clientWidth;
			height => <-.Data_Range_Size.y+<-.Data_Range_Size.height+10;
		};
		int height => UIpanel.height;
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel UIlabel {	
			parent => <-.UIframe;
			y = 0;
			width => parent.clientWidth;
			label => <-.label;
		};
		UIslider Zoom {
			parent => <-.UIframe;
			value = 0.8;
			y => <-.UIlabel.y + <-.UIlabel.height;
			width => parent.clientWidth;
			min = 0.8;
			max = 100.;
		};
		GMOD.copy_on_change copy_on_change_zoom_slider {
			input => (<-.params.zoom[<-.axis] * 0.05);
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_zoom_slider {
			in => <-.copy_on_change_zoom_slider.output;
			out => <-.Zoom.value;
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.copy_on_modify copy_on_modify_zoom_value {
			in => (<-.Zoom.value / 0.05);
			out => <-.params.zoom[<-.axis];
		};
		UIslider Data_Range_Size {
			parent => <-.UIframe;
			value => <-.params.DataRangeSize[<-.axis];
			y => <-.Zoom.y+<-.Zoom.height;
			width => parent.clientWidth;
			min = 0;
			mode = "integer";
			max = 255;
		};
	};
// Macro OrthogonalViewerUI
	macro OrthogonalViewerUI {
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.OrthogonalViewerParams+IPort2 &params;
		Mesh+IPort2 &Mesh[];
		group+nres tmp_coords [array_size(.Mesh)] {
			float min_axes0 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.min_vec[0];
			float max_axes0 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.max_vec[0];
			float min_axes1 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.min_vec[1];
			float max_axes1 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.max_vec[1];
			float min_axes2 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.min_vec[2];
			float max_axes2 => <-.Mesh[index_of(<-.tmp_coords)].coordinates.max_vec[2];
			};

		UImod_panel UImod_panel {
			option {
				set = 1;
			};
			title => name_of(<-.<-.<-,1);
		};
		UIframe UIframe {
			parent => <-.UImod_panel;
			y = 0;
			height => <-.sagittal.y + <-.sagittal.height + 5;
			width => parent.clientWidth;
		};
		UIlabel title {
			parent => <-.UIframe;
			label => "Show Orthoslice in 3D Scene";
			y = 0;
			width => parent.clientWidth;
		};
		UItoggle axial {
			parent => <-.UIframe;
			label => "Axial (xy)";
			set => <-.params.showPlane[2];
			y => <-.title.y + <-.title.height;
		};
		UItoggle coronal {
			parent => <-.UIframe;
			label => "Coronal (xz)";
			set => <-.params.showPlane[1];
			y => <-.axial.y + <-.axial.height;
		};
		UItoggle sagittal {
			parent => <-.UIframe;
			label => "Sagittal (yz)";
			set => <-.params.showPlane[0];
			y => <-.coronal.y + <-.coronal.height;
		};
		
		UIlabel slidertitle {
			parent => <-.UImod_panel;
			label => "Orthoslice Planes";
			y => <-.UIframe.y + <-.UIframe.height + 10;
			width => parent.clientWidth;
		};
		
		UIslider axialslider {
			parent => <-.UImod_panel;
			title => "Axial (xy)";
			value => <-.params.plane[2];
			mode = "integer";
			min => 0;
			max+nres => max_array(<-.tmp_coords.max_axes2);
			y => <-.slidertitle.y + <-.slidertitle.height;
			width => parent.clientWidth;
		};
		UIslider coronalslider {
			parent => <-.UImod_panel;
			title => "Coronal (xz)";
			value => <-.params.plane[1];
			mode = "integer";
			min => 0;
			max+nres => max_array(<-.tmp_coords.max_axes1);
			y => <-.axialslider.y + <-.axialslider.height;
			width => parent.clientWidth;
		};
		UIslider sagittalslider {
			parent => <-.UImod_panel;
			title => "Sagittal (yz)";
			value => <-.params.plane[0];
			mode = "integer";
			min => 0;
			max+nres => max_array(<-.tmp_coords.max_axes0);
			y => <-.coronalslider.y + <-.coronalslider.height;
			width => parent.clientWidth;
		};
		UItoggle global {
			parent => <-.UImod_panel;
			label => "global settings for directional views";
			set => <-.params.useGlobal;
			y => <-.sagittalslider.y + <-.sagittalslider.height+10;
			width => parent.clientWidth;
		};
		
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.globalUI globalUI {
			params => <-.params;
			y => <-.global.y + <-.global.height+10;
			parent => <-.UImod_panel;
		};
		
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.localUI axialUI {
			params => <-.params;
			y => <-.global.y + <-.global.height+10;
			parent => <-.UImod_panel;
			axis = 2;
			label= "Axial (xy)";
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.localUI coronalUI {
			params => <-.params;
			y => <-.axialUI.y + <-.axialUI.height+10;
			parent => <-.UImod_panel;
			axis = 1;
			label= "Coronal (xz)";
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.localUI sagittalUI {
			params => <-.params;
			y => <-.coronalUI.y + <-.coronalUI.height+10;
			parent => <-.UImod_panel;
			axis = 0;
			label= "Sagittal (yz)";
		};
	};
// Macro OrthogonalViewer
	GDM.Uviewer3D OrthogonalViewer {
		imlink in_field;
		Scene {	
			UItwoPoint {
				view => <-.ViewPanel;
			};
			Top {
				Top {
					xform_mode = "Parent";
				};
			};
 			GDM.GroupObject OrthogonalView {
            	child_objs => {
 							<-.Top.obj,
							<-.<-.OrthoScene.OrthogonalSlice,
							<-.<-.OrthoScene1.OrthogonalSlice,
							<-.<-.OrthoScene2.OrthogonalSlice
            	};
			};					
			Camera{
				Camera{
					objects => {<-.<-.OrthogonalView.obj};
				};			
			};
			View {
				ViewUI {
					ViewFrame {
						width => <-.<-.<-.<-.params.WindowCenter[0]-1;
						height => <-.<-.<-.<-.params.WindowCenter[1]-1;
					};
				};
			};
			olink ViewPanel => View.ViewUI.ViewPanel.UI.panel;
		};
		UItwoPoint UItwoPoint {
			view+nres => <-.Scene.ViewPanel;
			startEvent = "<Btn3Down>";
			runEvent = "<Btn3Motion>";
			stopEvent = "<Btn3Up>";
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMods.OrthogonalViewerParams params {
			plane = {0,0,0};
			showPlane = {0,0,0};	
			zoom = {19,19,19};
			globalZoom = 19;
			useGlobal = 0;
			DataRangeSize = {255,255,255};	
			globalDataRangeSize = 255;
			WindowCenter => switch (is_valid(UItwoPoint.x)+1,{<-.UItwoPoint.view.clientWidth/2,<-.UItwoPoint.view.clientHeight/2},{<-.UItwoPoint.x,<-.UItwoPoint.y});
		};
		OrthogonalViewerUI OrthogonalViewerUI {
			params => <-.params;
			Mesh => <-.in_field;
		};
		// right - down
		OrthoScene OrthoScene {
			params => <-.params;
			MeshIn => <-.in_field;
			axis => 2;
			ViewPanel => <-.Scene.ViewPanel;
			ViewFrameParams {
				height => (parent.clientHeight - params.WindowCenter[1])-1;
				width => (parent.clientWidth - params.WindowCenter[0])-1;
				x => params.WindowCenter[0]+1;
				y => params.WindowCenter[1]+1;
			};
			get_position {
				x_out => params.plane[0];
				y_out => params.plane[1];
			};
			DrawCross2D {
				x => <-.params.plane[0];
				y => params.plane[1];
			};
		};
		// right - up
		OrthoScene OrthoScene1 {
			params => <-.params;
			MeshIn => <-.in_field;
			axis => 1;
			ViewPanel => <-.Scene.ViewPanel;
			ViewFrameParams {
				height => (params.WindowCenter[1])-1;
				width => (parent.clientWidth - params.WindowCenter[0])-1;
				x => params.WindowCenter[0]+1;
				y => 0;
			};
			get_position {
				x_out => params.plane[0];
				y_out => params.plane[2];
			};
			DrawCross2D {
				x => params.plane[0];
				y => params.plane[2];
			};
		};
		// left - down
		OrthoScene OrthoScene2 {
			params => <-.params;
			MeshIn => <-.in_field;
			axis => 0;
			ViewPanel => <-.Scene.ViewPanel;
			ViewFrameParams {
				height => (parent.clientHeight - params.WindowCenter[1])-1;
				width => (params.WindowCenter[0])-1;
				x => 0;
				y => params.WindowCenter[1]+1;
			};
			get_position {
				x_out => params.plane[2];
				y_out => params.plane[1];
			};
			DrawCross2D {
				x => params.plane[2];
				y => params.plane[1];
			};
		};
		int+OPort2 SelectedPoint[3] => params.plane;
	};

	OrthogonalViewer OrthogonalViewer2 {
		imlink in_objs;
		group Extract_Mesh_ARR {
      			GDM.GroupObject in {
         			child_objs+IPort3 => <-.<-.in_objs;
      			};
      			int+nres ndim => switch(is_valid(.in.Top.nobjs),.in.Top.nobjs);
      			group tmp[.ndim] {
         		Field+nres &field => <-.in.Top.objects[index_of(<-.tmp)].input;
      			};
      			Field+nres+OPort2 &out_fld[] => .tmp.field;
   		}; 
		mlink+Port in_field => Extract_Mesh_ARR.out_fld;
	};
};

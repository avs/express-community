
flibrary slice_orthoplane_ARRmacs{
	group+Port slice_orthoplane_ARR_params {
		// Orthoslice parameters
		int+Port2 axis;
		float+Port2+nres dist;

		// Slice parameters
		group+nres+Port2 components[] {
			int component [];
		};
		group+nres+Port2 cell_datas[] {
			int cell_data[];
		};
	};

	macro  slice_orthoplane_ARR_func {
		Field+IPort2 &in_field [];

		int dim1 = 2;
		int dim2 = 2;
		int dim3 = 2;

		int+IPort2 data_range_size = 255;

		group+nres+Port tmp_coords <NEcolor0=0x00FF00,NEcolor1=0xF0FF00,NEcolor3=0x00FFFF,NEnumColors=4>[array_size(.in_field)] {
			int axis =><-.Params.axis;
			float min_axis => <-.in_field[index_of(<-.tmp_coords)].coordinates.min_vec[axis];
			float max_axis => <-.in_field[index_of(<-.tmp_coords)].coordinates.max_vec[axis];
			float min_axis0 => <-.in_field[index_of(<-.tmp_coords)].coordinates.min_vec[0];
			float max_axis0 => <-.in_field[index_of(<-.tmp_coords)].coordinates.max_vec[0];
			float min_axis1 => <-.in_field[index_of(<-.tmp_coords)].coordinates.min_vec[1];
			float max_axis1 => <-.in_field[index_of(<-.tmp_coords)].coordinates.max_vec[1];
			float min_axis2 => <-.in_field[index_of(<-.tmp_coords)].coordinates.min_vec[2];
			float max_axis2 => <-.in_field[index_of(<-.tmp_coords)].coordinates.max_vec[2];
			float points_xy [2][2]=> {min_axis0,min_axis1,max_axis0,max_axis1};
			float points_yz [2][2]=> {min_axis1,min_axis2,max_axis1,max_axis2};
			float points_xz [2][2]=> {min_axis2,min_axis0,max_axis2,max_axis0};
			float dist => <-.Params.dist;
		};

		slice_orthoplane_ARR_params+IPort2 &Params;

		Plane_Mesh plane_xy {
			xform {
				xlate+nres => cache({0.0, 0.0, Params.dist});
			};
			dims => {dim1, dim2};
			points+nres => {
				min_array(<-.tmp_coords.min_axis0),
				min_array(<-.tmp_coords.min_axis1),
				max_array(<-.tmp_coords.max_axis0),
				max_array(<-.tmp_coords.max_axis1)
			};
		};

		Plane_Mesh plane_yz {
			xform {
				mat = {
					{0,0,1,0},
					{0,1,0,0},
					{-1,0,0,0},
					{0,0,0,1}
				};
				xlate+nres => cache({Params.dist, 0.0, 0.0});
			};
			dims => {dim3, dim2};
			points+nres => {
				min_array(<-.tmp_coords.min_axis2),
				min_array(<-.tmp_coords.min_axis1),
				max_array(<-.tmp_coords.max_axis2),
				max_array(<-.tmp_coords.max_axis1)
			};
		};
		Plane_Mesh plane_xz {
			xform {
				mat = {
					{1,0,0,0},
					{0,0,1,0},
					{0,-1,0,0},
					{0,0,0,1}
				};
				xlate+nres => cache({0.0, Params.dist, 0.0});
			};
			dims => {dim1, dim3};
			points+nres => {
				min_array(<-.tmp_coords.min_axis0),
				min_array(<-.tmp_coords.min_axis2),
				max_array(<-.tmp_coords.max_axis0),
				max_array(<-.tmp_coords.max_axis2)
			};
		};

		Plane_Mesh planes[] => { plane_yz, plane_xz, plane_xy };

		Plane_Mesh &plane => planes[Params.axis];

		int has_cell_data => ( sum (DVcell_data_labels_ncomp) > 0);

		group+nres tmp_cell_data_labels [array_size( .DVcell_data_labels)] {
			int ncomp => <-.DVcell_data_labels[index_of(<-.tmp_cell_data_labels)].ncomp;
		};

		int DVcell_data_labels_ncomp []=> tmp_cell_data_labels.ncomp;

		DVcell_data_labels DVcell_data_labels [array_size(in_field)]{
			in => in_field[index_of(DVcell_data_labels )];
		};

		// DVslice
		DVslice DVslice [array_size(in_field)] {
			slice_orthoplane_ARR_params+IPort2 &param => Params;
			in => in_field[index_of(DVslice)];
			plane => <-.plane;
			dist = 0;
			map_comp => param.components[index_of(DVslice)].component;
			cell_data => switch(<-.has_cell_data, param.cell_datas[index_of(DVslice)].cell_data);
		};

		// output
		DataObjects objs {
			in_fields => DVslice.out;
			dos.Obj.xform_mode = 1;
			dos {
				Obj { 
					name+nres => name_of(<-.<-.<-.<-.<-)+"_"+index_of(dos);
					Datamap {
						int range_size => <-.<-.<-.data_range_size;
						DataRange {
							size => <-.range_size;
						} = {
							{
								size => <-.range_size
							}
						};
						int range_min => DataRange.minimum;
						int range_max => DataRange.maximum;
					};
				};
			};
		};

		GroupObject slice_obj {
			Top.name => name_of(<-.<-.<-.<-);
			child_objs => objs.dos.obj;
			Top.xform_mode = "Parent";
			Top.alt_xform+nres => <-.<-.plane.xform;
		};

		olink out_coords => tmp_coords;
		Mesh_Unif+OPort2 &out_plane => switch(array_size(.in_field) > 0,plane);
		GDM.DataObject DataObject {
			in => <-.out_plane;
		};
		olink obj_plane => DataObject.obj;
		omlink out_fld => DVslice.out;
		olink out_obj => slice_obj.obj;
	};

	// Slice_OrthoPlane_ARR_UI
	macro slice_orthoplane_ARR_UI {
		imlink in_field;
		slice_orthoplane_ARR_params+IPort2 & Params;
		
		group+nres tmp_coords [array_size(.in_field)] {
			float min_axis => <-.in_field[index_of(<-.tmp_coords)].coordinates.min_vec[<-.Params.axis];
			float max_axis => <-.in_field[index_of(<-.tmp_coords)].coordinates.max_vec[<-.Params.axis];
		};

		//DVnode_data_labels
		DVnode_data_labels DVnode_data_labels [array_size(in_field)]{
			in => in_field[index_of(DVnode_data_labels)];
			int+nres  ncomp => in.nnode_data;
		};
   	
		DVcell_data_labels DVcell_data_labels [array_size(in_field)]{
			in => in_field[index_of(DVcell_data_labels)];
		};

		group+nres tmp_axis [array_size(.in_field)] {
			int max_axis => <-.in_field[index_of(<-.tmp_axis)].nspace;
		};

		UImod_panel UIpanel {
			parent<NEportLevels={4,0}>;
			title => name_of(<-.<-.<-);
			message = "Select slice control panel.";
		};

		UIslider axis_slider {
			parent => <-.UIpanel;
			title = "axis";
			max+nres+IPort2 => max_array(<-.tmp_axis.max_axis)-1;
			value+IPort2 => Params.axis;
			mode = "integer";
			y		=  0;
			width	=> parent.clientWidth;
		};

		UIslider plane_slider {
			parent => <-.UIpanel;
			title = "plane";
			min+nres+IPort2 => min_array(<-.tmp_coords.min_axis);
			max+nres+IPort2 => max_array(<-.tmp_coords.max_axis);
			value+IPort2 => Params.dist;
			mode = "real";
			y => <-.axis_slider.y + <-.axis_slider.height + 4;
			width => parent.clientWidth;
		};

        // Simplify and do all data components by default ?
		group UIoptionBoxLabels [array_size(.in_field)]{

			int offset => switch ((index_of(UIoptionBoxLabels)>0)+1,<-.plane_slider.y + <-.plane_slider.height+8,UIoptionBoxLabels[index_of(UIoptionBoxLabels)-1].UIoptionBoxLabel.y+UIoptionBoxLabels[index_of(UIoptionBoxLabels)-1].UIoptionBoxLabel.height+40);

			UIoptionBoxLabel  UIoptionBoxLabel {
				parent => <-.<-.UIpanel;
				labels+IPort2 => DVnode_data_labels[index_of(UIoptionBoxLabels)].labels;
				max => DVnode_data_labels[index_of(UIoptionBoxLabels)].ncomp;
				&selectedItems+IPort2 => Params.components[index_of(UIoptionBoxLabels)].component;
				title => str_format("Map components %d",index_of(UIoptionBoxLabels));
				visible => parent.visible;
				y	=> <-.offset;
				width	=> parent.clientWidth;
			};

			UIoptionBoxLabel  UIoptionBoxLabel_cell {
				parent => <-.<-.UIpanel;
				labels+IPort2 => DVcell_data_labels[index_of(UIoptionBoxLabels)].labels;
				max => DVcell_data_labels[index_of(UIoptionBoxLabels)].ncomp;
				&selectedItems+IPort2 => Params.cell_datas[index_of(UIoptionBoxLabels)].cell_data;
				title => str_format("Map Cell components %d",index_of(UIoptionBoxLabels));
				visible => parent.visible & (DVcell_data_labels[index_of(UIoptionBoxLabels)].ncomp > 0);
				y	=> <-.UIoptionBoxLabel.y + <-.UIoptionBoxLabel.height + 8;
				width	=> parent.clientWidth;
			};
		};
	};// SliceUI

	// Usermacro
	macro slice_orthoplane_ARR {
		Field+IPort2 &in [];
		slice_orthoplane_ARR_params Slice_OrthoPlane_Arr_Params
		{
			axis = 0;
			components[array_size(<-.in)] { component = { 0 }; };
			cell_datas[array_size(<-.in)] { cell_data = { 0 }; };
			dist => (min_array(<-.slice_orthoplane_ARR_func.tmp_coords.min_axis) + 
				max_array(<-.slice_orthoplane_ARR_func.tmp_coords.max_axis)) / 2.0;
		};
		slice_orthoplane_ARR_UI slice_orthoplane_ARR_UI {
			in_field => <-.in;
			Params => <-.Slice_OrthoPlane_Arr_Params;
		};
		slice_orthoplane_ARR_func slice_orthoplane_ARR_func {
			in_field => <-.in;
			Params => <-.Slice_OrthoPlane_Arr_Params;
		};
		olink plane_out => slice_orthoplane_ARR_func.out_plane;
		olink plane_obj => slice_orthoplane_ARR_func.obj_plane;	
		omlink out_fld => slice_orthoplane_ARR_func.out_fld;
		olink out_obj => slice_orthoplane_ARR_func.out_obj;
		
	};
};

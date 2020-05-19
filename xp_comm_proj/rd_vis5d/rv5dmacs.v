
flibrary ReadVis5DMacs<compile_subs=0>
{
	macro LoopUI {
		ilink parent;
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DParams+IPort2 &params;
		int y;
		int height => .cycle_toggle.y + .cycle_toggle.height + 5;
		int steps => array_size(<-.params.time_steps);
		UIframe frame_loop {
			parent => <-.parent;
			y => <-.y;
			width => parent.clientWidth;
			height => <-.height;
		};
		UItoggle Run {
			parent => <-.frame_loop;
			x = 8;
			y = 5;
			width = 80;
		};
		UItoggle Step {
			parent => <-.frame_loop;
			x => <-.Run.x;
			y => <-.Run.y + <-.Run.height;
			width => <-.Run.width;
		};
		UItoggle Reset {
			parent => <-.frame_loop;
			x => <-.Step.x;
			y => <-.Step.y + <-.Step.height;
			width => <-.Step.width;
		};
		UItoggle Run_Backwards {
			parent => <-.frame_loop;
			x => <-.Run.x + <-.Run.width + 10;
			y => <-.Run.y;
			width = 137;
		};
		UItoggle Step_Backwards {
			parent => <-.frame_loop;
			x => <-.Step.x + <-.Step.width + 10;
			y => <-.Step.y;
			width => <-.Run_Backwards.width;
		};
		UItoggle Reset_Back {
			parent => <-.frame_loop;
			x => <-.Reset.x + <-.Reset.width + 10;
			y => <-.Reset.y;
			width => <-.Step_Backwards.width;
		};
		UIoption Once {
			set = 1;
		};
		UIoption Cycle;
		UIoption Bounce;
		UIoptionMenu cycle_toggle {
			parent => <-.frame_loop;
			cmdList => {<-.Once,<-.Cycle,<-.Bounce};
			label => "Cycle Options";
			selectedItem = 0;
			y => <-.Reset.y + <-.Reset.height + 5;
			x = 10;
		};
		GMOD.loop loop {
			reset => <-.Reset.set;
			reset_back => <-.Reset_Back.set;
			run => <-.Run.set;
			run_back => <-.Run_Backwards.set;
			step => <-.Step.set;
			step_back => <-.Step_Backwards.set;
			cycle => <-.cycle_toggle.selectedItem;
			start_val = 0.;
			end_val => (<-.steps - 1);
			incr = 1.;
			count => <-.params.time_step;
			done = 1;
		};
	};

	macro WindSelectUI {
		ilink parent;
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DParams+IPort2 &params;
		int y;
		int height => list_x.y + list_x.height + 5;
		UIpanel UIpanel {
			parent => <-.parent;
			y => <-.y;
			height => <-.height;
			width => parent.clientWidth;
		};
		UIlabel UIlabel {
			parent => <-.UIpanel;
			label => "Wind (COMPWIND) components:";
			y = 5;
			width => parent.clientWidth - x;
			alignment = "left";
		};
		string+OPort empty[1] => {"**none**"};
		FLD_MAP.concat_2_arrays concat_2_arrays {
			in1 => <-.empty;
			in2 => <-.params.var_names;
		};
		UIlist list_x {
			parent => <-.UIpanel;
			height = 170;
			strings => <-.concat_2_arrays.out;
			y => <-.UIlabel.y + <-.UIlabel.height;
			width => (parent.clientWidth / 3);
			selectedItem => <-.params.wind_components[0];
			listIsEditable = 0;
		};
		UIlist list_y {
			parent => <-.UIpanel;
			height => <-.list_x.height;
			strings => <-.concat_2_arrays.out;
			y => <-.list_x.y;
			x => (parent.clientWidth / 3);
			width => (parent.clientWidth / 3);
			selectedItem => <-.params.wind_components[1];
			listIsEditable = 0;
		};
		UIlist list_z {
			parent => <-.UIpanel;
			height => <-.list_y.height;
			strings => <-.concat_2_arrays.out;
			y => <-.list_y.y;
			x => ((2 * parent.clientWidth) / 3);
			width => (parent.clientWidth / 3);
			selectedItem => <-.params.wind_components[2];
			listIsEditable = 0;
		};
	};

	macro ReadVis5DUI {
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIlabel file_title {
			parent => <-.UImod_panel;
			label => "Vis5D file:";
			y = 0;
			alignment = "left";
		};
		UItext selected_file {
			parent => <-.UImod_panel;
			text<NEportLevels={2,2}> => <-.UIfileDialog.filename;
			y => <-.file_title.y + <-.file_title.height;
			width => parent.clientWidth - <-.browse_btn.width;
		};
		UIbutton browse_btn {
			parent => <-.UImod_panel;
			y => selected_file.y;
			label => "Browse";
			message = "Select the Vis5D file to be read";
			x => parent.clientWidth - width;
			width = 65;
			height => selected_file.height;
		};
		UIfileDialog UIfileDialog {
			visible => <-.browse_btn.do;
			title => "Select Vis5D file to read";
			searchPattern = "./*.v5d";
			filename => <-.params.filename;
		};
		UIlabel status {
			parent => <-.UImod_panel;
			height => <-.file_title.height*3;
			label => <-.params.status;
			y => <-.selected_file.y + <-.selected_file.height + 5;
			x => <-.file_title.x;
			width => (parent.clientWidth - x);
			alignment = "left";
		};
		UIlabel timestep_title {
			parent => <-.UImod_panel;
			label => "Available timesteps:";
			y => <-.status.y + <-.status.height;
			x => <-.file_title.x;
			width => (parent.clientWidth - <-.load_all.width);
			alignment = "left";
		};
		UItoggle load_all {
			parent => <-.UImod_panel;
			label => "Load all";
			x => (parent.clientWidth - width);
			y => <-.timestep_title.y;
			set => <-.params.load_all_steps;
		};
		UIlist timestep_list {
			parent => <-.UImod_panel;
			height = 190;
			strings => <-.params.time_steps;
			y => <-.timestep_title.y + <-.timestep_title.height;
			width => parent.clientWidth;
			selectedItem => <-.params.time_step;
			fontAttributes {
				family = "courier";
			};
			listIsEditable = 0;
		};
		LoopUI LoopUI {
			parent => <-.UImod_panel;
			params => <-.params;
			y => <-.timestep_list.y + <-.timestep_list.height;
		};
		UIlabel varlist_title {
			parent => <-.UImod_panel;
			label => "Variables:";
			y => <-.LoopUI.y + <-.LoopUI.height+5;
			alignment = "left";
		};
		int nvn => array_size(.params.var_names);
		int cnt[] => switch(nvn > 0,init_array(nvn,0,(nvn - 1)));
		UIlist var_list {
			parent => <-.UImod_panel;
			height = 170;
			strings => str_format("%03d: %s",cnt,<-.params.var_names);
			y => <-.varlist_title.y + <-.varlist_title.height;
			width => (parent.clientWidth / 2);
			fontAttributes {
				family = "courier";
			};
			listIsEditable = 0;
		};
		UIlabel var_details {
			parent => <-.UImod_panel;
			height => var_list.height;
			label => <-.params.var_info[<-.var_list.selectedItem];
			y => var_list.y;
			x => ((parent.clientWidth / 2) + 5);
			width => ((parent.clientWidth / 2) - 5);
			alignment = "left";
		};
		UItoggle title_visible {
			parent => <-.UImod_panel;
			y => <-.var_list.y + <-.var_list.height+5;
 			label => "Show Title";
			set => <-.params.show_title;
		};
		WindSelectUI WindSelectUI {
			y => <-.title_visible.y + <-.title_visible.height;
			parent => <-.UImod_panel;
			params => <-.params;
		};		
	};

	macro ReadVis5DFunc {
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DParams+IPort2 &params;
		string s[] => str_array(.params.time_steps[.params.time_step]," ");
		string+OPort titleString => switch(((array_size(.s) > 4) + 1)," ",((s[2] + "   ") + s[4]));
		Xform text<export_all=2> {
			int+IPort2 align_horiz = 1;
			int+IPort2 align_vert = 0;
			int+IPort2 drop_shadow = 0;
			int+IPort2 background = 0;
			int+IPort2 bounds = 0;
			int+IPort2 underline = 0;
			int+IPort2 lead_line = 0;
			int+IPort2 radial = 0;
			int+IPort2 do_offset = 0;
			float+IPort2 offset[3] = {0.05,0.05,0.};
			int xform_mode;
			string+IPort2 str => <-.titleString;
			int nspace = 3;
			float position[.nspace] = {0.,0.78,0.99};
			int+IPort2 stroke = 0;
			group StrokeTextAttribs {
				int font_type = 0;
				int style;
				int plane = 0;
				int orient = 0;
				int path = 0;
				int space_mode = 1;
				float spacing = 0.;
				float angle = 0.;
				float height = 1.;
				float expansion = 1.;
				float width;
			};
			float+write min_vec[.nspace];
			float+write max_vec[.nspace];
			xform<NEportLevels={0,2}>;
		};
		DefaultProps DefaultProps {
			font<NEportLevels={2,0}> = "-adobe-times-bold-r-*-*-40-*-*-*-*-*-*-*";
			inherit = 0;
		};
		GDpick_info DefaultPickInfo;
		DefaultObject DefaultObject {
			input => <-.text;
			xform => <-.text.xform;
			xform_mode = "Locked";
			props => <-.DefaultProps;
			pick_info => <-.DefaultPickInfo;
			name = "Vis5DtimestepTitle";
			visible => <-.params.show_title;
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DCore ReadVis5DCore{
			filename => <-.params.filename;
			load_all_steps => <-.params.load_all_steps;
			time_step => <-.params.time_step;
			time_steps => <-.params.time_steps;
			var_names => <-.params.var_names;
			var_info => <-.params.var_info;
			status => <-.params.status;
            wind_components => <-.params.wind_components;
		};
		GDM.DataObject fldDataObject{
			in => <-.ReadVis5DCore.fld;
		};
		MODS.set_xform set_xform {
			in_field1 => <-.ReadVis5DCore.vars2d;
			in_field2 => <-.ReadVis5DCore.fld;
		};
		olink vars3d_out => ReadVis5DCore.fld;
		olink vars2d_out => set_xform.out_fld;
		olink vars3d_obj => fldDataObject.obj;
		olink vars2d_obj => set_xform.out_obj;
		olink title => .DefaultObject;
	};

	macro ReadVis5D {
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DParams params;
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5DUI ReadVis5DUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5DFunc ReadVis5DFunc {
			params => <-.params;
		};
		olink filename => params.filename;
		olink vars3d_out => ReadVis5DFunc.vars3d_out;
		olink vars2d_out => ReadVis5DFunc.vars2d_out;
		olink vars3d_obj => ReadVis5DFunc.vars3d_obj;
		olink vars2d_obj => ReadVis5DFunc.vars2d_obj;
		olink title => ReadVis5DFunc.title;
	};


	macro ReadVis5DTopographyFunc {
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DTopographyParams+IPort2 &params;
		ilink Vis5Dfile;
		ilink Vis5Dxform;
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadTopoCore ReadTopoCore{
			filename => <-.Vis5Dfile;
			topofile => <-.params.topofile;
			subsampling => <-.params.subsampling;
			min_height => <-.params.min_height;	
		};
		link active => switch(params.show_topo,.ReadTopoCore.fld);
		MODS.set_xform set_xform {
			in_field1 => <-.active;
			in_field2 => <-.Vis5Dxform;
		};
		GDM.DataObject DataObject{
			in => <-.set_xform.out_fld;
			Datamap {
				DataRange[3] = {
					{
						DataMaxValue=-190.,,,,,,,,,,,,
						UIMaxValue=-190.,
						UIMinValue=-200.,,,,,,,,,,
					},
					{
						DataMaxValue=1000.,,,,,,,,,,,,
						UIMaxValue=1000.,
						UIMinValue=-190.,
						DataMinValue=><-.DataRange[0].DataMaxValue,
						controlPoints=>{
							DatamapValue[2],
							DatamapValue[3]
						},,,,,,,,
					},
					{
						UIMaxValue=1000.01,
						DataMaxValue=>dataMax,
						UIMinValue=1000.,
						DataMinValue=><-.DataRange[1].DataMaxValue,
						controlPoints=>{
							DatamapValue[4],
							DatamapValue[5]
						},,,,,,,,
					}
				};
				DatamapValue[6] = {,
					{
						v2=0.66,
					},
					{
						v4=1.,
						v3=1.,
						v2=0.26,
					},
					{
						v4=0.6,
						v3=1.,
						v2=0.05,
					},
					{
						v4=0.6,
						v3=1.,
						v2=0.05,
					},
					{
						v4=1.,
						v3=0.,
						v2=0.,
					}
				};
			};
		};
		olink out => set_xform.out_fld;
		olink obj => DataObject.obj;
	};

	macro ReadVis5DTopographyUI {
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DTopographyParams+IPort2 & params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIlabel file_title {
			parent => <-.UImod_panel;
			label => "Topography filename:";
			y = 0;
			width => parent.clientWidth;
			alignment = "left";
		};
		UItext UItext {
			parent => <-.UImod_panel;
			text => <-.UIfileDialog.filename;
			y => <-.file_title.y + <-.file_title.height;
			width => parent.clientWidth - <-.browse_btn.width;
		};
		UIbutton browse_btn {
			parent => <-.UImod_panel;
			y => UItext.y;
			label => "Browse";
			message = "Select the Topography file to be read";
			x => parent.clientWidth - width;
			height => UItext.height;
			width = 65;
		};
		UIfileDialog UIfileDialog {
			visible => <-.browse_btn.do;
			title => "Select Topography File";
			filename => <-.params.topofile;
			searchPattern = "./*.TOPO";
		};
		UIslider subsampling {
			parent => <-.UImod_panel;
			value => <-.params.subsampling;
			title => "Subsampling factor";
			width => parent.clientWidth;
			min = 1.;
			max = 10.;
			mode = "integer";
			decimalPoints = 0;
			y => <-.UItext.y + <-.UItext.height;
		};
		UItoggle UItoggle {
			parent => <-.UImod_panel;
			label => "Show Topography";
			set => <-.params.show_topo;
			y => <-.subsampling.y + <-.subsampling.height + 10;
			width = 160;
		};
		UIlabel min_sea_lvl_lbl {
			parent => <-.UImod_panel;
			label => "Min sea level:";
			y => <-.UItoggle.y + <-.UItoggle.height + 5;
			x => parent.clientWidth - width - <-.min_sea_lvl.width;
			height => <-.min_sea_lvl.height;
			alignment = "left";
		};
		UIfield min_sea_lvl {
			parent => <-.UImod_panel;
			value => <-.params.min_height;
			y => <-.min_sea_lvl_lbl.y;
			x => (parent.clientWidth - width);
			width = 50;
			decimalPoints = 0;
		};
	};

	macro ReadVis5DTopography {
		ilink Vis5Dfile;
		ilink Vis5Dxform;
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMods.ReadVis5DTopographyParams params {
			subsampling = 1;
			show_topo = 1;
			min_height = 0.0;
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5DTopographyUI ReadVis5DTopographyUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ReadVis5D.ReadVis5DMacs.ReadVis5DTopographyFunc ReadVis5DTopographyFunc {
			Vis5Dfile => <-.Vis5Dfile;
			Vis5Dxform => <-.Vis5Dxform;
			params => <-.params;
		};
		olink out => ReadVis5DTopographyFunc.out;
		olink obj => ReadVis5DTopographyFunc.obj;
	};
	
};


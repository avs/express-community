flibrary RegionGrowMacs{

	//RegionGrowUI
	//UI Macro
	//UImod_panel containing widgets

	macro copy_on_change_array {
		int+IPort2 dims;
		prim+IPort2 in_array[dims];
		macro copy_on_change_array_element[dims] {
			prim+IPort2 in => <-.in_array[index_of(copy_on_change_array_element)];
			prim+Port2 out => <-.out_array[index_of(copy_on_change_array_element)];			 
			GMOD.copy_on_change copy_on_change{
				input => <-.in;
				output => <-.out;
			};
		};
		prim+Port2 out_array[dims];		
	};

	macro VecCompPanel {
		ilink parent;
		int x = 0;
		int y = 0;
		int reset;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int+nres nvec_comp => switch(is_valid(params.vec_comp),array_size(params.vec_comp));
		int+nres height => UItoggle[nvec_comp-1].y_out+10;
		int+nres min_vec_comp => (min_array(params.combine_vec)!=2);
		int height_out => switch(min_vec_comp + 1,0,height+10);
		int y_next => switch((is_valid(nvec_comp) + 1),y,y+height_out);
		UIpanel UIpanel {
			parent => <-.parent;
			visible => switch((is_valid(<-.nvec_comp) + 1),0,min_vec_comp);
			x => <-.x;
			y => <-.y;
			width => parent.clientWidth;
			height => switch((is_valid(<-.nvec_comp) + 1),0,<-.height);
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel Magnitude_Vector_Components {
			parent => <-.UIframe;
			x = 0;
			y = 0;
			width => parent.clientWidth;		
		};
		UItoggle UItoggle [nvec_comp] {
			visible => (<-.params.combine_vec[index_of(UItoggle)]!=2);
			parent => <-.UIframe;
			label => <-.params.vec_comp[index_of(UItoggle)];
			width => parent.clientWidth;
			y => switch((index_of(UItoggle)>0)+1,Magnitude_Vector_Components.height + 10,UItoggle[index_of(UItoggle)-1].y_out);
			int y_out => switch(visible+1,y,y + height);
			set => <-.params.combine_vec[index_of(UItoggle)];
			do => <-.reset;
		};
	};

	macro OutputPanel {
		ilink parent;
		int x = 0;
		int y = 0;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int+nres height => Invert_Region.y+Invert_Region.height+10;
		int y_next => y+height+10;
		UIpanel UIpanel {
			parent => <-.parent;
			x => <-.x;
			y => <-.y;
			width => parent.clientWidth;
			height => <-.height;
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel Output {
			parent => <-.UIframe;
			x = 0;
			y = 0;
			width => parent.clientWidth;		
		};
		UIlabel Value {
			parent => <-.UIframe;
			x = 5;
			y => <-.Output.y + <-.Output.height+10;
			alignment = "left";
			height=> out_val.height;
			width => parent.clientWidth/3-5;		
		};
		UIfield out_val {
			parent => <-.UIframe;
			x => parent.clientWidth/3;
			y => <-.Value.y;
			value => <-.params.out_val;
			width => parent.clientWidth*2/3-5;		
			mode = "integer";
			nullString = "0";
		};
		UIlabel Label {
			parent => <-.UIframe;
			x = 5;
			alignment = "left";
			y => <-.Value.y + <-.Value.height+10;
			height=> out_label.height;
			width => parent.clientWidth/3-5;		
		};
		UItext out_label {
			parent => <-.UIframe;
			x => parent.clientWidth/3;
			y => <-.Label.y;
			width => parent.clientWidth*2/3-5;
			text => <-.params.out_label;		
		};
		UItoggle Use_Diagonals {
			parent => <-.UIframe;
			y => <-.Label.y + <-.Label.height+10;
			width => parent.clientWidth/2;
			set => <-.params.use_diagonals;
		};
		UItoggle AND_Precedence {
			parent => <-.UIframe;
			y => <-.Use_Diagonals.y;
			x => <-.Use_Diagonals.x + <-.Use_Diagonals.width;
			width => parent.clientWidth/2;
			set => <-.params.and_precedence;
		};
		UItoggle Set_Null_Value {
			parent => <-.UIframe;
			y => <-.Use_Diagonals.y + <-.Use_Diagonals.height;
			width => parent.clientWidth/2;
			set => <-.params.set_null_value;
		};
		UItoggle Invert_Region {
			parent => <-.UIframe;
			y => <-.Set_Null_Value.y;
			width => parent.clientWidth/2;
			x => <-.Set_Null_Value.x + <-.Set_Null_Value.width;	
			set => <-.params.invert_region;
		};
		GMOD.copy_on_change copy_on_change {
			input => str_format("Region_%i",<-.out_val.value);
			output => <-.params.out_label;
		};
	};

	macro SeedPanel {
		ilink parent;
		int in_seed[ndim];
		int x = 0;
		int y = 0;
		int+nres height => Seed_Point.height+10+(ndim*(UIslider.height+10));
		string slider_label [3] => {"x","y","z"};
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int+nres ndim => switch(is_valid(params.dims),array_size(params.dims));
		int+nres dims[] => params.dims;
		int height_out => switch(( ndim != 0) + 1,0,height+10);
		int y_next => switch((is_valid(ndim) + 1),y,y+height_out);
		UIpanel UIpanel {
			parent => <-.parent;
			visible => switch((is_valid(<-.ndim) + 1),0,<-.ndim);
			x => <-.x;
			y => <-.y;
			width => parent.clientWidth;
			height => switch((is_valid(<-.ndim) + 1),0,<-.height);
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel Seed_Point {
			parent => <-.UIframe;
			x = 0;
			y = 0;
			width => parent.clientWidth;		
		};
		UIslider UIslider [ndim] {
			parent => <-.UIframe;
			value => params.seed[index_of(UIslider)];
			title => slider_label[index_of(UIslider)];
			y => Seed_Point.height + 10 + index_of(UIslider) * (height + 10);
			width => parent.clientWidth;
			max => dims[index_of(UIslider)]-1;
			mode = "integer";
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_seed {
			dims => <-.ndim;
			in_array => switch(is_valid(<-.in_seed)+1,<-.dims/2,<-.in_seed);
			out_array => <-.params.seed;
		};
	};

	macro CombineItem{
		string+IPort2 label;
		ilink parent;
		int x = 0;
		int y = 0;
		int sel_and=0;
		int sel_or=0;
		int sel_and_or=0;
		int reset;
		
		GMOD.copy_on_change reset_and{
			trigger => <-.reset;
			input => 0;
			output => <-.sel_and;
		};		
		GMOD.copy_on_change reset_or{
			trigger => <-.reset;
			input => 0;
			output => <-.sel_or;
		};		
		UIlabel Name{
			parent => <-.parent;
			y => <-.y;
			label => <-.label;
			width => (parent.clientWidth)*2/3;
			alignment => "left";
		};
		UItoggle tog_and {
			parent => <-.parent;
			label = " ";
			width => parent.clientWidth/6-10;
			x => parent.clientWidth*2/3+5;
			y => <-.Name.y;
			set => <-.sel_and;
		};
		GMOD.parse_v parse_and {
			v_commands = "sel_or=0;";
			trigger => <-.tog_and.do;
			relative => <-;
		};
		UItoggle tog_or {
			parent => <-.parent;
			label = " ";
			width => parent.clientWidth/6-10;
			x => parent.clientWidth*5/6+5;
			y => <-.Name.y;
			set => <-.sel_or;
		};
		GMOD.parse_v parse_or {
			v_commands = "sel_and=0;";
			trigger => <-.tog_or.do;
			relative => <-;
		};
		int and_or => (2*sel_or)+sel_and;
		GMOD.copy_on_change combine{
			input => <-.and_or;
			output => <-.sel_and_or;
		};
		int height => Name.height;
	};

	macro CombinePanel {
		ilink parent;
		int x = 0;
		int y = 0;
		int reset;
		int+nres height => Component.height+10+(veclen*(CombineItem.height))+10;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int veclen => switch(is_valid(params.delta),array_size(params.delta));
		int height_out => switch((veclen != 0) + 1,0,height+10);
		int y_next => switch((is_valid(veclen) + 1),y,y+height_out);
		string slider_label [veclen] => params.nd_labels;
		int my_and_or[veclen] => params.combine_and_or;
		UIpanel UIpanel {
			parent => <-.parent;
			visible => switch((is_valid(<-.veclen) + 1),0,<-.veclen);
			x => <-.x;
			y => <-.y;
			width => parent.clientWidth;
			height => switch((is_valid(<-.veclen) + 1),0,<-.height);
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel Component {
			parent => <-.UIframe;
			x = 0;
			y = 0;
			width => parent.clientWidth*2/3;		
		};
		UIlabel Label_and {
			parent => <-.UIframe;
			x => parent.clientWidth*2/3+5;
			y => <-.Component.y;
			label="AND";
			width => parent.clientWidth*1/6;		
		};
		UIlabel Label_or {
			parent => <-.UIframe;
			x => parent.clientWidth*5/6+5;
			y => <-.Component.y;
			label="OR";
			width => parent.clientWidth*1/6;		
		};
		CombineItem CombineItem [veclen] {
			reset => <-.reset;
			parent => <-.UIframe;
			sel_and_or => <-.my_and_or[index_of(CombineItem)];
			label => <-.slider_label[index_of(CombineItem)];
			y => <-.Component.height + 10 + index_of(CombineItem) * (height);
		};
	};

	macro DeltaPanel {
		ilink parent;
		int x = 0;
		int y = 0;
		int+nres height => Delta.height+10+(veclen*(UIslider.height+10));
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int+nres veclen => switch(is_valid(params.delta),array_size(params.delta));
		int height_out => switch((veclen != 0) + 1,0,height+10);
		int y_next => switch((is_valid(veclen) + 1),y,y+height_out);
		string slider_label[veclen] => params.nd_labels;
		float min_vec[veclen] => params.min_vec;
		float max_vec[veclen] => params.max_vec;	
		int incr[veclen] => params.incremental;	
		float delta[veclen] => params.delta;
		UIpanel UIpanel {
			parent => <-.parent;
			visible => switch((is_valid(<-.veclen) + 1),0,<-.veclen);
			x => <-.x;
			y => <-.y;
			width => parent.clientWidth;
			height => switch((is_valid(<-.veclen) + 1),0,<-.height);
		};
		UIframe UIframe {
			parent => <-.UIpanel;
			y = 0;
			width => parent.clientWidth;
			height => parent.clientHeight;
		};
		UIlabel Delta {
			parent => <-.UIframe;
			x = 0;
			y = 0;
			width => parent.clientWidth*2/3;		
		};
		UIlabel Incr {
			parent => <-.UIframe;
			x => <-.Delta.x+<-.Delta.width;
			y => <-.Delta.y;
			label="Incremental?";
			width => parent.clientWidth*1/3;		
		};
		UIslider UIslider [veclen] {
			parent => <-.UIframe;
			value => <-.delta[index_of(UIslider)];
			title => <-.slider_label[index_of(UIslider)];
			y => <-.Delta.height + 10 + index_of(UIslider) * (height + 10);
			width => parent.clientWidth*2/3;
			min => 0;
			max => <-.max_vec[index_of(UIslider)] - <-.min_vec[index_of(UIslider)];
		};
		UItoggle UItoggle [veclen] {
			parent => <-.UIframe;
			label = " ";
			width => parent.clientWidth/6;
			x => parent.clientWidth*5/6-10;
			y => <-.UIslider[index_of(UItoggle)].y +(<-.UIslider[index_of(UItoggle)].height-height)/2;
			set => <-.incr[index_of(UItoggle)];
		};
	};

	macro region_magnitude {
		Node_Data+IPort2 &in_data;
		int+nres+IPort2 do[]=> combine_vec;
		int+nres active => ((in_data.nnode_data == 1) && (in_data.node_data[0].veclen==1));
		Node_Data tmp_data[in_data.nnode_data] {
			nnode_data+nres => switch(<-.do[index_of(tmp_data)]+1,in_data.node_data[index_of(tmp_data)].veclen,1,1);
			nnodes => <-.in_data.nnodes;
			node_data {
				veclen = 1;
				values+nres => switch(<-.<-.do[index_of(tmp_data)]+1,
					in_data.node_data[index_of(tmp_data)].values[][index_of(node_data)],
					magnitude(in_data.node_data[index_of(tmp_data)].values),
					in_data.node_data[index_of(tmp_data)].values
				);
				labels+nres => switch(<-.<-.do[index_of(tmp_data)]+1,
					str_format("%s_comp_%i",in_data.node_data[index_of(tmp_data)].labels,index_of(node_data)),
					"magnitude_"+in_data.node_data[index_of(tmp_data)].labels,
					in_data.node_data[index_of(tmp_data)].labels
				);
			};
		};
		macro flat[in_data.nnode_data] {
			Node_Data+IPort2 &in_data => tmp_data[index_of(flat)];
			Node_Data+OPort2 out_data[in_data.nnode_data] {
				nnode_data=1;
				nnodes => <-.in_data.nnodes;
				&node_data => in_data.node_data[index_of(out_data)];
			};
			int arr [in_data.nnode_data]=>init_array(in_data.nnode_data,0,in_data.nnode_data-1);
			int arr1 [in_data.nnode_data]=>init_array(in_data.nnode_data,index_of(flat),index_of(flat));
		};
		FLD_MAP.concat_2_arrays cat[in_data.nnode_data-1] {
			in1+nres=>switch((index_of(cat)!=0)+1,<-.flat[index_of(cat)].arr,<-.cat[index_of(cat)-1].out);
			in2=><-.flat[index_of(cat)+1].arr;
		};
		int out_cat[]=>cat[in_data.nnode_data-2].out;
		FLD_MAP.concat_2_arrays cat1[in_data.nnode_data-1] {
			in1+nres=>switch((index_of(cat1)!=0)+1,<-.flat[index_of(cat1)].arr1,<-.cat1[index_of(cat1)-1].out);
			in2=><-.flat[index_of(cat1)+1].arr1;
		};
		int out_cat1[]=>cat1[in_data.nnode_data-2].out;

		Node_Data cat_data[array_size(out_cat)] {
			nnodes => <-.in_data.nnodes;
			nnode_data = 1;
			&node_data => <-.flat[<-.out_cat1[index_of(cat_data)]].out_data[<-.out_cat[index_of(cat_data)]].node_data;		
		};
		FLD_MAP.combine_node_datas combine_node_datas {
			in => <-.cat_data;
		};
		Node_Data+nres+OPort2 &out_data => switch(active+1,combine_node_datas.out,in_data);	
	};	

	macro RegionMagnitudeFunc {
		Node_Data+IPort2 &in_data;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		int+nres nnode_data => in_data.nnode_data;
		int nnodes => in_data.nnodes;
		int+nres sel_array[nnode_data] => switch((is_valid(in_data)),2-(2*(in_data.node_data.veclen>1)));
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_combine_vec {
			dims => <-.nnode_data;
			in_array => <-.sel_array;
			out_array => <-.params.combine_vec;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_mag_names {
			dims => <-.nnode_data;
			in_array+nres => <-.in_data.node_data.labels;
			out_array => <-.params.vec_comp;
		};		
		region_magnitude region_magnitude {
			in_data => <-.in_data;
			do => <-.params.combine_vec;					
		};
		int out_nnode_data => out_data.nnode_data;
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_nd_labels {
			dims => <-.out_nnode_data;
			in_array => <-.out_data.node_data.labels;
			out_array => <-.params.nd_labels;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_min {
			dims => <-.out_nnode_data;
			in_array => <-.out_data.node_data.min;
			out_array => <-.params.min_vec;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_max {
			dims => <-.out_nnode_data;
			in_array => <-.out_data.node_data.max;
			out_array => <-.params.max_vec;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_delta {
			dims => <-.out_nnode_data;
			in_array => (<-.params.max_vec - <-.params.min_vec)/2;
			out_array => <-.params.delta;
		};
		int incr[out_nnode_data]=>init_array(out_nnode_data,0,0);
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_incr {
			dims => <-.out_nnode_data;
			in_array => <-.incr;
			out_array => <-.params.incremental;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.copy_on_change_array coc_combine_and_or {
			dims => <-.out_nnode_data;
			in_array => <-.incr;
			out_array => <-.params.combine_and_or;
		};
		olink out_data => region_magnitude.out_data;
	};

	macro RegionUI {
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		ilink in_seed;
		int reset_combine;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);	
		};
		SeedPanel SeedPanel {
			parent => <-.UImod_panel;
			in_seed => <-.in_seed;
			params => <-.params;
		};
		OutputPanel OutputPanel{
			y => <-.SeedPanel.y_next;
			parent => <-.UImod_panel;
			params => <-.params;
		};
		VecCompPanel VecCompPanel{
			y => <-.OutputPanel.y_next;
			reset => <-.reset_combine;
			parent => <-.UImod_panel;
			params => <-.params;
		};
		CombinePanel CombinePanel {
			y => <-.VecCompPanel.y_next;
			reset => <-.reset_combine;
			parent => <-.UImod_panel;
			params => <-.params;
		};
		DeltaPanel DeltaPanel {
			y => <-.CombinePanel.y_next;
			parent => <-.UImod_panel;
			params => <-.params;
		};
	};		

	macro AddNodeData {
		Mesh+Node_Data+IPort2 &in;
		Node_Data+IPort2 &data {
			nnodes => <-.in.nnodes;
			nnode_data=1;
		};
		Node_Data+OPort tmp_data[in.nnode_data+1] {
			nnode_data=1;
			nnodes => <-.in.nnodes;
			&node_data => switch ((index_of(tmp_data)!=(<-.in.nnode_data))+1,
								   <-.data.node_data[0],
								   <-.in.node_data[index_of(tmp_data)]);
		};
		FLD_MAP.combine_node_datas combine_node_datas {
			in => <-.tmp_data;
		};
		FLD_MAP.combine_mesh_data combine_mesh_data {
			in_mesh => <-.in;
			in_nd => combine_node_datas.out;
		};
		olink out => combine_mesh_data.out;
		olink obj => combine_mesh_data.obj;
	};

	macro RegionGrowFunc {
		Mesh+Node_Data+IPort2 &in_data;
		Node_Data+IPort2 &calc_data;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams+IPort2 &params;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowCore RegionGrowCore{
			dims => <-.params.dims;
			in => <-.calc_data;
			seed => <-.params.seed;
			delta => <-.params.delta;
			use_diagonals => <-.params.use_diagonals;
			out_val => <-.params.out_val;
			incremental => <-.params.incremental;
			set_null_value => <-.params.set_null_value;
			combine_and_or => <-.params.combine_and_or;
			out_label => <-.params.out_label;
			invert_region => <-.params.invert_region;
			and_precedence => <-.params.and_precedence;
		};		
		XP_COMM_PROJ.Region.RegionGrowMacs.AddNodeData AddNodeData {
			in => <-.in_data;
			data => <-.RegionGrowCore.out;
		};
		FLD_MAP.combine_mesh_data combine_mesh_data {
			in_mesh => <-.in_data;
			in_nd => <-.RegionGrowCore.out;
		};
		olink out => AddNodeData.out;
		olink obj => combine_mesh_data.obj;
	};

	macro RegionGrow {
		Mesh+Node_Data+IPort2+nres &in_data;
		int+IPort2 seed[];
		int+nres in_dims[] => in_data.dims;
		XP_COMM_PROJ.Region.RegionGrowMods.RegionGrowParams params {
			dims+nres => <-.in_dims;
			out_val = 1;
			use_diagonals = 0;
			set_null_value = 0;
			invert_region = 0;
			and_precedence = 0;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.RegionUI RegionUI {
			params => <-.params;
			in_seed => <-.seed;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.RegionMagnitudeFunc RegionMagnitudeFunc {
			in_data => <-.in_data;		
			params => <-.params;
		};
		XP_COMM_PROJ.Region.RegionGrowMacs.RegionGrowFunc RegionGrowFunc {
			in_data => <-.in_data;		
			calc_data => RegionMagnitudeFunc.out_data;		
			params => <-.params;
		};
		olink out => RegionGrowFunc.out;
		olink obj => RegionGrowFunc.obj;
	};	
};





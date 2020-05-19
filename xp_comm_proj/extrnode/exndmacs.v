
flibrary ExtractNodeDataMacs<compile_subs=0> {

	macro ExtractNodeDataFunc 
	{
		ilink in;
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMods.ExtractNodeDataParams+IPort2 &params;
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMods.Extract_Node_Data Extract_Node_Data
		{
			input_data => in;
			params => <-.params;
		};
		olink out => Extract_Node_Data.output;
	};

	macro ExtractNodeDataUI
	{
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMods.ExtractNodeDataParams &params<NEportLevels={2,0}>;
		UImod_panel UImod_panel
		{
			title => name_of(<-.<-.<-,1);
		};
		UIslider x {
			parent => <-.UImod_panel;
			y = 0;
			width => parent.clientWidth;
			mode = "integer";
			max => <-.params.dims[0]-1;
			value = 0.;
		};
		UIslider y
		{
			parent => <-.UImod_panel;
			y => ((<-.x.y + <-.x.height) + 10);
			width => parent.clientWidth;
			mode = "integer";
			max => <-.params.dims[1]-1;
			value = 0.;
		};
		UIslider z
		{
			parent => <-.UImod_panel;
			y => ((<-.y.y + <-.y.height) + 10);
			width => parent.clientWidth;
			mode = "integer";
			max => <-.params.dims[2]-1;
			value = 0.;
		};
		UIslider Offset
		{
			parent => <-.UImod_panel;
			value => <-.params.offset;
			y => ((<-.z.y + <-.z.height) + 10);
			width => parent.clientWidth;
			mode = "integer";
			max => <-.params.nnode_data-1;
		};
		UIlabel Data_value
		{
			parent => <-.UImod_panel;
			y => ((<-.Offset.y + <-.Offset.height) + 10);
			width => parent.clientWidth/2-5;
			alignment = "left";
		};
		UIfield Value
		{
			parent => <-.UImod_panel;
			y => ((<-.Offset.y + <-.Offset.height) + 10);
			x => <-.Data_value.x + <-.Data_value.width + 10;
			width => parent.clientWidth/2-5;
			value => <-.params.out;
			outputOnly = 1;
		};
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMods.CombineCoordinateIndex CombineCoordinateIndex
		{
			x_index => <-.x.value;
			y_index => <-.y.value;
			z_index => <-.z.value;
			dims => <-.params.dims;			
			output => <-.params.index;			
		};		
	};

	macro ExtractNodeData 
	{
		Mesh_Unif+Node_Data+IPort2 &in;
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMods.ExtractNodeDataParams params
		{
			dims => <-.in.dims;
			nnode_data => <-.in.nnode_data;
			out => <-.out[0][0];
			node_index => {index};
			offset = 0;
		};
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMacs.ExtractNodeDataFunc ExtractNodeDataFunc
		{
			in => <-.in;
			params => <-.params;
		};
		XP_COMM_PROJ.ExtractNodeData.ExtractNodeDataMacs.ExtractNodeDataUI ExtractNodeDataUI
		{
			params => <-.params;
		};
		olink out => ExtractNodeDataFunc.out;
	};
	
};

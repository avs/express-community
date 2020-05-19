
flibrary CombineCoordinateIndexMacs<compile_subs=0> {
	macro CombineCoordinateIndexFunc {
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMods.CombineCoordinateIndexParams &params<NEportLevels={2,0}>;
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMods.CombineCoordinateIndex CombineCoordinateIndex
		{
			x_index => <-.params.x; 
			y_index => <-.params.y; 
			z_index => <-.params.z; 
			dims => <-.params.dims;
		};
		olink out => CombineCoordinateIndex.output;
	};
	macro CombineCoordinateIndexUI {
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMods.CombineCoordinateIndexParams &params<NEportLevels={2,0}>;
		UImod_panel UImod_panel
		{
			title => name_of(<-.<-.<-,1);
		};
		UIslider x {
			parent => <-.UImod_panel;
			value = 29.;
			y = 0;
			width => parent.clientWidth;
			mode = "integer";
			value => <-.params.x;
			max => <-.params.dims[0];
		};
		UIslider y
		{
			parent => <-.UImod_panel;
			value = 63.;
			y => ((<-.x.y + <-.x.height) + 10);
			width => parent.clientWidth;
			mode = "integer";
			value => <-.params.y;
			max => <-.params.dims[1];
		};
		UIslider z
		{
			parent => <-.UImod_panel;
			value = 48.;
			y => ((<-.y.y + <-.y.height) + 10);
			width => parent.clientWidth;
			mode = "integer";
			value => <-.params.z;
			max => <-.params.dims[2];
		};		
	};
	macro CombineCoordinateIndex {
		ilink in_fld;
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMods.CombineCoordinateIndexParams params
		{
			dims+nres => <-.in_fld.dims;
		};
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMacs.CombineCoordinateIndexUI CombineCoordinateIndexUI
		{
			params => <-.params; 
		};
		XP_COMM_PROJ.CombineCoordinateIndex.CombineCoordinateIndexMacs.CombineCoordinateIndexFunc CombineCoordinateIndexFunc
		{
			params => <-.params;
		};
		olink out => CombineCoordinateIndexFunc.out;
	};
	
};

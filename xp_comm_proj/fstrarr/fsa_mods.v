
flibrary FindStringInArrayMods
		<build_dir="xp_comm_proj/fstrarr",
		out_hdr_file="gen.h",
		out_src_file="gen.cxx">
{
	//define parameter block group
	group FindStringInArrayParams<NEportLevels={0,1}>
	{
		int active<NEportLevels={2,0}>;
		int ignore_whitespace<NEportLevels={2,0}>;
		string search_string<NEportLevels={2,0}>;
	};
	
	//define low-level module
	module FindStringInArrayCore<src_file="mod_fsa.cxx">
	{
		int active<NEportLevels={2,0}>;
		int ignore_whitespace<NEportLevels={2,0}>;
		string search_string<NEportLevels={2,0}>;
		string strings<NEportLevels={2,0}>[];
		cxxmethod+notify_inst+req update
		(
			active+read+req,
			ignore_whitespace+read+notify+req,
			search_string+read+notify+req,
			strings+read+notify+req,
			selectedItem+write
		);
		int selectedItem<NEportLevels={0,2}>;
	};
};

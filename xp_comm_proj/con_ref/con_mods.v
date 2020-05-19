flibrary ConnectRefToArrayMods
		<build_dir="xp_comm_proj/con_ref",
		 out_src_file="gen.cxx",
		 out_hdr_file="gen.h">		 
{
	//define parameter block group
	group ConnectRefToArrayParams<NEportLevels={0,1}> {
		int value<NEportLevels={1,0}>;
		int max;
	};
	  
	//define low-level module
	module ConnectRefToArrayCore <src_file="con_ref.cxx">
	{
		group &ArrayOfIDs<NEportLevels={2,0}>[];
		int SelectedIndex<NEportLevels={2,0}>;
		cxxmethod+notify_inst+req connect(
			ArrayOfIDs+read+notify+req,
			SelectedIndex+read+notify+req,
			RefToConnect+write);
		group &RefToConnect<NEportLevels={2,0}>;
	};
};





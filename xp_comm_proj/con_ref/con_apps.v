flibrary ConnectRefToArrayApps<compile_subs=0>
{
	//Sample app
	APPS.DefaultApplication ConnectRefToArrayEg
	{
		group group<NEportLevels={0,1}>
		{
			int int = 11;
		};
		group group1<NEportLevels={0,1}>
		{
			float float = 22.37347;
		};
		group group2<NEportLevels={0,1}>
		{
			double double = 993734.8335;
		};
		group group3<NEportLevels={0,1}>
		{
			short short = 12;
		};
		group group4<NEportLevels={0,1}>
		{
			byte byte = 12;
		};
		group &group_ref<NEportLevels={1,1}>[] => 
		{
			.group,.group1,.group2,.group3,.group4
		};
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMacs.ConnectRefToArray ConnectRefToArray
		{
			input_in => <-.group_ref;
		};
		group &output1<NEportLevels={1,0}>=>.ConnectRefToArray.out_ref;
	};
};

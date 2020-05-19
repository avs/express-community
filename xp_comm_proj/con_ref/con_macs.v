
flibrary ConnectRefToArrayMacs<compile_subs=0>{

    macro ConnectRefToArrayFunc {
		ilink input_in;
		ilink output_in;
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMods.ConnectRefToArrayParams &params<NEportLevels={2,0}>;
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMods.ConnectRefToArrayCore ConnectRefToArrayCore{
			RefToConnect<NEportLevels={2,3}>;
			SelectedIndex => <-.params.value;
			ArrayOfIDs => <-.input_in;
			RefToConnect => <-.output_in;
		};	
		
	};

	macro ConnectRefToArrayUI {
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMods.ConnectRefToArrayParams &params<NEportLevels={2,0}>;
		UIslider UIslider {
			parent => <-.UImod_panel;
			max+nres => array_size(<-.<-.input_in)-1;
			min = 0;
			value => <-.params.value;
			mode = "integer";
			title = "Index Size";
		};
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-);
		};
	};
     
	macro ConnectRefToArray{
		ilink input_in;
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMods.ConnectRefToArrayParams params;
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMacs.ConnectRefToArrayUI ConnectRefToArrayUI
		{
			params => <-.params;
		};
		XP_COMM_PROJ.ConnectRefToArray.ConnectRefToArrayMacs.ConnectRefToArrayFunc ConnectRefToArrayFunc
		{
			params => <-.params;
			input_in => <-.input_in;
			output_in<NEportLevels={3,1}>;
		};
		link out_ref<NEportLevels={1,2}> => .ConnectRefToArrayFunc.ConnectRefToArrayCore.RefToConnect;
	};

	
};

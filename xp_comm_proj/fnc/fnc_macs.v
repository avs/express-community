flibrary FilterNodeMacs {

	macro FilterNodeConnectFunc{
		
		ilink in;
		int+IPort2 active;	

		XP_COMM_PROJ.FilterNodeConnect.FilterNodeMods.FilterNodeConnectCore FilterNodeConnectCore{
				in => <-.in;
				active => <-.active;
		};

		olink out => switch(active+1,in,.FilterNodeConnectCore.out);
		GDM.DataObject DataObject {
			in => <-.out;
		};
		olink obj => .DataObject.obj;
		
	};
	
	macro FilterNodeConnect{
		
		ilink in;
		int+IPort2 active=1;
		
		XP_COMM_PROJ.FilterNodeConnect.FilterNodeMacs.FilterNodeConnectFunc FilterNodeConnectFunc{
			in => <-.in;
			active => <-.active;
		};
		olink out => .FilterNodeConnectFunc.out;
		olink obj => .FilterNodeConnectFunc.obj;
		
	};
	
};
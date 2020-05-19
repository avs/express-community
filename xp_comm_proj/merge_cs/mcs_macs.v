flibrary MergeCellSetsMacs  {
	macro MergeCellSets {
		ilink in;
		XP_COMM_PROJ.MergeCellSets.MergeCellSetsMods.MergeCellSetsCore MergeCellSetsCore{ 
			in => <-.in;
		};
		olink out => MergeCellSetsCore.out;
		GDM.DataObject DataObject {
			in => <-.MergeCellSetsCore.out;
		};
		olink obj => DataObject.obj;
	};
};


flibrary Convert2TriStripMacs <compile_subs=0> {

	macro Convert2TriStripFunc {
		ilink in_fld;
		XP_COMM_PROJ.Convert2TriStrip.Convert2TriStripMods.Convert2TriStripCore Convert2TriStripCore {
			in => <-.in_fld;
		};
		olink out => Convert2TriStripCore.output;
	};

	macro Convert2TriStrip {
		ilink in_fld;
		XP_COMM_PROJ.Convert2TriStrip.Convert2TriStripMacs.Convert2TriStripFunc Convert2TriStripFunc {
			in_fld => <-.in_fld;		
		};
		GDM.DataObject DataObject{
		   in => <-.Convert2TriStripFunc.out;
		};
		olink out => Convert2TriStripFunc.out;
		olink out_obj => DataObject.obj;
	};

};

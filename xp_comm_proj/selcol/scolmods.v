
flibrary SelectColumnMods<build_dir="xp_comm_proj/selcol", out_hdr_file="gen.h"> {
  
	group+OPort SelectColumnParams {

		string labels[];
		int selected[];
	};
       
	module SelectColumnCore<src_file="selcol.c"> {
		Data_Array &Columns_In<NEportLevels={2,0}>[];
		int selected<NEportLevels={2,0}>[];
		omethod+notify_inst+req update(
			Columns_In+read+notify+req,
			selected+read+notify,
			labels+write,
			Columns_Out+write
		) = "SelectColumnCore";
		string labels<NEportLevels={2,0}>[];
		Data_Array &Columns_Out<NEportLevels={0,2}>[];
	};

};

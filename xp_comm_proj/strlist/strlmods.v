
flibrary StringListMods<build_dir="xp_comm_proj/strlist",out_hdr_file="gen.h"> 
{

	group+OPort StringListParams{
		int+Port2 index;
		int+Port2 append;
		int+Port2 insert;
		int+Port2 delete;
	};


	module StringListCore<src_file="strlist.c"> {

		string+Port2 list[];
		string+IPort2 item;
		StringListParams+IPort2 &params;
		int+IPort2 index => params.index;
		int+IPort2 append => params.append;
		int+IPort2 insert => params.insert;
		int+IPort2 delete => params.delete;
		omethod+notify_inst+req Update(
			.list+read+write,
			.item+read,
			.index+read,
			.append+read+write+notify,
			.insert+read+write+notify,
			.append+read+write+notify,
			.delete+read+write+notify
		) = "StringListUpdate";
	};
};

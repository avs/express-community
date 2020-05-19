
flibrary DVdownsize_scatMods<build_dir="xp_comm_proj/dvdown",
						out_hdr_file="gen.h"> 
{
	group DVdownsizeParams {
		int factor;
	};

	module DVdownsize_core<src_file="dvdown.c"> 
	{
		omethod+notify_inst+req upd_downsize(
							.in+read+notify+req,
							.factor+read+notify,
							.one_factor+read+req+notify,
							.out+write) = "DVdownsize_scat_update";
		Mesh+Node_Data &in<NEportLevels={2,0}> 
		{
			xform+nonotify;
			nnodes+req;
		};
		float+nres factor<NEportLevels={2,0}>[];
		int+nres one_factor<NEportLevels={0,0}> => factor[0];
		Mesh+Node_Data out<NEportLevels={0,2}> 
		{
			&xform+nowrite => in.xform;
		};
	};
};

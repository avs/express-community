flibrary NurbsMods<build_dir="xp_comm_proj/nurbs",
	  	   out_hdr_file="gen_nurbs.h",
		   out_scr_file="gen_nurbs.cxx">
{            

	ptr nurbs_type<NEnumColors=3,NEcolor0=15761408,NEcolor1=8453888,NEcolor2=4202734>;

	group Load_Params<NEportLevels={0,1}> 
	{
		string Filename<NEportLevels={1,0}>;
	};
	
	group Field_Params<NEportLevels={0,1}> 
	{
		float nu<NEportLevels={1,0}>;
		float nv<NEportLevels={1,0}>;
	};

	group Mesh_Params<NEportLevels={0,1}> 
	{
		int do<NEportLevels={1,0}>;
	};
	
	module rd_nurbs<src_file="rd_nurbs.c",
					libdeps="FLD",
					cxx_hdr_files="fld/Xfld.h">
	{
		string+read+req filename<NEportLevels={2,0}>;
		omethod+notify_val+notify_inst update = "rd_nurbs";
		nurbs_type+nonotify &nurbs<NEportLevels={1,2}>;
	};
	
	module nurbs_to_field<src_file="nurbs2f.c",
						 libdeps="FLD",
						 cxx_hdr_files="fld/Xfld.h">
	{
		ptr+read+notify+req &nurbs<NEportLevels={2,1}>;
		int+read+req nu<NEportLevels={2,0}>;
		int+read+req nv<NEportLevels={2,0}>;
		Mesh_Struct+Node_Data+write+nonotify out<NEcolor1=65280,NEportLevels={0,2}> 
		{
			ndim = 2;
			nspace = 3;
		};
		omethod+notify_val+notify_inst update = "nurbs2f";
	};
	
	module nurbs_ctrl_to_mesh<src_file="nurbs2m.c",
							  libdeps="FLD",
							  cxx_hdr_files="fld/Xfld.h"> 
	{
		nurbs_type+read+notify+req &nurbs<NEportLevels={2,1}>;
		int+read+req show_ctrl<NEportLevels={3,0}>;
		Mesh_Struct+Node_Data+write+nonotify out2<NEcolor1=65280,NEportLevels={0,2}> 
		{
			ndim = 2;
			nspace = 3;
		};
		omethod+notify_val+notify_inst update = "nurbs2m";
	};
	
	module nurbs_utilities<src_file="nurbutil.c"> {
		omethod update = "nurbs_util";
	};

};

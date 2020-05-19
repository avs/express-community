
flibrary Convert2TriStripMods <
	build_dir="xp_comm_proj/cvt2tri",
	libimm=1,
	out_hdr_file="gen.h">
{
	// module: ConvertToTriStrip
	// purpose: Convert input mesh to tri strip for faster rendering
	//
	module Convert2TriStripCore <src_file="cvt2tri.c"> {
		omethod+notify_inst+req Update(
			.in+notify+read+req,
			.output+write
		) = "CvtToPolyTri";
		Mesh+Node_Data &in<export=2>;
		Mesh+Node_Data output<export=2>;
	};

	
};

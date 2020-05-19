flibrary ReadPolygonMods <build_dir="xp_comm_proj/rd_poly",
							out_hdr_file="gen.h"> {
	
	//Parameter block

	group+OPort ReadPolygonParams {
		string+Port2 filename;
	};

	module ReadPolygonCore<src_file="rd_poly.c"> {
		ReadPolygonParams+IPort2 &params ;
		string+read+notify+req filename => .params.filename;
		omethod+notify_inst+req read_polygon(filename+read+notify+req,out+write) = "readPolygonCore";
		Mesh+Node_Data+OPort2 out;
	};
};

module tubes<src_file="makeTubes.c",
             out_hdr_file="Tube_gen.h",
             out_src_file="Tube_gen.cxx",
             build_dir="xp_comm_proj/stm3/tube"> {

	Mesh+read+notify+req        &in<NEportLevels={2,0}> {
		xform+nonotify;
	};
	float+read+notify+req       bondradius<export=2,NEportLevels={0,1}> = .1;
	int+read+notify+req         facets<export=2,NEportLevels={2,0}> = 16;

	Mesh+Node_Data+Float+write  out<NEportLevels={0,2}> {
		&xform+nowrite+nonotify => <-.in.xform;
	};
	omethod+notify+req update = "makeTubes";
};

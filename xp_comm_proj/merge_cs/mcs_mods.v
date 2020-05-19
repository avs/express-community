flibrary MergeCellSetsMods <cxx_name="", build_dir="xp_comm_proj/merge_cs",
							out_src_file="gen.cxx",out_hdr_file="gen.h"> {

	module MergeCellSetsCore<src_file="merge_cs.cxx",
		cxx_hdr_files="fld/Xfld.h" > {


		Field+IPort2 &in {
			xform+nonotify;
		};
		cxxmethod+req MergeCellSetsCore<status=1>(
			.in+read+notify+req,
			.out+write
		);
		Field+OPort2 out {
			nnodes+nowrite => <-.in.nnodes;
			nspace+nowrite => <-.in.nspace;
			nnode_data+nowrite => <-.in.nnode_data;
			&node_data+nowrite => <-.in.node_data;
			coordinates.values+nowrite => <-.<-.in.coordinates.values;
			&xform+nowrite => <-.in.xform;
		};
	};
};


flibrary TubesMods <build_dir="xp_comm_proj/tubes",
					  out_src_file="gen.cxx",
					  out_hdr_file="gen.hxx"> {

// custom mesh with optional cell and node data
Cell_Set VIScell_set {
	int+opt+notify ncell_data;
	Data_Array+opt+notify cell_data[.ncell_data];
};

Grid+Xform VISmesh {
	int+req+notify ncell_sets;
	VIScell_set cell_set[.ncell_sets];
	int+opt+notify nnode_data;
	Data_Array+opt+notify node_data[.nnode_data] {
		nvals => <-.nnodes;
	};
};

// parameter block group
group+OPort TubesParams {
	int+Port2 axis;
	int+Port2 do_transform;
	float+Port2 z_scale;
	int+Port2 facets;
	int+Port2 normalise;
	float+Port2 scale;
	int+Port2 node_cell;
	int+Port2 node_data_comp;
	int+Port2 cell_data_comp;
	int+Port2 col_data_comp;
	boolean+Port2 offsetScale;
	boolean+Port2 invertScale;
	int+Port2 col_scale_comp[2];
	boolean+Port2 only_tube_allowed;
	boolean+Port2 activate_simpletube;
	boolean+Port2 use_blendy_scaling;
};

// tube low-level module
module tube <src_file="tube.cxx",
			 libdeps="FLD",
			 cxx_hdr_files="fld/Xfld.h",
			 process="express">
{
	cxxmethod+req upd(
		.in+notify+read+req,
		.axis+notify+read+req,
		.xform+notify+read+req,
		.zscale+notify+read+req,
		.resolution+notify+read+req,
		.normalise+read+notify+req,
		.scale+read+notify+req,
		.node_cell+read+notify+req,
		.node_data_comp+read+notify+req,
		.cell_data_comp+read+notify+req,
		.col_data_comp+notify+read+req,
		.out+write,
		.err+write,
		.err_str+write
	);

	VISmesh+IPort2 &in;
	int+IPort2	  axis;
	int+IPort2	  xform;
	float+IPort2	zscale;
	int+IPort2	  resolution;
	int+IPort2	  normalise;
	float+IPort2	scale;
	int+IPort2	  node_cell;
	int+IPort2	  node_data_comp;
	int+IPort2	  cell_data_comp;
	int+IPort2	  col_data_comp;
	Field+OPort2	out;
	int+OPort2	  err;
	string+OPort2  err_str;
};

// parameter block

/*group+OPort BlendyTubesParams {
	 float+Port2	scale;
	 boolean+Port2 offsetScale;
	 boolean+Port2 invertScale;
	 int+Port2	  facets;
};*/

// module

module BlendyTubesCore <src_file="blndtube.cxx",
						  libdeps="FLD",
						  cxx_hdr_files="fld/Xfld.h">
{
	Mesh+Node_Data+read+notify+req+IPort2 &lines_in;

	TubesParams+IPort2 &params;

	float+read+notify+req scale			=> .params.scale;
	boolean+read+notify+req offsetScale => .params.offsetScale;
	boolean+read+notify+req invertScale => .params.invertScale;
	int+read+notify+req facets			 => .params.facets;

	cxxmethod+notify_inst+req update;

	Mesh+Node_Data+write+OPort2 tubes_out;
};
};


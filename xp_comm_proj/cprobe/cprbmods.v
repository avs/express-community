
flibrary ClusterProbeMods <build_dir="xp_comm_proj/cprobe",
                           out_hdr_file="gen.h",
	                   out_src_file="gen.cxx",
		           cxx_hdr_files="fld/Xfld.h",
		           libdeps="FLD",
                           cxx_name=""> {

  // define the parameter block group

  // define support groups/modules

  // define the low-level module

  module ClusterProbeCreate <src_file="cprobe.cxx"> {
    Grid+Xform &in_base <NEportLevels={2,0}>;
    Grid+Xform &in_glyph <NEportLevels={2,0}>;

     Mesh+Xform out <NEportLevels={0,2}> {
      nspace = 3;
      ncell_sets = 1;
      Cell_Set+Point cell_set[ncell_sets] {
	ncells => <-.nnodes;
	node_connect_list => init_array( ncells, 0, (ncells-1));
      };
      &xform => <-.in_base.xform;
    };

    cxxmethod+notify_inst+req update (
      in_base+read+notify+req,
      in_glyph+read+notify+req,
      out+write );
  };
};

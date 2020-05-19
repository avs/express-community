
flibrary MultiProbeMods <build_dir="xp_comm_proj/mprobe",
                         out_hdr_file="gen.h",
	                 out_src_file="gen.cxx",
		         cxx_hdr_files="fld/Xfld.h",
		         libdeps="FLD",
                         cxx_name=""> {

  // define the parameter block group

  // define support groups/modules

  // define the low-level module

  module MultiProbeMerge <src_file="mprobe.cxx"> {
    Grid+Xform &in <NEportLevels={2,0}>[];

    int ngrids => array_size( in );
    
    Mesh out <NEportLevels={0,2}> {
      nspace = 3;
      ncell_sets = 1;
      Cell_Set+Point cell_set[ncell_sets] {
	ncells => <-.nnodes;
	node_connect_list => init_array( ncells, 0, (ncells-1));
      };
    };

    cxxmethod+notify_inst+req update (
      in+read+notify+req,
      ngrids+read+req,
      out+write );
  };
};

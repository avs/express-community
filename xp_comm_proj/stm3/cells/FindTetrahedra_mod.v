module FindTetrahedraCore<src_file="FindTetrahedra.cxx",
               			  out_hdr_file="FindTetrahedra_gen.h",
               			  out_src_file="FindTetrahedra_gen.cxx",
               			  cxx_hdr_files="../base/ReadMol_gen.h fld/Xfld.h",
               			  build_dir="xp_comm_proj/stm3/cells"> {

   cxxmethod+req BuildTetra(
      .in+read+notify+req,
      .atom_names+write,
      .operation+read+notify,
      .selected_atoms+read+notify,
      .selected_atoms_idx+read+write,
      .tetrahedras+write
	);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	
   string	atom_names<NEportLevels={0,2}>[];
   enum		operation<NEportLevels={2,0}>{
				choices => {"exact", "like"};
   } = "exact";
   int		selected_atoms<NEportLevels={2,0}>[];
	
   Mesh+Node_Data+Float tetrahedras<NEportLevels={0,2}> {
		nspace = 3;
		ncell_sets = 1;
		cell_set+Tet;
		nnode_data = 1;
		node_data = {
			{veclen = 3,
			 id = 667,
			 labels = "rgb"}
		};
	};

   int selected_atoms_idx<NEportLevels={0,0},NEvisible=0>[0];
};


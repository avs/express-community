module SelectResidues<src_file="SelectResidues.cxx",
                       out_hdr_file="SelectResidues_gen.h",
                       out_src_file="SelectResidues_gen.cxx",
                       cxx_hdr_files="../base/ReadMol_gen.h",
						     cxx_src_files="copy_mol.cxx",
                       build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req Select(
      .in+read+notify+req,
      .residue_names+write,
      .operation+read+notify,
      .selected_residues+read+notify,
      .last_selected_residues+read+write,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   string residue_names<NEportLevels={0,2}>[];
   enum operation<NEportLevels={2,0}>{
      choices => {"pass selected", "remove selected", "pass like", "remove like"};
   } = "pass selected";
   int selected_residues<NEportLevels={2,0}>[];
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};

   int last_selected_residues<NEportLevels={0,0},NEvisible=0>[0];
};


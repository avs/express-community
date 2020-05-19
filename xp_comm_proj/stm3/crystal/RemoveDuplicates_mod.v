module RemoveDuplicates<src_file="RemoveDuplicates.cxx",
                   		out_hdr_file="RemoveDuplicates_gen.h",
                   		out_src_file="RemoveDuplicates_gen.cxx",
                   		cxx_hdr_files="../base/ReadMol_gen.h",
                   		build_dir="xp_comm_proj/stm3/crystal"> {

   cxxmethod+req RemoveDuplicates(
      .in+read+notify+req,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};	
};


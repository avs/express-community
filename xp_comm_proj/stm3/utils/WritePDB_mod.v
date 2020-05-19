module WritePdbCore<src_file="WritePDB.cxx",
                    out_hdr_file="WritePDB_gen.h",
                    out_src_file="WritePDB_gen.cxx",
                    cxx_hdr_files="../base/ReadMol_gen.h",
                    build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req Write(
      .molecule+read+notify+req,
      .filename+read+notify+req,
		.animation_support+read,
      .step+read
	);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule<NEportLevels={2,0}>;
   string		filename<NEportLevels={2,0}>;
	
	// for animation support
	enum        animation_support<NEportLevels={2,0}> {
   	  choices => {"no animation", "Multi PDB", "MultiFile PDB"};
	} = "no animation";
	int			step<NEportLevels={2,0}> = 0;
};


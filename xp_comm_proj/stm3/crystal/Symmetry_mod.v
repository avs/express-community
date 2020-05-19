module Symmetry<src_file="Symmetry.cxx",
                   out_hdr_file="Symmetry_gen.h",
                   out_src_file="Symmetry_gen.cxx",
                   cxx_hdr_files="../base/ReadMol_gen.h",
#ifdef MSDOS
						 //link_files="../../../sginfo/sginfo.lib",
						 link_files="../extern/sginfo/sginfo.lib",
#else
						 link_files="-L${XP_PATH1}/xp_comm_proj/stm3/extern/sginfo -lsginfo",
#endif
                   build_dir="xp_comm_proj/stm3/crystal"> {

   cxxmethod+req Symmetry(
      .in+read+notify+req,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};	
};


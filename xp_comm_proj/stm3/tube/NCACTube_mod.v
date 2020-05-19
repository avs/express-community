module NCACTube<src_file="NCACTube.cxx",
                 out_hdr_file="NCACTube_gen.h",
                 out_src_file="NCACTube_gen.cxx",
                 cxx_hdr_files="../base/ReadMol_gen.h",
                 build_dir="xp_comm_proj/stm3/tube"> {

   cxxmethod+req FindAtoms(
      .molecule.xyz_lst+read+notify+req,
      .molecule.atom_name+read+notify+req,
	  .tube_coordinates+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule<NEportLevels={2,0}>;
   float tube_coordinates<NEportLevels={0,2}>[];
};

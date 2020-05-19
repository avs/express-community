module ReadMol<src_file="ReadMol.cxx",
               out_hdr_file="ReadMol_gen.h",
               out_src_file="ReadMol_gen.cxx",
               build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req+notify_inst ReadMol(
      .filename+read+notify+req,
      .format+read+req,
      .units+read+write+notify+req,
      .read_hydrogens+read+notify+req,
		.out+write);
	  
   string filename<NEportLevels={2,0}>;
   enum   format<NEportLevels={2,0}> {
			choices => {"PlainCoord", "Coordinate", "Gaussian Cube", "PDB",
            			"PGamess", "Gamess Cube", "Dcar", "DMOL_XYZ",
            			"Mopac ESP", "Mopac XYZ", "mol2", "mol", "Chem3D", "mm3", "SHEL-X"};
   } = "PDB";
   enum   units<NEportLevels={2,0}> {
			choices => {"Bohr", "Angstrom"};
   } = "Angstrom";
   enum   read_hydrogens<NEportLevels={2,0}> {
			choices => {"no", "yes"};
   } = "yes";

   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		num_bonds = 0;
   };
};


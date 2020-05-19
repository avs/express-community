module ReadAnimMol<src_file="ReadAnimMol.cxx",
               out_hdr_file="ReadAnimMol_gen.h",
               out_src_file="ReadAnimMol_gen.cxx",
               cxx_hdr_files="ReadMol_gen.h",
               build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req+notify_inst ReadAnimMol(
      .filename+read+notify+req,
      .filename_dcd+read+notify,
      .format+read+req,
      .units+read+write+notify+req,
      .read_hydrogens+read+notify+req,
	  .step+read+notify+req,
	  .num_steps+write,
	  .file_offsets+read+write,
	  .big_endian+read+write,
	  .num_atoms+read+write,
	  .out+write);
	  
   string filename<NEportLevels={2,0}>;
   string filename_dcd<NEportLevels={2,0}>;
   enum   format<NEportLevels={2,0}> {
			choices => { "Kino", "Multi PDB", "DCD", "MultiFile PDB" };
   } = "Kino";
   enum   units<NEportLevels={2,0}> {
		choices => {"Bohr", "Angstrom"};
   } = "Angstrom";
   enum   read_hydrogens<NEportLevels={2,0}> {
		choices => {"no", "yes"};
   } = "yes";
   
   int step<NEportLevels={2,0}> = 0;
   
   int num_steps<NEportLevels={0,2}> = 0;
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		num_bonds = 0;
   };

   int file_offsets<NEportLevels={0,0},NEvisible=0>[];
   int big_endian<NEportLevels={0,0},NEvisible=0> = 0;
   int num_atoms<NEportLevels={0,0},NEvisible=0> = 0;
};


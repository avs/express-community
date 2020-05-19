module LatticeReplication<src_file="LatticeReplication.cxx",
                   out_hdr_file="LatticeReplication_gen.h",
                   out_src_file="LatticeReplication_gen.cxx",
                   cxx_hdr_files="../base/ReadMol_gen.h",
                   build_dir="xp_comm_proj/stm3/crystal"> {

   cxxmethod+req Replicate(
      .in+read+notify+req,
      .replications_a+read+notify,
      .replications_b+read+notify,
      .replications_c+read+notify,
      .reverse_a+read+notify,
      .reverse_b+read+notify,
      .reverse_c+read+notify,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	
   int   replications_a<NEportLevels={2,0}> = 1;
   int   replications_b<NEportLevels={2,0}> = 1;
   int   replications_c<NEportLevels={2,0}> = 1;
   enum  reverse_a<NEportLevels={2,0}> {choices => {"no", "yes"};} = "no";
   enum  reverse_b<NEportLevels={2,0}> {choices => {"no", "yes"};} = "no";
   enum  reverse_c<NEportLevels={2,0}> {choices => {"no", "yes"};} = "no";

   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};	
};


module ThresholdChargeCore<src_file="ThresholdCharge.cxx",
                     		out_hdr_file="ThresholdCharge_gen.h",
                     		out_src_file="ThresholdCharge_gen.cxx",
                     		cxx_hdr_files="../base/ReadMol_gen.h",
                     		build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req Select(
      .molecule_in+read+notify+req,
      .operation+read+notify,
      .min_val+read+notify,
      .max_val+read+notify+write,
      .molecule_out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule_in<NEportLevels={2,0}>;
	
   enum operation<NEportLevels={2,0}>{
      choices => {"below min", "below or eq min", "between min and max proper", "between min and max", "above max", "above or eq max", "exactly max"};
   } = "below min";
   float min_val<NEportLevels={2,0}>;
   float max_val<NEportLevels={2,0}>;
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType molecule_out<NEportLevels={0,2}> {
		&unit_cell => <-.molecule_in.unit_cell;
		&symmetry => <-.molecule_in.symmetry;
	};
};


module SummaryResidueNames<src_file="SummaryResidueNames.cxx",
               			   out_hdr_file="SummaryResidueNames_gen.h",
               			   out_src_file="SummaryResidueNames_gen.cxx",
				   			   cxx_hdr_files="ReadMol_gen.h",
               			   build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req SummaryResidueNames(
      .in+read+notify+req,
      .summarize+read+notify+req,
      .coords+write,
      .names+write
	);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   
	enum summarize<NEportLevels={2,0}> {
    	choices => {"no", "yes"};
	} = "no";
	
   float  coords<NEportLevels={0,2}>[][];
   string names<NEportLevels={0,2}>[];
};


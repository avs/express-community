module SubCell<src_file="SubCell.cxx",
					out_hdr_file="SubCell_gen.h",
               out_src_file="SubCell_gen.cxx",
               cxx_hdr_files="../base/ReadMol_gen.h",
               build_dir="xp_comm_proj/stm3/crystal"> {

   cxxmethod+req SubCell(
      .in+read+notify+req,
      .miller_planes+read+notify,
      .epsilon+read+notify,
      .out+write
	);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;

	int	miller_planes<NEportLevels={2,0}>[6] => {0, 1, 0, 1, 0, 1};	// planes indices along a (lower, upper), b, c basis vectors
	float epsilon<NEportLevels={2,0}> = 0.001;								// tolerance for atoms just outside the cell
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {	// the unit_cell will be recalculated
		&symmetry => <-.in.symmetry;
	};
};


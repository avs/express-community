module CropAtoms<src_file="CropAtoms.cxx",
					  out_hdr_file="CropAtoms_gen.h",
                 out_src_file="CropAtoms_gen.cxx",
                 cxx_hdr_files="../base/ReadMol_gen.h",
					  cxx_src_files="copy_mol.cxx",
                 build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req Crop(
      .in+read+notify+req,
      .reverse+read+notify,
      .cross_bonds+read+notify,
      .shape+read+notify,
      .radius+read+notify,
      .center+read+notify,
      .sides+read+notify,
      .outside+write,
      .inside+write
	);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;

   enum reverse<NEportLevels={2,0}>{
      choices => {"no", "yes"};
   } = "no";
   enum cross_bonds<NEportLevels={2,0}>{
      choices => {"no", "yes"};
   } = "no";
   enum shape<NEportLevels={2,0}>{
      choices => {"block", "cylinder x", "cylinder y", "cylinder z", "sphere"};
   } = "sphere";
	float radius<NEportLevels={2,0}>;
	float center<NEportLevels={2,0}>[3];
	float sides<NEportLevels={2,0}>[3];
	
   XP_COMM_PROJ.STM3.BASE.MolecularDataType outside<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};
   XP_COMM_PROJ.STM3.BASE.MolecularDataType inside<NEportLevels={0,2}> {
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};
};


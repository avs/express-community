module CountBonds<src_file="CountBonds.cxx",
                  out_hdr_file="CountBonds_gen.h",
                  out_src_file="CountBonds_gen.cxx",
                  cxx_hdr_files="../base/ReadMol_gen.h",
						NEhelpContextID=1,NEhelpFile="STM3/CountBonds.html",
                  build_dir="xp_comm_proj/stm3/utils"> {

   cxxmethod+req CountBonds(
      .in+read+notify+req,
      .out+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
   	num_atoms => <-.in.num_atoms;
		atom_z => <-.in.atom_z;
		atom_name => <-.in.atom_name;
		residue_name => <-.in.residue_name;
		residue_id => <-.in.residue_id;
		xyz_lst => <-.in.xyz_lst;
		num_bonds => <-.in.num_bonds;
		connect_lst => <-.in.connect_lst;
		bond_type_lst => <-.in.bond_type_lst;
		offset_lst => <-.in.offset_lst;
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
	};
};


module ComputeBonds<src_file="ComputeBonds.cxx",
                   out_hdr_file="ComputeBonds_gen.h",
                   out_src_file="ComputeBonds_gen.cxx",
				       cxx_hdr_files="ReadMol_gen.h",
#ifdef MSDOS
						 //link_files="../../../ann/lib/ANN.lib",
						 link_files="../extern/ann/lib/ANN.lib",
#else
						 //link_files="-L${XP_PATH1}/ann/lib -lANN",
						 link_files="-L${XP_PATH1}/xp_comm_proj/stm3/extern/ann/lib -lANN",
#endif
                   build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req ComputeBonds(
      .in+read+notify+req,
      .compute_h_bonds+read+notify+req,
      .max_h_bond_dist+read+notify+req,
      .max_h_bond_angle+read+notify+req,
      .adj_bonding_len+read+notify,
      .adj_H_bonding_len+read+notify,
	   .enable+req+notify,
      .force_recomputation+read+notify+req,
      .num_bonds+write,
      .connect_lst+write,
      .bond_type_lst+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   int   compute_h_bonds<NEportLevels={2,0}>     = 1;
   float max_h_bond_dist<NEportLevels={2,0}>     = 2.3;
   float max_h_bond_angle<NEportLevels={2,0}>    = 30.;
   float adj_bonding_len<NEportLevels={2,0}>     = 1.;
   float adj_H_bonding_len<NEportLevels={2,0}>   = 1.;
   int   enable<NEportLevels={2,0}>              = 1;
   int   force_recomputation<NEportLevels={2,0}> = 0;
   
   int num_bonds<NEportLevels={0,0}> = 0;
   int connect_lst<NEportLevels={0,0}>[2*.num_bonds];
   int bond_type_lst<NEportLevels={0,0}>[.num_bonds];

   XP_COMM_PROJ.STM3.BASE.MolecularDataType out<NEportLevels={0,2}> {
		num_atoms => <-.in.num_atoms;
		atom_z => <-.in.atom_z;
		atom_name => <-.in.atom_name;
		residue_name => <-.in.residue_name;
		residue_id => <-.in.residue_id;
		xyz_lst => <-.in.xyz_lst; 
		charge_lst => <-.in.charge_lst;
		num_bonds => <-.num_bonds;
		connect_lst => <-.connect_lst;
		bond_type_lst => <-.bond_type_lst;
    	offset_lst => <-.in.offset_lst;
		&unit_cell => <-.in.unit_cell;
		&symmetry => <-.in.symmetry;
   };
};


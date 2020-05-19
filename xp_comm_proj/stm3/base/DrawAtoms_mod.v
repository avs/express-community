module DrawAtoms<src_file="DrawAtoms.cxx",
                 out_hdr_file="DrawAtoms_gen.h",
                 out_src_file="DrawAtoms_gen.cxx",
				     cxx_hdr_files="fld/Xfld.h ReadMol_gen.h",
                 build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req DrawAtoms(
      .in+read+notify+req,
      .representation+read+notify+req,
      .normal_scale+read+notify+req,
      .cpk_scale+read+notify+req,
      .licorice_scale+read+notify+req,
		.default_colors+read+notify,
		.use_manual_limits+read+notify,
		.charge_manual_limits+read+notify+write,
      .fld+write);
	  
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   
   int   representation<NEportLevels={2,0}>   = 0;
   float normal_scale<NEportLevels={2,0}>     = 0.2;
   float cpk_scale<NEportLevels={2,0}>        = 1.0;
   float licorice_scale<NEportLevels={2,0}>   = 0.1;
   float default_colors<NEportLevels={2,0}>[] = {0.5, 1.0, 0.5};

   enum  use_manual_limits<NEportLevels={2,0}> {
        choices => {"no", "yes"};
   } = "no";
   float charge_manual_limits<NEportLevels={2,0}>[2];

   Mesh+Node_Data+Float fld<NEportLevels={0,2}> {
		nspace = 3;
		//nnodes => <-.in.num_atoms;
		coordinates.values+nres => <-.<-.in.xyz_lst;
		ncell_sets = 1;
		cell_set+Point = {
		{
			ncells=><-.nnodes,name=>"atoms",node_connect_list=>init_array(ncells,0,(ncells - 1))
		}};
		nnode_data = 2;
		node_data = {
			{veclen = 3,
			 id = 667,
			 labels = "rgb"},
			{veclen = 1,
			 id = 668,
			 labels = "radius"}
		};
	};
};


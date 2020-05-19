
module DrawBonds<src_file="DrawBonds.cxx",
				 out_hdr_file="DrawBonds_gen.h",
				 out_src_file="DrawBonds_gen.cxx",
				 cxx_hdr_files="fld/Xfld.h ReadMol_gen.h",
				 build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req DrawBonds(
   	.in+read+notify,
		.lineflag+read+notify,
		.representation+read+notify+req,
		.split+read+notify,
		.facets+read+notify,
		.bondradius+read+notify,
		.default_colors+read+notify,
		.charge_manual_limits+read+notify,
		.out+write
   );

   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   
   enum	lineflag<NEportLevels={2,0}> {
         	choices => {"lines", "tubes", "all H", "H-bonds"};
   } = "tubes";
   int	representation<NEportLevels={2,0}> = 0;
   enum	split<NEportLevels={2,0}> {
				choices => {"no", "yes"};
   } = "no";
   int	facets<NEportLevels={2,0}> = 16;
   float bondradius<NEportLevels={2,0}> = .1;
   float default_colors<NEportLevels={2,0}>[] = {0.5, 1.0, 0.5};
   float charge_manual_limits<NEportLevels={2,0}>[2];

   Mesh+Node_Data+Float out<NEportLevels={0,2}> {
		nspace = 3;
		nnode_data = 1;
		node_data = {{
			veclen = 3,
			id = 667,
			labels = "rgb"}
		};
	};
};

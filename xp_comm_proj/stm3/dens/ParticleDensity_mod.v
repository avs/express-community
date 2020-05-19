module ParticleDensity<src_file="ParticleDensity.cxx",
                       out_hdr_file="ParticleDensity_gen.h",
                       out_src_file="ParticleDensity_gen.cxx",
		                 cxx_hdr_files="../base/ReadMol_gen.h fld/Xfld.h",
						     cxx_src_files="ComputeSlot.cxx",
                       build_dir="xp_comm_proj/stm3/dens"> {

   	cxxmethod+req ComputeMeanDensity(
			.in+read+notify+req,
			.use_manual_limits+read+notify,
			.manual_limits+read+notify,
			.use_cubic_slots+read+notify,
			.side_slots+read+notify,
			.smoothing+read+notify,
			.smoothing_kernel+read+notify,
			.out_density+write
	);
	  
   	XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	enum use_manual_limits<NEportLevels={2,0}> {
    	choices => {"no", "yes"};
	} = "no";
   	float manual_limits<NEportLevels={2,0}>[2][3];
	enum use_cubic_slots<NEportLevels={2,0}> {
    	choices => {"no", "yes"};
	} = "yes";
   	int   side_slots<NEportLevels={2,0}> = 64;
   	float smoothing<NEportLevels={2,0}> = 0.;
   	int   smoothing_kernel<NEportLevels={2,0}> = 3;

	Field_Unif+Float out_density<NEportLevels={0,2}> {
		nspace = 3;
        npoints = 2;
		ndim = 3;
		nnode_data = 0;
        cell_set {
			ncell_data = 1;
            cell_data = {
				{veclen=1}
            };
        };
	};
};


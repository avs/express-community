module VelocityMean<src_file="VelocityMean.cxx",
						  out_hdr_file="VelocityMean_gen.h",
						  out_src_file="VelocityMean_gen.cxx",
						  cxx_hdr_files="../base/ReadMol_gen.h fld/Xfld.h",
						  build_dir="xp_comm_proj/stm3/dens"> {

	cxxmethod+req ComputeMeanVelocity(
		.in+read+notify+req,
		.use_manual_limits+read+notify,
		.manual_limits+read+notify,
		.use_cubic_slots+read+notify,
		.side_slots+read+notify,
		.timestep+read+notify,
		.past_positions+read+write,
		.out_velocity+write
	);

	cxxmethod+req Clear(
		.clear+read+notify,
		.past_positions+write
	);

	XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
	enum	use_manual_limits<NEportLevels={2,0}> {choices => {"no", "yes"};} = "no";
	float manual_limits<NEportLevels={2,0}>[2][3];
	enum	use_cubic_slots<NEportLevels={2,0}> {choices => {"no", "yes"};} = "yes";
	int   side_slots<NEportLevels={2,0}> = 64;
	int	clear<NEportLevels={2,0}> = 0;
	float timestep<NEportLevels={2,0}> = 1.;

	Field_Unif+Float out_velocity<NEportLevels={0,2}> {
		nspace = 3;
		npoints = 2;
		ndim = 3;
		nnode_data = 0;
		cell_set {
			ncell_data = 1;
			cell_data = {
				{veclen=3}
			};
		};
	};
	
	float past_positions<NEportLevels={0,0},NEvisible=0>[0];
};


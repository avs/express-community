flibrary SeededThreshold_Mods<build_dir="xp_comm_proj/seedthr",out_hdr_file="gen.h"> 
{
	group SeededThresholdParams
	{
		float min;
		float max;
		int seed_max_x;
		int seed_max_y;
		int seed_max_z;
		int seed_x;
		int seed_y;
		int seed_z;
	};
  
	// module: SeededThreshold
	// purpose:
	//
	module SeededThresholdCore<src_file="thresh.c"> 
	{
		omethod+req Update( .min+read+notify,
							.max+read+notify,
							.seed+read+notify+req,
							.Input_Field+read+notify+req,
							.Output_Field+write
						   ) = "CalcThreshold";
		float+IPort2 min;
		float+IPort2 max;
		int+IPort2 seed[];
		Mesh_Unif+Node_Data+IPort2 &Input_Field;
		Mesh_Unif+Node_Data+OPort2 Output_Field;
	};
};

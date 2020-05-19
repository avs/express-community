module AccumulateTraces<src_file="AccumulateTraces.cxx",
                        out_hdr_file="AccumulateTraces_gen.h",
                        out_src_file="AccumulateTraces_gen.cxx",
		                  cxx_hdr_files="../base/ReadMol_gen.h fld/Xfld.h",
                        build_dir="xp_comm_proj/stm3/traces"> {

   	cxxmethod+req Accumulate(
      	.in+read+notify+req,
      	.use_manual_limits+read,
      	.manual_limits+read,
      	.start+read,
      	.end+read,
      	.increment+read,
      	.step+read,
      	.operation+read+notify,
			.accumulator_size+read,
			.out_trace+write,
			.out_occupancy+write,
      	.local_ptr+read);
	  
   	cxxmethod+req Clear(
      	.clear+read+notify,
      	.local_ptr+read
		);
	
   	cxxmethod+notify_inst OnInstance(
      	.local_ptr+read+write
   	);

   	cxxmethod+notify_deinst OnDeinstance(
      	.local_ptr+read+write
   	);
	  
   	XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0}>;
   	int   clear<NEportLevels={2,0}> = 0;
   	int   use_manual_limits<NEportLevels={2,0}> = 0;
   	float manual_limits<NEportLevels={2,0}>[2][3];
   	int   start<NEportLevels={2,0}> = 0;
   	int   end<NEportLevels={2,0}> = 0;
   	int   increment<NEportLevels={2,0}> = 0;
   	int   step<NEportLevels={2,0}> = 0;
   	int   operation<NEportLevels={2,0}> = 0;
   	int   accumulator_size<NEportLevels={2,0}> = 64;

		Mesh+Node_Data+Int out_trace<NEportLevels={0,2}> {
			nspace = 3;
			ncell_sets = 1;
			cell_set+Polyline;
			nnode_data = 1;
			node_data = {
				{ veclen = 1 }
			};
		};

		Field_Unif+Byte out_occupancy<NEportLevels={0,2}> {
			nspace = 3;
			ndim = 3;
			dims => {accumulator_size, accumulator_size, accumulator_size};
			nnode_data = 1;
			node_data = {
				{ veclen = 1 }
			};
		};

		ptr+nosave local_ptr<NEportLevels={0,0},NEvisible=0>;
};


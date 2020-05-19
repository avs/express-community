
flibrary CombineCoordinateIndexMods<build_dir="xp_comm_proj/combidx",out_hdr_file="gen.h"> 
{
	group+OPort CombineCoordinateIndexParams {
		int+Port2 x;
		int+Port2 y;
		int+Port2 z;
		int+Port2 dims[];
	};

	module CombineCoordinateIndex<src_file="combidx.c"> {
		int+IPort2 x_index;
		int+IPort2 y_index;
		int+IPort2 z_index;
		int+IPort2 dims[];
		omethod+notify_inst+req Update(
			.x_index+read+notify+req,
			.y_index+read+notify+req,
			.z_index+read+notify+req,
			.dims+read+notify+req,
			.output+write
		) = "CombineCoordinateIndex";
		int+OPort2 output;
	};
};

flibrary CoordinatesSliceMods <build_dir="xp_comm_proj/cslice", out_hdr_file="gen.h">
{
	//Input (from another function)

	group+Port Coords_3D {
		int+Port2 Dimension;
		float+Port2 xyz_Coords[.Dimension][3];
	};
	
	//Parameter block

	group+Port CoordinatesSliceParams {
		int+Port2 axis;
		int+Port2 dec;
		float+Port2 plane;
		float+Port2 max;
		float+Port2 min;
		float maxvals[];
		float minvals[];
	};
	
	//CoordinatesSliceCore-Module

	module CoordinatesSliceCore<src_file="cslice.c"> {

		float+IPort2 plane;
		int+IPort2 axis;
		int+IPort2 dec;
		Coords_3D+IPort2 &_3D_Coords;

		omethod+notify_inst+req update(
			.plane+read+notify,
			.axis+read+notify,
			._3D_Coords+read+notify,
			.Dimension+write,
			.xy_out+write,
			.dec+write
		) = "Slice_Coords";

		int+OPort2 Dimension = 0;
		float+OPort2 xy_out[.Dimension][2];
	};
};

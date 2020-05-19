
flibrary VolumeIntegralMods <build_dir="xp_comm_proj/volint",
                     cxx_hdr_files="fld/Xfld.h",
                     out_hdr_file="voli_gen.h",
                     out_src_file="voli_gen.cxx"> {

	group+OPort VolumeIntegralParams{
		float+Port2 conversion;
		double+Port2 area;
		double+Port2 volume;	
	};

	module VolumeIntegralCore<src_file="vol_int.cxx"> {
		Mesh+Node_Data+IPort2 &in;
		float+IPort2 conversion;
		double+OPort2 area;
		double+OPort2 volume;
		cxxmethod+notify_inst+req update(
			.in+read+req+notify,
			.conversion+read+req+notify,
			.area+write,
			.volume+write
		);
	};

};


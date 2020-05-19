flibrary Crop3DMods <cxx_name="", build_dir="xp_comm_proj/crop3d",
					 out_src_file="gen.cxx",out_hdr_file="gen.h"> {

	group+OPort Crop3DParams {
		float+Port2 xmin;
		float+Port2 xmax;
		float+Port2 ymin;
		float+Port2 ymax;
		float+Port2 zmin;
		float+Port2 zmax;
	};
	
	module Crop3DCore<src_file="crop3d.cxx",
		cxx_src_files="cropbox.cxx",
		c_src_files="avl.c",
		cxx_hdr_files="fld/Xfld.h"> {

		Field+IPort2 &in {
			xform+nonotify;
			nnodes+req;
		};
		Crop3DParams+IPort2 &params;
		float xmin => params.xmin;
		float xmax => params.xmax;
		float ymin => params.ymin;
		float ymax => params.ymax;
		float zmin => params.zmin;
		float zmax => params.zmax;
		cxxmethod+req Crop3D<status=1>(
			.in+read+notify+req,
			.xmin+read+notify,
			.xmax+read+notify,
			.ymin+read+notify,
			.ymax+read+notify,
			.zmin+read+notify,
			.zmax+read+notify,
			.out+write
		);
		Field+OPort2 out {
			&xform+nowrite => <-.in.xform;
		};
	};
};

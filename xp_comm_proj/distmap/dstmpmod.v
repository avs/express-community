
flibrary DistanceMapMods <build_dir="xp_comm_proj/distmap",out_hdr_file="gen.h">
{

	module add_points<src_file="pts.c"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.pt_params+IPort2 &par;
		int point[] => par.point;
		int axis => par.axis;
		int plane => par.plane;
		int+IPort2 switch;
		int+IPort2 init_point0[];
		int+IPort2 init_point1[];
		omethod+req add_pt(
			point+read+notify+req,
			axis+read+req,
			plane+read+req,
			switch+read+req,
			init_point0+read+write,
			init_point1+read+write
		) = "add_pt";
	};

	module create_outf<src_file="create_outf.c"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.out_params+IPort2 &par;
		int smooth => par.smooth;
		int init_mask => par.init;
		int mask_num => par.mask_num;
		int do => par.do;
		int t1 => par.t1;
		int thr1 => par.t2;
		Mesh_Unif+Node_Data+IPort2 &in;
		Mesh_Unif+Node_Data+IPort2 &mask_fld;
		byte+IPort2 mask_array[];
		omethod+req create_outf(
			do+read+notify+req,
			smooth+read+req,
			t1+read+req,
			thr1+read+req,
			init_mask+read+req,
			mask_num+read+req,
			mask_array+read+write+req,
			mask_fld+read+req,
			in+read+req,
			out+write
		) = "create_outf";
		Mesh_Unif+Node_Data+OPort2 out;
	};

	module create_outm<src_file="create_outm.c"> {
		Mesh_Unif+Node_Data+IPort2 &in;
		byte+IPort2 mask_array[((in.dims[0] * in.dims[1]) * in.dims[2])];
		omethod+req create_outm(
			in+read+notify+req,
			mask_array+read+write
		) = "create_outm";
	};

	module dis_fill<src_file="dis_fill.c"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.fill_params+IPort2 &par;
		int init_mask => par.fill_mode;
		int mask_num => par.mask_num;
		int tolerance => par.tolerance;
		int reset => par.reset;
		byte+IPort2 mask[];
		Mesh_Unif+Node_Data+IPort2 &in {
			ndim = 3;
			node_data {
				byte values[.nvals][.veclen];
				byte+opt null_value;
				byte min;
				byte max;
				byte min_vec[.veclen];
				byte max_vec[.veclen];
			};
		};
		int+IPort2 init_point0[];
		int+IPort2 weight[] = {1};
		omethod+req dis_fill(
			in+read+notify+req,
			mask_num+read+notify+req,
			init_mask+read+write,
			mask+read+req,
			tolerance+read+notify+req,
			reset+read+notify+req,
			weight+read,
			init_point0+read+notify,
			out+read+write
		) = "dis_fill";
		Mesh_Unif+Node_Data+OPort2 out;
	};

	module init_points<src_file="init_points.c"> {
		Mesh_Unif+Node_Data+IPort2 &in_buf;
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.pt_init_params+IPort2 &par;
		int init_inside_points => par.init_points0;
		int init_outside_points => par.init_points1;
		int+IPort2 init_point0[];
		int+IPort2 init_point1[];
		omethod+req init_pt(
			in_buf+read+notify+req,
			init_inside_points+read+notify+req,
			init_outside_points+read+notify+req,
			init_point0+read+write,
			init_point1+read+write
		) = "init_pt";
	};

	module reset_all<src_file="reset_all.c"> {
		Mesh_Unif+Node_Data+IPort2 &in_buf;
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.distmapParams+IPort2 &par;
		int reset => par.fill_params.reset;
		byte+IPort2 mask[];
		int+IPort2 init_point0[];
		int+IPort2 init_point1[];
		omethod+req reset_all(
			in_buf+read+notify+req,
			reset+read+write+notify+req,
			mask+read,
			init_point0+read+write,
			init_point1+read+write
		) = "reset_all";
	};

	module init_mask<src_file="init_mask.c"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapParams.distmapParams+IPort2 &par;
		int init => par.fill_params.init;
		int mask_num => par.fill_params.mask_num;
		byte+IPort2 mask[];
		omethod+req init_mask(
			init+read+write+notify+req,
			mask_num+read+req,
			mask+read+write
		) = "init_mask";
	};

};

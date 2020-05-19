
flibrary DistanceMapParams <compile_subs=0> 
{

	group pt_params {
		int+IPort2 point[3];
		int+OPort2 dims[3];
		int+OPort2 axis;
		int+OPort2 plane;
	};
	
	group pt_init_params {
		int+OPort2 init_points0;
		int+OPort2 init_points1;
	};

	group fill_params {
		int+OPort2 fill_mode;
		int+OPort2 tolerance;
		int+OPort2 margin_width;
		int init;
		int reset;
		int mask_num;
		prim fill_val;
	};

	group out_params {
		int init;
		int smooth;
		int mask_num;
		int+OPort2 do = 0;
		int+OPort2 t1;
		int+OPort2 use1 = 1;
		int+OPort2 t2 = 0;
		int+OPort2 use2 = 0;
		int+OPort2 t3 = 0;
		int+OPort2 use3 = 0;
		int+OPort2 s = 60;
	};

	group+OPort points {
		int+OPort init_point1[100][3] = init_array(300,-1,-1);
		int+OPort init_point0[100][3] = init_array(300,-1,-1);
		int+Port n_points0 = 0;
		int+Port n_points1 = 0;
	};

	group distmapParams {
		pt_params pt_params;
		pt_init_params pt_init_params;
		fill_params fill_params;
		out_params out_params;
		points points;
	};

};

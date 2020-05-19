
flibrary BodyOfRevolutionMods <build_dir = "xp_comm_proj/bodofrev",
								cxx_name = "",
								out_hdr_file = "gen.h",
								out_src_file = "gen.cxx"> 
{

	// define the Parameter Block
	
	group+OPort BodyOfRevolutionParams{
		int+Port2 active;
		int+Port2 nose_cap_on;
		int+Port2 num_thetas;
		float+Port2 density;
		float+Port2 mass;
	};

	// define the low-level module

	module BodyOfRevolutionCore <src_file = "bodofrev.cxx",
								 cxx_src_files = "meshUtils.cxx",
								 cxx_hdr_files = "fld/Xfld.h" > 
	{


		// Inputs
		BodyOfRevolutionParams+IPort2 &params;
		
		int+IPort2 active => params.active;
		int+IPort2 nose_cap_on => params.nose_cap_on;
		int+IPort2 num_thetas => params.num_thetas;
		float+IPort2 r_outside[];
		float+IPort2 z_outside[];
		float+IPort2 r_inside[];

		// Don't allow z_inside input for now as the mass properties
		// assume the ends must be square.	

		float+IPort2 z_inside <NEvisible=0> [] => z_outside;

		float+IPort2 density => params.density;
	
		// Outputs

		float+OPort2 mass=>params.mass;
		float+OPort2 zcg;
		float+OPort2 izz;
	
		Field+Space3+OPort2 out {
 			ncell_sets = 2;

			// Polytri definition
			!cell_set[0] {	
				cell_corner_nnodes => cell_nnodes;
				cell_nnodes = 3;
				cell_ndim = 2;
				cell_order = 1;
				poly_flag = 1;
				poly_type-opt = 2;	 /* to distinguish from Polyhedron */
				poly_nnodes => cache(init_array(npolys,2,2));
				poly_connect_list[npolys*2];	/* node connectivity array */
				ncells => cache(get_ncells_polytri(npolys, poly_connect_list));
				node_connect_list => cache(get_connect_polytri(npolys, poly_connect_list));
				int+virtual cell_type = 10;
				string+virtual cell_name = "Polytri";
				float+virtual node_uvw[3][3] = {
					{1.0,0.0,0.0}, 
					{0.0,1.0,0.0}, 
					{0.0,0.0,0.0}
				};
			};
			
			// Polyline definition
			!cell_set[1] {
				cell_corner_nnodes => cell_nnodes;
				cell_nnodes = 2;
				cell_ndim = 1;
				cell_order = 1;
				poly_flag = 1;
				poly_nnodes => cache(init_array(npolys,2,2));
				poly_connect_list[npolys*2];	/* node connectivity array */
				ncells => cache(get_ncells_polyline(npolys, poly_connect_list));
				node_connect_list => cache(get_connect_polyline(npolys, 
				poly_connect_list));
				int+virtual cell_type = 3;
				string+virtual cell_name = "Polyline";
				float+virtual node_uvw[2][3] = {{-1.0,0.0,0.0}, {1.0,0.0,0.0}};
			};
			
			// Normals node data
			nnode_data = 1;
			!node_data[0] {
				veclen = 3;
				id = 666;
			};
			
		};
		
		cxxmethod+notify_inst+req update(
			.active+read+notify+req,
			.nose_cap_on+read+notify+req,
			.num_thetas+read+notify+req,
			.r_outside+read+notify+req,
			.z_outside+read+notify+req,
			.r_inside+read+notify,
			.z_inside+read+notify,
			.density+read+notify+req,
			.volume+write,
			.mass+write,
			.zcg+write,
			.izz+write,
			.out+write
		);
	};
};

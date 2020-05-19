
flibrary SemiSphereMods <build_dir = "xp_comm_proj/semisphr",
                         cxx_name = "",
		         out_hdr_file = "gen.h",
		         out_src_file = "gen.cxx"> {

  // define the low-level module
  group+OPort SemiSphereParams {
    int+Port2 num_thetas;
    float+Port2 r_outside;
    float+Port2 r_inside;
    float+Port2 z_offset;
    float+Port2 half_angle;
    float+Port2 density;

    float+Port2 mass;
    float+Port2 zcg;
    float+Port2 izz; 
  };

  module modSemiSphere <src_file = "semisphr.cxx",
                        cxx_hdr_files = "fld/Xfld.h",
                        cxx_src_files = "meshutils.cxx"> {

    // Inputs

    int+IPort2 active = 1;
    int+IPort2 num_thetas = 10;
    float+IPort2 r_outside;
    float+IPort2 r_inside;
    float+IPort2 z_offset;
    float+IPort2 half_angle;
    float+IPort2 density = 1;
  
    // Outputs

    float+OPort2 mass;
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
	poly_type-opt = 2;   /* to distinguish from Polyhedron */
	poly_nnodes =>cache(init_array(npolys,2,2));
	poly_connect_list[npolys*2];  /* node connectivity array */
	ncells => cache(get_ncells_polytri(npolys, poly_connect_list));
	node_connect_list => cache(get_connect_polytri(npolys, 
	poly_connect_list));
	int+virtual cell_type = 10;
	string+virtual cell_name = "Polytri";
	float+virtual node_uvw[3][3] = {{1.0,0.0,0.0}, 
	{0.0,1.0,0.0}, 
	{0.0,0.0,0.0}};
      };
      
      // Polyline definition
      !cell_set[1] {
	cell_corner_nnodes => cell_nnodes;
	cell_nnodes = 2;
	cell_ndim = 1;
	cell_order = 1;
	poly_flag = 1;
	poly_nnodes =>cache(init_array(npolys,2,2));
	poly_connect_list[npolys*2];  /* node connectivity array */
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
      .num_thetas+read+notify+req,
      .r_outside+read+notify+req,
      .r_inside+read+notify,
      .z_offset+read+notify+req,
      .half_angle+read+notify+req,
      .density+read+notify+req,
      .volume+write,
      .mass+write,
      .cg+write,
      .izz+write,
      .out+write
    );
  };
};

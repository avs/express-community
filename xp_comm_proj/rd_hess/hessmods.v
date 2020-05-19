
flibrary ReadHessMods <build_dir="xp_comm_proj/rd_hess"> {

    module Read_Hess_File < src_file      = "rd_hess.cxx",
                            out_src_file  = "gen.cxx",
                            out_hdr_file  = "gen.h",
                            cxx_hdr_files = "fld/Xfld.h" > {

        string+IPort2  hess_filename;

        Field+Space3+OPort2  out_fld {

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

        cxxmethod+req update (
            .hess_filename +read+req+notify,
            .out           +write
        );

    };

};


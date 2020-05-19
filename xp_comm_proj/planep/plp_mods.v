
flibrary PlaneProjectMods <build_dir="xp_comm_proj/planep"> {

   module plane_proj <src_file="planep.c"> {
      Mesh+Iparam &in {
         nnodes+req;
      };
      Plane_Grid+Xform+Iparam &plane {
         points+req;
      };

      omethod+notify_val plane_proj_update<status=1> = "plane_proj_update";

      Node_Data+Vector+Float+Oparam out;
      float+Oparam dist;
   }; 



   macro plane_proj2 {
      Mesh+IPort2 &in;                 /*  input field */

      Plane_Grid+Xform+IPort2 &plane;  /* slice plane */

      float+IPort2  dist;              /* plane distance */
      int+IPort2    map_comp[];
      int+IPort2    cell_data[];

      int+nres has_node_data => (is_valid(in.nnode_data) && in.nnode_data > 0);

      DVplane_dist DVplane_dist {
         in => <-.in;
         plane => <-.plane;
      };		
      DVcomb_mesh_and_data DVcomb_mesh_and_data {
         in_mesh => <-.in;
         in_nd => DVplane_dist.out;
      };
      DViso DViso {
         in => DVcomb_mesh_and_data.out;
         level => DVplane_dist.dist+(<-.dist);
         &cell_data => <-.cell_data;
      };
      DVnmap DVnmap {
         in => switch(<-.has_node_data, <-.in);
         nparam => DViso.nparam;
         map_comp => <-.map_comp;
      };
      plane_proj plane_proj {
         in => DViso.out;
         plane => <-.plane;
      };
      Mesh out_2d_mesh {
         nnodes => <-.DViso.out.nnodes;
         nspace => <-.DViso.out.nspace;
         ncell_sets => <-.DViso.out.ncell_sets;
         &cell_set[] => <-.DViso.out.cell_set;
         coordinates {
            values+nres => <-.<-.plane_proj.out.node_data[0].values;
         };
      };

      DVcomb_mesh_and_data DVcomb_mesh_and_data_1 {
         in_mesh => DViso.out;
         in_nd => DVnmap.out;
      };
      DVcomb_mesh_and_data DVcomb_mesh_and_data_2 {
         in_mesh => <-.out_2d_mesh;
         in_nd => DVnmap.out;
      };

      Mesh+nres+OPort2 &out    => switch(has_node_data+1, DViso.out, DVcomb_mesh_and_data_1.out);
      Mesh+nres+OPort2 &out_2d => switch(has_node_data+1, out_2d_mesh, DVcomb_mesh_and_data_2.out);
   };


};


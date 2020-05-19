macro Float2ByteUnif<NEhelpContextID=1,NEhelpFile="STM3/Float2ByteUnif.html"> {
   link in_float<NEportLevels={2,1}>;
   group node_scalar {
      float+nres bias => in_float.node_data[0].min;
      float+nres scale => 1.0/(in_float.node_data[0].max - in_float.node_data[0].min);
      
      Node_Data out_data<NEportLevels={0,2}> {
         nnode_data = 1;
         nnodes+nres => in_float.nnodes;
         !node_data[0] {
            values+byte;
            null_value+byte;
            min+byte;
            max+byte;
            min_vec+byte;
            max_vec+byte;
            values+nres =>cache((in_float.node_data[0].values - bias) * scale * 255);
            veclen=1;
         };
      };
   };
   group combine_mesh_data {
      Mesh &in_mesh<NEportLevels={2,0}> => <-.in_float;
      Node_Data+nres &in_nd<NEportLevels={2,0}> => <-.node_scalar.out_data;
      int error => ((is_valid(in_mesh.nnodes) && is_valid(in_nd.nnodes)) && (in_mesh.nnodes != in_nd.nnodes));
      GMOD.print_error print_error {
         error => <-.error;
         error_source = "combine_mesh_data";
         error_message = "Invalid field: mesh dimensions and size of data do not match";
         on_inst = 1;
      };
      Mesh+Node_Data &out<NEportLevels={0,2}> => switch((!.error),merge(.in_nd,.in_mesh,,));
   };
   link out_byte<NEportLevels={1,2}> => .combine_mesh_data.out;
};

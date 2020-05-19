
flibrary KrigMacs <compile_subs=0> {


macro Interp2D{
   Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
   
   XP_COMM_PROJ.Krig.KrigMods.agxint agxint{
      x_array => <-.in_fld.coordinates.values[][0];
      y_array => <-.in_fld.coordinates.values[][1];
      values => <-.in_fld.node_data[0].values[][0];

      grid_out<NEportLevels={0,2}>;
      num_x<NEportLevels={0,2}> = 100;
      num_y<NEportLevels={0,2}> = 100;
   };
   FLD_MAP.uniform_mesh uniform_mesh{
      in_dims => {agxint.num_x,
         <-.agxint.num_y};
   };
   FLD_MAP.node_scalar node_scalar{
      in_data => <-.agxint.grid_out;
   };
   FLD_MAP.combine_mesh_data combine_mesh_data{
      in_mesh => <-.uniform_mesh.out;
      in_nd => <-.node_scalar.out;
   };
   olink out_fld<NEportLevels={1,2}> => .combine_mesh_data.out;
};



macro Krig2D{
   Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
   
   olink zest<NEportLevels={1,2}> => .XuKrigging2D.zest;
   olink zstd<NEportLevels={1,2}> => .XuKrigging2D.zstd;
   XP_COMM_PROJ.Krig.KrigMods.XuKrigging2D XuKrigging2D{
      xarr=> <-.in_fld.coordinates.values[][0];
      yarr=> <-.in_fld.coordinates.values[][1];
      values=> <-.in_fld.node_data[0].values[][0];

      poly_x = {0.,0.,1.,1.};
      poly_y = {0.,1.,0.,1.};
      nx = 20;
      ny = 20;
      radius = 1.;
      trigger = 1;
   };
};



macro Krig{
   Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
   
   XP_COMM_PROJ.Krig.KrigMods.XuKrigging3D XuKrigging3D{
      xarr=> <-.in_fld.coordinates.values[][0];
      yarr=> <-.in_fld.coordinates.values[][1];
      zarr=> <-.in_fld.coordinates.values[][2];
      values=> <-.in_fld.node_data[0].values[][0];
   };
   olink zest<NEportLevels={1,2}> => .XuKrigging3D.zest;
   olink zstd<NEportLevels={1,2}> => .XuKrigging3D.zstd;
};

};



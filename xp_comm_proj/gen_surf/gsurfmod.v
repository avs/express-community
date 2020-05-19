
flibrary GenerateSurfaceMods<build_dir="xp_comm_proj/gen_surf",
  		          out_hdr_file="gen.h"> {

  group GenerateSurfaceParams <NEportLevels={0,1}>{
    float+Port2 threshold;
  };

  module GenerateSurfaceCore<src_file="gen_surf.c"> {
    GenerateSurfaceParams+IPort2 &Params;
    float+IPort2 threshold=>Params.threshold;
    Mesh_Unif+Node_Data+IPort2 &input;
    omethod+req Update(.threshold+read+notify+req,
                       .input+read+notify+req,
                       .output+write
                       ) = "GenerateSurface";
    Mesh+Node_Data output<export=2,NEportLevels={0,2}>;
  };
};

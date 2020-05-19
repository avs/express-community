
flibrary SphereSurfaceMods <build_dir="xp_comm_proj/spsurf",
                            out_src_file="gen.cxx",
                            out_hdr_file="gen.h">
{

  // define the parameter block group

  group+OPort SphereSurfaceParams {
    int+Port2 longdim;
    int+Port2 latdim;
    double+Port2 radius;
    double+Port2 start_long;
    double+Port2 end_long;
    double+Port2 start_lat;
    double+Port2 end_lat;
    int+Port2 closedlong;
    int+Port2 closedlat;
  };



  // define the low-level modules

  module extract_node_data {
    Node_Data+IPort2 &in <export=2>;

    Node_Data+OPort2 out <export=2> {
      nnode_data => in.nnode_data;
      &node_data<NEportLevels={0,1},export=3> => in.node_data;
      nnodes => in.nnodes;
    };
  };


#ifdef MSDOS
  module SphereSurfaceGen <src_file="spsurf.c"> {
#else
  module SphereSurfaceGen <src_file="spsurf.c",
                           link_files="-lm"> {
#endif

    SphereSurfaceParams+IPort2 &SphereSurfaceParams<export=2>;

    int longdim => SphereSurfaceParams.longdim;
    int latdim => SphereSurfaceParams.latdim;
    double end_lat => SphereSurfaceParams.end_lat;
    double start_lat => SphereSurfaceParams.start_lat;
    double end_long => SphereSurfaceParams.end_long;
    double start_long => SphereSurfaceParams.start_long;
    double radius => SphereSurfaceParams.radius;
    int closedlong => SphereSurfaceParams.closedlong;
    int closedlat => SphereSurfaceParams.closedlat;

    omethod+notify_inst+req update(
      longdim+read+notify+req,
      latdim+read+notify+req,
      end_lat+read+notify+req,
      start_lat+read+notify+req,
      end_long+read+notify+req,
      start_long+read+notify+req,
      radius+read+notify+req,
      closedlong+read+notify+req,
      closedlat+read+notify+req,
      mesh+write
    ) = "sphere_surface_gen";

    Grid_Struct+Cells+Xform+OPort2 mesh<NEcolor1=65280,export=2> {
      ndim = 2;
    };
  };

};


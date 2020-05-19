
flibrary GocadMods <build_dir="xp_comm_proj/rdgocad",
                    out_hdr_file="gen.h",
                    out_src_file="gen.cxx">
{
  group GocadParams<NEportLevels={0,1},export=1> {
    float x<NEportLevels=2,export=2>;
    float y<NEportLevels=2,export=2>;
    float z<NEportLevels=2,export=2>;
    boolean scale<NEportLevels=2,export=2>;
    string file_name<NEportLevels=2,export=2>;
  };

  module ReadGOCAD <src_file="gocad.cxx",
                    libdeps="FLD",
                    cxx_hdr_files="fld/Xfld.h">
  {
    GocadParams &group_ref<NEportLevels={2,0},export=2>;

    boolean scale<export=2> => group_ref.scale;
    string file_name<export=2> => group_ref.file_name;
    float x<export=2> => group_ref.x;
    float y<export=2> => group_ref.y;
    float z<export=2> => group_ref.z;

    Grid Grid<export=2> {
      nspace = 3;
      coordinates {
        min => ;
        max => ;
      };
    };
    Cells Cells<export=2> {
      ncell_sets = 1;
      cell_set = {
        { cell_nnodes=3,poly_flag=0,cell_order=1,cell_ndim=2 }
      };
    };
    Xform Xform<export=2>;

    Mesh &Mesh<NEportLevels={0,2},export=2> => merge(.Grid,.Cells,.Xform);

    cxxmethod+notify_inst+req update<export=2>(
       Grid+write,
       Cells+write,
       scale+read+notify+req,
       x+read+notify+req,
       y+read+notify+req,
       z+read+notify+req,
       file_name+read+notify+req
    );
  };

};


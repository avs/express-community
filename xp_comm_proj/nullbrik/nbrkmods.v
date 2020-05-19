
flibrary NullBrickMods <build_dir="xp_comm_proj/nullbrik",
		        out_hdr_file="gen.h",
		        out_src_file="gen.cxx",
                        cxx_hdr_files="fld/Xfld.h",
                        libdeps="FLD",
                        cxx_name=""> {

  // define the parameter block group

  group NullBrickParams <NEportLevels={0,1}> {
    int xmin <NEportLevels={2,2}>;
    int xmax <NEportLevels={2,2}>;
    int ymin <NEportLevels={2,2}>;
    int ymax <NEportLevels={2,2}>;
    int zmin <NEportLevels={2,2}>;
    int zmax <NEportLevels={2,2}>;
    double nval <NEportLevels={2,2}>;
    int setnull <NEportLevels={2,2}>;
  };
  
  NullBrickParams &NullBrickParams_ref <NEportLevels={1,1}> {
    int xmin <NEportLevels={0,2}>;
    int xmax <NEportLevels={0,2}>;
    int ymin <NEportLevels={0,2}>;
    int ymax <NEportLevels={0,2}>;
    int zmin <NEportLevels={0,2}>;
    int zmax <NEportLevels={0,2}>;
    double nval <NEportLevels={0,2}>;
    int setnull <NEportLevels={0,2}>;
  };
  
  // define the low-level module

  module NullBrickNodeData <src_file="nullbrik.cxx"> {
    Node_Data_Struct &in <NEportLevels={2,0}>;

    NullBrickParams_ref params <NEportLevels={2,0}>;

    int xmin => params.xmin;
    int xmax => params.xmax;
    int ymin => params.ymin;
    int ymax => params.ymax;
    int zmin => params.zmin;
    int zmax => params.zmax;
    double nval => params.nval;
    int setnull => params.setnull;

    Node_Data_Struct out <NEportLevels={0,2}> {
      &nnode_data => in.nnode_data;
      &ndim => in.ndim;
      &dims => in.dims;
    };

    cxxmethod+req update (
      in+read+notify+req,
      xmin+read+notify+req,
      xmax+read+notify+req,
      ymin+read+notify+req,
      ymax+read+notify+req,
      zmin+read+notify+req,
      zmax+read+notify+req,
      nval+read+notify+req,
      setnull+read+notify+req,
      out+write );
  };
};


flibrary ReadHDFImageMods <build_dir="xp_comm_proj/r_hdfimg",
			   out_hdr_file="gen.h",
			   out_src_file="gen.cxx"> {

  // define the parameter block groups

  group ReadHDFImageParams <NEportLevels={0,1}> {
    string filename <NEportLevels={2,2}>;
  };

  // define the low-level module

  module ReadHDFImageField
      <hdr_dirs="/usr/local/include",
       c_hdr_files="im.h",
       link_files="-L/usr/local/lib -lim -lsdsc -ltiff -ljpeg",
       src_file="r_hdfimg.c"> {
    omethod+notify_inst+req update(
      .filename+notify+read+req,
      .out+write )
      = "read_hdf_image";

    ReadHDFImageParams &ReadHDFImageParams <NEportLevels={2,1}>;
    string filename => ReadHDFImageParams.filename;
    Mesh_Unif+Node_Data out <NEcolor0=255,NEcolor1=65280,
                             NEnumColors=4, NEportLevels={0,2}> {
      ndim = 2;
      points => { {0,0}, {(dims[0] - 1),(dims[1] - 1)} };
      nspace = 2;
      nnode_data = 1;
      node_data {
        id = 669; // RGB Special Data ID
      };
    };
  };
};

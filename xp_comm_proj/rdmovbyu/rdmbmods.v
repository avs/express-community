
flibrary ReadMovieBYUMods <build_dir="xp_comm_proj/rdmovbyu",
		           out_hdr_file="gen.h",
		           out_src_file="gen.cxx"> {

  //////////////////////////////////////
  // define the parameter block groups

  group ReadMovieBYUParams <NEportLevels={0,1}> {
    string filename <NEportLevels={2,2}>;
  };

  group ReadMovieOffsetParams <NEportLevels={0,1}> {
    string filename <NEportLevels={2,2}>;
  };


  //////////////////////////////////
  // define the low-level modules

  // ReadMovieBYUFile
  //   Low-level module that reads in a BYU file and constructs field data from it

  module ReadMovieBYUFile <src_file="rdmovbyu.c"> {
    ReadMovieBYUParams &ReadMovieBYUParams <NEportLevels={2,0}>;

    string filename => ReadMovieBYUParams.filename;

    Mesh+Cell_Data+Node_Data+Oparam out;

    omethod+notify_val+notify_inst read_byu_update <status=1>
        = "read_byu_update";
  };


  // ReadMovieOffsetFile
  //   Low-level module that reads in an offset file and creates a floating point array

  module ReadMovieOffsetFile <src_file="rdmovoff.c"> {  
    string &filename <NEportLevels={2,0}>;

    int+read+write+nonotify size;
    float+Oparam out[];
	
    omethod+notify_val+notify_inst read_off_update <status=1>
        = "read_off_update";
  };


  // ReadMovieBYUFltToCoord
  //   Low-level module that takes the output floating point array of offsets and
  //   generates an array of vector coordinates from them.  NOT used in other modules/macros.

  module ReadMovieBYUFltToCoord {
    float+IPort2 &in[];
    float+OPort2+nres coord[(array_size(.in) / 3)][3] => .in;
  };


  // ReadMovieBYUApplyOffset
  //   Low-level module that takes the output floating point array of offsets and
  //   applies those offsets to a set of field data.  The offset applied is scaled
  //   by a floating point scale factor.

  module ReadMovieBYUApplyOffset {
    Mesh+Node_Data+IPort2 &in ;
    float+IPort2 &offset_data[];
    float+IPort2  scale = 1.0;

    int size => array_size(offset_data);
    float newcoords[size/3][3] => offset_data;

    Grid+Xform out_grid {
      nnodes => in.nnodes;
      nspace => in.nspace;
      &xform => in.xform;
      int npoints => nnodes;
      float points[nnodes][nspace];
      coordinates {
      	values+nres => in.coordinates.values +
	      scale*newcoords;
      };
    };

    Mesh+Node_Data+OPort2 &out => merge(out_grid, in);

    GDM.DataObject DataObject {
      in => <-.out;
      Obj {
	      name => name_of(<-.<-.<-);
      };
    };

    olink out_obj => DataObject.obj;
  };


};

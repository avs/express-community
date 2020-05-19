
flibrary ReadPlot3dMods <build_dir="xp_comm_proj/rdplot3d",
		         out_hdr_file="gen.h",
		         out_src_file="gen.cxx"> {

  // define the parameter block group

  group ReadPlot3dParams <NEportLevels={0,1}> {
    string+Port2 filename_xyz <export=2>;  /* name of Mesh file to read */
    string+Port2 filename_q <export=2>;    /* name of Result file to read */ 
    int+Port2 iblanks = 1;
    int+Port2 format = 1;
    int+Port2 use_q = 1;
    int+Port2 trigger = 0;
    int+Port2 dims3d = 1;
    int+Port2 multiblock = 1;
  };

  // define the low-level module

  module ReadPlot3dFile <src_file="rdplot3d.c"> {
    ReadPlot3dParams &ReadPlot3dParams <NEportLevels={2,0}>;

    string filename_xyz => ReadPlot3dParams.filename_xyz;
    string filename_q => ReadPlot3dParams.filename_q;
    int iblanks => ReadPlot3dParams.iblanks;
    int format => ReadPlot3dParams.format;
    int use_q => ReadPlot3dParams.use_q;
    int trigger => ReadPlot3dParams.trigger;
    int dims3d => ReadPlot3dParams.dims3d;
    int multiblock => ReadPlot3dParams.multiblock;

    Multi_Block out <NEportLevels={0,2}>;
    link fields <NEportLevels={1,2},export=1> => out.fields;
    
    omethod+req rd_plot3d_update(
        filename_xyz+read+notify,
        filename_q+read+notify,
        iblanks+read+notify+req,
        format+read+notify+req,
        use_q+read+notify+req,
        trigger+read+notify+req,
        dims3d+read+notify+req,
        multiblock+read+notify+req,
        out+write) = "read_plot3d_update";
  };
};

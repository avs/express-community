
flibrary CFDRangeMods <build_dir="xp_comm_proj/cfdrange",
		       out_hdr_file="gen.h",
		       out_src_file="gen.cxx"> {

  // define the parameter block group

  group CFDRangeParams <NEportLevels={0,1}> {
    double min <NEportLevels={2,2}>;
    double max <NEportLevels={2,2}>;
    int lock_mode <NEportLevels={2,2}>;
  };

  // define the low-level module

  module CFDRangeNodeData <src_file="cfdrange.c"> {

    Node_Data+read+notify+req &in <NEportLevels={2,0}>[];

    CFDRangeParams &CFDRangeParams <NEportLevels={2,0}>;
    double+read+notify+req min => CFDRangeParams.min;
    double+read+notify+req max => CFDRangeParams.max;
    int+read+notify+req lock_mode => CFDRangeParams.lock_mode;

    int nblocks => array_size(in);
    
    Node_Data+write+nonotify+OPort2 out[nblocks] => in;

    omethod+notify_val+notify_inst update <status=1> = "cfd_range_update";

  };
};

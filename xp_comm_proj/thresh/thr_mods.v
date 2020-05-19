
flibrary ThresholdMods <build_dir="xp_comm_proj/thresh",
		       process="user",
		       out_hdr_file="gen.h",
		       out_src_file="gen.cxx"> {

  // define the parameter block group

  group ThresholdParams <NEportLevels={0,1}> {
    double min <NEportLevels={2,2}>;
    double max <NEportLevels={2,2}>;
  };

  // define the low-level module

  module ThresholdNodeData <src_file="thresh.c"> {
    Node_Data &in <NEportLevels={2,0}>;
    ThresholdParams &ThresholdParams <NEportLevels={2,0}>;
    double minimum => ThresholdParams.min;
    double maximum => ThresholdParams.max;
    Node_Data out <NEportLevels={0,2}> {
      nnode_data+nres => <-.in.nnode_data;
      nnodes+nres => <-.in.nnodes;
    };
    omethod+notify_inst+req update (
      in+read+notify+req,
      minimum+read+notify+req,
      maximum+read+notify+req,
      out+write )
      = "ThresholdNodeData";
  };
};

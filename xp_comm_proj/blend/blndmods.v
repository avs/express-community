
flibrary BlendNodeDataMods <build_dir="xp_comm_proj/blend",
	                    out_hdr_file="gen.h",
		            out_src_file="gen.cxx",
		            cxx_hdr_files="fld/Xfld.h",
		            libdeps="FLD"> {

  // define the parameter block group

  group BlendParams <NEportLevels={0,1}> {
    float ratio <NEportLevels={2,2}> {
      min = 0.0;
      max = 1.0;
    };
  };

  BlendParams &BlendParams_ref <NEportLevels={1,1}> {
    ratio <NEportLevels={0,2}>;
  };

  // define support groups/modules

  // define the low-level module

  module BlendNodeData <src_file="blend.cxx"> {
    BlendParams_ref &params <NEportLevels={2,0}>;

    Node_Data &in1 <NEportLevels={2,0}>;
    Node_Data &in2 <NEportLevels={2,0}>;

    float ratio => params.ratio;

    Node_Data out <NEportLevels={0,2}>;

    cxxmethod+notify_inst+req update (
      in1+read+notify+req,
      in2+read+notify+req,
      ratio+read+notify+req,
      out+write );
  };
};

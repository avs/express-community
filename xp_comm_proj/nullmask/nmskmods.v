
flibrary NullMaskMods <build_dir="xp_comm_proj/nullmask",
	               out_hdr_file="gen.h",
		       out_src_file="gen.cxx",
		       cxx_hdr_files="fld/Xfld.h",
		       libdeps="FLD"> {

  // define the parameter block group

  group NullMaskParams <NEportLevels={0,1}> {
    double out_valid <NEportLevels={2,2}>;
    double out_null <NEportLevels={2,2}>;
    double in_null <NEportLevels={2,2}>;
    int user_null <NEportLevels={2,2}>;
    int invert <NEportLevels={2,2}>;
    int set_null <NEportLevels={2,2}>;
  };

  // define support groups/modules

  // define the low-level module

  module NullMaskNodeData <src_file="nullmask.cxx"> {
    NullMaskParams &NullMaskParams <NEportLevels={2,0}>;

    Node_Data &in <NEportLevels={2,0}>;

    double out_valid => NullMaskParams.out_valid;
    double out_null => NullMaskParams.out_null;
    double in_null => NullMaskParams.in_null;
    int user_null => NullMaskParams.user_null;
    int invert => NullMaskParams.invert;
    int set_null  => NullMaskParams.set_null;

    Node_Data out <NEportLevels={0,2}>;

    cxxmethod+notify_inst+req update (
      in+read+notify+req,
      out_valid+read+notify+req,
      out_null+read+notify+req,
      in_null+read+notify+req,
      user_null+read+notify+req,
      invert+read+notify+req,
      set_null+read+notify+req,
      out+write );
  };
};

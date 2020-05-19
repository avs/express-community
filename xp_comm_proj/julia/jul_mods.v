
flibrary JuliaMods {

  // define the parameter block group

  group JuliaParams <NEportLevels={0,1}> {
    double mandelx <NEportLevels={2,2}>;
    double mandely <NEportLevels={2,2}>;
    double leftx <NEportLevels={2,2}>;
    double rightx <NEportLevels={2,2}>;
    double topy <NEportLevels={2,2}>;
    double bottomy <NEportLevels={2,2}>;
    double max_value <NEportLevels={2,2}>;
    int max_iter <NEportLevels={2,2}>;
    int resx <NEportLevels={2,2}>;
    int resy <NEportLevels={2,2}>;
  };

  // define the low-level module

  module JuliaGen <build_dir="xp_comm_proj/julia",
			src_file="julia.cxx",
  		        out_hdr_file="gen.h",
		        out_src_file="gen.cxx",
			libdeps="FLD",
			cxx_hdr_files="fld/Xfld.h">{

    JuliaParams &JuliaParams <NEportLevels={2,0}>;

    double mandelx => JuliaParams.mandelx;
    double mandely => JuliaParams.mandely;
    double leftx => JuliaParams.leftx;
    double rightx => JuliaParams.rightx;
    double topy => JuliaParams.topy;
    double bottomy => JuliaParams.bottomy;
    double max_value => JuliaParams.max_value;
    int max_iter => JuliaParams.max_iter;
    int resx => JuliaParams.resx;
    int resy => JuliaParams.resy;

    Mesh_Unif+Node_Data+Xform out <NEportLevels={0,2},NEcolor2=16776960> {
      ndim = 2;
      dims => {<-.resx,<-.resy};
      nspace => ndim;
      nnode_data = 1;
      node_data {
         values+byte;
         null_value+byte;
         min+byte;
         max+byte;
         min_vec+byte;
         max_vec+byte;
      } = {{veclen=1,,,,,,,,}};
      points => {{<-.leftx,<-.bottomy},{<-.rightx,<-.topy}};
    };

    cxxmethod+notify_inst+req update (
      .mandelx+read+notify+req,
      .mandely+read+notify+req,
      .leftx+read+notify+req,
      .rightx+read+notify+req,
      .topy+read+notify+req,
      .bottomy+read+notify+req,
      .max_value+read+notify+req,
      .max_iter+read+notify+req,
      .resx+read+notify+req,
      .resy+read+notify+req,
      .out+write );
  };
};

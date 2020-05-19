
flibrary CatmullMods {

  // define the parameter block group

  group CatmullParams <NEportLevels={0,1}> {
    int steps<NEportLevels={2,2}> = 10;
    enum line_type<NEportLevels={2,2}> {
      choices => {"closed","open"};
    } = "closed";
  };


  // define the low-level module

  module CatmullRomSpline < build_dir="xp_comm_proj/catmull",
                          src_file="catmull.c",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.h",
                    		  c_src_files="spline.c",
                          c_hdr_files="spline.h"> {

      float &points<NEportLevels={2,0}>[];

      CatmullParams &CatmullParams <NEportLevels={2,0}>;

      omethod+notify_inst+req update(
         .points+read+notify+req,
         .line_type+read+notify,
         .steps+read+notify,
         .nspace+read,
         .out+write
      ) = "update";

      int line_type => CatmullParams.line_type;
      int steps => CatmullParams.steps;

      float dims[] => array_dims(points);
      int nspace => (array_size(points) / dims[1]);

      float out<NEportLevels={0,2}>[((((array_size(.points) / .nspace) - .line_type) * .steps) + 1)][.nspace];
  };

};

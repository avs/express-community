
flibrary StepCalcMods <build_dir = "xp_comm_proj/stepcalc",
                       cxx_name = "",
		       out_hdr_file = "gen.h",
		       out_src_file = "gen.cxx"> {

  // define the low-level module

  module modStepCalc <src_file = "stepcalc.cxx"> {
    int+IPort2 active = 1;
    float+IPort2 stepX[];   
    float+IPort2 stepY[];
    float+IPort2 inputValue; 
    float+OPort2 outputValue; 

    cxxmethod+notify_inst+req update(
      .active+read+req,
      .stepX+read+notify+req,
      .stepY+read+notify+req, 
      .inputValue+read+notify+req,
      .outputValue+write
    );
  };
};

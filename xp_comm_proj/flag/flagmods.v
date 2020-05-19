
flibrary FlagMods <build_dir="xp_comm_proj/flag",
                   out_hdr_file="gen.h",
                   out_src_file="gen.cxx"> {

  // define the parameter block group

  group+OPort FlagParams {
    int+Port2 count;
    int+Port2 reset;
    int+Port2 flag_motion = 1;
    int+Port2 flag_color;
    int+Port2 release_top;
    int+Port2 release_bottom;
    float+Port2 flag_wind_1;
    float+Port2 flag_wind_2;
    float+Port2 flag_strength;
  };

  group+OPort FlagLoopParams {
    boolean+Port2 run;
    boolean+Port2 step;
  };


  // define the low-level module

  module FlagRaw <src_file="flag.c"> {
    FlagParams+IPort2 &FlagParams;

    int+read+notify+req count => FlagParams.count;
    int+read+notify+req reset => FlagParams.reset;
    int+read+notify+req flag_motion => FlagParams.flag_motion;
    int+read+notify+req flag_color => FlagParams.flag_color;
    int+read+notify+req release_top => FlagParams.release_top;
    int+read+notify+req release_bottom => FlagParams.release_bottom;
    float+read+notify+req flag_wind_1 => FlagParams.flag_wind_1;
    float+read+notify+req flag_wind_2 => FlagParams.flag_wind_2;
    float+read+notify+req flag_strength => FlagParams.flag_strength;

    int+nonotify traverse = 0;

    omethod+notify_val+notify_inst update = "flag_demo";
    omethod+notify_del delete = "flag_demo_del";

    Mesh_Struct+Dim2+Space3+Node_Data+Oparam out;
  };


  GMOD.loop FlagLoop {
    FlagLoopParams &FlagLoopParams <NEportLevels={2,1}>;
    run => FlagLoopParams.run;
    step => FlagLoopParams.step;
    start_val = 0;
    end_val = 1000;
    incr = 1;
    count = 0;
    cycle = 1;
  };

};


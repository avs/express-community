
flibrary ReadAiffMods <build_dir="xp_comm_proj/rd_aiff",
                       out_hdr_file="gen.h",
                       out_src_file="gen.cxx"> {

  // define the parameter block group

  group+OPort ReadAiffParams {
    string+Port2 file_name;
    int+Port2    Num_channels;
    int+Port2    Sample_size;
    int+Port2    Num_samples;
    double+Port2 Sample_rate;
  };


  // define the low-level module

  module ReadAiffData <src_file="readaiff.cxx",
                       cxx_src_files="libaiff.cxx"> {

    ReadAiffParams+IPort2 &parameters;

    cxxmethod+notify_inst+req update;

    string+read+notify+req file_name    => parameters.file_name;
    int+write              Num_channels => parameters.Num_channels;
    int+write              Sample_size  => parameters.Sample_size;
    int+write              Num_samples  => parameters.Num_samples;
    double+write           Sample_rate  => parameters.Sample_rate;

    short+write+OPort2     out_data[Num_samples];
  };


};


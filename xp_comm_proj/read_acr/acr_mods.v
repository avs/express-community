
flibrary ACR_NEMA_Mods <build_dir="xp_comm_proj/read_acr",
                        out_hdr_file="gen.h",
                        out_src_file="gen.cxx"> {

  // define the parameter block group -- THERE IS NOT ONE


  // define the low-level module

  module read_acr_nema <src_file="read_acr.c",weight=0> {
    string filename<NEportLevels={2,1}>;
    short short_array<NEportLevels={1,2}>[];
    int dims_out<NEportLevels={1,2}>[];
    omethod+notify_inst+req update(
      filename+read+notify+req,
      short_array+write,
      dims_out+write
    ) = "read_acr";
  };
};

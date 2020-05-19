
flibrary ConvertConnectListMods <build_dir="xp_comm_proj/cvtclst",
                        libimm=1,
                        out_hdr_file="gen.h">
{

  // module: cvt_connect_list
  // purpose: Convert from an AVS5 style poly connect list to an
  //   AVS/Express style list. This includes breaking up the AVS5
  //   list into two lists, and changing the indexing from 1 based
  //   to 0 based for the connect values.
  //

  module ConvertConnectListCore<src_file="cvt_cnct.c"> {

    omethod+notify_inst+req update(.input+read+notify+req,
                                   .poly_nodes+write,
                                   .connect_list+write
                                   ) = "ConvertConnectList";
    int+IPort2 input[];
    int+OPort2 poly_nodes[];
    int+OPort2 connect_list[];
  };
};

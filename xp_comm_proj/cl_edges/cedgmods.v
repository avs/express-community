
flibrary CleanExtEdgesMods <build_dir="xp_comm_proj/cl_edges",
//                          process="user",
                            out_hdr_file="gen.h",
                            out_src_file="gen.cxx"> {


   module CleanEdgesCore <src_file="cl_edges.cxx",
                          cxx_hdr_files="fld/Xfld.h"> {
      Mesh+IPort2 &in;
      int+IPort2 outer_edges = 0;

      cxxmethod+notify_inst+req update (
         .in+read+req+notify,
         .outer_edges+read+req+notify,
         .nlines_out+write,
         .lines_out+write
      );

      int+OPort2 nlines_out;
      int+OPort2 lines_out[(2 * nlines_out)];
   };

};


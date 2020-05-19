
flibrary ReadPatranMods <build_dir="xp_comm_proj/rd_patrn",
                         out_hdr_file="gen.h",
                         out_src_file="gen.cxx"> {

   group+OPort ReadPatranParams {
      string+Port2 filename;

      int+Port2 points_visible;
      int+Port2 line_visible;
      int+Port2 tri_visible;
      int+Port2 quad_visible;
      int+Port2 tet_visible;
      int+Port2 patch_visible;
   };

   group+OPort neutral_output {

      // --- Descriptive Header Text ---
      string+Port2 title1;
      string+Port2 title2;

      // --- Node Data ---
      // vertices for all parts of model
      // *** NB *** they are double
      int+Port2 n_nodes;
      double+Port2 xyz[n_nodes][3];

      // --- Connectivity Data ---
      // simple elements
      int n_bar;
      int bar_conn[n_bar*2];
      int n_tri;
      int tri_conn[n_tri*3];
      int n_quad;
      int quad_conn[n_quad*4];
      int n_tet;
      int tet_conn[n_tet*4];
      int n_wedge;               // Not currently used
      int wedge_conn[n_wedge*6]; // Not currently used
      int n_hex;                 // Not currently used
      int hex_conn[n_hex*8];     // Not currently used

      // primitives to define complex shapes
      int n_line;                // Not currently used
      int line[2*n_line];        // Not currently used
      int n_patch;
      int patch[4*n_patch];

   };



   module ReadPatranCore <src_file="rd_patrn.cxx",
                          cxx_src_files = "patran.cxx",
                          cxx_hdr_files = "patran.hxx",
                          c_hdr_files = "pneutral.h rnum.h",
                          c_src_files = "rnum.c",
                          cxx_members= "load_neutral loadN;"> {

      ReadPatranParams+req+IPort2 &ReadPatranParams;
      string+req+read+notify filename => .ReadPatranParams.filename;

      neutral_output+write+OPort2 neutral_output;

      cxxmethod+notify_inst+req update;
   };

}; // end of library


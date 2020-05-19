
flibrary IsolabelMods <process="express",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.h",
                       build_dir="xp_comm_proj/isolabel"> {

   group+OPort Isolabel_Params {
      int+Port2    num_labels;
      string+Port2 format;
      int+Port2    dec_pnts;
   };


   module Isolabel_core <src_file="isolabel.c"> {

      omethod+req update (
                          .in+read+notify+req,
                          .in.nnodes+req,
                          .num_labels+read+notify+req,
                          .format+read+notify+req,
                          .out+write,
                          .string_vals+write
      ) = "isolabel_update";

      Mesh+Node_Data+IPort2  &in;
      int+IPort2             num_labels;
      string+IPort2          format;

      Mesh+OPort2 out {
		   ncell_sets = 1;
   		Point cell_set {
	   		ncells => <-.nnodes;
		   	node_connect_list => init_array(ncells, 0, ncells-1);
		   };
      };

      string+OPort2 string_vals[];
   };

};




flibrary OutlineDlgMods <build_dir="xp_comm_proj/dlgtools/outline",
                         out_src_file="gen.cxx",
                         out_hdr_file="gen.h"> {

   group+OPort OutlineDlgParams {
      int+Port2 n_polys;
      string+Port2 names[n_polys];
      int+Port2 selected[];
   };


   module OutlineDlgCore <src_file="outl_dlg.c"> {
      XP_COMM_PROJ.DlgToolkit.DlgData+IPort2 &dlg_in;
      int+IPort2 selected[];

      omethod+notify+req update(
         .dlg_in+read+notify+req,
         .selected+read+notify+req,
         .outline+write+nonotify
      ) = "dlg_outline_om";

      Mesh+OPort2 outline;
   };


   module SelectAllCore <src_file="sel_all.c"> {
      int+IPort2 choice[];
      int+IPort2 all_flag;
      int+IPort2 n_polys;

      omethod+notify+req update(
         choice+read+notify+req,
         all_flag+read+notify+req,
         n_polys+read+notify+req,
         list_size+write+nonotify,
         selected+write+nonotify
      ) = "select_all_om";

      int list_size;
      int+OPort2 selected[list_size];
   };

};


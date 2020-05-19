
flibrary WriteDlgMods <build_dir="xp_comm_proj/dlgtools/write",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.h"> {

   module WriteDlgCore <src_file="wr_dlg.c"> {
      XP_COMM_PROJ.DlgToolkit.DlgData+IPort2 &dlg;
      string+IPort2 filename;

      omethod+notify+req update(
         dlg+read+nonotify+req,
         filename+read+notify+req
      ) = "write_dlg_om";
   };

};


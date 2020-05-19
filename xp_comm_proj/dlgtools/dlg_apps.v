
flibrary DlgToolkitApps<compile_subs=0> {

   APPS.SingleWindowApp DlgToolkitEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.outline_dlg.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.DlgToolkit.ReadDlg.ReadDlgMacs.read_dlg read_dlg {
         filename = "$XP_PATH<1>/xp_comm_proj/dlgtools/data/florida.dlg";
      };

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMacs.outline_dlg outline_dlg {
         in => <-.read_dlg.out;
      };

/*
      XP_COMM_PROJ.DlgToolkit.WriteDlg.WriteDlgMacs.write_dlg write_dlg {
         in => <-.read_dlg.out;
         filename = "$XP_PATH<1>/test.dlg";
      };
*/
   };

};


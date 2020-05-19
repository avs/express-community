
flibrary ReadDlgMods <build_dir="xp_comm_proj/dlgtools/read",
                      out_src_file="gen.cxx",
                      out_hdr_file="gen.h">
{

   module ReadDlgCore <src_file="rd_dlg.c",
                       c_src_files="ram.c"> {

      string+IPort2 filename;

      omethod+notify+req update(
           .filename+read+notify+req,
           .file_info+write+nonotify,
           .dlg_out+write+nonotify
      ) = "read_dlg_om";

      string+OPort2 file_info;

      XP_COMM_PROJ.DlgToolkit.DlgData+OPort2 dlg_out
      {
          header {
            have_header = 0;
          };
          pb {
            have_political_boundaries = 0;
            n_nodes = 0;
            n_areas = 0;
            n_lines = 0;
            nodes {
              n_lines = 0;
            };
            lines {
              n_nodes=0;
              n_attributes = 0;
            };
            areas {
              n_lines = 0;
              n_attributes = 0;
            };
          };
          polys {
            n_polys = 0;
            total_nodes = 0;
            n_data = 0;
          };
      };
   };

};


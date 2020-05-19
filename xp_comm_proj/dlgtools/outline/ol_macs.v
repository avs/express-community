
flibrary OutlineDlgMacs {
  
   macro OutlineDlgUI {

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMods.OutlineDlgParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         message = "Select the DLG Outline control panel.";
         title => "DLG Outline";
      };

      UItoggle toggle_all {
         x = 2;
         y = 2;
         parent => <-.panel;
         label => "All Regions";
         set = 1;
      };

      UIoptionBoxLabel select_polygon {
         parent => <-.panel;
         labels => <-.params.names;
         title => "Select Polygons to Outline...";
         x = 0;
         y => <-.toggle_all.y + <-.toggle_all.height + 5;
         width => parent.width;
         selectedItems+OPort2;
      };


      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMods.SelectAllCore SelectAllCore {
         choice => <-.select_polygon.selectedItems;
         all_flag => <-.toggle_all.set;
         n_polys => <-.params.n_polys;
         selected+IPort2 => <-.params.selected;
      };
   };



   macro OutlineDlgFunc {
      ilink in;

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMods.OutlineDlgParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMods.OutlineDlgCore OutlineDlgCore {
         dlg_in => <-.in;
         selected => <-.params.selected;
      };

#ifdef VIZ_EXPRESS
      GDM.DataObject DataObject {
         in => <-.OutlineDlgCore.outline;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };
#else
      DataObjectLite DataObjectLite {
         in => <-.OutlineDlgCore.outline;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };
#endif

      olink out_fld => .OutlineDlgCore.outline;
#ifdef VIZ_EXPRESS
      olink out_obj => .DataObject.obj;
#else
      olink out_obj => .DataObjectLite.obj;
#endif
   };



   macro outline_dlg {
      ilink in;

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMods.OutlineDlgParams params {
         n_polys+nres => <-.in.polys.n_polys;
         names+nres => <-.in.polys.names;
      };

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMacs.OutlineDlgUI OutlineDlgUI {
         UImod_panel panel {
            title => name_of(<-.<-.<-);
         };
         params => <-.params;
      };

      XP_COMM_PROJ.DlgToolkit.OutlineDlg.OutlineDlgMacs.OutlineDlgFunc OutlineDlgFunc {
         in => <-.in;
         params => <-.params;
         out_obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_obj => .OutlineDlgFunc.out_obj;
      olink out_fld => .OutlineDlgFunc.out_fld;
   };

};


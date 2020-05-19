
flibrary WriteDlgMacs {
  
   macro WriteDlgUI {
      string &filename<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         message = "Select the DLG file to write to.";
         title => name_of(<-.<-.<-);
      };

      UIlabel DlgFileLabel {
         x = 10;
         y = 0;
         parent => <-.panel;
         alignment = "left";
         label = "DLG File...";
      };

      UItext DlgFileText {
         x = 0;
         y => <-.DlgFileLabel.y + <-.DlgFileLabel.height + 5;
         width => (parent.width * (2. / 3.)) - 2;
         parent => <-.panel;
         text => <-.filename;
      };
      UIbutton BrowseBtn {
         x => <-.DlgFileText.x + <-.DlgFileText.width + 4;
         y => <-.DlgFileText.y;
         width => (parent.width / 3.) - 2;
         parent => <-.panel;
         label = "Browse...";
      };

      UIfileDialog DlgFileDialog {
         visible => <-.BrowseBtn.do;
         title = "DLG File to Write";
         filename => <-.filename;
         searchPattern = "*.dlg";
      };
   };



   macro WriteDlgFunc {
      ilink in;
      string &filename<NEportLevels={2,1}>;

      XP_COMM_PROJ.DlgToolkit.WriteDlg.WriteDlgMods.WriteDlgCore WriteDlgCore {
         dlg => <-.in;
         filename => <-.filename;
      };
   };



   macro write_dlg {
      ilink in;
  
      string+OPort filename;

      XP_COMM_PROJ.DlgToolkit.WriteDlg.WriteDlgMacs.WriteDlgFunc WriteDlgFunc {
         in => <-.in;
         filename => <-.filename;
      };

      XP_COMM_PROJ.DlgToolkit.WriteDlg.WriteDlgMacs.WriteDlgUI WriteDlgUI {
         filename => <-.filename;
         UImod_panel panel {
            title => name_of(<-.<-.<-);
         };
      };
   };

};



flibrary ReadDlgMacs {
  
   macro ReadDlgUI {
      string &filename<NEportLevels={2,1}>;
      string &file_info<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         message = "Select Read DLG control panel.";
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
         title = "DLG File to Read";
         filename => <-.filename;
         searchPattern = "*.dlg";
      };

      UItext UItext {
         x => <-.DlgFileText.x;
         y => <-.DlgFileText.y + <-.DlgFileText.height + 10;
         width => parent.width - (2*x);
         height = 300;
         parent => <-.panel;
         text+IPort2 => <-.file_info;
         multiLine = 1;
         active = 1;
         resizeToText = 1;
         outputOnly = 1;
      };
   };


   macro ReadDlgFunc {
      string &filename<NEportLevels={2,1}>;

      XP_COMM_PROJ.DlgToolkit.ReadDlg.ReadDlgMods.ReadDlgCore ReadDlgCore {
         filename => <-.filename;
      };

      olink out => .ReadDlgCore.dlg_out;
      olink file_info => .ReadDlgCore.file_info;
   };



   macro read_dlg {
      string+OPort filename;

      XP_COMM_PROJ.DlgToolkit.ReadDlg.ReadDlgMacs.ReadDlgFunc ReadDlgFunc {
         filename => <-.filename;
      };

      XP_COMM_PROJ.DlgToolkit.ReadDlg.ReadDlgMacs.ReadDlgUI ReadDlgUI {
         filename => <-.filename;
         file_info => <-.ReadDlgFunc.file_info;

         UImod_panel panel {
            title => name_of(<-.<-.<-);
         };
      };

      olink out => .ReadDlgFunc.out;
   };


};


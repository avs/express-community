


flibrary GetFilesMacs<compile_subs=0> {

   macro MultiFileShell {
      XP_COMM_PROJ.GetFiles.GetFilesMods.GetFilesCore GetFilesCore {
         dir => <-.dirText.text;
         ext => <-.extText.text;
         nFiles = 0;
      };

      UIshell multiFileShell {
         visible => <-.visible;
         height = 400;
      };
      UIpanel panel {
         parent => <-.multiFileShell;
         x = 0;
         y = 0;
         width => multiFileShell.width;
         height => okButton.y - y - 5;
      };

      UIbutton okButton {
         parent => <-.multiFileShell;
         label => "OK";
         x => ((panel.width / 3) - (width / 2));
         y => multiFileShell.clientHeight - height - 5;
         width = 60;
         message = "Do multiple file selection";
      };
      UIbutton cancelButton {
         parent => <-.multiFileShell;
         label => "Cancel";
         do => (!<-.visible);
         x => (((2 * panel.width) / 3) - (width / 2));
         y => okButton.y;
         width => okButton.width;
         message = "Cancel multiple file selection";
      };


      UIlabel dirLabel {
         parent => <-.panel;
         label => "Directory";
         y = 16;
         x = 8;
         width = 60;
         alignment = "left";
      };
      UItext dirText {
         parent => <-.panel;
         text = "";
         x => (dirLabel.x + dirLabel.width);
         y => (dirLabel.y - (height / 4));
         width => (((panel.width - x) - dirButton.width) - (2 * dirLabel.x));
      };
      UIbutton dirButton {
         parent => <-.panel;
         label => "Browse";
         x => ((dirText.x + dirText.width) + dirLabel.x);
         y => (dirLabel.y - (height / 6));
         width => okButton.width;
      };
      UIdirectoryDialog dirDialog {
         visible => <-.dirButton.do;
         parent => <-.multiFileShell;
         filename => <-.dirText.text;
         isModal = 1;
         searchPattern => <-.extText.text;
      };


      UIlabel extLabel {
         parent => <-.panel;
         label => "File type";
         x => dirLabel.x;
         y => ((dirLabel.y + dirLabel.height) + (2 * dirLabel.y));
         width => dirLabel.width;
         alignment = "left";
      };
      UItext extText {
         parent => <-.panel;
         text = "";
         x => dirText.x;
         y => (extLabel.y - (height / 4));
         width => dirText.width;
      };

      UIlabel fileLabel {
         parent => <-.panel;
         label => "File Selection";
         x => ((panel.width / 2) - (width / 2));
         y => ((extLabel.y + extLabel.height) + dirLabel.y);
      };
      UImultiList fileMultiList {
         parent => <-.panel;
         strings => <-.GetFilesCore.files;
         x => dirLabel.x;
         y => (fileLabel.y + fileLabel.height);
         width => (panel.width - (2 * dirLabel.x));
         height => (panel.height - y - 5);
      };

      int visible<NEportLevels={2,1}> = 0;
      int do<NEportLevels={1,2}>;
      GMOD.parse_v do_ok {
         v_commands = "visible = 0; do = 1;";
         trigger => <-.okButton.do;
         on_inst = 0;
         relative => ".";
      };

      string filenames<NEportLevels={1,2}>[] => .fileMultiList.selectedText;
      string dir<NEportLevels={1,2}> => .dirText.text;
      string filenames_with_dir<NEportLevels={1,2}>[];

      GMOD.parse_v do_filenames_with_dir {
#ifdef MSDOS
         v_commands = "filenames_with_dir = str_format(\"%s\\\\%s\", dir, filenames);";
#else
         v_commands = "filenames_with_dir = str_format(\"%s/%s\", dir, filenames);";
#endif         
         trigger => <-.do;
         on_inst = 0;
         relative = ".";
      };

   };


   macro Multiple_File_Select {
      UImod_panel UImod_panel {
         title => name_of(<-.<-,1);
      };
      UIbutton selButton {
         parent => <-.UImod_panel;
         label => "Select";
         x = 0;
         y = 0;
         message = "Display selection dialog";
      };

      UIlist selList {
         parent => <-.UImod_panel;
         x = 0;
         y => <-.selButton.y + <-.selButton.height + 15;
         width => UImod_panel.width;
         height => (20 * UIdata.UIfonts[0].lineHeight);
         strings => <-.MultiFileShell.filenames_with_dir;
         active = 1;
      };

      GMOD.instancer instancer {
         Value => <-.selButton.do;
         active = 2;
         Group => MultiFileShell;
      };
      XP_COMM_PROJ.GetFiles.GetFilesMacs.MultiFileShell MultiFileShell<instanced=0> {
         visible => <-.selButton.do;
      };

      olink filenames_with_dir => MultiFileShell.filenames_with_dir;
      olink selected_filename => selList.selectedText;
   };


};


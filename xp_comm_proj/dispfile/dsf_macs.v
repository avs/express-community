
flibrary DisplaySplitFilenameMacs <compile_subs=0> {

   macro DSF_UI {
      ilink parent;

      string  &FileName<NEportLevels={2,1}>;
      string  &Path<NEportLevels={2,1}>;
      boolean &ValidName<NEportLevels={2,1}>;

      int+Port x = 0;
      int+Port y = 0;
      int+Port width = 200;
      int+Port height => .PathLabel.height + .FileLabel.height + 12;

      UIframe UIframe {
         parent => <-.parent;
         y => <-.y;
         x => <-.x;
         width => <-.width;
         height => <-.height;
      };


      GMOD.copy_on_change path_change {
         input => <-.Path;
      };  
      UIlabel PathLabel {
         x = 5;
         y = 5;
         width = 35;
         parent => <-.UIframe;
         label => "Path>";
         alignment = "right";
         fontAttributes {
            weight = "bold";
         };
      };
      UIlabel PathValue {
         x => <-.PathLabel.x + <-.PathLabel.width + 5;
         y => <-.PathLabel.y;
         width => <-.UIframe.width - x - 5;
         parent => <-.UIframe;
         label => <-.path_change.output;
         alignment = "left";
         visible => <-.ValidName;
      };


      GMOD.copy_on_change file_change {
         input => <-.FileName;
      };
      UIlabel FileLabel {
         x => <-.PathLabel.x;
         y => <-.PathLabel.y +  <-.PathLabel.height + 2;
         width => <-.PathLabel.width;
         parent => <-.UIframe;
         label => "File>";
         alignment = "right";
         fontAttributes {
            weight = "bold";
         };
      };
      UIlabel FileValue {
         x => <-.FileLabel.x + <-.FileLabel.width + 5;
         y => <-.FileLabel.y;
         width => <-.UIframe.width - x - 5;
         parent => <-.UIframe;
         label => <-.file_change.output;
         alignment = "left";
         visible => <-.ValidName;
      };
   };



   macro display_split_filename {
      ilink parent;
      ilink FullFileName;

      XP_COMM_PROJ.DisplaySplitFilename.DisplaySplitFilenameMods.DSFCore DSFCore {
         FullFileName => <-.FullFileName;
      };

      XP_COMM_PROJ.DisplaySplitFilename.DisplaySplitFilenameMacs.DSF_UI DSF_UI {
         parent => <-.parent;
         FileName => <-.DSFCore.FileName;
         Path => <-.DSFCore.Path;
         ValidName => <-.DSFCore.ValidName;
      };
   };



   APPS.DefaultApplication DisplaySplitFilenameEg {
      UIshell UIshell {
         width = 320;
         title => name_of(<-.<-,1);
      };

      UIbutton ShowFileDialogButton {
         parent => <-.UIshell;
         label = "Select File...";
         x = 5;
         y = 5;
      };

      UIfileDialog UIfileDialog {
         visible => <-.ShowFileDialogButton.do;
         parent => <-.UIshell;
         title => "Select Filename to Split";
         searchPattern = "*.*";
      };

      XP_COMM_PROJ.DisplaySplitFilename.DisplaySplitFilenameMacs.display_split_filename display_split_filename {
         parent => <-.UIshell;
         FullFileName => <-.UIfileDialog.filename;

         DSF_UI {
            x = 5;
            y => <-.<-.ShowFileDialogButton.y + <-.<-.ShowFileDialogButton.height + 10;
            width = 300;
         };
      };
   };


};


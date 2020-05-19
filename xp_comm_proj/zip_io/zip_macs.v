
// Defines library used to hold high-level components
// Also specifies that this library should not be considered during compilation
flibrary ZipIOMacs <compile_subs=0> {


   // User Interface macros
   macro CompressFileUI {

      // Reference to external parameter block
      XP_COMM_PROJ.ZipIO.ZipIOMods.ZipIOParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         message = "Select Compress File control panel.";
         title = "Compress File";
      };

      UIlabel titleLabel {
         parent => <-.panel;
         label => "Compress File Control Panel";
         x = 0;
         y = 0;
         width => parent.width;
         color {
            foregroundColor = "white";
            backgroundColor = "blue";
         };
      };

      UIlabel filenameLabel {
         parent => <-.panel;
         label => "File to Compress...";
         x = 0;
         y => <-.titleLabel.y + <-.titleLabel.height + 10;
      };
      UItext filenameText {
         parent => <-.panel;
         text => <-.params.filename;
         x => <-.filenameLabel.x;
         y => <-.filenameLabel.y + <-.filenameLabel.height + 5;
         width => (0.7 * parent.width) - 2;
      };
      UIbutton filenameButton {
         parent => <-.panel;
         label => "Browse...";
         x => <-.filenameText.x + <-.filenameText.width + 2;
         y => <-.filenameText.y;
         width => (0.3 * parent.width) - 4;
      };
      UIfileDialog filenameDialog {
         visible => <-.filenameButton.do;
         filename => <-.params.filename;
         parent => <-.panel;
         title => "Select File to Compress";
         searchPattern = "*.*";
         dirMaskCache = "*.*";
      };

      UItoggle removeOrigToggle {
         parent => <-.panel;
         label = "Remove Original after Compression";
         x => <-.filenameText.x + 30;
         y => <-.filenameText.y + <-.filenameText.height + 10;
         width => parent.width - x;
         set => <-.params.remove_orig;
         color {
            foregroundColor = "red";
         };
      };

      UIlabel outputDirLabel {
         parent => <-.panel;
         label => "Output Directory...";
         x => <-.filenameText.x;
         y => <-.removeOrigToggle.y + <-.removeOrigToggle.height + 20;
      };
      UItext outputDirText {
         parent => <-.panel;
         text => <-.params.output_dir;
         x => <-.outputDirLabel.x;
         y => <-.outputDirLabel.y + <-.outputDirLabel.height + 5;
         width => <-.filenameText.width;
      };
   };


   CompressFileUI DecompressFileUI {
      panel {
         message = "Select Decompress File control panel.";
         title = "Decompress File";
      };

      titleLabel {
         label => "Decompress File Control Panel";
      };

      filenameLabel {
         label => "File to Decompress...";
      };

      filenameDialog {
         title => "Select File to Decompress";
         searchPattern = "*.gz";
         dirMaskCache = "*.gz";
      };

      removeOrigToggle {
         label = "Remove Original after Decompression";
      };
   };




   // compress_file User Macro.
   // Contains and connects together the real instances of the parameter block,
   // low-level module and UI macro.
   macro compress_file {
      string &in_filename<NEportLevels={2,1}>;

      // Real instance of the ZipIO parameter block.
      // This is the only real instance used in the project.
      XP_COMM_PROJ.ZipIO.ZipIOMods.ZipIOParams params {
         remove_orig = 0;
      };

      // UI macro with connections to parameter blocks
      XP_COMM_PROJ.ZipIO.ZipIOMacs.CompressFileUI CompressFileUI {
         params => <-.params;
      };

      // Functional macro with connections to parameter blocks
      XP_COMM_PROJ.ZipIO.ZipIOMods.CompressFileCore CompressFileCore {
         in_filename => <-.in_filename;
         params => <-.params;
      };

      // Output links. These help simplify generated V-code. 
      olink out_filename => .CompressFileCore.out_filename;
   };



   // decompress_file User Macro.
   // Contains and connects together the real instances of the parameter block,
   // low-level module and UI macro.
   macro decompress_file {
      string &in_filename<NEportLevels={2,1}>;

      // Real instance of the ZipIO parameter block.
      // This is the only real instance used in the project.
      XP_COMM_PROJ.ZipIO.ZipIOMods.ZipIOParams params {
         remove_orig = 0;
      };

      // UI macro with connections to parameter blocks
      XP_COMM_PROJ.ZipIO.ZipIOMacs.DecompressFileUI DecompressFileUI {
         params => <-.params;
      };

      // Functional macro with connections to parameter blocks
      XP_COMM_PROJ.ZipIO.ZipIOMods.DecompressFileCore DecompressFileCore {
         in_filename => <-.in_filename;
         params => <-.params;
      };

      // Output links. These help simplify generated V-code. 
      olink out_filename => .DecompressFileCore.out_filename;
   };


};



flibrary ReadRawMacs<compile_subs=0> 
{

   macro ReadRawUI {
      string &filename<NEportLevels={2,1}>;

      UImod_panel ReadRawPanel {
         parent<NEportLevels={4,0}>;
         message = "Select the RAW file to read.";
         title => name_of(<-.<-.<-);
      };

      UIlabel RawFileLabel {
         x = 10;
         y = 0;
         parent => <-.ReadRawPanel;
         alignment = "left";
         label = "Raw File...";
      };

      UItext RawFileText {
         x = 0;
         y => <-.RawFileLabel.y + <-.RawFileLabel.height + 5;
         width => (parent.width * (2. / 3.)) - 2;
         parent => <-.ReadRawPanel;
         text => <-.filename;
      };
      UIbutton BrowseBtn {
         x => <-.RawFileText.x + <-.RawFileText.width + 4;
         y => <-.RawFileText.y;
         width => (parent.width / 3.) - 2;
         parent => <-.ReadRawPanel;
         label = "Browse...";
      };

      UIfileDialog RawFileDialog {
         visible => <-.BrowseBtn.do;
         filename => <-.filename;
         searchPattern = "*.raw";
      };
   };


   macro ReadRawFunc {
      string &filename<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadRaw.ReadRawMods.ReadRawCore ReadRawCore {
         filename => <-.filename;
      };

      FLD_MAP.tri_mesh tri_mesh {
         coord => <-.ReadRawCore.coords;
         connect => <-.ReadRawCore.conn;
      };

      olink out_fld => .tri_mesh.out;
      olink out_obj => .tri_mesh.obj;
   };


   macro read_raw {
      string+OPort filename;

      XP_COMM_PROJ.ReadRaw.ReadRawMacs.ReadRawUI ReadRawUI {
         filename => <-.filename;
      };

      XP_COMM_PROJ.ReadRaw.ReadRawMacs.ReadRawFunc ReadRawFunc {
         filename => <-.filename;
      };

      olink out_fld => .ReadRawFunc.out_fld;
      olink out_obj => .ReadRawFunc.out_obj;
   };


};


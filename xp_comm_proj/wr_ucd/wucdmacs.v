
flibrary WriteUCDMacs <compile_subs=0> {

   macro WriteUCDUI {
      string &filename<NEportLevels={2,1}>;
      int    &err_vis<NEportLevels={2,1}>;
      string &err_msg<NEportLevels={2,1}>;
      int    &binary<NEportLevels={2,1}>;

      UImod_panel panel {
         title => name_of(<-.<-.<-,1);
         message = "Select Write UCD control panel.";
         parent<NEportLevels={4,0}>;
      };
      UIlabel UCD_Filename {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = 0;
         label = "Write UCD File Name";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };
      UItext file_name {
         parent => panel;
         x = 0;
         y => <-.UCD_Filename.y + UCD_Filename.height + 5;
         width => (parent.width * 0.6667) - 2;
         text => <-.filename;
         showLastPosition = 1;
      };
      UIbutton visible {
         parent => panel;
         x => <-.file_name.width + 4;
         y => <-.file_name.y;
         width => parent.width - <-.file_name.width - 4;
         height => <-.file_name.height;
         label = "Browse...";
      };

      UIradioBox UIradioBox {
         parent => panel;
          cmdList => {<-.UIoptionBinary, <-.UIoptionASCII};           
          UIradioBox.selectedItem => !binary;
          x = 4;
          y => <-.file_name.y + file_name.height + 4;
          width = 200;
      };
      UIoption UIoptionBinary {
         label = "Binary Format";
      };
      UIoption UIoptionASCII {
         label = "ASCII Format";
      };

      UIfileSB file_browser {
         GMOD.copy_on_change copy_on_change {
            trigger => <-.<-.visible.do; 
            input => <-.<-.visible.do;
            output => <-.visible;
         };
         title = "Write UCD Filename";
         searchPattern = "*.inp";
         filename => <-.filename;
      };


      UIlabel err_label {
         parent => <-.panel;
         visible => <-.err_vis;
         x = 0;
         y => file_name.y + file_name.height + 5;
         width => parent.width;
         alignment = 0;
         label = "Read Error:";
         color {
            foregroundColor = "red";
         };
      };
      UIlabel err_label_msg {
         parent => <-.panel;
         visible => <-.err_vis;
         x = 0;
         y => err_label.y + err_label.height + 5;
         width => parent.width;
         alignment = 0;
         label => <-.err_msg;
      };
   };



   macro write_ucd {
      Mesh+Node_Data &in_fld<NEportLevels={2,1}>;

      string+OPort filename = "$XP_PATH<1>/test.inp";
      string+OPort name = "default_avs_field";
      int+OPort binary = 0;

      XP_COMM_PROJ.WriteUCD.WriteUCDMods.WriteUCDCore WriteUCDCore {
         filename => <-.filename;
         in => {<-.in_fld};
         nobjs = 1;
         top_name => <-.name;
         binary => <-.binary;
      };

      WriteUCDUI WriteUCDUI {
         filename => <-.filename;
         err_vis =>  <-.WriteUCDCore.err;
         err_msg =>  <-.WriteUCDCore.err_str;
         binary => <-.binary;
      };
   };



   macro write_ucd_ARR {
      imlink in_flds;

      string+OPort filename = "$XP_PATH<1>/test.inp";
      string+OPort name = "default_avs_field";
      string name = "default_avs_field";
      int binary;

      XP_COMM_PROJ.WriteUCD.WriteUCDMods.WriteUCDCore WriteUCDCore {
         filename => <-.filename;
         in => <-.in_flds;
         nobjs => array_size(in);
         top_name => <-.name;
         binary => <-.binary;
         
      };

      WriteUCDUI WriteUCDUI {
         filename => <-.filename;
         err_vis =>  <-.WriteUCDCore.err;
         err_msg =>  <-.WriteUCDCore.err_str;
         binary => <-.binary;
      };
   };

};


flibrary ReadDtedMacs<compile_subs=0> {

   //
   // User interface macro for standard Read Dted module
   //
   macro Read_Dted_UI {
      XP_COMM_PROJ.ReadDted.ReadDtedMods.ReadDtedParams &params<NEportLevels={2,1}>;


      UImod_panel modPanel {
         title => name_of(<-.<-.<-,1);
         message = "Select Read DTED control panel.";
         parent<NEportLevels={4,0}>;
      };

      UIlabel titleLabel {
         parent => <-.modPanel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = 0;
         label = "Read DTED Control Panel";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UItext filenameText {
         parent => modPanel;
         x => titleLabel.x;
         y => titleLabel.y + titleLabel.height + 5;
         width => parent.width * 0.7;
         text => <-.params.Filename;
         showLastPosition = 1;
      };
      UIbutton visible {
         parent => modPanel;
         x => filenameText.x + filenameText.width + 5;
         y => filenameText.y;
         width => parent.width - x - 5;
         height => <-.filenameText.height;
         label = "Browse...";
      };

      UIfileSB file_browser {
         GMOD.copy_on_change copy_on_change {
            trigger => <-.<-.visible.do; 
            input => <-.<-.visible.do;
            output => <-.visible;
         };
         title = "Read DTED Filename";
         searchPattern = "./*.dt?";
         filename => <-.params.Filename;
      };


      UIlabel startRowLabel {
         parent => <-.modPanel;
         label => "Start Row";
         x => filenameText.x;
         y => startRowField.y + (startRowField.height * 0.25);
         width => parent.width * 0.55;
         alignment = "right";
      };
      UIfield startRowField {
         parent => <-.modPanel;
         value => <-.params.StartRow;
         x => startRowLabel.x + startRowLabel.width + 10;
         y => filenameText.y + filenameText.height + 25;
         width => parent.width - x - 5;
         min = 1.;
         mode = "integer";
         decimalPoints = 0;
      };


      UIlabel startColumnLabel {
         parent => <-.modPanel;
         label => "Start Column";
         x => startRowLabel.x;
         y => startColumnField.y + (startColumnField.height * 0.25);
         width => parent.width * 0.55;
         alignment = "right";
      };
      UIfield startColumnField {
         parent => <-.modPanel;
         value => <-.params.StartColumn;
         x => startColumnLabel.x + startColumnLabel.width + 10;
         y => startRowField.y + startRowField.height + 5;
         width => parent.width - x - 5;
         min = 1.;
         mode = "integer";
         decimalPoints = 0;
      };


      UIlabel numberOfRowsLabel {
         parent => <-.modPanel;
         label => "Number of Output Rows";
         x => startColumnLabel.x;
         y => numberOfRowsField.y + (numberOfRowsField.height * 0.25);
         width => parent.width * 0.55;
         alignment = "right";
      };
      UIfield numberOfRowsField {
         parent => <-.modPanel;
         value => <-.params.NumberOfRows;
         x => numberOfRowsLabel.x + numberOfRowsLabel.width + 10;
         y => startColumnField.y + startColumnField.height + 5;
         width => parent.width - x - 5;
         min = 1.;
         mode = "integer";
         decimalPoints = 0;
      };


      UIlabel numberOfColumnsLabel {
         parent => <-.modPanel;
         label => "Number of Output Columns";
         x => numberOfRowsLabel.x;
         y => numberOfColumnsField.y + (numberOfColumnsField.height * 0.25);
         width => parent.width * 0.55;
         alignment = "right";
      };
      UIfield numberOfColumnsField {
         parent => <-.modPanel;
         value => <-.params.NumberOfColumns;
         x => numberOfColumnsLabel.x + numberOfColumnsLabel.width + 10;
         y => numberOfRowsField.y + numberOfRowsField.height + 5;
         width => parent.width - x - 5;
         min = 1.;
         mode = "integer";
         decimalPoints = 0;
      };

      UIslider yscaleSlider {
         parent => <-.modPanel;
         value => params.YScaleFactor;
         title => "Row Scaling";
         x => numberOfColumnsLabel.x;
         y => numberOfColumnsField.y + numberOfColumnsField.height + 15;
         width => parent.width;
         message = "Row Scaling Value";
         min = 0.1;
         max = 5.;
         mode = "real";
      };

      UIslider xscaleSlider {
         parent => <-.modPanel;
         value => params.XScaleFactor;
         title => "Column Scaling";
         x => yscaleSlider.x;
         y => yscaleSlider.y + yscaleSlider.height + 5;
         width => parent.width;
         message = "Column Scaling Value";
         min = 0.1;
         max = 5.;
         mode = "real";
      };


      UItoggle updateToggle {
         parent => <-.modPanel;
         x => xscaleSlider.x;
         y => xscaleSlider.y + xscaleSlider.height + 15;
         width => parent.width;
         label => "Enable module update";
         set => params.ActivateFlag;
      };
   };


   //
   // Modified User interface macro for alternative Read Dted module
   //
   Read_Dted_UI Read_Dted_UI_Alt {
      titleLabel {
         label => "Alternative Read DTED Control Panel";
      };
      numberOfRowsLabel {
         label => "Number of Input Rows";
      };
      numberOfColumnsLabel {
         label => "Number of Input Columns";
      };
   };



   //
   // Functional macro for standard Read Dted module
   //
   macro Read_Dted {
      XP_COMM_PROJ.ReadDted.ReadDtedMods.ReadDtedParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadDted.ReadDtedMods.ReadDtedCore ReadDtedCore {
         params => <-.params;
      };

      GDM.DataObject DtedObj {
         in => <-.ReadDtedCore.Data;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_fld => .ReadDtedCore.Data;
      olink out_obj => .DtedObj.obj;
   };


   //
   // Modified Functional macro for alternative Read Dted module
   //
   Read_Dted Read_Dted_Alt {
      -ReadDtedCore;

      XP_COMM_PROJ.ReadDted.ReadDtedMods.ReadDtedCoreAlt ReadDtedCoreAlt {
         params => <-.params;
      };

      DtedObj {
         in => <-.ReadDtedCoreAlt.Data;
      };

      out_fld => .ReadDtedCoreAlt.Data;
   };



   //
   // User macro for standard Read Dted module
   //
   macro read_dted {
      XP_COMM_PROJ.ReadDted.ReadDtedMods.ReadDtedParams params {
         StartRow = 1;
         StartColumn = 1;
         XScaleFactor = 1.0;
         YScaleFactor = 1.0;
         ActivateFlag = 1;
      };

      XP_COMM_PROJ.ReadDted.ReadDtedMacs.Read_Dted_UI Read_Dted_UI {
         params => <-.params;
      };

      XP_COMM_PROJ.ReadDted.ReadDtedMacs.Read_Dted Read_Dted {
         params => <-.params;
         DtedObj {
            Obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      olink out_fld => .Read_Dted.out_fld;
      olink out_obj => .Read_Dted.out_obj;
   };


   //
   // Modified User macro for alternative Read Dted module
   //
   read_dted read_dted_alt {
      -Read_Dted;
      -Read_Dted_UI;

      XP_COMM_PROJ.ReadDted.ReadDtedMacs.Read_Dted_UI_Alt Read_Dted_UI_Alt {
         params => <-.params;
      };

      XP_COMM_PROJ.ReadDted.ReadDtedMacs.Read_Dted_Alt Read_Dted_Alt {
         params => <-.params;
         DtedObj {
            Obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      out_fld => .Read_Dted_Alt.out_fld;
      out_obj => .Read_Dted_Alt.out_obj;
   };



   //
   // Example application that demonstrates use of both standard and
   // alternative forms of the Read Dted module
   //
   APPS.MultiWindowApp ReadDtedEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.read_dted.out_obj};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.AltScene.View.View};
         };
         GDM.Uscene3D AltScene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.read_dted_alt.out_obj};
            };
         };
      };


      XP_COMM_PROJ.ReadDted.ReadDtedMacs.read_dted read_dted {
         params {
            Filename = "$XP_PATH<1>/xp_comm_proj/rd_dted/w106/n38.dt0";
            StartRow = 10;
            StartColumn = 10;
            NumberOfRows = 40;
            NumberOfColumns = 40;
            XScaleFactor = 0.75;
            YScaleFactor = 0.75;
         };
         Read_Dted {
            DtedObj {
               Modes {
                  mode = {0,2,0,0,0};
               };
            };
         };
      };


      XP_COMM_PROJ.ReadDted.ReadDtedMacs.read_dted_alt read_dted_alt {
         params {
            Filename = "$XP_PATH<1>/xp_comm_proj/rd_dted/w106/n38.dt0";
            StartRow = 10;
            StartColumn = 10;
            NumberOfRows = 40;
            NumberOfColumns = 40;
            XScaleFactor = 0.75;
            YScaleFactor = 0.75;
         };
         Read_Dted_Alt {
            DtedObj {
               Modes {
                  mode = {0,2,0,0,0};
               };
            };
         };
      };

   };

};


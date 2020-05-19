
flibrary ReadTinMacs 
{ 
   macro ReadTinUI {
      XP_COMM_PROJ.ReadTin.ReadTinMods.ReadTinParams &params<NEportLevels={2,1}>; 

      UImod_panel UImod_panel {
         title => "Read Tin";
         parent<NEportLevels={3,0}>;
      };

      UIlabel FilenameLabel {
         parent => <-.UImod_panel;
         label => "Read Tin Filename...";
         x = 0;
         y = 0;
         width => parent.width;
      };
      UItext FilenameText {
         parent => <-.UImod_panel;
         text => <-.params.filename;
         x => <-.FilenameLabel.x;
         y => <-.FilenameLabel.y + <-.FilenameLabel.height + 5;
         width => parent.width * (2./3.) - 3;
      };
      UIbutton FilenameButton {
         parent => <-.UImod_panel;
         label => "Browse...";
         x => <-.FilenameText.x + <-.FilenameText.width + 6;
         y => <-.FilenameText.y;
         width => parent.width * (1./3.) - 3;
      };
      UIfileDialog FilenameFileDialog {
         visible => <-.FilenameButton.do;
         title => "Read Tin Filename...";
         searchPattern = "*.net";
         filename => <-.params.filename;
      };

      UItoggle ZeroElevationToggle {
         parent => <-.UImod_panel;
         label => "Zero Elevation";
         set => <-.params.ZeroElevations;
         x => <-.FilenameText.x;
         y => <-.FilenameText.y + <-.FilenameText.height + 30;
         width => parent.width;
      };
      UIdial ZScaleDial {
         parent => <-.UImod_panel;
         value => <-.params.ZScale;
         title => "Elevation Scale";
         min = 0.;
         max = 10.;
         mode = "real";
         x => <-.ZeroElevationToggle.x;
         y => <-.ZeroElevationToggle.y + <-.ZeroElevationToggle.height + 10;
      };
   };



   macro ReadTinFunc {
      XP_COMM_PROJ.ReadTin.ReadTinMods.ReadTinParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadTin.ReadTinMods.ReadTinCore ReadTinCore {
      	filename => <-.params.filename;
      	ZScale => <-.params.ZScale;
      	ZeroElevations => <-.params.ZeroElevations;
      };
      GDM.DataObject DataObject {
         in => <-.ReadTinCore.Geometry;
         Obj.name => name_of(<-.<-.<-);
      };

      olink out_mesh => .ReadTinCore.Geometry;
      olink out_obj  => .DataObject.obj;
   };



   macro read_tin {
      XP_COMM_PROJ.ReadTin.ReadTinMods.ReadTinParams params;

      ReadTinFunc ReadTinFunc {
  			params => <-.params;
      	DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };
      ReadTinUI ReadTinUI {
   		params => <-.params;
         UImod_panel {
            title => name_of(<-.<-.<-,1);
         };
      };

      olink out_mesh => .ReadTinFunc.out_mesh;
      olink out_obj  => .ReadTinFunc.out_obj;
   };

};



flibrary PrintFieldMacs {

  // PrintFieldUI
  //   UI Macro

  macro PrintFieldUI {

    XP_COMM_PROJ.PrintField.PrintFieldMods.PrintFieldParams
      &PrintFieldParams <NEportLevels={2,0},export=1>;

    UImod_panel UImod_panel {
      title = "PrintField";
      message = "Print Field panel.";
      parent<NEportLevels={4,0}>;
    };

    UIlabel PrtfldWin {
      parent => <-.UImod_panel;
      y = 0;
      width => parent.width;
      alignment = 0;
      label = "Print Field Data Selection:";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    // Grid, Cell, Nodes, Xform  

    UItoggle gridtoggle <NEx=495.,NEy=88.> {
      parent => <-.UImod_panel;
      label => "Grid Data";
      y => PrtfldWin.y + PrtfldWin.height;
      set <NEportLevels={2,0},export=2> => <-.PrintFieldParams.gridout;
    };

    UItoggle celltoggle<NEx=495.,NEy=132.> {
      parent => <-.UImod_panel;
      label => "Cell Data";
      y => gridtoggle.y + gridtoggle.height;
      set<NEportLevels={2,0}> => <-.PrintFieldParams.cellout;
    };

    UItoggle nodestoggle<NEx=495.,NEy=264.> {
      parent => <-.UImod_panel;
      label => "Nodes Data";
      y => celltoggle.y + celltoggle.height;
      set<NEportLevels={2,0}> => <-.PrintFieldParams.nodesout;
    };

    UItoggle xformtoggle<NEx=495.,NEy=220.> {
      parent => <-.UImod_panel;
      label => "Xform Data";
      y => nodestoggle.y + nodestoggle.height;
      set<NEportLevels={2,0}> => <-.PrintFieldParams.xformout;
    };

    UItoggle alltoggle<NEx=495.,NEy=40.> {
      parent => <-.UImod_panel;
      label => "Print All Points";
      y => xformtoggle.y + xformtoggle.height;
      set<NEportLevels={2,0},export=2> => <-.PrintFieldParams.allout;
    };

    UIlabel UCD_Filename {
      parent => <-.UImod_panel;
      y => alltoggle.y + alltoggle.height*2;
      width => parent.width;
      alignment = 0;
      label = "Enter Print Field File Name";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UItext file_name {
      parent => <-.UImod_panel;
      y => UCD_Filename.y + UCD_Filename.height;
      width => parent.width*3/4 -15;
      text<NEportLevels={2,0},export=2> => <-.PrintFieldParams.filename;
      showLastPosition = 1;
    };

    UIbutton visible {
      parent => <-.UImod_panel;
      x => file_name.x + file_name.width + 5;
      y => file_name.y;
      // width => parent.widith*1/4 - 40;
      width = 70;
      height => <-.file_name.height;
      label = "Browse";
    };

    UItoggle htmltoggle<NEx=495.,NEy=264.> {
      parent => <-.UImod_panel;
      label => "Print HTML file";
      y => file_name.y + file_name.height;
      width => parent.width;
      set<NEportLevels={2,0}> => <-.PrintFieldParams.htmlout;
    };

    UIbutton trigger<NEx=495.,NEy=264.> {
      parent => <-.UImod_panel;
      label => "Click to Start Printing";
      y => htmltoggle.y + htmltoggle.height*2;
      width => parent.width;
      do<NEportLevels={2,0}> => <-.PrintFieldParams.trigger;
    };

    UIfileSB file_browser {
      GMOD.copy_on_change copy_on_change {
        trigger => <-.<-.visible.do; 
        input => <-.<-.visible.do;
        output => <-.visible;
      };
      title = "Select/Enter Print Field Filename";
      searchPattern = "/tmp/*";
      filename<NEportLevels={2,0},export=2> => <-.PrintFieldParams.filename;
    };

    UIlabel UIPrtData {
      parent => <-.UImod_panel;
      y => trigger.y + trigger.height*2;
      width => parent.width;
      alignment = 0;
      label = "Node/Cell Data Attributes";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UItext UIgridtxt<NEx=286.,NEy=176.> {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.PrintFieldParams.notes;
      y => UIPrtData.y + UIPrtData.height;
      width => parent.width;
      height<NEdisplayMode="open"> = 160; 
      multiLine = 1;
      outputOnly<NEdisplayMode="open"> = 1;
    };

    UIlabel UIPrtStatus {
      parent => <-.UImod_panel;
      y => UIgridtxt.y + UIgridtxt.height + 10;
      width => parent.width;
      alignment = 0;
      label = "Printing Status";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UItext UItext<NEx=286.,NEy=176.> {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.PrintFieldParams.message;
      y => UIPrtStatus.y + UIPrtStatus.height;
      width => parent.width;
      outputOnly<NEdisplayMode="open"> = 1;
    };
  };

  // PrintField----
  //   Functional macro (Field I/O)

  // PrintField
  //   User Macro (PrintFieldUI)

  macro print_field {
    link in <NEportLevels={2,1},NEx=88.,NEy=44.>;

    XP_COMM_PROJ.PrintField.PrintFieldMods.PrintFieldParams
        PrintFieldParams <NEx=22.,NEy=88.> {
	  gridout=1;
	  cellout=1;
	  xformout=1;
	  nodesout=1;
          htmlout=1;
          trigger=1;
          allout=0;
	};

    XP_COMM_PROJ.PrintField.PrintFieldMods.PrintField 
        PrintField <NEx=385.,NEy=165.> {
      PrintFieldParams => <-.PrintFieldParams;
      in => <-.in;
    };

    XP_COMM_PROJ.PrintField.PrintFieldMacs.PrintFieldUI
        PrintFieldUI <NEx=231.,NEy=165.> {
      &PrintFieldParams => <-.PrintFieldParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };
  };

  // PrintFieldEg
  //   Example Application (SingleWindowApp)

  APPS.SingleWindowApp PrintFieldEg <NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 24;
        y = 288;
      };
      Modules {
        IUI {
          optionList {
            selectedItem = 1;
          };
        };
      };
    };

    GDM.Uviewer3D Uviewer3D{
       Scene {
          Top {
             child_objs => {
                <-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice.out_obj};
          };
          View {
             View {
                renderer => "Software";
             };
          };
       };
    };

    MODS.Read_Field Read_Field<NEx=33.,NEy=22.> {
      read_field_ui {
        file_browser {
          x = 490;
          y = 267;
          width = 300;
          height = 390;
          ok = 1;
          dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
        };
        filename = "$XP_PATH<0>/data/field/hydrogen.fld";
      };

      DVread_field {
        Mesh_Unif+Node_Data Output_Field;
      };
    };

    XP_COMM_PROJ.PrintField.PrintFieldMacs.print_field
        print_field <NEx=176.,NEy=88.> {
      in => <-.Read_Field.field;
      PrintFieldUI {
        UImod_panel {
          option {
            set = 1;
          };
        };
      };
    };
    MODS.bounds bounds<NEx=539.,NEy=99.> {
       in_field => <-.Read_Field.field;
    };
    MODS.orthoslice orthoslice<NEx=396.,NEy=121.> {
       in_field => <-.Read_Field.field;
       OrthoSliceParam {
         axis = 2;
       };
    };	
  };
};





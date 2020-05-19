
flibrary NullMaskMacs {

  macro NullMaskUI {
    UImod_panel panel<NEx=55.,NEy=66.> {
      parent<NEportLevels={3,0}>;
      title = "NullMaskUI";
    };

    XP_COMM_PROJ.NullMask.NullMaskMods.NullMaskParams
        &NullMaskParams <NEx=242.,NEy=33.,NEportLevels={2,1}>;

    UIlabel toplabel <NEx=176.,NEy=121.> {
      parent => <-.panel;
      label => "Null Mask Controls";
      width => parent.width;
      y = 0;
    };

    UIlabel validlabel <NEx=176.,NEy=165.> {
      parent => <-.panel;
      label => "Output Valid";
      width => (parent.width / 2);
      y = 30;
    };
    UIfield out_valid <NEx=176.,NEy=209.> {
      parent => <-.panel;
      value => <-.NullMaskParams.out_valid;
      width => (parent.width / 2);
      x => (<-.validlabel.x + <-.validlabel.width);
      y => validlabel.y;
    };

    UIlabel nulllabel <NEx=176.,NEy=253.> {
      parent => <-.panel;
      label => "Output Null";
      width => (parent.width / 2);
      y = 60;
    };
    UIfield out_null <NEx=176.,NEy=297.> {
      parent => <-.panel;
      value => <-.NullMaskParams.out_null;
      width => (parent.width / 2);
      x => (<-.nulllabel.x + <-.nulllabel.width);
      y => nulllabel.y;
    };

    UIlabel innulllabel <NEx=176.,NEy=341.> {
      parent => <-.panel;
      label => "User-Defined Null";
      width => (parent.width / 2);
      y = 90;
    };
    UIfield in_null <NEx=176.,NEy=385.> {
      parent => <-.panel;
      value => <-.NullMaskParams.in_null;
      width => (parent.width / 2);
      x => (<-.innulllabel.x + <-.innulllabel.width);
      y => innulllabel.y;
    };

    UItoggle usernull <NEx=495.,NEy=165.> {
      parent => <-.panel;
      set <NEportLevels={2,2}> => <-.NullMaskParams.user_null;
      width => parent.width;
      y => 120;
      label = "Use User-Defined Null";
    };

    UItoggle invert <NEx=495.,NEy=253.> {
      parent => <-.panel;
      set <NEportLevels={2,2}> => <-.NullMaskParams.invert;
      width => parent.width;
      y => 150;
      label = "Invert Output";
    };

    UItoggle setnull <NEx=495.,NEy=341.> {
      parent => <-.panel;
      set <NEportLevels={2,2}> => <-.NullMaskParams.set_null;
      width => parent.width;
      y => 180;
      label = "Set Null in Output";
    };
  };

  // Functional Macro

  macro NullMask {
    link in_fld <NEportLevels={2,1},NEx=110.,NEy=22.>;

    XP_COMM_PROJ.NullMask.NullMaskMods.NullMaskParams
        &NullMaskParams <NEx=66.,NEy=77.,NEportLevels={2,1}>;

    XP_COMM_PROJ.NullMask.NullMaskMods.NullMaskNodeData
        NullMaskNodeData <NEx=297.,NEy=154.> {
      NullMaskParams => <-.NullMaskParams;
      in => <-.in_fld;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data <NEx=253.,NEy=220.> {
      in_mesh => <-.in_fld;
      in_nd => <-.NullMaskNodeData.out;
    };

    link out_fld <NEportLevels={1,2},NEx=341.,NEy=363.>
        => .combine_mesh_data.out;

    link out_obj <NEportLevels={1,2},NEx=572.,NEy=319.>
        => .combine_mesh_data.obj;
  };
  
  // 
  //   User Macro (and UI)

  macro null_mask {

    link in_fld <NEportLevels={2,1},NEx=55.,NEy=99.>;

    XP_COMM_PROJ.NullMask.NullMaskMods.NullMaskParams
        NullMaskParams <NEx=55.,NEy=44.> {
      out_valid = 0.;
      out_null = 255.;
      in_null = 0.;
      user_null = 0;
      invert = 0;
      set_null = 1;
    };
   
    XP_COMM_PROJ.NullMask.NullMaskMacs.NullMaskUI NullMaskUI <NEx=275.,NEy=99.> {
      panel {
         parent<NEportLevels={4,0}>;
         title => name_of(<-.<-.<-);
      };
      NullMaskParams => <-.NullMaskParams;
    };

    XP_COMM_PROJ.NullMask.NullMaskMacs.NullMask NullMask <NEx=209.,NEy=154.> {
      in_fld => <-.in_fld;
      NullMaskParams => <-.NullMaskParams;
    };

    link out_fld<NEportLevels={1,2},NEx=308.,NEy=242.> => .NullMask.out_fld;
    link out_obj<NEportLevels={1,2},NEx=341.,NEy=198.> => .NullMask.out_obj;
  };

  // NullMaskEg
  //   Example Application (SingleWindowApp)

  APPS.MultiWindowApp NullMaskEg<NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 0;
        y = 0;
      };
      Modules {
        IUI {
          optionList {
            selectedItem = 5;
          };
          mod_panel {
            x = 0;
            y = 0;
          };
        };
      };
    };

    GDM.Uviewer3D Uviewer3D<NEx=603,NEy=453> {
      Scene {
        Top {
          child_objs => {<-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice.out_obj};
          Xform {
            ocenter = {31.5,31.5,31.5};
            dcenter = {4.,4.,4.};
          };
        };
        View {
          View {
            trigger = 2;
          };
          ViewUI {
            ViewPanel {
              UI {
                panel {
                  defaultWidth = 324;
                  defaultHeight = 320;
                  defaultX = 226;
                  defaultY = 601;
                };
              };
            };
          };
        };
      };
      Scene_Selector {
        input_views => {<-.Scene.View.View,
            <-.Scene#1.View.View,<-.Scene#2.View.View};
      };
      Scene_Editor {
        View_Editor {
          IUI {
            optionList {
              selectedItem = 3;
            };
            General {
              IUI {
                Renderer {
                  OPcmdList = {,{set=1,,,},,,};
                };
                Timer {
                  y = 0;
                };
              };
            };
            Output {
              IUI {
                OutFBType {
                  OPcmdList = {,{set=1,,,},,};
                };
                OutZBType {
                  OPcmdList = {,,{set=1,,,},,};
                };
              };
            };
            Create {
              IUI {
                panel {
                  visible = 1;
                };
                Camera_Mode {
                  OPcmdList = {{set=1,,,},,};
                };
                Scene_Mode {
                  OPcmdList = {{set=1,,,},};
                };
              };
            };
          };
        };
        Light_Editor {
          GDlinfo_edit {
            sel_light => Scene#2.Lights.Lights[0];
          };
        };
      };
      GDM.Uscene3D Scene#1 {
        Top {
          child_objs<NEportLevels={4,1}> => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.null_mask.out_obj};
          Xform {
            ocenter = {31.5,31.5,31.5};
            dcenter = {4.,4.,4.};
          };
        };
        View {
          View {
            trigger = 2;
          };
          ViewUI {
            ViewPanel {
              UI {
                panel {
                  defaultWidth = 325;
                  defaultHeight = 319;
                  defaultX = 578;
                  defaultY = 601;
                };
              };
            };
          };
        };
      };
      GDM.Uscene3D Scene#2 {
        Top {
          child_objs<NEportLevels={4,1}> => {
              <-.<-.<-.bounds.out_obj,<-.<-.<-.null_mask_no_null.out_obj};
          Xform {
            ocenter = {31.5,31.5,31.5};
            dcenter = {4.,4.,4.};
          };
        };
        View {
          View {
            trigger = 2;
          };
          ViewUI {
            ViewPanel {
              UI {
                panel {
                  defaultWidth = 324;
                  defaultHeight = 320;
                  defaultX = 921;
                  defaultY = 601;
                };
              };
            };
          };
        };
      };
    };
   MODS.Read_Field Read_Field<NEx=22.,NEy=33.> {
     read_field_ui {
       file_browser {
         x = 490;
         y = 267;
         width = 300;
         height = 390;
         ok = 1;
         dirMaskCache = "$XP_ROOT/data/field/*.fld";
       };
       filename = "$XP_ROOT/data/field/hydrogen.fld";
     };
     DVread_field {
       Mesh_Unif+Node_Data Output_Field;
     };
   };
   MODS.set_null set_null<NEx=22.,NEy=88.> {
     in_field => <-.Read_Field.field;
   };
   MODS.orthoslice orthoslice<NEx=22.,NEy=143.> {
     in_field => <-.set_null.out_fld;
     OrthoSliceParam {
       axis = 2;
     };
   };
   MODS.bounds bounds<NEx=220.,NEy=88.> {
     in_field => <-.Read_Field.field;
   };
   XP_COMM_PROJ.NullMask.NullMaskMacs.null_mask null_mask<NEx=33.,NEy=275.> {
     in_fld => <-.orthoslice.out_fld;
   };
   XP_COMM_PROJ.NullMask.NullMaskMacs.null_mask
       null_mask_no_null<NEx=209.,NEy=275.> {
     in_fld => <-.orthoslice.out_fld;
     NullMaskParams {
       invert = 1;
       set_null = 0;
     };
     NullMaskUI {
       panel {
         option {
           set = 1;
         };
       };
     };
   };
 };
};




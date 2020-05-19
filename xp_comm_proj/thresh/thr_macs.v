
flibrary ThresholdMacs {

  // ThresholdUI
  //   UI Macro
  //   UImod_panel containing a label and two dials

  macro ThresholdUI {
    XP_COMM_PROJ.Threshold.ThresholdMods.ThresholdParams
      &ThresholdParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel UImod_panel <NEx=330.,NEy=20.> {
      title = "Threshold";
    };

    UIdial UIdial_min <NEx=440.,NEy=120.> {
      parent => <-.UImod_panel;
      value => <-.ThresholdParams.min;
      title = "Minimum";
      y = 50;
      width = 100;
    };

    UIdial UIdial_max <NEx=440.,NEy=170.> {
      parent => <-.UImod_panel;
      value => <-.ThresholdParams.max;
      title = "Maximum";
      y = 50;
      x = 120;
      width = 100;
    };

    Controls.UIlabel UIlabel <NEx=440.,NEy=70.> {
      parent => <-.UImod_panel;
      label = "Threshold Module controls:";
      y = 10;
      width = 200;
    };
  };

  // Threshold
  //   Functional macro (Field I/O)

  macro Threshold {
    link in <NEportLevels={2,1},NEx=100.,NEy=50.>;

    XP_COMM_PROJ.Threshold.ThresholdMods.ThresholdParams
        &ThresholdParams <NEportLevels={2,1},NEx=300.,NEy=50.>;

    XP_COMM_PROJ.Threshold.ThresholdMods.ThresholdNodeData
        ThresholdNodeData <NEx=100.,NEy=250.> {
      in => <-.in;
      ThresholdParams => <-.ThresholdParams;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data <NEx=200.,NEy=342.> {
      in_mesh => <-.in;
      in_nd => <-.ThresholdNodeData.out;
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=420.>
        => combine_mesh_data.out;
  };

  // threshold
  //   User Macro (Field I/O and ThresholdUI)

  macro threshold {
    link in <NEportLevels={2,1},NEx=100.,NEy=50.>;

    XP_COMM_PROJ.Threshold.ThresholdMods.ThresholdParams
        ThresholdParams <NEx=300.,NEy=50.>;

    XP_COMM_PROJ.Threshold.ThresholdMacs.Threshold Threshold <NEx=100.,NEy=250.> {
      &ThresholdParams => <-.ThresholdParams;
      in => <-.in;
    };

    XP_COMM_PROJ.Threshold.ThresholdMacs.ThresholdUI
        ThresholdUI <NEx=300.,NEy=250.> {
      &ThresholdParams => <-.ThresholdParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=400.> => Threshold.out;
  };

  // ThresholdEg
  //   Example Application (SingleWindowApp)

  APPS.SingleWindowApp ThresholdEg <NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 61;
        y = 258;
      };

      Modules {
        IUI {
          optionList {
            selectedItem = 0;
          };
        };
      };
    };

    GDM.Uviewer3D Uviewer3D <NEx=253.,NEy=297.> {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.orthoslice.out_obj,<-.<-.<-.bounds.out_obj
          };
          Xform {
            mat = {
              -0.0280839,0.0715985,-0.0496336,0.,0.00878093,-0.0495567,
              -0.0764564,0.,-0.086676,-0.0282192,0.0083361,0.,0.,0.,0.,1.
            };
            xlate = {-4.,-4.,-4.};
          };
        };

        Camera {
          Camera {
            perspec = 1;
            front = 1.;
          };
        };
  
        View {
          View {
            trigger = 1;
          };
        };
      };
  
      Scene_Editor {
        Camera_Editor {
          GDcamera_edit {
            front = 1.;
          };
        };
      };
    };

    XP_COMM_PROJ.Threshold.ThresholdMacs.threshold threshold <NEx=77.,NEy=154.> {
      in => <-.downsize.out_fld;

      ThresholdParams {
        min = 50.;
        max = 150.;
      };

      ThresholdUI {
        UImod_panel {
          option {
            set = 1;
          };
        };
      };
    };

    MODS.Read_Field Read_Field <NEx=77.,NEy=44.> {
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

    MODS.downsize downsize <NEx=77.,NEy=99.> {
      in_field => <-.Read_Field.field;
  
      DownsizeParam {
        factor0 = 2.;
        factor1 = 2.;
        factor2 = 2.;
      };
    };

    MODS.bounds bounds <NEx=253.,NEy=99.> {
      in_field => <-.Read_Field.field;

      BoundsUI {
        DVnode_data_labels {
          labels[];
        };

        UIradioBoxLabel {
          label_cmd {
            cmd[];
          };
        };
      };
    };

    MODS.orthoslice orthoslice <NEx=77.,NEy=220.> {
      in_field => <-.threshold.out;

      OrthoSliceParam {
        axis = 1;
        plane => 15;
      };
    };
  };
};




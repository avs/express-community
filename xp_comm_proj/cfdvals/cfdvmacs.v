
flibrary CFDValsMacs {

  // CFDValsUI
  //   UI Macro

  macro CFDValsUI {
    XP_COMM_PROJ.CFDVals.CFDValsMods.CFDValsParams
      &CFDValsParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel panel <NEx=330.,NEy=20.> {
      title = "CFDVals";
      message = "Select CFD Values control panel.";
      parent <NEportLevels={4,0}>;
    };

    UIlabel CFD_ValuesTitle {
      parent => <-.panel;
      y = 0;
      width => 240;
      label = "CFD Derived Values";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UIlabel gamma_label {
      parent => <-.panel;
      y => <-.CFD_ValuesTitle.y + <-.CFD_ValuesTitle.height + 5;
      x = 0;
      width = 80;
      height = 30;
      label = "Gamma";
    };

    UIfield gamma_value {
      parent => <-.panel;
      y => <-.CFD_ValuesTitle.y + <-.CFD_ValuesTitle.height + 5;
      x = 90;
      height = 30;
      value => <-.CFDValsParams.gamma;
      width = 130;
    };

    UIlabel gas_label {
      parent => <-.panel;
      y => <-.gamma_label.y + <-.gamma_label.height + 5;
      x = 0;
      height = 30;
      width = 80;
      label = "Gas Constant";
    };

    UIfield gas_value {
      parent => <-.panel;
      y => <-.gamma_label.y + <-.gamma_label.height + 5;
      height = 30;
      x = 90;
      value => <-.CFDValsParams.gas_constant;
      width = 130;
    };

    UItoggle energy {
      parent => <-.panel;
      y => <-.gas_label.y + <-.gas_label.height + 5;
      width = 150;
      label = "Energy";
      set => <-.CFDValsParams.do_energy;
    };

    UItoggle pres {
      parent => <-.panel;
      y => <-.energy.y + <-.energy.height + 5;
      width = 150;
      label = "Pressure";
      set => <-.CFDValsParams.do_pres;
    };

    UItoggle enthalpy {
      parent => <-.panel;
      y => <-.pres.y + <-.pres.height + 5;
      width = 150;
      label = "Enthalpy";
      set => <-.CFDValsParams.do_enthalpy;
    };

    UItoggle mach {
      parent => <-.panel;
      y => <-.enthalpy.y + <-.enthalpy.height + 5;
      width = 150;
      label = "Mach Number";
      set => <-.CFDValsParams.do_mach;
    };

    UItoggle temp {
      parent => <-.panel;
      y => <-.mach.y + <-.mach.height + 5;
      width = 150;
      label = "Temperature";
      set => <-.CFDValsParams.do_temp;
    };

    UItoggle totpres {
      parent => <-.panel;
      y => <-.temp.y + <-.temp.height + 5;
      width = 150;
      label = "Total Pressure";
      set => <-.CFDValsParams.do_totpres;
    };

    UItoggle tottemp {
      parent => <-.panel;
      y => <-.totpres.y + <-.totpres.height + 5;
      width = 150;
      label = "Total Temperature";
      set => <-.CFDValsParams.do_tottemp;
    };
  };

  // CFDVals
  //   Functional macro (Field I/O)

  macro CFDVals {

    XP_COMM_PROJ.CFDVals.CFDValsMods.CFDValsParams
        &CFDValsParams <NEportLevels={2,1},NEx=300.,NEy=50.>;

    XP_COMM_PROJ.CFDVals.CFDValsMods.CFDValsNodeData
        CFDValsNodeData <NEx=100.,NEy=250.> {
      &in<NEportLevels={3,0}>;
      &CFDValsParams => <-.CFDValsParams;
    };

  mlink out <NEportLevels={1,2},NEx=100.,NEy=420.> => CFDValsNodeData.fields;

  };

  // 
  //   User Macro (and UI)

  macro cfd_values {

    XP_COMM_PROJ.CFDVals.CFDValsMods.CFDValsParams
        CFDValsParams <NEx=300.,NEy=50.> {
      gamma = 1.4;
      gas_constant = 1.0;
      do_energy = 1;
      do_pres = 0;
      do_enthalpy = 0;
      do_mach = 0;
      do_temp = 0;
      do_totpres = 0;
      do_tottemp = 0;
    };

    XP_COMM_PROJ.CFDVals.CFDValsMacs.CFDVals CFDVals <NEx=100.,NEy=250.> {
      &CFDValsParams => <-.CFDValsParams;
      CFDValsNodeData {
        &in<NEportLevels={4,0}>;
      };
    };

    XP_COMM_PROJ.CFDVals.CFDValsMacs.CFDValsUI
        CFDValsUI <NEx=300.,NEy=250.> {
      &CFDValsParams => <-.CFDValsParams;
      panel {
        title => name_of(<-.<-.<-);
      };
    };

    mlink out <NEportLevels={1,2},NEx=100.,NEy=400.> => CFDVals.out;
  };

  // CFDValsEg
  //   Example Application (SingleWindowApp)

  APPS.SingleWindowApp CFDValsEg <NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 51;
        y = 252;
      };
      Modules {
        IUI {
          optionList {
            cmdList => {
              <-.<-.<-.<-.read_plot3d.ReadPlot3dUI.panel.option,
              <-.<-.<-.<-.cfd_values.CFDValsUI.panel.option,
              <-.<-.<-.<-.orthoslice_ARR.OrthoSliceUI.UIpanel.option,
              <-.<-.<-.<-.bounds_ARR.UIpanel.option
            };
            selectedItem = 0;
          };
        };
      };
    };
    GDM.Uviewer3D Uviewer3D<NEx=429.,NEy=352.> {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.orthoslice_ARR.out_obj,<-.<-.<-.bounds_ARR.out_obj};
            Xform {
              mat = {
                0.711562,12.2744,5.05639,0.,
                10.0468,-3.80763,7.8293,0.,
                8.67696,3.40216,-9.47994,0.,
                0.,0.,0.,1.
              };
              xlate = {109.571,97.0259,27.0131};
              center = {0.1524,0.0762,0.3048};
              ocenter = {0.1524,0.0762,0.3048};
              dcenter = {-109.114,-96.4494,-27.0131};
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
      XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMacs.read_plot3d
          read_plot3d<NEx=22.,NEy=22.> {
        ReadPlot3dParams {
          filename_xyz = "$XP_ROOT/data/plot3d/multb_x.dat";
          filename_q = "$XP_ROOT/data/plot3d/multb_q.dat";
          format = 0;
	  trigger = 1;
        };
        ReadPlot3dUI {
          panel {
            option {
              set = 1;
            };
          };
          xyz_file_browser {
            x = 10;
            y = 10;
            width = 300;
            height = 390;
            ok = 1;
            dirMaskCache = "$XP_ROOT/data/plot3d/*.dat";
          };
          q_file_browser {
            width = 300;
            height = 390;
            x = 20;
            y = 20;
            ok = 1;
            dirMaskCache = "$XP_ROOT/data/plot3d/*.dat";
          };
          Format {
            OPcmdList = { {set=1,,,},,};
          };
        };
      };
      XP_COMM_PROJ.CFDVals.CFDValsMacs.cfd_values
          cfd_values <NEx=99.,NEy=110.> {
        CFDValsParams {
        do_energy = 0;
        do_temp = 1;
      };
      CFDVals {
        CFDValsNodeData<NEx=88.,NEy=253.> {
          in => <-.<-.<-.read_plot3d.fields;
        };
      };
      out<NEx=165.,NEy=352.>;
    };
    MODS_ARR.orthoslice_ARR orthoslice_ARR<NEx=231.,NEy=198.> {
      in_field => <-.cfd_values.out;
      OrthoSliceParam {
        plane => 5;
      };
    };
    MODS_ARR.bounds_ARR bounds_ARR<NEx=407.,NEy=198.> {
      in_field => <-.cfd_values.out;
      BoundsParam {
        edges = 1;
        data = 1;
      };
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
  };
};





flibrary ReadPlot3dMacs {

  // ReadPlot3dUI
  //   UI Macro

  macro ReadPlot3dUI {
    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMods.ReadPlot3dParams
      &ReadPlot3dParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel panel <NEx=330.,NEy=20.> {
      title = "ReadPlot3d";
      message = "Select read plot3D control panel.";
    };

    UIlabel Plot3dTitle {
      parent => <-.panel;
      y = 0;
      width => 240;
      label = "Read PLOT3D";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UIlabel XYZ_Filename {
      parent => <-.panel;
      y => <-.Plot3dTitle.height + 5;
      width => 200;
      alignment = 0;
      label = "XYZ (Grid) Filename:";
    };

    UItext XYZ_file_name {
      parent => <-.panel;
      y => XYZ_Filename.y + XYZ_Filename.height + 5;
      text => <-.ReadPlot3dParams.filename_xyz;
      width = 170;
      showLastPosition = 1;
    };

    UIbutton XYZ_visible {
      parent => <-.panel;
      x => XYZ_file_name.x + XYZ_file_name.width + 5;
      y => XYZ_file_name.y;
      width = 65;
      height => <-.XYZ_file_name.height;
      label = "Browse...";
    };

    UIfileSB xyz_file_browser {
      GMOD.copy_on_change copy_on_change {
        trigger => <-.<-.XYZ_visible.do; 
        input => <-.<-.XYZ_visible.do;
        output => <-.visible;
      };
      title = "Plot3d XYZ Filename";
      searchPattern = "$XP_PATH<0>/data/plot3d/*.dat";
      &filename => <-.ReadPlot3dParams.filename_xyz;
    };

    UIlabel Q_Filename {
      parent => <-.panel;
      y => XYZ_visible.height + XYZ_visible.y + 10;
      width => 200;
      alignment = 0;
      label = "Q (Results) Filename:";
    };

    UItext Q_file_name {
      parent => <-.panel;
      y => Q_Filename.y + Q_Filename.height + 5;
      text => <-.<-.filename_q;
      text => <-.ReadPlot3dParams.filename_q;
      width = 170;
      showLastPosition = 1;
    };

    UIbutton Q_visible {
      parent => <-.panel;
      x => Q_file_name.x + Q_file_name.width + 5;
      y => Q_file_name.y;
      width = 65;
      height => <-.Q_file_name.height;
      label = "Browse...";
    };

    UIfileSB q_file_browser {
      GMOD.copy_on_change copy_on_change {
        trigger => <-.<-.Q_visible.do; 
        input => <-.<-.Q_visible.do;
        output => <-.visible;
      };
      x = 0;
      y = 100;
      title = "Plot3d Q Filename";
      searchPattern = "$XP_PATH<0>/data/plot3d/*.dat";
      &filename => <-.ReadPlot3dParams.filename_q;
    };

    UItoggle IBlanks {
      parent => <-.panel;
      y => Q_visible.y + Q_visible.height + 5;
      width = 120;
      height => <-.Q_visible.height;
      label = "IBlanks";
      set => <-.ReadPlot3dParams.iblanks;
    };

    UItoggle UseQ {
      parent => <-.panel;
      y => Q_visible.y + Q_visible.height + 5;
      x = 125;
      width = 120;
      height => <-.Q_visible.height;
      label = "Use Q file data";
      set => <-.ReadPlot3dParams.use_q;
    };

    UItoggle Dims3d {
      parent => <-.panel;
      y => UseQ.y + UseQ.height + 5;
      width = 120;
      height => <-.UseQ.height;
      label = "3D";
      set => <-.ReadPlot3dParams.dims3d;
    };

    UItoggle Multiblock {
      parent => <-.panel;
      y => UseQ.y + UseQ.height + 5;
      width = 120;
      height => <-.UseQ.height;
      x = 125;
      label = "Multiblock";
      set => <-.ReadPlot3dParams.multiblock;
    };

    VUIOptionMenuLabel Format {
      parent => <-.panel;
      y => <-.Dims3d.y + <-.Dims3d.height + 5;
      width = 240;
      options => { "ASCII Formatted",
                   "Binary", 
                   "Binary w/padding" };
      label = "File Format";
      selectedItem => <-.ReadPlot3dParams.format;
    };

    UIbutton ReadTrigger {
      parent => <-.panel;
      y => <-.Format.y + <-.Format.height + 5;
      width = 120;
      x = 0;
      height => <-.Q_visible.height;
      label = "Read";
      do => <-.ReadPlot3dParams.trigger;
    };

    UIbutton ClearTrigger {
      parent => <-.panel;
      y => <-.Format.y + <-.Format.height + 5;
      width = 120;
      x = 120;
      height => <-.Q_visible.height;
      label = "Clear";
      GMOD.parse_v ParseX {
        v_commands = "filename_xyz=;
        filename_q=;";
        trigger => <-.do;
        on_inst = 0;
        relative => <-.<-.ReadPlot3dParams;
      };
    };
  };

  // ReadPlot3d
  //   Functional macro (Field I/O)

  macro ReadPlot3d {
    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMods.ReadPlot3dParams
      &ReadPlot3dParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMods.ReadPlot3dFile
        ReadPlot3dFile <NEx=100.,NEy=250.> {
      ReadPlot3dParams => <-.ReadPlot3dParams;
    };

    link out <NEportLevels={1,2},export_all=2> => ReadPlot3dFile.out;
    mlink fields <NEportLevels={1,2},export_all=2> => ReadPlot3dFile.fields;
  };

  // read_plot3d
  //   User Macro (and UI)

  macro read_plot3d {
    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMods.ReadPlot3dParams
        ReadPlot3dParams <NEx=300.,NEy=50.> {
      iblanks = 1;
      format = 1;
      use_q = 1;
      trigger = 0;
      dims3d = 1;
      multiblock = 1;
    };

    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMacs.ReadPlot3d
        ReadPlot3d <NEx=100.,NEy=250.> {
      &ReadPlot3dParams => <-.ReadPlot3dParams;
    };

    XP_COMM_PROJ.ReadPlot3d.ReadPlot3dMacs.ReadPlot3dUI
        ReadPlot3dUI <NEx=300.,NEy=250.> {
      &ReadPlot3dParams => <-.ReadPlot3dParams;
      panel {
        title => name_of(<-.<-.<-);
      };
    };

    link out <NEportLevels={1,2},export_all=2> => ReadPlot3d.out;
    mlink fields <NEportLevels={1,2},export_all=2> => ReadPlot3d.fields;
  };

  // ReadPlot3dEg
  //   Example Application (SingleWindowApp)

  APPS.SingleWindowApp ReadPlot3dEg <NEdisplayMode="maximized"> {
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
              <-.<-.<-.<-.bounds.UIpanel.option,
              <-.<-.<-.<-.orthoslice.OrthoSliceUI.UIpanel.option
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
            <-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice.out_obj
          };
          Xform {
            ocenter = {3.27323,4.16378,2.86213};
            dcenter = {1.18071,1.50195,1.03242};
            mat = {
              0.250671,0.143757,-0.292323,0.,
              0.00203375,0.368155,0.182793,0.,
              0.325753,-0.112922,0.223806,0.,
              0.,0.,0.,1.
            };
            xlate = {-1.18071,-1.50195,-1.03242};
            center = {3.27323,4.16378,2.86213};
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
        read_plot3d <NEx=33.,NEy=22.> {
      ReadPlot3dParams {
        filename_xyz = "$XP_PATH<0>/data/plot3d/blntfinx.bin";
        filename_q = "$XP_PATH<0>/data/plot3d/blntfinq.bin";
        iblanks = 0;
        multiblock = 0;
      };
      ReadPlot3dUI {
        panel {
          option {
            set = 1;
          };
        };
        xyz_file_browser {
          x = 490;
          y = 267;
          width = 300;
          height = 390;
          ok = 1;
          dirMaskCache = "$XP_PATH<0>/data/plot3d/*";
        };
        q_file_browser {
          width = 300;
          height = 390;
          x = 8;
          ok = 1;
          dirMaskCache = "$XP_PATH<0>/data/plot3d/*";
        };
        Format {
          OPcmdList = {,{set=1,,,},};
        };
      };
    };

    link link<NEportLevels=1,NEx=99.,NEy=66.> => .read_plot3d.fields[0];

    MODS.bounds bounds <NEx=429.,NEy=154.> {
      in_field => <-.link;
      BoundsParam {
        edges = 1;
        imin = 1;
        imax = 1;
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

    MODS.orthoslice orthoslice <NEx=264.,NEy=154.> {
      in_field => <-.link;
      OrthoSliceParam {
        axis = 2;
        plane => 26;
      };
    };
  };
};





// replace Flag with general name
// replace FlagRaw with low-level module name

flibrary FlagMacs {

  // FlagUI
  //   UI Macro

  macro FlagUI {
    
    XP_COMM_PROJ.Flag.FlagMods.FlagLoopParams &FlagLoopParams<NEx=242.,NEy=44.,NEportLevels={2,1}>;

    XP_COMM_PROJ.Flag.FlagMods.FlagParams &FlagParams<NEx=429.,NEy=22.,NEportLevels={2,1}>;

    UImod_panel panel <NEx=11.,NEy=121.> {
      title = "Flag";
    };

    // loop frame

    UIframe loopframe<NEx=121.,NEy=165.> {
      parent => <-.panel;
      x = 4;
      y = 0;
      width => parent.width - 8;
      height => <-.toggle_step.y + <-.toggle_step.height + 5;
    };

    UIlabel label_params<NEx=242.,NEy=220.> {
      parent => <-.loopframe;
      label => "Loop Controls";
      x = 0;
      y = 0;
      width => parent.width;
      color {
         backgroundColor = "blue";
         foregroundColor = "white";
      };
    };

    UItoggle toggle_run<NEx=242.,NEy=264.> {
      parent => <-.loopframe;
      set+Port2 => <-.FlagLoopParams.run;
      label = "run/pause";
      x = 0;
      y => <-.label_params.y + <-.label_params.height + 5;
      width => (parent.width / 2) - 2;
    };

    UItoggle toggle_step<NEx=242.,NEy=308.> {
      parent => <-.loopframe;
      set+Port2 => <-.FlagLoopParams.step;
      label = "step";
      x => (parent.width / 2) + 2;
      y => <-.toggle_run.y;
      width => (parent.width / 2) - 2;
    };


    // flag control frame

    UIframe flagframe<NEx=297.,NEy=165.> {
      parent => <-.panel;
      x = 4;
      y => <-.loopframe.y + <-.loopframe.height + 10;
      width => parent.width - 8;
      height => <-.colorframe.y + <-.colorframe.height + 5;
    };

    UIlabel label_flag<NEx=407.,NEy=220.> {
       parent => <-.flagframe;
       label => "Flag Controls";
       x = 0;
       y = 0;
       width => parent.width;
       color {
          backgroundColor = "blue";
          foregroundColor = "white";
       };
    };

    UIslider UIsliderwind1<NEx=407.,NEy=264.> {
       parent => <-.flagframe;
       value+Port2 => <-.FlagParams.flag_wind_1;
       title = "Wind Vector 1";
       x = 2;
       y => <-.label_flag.y + <-.label_flag.height + 10;
       width => parent.width - 10;
       min = 0;
       max = 1;
    };

    UIslider UIsliderwind2<NEx=407.,NEy=308.> {
       parent => <-.flagframe;
       value+Port2 => <-.FlagParams.flag_wind_2;
       title => "Wind Vector 2";
       x = 2;
       y => <-.UIsliderwind1.y + <-.UIsliderwind1.height + 5;
       width => parent.width - 10;
       min = 0;
       max = 1;
    };

    UIslider UIsliderstrength<NEx=407.,NEy=352.> {
       parent => <-.flagframe;
       value+Port2 => <-.FlagParams.flag_strength;
       title => "Material Strength";
       x = 2;
       y => <-.UIsliderwind2.y + <-.UIsliderwind2.height + 5;
       width => parent.width - 10;

       //
       // watch out here, code will crash if out of range!
       //

       min = 6.0;
       max = 30.0;
       // value = 20; // default starting value
    };

    UItoggle Reset_Toggle<NEx=407.,NEy=396.> {
       parent => <-.flagframe;
       label => "Flag Reset";
       set+Port2 => <-.FlagParams.reset;
       x = 2;
       y => <-.UIsliderstrength.y + <-.UIsliderstrength.height + 15;
       width => parent.width - 10;
    };

    UItoggle RelTopToggle<NEx=407.,NEy=440.> {
       parent => <-.flagframe;
       label => "Release Top";
       set+Port2 => <-.FlagParams.release_top;
       x = 2;
       y => <-.Reset_Toggle.y + <-.Reset_Toggle.height + 5;
       width => parent.width - 10;
    };

    UItoggle RelBotToggle<NEx=407.,NEy=484.> {
       parent => <-.flagframe;
       label => "Release Bottom";
       set+Port2 => <-.FlagParams.release_bottom;
       x = 2;
       y => <-.RelTopToggle.y + <-.RelTopToggle.height + 5;
       width => parent.width - 10;
    };

    UIframe colorframe<NEx=561.,NEy=220.> {
       parent => <-.flagframe;
       x = 2;
       y => <-.RelBotToggle.y + <-.RelBotToggle.height + 15;
       width => parent.width - 10;
       height => <-.ColorRadioBox.y + <-.ColorRadioBox.height + 5;
    };

    UIoption UIoption<NEx=660.,NEy=22.> {
      label = "Solid Color";
    };
    UIoption UIoption#1<NEx=660.,NEy=55.> {
      label = "Velocity TriColor";
    };
    UIoption UIoption#2<NEx=660.,NEy=88.> {
      label = "Force TriColor";
    };
    UIoption UIoption#3<NEx=660.,NEy=121.> {
      label = "Force Magnitude";
    };
    UIoption UIoption#4<NEx=660.,NEy=154.> {
      label = "Texture Map";
    };

    UIradioBox ColorRadioBox<NEx=682.,NEy=275.> {
      parent => <-.colorframe;
      cmdList => {
        <-.UIoption,
        <-.UIoption#1,
        <-.UIoption#2,
        <-.UIoption#3,
        <-.UIoption#4
      };
      selectedItem+Port2 => <-.FlagParams.flag_color;
      x = 2;
      y = 0;
      width => parent.width - 4;
    };
  };



  // Flag
  //   Functional macro (Field I/O)

  macro Flag {

    XP_COMM_PROJ.Flag.FlagMods.FlagParams &FlagParams<NEportLevels={2,1}>;

    XP_COMM_PROJ.Flag.FlagMods.FlagLoopParams &FlagLoopParams<NEportLevels={2,1}>;

    XP_COMM_PROJ.Flag.FlagMods.FlagRaw FlagRaw {
      FlagParams => <-.FlagParams;
    };

    XP_COMM_PROJ.Flag.FlagMods.FlagLoop FlagLoop {
      FlagLoopParams => <-.FlagLoopParams;
      count+Port2 => <-.FlagParams.count;
    };

    GDM.DataObject DataObject {
      in => <-.FlagRaw.out;
      texture_in<NEportLevels={3,1}>;
      Obj {
        name => name_of(<-.<-.<-);
      };
    };

    olink out_fld => .FlagRaw.out;
    olink out_obj => .DataObject.obj;
  };


  // flag
  //   User Macro

  macro flag {
    ilink texture;

    XP_COMM_PROJ.Flag.FlagMods.FlagParams FlagParams {
      count = 0;
      reset = 0;
      flag_color = 0;
      release_top = 0;
      release_bottom = 0;
      flag_wind_1 = 0.5;
      flag_wind_2 = 0.1;
      flag_strength = 20.;
    };

    XP_COMM_PROJ.Flag.FlagMods.FlagLoopParams FlagLoopParams {
      run = 0;
      step = 0;
    };
 
    XP_COMM_PROJ.Flag.FlagMacs.FlagUI FlagUI {
      FlagLoopParams => <-.FlagLoopParams;
      FlagParams => <-.FlagParams;
    };

    XP_COMM_PROJ.Flag.FlagMacs.Flag Flag {
      FlagParams => <-.FlagParams;
      FlagLoopParams => <-.FlagLoopParams;
      DataObject {
         texture_in => <-.<-.texture;
         Obj {
            name => name_of(<-.<-.<-.<-);
         };
      };
    };

    olink out_fld => .Flag.out_fld;
    olink out_obj => .Flag.out_obj;
  };



  // FlagEg
  //   Example Application

  APPS.SingleWindowApp FlagEg {
    Scheduler.status_check = 0;

    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {<-.<-.<-.flag.out_obj};
          Xform {
            ocenter = {9.49752,5.47979,0.00913261};
            dcenter = {3.99791,2.30668,0.00384431};
            mat = {
              0.420943,0.,0.,0.,
              0.,0.420943,0.,0.,
              0.,0.,0.420943,0.,
              0.,0.,0.,1.
            };
            xlate = {-3.99791,-2.30668,-0.00384431};
            center = {9.49752,5.47979,0.00913261};
          };
        };
        View {
          View {
            trigger = 1;
          };
        };
        Camera {
          Camera {
            auto_norm = "None";
          };
        };
      };
    };

    MODS.Read_Image Read_Image {
      read_image_ui {
        file_browser {
          filename = "$XP_PATH<1>/xp_comm_proj/flag/iac.gif";
          dirMaskCache = "$XP_PATH<1>/xp_comm_proj/flag/*";
        };
      };
    };

    XP_COMM_PROJ.Flag.FlagMacs.flag flag {
      FlagParams {
        flag_color = 4;
      };
      texture => <-.Read_Image.field;
    };
  };

};


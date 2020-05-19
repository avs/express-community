
flibrary AnimFilenameMacs {

  // AnimFilenameUI
  //   UI Macro
  //   Components for parameters of the AnimFilenameGen and AnimFilenameLoop
  //   modules.

  macro AnimFilenameUI {
    UImod_panel UImod_panel<NEx=341.,NEy=55.> {
      title = "AnimFilename";
    };

    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimFilenameParams 
        &AnimFilenameParams<NEx=99.,NEy=55.,NEportLevels={2,1}>;

    UIlabel label_pre <NEx=11.,NEy=165.> {
      y = 0;
      width = 250;
      parent => <-.UImod_panel;
      label => "Pre-number text";
      alignment = 0;
    };

    UItext entry_pre<NEx=22.,NEy=209.> {
      y = 24;
      width = 250;
      height = 40;
      parent => <-.UImod_panel;
      updateMode = 7;
      text<NEportLevels={2,2}> => <-.AnimFilenameParams.pre;
    };

    UIlabel label_post<NEx=165.,NEy=165.> {
      y = 64;
      parent => <-.UImod_panel;
      label => "Post-number text";
      width = 250;
      alignment = 0;
    };

    UItext entry_post<NEx=176.,NEy=209.> {
      y = 88;
      width = 250;
      height = 40;
      parent => <-.UImod_panel;
      updateMode = 7;
      text<NEportLevels={2,2}> => <-.AnimFilenameParams.post;
    };

    UIslider slider_precision<NEx=319.,NEy=165.> {
      y = 128;
      width = 250;
      parent => <-.UImod_panel;
      min = 1.;
      max = 10.;
      value => <-.AnimFilenameParams.precision;
      decimalPoints = 0;
      title => "File number precision";
      increment = 1.;
    };

    UIlabel label_control<NEx=473.,NEy=165.> {
      y = 188;
      parent => <-.UImod_panel;
      label => "Loop Control";
      alignment = 0;
    };

    UItoggle toggle_reset<NEx=473.,NEy=209.> {
      parent => <-.UImod_panel;
      set<NEportLevels={2,2}> => <-.AnimLoopParams.reset;
      y = 212;
      label = "reset";
    };

    UItoggle toggle_run<NEx=473.,NEy=253.> {
      parent => <-.UImod_panel;
      set<NEportLevels={2,2}> => <-.AnimLoopParams.run;
      y = 236;
      label = "run";
    };

    UItoggle toggle_cycle<NEx=473.,NEy=297.> {
      parent => <-.UImod_panel;
      set<NEportLevels={2,2}> => <-.AnimLoopParams.cycle;
      y = 260;
      label = "cycle";
    };

    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimLoopParams
        &AnimLoopParams<NEx=539.,NEy=22.,NEportLevels={2,1}>;

    UIlabel label_params<NEx=627.,NEy=165.> {
      parent => <-.UImod_panel;
      label => "Loop Parameters";
      y = 284;
    };

    UIlabel label_start<NEx=627.,NEy=209.> {
      parent => <-.UImod_panel;
      label => "start value";
      y = 308;
      alignment = 0;
    };

    UIfield entry_start<NEx=627.,NEy=253.> {
      parent => <-.UImod_panel;
      value => <-.AnimLoopParams.start_val;
      y = 308;
      x => (<-.label_start.width + 5);
      updateMode = 7;
      decimalPoints = 0;
      mode = 1;
    };

    UIlabel label_end<NEx=627.,NEy=297.> {
      parent => <-.UImod_panel;
      label => "end value";
      y = 338;
      alignment = 0;
    };

    UIfield entry_end<NEx=627.,NEy=341.> {
      parent => <-.UImod_panel;
      value => <-.AnimLoopParams.end_val;
      y = 338;
      x => (label_end.width + 5);
      updateMode = 7;
      decimalPoints = 0;
      mode = 1;
    };

    UIlabel label_incr<NEx=627.,NEy=385.> {
      parent => <-.UImod_panel;
      label => "increment";
      y = 368;
      alignment = 0;
    };

    UIfield entry_incr<NEx=627.,NEy=429.> {
      parent => <-.UImod_panel;
      value => <-.AnimLoopParams.incr;
      x => (label_incr.width + 5);
      y = 368;
      updateMode = 7;
      decimalPoints = 0;
      mode = 1;
    };
  };

  // AnimFilename
  //   Functional Macro
  //   Combines the altered loop module with the dynamic filename
  //   generation module

  macro AnimFilename {

    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimFilenameGen
        AnimFilenameGen <NEx=33.,NEy=88.> {
      AnimFilenameParams <NEportLevels={3,1}>;
      number => <-.AnimFilenameLoop.count;
      output<NEportLevels={0,3}>;
    };

    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimFilenameLoop
        AnimFilenameLoop<NEx=99.,NEy=33.> {
      AnimLoopParams<NEportLevels={3,1}>;
     };
  };

  // animfilename
  //   User Macro
  //   uses both parameter groups, the AnimFilenameGen module, and
  //   the modified GMOD.loop module

  macro animfilename {
    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimFilenameParams
        AnimFilenameParams <NEx=22.,NEy=55.>;

    XP_COMM_PROJ.AnimFilename.AnimFilenameMods.AnimLoopParams
        AnimLoopParams <NEx=22.,NEy=11.>;

    XP_COMM_PROJ.AnimFilename.AnimFilenameMacs.AnimFilenameUI
        AnimFilenameUI <NEx=286.,NEy=99.> {
      AnimFilenameParams => <-.AnimFilenameParams;
      AnimLoopParams => <-.AnimLoopParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    XP_COMM_PROJ.AnimFilename.AnimFilenameMacs.AnimFilename
        AnimFilename<NEx=198.,NEy=143.> {

      AnimFilenameGen {
        AnimFilenameParams => <-.<-.AnimFilenameParams;
      };

      AnimFilenameLoop {
         AnimLoopParams => <-.<-.AnimLoopParams;
      };
    };

    link out <NEportLevels={1,2},NEx=308.,NEy=297.> =>
        AnimFilename.AnimFilenameGen.output;

  };
};



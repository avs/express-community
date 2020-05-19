
// replace _NAME_ with general name
// replace _NAMELL_ with low-level module name

flibrary CFDRangeMacs {

  // CFDRangeUI
  //   UI Macro

  macro CFDRangeUI {
    XP_COMM_PROJ.CFDRange.CFDRangeMods.CFDRangeParams
      &CFDRangeParams <NEportLevels={2,1},NEx=187.,NEy=120.>;

    UI.UImod_panel panel <NEx=330.,NEy=20.> {
      title = "CFDRange";
      message = "Select CFD Range control panel.";
      parent<NEportLevels={4,0}>;
    };

    UIlabel CFD_ValuesTitle {
      parent => <-.panel;
      y = 0;
      width => 240;
      label = "Multiblock Range Values";
      color {
        backgroundColor = "blue";
        foregroundColor = "white";
      };
    };

    UIlabel min_label {
      parent => <-.panel;
      y => <-.CFD_ValuesTitle.y + <-.CFD_ValuesTitle.height + 5;
      x = 0;
      width = 80;
      height = 30;
      label = "Min";
    };

    UIfield min_value {
      parent => <-.panel;
      y => <-.CFD_ValuesTitle.y + <-.CFD_ValuesTitle.height + 5;
      x = 90;
      height = 30;
      value => <-.CFDRangeParams.min;
      width = 130;
    };

    UIlabel max_label {
      parent => <-.panel;
      y => <-.min_label.y + <-.min_label.height + 10;
      x = 0;
      height = 30;
      width = 80;
      label = "Max";
    };

    UIfield max_value {
      parent => <-.panel;
      y => <-.min_label.y + <-.min_label.height + 10;
      height = 30;
      x = 90;
      value => <-.CFDRangeParams.max;
      width = 130;
    };

    UItoggle lock_toggle {
      parent => <-.panel;
      y => <-.max_label.y + <-.max_label.height + 10;
      x = 0;
      width = 200;
      label = "Lock to User Range";
      set => <-.CFDRangeParams.lock_mode;
    };
  };

  macro cfd_range {
    XP_COMM_PROJ.CFDRange.CFDRangeMods.CFDRangeParams
        CFDRangeParams <NEx=300.,NEy=50.> {
      min = 0.0;
      max = 1.0;
      lock_mode = 0;
    };

    XP_COMM_PROJ.CFDRange.CFDRangeMods.CFDRangeNodeData 
        CFDRangeNodeData <NEx=100.,NEy=250.> {
      &CFDRangeParams => <-.CFDRangeParams;
      in<NEportLevels={3,0}>;
    };

    XP_COMM_PROJ.CFDRange.CFDRangeMacs.CFDRangeUI
        CFDRangeUI <NEx=300.,NEy=250.> {
      &CFDRangeParams => <-.CFDRangeParams;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    mlink out <NEportLevels={1,2},NEx=100.,NEy=400.> => CFDRangeNodeData.out;
  };

  // CFDRangeEg
  //   Example Application (SingleWindowApp)

  // APPS.SingleWindowApp CFDRangeEg <NEdisplayMode="maximized"> {

  // };
};




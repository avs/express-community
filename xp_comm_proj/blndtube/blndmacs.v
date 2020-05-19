
flibrary BlendyTubesMacs <compile_subs=0> {

  // user interface
  macro SimpleTubesUI {
    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesParams &params<NEportLevels={2,1}>;

    UImod_panel panel {
      title => "Simple Tubes";
      parent<NEportLevels={4,0}>;
    };

    UIslider Facets {
      x = 0;
      y = 0;
      parent => <-.panel;
      min = 3.;
      max = 32.;
      value+Port2 => <-.params.facets;
      mode = "integer";
      decimalPoints = 0;
      increment = 3.;
    };
    UIslider Scale {
      x => <-.Facets.x;
      y => <-.Facets.y + <-.Facets.height + 20;
      parent => <-.panel;
      min = 0.;
      max = 10.;
      value => <-.params.scale;
      mode = "real";
      decimalPoints = 2;
    };

    UItoggle Offset_Scale {
      x => <-.Scale.x;
      y => <-.Scale.y + <-.Scale.height + 20;
      width => ((parent.width - x)/2) - 2;
      parent => <-.panel;
      set => <-.params.offsetScale;
    };
    UItoggle Invert_Scale {
      x => <-.Offset_Scale.x + <-.Offset_Scale.width + 4;
      y => <-.Offset_Scale.y;
      width => <-.Offset_Scale.width;
      parent => <-.panel;
      set => <-.params.invertScale;
    };
  };


  // user interface
  macro BlendyTubesUI {
    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesParams &params<NEportLevels={2,1}>;
    Mesh+Node_Data &in<NEportLevels={2,1}>;

    UImod_panel blendy_tubes {
      title => "Blendy Tubes";
      parent<NEportLevels={4,0}>;
    };

    UIslider Facets {
      x = 0;
      y = 0;
      parent => <-.blendy_tubes;
      min = 3.;
      max = 32.;
      value+Port2 => <-.params.facets;
      mode = "integer";
      decimalPoints = 0;
      increment = 3.;
    };
    UIslider Scale {
      x => <-.Facets.x;
      y => <-.Facets.y + <-.Facets.height + 20;
      parent => <-.blendy_tubes;
      min = 0.;
      max = 10.;
      value = 1.5;
      mode = "real";
      decimalPoints = 2;
    };


    UItoggle Offset_Scale {
      x => <-.Scale.x;
      y => <-.Scale.y + <-.Scale.height + 20;
      width => ((parent.width - x)/2) - 2;
      parent => <-.blendy_tubes;
      set => <-.params.offsetScale;
    };
    UItoggle Invert_Scale {
      x => <-.Offset_Scale.x + <-.Offset_Scale.width + 4;
      y => <-.Offset_Scale.y;
      width => <-.Offset_Scale.width;
      parent => <-.blendy_tubes;
      set => <-.params.invertScale;
    };


    DVnode_data_labels DVnode_data_labels {
      in => <-.in;
    };
    UIradioBoxLabel Scale_Component {
      parent => <-.blendy_tubes;
      labels => <-.DVnode_data_labels.labels;
      x => <-.Offset_Scale.x;
      y => <-.Offset_Scale.y + <-.Offset_Scale.height + 20;
      selectedItem<NEportLevels={2,2}>;
    };
    UIradioBoxLabel Colour_Component {
      parent => <-.blendy_tubes;
      labels => <-.DVnode_data_labels.labels;
      x => <-.Scale_Component.x;
      y => <-.Scale_Component.y + <-.Scale_Component.height + 20;
      selectedItem<NEportLevels={2,2}>;
    };


    int scaleComp<NEportLevels={1,2}>  => .Scale_Component.selectedItem;
    int colourComp<NEportLevels={1,2}> => .Colour_Component.selectedItem;

    int+Port initialScale = 50;

    float+Port coordSize => sqrt(   pow((in.coordinates.max_vec[0] - in.coordinates.min_vec[0]),2)
                                  + pow((in.coordinates.max_vec[1] - in.coordinates.min_vec[1]),2)
                                  + pow((in.coordinates.max_vec[2] - in.coordinates.min_vec[2]),2)
                                );

    GMOD.parse_v parse_v {
      v_commands = "finalScale = Scale.value*(coordSize/(in.node_data[scaleComp].max-in.node_data[scaleComp].min))/initialScale";
      trigger => <-.Scale.value + <-.scaleComp + <-.initialScale;
      no_errors = 1;
      relative => <-;
    };

    float+Port finalScale => .params.scale;

  };



  // functional macro
  macro BlendyTubesFunc {
    ilink in_fld;

    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesParams &params<NEportLevels={2,1}>;

    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesCore BlendyTubesCore {
      params => <-.params;
      lines_in => <-.in_fld;
    };

    GDM.DataObject DataObject {
      in => <-.BlendyTubesCore.tubes_out;
      Obj {
        name => name_of(<-.<-.<-);
      };
    };

    olink out_fld => .BlendyTubesCore.tubes_out;
    olink out_obj => .DataObject.obj;
  };



  // user macro
  macro simple_tubes {
    ilink lines;

    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesParams params {
      scale = 1.;
      facets = 3;
      invertScale = 0;
      offsetScale = 0;
    };

    SimpleTubesUI SimpleTubesUI {
      panel.title => name_of(<-.<-.<-);
      params => <-.params;
    };
    BlendyTubesFunc BlendyTubesFunc {
      params => <-.params;
      in_fld => <-.lines;
      DataObject.Obj.name => name_of(<-.<-.<-.<-);
    };

    olink out_fld => .BlendyTubesFunc.out_fld;
    olink out_obj => .BlendyTubesFunc.out_obj;
  };



  // user macro
  macro blendy_tubes {
    ilink lines;
    ilink data;

    XP_COMM_PROJ.BlendyTubes.BlendyTubesMods.BlendyTubesParams params {
      scale = 1.;
      facets = 3;
      invertScale = 0;
      offsetScale = 1;
    };

    int+Port col_comp   => .BlendyTubesUI.colourComp;
    int+Port scale_comp => .BlendyTubesUI.scaleComp;

    DVinterp_data DVinterp_data {
      in => <-.data;
      probe => <-.lines;
      comps => {<-.col_comp,<-.scale_comp};
    };

    BlendyTubesUI BlendyTubesUI {
      blendy_tubes.title => name_of(<-.<-.<-);
      params => <-.params;
      in => <-.data;
    };
    BlendyTubesFunc BlendyTubesFunc {
      params => <-.params;
      in_fld => <-.DVinterp_data.out;
      DataObject.Obj.name => name_of(<-.<-.<-.<-);
    };

    olink out_fld => .BlendyTubesFunc.out_fld;
    olink out_obj => .BlendyTubesFunc.out_obj;
  };


};


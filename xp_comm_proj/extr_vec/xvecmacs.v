
flibrary ExtractVectorMacs {

  // ExtractVectorUI
  //   UI Macro

  macro ExtractVectorUI {

    Node_Data &in <NEportLevels={2,0},NEx=55.,NEy=33.>;

    UImod_panel UImod_panel <NEx=11.,NEy=154.> {
      title = "ExtractVector";
    };

    XP_COMM_PROJ.ExtractVector.ExtractVectorMods.ExtractVectorParams
        &ExtractVectorParams <NEx=451.,NEy=22.,NEportLevels={2,1}>;

    int veclen <NEportLevels={1,0},NEx=550.,NEy=154.>
        => ExtractVectorParams.veclen;

    int current_elem <NEportLevels={1,1},NEx=11.,NEy=198.> {
      min = 0;
      max => (<-.veclen - 1);
    } = 0;

    int current_comp <NEportLevels={1,1},NEx=550.,NEy=110.>
        => ExtractVectorParams.components[.current_elem];

    int current_comp_elem
        <NEportLevels={1,1},NEx=550.,NEy=66.,NEwidth=209.,NEheight=66.>
        => ExtractVectorParams.sub_components[.current_elem];

    UIlabel title <NEx=297.,NEy=187.> {
      label = "Extract Vector";
      parent => <-.UImod_panel;
      y = 0;
      width => parent.width;
      color {
         foregroundColor = "#ffffff";
         backgroundColor = "#003477";
      };
      alignment = "center";
    };

    UIslider Output_Veclen <NEx=143.,NEy=242.> {
      parent => <-.UImod_panel;
      value <NEportLevels={2,2}> => <-.veclen;
      y = 24;
      width => parent.width;
      min = 1;
      decimalPoints = 0;
    };

    UIslider Output_Element <NEx=143.,NEy=286.> {
      parent => <-.UImod_panel;
      value <NEportLevels={2,2}> => <-.current_elem;
      y = 84;
      width => parent.width;
      max => <-.current_elem.max;
      min => <-.current_elem.min;
      decimalPoints = 0;
    };

    int num_comp <NEportLevels={1,1},NEx=77.,NEy=88.> => in.nnode_data;

    UIoption in_comps <NEx=11.,NEy=330.>[num_comp] {
      label => <-.in.node_data[index_of(in_comps)].labels;
    };

    UIoptionMenu Component <NEx=143.,NEy=374.> {
      parent => <-.UImod_panel;
      cmdList => <-.in_comps;
      selectedItem <NEportLevels={2,2}> => <-.current_comp;
      x = 0;
      y = 144;
      width => parent.width;
    };

    UIoption in_subcomp <NEx=297.,NEy=330.>[comp_veclen] {
      label => index_of(in_subcomp);
    };

    int comp_veclen <NEx=297.,NEy=242.,NEportLevels={1,0}>
        => in.node_data[current_comp].veclen;

    UIoptionMenu Sub_component <NEx=418.,NEy=374.> {
      parent => <-.UImod_panel;
      cmdList => <-.in_subcomp;
      selectedItem <NEportLevels={2,2}> => <-.current_comp_elem;
      x = 0;
      y = 174;
      width => parent.width;
    };
  };

  // ExtractVector
  //   Functional macro (Field I/O)

  macro ExtractVector {
    link in <NEportLevels={2,1},NEx=100.,NEy=50.>;

    XP_COMM_PROJ.ExtractVector.ExtractVectorMods.ExtractVectorParams
        &ExtractVectorParams <NEx=451.,NEy=66.,NEportLevels={2,1}>;

    DVextr_vector DVextr_vector<NEx=330.,NEy=132.> {
      in => <-.in;
      veclen => <-.ExtractVectorParams.veclen;
      components => <-.ExtractVectorParams.components;
      sub_components => <-.ExtractVectorParams.sub_components;
    };

    DVcomb_mesh_and_data DVcomb_mesh_and_data <NEx=627.,NEy=407.> {
      in_mesh => <-.in;
      in_nd => <-.DVextr_vector.out;
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=420.>
        => .DVcomb_mesh_and_data.out;
  };

  // 
  //   User Macro (and UI)

  macro extract_vector {
    link in <NEportLevels={2,1},NEx=100.,NEy=50.>;

    XP_COMM_PROJ.ExtractVector.ExtractVectorMods.ExtractVectorParams
        ExtractVectorParams <NEx=300.,NEy=50.> {
      veclen = 3;
      components = {0,0,0};
      sub_components = {0,1,2};
    };

    XP_COMM_PROJ.ExtractVector.ExtractVectorMacs.ExtractVector
        ExtractVector <NEx=100.,NEy=250.> {
      &ExtractVectorParams => <-.ExtractVectorParams;
      in => <-.in;
    };

    XP_COMM_PROJ.ExtractVector.ExtractVectorMacs.ExtractVectorUI
        ExtractVectorUI <NEx=300.,NEy=250.> {
      &ExtractVectorParams => <-.ExtractVectorParams;
      &in => <-.in;
      UImod_panel {
        title => name_of(<-.<-.<-);
      };
    };

    link out <NEportLevels={1,2},NEx=100.,NEy=400.> => ExtractVector.out;
  };
};





flibrary BlendNodeDataMacs {

  // Functional Macro

  macro Blend {
    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMods.BlendParams_ref
        &BlendParams_ref <NEx=55.,NEy=110.,NEportLevels={2,1}>;

    link in_fld1 <NEportLevels={2,1},NEx=88.,NEy=66.>;

    link in_fld2 <NEportLevels={2,1},NEx=121.,NEy=22.>;

    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMods.BlendNodeData
        BlendNodeData <NEx=253.,NEy=165.> {
      params => <-.BlendParams_ref;
      in1 => <-.in_fld1;
      in2 => <-.in_fld2;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data <NEx=220.,NEy=231.> {
      in_mesh => <-.in_fld1;
      in_nd => <-.BlendNodeData.out;
    };

    link out_fld <NEportLevels={1,2},NEx=341.,NEy=341.>
        => .combine_mesh_data.out;

    link out_obj <NEportLevels={1,2},NEx=374.,NEy=286.>
        => .combine_mesh_data.obj;
  };

  // UI Macro

  macro BlendUI {
    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMods.BlendParams_ref
        &BlendParams_ref <NEx=231.,NEy=33.,NEportLevels={2,1}>;

    UImod_panel panel <NEx=22.,NEy=88.> {
      parent<NEportLevels={3,0}>;
      title = "Blend";
    };

    UIlabel title <NEx=154.,NEy=143.> {
      parent => <-.panel;
      label => "Blend Node Data";
      width => parent.clientWidth;
      color {
        foregroundColor = "white";
        backgroundColor = "black";
      };
    };

    UIlabel label_ratio <NEx=154.,NEy=198.> {
      parent => <-.panel;
      label => "Blend Ratio";
      width => (parent.clientWidth / 2);
    };

    UIfield ratio_typein <NEx=154.,NEy=253.> {
      parent => <-.panel;
      value => <-.BlendParams_ref.ratio;
      y => <-.label_ratio.y;
      width => (parent.clientWidth / 2);
      x => width;
      min = 0.0;
      max = 1.0;
      mode = "real";
      decimalPoints = 4;
    };

    UIslider ratio_slider <NEx=154.,NEy=308.> {
      parent => <-.panel;
      value => <-.BlendParams_ref.ratio;
      width => parent.clientWidth;
      min = 0.0;
      max = 1.0;
      mode = "real";
      decimalPoints = 4;
      title =>;
    };
  };

  // User Macro

  macro blend_node_data {
    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMods.BlendParams
        BlendParams<NEx=22.,NEy=154.> {
      ratio = 0.;
    };

    link in_fld1 <NEportLevels={2,1},NEx=77.,NEy=99.>;

    link in_fld2 <NEportLevels={2,1},NEx=110.,NEy=44.>;

    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMacs.Blend Blend<NEx=187.,NEy=209.> {
      BlendParams_ref => <-.BlendParams;
      in_fld1 => <-.in_fld1;
      in_fld2 => <-.in_fld2;
      combine_mesh_data {
        DataObject {
	  Obj {
            name => name_of(<-.<-.<-.<-.<-);
          };
        };
      };
    };

    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMacs.BlendUI
        BlendUI <NEx=363.,NEy=209.> {
      BlendParams_ref => <-.BlendParams;
      panel {
        parent <NEportLevels={4,0}>;
        title => name_of(<-.<-.<-);
      };
    };

    link out_fld <NEportLevels={1,2},NEx=308.,NEy=330.> => .Blend.out_fld;
  
    link out_obj <NEportLevels={1,2},NEx=341.,NEy=275.> => .Blend.out_obj;
  };

  // Example Application (SingleWindowApp)

  APPS.SingleWindowApp BlendNodeDataEg <NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 65;
        y = 129;
      };
      Modules {
        IUI {
          optionList {
            selectedItem = 3;
          };
        };
      };
    };
    GDM.Uviewer3D Uviewer3D<NEx=603,NEy=453> {
      Scene {
        Top {
          child_objs => {<-.<-.<-.surf_plot.out_obj};
          Xform {
            ocenter = {3.5,3.5,0.5};
            dcenter = {4.,4.,0.571429};
            mat = {1.22314,-0.038186,0.123873,0.,
                   0.0693993,1.18567,-0.319748,0.,
                   -0.109482,0.324954,1.18123,0.,
		   0.,0.,0.,1.};
            xlate = {-4.,-4.,-0.571429};
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

    MODS.Read_Field one<NEx=121.,NEy=77.> {
      read_field_ui {
        file_browser {
          x = 490;
          y = 267;
          width = 300;
          height = 390;
          ok = 1;
          dirMaskCache = "./xp_comm_proj/blend/*";
        };
        flip = 1;
        filename = "./xp_comm_proj/blend/one.fld";
      };
      DVread_field {
        Mesh_Unif+Node_Data Output_Field;
      };
    };
    MODS.Read_Field two<NEx=462.,NEy=77.> {
      read_field_ui {
        file_browser {
          x = 490;
          y = 267;
          width = 300;
          height = 390;
          ok = 1;
          dirMaskCache = "./xp_comm_proj/blend/*";
        };
        flip = 1;
        filename = "./xp_comm_proj/blend/two.fld";
      };
      DVread_field {
        Mesh_Unif+Node_Data Output_Field;
      };
    };
 
    XP_COMM_PROJ.BlendNodeData.BlendNodeDataMacs.blend_node_data
        blend_node_data <NEx=363.,NEy=275.> {
      BlendParams {
        ratio<NEportLevels={3,2}> => <-.<-.Loop.count;
      };
      in_fld1 => <-.one.field;
      in_fld2 => <-.two.field;
      BlendUI {
        title {
          y = 0;
        };
        label_ratio {
          y = 24;
        };
        ratio_slider {
          y = 54;
        };
      };
    };
 
    MODS.Loop Loop <NEx=33.,NEy=220.> {
      cycle = 2;
      end = 1.;
      incr = 0.02;
      count = 0.;
      UIpanel {
        option {
          set = 1;
        };
      };
      LoopUI {
        Once {
          set = 0;
        };
        Bounce {
          set = 1;
        };
        cycle_toggle {
          x = 0;
        };
        start_typein {
          x = 0;
        };
        end_typein {
          x = 0;
        };
        incr_typein {
          x = 0;
        };
        count_typein {
          x = 0;
        };
      };
    };
    MODS.surf_plot surf_plot <NEx=363.,NEy=352.> {
      in_field => <-.blend_node_data.out_fld;
    };
  };
};




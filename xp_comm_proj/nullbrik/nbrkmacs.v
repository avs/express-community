
flibrary NullBrickMacs <cxx_name=""> {

  // NullBrick
  //   Functional macro (Field I/O)

  macro NullBrick <NEx=165.,NEy=110.> {

    link in_fld <NEportLevels={2,1},NEx=44.,NEy=88.>;
    
    XP_COMM_PROJ.NullBrick.NullBrickMods.NullBrickParams_ref
        &NullBrickParams_ref <NEx=88.,NEy=33.,NEportLevels={2,1}>;
	
    XP_COMM_PROJ.NullBrick.NullBrickMods.NullBrickNodeData
	NullBrickNodeData <NEx=286.,NEy=176.> {
      in => <-.in_fld;
      params => <-.NullBrickParams_ref;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data <NEx=220.,NEy=242.> {
      in_mesh => <-.in_fld;
      in_nd => <-.NullBrickNodeData.out;
    };

    link out_fld <NEportLevels={1,2},NEx=341.,NEy=385.>
        => .combine_mesh_data.out;
    link out_obj <NEportLevels={1,2},NEx=385.,NEy=341.>
        => .combine_mesh_data.obj;
  };
  
  // NullBrickUI
  //   UI Macro

  macro NullBrickUI <NEx=110.,NEy=66.> {
    
    link in_fld <NEportLevels={2,1},NEx=99.,NEy=66.>;
    
    XP_COMM_PROJ.NullBrick.NullBrickMods.NullBrickParams_ref
        &p_ref <NEx=429.,NEy=33.,NEportLevels={2,1}>;

    Node_Data_Struct &in_nd <NEx=264.,NEy=66.,NEportLevels={1,0}> => .in_fld;
    
    UImod_panel panel<NEx=22.,NEy=121.> {
      parent <NEportLevels={3,0}>;
      title = "NullBrick";
    };
    
    UIlabel title <NEx=176.,NEy=165.> {
      parent => <-.panel;
      label => "Null Brick Controls";
      y = 0;
      width => parent.clientWidth;
      color {
	foregroundColor = "white";
	backgroundColor = "black";
      };
    };
    
    UIlabel label_extents <NEx=363.,NEy=165.> {
      parent => <-.panel;
      label => "Brick Extents";
      y = 26;
      width => parent.clientWidth;
      alignment = "left";
    };
    
    UIslider xmin <NEx=363.,NEy=220.> {
      parent => <-.panel;
      value => p_ref.xmin;
      y = 52;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[0] - 1);
      mode = "integer";
      decimalPoints = 0;
    };

    UIslider xmax <NEx=363.,NEy=275.> {
      parent => <-.panel;
      value => p_ref.xmax;
      y = 112;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[0] - 1);
      mode = "integer";
      decimalPoints = 0;
    };
  
    UIslider ymin <NEx=363.,NEy=330.> {
      parent => <-.panel;
      value => p_ref.ymin;
      y = 172;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[1] - 1);
      mode = "integer";
      decimalPoints = 0;
    };

    UIslider ymax <NEx=363.,NEy=385.> {
      parent => <-.panel;
      value => p_ref.ymax;
      y = 232;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[1] - 1);
      mode = "integer";
      decimalPoints = 0;
    };

    UIslider zmin <NEx=363.,NEy=440.> {
      parent => <-.panel;
      value => p_ref.zmin;
      y = 292;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[2] - 1);
      mode = "integer";
      decimalPoints = 0;
    };

    UIslider zmax <NEx=363.,NEy=275.> {
      parent => <-.panel;
      value => p_ref.zmax;
      y = 352;
      width => parent.clientWidth;
      min = 0.;
      max => (in_nd.dims[2] - 1);
      mode = "integer";
      decimalPoints = 0;
    };

    UIlabel label_nval<NEx=528.,NEy=165.> {
      y = 412;
      width => (parent.clientWidth / 2);
      parent => <-.panel;
      label => "Null/Brick Value";
      alignment = "left";
    };

    UIfield nval<NEx=528.,NEy=220.> {
      parent => <-.panel;
      value => <-.p_ref.nval;
      y => label_nval.y;
      x => label_nval.width;
      mode = "real";
      decimalPoints = 6;
    };
    
    UItoggle setnull<NEx=528.,NEy=275.> {
      parent => <-.panel;
      label => "Set Null Flag";
      set<NEportLevels={2,2}> => <-.p_ref.setnull;
      y = 442;
      width => parent.clientWidth;
    };
  };

  // 
  //   User Macro (and UI)

  macro null_brick <NEx=209.,NEy=143.> {

    link in_fld <NEportLevels={2,1},NEx=44.,NEy=33.>;
    
    XP_COMM_PROJ.NullBrick.NullBrickMods.NullBrickParams
        NullBrickParams <NEx=44.,NEy=88.> {
      nval = 0.;
      setnull = 0;
      xmin = 0;
      xmax = 0;
      ymin = 0;
      ymax = 0;
      zmin = 0;
      zmax = 0;
    };
    
    XP_COMM_PROJ.NullBrick.NullBrickMacs.NullBrick NullBrick <NEx=231.,NEy=165.> {
      in_fld => <-.in_fld;
      NullBrickParams_ref => <-.NullBrickParams;
    };
    
    XP_COMM_PROJ.NullBrick.NullBrickMacs.NullBrickUI
        NullBrickUI <NEx=407.,NEy=165.> {
      in_fld => <-.in_fld;
      p_ref => <-.NullBrickParams;
      panel {
        title => name_of(<-.<-.<-);
        option {
          set = 1;
        };
      parent<NEportLevels={4,0}>;
      };
    };

    link out_fld <NEportLevels={1,2},NEx=341.,NEy=308.> => .NullBrick.out_fld;
    link out_obj <NEportLevels={1,2},NEx=374.,NEy=242.> => .NullBrick.out_obj;

  };

  // NullBrickEg1
  //   Example Application (SingleWindowApp)

  APPS.MultiWindowApp NullBrickEg1 {
    UI {
      shell {
	x = 0;
	y = 0;
      };
      Modules {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	  mod_panel {
	    x = 0;
	    y = 0;
	  };
	};
      };
    };
    GDM.Uviewer3D Uviewer3D<NEx=352.,NEy=264.> {
      Scene {
	Top {
	  child_objs => {
	  <-.<-.<-.volume_render.DataObject.obj,<-.<-.<-.bounds.out_obj};
	  Xform {
	    ocenter = {31.5,31.5,31.5};
	    dcenter = {3.89464,4.1821,
	    5.58911};
	    mat = {
	      0.00116744,-0.0715184,-0.0392415,0.,-0.0534801,0.0289643,-0.0543791,
	      0.,0.0616009,0.0265014,-0.0464668,0.,0.,0.,0.,1.
	    };
	    xlate = {-3.89464,-4.1821,
	    -5.58911};
	  };
	};
	View {
	  View {
	    renderer => "Software";
	    trigger = 1;
	  };
	};
      };
      Scene_Editor {
	Camera_Editor {
	  GDcamera_edit {
	    front = -88.;
	  };
	};
      };
    };
    XP_COMM_PROJ.NullBrick.NullBrickMacs.null_brick null_brick<NEx=165.,NEy=132.> {
      in_fld => <-.Read_Field.field;
      NullBrickParams {
	xmax = 38;
	ymax = 30;
	zmax = 36;
      };
      NullBrick {
	combine_mesh_data {
	  out<NEportLevels={0,2}>;
	};
      };
    };
    MODS.Read_Field Read_Field<NEx=165.,NEy=55.> {
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
    MODS.volume_render volume_render<NEx=165.,NEy=198.> {
      in_mesh => <-.null_brick.out_fld;
      DataObject {
	Props {
	  fat_ray = 0;
	  inherit = 0;
	};
      };
      GDprops_edit {
	fat_ray = 0;
	inherit = 0;
      };
      inherit {
	set = 1;
      };
      surf_rb {
	x = 0;
      };
      vol_rb {
	x = 0;
      };
      interp_tri {
	set = 1;
      };
      interp_rb {
	x = 0;
      };
      ray_direct {
	set = 1;
      };
      ray_rb {
	x = 0;
      };
      dist_glob {
	set = 1;
      };
      dist_rb {
	x = 0;
      };
      alpha_const {
	set = 1;
      };
      alpha_rb {
	x = 0;
      };
      GDmodes_edit {
	volume = "Ray Tracer";
      };
      vol_inherit {
	set = 1;
      };
    };
    MODS.bounds bounds<NEx=352.,NEy=132.> {
      in_field => <-.Read_Field.field;
    };
  };
  
  // NullBrickEg2
  //   Example Application (SingleWindowApp)

  APPS.MultiWindowApp NullBrickEg2<NEdisplayMode="maximized"> {
    UI {
      shell {
	x = 0;
	y = 0;
      };
      Modules {
	IUI {
	  optionList {
	    cmdList => {
	      <-.<-.<-.<-.null_brick.NullBrickUI.panel.option,
	      <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
	      <-.<-.<-.<-.bounds.UIpanel.option,
	    <-.<-.<-.<-.isosurface.UIpanel.option};
	    selectedItem = 0;
	  };
	  mod_panel {
	    x = 0;
	    y = 0;
	  };
	};
      };
    };
    GDM.Uviewer3D Uviewer3D<NEx=352.,NEy=264.> {
      Scene {
	Top {
	  child_objs => {
	  <-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
	  Xform {
	    ocenter = {31.5,31.5,31.5};
	    dcenter = {4.,4.,4.};
	    mat = {
	      -0.061988,-0.10435,0.0367775,0.,-0.0246835,-0.0280512,-0.121194,
	      0.,0.107853,-0.0663948,-0.00659887,0.,0.,0.,0.,1.
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
	    renderer => "OpenGL";
	    trigger = 1;
	  };
	  ViewUI {
	    ViewPanel {
	      UI {
		panel {
		  defaultX = 299;
		  defaultY = 192;
		};
	      };
	    };
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
    XP_COMM_PROJ.NullBrick.NullBrickMacs.null_brick null_brick<NEx=165.,NEy=132.> {
      in_fld => <-.Read_Field.field;
      NullBrickParams {
	xmin = 14;
	xmax = 55;
	ymax = 34;
	zmin = 21;
	zmax = 34;
      };
    };
    MODS.Read_Field Read_Field<NEx=165.,NEy=55.> {
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
    MODS.bounds bounds<NEx=352.,NEy=132.> {
      in_field => <-.Read_Field.field;
    };
    MODS.isosurface isosurface<NEx=165.,NEy=198.> {
      in_field => <-.null_brick.out_fld;
      IsoParam {
	iso_level => 66.;
      };
      DVcell_data_labels {
	labels[];
      };
      DVnode_data_labels {
	labels[];
      };
      IsoUI {
	UIradioBoxLabel {
	  label_cmd {
	    cmd[];
	  };
	};
	UIoptionBoxLabel {
	  label_cmd {
	    cmd[];
	  };
	};
	UIoptionBoxLabel_cell {
	  label_cmd {
	    cmd[];
	  };
	};
      };
    };
  };
};

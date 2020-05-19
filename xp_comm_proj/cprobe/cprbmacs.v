
flibrary ClusterProbeMacs {

  // Functional Macro

  // UI Macro

  macro ClusterProbeUI {
    UImod_panel panel<NEx=22.,NEy=110.> {
      parent <NEportLevels={3,0}>;
      title = "ClusterProbe";
    };
    
    link in_glyph <NEportLevels={2,1},NEx=143.,NEy=44.>;
    
    UIlabel title <NEx=165.,NEy=176.> {
      parent => <-.panel;
      label => "Cluster Probe";
      width => parent.clientWidth;
      color {
        foregroundColor = "white";
        backgroundColor = "blue";
      };
      alignment = "center";
    };
    
    UItoggle toggle_edit <NEx=165.,NEy=242.> {
      parent => <-.panel;
      label => "Glyph Transformation Editor";
      width => parent.clientWidth;
      alignment = "left";
    };
    
    XformEditor XformEditor <NEx=352.,NEy=330.> {
      obj_in => <-.in_glyph;
      vis => <-.toggle_edit.set;
    };
  };
  
  // User Macros

  macro cluster_probe {

    link in_base <NEportLevels={2,1},NEx=44.,NEy=88.>;

    link in_glyph <NEportLevels={2,1},NEx=99.,NEy=33.>;

    XP_COMM_PROJ.ClusterProbe.ClusterProbeMods.ClusterProbeCreate
        ClusterProbeCreate <NEx=220.,NEy=187.> {
      in_base => <-.in_base;
      in_glyph => <-.in_glyph;
    };
     
    MODS.reset_xform reset_xform <NEx=220.,NEy=253.> {
      in_field => <-.ClusterProbeCreate.out;
    };
    
    DataObject DataObject <NEx=374.,NEy=341.> {
      in => <-.reset_xform.out_fld;
      Obj.name => name_of(<-.<-.<-);
    };

    link out <NEportLevels={1,2},NEx=341.,NEy=407.> => .reset_xform.out_fld;

    link out_obj <NEportLevels={1,2},NEx=517.,NEy=407.> => .DataObject.obj;
  };

  macro cluster_probe_edit <NEx=66.,NEy=66.> {

    link in_base <NEportLevels={2,1},NEx=44.,NEy=121.>;

    link in_glyph <NEportLevels={2,1},NEx=99.,NEy=66.>;
    
    XP_COMM_PROJ.ClusterProbe.ClusterProbeMods.ClusterProbeCreate
        ClusterProbeCreate <NEx=220.,NEy=187.> {
      in_base => <-.in_base;
      in_glyph => <-.in_glyph;
    };

    XP_COMM_PROJ.ClusterProbe.ClusterProbeMacs.ClusterProbeUI
        ClusterProbeUI <NEx=451.,NEy=187.> {
      panel {
	parent <NEportLevels={4,0}>;
	title => name_of(<-.<-.<-);
      };
      
      in_glyph => <-.in_glyph;
    };

    MODS.reset_xform reset_xform <NEx=220.,NEy=253.> {
      in_field => <-.ClusterProbeCreate.out;
    };
    
    DataObject DataObject <NEx=374.,NEy=341.> {
      in => <-.reset_xform.out_fld;
      Obj.name => name_of(<-.<-.<-);
    };

    link out <NEportLevels={1,2},NEx=341.,NEy=407.> => .reset_xform.out_fld;

    link out_obj <NEportLevels={1,2},NEx=517.,NEy=407.> => .DataObject.obj;

  };
  
  // Example Application (SingleWindowApp)

  APPS.SingleWindowApp ClusterProbeEg <NEdisplayMode="maximized"> {
    UI {
      shell {
        x = 197;
        y = 290;
      };
      Modules {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
    };
    GDM.Uviewer3D Uviewer3D <NEx=603,NEy=453> {
      Scene {
	Top {
	  child_objs => {<-.<-.<-.cluster_probe_edit.out_obj};
	  Xform {
	    ocenter = {0.525,0.525,0.025};
	    dcenter = {0.271148,0.271148,-1.72885};
	    mat = {4.57713,-2.22155,3.5568,0.,4.17399,2.92176,-3.54647,0.,
	      -0.404893,5.00643,3.64802,0.,0.,0.,0.,1.
	    };
	    xlate = {-0.271149,-0.271149,1.72885};
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
	    trigger = 2;
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

    XP_COMM_PROJ.ClusterProbe.ClusterProbeMacs.cluster_probe_edit
        cluster_probe_edit <NEx=187.,NEy=209.> {
      in_base => <-.Plane.out_fld;
      in_glyph => <-.Box.out_fld;
      ClusterProbeUI {
	panel {
	  option {
	    set = 1;
	  };
	};
	title {
	  y = 0;
	};
	toggle_edit {
	  y = 24;
	};
	XformEditor {
	  GDxform_editor {
	    scale = 0.05;
	    abs_scale = 0.05000000075;
	  };
	  XformEditorUI {
	    trans_shell {
	      x = 403;
	      y = 232;
	      ok = 1;
	    };
	  };
	};
      };
    };

    GEOMS.Plane Plane<NEx=88.,NEy=88.>;
    GEOMS.Box Box<NEx=308.,NEy=88.> {
      box {
	xform {
	  mat = {0.05,0.,0.,0.,0.,0.05,0.,0.,0.,0.,0.05,0.,0.,0.,0.,1.};
	};
      };
      dim1 = 4;
      dim2 = 4;
      dim3 = 4;
    };
  };
};




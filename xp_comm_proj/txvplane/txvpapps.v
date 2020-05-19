
flibrary TextureVPlaneApps {

  // Functional Macro
  APPS.SingleWindowApp TextureVPlaneEg <NEdisplayMode="maximized"> {
    UI {
      shell {
	x = 88;
	y = 64;
      };
      Modules {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
    };
    GDM.Uviewer3D Uviewer3D<NEx=275.,NEy=330.> {
      Scene {
	Top {
	  child_objs => {<-.<-.<-.texture_vplane.out_obj};
	  Xform {
	    ocenter = {0.,0.,1.};
	    dcenter = {0.,0.,4.};
	    mat = {4.,0.,0.,0.,0.,0.,4.,0.,0.,-4.,0.,0.,0.,0.,0.,1.};
	    xlate = {0.,0.,-4.};
	    center = {0.,0.,1.};
	  };
	};
	View {
	  View {
	    trigger = 1;
	  };
	};
      };
      Scene_Editor {
	View_Editor {
	  GDobj_sel {
	    curobj_name = "Top";
	  };
	  InfoPanel {
	    curobj_sel {
	      selectedItem = 1;
	      x = 490;
	      y = 298;
	      width = 300;
	      height = 100;
	      ok = 1;
	    };
	  };
	};
	Track_Editor {
	  IUI {
	    optionList {
	      selectedItem = 1;
	    };
	    Interactor_Behavior {
	      IUI {
		RotateMode {
		  OPcmdList = {{set=1,,,},,};
		};
		XYTranslateMode {
		  OPcmdList = {{set=1,,,},,,};
		};
	      };
	    };
	    Transform {
	      IUI {
		panel {
		  visible = 1;
		};
		ScaleLabel {
		  y = 0;
		};
	      };
	    };
	  };
	  GDxform_edit {
	    abs_x_cent = 0.;
	    abs_y_cent = 0.;
	    abs_z_cent = 1.;
	    abs_x_trans = 0.;
	    abs_y_trans = 0.;
	    abs_z_trans = -4.;
	    abs_x_rot = 90.;
	    abs_y_rot = 0.;
	    abs_z_rot = 0.;
	    abs_scale = 4.;
	  };
	};
	Object_Editor {
	  IUI {
	    optionList {
	      selectedItem = 0;
	    };
	    General {
	      IUI {
		panel {
		  visible = 1;
		};
		AltSpace {
		  OPcmdList = {{set=1,,,},};
		};
		TransformMode {
		  OPcmdList = {{set=1,,,},,,};
		};
	      };
	    };
	  };
	};
      };
    };
    XP_COMM_PROJ.TextureVPlane.TextureVPlaneMacs.texture_vplane texture_vplane<NEx=275.,NEy=209.> {
      in_texture => <-.Read_Image.field;
      TextureVPlane {
	struct_mesh {
	  out {
	    xform {
	      ocenter = {0.,0.,1.};
	      dcenter = {0.,0.,1.};
	      center = {0.,0.,1.};
	    };
	  };
	};
      };
      TextureVPlaneUI {
	panel {
	  option {
	    set = 1;
	  };
	};
      };
    };

    MODS.Read_Image Read_Image<NEx=275.,NEy=99.> {
      read_image_ui {
	file_browser {
	  filename = "$XP_ROOT/data/image/mandrill.x";
	  x = 490;
	  y = 267;
	  width = 300;
	  height = 390;
	  ok = 1;
	  dirMaskCache = "$XP_ROOT/data/image/*";
	};
      };
    };
  };
};
	
	

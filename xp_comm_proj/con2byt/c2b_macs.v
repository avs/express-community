flibrary Convert2ByteMacs {

  // Convert2Byte
  // Functional & user macro
  macro Convert2Byte {
    link in<NEportLevels={2,1},NEx=33.,NEy=33.>;

    XP_COMM_PROJ.Convert2Byte.Convert2ByteMods.ConvertToByteNodeData ConvertToByteNodeData<NEx=198.,NEy=132.> {
       in => <-.in;
    };
    FLD_MAP.combine_mesh_data combine_mesh_data<NEx=66.,NEy=187.> {
       in_mesh => <-.in;
       in_nd => <-.ConvertToByteNodeData.out;
    };
    link out<NEportLevels={1,2},NEx=33.,NEy=200.>
       => combine_mesh_data.out;
  };



  // Convert2ByteEg
  // Example Application
    APPS.MultiWindowApp Convert2ByteEg<NEdisplayMode="maximized"> {
       UI {
	  shell {
	     x = 0;
	     y = 0;
	  };
	  Modules {
	     IUI {
		optionList {
		   selectedItem = 1;
		};
		mod_panel {
		   x = 0;
		   y = 0;
		};
	     };
	  };
       };
       GDM.Uviewer3D Uviewer3D<NEx=363.,NEy=418.> {
	  Scene {
	     Top {
		child_objs => {
		   <-.<-.<-.volume_render.DataObject.obj};
		Xform {
		   ocenter = {3.27323,4.16378,
    2.86213};
		   dcenter = {1.18071,1.50195,
    1.03242};
		   mat = {
		      -0.294013,0.044999,0.204085,0.,-0.0076254,0.349714,-0.0880944,
    0.,-0.208848,-0.0761173,-0.284091,0.,0.,0.,0.,1.
		   };
		   xlate = {-1.18071,-1.50195,
    -1.03242};
		};
	     };
	     View {
		View {
		   renderer => "Software";
		};
		ViewUI {
		   ViewPanel {
		      UI {
			 panel {
			    defaultX = 551;
			    defaultY = 247;
			 };
		      };
		   };
		};
	     };
	  };
       };
       MODS.volume_render volume_render<NEx=275.,NEy=341.> {
	  in_mesh => <-.extract_component.out_fld;
	  inherit {
	     set = 1;
	  };
	  surf_rb {
	     x = 0;
	  };
	  ray_tracer {
	     set = 1;
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
       };
       MODS.extract_component extract_component<NEx=198.,NEy=264.> {
	  in_field => <-.Convert2Byte.out;
	  ExtrCompUI {
	     panel {
		option {
		   set = 1;
		};
	     };
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
       XP_COMM_PROJ.Convert2Byte.Convert2ByteMacs.Convert2Byte Convert2Byte<NEx=154.,NEy=198.> {
	  in => <-.Read_Field.field;
       };
       MODS.Read_Field Read_Field<NEx=99.,NEy=88.> {
	  read_field_ui {
	     file_browser {
		x = 490;
		y = 267;
		width = 300;
		height = 390;
		ok = 1;
		dirMaskCache = "$XP_ROOT/data/field/*.fld";
	     };
	     filename = "$XP_ROOT/data/field/bluntfin.fld";
	  };
	  DVread_field {
	     Mesh_Struct+Node_Data Output_Field;
	  };
       };
    };
};

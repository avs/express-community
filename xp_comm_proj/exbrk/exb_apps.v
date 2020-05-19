
flibrary ExcavateBrickApps{
	APPS.SingleWindowApp ExcavateBrickEg<NEdisplayMode="maximized"> {
	   UI {
		  Modules {
			 IUI {
				optionList {
				   cmdList => {
					  <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
					  <-.<-.<-.<-.bounds.UIpanel.option,
					  <-.<-.<-.<-.Excavate_Brick.ExcavateBrickUI.UImod_panel.option,
					  <-.<-.<-.<-.isosurface.UIpanel.option};
				   selectedItem = 2;
				};
			 };
		  };
	   };
	   GDM.Uviewer3D Uviewer3D {
		  Scene {
			 Top {
				child_objs => {
				   <-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
				Xform {
				   ocenter = {31.5, 31.5, 31.5};
				   dcenter = {3.59544, 5.05557, 1.09576};
				   mat = {-0.0545283, -0.0183956, -0.0474022, 0.,
				          -0.00499453, 0.071108, -0.0218498, 0.,
						  0.0506006, -0.0128048, -0.0532383, 0.,
						  0., 0., 0., 1. };
				   xlate = {-3.59544, -5.05557, -1.09576};
				};
			 };
		  };
		  Scene_Editor {
			 View_Editor {
				InfoPanel {
				   curobj_sel {
					  selectedItem = 0;
					  width = 235;
					  height = 249;
					  x = 0;
					  y = 0;
					  ok = 1;
				   };
				};
			 };
		  };
	   };
	   MODS.Read_Field Read_Field {
		  read_field_ui {
			 file_browser {
				ok = 1;
			 };
			 filename = "$XP_PATH<0>/data/field/hydrogen.fld";
		  };
		  DVread_field {
			 Mesh_Unif+Node_Data Output_Field;
		  };
	   };
	   MODS.bounds bounds {
		  in_field => <-.Excavate_Brick.out;
	   };
	   XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMacs.Excavate_Brick Excavate_Brick {
		  Input_Field => <-.Read_Field.field;
		  Params {
			 X = 50;
			 Y = 38;
			 Z = 30;
			 belowX = 1;
			 belowY = 1;
			 belowZ = 1;
		  };
	   };
	   MODS.isosurface isosurface {
		  in_field => <-.Excavate_Brick.out;
	   };
	};
};

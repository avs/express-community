flibrary VolumeToContourApps{
	APPS.SingleWindowApp VolumeToContourEg {
	   UI {
		  shell {
			 x = 88;
			 y = 88;
		  };
		  Modules {
			 IUI {
				optionList {
				   selectedItem = 0;
				};
			 };
		  };
	   };
	   GDM.Uviewer3D Uviewer3D {
		  Scene {
			 Top {
				child_objs => {
				   <-.<-.<-.Volume_To_Contour.obj,<-.<-.<-.orthoslice.out_obj};
				Xform {
				   ocenter = {12., 12., 15.5};
				   dcenter = {4.59086, 4.169, 6.19057};
				   mat = {
					  -0.169914, -0.119714, -0.0763619, 0.,
					  0.0322685, -0.148522, 0.161039, 0.,
					  -0.13828, 0.112442, 0.131411, 0.,
					  0.,0.,0.,1.
				   };
				   xlate = {-5.37479, -3.15301,	-6.18431};
				};
			 };
			 Lights {
				Lights = {
				   {
					  type="BiDirectional"
				   },,,};
			 };
		  };
		  Scene_Editor {
			 View_Editor {
				InfoPanel {
				   curobj_sel {
					  width = 235;
					  height = 249;
					  x = 0;
					  y = 0;
				   };
				};
			 };
		  };
	   };
	   XP_COMM_PROJ.VolumeToContour.VolumeToContourMacs.Volume_To_Contour Volume_To_Contour {
		  in => <-.Read_Field.field;
		  Params {
			 axis = 1;
			 ncontours = 6;
			 level_min = -17.28;
			 level_max = 6.74;
		  };
	   };
	   MODS.Read_Field Read_Field {
		  read_field_ui {
			 file_browser {
				ok = 1;
			 };
			 filename = "$XP_PATH<0>/data/field/wind.fld";
		  };
		  DVread_field {
			 Mesh_Unif+Node_Data Output_Field;
		  };
	   };
	   MODS.orthoslice orthoslice {
		  in_field => <-.Read_Field.field;
	   };
	};
};

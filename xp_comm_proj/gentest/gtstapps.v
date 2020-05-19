flibrary GenerateTestDataApps{
	APPS.SingleWindowApp GenerateTestDataEg {
	   UI {
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
				   <-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice.out_obj,
				   <-.<-.<-.isosurface.out_obj};
				Xform {
				   ocenter = {32.,62.5,7.};
				   dcenter = {2.93204,2.59749,
	1.44795};
				   mat = {
					  -0.0516071,0.032907,-0.0270626,0.,-0.0339523,-0.00608386,0.0573479,
	0.,0.025739,0.057954,0.0213867,0.,0.,0.,0.,1.
				   };
				   xlate = {-2.93204,-2.59749,
	-1.44795};
				};
			 };
			 Lights {
				Lights = {
				   {
					  type="BiDirectional"
				   },,,};
			 };
			 View {
				ViewUI {
				   ViewPanel {
					  UI {
						 panel {
							defaultX = -2;
							defaultY = 198;
						 };
					  };
				   };
				};
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
	   XP_COMM_PROJ.GenerateTestData.GenerateTestDataMacs.GenerateTestData GenerateTestData {
		  Params {
			 pattern = 1;
			 width = 65;
			 height = 126;
			 num_frames = 15;
		  };
		  GenerateTestDataUI {
			 UImod_panel {
				option {
				   set = 1;
				};
			 };
		  };
	   };
	   MODS.bounds bounds {
		  in_field => <-.GenerateTestData.out;
	   };
	   MODS.orthoslice orthoslice {
		  in_field => <-.GenerateTestData.out;
		  OrthoSliceParam {
			 axis = 1;
		  };
	   };
	   MODS.isosurface isosurface {
		  in_field => <-.GenerateTestData.out;
		  DVcell_data_labels {
			 labels[];
		  };
		  DVnode_data_labels {
			 labels[];
		  };
	   };
	};
};

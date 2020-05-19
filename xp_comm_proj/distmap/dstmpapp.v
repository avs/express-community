
flibrary DistanceMapApps<compile_subs=0> {

	APPS.MultiWindowApp DistanceMapSegmentationEg<NEdisplayMode="maximized"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.DistanceMapSegmentation DistanceMapSegmentation {
			in_volume => <-.Read_Field.field;
			  distmap_params.fill_params {
				tolerance = 280;
			};
			  distmap_params.out_params {
				t1 = 158;
				smooth = 0;
			};
		};
		MODS.Read_Field Read_Field{
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		MODS.bounds bounds {
			in_field => <-.DistanceMapSegmentation.out_field;
		};
		MODS.volume_render volume_render {
			in_mesh => <-.DistanceMapSegmentation.out_field;
			DataObject {
				VolRenderDatamap {
					DataRange = {
						{
							selectAlphaRange=0,,,,,,,
						},
					};
					DatamapValue = {,
						{v1=0.06},
						{v1=0.09},
						{v1=0.36}
					};
				};
				Props {
					fat_ray = 0;
				};
			};
			GDmodes_edit {
				volume = "Ray Tracer";
			};
		};
		GDM.Uviewer3D Uviewer3D{
			Scene {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.volume_render.out_obj,<-.<-.<-.bounds.out_obj};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {4.,4.,4.};
						mat = {
							0.117975,0.0237191,-0.0405511,0.,
							-0.0138426,0.122295,0.0312607,0.,
							0.0448929,-0.0246223,0.116204,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-4.};
					};
				};
				View {
					View {
						renderer => "Software";
					};
				};
			};
			Scene_Selector {
				input_views => {
					<-.Scene.View.View,
					<-.Input.View.View,
					<-.Mask.View.View,
					<-.Fill.View.View,
					<-.Result.View.View
				};
			};
			GDM.Uscene3D Input {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DistanceMapSegmentation.data_obj};
				};
			};
			GDM.Uscene2D Mask {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DistanceMapSegmentation.mask_obj};
				};
			};
			GDM.Uscene2D Fill {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DistanceMapSegmentation.fill_obj};
				};
			};
			GDM.Uscene2D Result {
				Top {
					child_objs<NEportLevels={4,1}> => {
						<-.<-.<-.DistanceMapSegmentation.res_obj};
				};
			};
		};
	};

	APPS.MultiWindowApp DistanceMapSegmentationDialogEg<NEdisplayMode="maximized"> {
		XP_COMM_PROJ.DistanceMap.DistanceMapMacs.DistanceMapSegmentationDialog DistanceMapSegmentationDialog<NEx=319.,NEy=231.,NEdisplayMode="maximized"> {
			in => <-.Read_Field.field;
			visible => 1;
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					x = 556;
					y = 305;
					width = 300;
					height = 386;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
	};

};

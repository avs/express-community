flibrary AnimatedImageGeneratorApps {
	APPS.SingleWindowApp AnimatedImageGeneratorEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.bounds bounds{
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
		XP_COMM_PROJ.AnimatedImageGenerator.AnimatedImageGeneratorMacs.AnimatedImageGenerator AnimatedImageGenerator {
			view_in => <-.Uviewer3D.Scene_Selector.curr_view;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
					Xform {
						ocenter = {31.5,31.5,31.5};
						dcenter = {4.,4.,4.};
						mat = {
							0.0869414,-0.0510596,0.0771956,0.,
							0.01724,0.112994,0.0553202,0.,
							-0.090934,-0.0273952,0.0842944,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-4.};
					};
				};
			};
		};
	};
};

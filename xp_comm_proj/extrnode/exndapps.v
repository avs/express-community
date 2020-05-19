
flibrary ExtractNodeDataApps<compile_subs=0> {
	APPS.SingleWindowApp ExtractNodeDataEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {<-.<-.<-.Read_Field.out_obj};
					Xform {
						ocenter = {12., 12., 15.5};
						dcenter = {4., 4., 5.16667};
						mat = {
							0.315584, 0.0563122, -0.0913619, 0.,
							-0.020591, 0.31026, 0.120107, 0.,
							0.105328, -0.108068, 0.297218, 0.,
							0., 0., 0., 1.
						};
						xlate = {-4., -4., -5.16667};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/wind.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		IAC.MISC.ExtractNodeData ExtractNodeData {
			in => <-.Read_Field.field;
		};
	};
};

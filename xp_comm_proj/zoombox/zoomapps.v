flibrary ZoomBoxApps<compile_subs=0>
{
	APPS.SingleWindowApp ZoomBoxEg <NEdisplayMode="maximized"> {
		UI {
			shell {
				x = 49;
				y = 412;
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
						<-.<-.<-.Read_Field.out_obj};
					Xform {
						ocenter = {0.239715, 1.57981, 2.86213};
						dcenter = {0.0375187, 1.18029, 1.84223};
						mat = {
							1.46899, -0.390313, 0.161027, 0.,
							0.36656, 0.889648, -1.18759, 0.,
							0.20954, 1.18, 0.948636, 0.,
							0., 0., 0., 1.
						};
						xlate = {-0.000714421, -0.531953, -1.03242};
						center = {0.239715, 1.57981, 2.86213};
					};
				};
				View {
					View {
						trigger = 1;
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
						set = 1;
					};
				};
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/bluntfin.fld";
			};
			DVread_field {
				Mesh_Struct+Node_Data Output_Field;
			};
		};
		XP_COMM_PROJ.ZoomBox.ZoomBoxMacs.ZoomBox ZoomBox;
	};

};

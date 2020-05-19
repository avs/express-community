
flibrary Convert2TriStripApps {
	APPS.MultiWindowApp Convert2TriStripEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/helens1.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		XP_COMM_PROJ.Convert2TriStrip.Convert2TriStripMacs.Convert2TriStrip Convert2TriStrip<NEx=330.,NEy=242.> {
			in_fld => <-.Read_Field.field;
		};
		GDM.Uviewer2D Uviewer2D_Field {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Read_Field.out_obj};
					Xform {
						ocenter = {163.,231.5,0.};
						dcenter = {2.81641,4.,0.};
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								option {
									label => "Read_Field Output";
								};
							};
						};
					};
				};
			};
		};
		GDM.Uviewer2D Uviewer2D_Convert {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Convert2TriStrip.out_obj};
					Xform {
						ocenter = {163.,231.5,0.};
						dcenter = {2.81641,4.,0.};
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								option {
									label => "Convert2TriStrip Output";
								};
							};
						};
					};
				};
			};
		};
	};
};

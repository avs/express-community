
flibrary ExtractVectorApps {

	APPS.SingleWindowApp ExtractVectorEg<NEdisplayMode="maximized"> {
		UI {
			Modules {
				IUI {
					optionList {
						cmdList => {

							<-.<-.<-.<-.extract_vector.ExtractVectorUI.UImod_panel.option,
							<-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
							<-.<-.<-.<-.glyph.UIpanel.option,
							<-.<-.<-.<-.slice_plane.UIpanel.option,
							<-.<-.<-.<-.bounds.UIpanel.option
						};
						selectedItem = 0;
					};
				};
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.glyph.out_obj,<-.<-.<-.bounds.out_obj};
					Xform {
						ocenter = {5.24261,4.74615,3.52071};
						dcenter = {1.65557,1.63558,1.10819};
						mat = {
							0.263872,0.116171,0.048164,0.,
							-0.125546,0.249865,0.0851451,0.,
							-0.00733167,-0.0975488,0.275453,0.,
							0.,0.,0.,1.
						};
						xlate = {-1.5999,-1.44491,-1.10819};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/bluntfin.fld";
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		XP_COMM_PROJ.ExtractVector.ExtractVectorMacs.extract_vector extract_vector {
			in => <-.Read_Field.field;
		};
		MODS.slice_plane slice_plane {
			in_field => <-.extract_vector.out;
			DVcell_data_labels {
				labels[];
			};
			DVnode_data_labels {
				labels[];
			};
		};
		GEOMS.Arrow1 Arrow1;
		MODS.glyph glyph {
			in_field => <-.slice_plane.out_fld;
			in_glyph => <-.Arrow1.out_fld;
		};
	};
};

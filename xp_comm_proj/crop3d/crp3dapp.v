flibrary Crop3DApps {
	APPS.SingleWindowApp Crop3DEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.external_edges.out_obj,
						<-.<-.<-.extract_cell_component.out_obj};
					Xform {
						dcenter = {-20.3031,-6.17075, -24.723};
						mat = {
							0.244376,0.0332963,-0.0682781,0.,
							0.0235186,0.18555,0.174665,0.,
							0.0722302,-0.173067,0.174126,0.,
							0.,0.,0.,1.
						};
						xlate = {20.3031,6.17075, 24.723};
						ocenter = {11.,3.5,15.};
					};
				};
				Lights {
					Lights = {
						{
							type="BiDirectional"
						},,,};
				};
			};
		};
		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				file_browser {
					ok = 1;
				};
				filename = "$XP_PATH<0>/data/ucd/avs_cell.inp";
			};
		};
		MODS.external_edges external_edges {
			in_field => <-.Read_UCD.field;
		};
		MODS.extract_cell_component extract_cell_component {
			in_field => <-.Crop3D.out;
			ExtrCompParam {
				component = 2;
			};
			ExtrCompUI {
				DVcell_data_labels {
					labels[];
				};
				UIradioBoxLabel {
					label_cmd {
						cmd[3];
					};
				};
			};
		};
		XP_COMM_PROJ.Crop3D.Crop3DMacs.Crop3D Crop3D{
			in => <-.Read_UCD.field;
			params {
				xmin = 0.63;
				xmax = 20.42;
				ymax = 7.49;
				zmax = 27.8;
			};
		};
	};
};

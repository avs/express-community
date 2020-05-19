flibrary Axes3DApps <compile_subs=0> {
	APPS.SingleWindowApp Axes3DEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.Axes3D.obj,<-.<-.<-.Read_Geom.geom};
					Xform {
						ocenter = {0.,-0.25,-0.25};
						dcenter = {0.,-0.285714,-0.285714};
						mat = {
							0.365582,-0.45033,0.98472,0.,
							1.04384,0.422891,-0.194134,0.,
							-0.287879,0.961504,0.546588,0.,
							0.,0.,0.,1.
						};
						xlate = {0.,0.285714,0.285714};
					};
				};
				View {
					View {
						trigger = 1;
					};
				};
			};
		};
		IAC.MISC.Axes3D Axes3D {
			infld => <-.Read_Geom.field;
		};
		MODS.Read_Geom Read_Geom {
			read_geom_ui {
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/geom/*.geo";
				};
				filename = "$XP_PATH<0>/data/geom/teapot.geo";
			};
		};
	};

	APPS.SingleWindowApp Axes3DEg2<NEdisplayMode="maximized"> {
		UI {
			Modules {
				IUI {
					optionList {
						cmdList => {
							<-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
							<-.<-.<-.<-.downsize.panel.option,
							<-.<-.<-.<-.surf_plot.UIpanel.option,
							<-.<-.<-.<-.Axes3D.Axes3DUI.UImod_panel.option};
						selectedItem = 3;
					};
				};
			};
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.surf_plot.out_obj,<-.<-.<-.Axes3D.obj};
					Xform {
						ocenter = {166.019,242.092,74.9238};
						dcenter = {2.72687,3.99231,1.2473};
						mat = {
							0.01442,-0.00359103,0.00557123,0.,
							0.00662826,0.00777563,-0.012144,0.,
							1.82427e-005,0.0133609,0.00856479,0.,
							0.,0.,0.,1.
						};
						xlate = {-2.72687,-3.99231,-1.22001};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
				};
				filename = "$XP_PATH<0>/data/field/helens1.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.downsize downsize {
			in_field => <-.Read_Field.field;
			DownsizeParam {
				factor0 = 3.;
				factor1 = 3.;
			};
		};
		MODS.surf_plot surf_plot {
			in_field => <-.downsize.out_fld;
			SurfPlotParam {
				nspace => 2;
				scale = 0.05;
			};
			SurfPlotUI {
				DVnode_data_labels {
					labels[];
				};
				UIradioBoxLabel {
					label_cmd {
						cmd[];
					};
				};
			};
		};
		XP_COMM_PROJ.Axes3D.Axes3DMacs.Axes3D Axes3D {
			infld => <-.surf_plot.out_fld;
			params {
				min_vals => {0,0,
					<-.infld.node_data[0].min};
				max_vals => {
					(<-.infld.dims[0] - 1),(<-.infld.dims[1] - 1),
					<-.infld.node_data[0].max};
			};
		};
	};
};

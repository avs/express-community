
flibrary OrthogonalViewerApps {
	APPS.SingleWindowApp OrthogonalViewerSingleEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
						set = 1;
					};
				};
				file_browser {
					x = 556;
					y = 304;
					width = 300;
					height = 388;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
		};

		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.OrthogonalViewer OrthogonalViewer {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
				};
			};
			in_field => {<-.Read_Field.field,<-.isosurface.out_fld};
			params {
				showPlane = {1,1,1};
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
	};

	APPS.MultiWindowApp OrthogonalViewerMultiEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
						set = 1;
					};
				};
				file_browser {
					x = 556;
					y = 304;
					width = 300;
					height = 388;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.OrthogonalViewer OrthogonalViewer {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
				};
			};
			in_field => {<-.Read_Field.field,<-.isosurface.out_fld};
			params {
				showPlane = {1,1,1};
			};
		};

		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
	};
APPS.SingleWindowApp OrthogonalViewerSingleEg2<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
						set = 1;
					};
				};
				file_browser {
					x = 556;
					y = 304;
					width = 300;
					height = 388;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
		};

		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.OrthogonalViewer2 OrthogonalViewer2 {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
				};
			};
			in_objs => {<-.Read_Field.out_obj,<-.isosurface.out_obj};
			params {
				showPlane = {1,1,1};
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
	};

	APPS.MultiWindowApp OrthogonalViewerMultiEg2<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				panel {
					option {
						set = 1;
					};
				};
				file_browser {
					x = 556;
					y = 304;
					width = 300;
					height = 388;
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
		};
		XP_COMM_PROJ.OrthogonalViewer.OrthogonalViewerMacs.OrthogonalViewer2 OrthogonalViewer2 {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
				};
			};
			in_objs => {<-.Read_Field.out_obj,<-.isosurface.out_obj};
			params {
				showPlane = {1,1,1};
			};
		};

		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
	};
};

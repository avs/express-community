
flibrary VolumeIntegralApps <compile_subs=0> {

	APPS.MultiWindowApp VolumeIntegralEg<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
					Props {
						material = {0.3, 0.7, 0.9, 13.25};
					};
					Xform {
						ocenter = {12., 12., 15.5};
						dcenter = {4., 4., 5.16667};
						mat = {
							0.206436, 0.0523235, -0.256432, 0.,
							-0.0018648, 0.32689, 0.0651983, 0.,
							0.26171, -0.0389432, 0.202738, 0.,
							0., 0., 0., 1.
						};
						xlate = {-3.27631, -4.09868, -5.16667};
					};
				};
				Lights {
					LightXform = {
						{mat=
							{
								0.418148, 0.409964, 0.810604, 0.,
								0.357332, 0.746185, -0.561712, 0.,
								-0.835144, 0.524534, 0.165523, 0.,
								0., 0., 0., 1.
							},},
						,,};
				};
				Camera {
					Camera {
						auto_norm = "None";
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								panel {
									defaultWidth = 372;
									defaultHeight = 335;
									defaultX = 343;
									defaultY = 289;
								};
							};
						};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/water.fld";
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
			IsoParam {
				iso_level => 0.27;
			};
		};
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMacs.VolumeIntegral VolumeIntegral {
			in => <-.isosurface.out_fld;
		};

		double+Port AREA => VolumeIntegral.area;
		double+Port VOLUME => VolumeIntegral.volume;

	}; // end of example 1

	// Example 2

	APPS.MultiWindowApp VolumeIntegralEg2<NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.external_faces.out_obj};
					Props {
						material = {0.3,0.7,0.9, 13.25};
					};
				};
				View {
					ViewUI {
						ViewPanel {
							UI {
								panel {
									defaultX = 62;
									defaultY = 322;
									defaultWidth = 372;
									defaultHeight = 335;
								};
							};
						};
					};
				};
			};
		};
		XP_COMM_PROJ.VolumeIntegral.VolumeIntegralMacs.VolumeIntegral VolumeIntegral {
			in => <-.external_faces.out_fld;
		};
		double+Port AREA => .VolumeIntegral.area;
		double+Port VOLUME => .VolumeIntegral.volume;
		MODS.Read_UCD Read_UCD {
			read_ucd_ui {
				filename = "$XP_PATH<0>/data/ucd/hex.inp";
			};
		};
		MODS.external_faces external_faces {
			in_field => <-.Read_UCD.field;
		};
	}; // end of example 2

}; // end of lib

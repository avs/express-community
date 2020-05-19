flibrary CutPolylinesApps {
	APPS.SingleWindowApp CutPolylinesEg {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.cut_plane.out_obj_cut,
						<-.<-.<-.CutPolylines.obj
					};
					Xform {
						ocenter = {59.5,59.5,23.0028};
						dcenter = {3.99628,4.01257,1.73037};
						mat = {
							-0.0488468,0.00884551,0.0198862,0.,
							0.0160294,-0.0184313,0.0475716,0.,
							0.0147229,0.0494142,0.0141842,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.0036,-4.03714,-1.73743};
					};
				};
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
		MODS.cut_plane+OPort cut_plane {
			in_field => <-.isosurface.out_fld;
			CutParam {
				dist = -3.24;
			};
		};
		XP_COMM_PROJ.CutPolylines.CutPolylinesMacs.CutPolylines CutPolylines {
			in_cut+nres => <-.cut_plane;
			in_polyline => <-.bounds.out_fld;
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
	};
};

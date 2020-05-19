
flibrary slice_orthoplane_ARRapps {
	APPS.SingleWindowApp slice_orthoplane_ARREg {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.slice_orthoplane_ARR.out_obj,
						<-.<-.<-.bounds.out_obj,
						<-.<-.<-.slice_orthoplane_ARR.out_obj,
						<-.<-.<-.bounds#1.out_obj};
					Xform {
						mat = {
							0.10741,0.000357806,0.0147121,0.,
							-0.00307484,0.106535,0.0198576,0.,
							-0.0143917,-0.020091,0.10556,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-4.};
						ocenter = {31.5,31.5,31.5};
						dcenter = {4.,4.,4.};
						center = {31.5,31.5,31.5};
					};
				};
			};
		};
		XP_COMM_PROJ.slice_orthoplane_ARR.slice_orthoplane_ARRmacs.slice_orthoplane_ARR slice_orthoplane_ARR {
			Slice_OrthoPlane_Arr_Params {
				dist => 28.49;
				axis = 2;
			};
			in => {
				<-.isosurface.out_fld,<-.crop.out_fld};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.Read_Field.field;
		};
		MODS.crop crop {
			in_field => <-.Read_Field.field;
			CropParam {
				min = {18,0,0};
				max = {63,35,63};
			};
		};
		MODS.bounds bounds#1 {
			in_field => <-.crop.out_fld;
		};
	};
};
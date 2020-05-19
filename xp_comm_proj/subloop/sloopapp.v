
flibrary SubLoopApps{
	APPS.SingleWindowApp SubLoopEg <NEdisplayMode="maximized"> {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.bounds.out_obj,<-.<-.<-.orthoslice_x.out_obj,
						<-.<-.<-.orthoslice_z.out_obj};
					Xform {
						ocenter = {59.5,59.5,16.5};
						dcenter = {4.,4.,1.10924};
						mat = {
							0.0515233,0.0120882,-0.0189577,0.,
							-0.0214614,0.0405721,-0.0324574,0.,
							0.00670282,0.0369859,0.0418007,0.,
							0.,0.,0.,1.
						};
						xlate = {-4.,-4.,-1.10924};
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
				file_browser {
					ok = 1;
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/lobster.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		int fld_dims[] => Read_Field.field.dims;
		MODS.Loop Loop {
			end => <-.fld_dims[0] - 1;
			incr = 1.;
		};
		XP_COMM_PROJ.SubLoop.SubLoopMacs.SubLoop SubLoop {
			trigger => <-.Loop.count;
			end => <-.fld_dims[2] - 1;
			incr = 1.;
			params {
				run = 1;
				cycle = 2;
			};
		};
		MODS.orthoslice orthoslice_x {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 0;
				plane => <-.<-.Loop.count;
			};
		};
		MODS.orthoslice orthoslice_z {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
				plane => <-.<-.SubLoop.count;
			};
		};
	};
};

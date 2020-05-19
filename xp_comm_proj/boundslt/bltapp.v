
flibrary BoundsLiteApps {
	APPS.SingleWindowApp BoundsLiteEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			DVread_field {
				Mesh_Struct+Node_Data Output_Field;
			};
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/bluntfin.fld";
			};
			DataObject {
				Props {
					trans = 0.3600000143;
					inherit = 0;
					material = {0.42,1.,0.17,1.};
				};
				Modes {
					mode = {0,0,4,0,1};
					normals = "None";
				};
			};
		};
		XP_COMM_PROJ.BoundsLite.BoundsLiteMacs.BoundsLite BoundsLite {
      		in_field => <-.Read_Field.field;
			BoundsLiteUI {
				UImod_panel {
					option {
						set = 1;
					};
				};
			};
			params {
				hull = 1;
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.BoundsLite.out_obj,<-.<-.<-.bounds.out_obj,
						<-.<-.<-.Read_Field.out_obj};
					Xform {
						ocenter = {3.27323,4.16378,2.86213};
						dcenter = {1.18071,1.50195,1.03242};
						mat = {
							0.297051,0.0789263,-0.199961,0.,
							0.0226098,0.327708,0.162937,0.,
							0.213782,-0.144327,0.260614,0.,
							0.,0.,0.,1.
						};
						xlate = {-1.18071,-1.50195,-1.03242};
					};
				};
				Lights {
					Lights = {
						{
							type="BiDirectional"
						},,,};
				};
				Camera {
					Camera {
						auto_norm = "None";
					};
				};
			};
			Scene_Editor {
				Object_Editor {
					GDmodes_edit {
						surf = "Gouraud";
						volume = "Inherit";
						bounds = "None";
						normals = "None";
					};
					GDprops_edit {
						inherit = 0;
						metal = 0.;
						ambient = 0.42;
						diffuse = 1.;
						specular = 0.17;
						spec_exp = 1.;
						trans = 0.36;
						cull = "Normal";
						voxel_interp = "Trilinear";
						ray_algo = "Direct Composite";
						ray_norm = "Global";
					};
				};
			};
		};
	};
};



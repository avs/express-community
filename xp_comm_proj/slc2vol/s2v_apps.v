flibrary Slice_to_Volume_Apps<compile_subs=0>
{

	APPS.SingleWindowApp Slice_to_Volume_Eg {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {<-.<-.<-.cut_plane.out_obj_cut,<-.<-.<-.bounds.out_obj};
					Xform {
						ocenter = {47.545,31.5,31.5};
						dcenter = {6.03746,4.,4.};
						mat = { 0.00977105,-0.0367689,-0.121151,0.,
							  0.0315542,0.118384,-0.0333843,0.,
							0.122612,-0.0275359,0.018246,0.,
							0.,0.,0.,1.
						};
						xlate = {-6.03746,-4.,-4.};
						center = {47.545,31.5,31.5};
					};
				};
			};
		};
		XP_COMM_PROJ.Slice_to_Volume.Slice_to_Volume_Macs.Slice_to_Volume Slice_to_Volume {
			In_Slice => <-.orthoslice.out_fld;
			Ref_Volume => <-.Read_Field.field;
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
			DVread_field {
				Mesh_Unif+Node_Data Output_Field;
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis => 2;
				plane => 43;
			};
		};
		MODS.cut_plane cut_plane {
			in_field => <-.Slice_to_Volume.out;
			CutParam {
				dist = 32.0;
			};
			plane {
				xform {
					mat = {
						0.,0.,-1.,0.,0.,1.,0.,0.,1.,0.,0.,0.,0.,0.,0.,1.
					};
				};
			};
			CutUI {
				plane_xform {
					GDxform_editor {
						y_rot = 90.;
						abs_y_rot = 90.;
					};
				};
			};
		};
		MODS.bounds bounds {
			in_field => <-.Read_Field.field;
		};
	};

};

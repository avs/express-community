flibrary ImageCompareApps <compile_subs=0>{
	APPS.SingleWindowApp ImageCompareEg<NEdisplayMode="maximized"> {
		GDM.Uviewer2D Uviewer2D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.ImageCompare.obj};
				};
			};
		};
		XP_COMM_PROJ.ImageCompare.ImageCompareMacs.ImageCompare ImageCompare {
			inImage1 => <-.orthoslice.out_fld;
			inDatamap1 => <-.Read_Field.DataObject.Datamap;
			inImage2 => <-.orthoslice#1.out_fld;
			inDatamap2 => <-.Read_Field.DataObject.Datamap;
			params {
				Mode = 3;
				Switch = 1;
				Value = 0.5;
			};
		};
		MODS.Read_Field Read_Field {
			read_field_ui {
				file_browser {
					ok = 1;
				};
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 1;
				plane => 26;
			};
		};
		MODS.orthoslice orthoslice#1 {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 1;
				plane => 13;
			};
		};
	};
};

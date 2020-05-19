flibrary CB_ColorMapApps {
	APPS.MultiWindowApp CB_ColorMapEg<NEdisplayMode="maximized"> {
		MODS.Read_Field Read_Field {
			read_field_ui {
				filename = "$XP_PATH<0>/data/field/hydrogen.fld";
			};
		};
		MODS.orthoslice orthoslice {
			in_field => <-.Read_Field.field;
			OrthoSliceParam {
				axis = 2;
				plane => 32;
			};
		};
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMacs.CB_ColorMap CB_ColorMap {
			CB_ColorMapParams {
				map_type = "No of Colors";
				catidx = 1;
				mapidx = 3;
			};
		};
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMacs.DataObjectDmap DataObjectDmap {
			in => <-.orthoslice.out_fld;
			Datamap => <-.CB_ColorMap.CB_Datamap;
			AltObject<instanced=0>;
		};
		GDM.Uviewer Uviewer {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.DataObjectDmap.obj};
				};
			};
		};
	};
};

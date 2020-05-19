flibrary DownSampleApps{
	APPS.SingleWindowApp DownSampleEg1 {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
						<-.<-.<-.DownSample.out_obj};
					};
				};
			};
		MODS.Read_Field Read_Field;
		XP_COMM_PROJ.DownSample.DownSampleMacs.DownSample DownSample {
			in => <-.Read_Field.field;
		};
	};

	APPS.SingleWindowApp DownSampleEg2 {
		GDM.Uviewer3D Uviewer3D {
			Scene {
				Top {
					child_objs => {
					<-.<-.<-.isosurface.out_obj};
				};
			};
		};
		MODS.Read_Field Read_Field;

		XP_COMM_PROJ.DownSample.DownSampleMacs.DownSample DownSample {
			in => <-.Read_Field.field;
		};
		MODS.isosurface isosurface {
			in_field => <-.DownSample.out_field;
		};
	};

};

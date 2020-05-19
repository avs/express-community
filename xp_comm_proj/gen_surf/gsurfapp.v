flibrary GenerateSurfaceApps{
	APPS.MultiWindowApp GenerateSurfaceEg{
	   GDM.Uviewer3D Uviewer3D {
		  Scene {
			 Top {
				child_objs => {
				   <-.<-.<-.external_faces.out_obj,<-.<-.<-.orthoslice.out_obj};
				Xform {
				   ocenter = {12.,12.,21.1141};
				   dcenter = {2.85159,4.33301,2.61503};
				   mat = {0.111109,-0.0823054,0.113721,0.,
				          0.137739,0.0919189,-0.0680496,0.,
				          -0.0271033,0.129725,0.120369,0.,
				          0.,0.,0.,1.
				   };
				   xlate = {-2.85159,-4.33301,-2.61503};
				};
			 };
		  };
	   };
	   XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMacs.GenerateSurface GenerateSurface {
		  in => <-.extract_component.out_fld;
		  Params {
			 threshold = 3.01;
		  };
	   };
	   MODS.Read_Field Read_Field {
		  read_field_ui {
			 file_browser {
				ok = 1;
			 };
			 filename = "$XP_PATH<0>/data/field/wind.fld";
		  };
		  DVread_field {
			 Mesh_Unif+Node_Data Output_Field;
		  };
	   };
	   MODS.external_faces external_faces {
		  in_field => <-.GenerateSurface.out;
	   };
	   MODS.orthoslice orthoslice {
		  in_field => <-.Read_Field.field;
		  OrthoSliceParam {
			 axis = 2;
			 plane => 13;
		  };
	   };
	   MODS.extract_component extract_component {
		  in_field => <-.orthoslice.out_fld;
	   };
	};
};

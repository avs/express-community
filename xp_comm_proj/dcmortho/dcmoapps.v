library DICOM_Orthoslice_Apps {
   APPS.MultiWindowApp DICOM_OrthosliceEg {
      GDM.Uviewer3D Uviewer3D<NEx=603,NEy=453> {
	 Scene {
	    Top {
	       child_objs => {
	       <-.<-.<-.isosurface.out_obj};
	    };
	    View {
	       View {
		  trigger = 2;
	       };
	       ViewUI {
		  ViewPanel {
		     UI {
			panel {
			   defaultX = 200;
			   defaultY = 0;
			};
		     };
		  };
	       };
	    };
	 };
	 Scene_Selector {
	    input_views => {<-.Scene.View.View,
	    <-.Sagittal.View.View,<-.Coronal.View.View,<-.Axial.View.View};
	 };
	 GDM.Uscene2D Sagittal {
	    Top {
	       child_objs<NEportLevels={4,1}> => {
	       <-.<-.<-.reset_xform.out_obj};
	    };
	    View {
	       View {
		  trigger = 2;
	       };
	       ViewUI {
		  ViewPanel {
		     UI {
			panel {
			   defaultX = 200;
			   defaultY = 500;
			};
		     };
		  };
	       };
	    };
	 };
	 GDM.Uscene2D Coronal {
	    Top {
	       child_objs<NEportLevels={4,1}> => {
	       <-.<-.<-.reset_xform#1.out_obj};
	    };
	    View {
	       View {
		  trigger = 2;
	       };
	       ViewUI {
		  ViewPanel {
		     UI {
			panel {
			   defaultX = 700;
			   defaultY = 0;
			};
		     };
		  };
	       };
	    };
	 };
	 GDM.Uscene2D Axial {
	    Top {
	       child_objs<NEportLevels={4,1}> => {
	       <-.<-.<-.Axial_Slice.obj};
	    };
	    View {
	       View {
		  trigger = 2;
	       };
	       ViewUI {
		  ViewPanel {
		     UI {
			panel {
			   defaultX = 700;
			   defaultY = 500;
			};
		     };
		  };
	       };
	    };
	 };
      };
      XP_COMM_PROJ.ReadDICOM.ReadDICOM ReadDICOM<NEx=66.,NEy=33.>;
      XP_COMM_PROJ.DICOMOrtho.DICOM_Orthoslice Sagittal_Slice<NEx=231.,NEy=165.> {
	 in => <-.ReadDICOM.out;
      };
      XP_COMM_PROJ.DICOMOrtho.DICOM_Orthoslice Coronal_Slice<NEx=385.,NEy=165.> {
	 in => <-.ReadDICOM.out;
	 DICOM_Orthoslice_Unif {
	    DICOMOrthoParams {
	       axis = 1;
	    };
	 };
      };
      XP_COMM_PROJ.DICOMOrtho.DICOM_Orthoslice Axial_Slice<NEx=572.,NEy=165.> {
	 in => <-.ReadDICOM.out;
	 DICOM_Orthoslice_Unif {
	    DICOMOrthoParams {
	       axis = 2;
	    };
	 };
	 DICOM_Orthoslice_UI {
	    DICOM_Orthoslice_UI {
	       option {
		  set = 1;
	       };
	    };
	 };
      };
      MODS.reset_xform reset_xform<NEx=231.,NEy=231.> {
	 in_field => <-.Sagittal_Slice.out;
      };
      MODS.reset_xform reset_xform#1<NEx=385.,NEy=231.> {
	 in_field => <-.Coronal_Slice.out;
      };
      MODS.downsize downsize<NEx=77.,NEy=165.> {
	 in_field => <-.ReadDICOM.out;
	 DownsizeParam {
	    factor0 = 4.;
	    factor1 = 4.;
	    factor2 = 2.;
	 };
      };
      MODS.isosurface isosurface<NEx=77.,NEy=231.> {
	 in_field => <-.downsize.out_fld;
	 DVcell_data_labels {
	    labels[];
	 };
      };
   };
};

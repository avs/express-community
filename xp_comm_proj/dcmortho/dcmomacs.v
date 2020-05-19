library DICOMOrthoMacs {
   group DICOMOrthoParams<NEportLevels={0,2}> {
      int axis<NEportLevels={2,2}>;
      int plane<NEportLevels={2,2}>;
   };
   macro DICOM_Orthoslice_Unif {
      ilink in;
      DICOMOrthoParams DICOMOrthoParams;
      XP_COMM_PROJ.DICOMOrtho.DICOM_orthoslice_unif DICOM_orthoslice_unif {
	 in => <-.in;
	 axis => <-.DICOMOrthoParams.axis;
	 plane => <-.DICOMOrthoParams.plane;
      };
      olink out => DICOM_orthoslice_unif.out;
   };
   macro DICOM_Orthoslice_UI {
      Mesh_Unif &in;
      DICOMOrthoParams &DICOMOrthoParams;
      UImod_panel DICOM_Orthoslice_UI {
	 title ="DICOM_Orthoslice";
      };
      UIslider UIaxis {
	 x = 2;
	 y = 2;
	 parent => <-.DICOM_Orthoslice_UI;
	 width => parent.clientWidth -4;
	 min = 0;
	 max => <-.in.ndim -1;
	 mode = "integer";
	 title = "Axis";
	 value => <-.DICOMOrthoParams.axis;
      };
      UIslider UIplanes {
	 x = 2;
	 y => <-.UIaxis.y + <-.UIaxis.height +2;
	 parent => <-.DICOM_Orthoslice_UI;
	 width => parent.clientWidth -4;
	 min = 0;
	 max => <-.in.dims[<-.DICOMOrthoParams.axis] -1;
	 mode = "integer";
	 title = "Plane";
	 value => <-.DICOMOrthoParams.plane;
      };
   };
   macro DICOM_Orthoslice {
      ilink in;
      DICOM_Orthoslice_Unif DICOM_Orthoslice_Unif{
	 in => <-.in;
      };
      GDM.DataObject DataObject {
	 in => <-.DICOM_Orthoslice_Unif.out;
      };
      DICOM_Orthoslice_UI DICOM_Orthoslice_UI {
	 in => <-.in;
	 DICOMOrthoParams => <-.DICOM_Orthoslice_Unif.DICOMOrthoParams;
	 DICOM_Orthoslice_UI {
	    title => name_of(<-.<-.<-);
	 };
      }; 
      olink out => DICOM_Orthoslice_Unif.out;
      olink obj => DataObject.obj;
   };
};

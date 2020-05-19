
library ReadDICOMData {
   // Patient
   //   Data structure containing patient information extracted from DICOM file.
   group Patient {
      string PatientsName;
      string PatientID;
      string PatientsBirthDate;
      string PatientsBirthTime;
      string PatientsSex;
      string ReferencedPatientSequence;
      string OtherPatientID;
      string OtherPatientNames;
      string EthnicGroup;
      string PatientComments;
   };

   // General_Study
   //   Data structure containing general study information extracted from DICOM file.
   group General_Study {
      string StudyInstanceUID;
      string StudyDate;
      string StudyTime;
      string ReferringPhysiciansName;
      string StudyID;
      string AccessionNumber;
      string StudyDescription;
      string NameofPhysiciansReadingStudy;
      string ReferencedStudySequence;
   };

   // General_Series
   //   Data structure containing general series information extracted from DICOM file.
   group General_Series {
      string Modality;
      string SeriesInstanceUID;
      string SeriesNumber;
      string Laterality;
      string SeriesDate;
      string SeriesTime;
      string PerformingPhysiciansName;
      string ProtocolName;
      string SeriesDescription;
      string OperatorsName;
      string ReferencedStudyComponentSequence;
      string BodyPartExamined;
      string PatientPosition;
      int    SmallestPixelValueinSeries;
      int    LargestPixelValueinSeries;
   };

   // Image_Series
   //   Data structure containing information pertinent to the use of the data
   group Image_Series {
      string Interpretation;
      int Rows;
      int Columns;
      int Slices; // actual # read in
      int BitsAllocated;
      float SliceSpacing;
      float XSpacing;
      float YSpacing;
      float WindowCentre;
      float WindowWidth;
   };
   
   // General_Equipment
   //   Data structure containing general equipment information extracted from DICOM file.
   group General_Equipment {
      string Manufacturer;
      string InstitutionName;
      string InstitutionAddress;
      string StationName;
      string InstitutionalDepartmentName;
      string ManufacturersModelName;
      string DeviceSerialNumber;
      string SoftwareVersions;
      string SpatialResolution;
      string DateofLastCalibration;
      string TimeofLastCalibration;
      int    PixelPaddingValue;
   };


   // DICOM_Header
   //   data structure that groups the previously mentioned data structures together.
   //   This data contained within this structure is extracted from the DICOM file by
   //   the ReadDicomFile module.
   group DICOM_Header {
      Patient+OPort2 patient;
      General_Study+OPort2 generalStudy;
      General_Series+OPort2 generalSeries;
      General_Equipment+OPort2 generalEquipment;
      Image_Series+OPort2 imageSeries;
   };


   // DICOM_Volume
   //   modified uniform field stucture.
   //   used to contain data generated by the ReadDicomFile module.
   Field_Unif DICOM_Volume {
      ndim = 3;
      nspace = 3;
      nnode_data = 1;
   };

};

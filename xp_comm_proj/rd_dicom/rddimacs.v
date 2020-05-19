
library ReadDICOMMacs {
   macro ReadDICOMPatientUI {
      int visible = 0;
      UIshell PatientShell {
	 visible => <-.visible;
	 height = 400;
	 showStatusBar = 0;
	 title = "Patient Information";
      };
      UIpanel panel {
	 x = 0;
	 y = 0;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 parent => <-.PatientShell;
      };
      group &patient<NEportLevels={2,0}>;
      UIlabel Patient {
	 y = 10;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Patient Information";
      };
      UItext Info {
	 parent => <-.panel;
	 y => <-.Patient.y + <-.Patient.height + 4;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 outputOnly = 1;
         multiLine = 1;
	 text => str_format(
	    "Name : %s\nIDP : %s\nBirth Date : %s\nBirth Time : %s\nSex : %s\nReferenced Seq : %s\nOther ID : %s\nOther Names : %s\nEthnic Group : %s\nComments : %s",
	    switch(is_valid(<-.patient.PatientsName)+1,"",<-.patient.PatientsName),
	    switch(is_valid(<-.patient.PatientID)+1,"",<-.patient.PatientID),
	    switch(is_valid(<-.patient.PatientsBirthDate)+1,"",<-.patient.PatientsBirthDate),
	    switch(is_valid(<-.patient.PatientsBirthTime)+1,"",<-.patient.PatientsBirthTime),
	    switch(is_valid(<-.patient.PatientsSex)+1,"",<-.patient.PatientsSex),
	    switch(is_valid(<-.patient.ReferencedPatientSequence)+1,"",<-.patient.ReferencedPatientSequence),
	    switch(is_valid(<-.patient.OtherPatientID)+1,"",<-.patient.OtherPatientID),
	    switch(is_valid(<-.patient.OtherPatientNames)+1,"",<-.patient.OtherPatientNames),
	    switch(is_valid(<-.patient.EthnicGroup)+1,"",<-.patient.EthnicGroup),
	    switch(is_valid(<-.patient.PatientComments)+1,"",<-.patient.PatientComments)
	 );
      };
   };
   macro ReadDICOMStudyUI {
      int visible = 0;
      UIshell StudyShell {
	 visible => <-.visible;
	 height = 400;
	 showStatusBar = 0;
	 title = "Study Information";
      };
      UIpanel panel {
	 x = 0;
	 y = 0;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 parent => <-.StudyShell;
      };
      group &generalStudy<NEportLevels={2,0}>;
      UIlabel GeneralStudy {
	 y = 10;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Study Information";
      };
      UItext Info {
	 parent => <-.panel;
	 y => <-.Patient.y + <-.Patient.height + 4;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 outputOnly = 1;
         multiLine = 1;
	 text => str_format(
	    "Instance UID : %s\nDate : %s\nTime : %s\nReferring Phys Name : %s\nID : %s\nAccession Number : %s\nDescription : %s\nReading Phys Name : %s\nReferenced Seq : %s",
	    switch(is_valid(<-.generalStudy.StudyInstanceUID)+1,"",<-.generalStudy.StudyInstanceUID),
	    switch(is_valid(<-.generalStudy.StudyDate)+1,"",<-.generalStudy.StudyDate),
	    switch(is_valid(<-.generalStudy.StudyTime)+1,"",<-.generalStudy.StudyTime),
	    switch(is_valid(<-.generalStudy.ReferringPhysiciansName)+1,"",<-.generalStudy.ReferringPhysiciansName),
	    switch(is_valid(<-.generalStudy.StudyID)+1,"",<-.generalStudy.StudyID),
	    switch(is_valid(<-.generalStudy.AccessionNumber)+1,"",<-.generalStudy.AccessionNumber),
	    switch(is_valid(<-.generalStudy.StudyDescription)+1,"",<-.generalStudy.StudyDescription),
	    switch(is_valid(<-.generalStudy.NameofPhysiciansReadingStudy)+1,"",<-.generalStudy.NameofPhysiciansReadingStudy),
	    switch(is_valid(<-.generalStudy.ReferencedStudySequence)+1,"",<-.generalStudy.ReferencedStudySequence)
	 );    
      };
   };
   
   macro ReadDICOMSeriesUI {
      int visible = 0;
      UIshell SeriesShell {
	 visible => <-.visible;
	 height = 400;
	 showStatusBar = 0;
	 title = "Series Information";
      };
      UIpanel panel {
	 x = 0;
	 y = 0;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 parent => <-.SeriesShell;
      };
      group &generalSeries<NEportLevels={2,0}>;
      UIlabel GeneralSeries {
	 y = 10;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Series Information";
      };
      UItext Info {
	 parent => <-.panel;
	 y => <-.GeneralSeries.y + <-.GeneralSeries.height + 4;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 outputOnly = 1;
         multiLine = 1;
	 text => str_format(
	    "Modality : %s\nInstance UID : %s\nNumber : %s\nLaterality : %s\nDate : %s\nTime : %s\nPerf Phys Name : %s\nProtocol Name : %s\nDescription : %s\nOperator's Name : %s\nRef Study Comp Seq : %s\nBody Part Examined : %s\nPatient Position : %s\nSmallest Pixel Value : %s\nLargest Pixel Value : %s",
	    switch(is_valid(<-.generalSeries.Modality)+1,"",<-.generalSeries.Modality),
	    switch(is_valid(<-.generalSeries.SeriesInstanceUID)+1,"",<-.generalSeries.SeriesInstanceUID),
	    switch(is_valid(<-.generalSeries.SeriesNumber)+1,"",<-.generalSeries.SeriesNumber),
	    switch(is_valid(<-.generalSeries.Laterality)+1,"",<-.generalSeries.Laterality),
	    switch(is_valid(<-.generalSeries.SeriesDate)+1,"",<-.generalSeries.SeriesDate),
	    switch(is_valid(<-.generalSeries.SeriesTime)+1,"",<-.generalSeries.SeriesTime),
	    switch(is_valid(<-.generalSeries.PerformingPhysiciansName)+1,"",<-.generalSeries.PerformingPhysiciansName),
	    switch(is_valid(<-.generalSeries.ProtocolName)+1,"",<-.generalSeries.ProtocolName),
	    switch(is_valid(<-.generalSeries.SeriesDescription)+1,"",<-.generalSeries.SeriesDescription),
	    switch(is_valid(<-.generalSeries.OperatorsName)+1,"",<-.generalSeries.OperatorsName),
	    switch(is_valid(<-.generalSeries.ReferencedStudyComponentSequence)+1,"",<-.generalSeries.ReferencedStudyComponentSequence),
	    switch(is_valid(<-.generalSeries.BodyPartExamined)+1,"",<-.generalSeries.BodyPartExamined),
	    switch(is_valid(<-.generalSeries.PatientPosition)+1,"",<-.generalSeries.PatientPosition),
	    switch(is_valid(<-.generalSeries.SmallestPixelValueinSeries)+1,"",<-.generalSeries.SmallestPixelValueinSeries),
	    switch(is_valid(<-.generalSeries.LargestPixelValueinSeries)+1,"",<-.generalSeries.LargestPixelValueinSeries)
	 );
      };
   };
   macro ReadDICOMEquipmentUI {
      int visible = 0;
      UIshell EquipmentShell {
	 visible => <-.visible;
	 height = 400;
	 showStatusBar = 0;
	 title = "Equipment Information";
      };
      UIpanel panel {
	 x = 0;
	 y = 0;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 parent => <-.EquipmentShell;
      };
      group &generalEquipment<NEportLevels={2,0}>;
      UIlabel GeneralEquipment {
	 y = 10;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Equipment Information";
      };
      UItext Info {
	 parent => <-.panel;
	 y => <-.GeneralEquipment.y + <-.GeneralEquipment.height + 4;
	 width => parent.clientWidth;
	 height => parent.clientHeight;
	 outputOnly = 1;
         multiLine = 1;
	 text => str_format(
	    "Manufacturer : %s\nInstitution Name : %s\nInstitution Address : %s\nStation Name : %s\nInstitution Dept Name : %s\nManufacturer's Model : %s\nDevice SN : %s\nSoftware Versions : %s\nSpatial Resolution : %s\nLast Calibration Date : %s\nPixel Padding Value : %s",
	    switch(is_valid(<-.generalEquipment.Manufacturer)+1,"",<-.generalEquipment.Manufacturer),
	    switch(is_valid(<-.generalEquipment.InstitutionName)+1,"",<-.generalEquipment.InstitutionName),
	    switch(is_valid(<-.generalEquipment.InstitutionAddress)+1,"",<-.generalEquipment.InstitutionAddress),
	    switch(is_valid(<-.generalEquipment.StationName)+1,"",<-.generalEquipment.StationName),
	    switch(is_valid(<-.generalEquipment.InstitutionalDepartmentName)+1,"",<-.generalEquipment.InstitutionalDepartmentName),
	    switch(is_valid(<-.generalEquipment.ManufacturersModelName)+1,"",<-.generalEquipment.ManufacturersModelName),
	    switch(is_valid(<-.generalEquipment.DeviceSerialNumber)+1,"",<-.generalEquipment.DeviceSerialNumber),
	    switch(is_valid(<-.generalEquipment.SoftwareVersions)+1,"",<-.generalEquipment.SoftwareVersions),
	    switch(is_valid(<-.generalEquipment.SpatialResolution)+1,"",<-.generalEquipment.SpatialResolution),
	    switch(is_valid(<-.generalEquipment.DateofLastCalibration)+1,"",<-.generalEquipment.DateofLastCalibration),
	    switch(is_valid(<-.generalEquipment.TimeofLastCalibration)+1,"",<-.generalEquipment.TimeofLastCalibration),
	    switch(is_valid(<-.generalEquipment.PixelPaddingValue)+1,"",<-.generalEquipment.PixelPaddingValue)
	 );
      };
   };

   macro ReadDCMLISTUI {
      int visible = 0;
#ifdef MSDOS
      string searchPath<NEportLevels={2,1}> = "C:\\";
#else
      string searchPath<NEportLevels={2,1}> = ".";
#endif	 
      int rescan<NEportLevels={2,1}>;
      int vis<NEportLevels={2,1}>;
      macro UIpopWindow{
	 int vis => <-.visible;
	 UIshell shell {
	    x = 100;
	    y = 150;
	    width = 700;
	    height = 300;
	    visible => <-.vis;
	    title => "Choose DICOM series (" + <-.<-.UIdirectory.filename + ")";
	 };
	 UIpanel panel {
	    x = 0;
	    y = 0;
	    width => parent.clientWidth;
	    height=> parent.clientHeight;
	    parent => <-.shell;
	 };
      };
      XP_COMM_PROJ.ReadDICOM.ReadDICOMMods.read_DCMLIST read_DCMLIST {
	 dirname => <-.UIdirectory.filename;
	 rescan => <-.UIrescan.do;
	 delete => <-.UIdelete.do;
	 selected_series => <-.i;
      };
      UIlabel UIheadings {
	 parent => <-.UIpopWindow.panel;
	 label => "Patient Name                     PatientID         StudyID       Date  SeriesID  Date    Images";
	 x = 2;
	 y = 0;
	 width => parent.clientWidth -4;
	 fontAttributes {
	    family = "courier";
	    weight = "bold";
	 };
	 alignment = "left";
      };
      UIlist UIlist {
	 parent => <-.UIpopWindow.panel;
	 y => ((<-.UIheadings.y + UIheadings.height) + 2);
	 width => parent.clientWidth;
	 height => parent.clientHeight - y - UIread.height;
	 strings => <-.read_DCMLIST.series;
	 fontAttributes {
	    family = "courier";
	 };
	 selectedItem<NEportLevels={0,2}>;
      };
      UIbutton UIread {
	 x = 4;
	 y => parent.clientHeight - height;
	 width => parent.clientWidth/4 - 6;
	 parent => <-.UIpopWindow.panel;
	 active => is_valid(<-.i);
	 label = "Read Series";
	 message = "Load the selected series into the application";
      };
      UIbutton UIrescan {
	 x => parent.clientWidth/4 + 2;
	 y => parent.clientHeight - height;
	 width => parent.clientWidth/4 - 2;
	 parent => <-.UIpopWindow.panel;
	 label = "Rescan Directory";
	 message = "Reload the DCMLIST file";
      };
      UIbutton UIchooseDir {
	 x => 2*parent.clientWidth/4 + 2;
	 y => parent.clientHeight - height;
	 width => parent.clientWidth/4 - 2;
	 parent => <-.UIpopWindow.panel;
	 label = "Change Directory";
	 message = "Change the directory in which the DICOM files are stored";
      };
      UIbutton UIdelete {
	 x => 3*parent.clientWidth/4 + 2;
	 y => parent.clientHeight - height;
	 width => parent.clientWidth/4 - 6;
	 parent => <-.UIpopWindow.panel;
	 label = "Delete Series";
	 message = "Delete the seleted series";
#ifdef MSDOS
	 active = 0; // eFilm does the work so you don't have to
#else	    
	 active => is_valid(<-.i);
#endif	    
      };
      UIdirectoryDialog UIdirectory {
	 visible => <-.UIchooseDir.do;
#ifdef MSDOS
	 searchPattern => str_format("%s\\*",<-.searchPath);
#else	    
	 searchPattern => str_format("%s/*",<-.searchPath);
#endif	    
      };
//       GMOD.copy_on_change set_default_directory {
// 	 input => <-.searchPath;
// 	 output => <-.UIdirectory.filename;
//       };
      int i<NEportLevels=1> => .UIlist.selectedItem;
      XP_COMM_PROJ.StrArrSub.string_array_subset string_range {
	 in => <-.read_DCMLIST.filenames;
	 start => read_DCMLIST.series_st_en[i][0];
	 end => read_DCMLIST.series_st_en[i][1];
      };
      string dirname<NEportLevels={0,2}> => UIdirectory.filename;
      string series<NEportLevels={0,2}> => read_DCMLIST.series[i];
      int read<NEportLevels={0,2}> => UIread.do;
      olink filenames => string_range.out;
   };

   macro ReadDICOM_Series {
      UImod_panel panel {
	 height = 1000;
	 message = "Read DICOM Series control panel.";
	 title => "Read DICOM Series";
      };
      UIbutton SelectSeriesUI {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Select Patient Series ";
      };
      ReadDCMLISTUI ReadDCMLISTUI {
	 visible => <-.SelectSeriesUI.do;
      };
      XP_COMM_PROJ.ReadDICOM.ReadDICOMMods.read_dicom_files read_dicom_files {
	 filenames => <-.ReadDCMLISTUI.filenames;
	 trigger => <-.ReadDCMLISTUI.read;
      };
      XP_COMM_PROJ.ReadDICOM.ReadDICOMData.DICOM_Header &Header<NEportLevels={1,0}> => read_dicom_files.header;
      UItext UISeriesInfo {
	 parent => <-.panel;
	 y => <-.SelectSeriesUI.y + <-.SelectSeriesUI.height + 4;
	 width => parent.clientWidth;
	 height => 8 * UIdata.UIfonts[0].lineHeight;
	 outputOnly = 1;
         multiLine = 1;
	 text => switch(is_valid(<-.Header.imageSeries.Columns)+1, "", str_format("Patient: %s\nDate: %s\nDesc: %s\nModality: %s %s\nDims: %d x %d x %d\nSize: %.1f x %.1f x %.1f mm",
	 <-.Header.patient.PatientsName,
	 <-.Header.generalSeries.SeriesDate,
	 <-.Header.generalSeries.SeriesDescription,
	 <-.Header.generalSeries.Modality,
	 <-.Header.generalSeries.ProtocolName,
	 <-.Header.imageSeries.Columns, <-.Header.imageSeries.Rows,
	 <-.Header.imageSeries.Slices,
	 <-.Header.imageSeries.Columns * <-.Header.imageSeries.XSpacing,
	 <-.Header.imageSeries.Rows * <-.Header.imageSeries.YSpacing,
	 <-.Header.imageSeries.Slices * <-.Header.imageSeries.SliceSpacing));
      };
      UIbutton PatientButton {
	 y => <-.UISeriesInfo.y + <-.UISeriesInfo.height + 4;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Patient Information";
      };
      UIbutton StudyButton {
	 y => <-.PatientButton.y + <-.PatientButton.height + 4;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Study Information";
      };
      UIbutton SeriesButton {
	 y => <-.StudyButton.y + <-.StudyButton.height + 4;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = " Series Information";
      };
      UIbutton EquipmentButton {
	 y => <-.SeriesButton.y + <-.SeriesButton.height + 4;
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = " Equipment Information";
      };
      ReadDICOMPatientUI PatientInfo {
	 visible => <-.PatientButton.do;
	 patient => <-.Header.patient;
      };
      ReadDICOMStudyUI StudyInfo {
	 visible => <-.StudyButton.do;
	 generalStudy => <-.Header.generalStudy;
      };
      ReadDICOMSeriesUI SeriesInfo {
	 visible => <-.SeriesButton.do;
	 generalSeries => <-.Header.generalSeries;
      };
      ReadDICOMEquipmentUI EquipmentInfo {
	 visible => <-.EquipmentButton.do;
	 generalEquipment => <-.Header.generalEquipment;
      };
      UIlabel Copyright {
	 width => parent.clientWidth;
	 height => UIdata.UIfonts[0].lineHeight * 4;
         y => EquipmentButton.y + EquipmentButton.height + 8;
	 parent => <-.panel;
	 alignment = "center";
	 label = "Written by James S. Perrin\nUniversity of Manchester\nBased on Read_DICOM(1)\nDCMTK Copyright (C) 1994-2004, OFFIS";
      };
      GDM.DataObject DICOMVolume {
	 in => <-.read_dicom_files.volume;
      };
      olink out => read_dicom_files.volume;
      olink obj => DICOMVolume.obj;
      olink header => Header;
   };

   macro ReadDICOM_File {
      UImod_panel panel {
	 height = 1000;
	 message = "Read DICOM File control panel.";
	 title => "Read DICOM File";
      };

      string dicom_filename;

      UItext file_name {
	 parent => <-.panel;
	 //y => Field_Filename.y + Field_Filename.height + 5;
         y  = 4;
	 text => <-.dicom_filename;
	 width = 170;
	 showLastPosition = 1;
      };
      UIbutton browser_visible {
	 parent => <-.panel;
	 x => file_name.x + file_name.width + 5;
	 y => file_name.y;
	 width = 75;
	 height => <-.file_name.height;
	 label = "Browse...";
      };

      UIfileSB file_browser {
	 GMOD.copy_on_change copy_on_change {
	    trigger => <-.<-.browser_visible.do;
	    input   => <-.<-.browser_visible.do;
	    output  =>    <-.visible;
	 };
	 title = "Read DICOM Filename";
	 //searchPattern = "$XP_PATH<0>/data/hdf5/*.h5";
	 //searchPattern = "*.h5";
	 filename => <-.dicom_filename;
      };

      UIbutton apply {
	 parent => <-.panel;
	 y      => <-.file_name.y + <-.file_name.height + 4;
	 width  = 100;
	 height => <-.file_name.height;
	 label  = "Read File";
	 active => is_valid(  <-.dicom_filename );
	 //do     => param.trigger;
      };

#ifdef UNDEFINED
      UIbutton SelectSeriesUI {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Select Patient Series ";
      };
      ReadDCMLISTUI ReadDCMLISTUI {
	 visible => <-.SelectSeriesUI.do;
      };
#endif

      XP_COMM_PROJ.ReadDICOM.ReadDICOMMods.read_dicom_files read_dicom_files {
	 //filenames => <-.ReadDCMLISTUI.filenames;
	 //trigger   => <-.ReadDCMLISTUI.read;
         filenames => { <-.dicom_filename } ;
         trigger   => <-.apply.do;
      };

      XP_COMM_PROJ.ReadDICOM.ReadDICOMData.DICOM_Header &Header<NEportLevels={1,0}> => read_dicom_files.header;

      UItext UISeriesInfo {
	 parent => <-.panel;
	 width  => parent.clientWidth;
	 height => 8 * UIdata.UIfonts[0].lineHeight;
         y      => apply.y + apply.height + 4;
	 outputOnly = 1;
         multiLine  = 1;
	 text => switch(is_valid(<-.Header.imageSeries.Columns)+1, "", str_format("Patient: %s\nDate: %s\nDesc: %s\nModality: %s %s\nDims: %d x %d\nSize: %.1f x %.1f mm",
	 <-.Header.patient.PatientsName,
	 <-.Header.generalSeries.SeriesDate,
	 <-.Header.generalSeries.SeriesDescription,
	 <-.Header.generalSeries.Modality,
	 <-.Header.generalSeries.ProtocolName,
	 <-.Header.imageSeries.Columns, <-.Header.imageSeries.Rows,
	 <-.Header.imageSeries.Columns * <-.Header.imageSeries.XSpacing,
	 <-.Header.imageSeries.Rows * <-.Header.imageSeries.YSpacing));
      };
      UIbutton PatientButton {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Patient Information";
      };
      UIbutton StudyButton {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = "Study Information";
      };
      UIbutton SeriesButton {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = " Series Information";
      };
      UIbutton EquipmentButton {
	 width => parent.clientWidth;
	 parent => <-.panel;
	 label = " Equipment Information";
      };
      ReadDICOMPatientUI PatientInfo {
	 visible => <-.PatientButton.do;
	 patient => <-.Header.patient;
      };
      ReadDICOMStudyUI StudyInfo {
	 visible => <-.StudyButton.do;
	 generalStudy => <-.Header.generalStudy;
      };
      ReadDICOMSeriesUI SeriesInfo {
	 visible => <-.SeriesButton.do;
	 generalSeries => <-.Header.generalSeries;
      };
      ReadDICOMEquipmentUI EquipmentInfo {
	 visible => <-.EquipmentButton.do;
	 generalEquipment => <-.Header.generalEquipment;
      };
      UIlabel Copyright {
	 width => parent.clientWidth;
	 height => UIdata.UIfonts[0].lineHeight * 5;
         y => EquipmentButton.y + EquipmentButton.height + 8;
	 parent => <-.panel;
	 alignment = "center";
	 label = "Written by James S. Perrin\nUniversity of Manchester\nBased on Read_DICOM(1)\nDCMTK Copyright (C) 1994-2004, OFFIS";
      };
      GDM.DataObject DICOMVolume {
	 in => <-.read_dicom_files.volume;
      };
      olink out => read_dicom_files.volume;
      olink obj => DICOMVolume.obj;
      olink header => Header;
   };
};


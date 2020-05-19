
flibrary ReadPapyrusMacs<compile_subs=0> {

   macro ReadPapyrusUI {
      UImod_panel panel<NEx=44.,NEy=33.> {
         height = 1000;
         parent<NEportLevels={4,0}>;
         message = "Select Read Papyrus control panel.";
         title = "ReadPapyrus";
      };

      XP_COMM_PROJ.ReadPapyrus.ReadPapyrusMods.Image_Header &Image_Header<NEx=22.,NEy=209.,NEportLevels={2,1}>;
      string &filename<NEx=22.,NEy=275.,NEportLevels={2,1}>;


      UIlabel Copyright<NEx=164.,NEy=88.> {
         y = 0;
         width => parent.clientWidth;
         height = 76;
         parent => <-.panel;
         alignment = "left";
         label = "Reader made by AVS thanks to :\n   PAPYRUS toolkit v3.5\n   Copyright (c) 1998 by\n   University Hospital of Geneva";
      };
      UIlabel Papyrus_Filename<NEx=332.,NEy=88.> {
         y => ((<-.Copyright.y + <-.Copyright.height) + 5);
         width = 200;
         parent => <-.panel;
         alignment = "left";
      };
      UItext file_name<NEx=484.,NEy=88.> {
         y => ((<-.Papyrus_Filename.y + <-.Papyrus_Filename.height) + 5);
         width = 170;
         parent => <-.panel;
         text => <-.filename;
         showLastPosition = 1;
      };
      UIbutton visible<NEx=693.,NEy=88.> {
         x => ((<-.file_name.x + <-.file_name.width) + 5);
         y => <-.file_name.y;
         width = 75;
         height => <-.file_name.height;
         parent => <-.panel;
         label = "Browse ...";
      };
      UIfileDialog file_browser<NEx=693.,NEy=143.> {
         title => "Read Papyrus Filename";
         filename => <-.filename;
         searchPattern = "*.pap";
         visible => <-.visible.do;
      };
      UIlabel Num_Frames<NEx=858.,NEy=88.> {
         y => ((<-.file_name.y + <-.file_name.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         visible => is_valid(<-.<-.Read_Papyrus_File.output_image.dims[2]);
         label => str_format("Number of Frames : %s",<-.<-.Read_Papyrus_File.output_image.dims[2]);
      };
      UIbutton PatientButton<NEx=253.,NEy=209.> {
         y => ((<-.Num_Frames.y + <-.Num_Frames.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         label = "Patient Information";
      };
      UIshell PatientShell<NEx=253.,NEy=264.> {
         visible => <-.PatientButton.do;
         height = 400;
         showStatusBar = 0;
      };
      UIbutton GeneralStudyButton<NEx=440.,NEy=209.> {
         y => ((<-.PatientButton.y + <-.PatientButton.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         label = "General Study Information";
      };
      UIshell GeneralStudyShell<NEx=440.,NEy=264.> {
         visible => <-.GeneralStudyButton.do;
         height = 400;
         showStatusBar = 0;
      };
      UIbutton GeneralSeriesButton<NEx=638.,NEy=209.> {
         y => ((<-.GeneralStudyButton.y + <-.GeneralStudyButton.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         label = "General Series Information";
      };
      UIshell GeneralSeriesShell<NEx=638.,NEy=264.> {
         visible => <-.GeneralSeriesButton.do;
         height = 400;
         showStatusBar = 0;
      };
      UIbutton GeneralEquipmentButton<NEx=836.,NEy=209.> {
         y => ((<-.GeneralSeriesButton.y + <-.GeneralSeriesButton.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         label = "General Equipment Information";
      };
      UIshell GeneralEquipmentShell<NEx=836.,NEy=264.> {
         visible => <-.GeneralEquipmentButton.do;
         height = 400;
         showStatusBar = 0;
      };
      UIbutton ImageSeriesButton {
         y => ((<-.GeneralEquipmentButton.y + <-.GeneralEquipmentButton.height) + 10);
         width => parent.clientWidth;
         parent => <-.panel;
         label = "Image Series Information";
      };
      UIshell ImageSeriesShell {
         visible => <-.ImageSeriesButton.do;
         height = 400;
         showStatusBar = 0;
      };
          macro Patient<NEx=253.,NEy=319.> {
              UIshell &panel<NEportLevels={2,1}> => <-.PatientShell;
              group &patient<NEportLevels={2,0}> => <-.Image_Header.patient;
              int Offset = 112;
              UIlabel Patient {
                 y = 10;
                 width => parent.clientWidth;
                 parent => <-.panel;
                 label = "PATIENT INFORMATION";
              };
              UIlabel PatientsNameP_1 {
                 y => (<-.Patient.y + <-.Patient.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Name :";
              };
              UIlabel PatientsNameP_2 {
                 x = Offset;
                 y => (<-.Patient.y + <-.Patient.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientsNameP)+1,"",<-.patient.PatientsNameP);
              };
              UIlabel PatientIDP_1 {
                 y => (<-.PatientsNameP_1.y + <-.PatientsNameP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "IDP :";
              };
              UIlabel PatientIDP_2 {
                 x = Offset;
                 y => (<-.PatientsNameP_1.y + <-.PatientsNameP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientIDP)+1,"",<-.patient.PatientIDP);
              };
              UIlabel PatientsBirthDateP_1 {
                 y => (<-.PatientIDP_1.y + <-.PatientIDP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Birth Date :";
              };
              UIlabel PatientsBirthDateP_2 {
                 x = Offset;
                 y => (<-.PatientIDP_1.y + <-.PatientIDP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientsBirthDateP)+1,"",<-.patient.PatientsBirthDateP);
              };
              UIlabel PatientsBirthTimeP_1 {
                 y => (<-.PatientsBirthDateP_1.y + <-.PatientsBirthDateP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Birth Time :";
              };
              UIlabel PatientsBirthTimeP_2 {
                 x = Offset;
                 y => (<-.PatientsBirthDateP_1.y + <-.PatientsBirthDateP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientsBirthTimeP)+1,"",<-.patient.PatientsBirthTimeP);
              };
              UIlabel PatientsSexP_1 {
                 y => (<-.PatientsBirthTimeP_1.y + <-.PatientsBirthTimeP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Sex :";
              };
              UIlabel PatientsSexP_2 {
                 x = Offset;
                 y => (<-.PatientsBirthTimeP_1.y + <-.PatientsBirthTimeP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientsSexP)+1,"",<-.patient.PatientsSexP);
              };
              UIlabel ReferencedPatientSequenceP_1 {
                 y => (<-.PatientsSexP_1.y + <-.PatientsSexP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Referenced Seq :";
              };
              UIlabel ReferencedPatientSequenceP_2 {
                 x = Offset;
                 y => (<-.PatientsSexP_1.y + <-.PatientsSexP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.ReferencedPatientSequenceP)+1,"",<-.patient.ReferencedPatientSequenceP);
              };
              UIlabel OtherPatientID_1 {
                 y => (<-.ReferencedPatientSequenceP_1.y + <-.ReferencedPatientSequenceP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Other ID :";
              };
              UIlabel OtherPatientID_2 {
                 x = Offset;
                 y => (<-.ReferencedPatientSequenceP_1.y + <-.ReferencedPatientSequenceP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.OtherPatientID)+1,"",<-.patient.OtherPatientID);
              };
              UIlabel OtherPatientNamesP_1 {
                 y => (<-.OtherPatientID_1.y + <-.OtherPatientID_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Other Names :";
              };
              UIlabel OtherPatientNamesP_2 {
                 x = Offset;
                 y => (<-.OtherPatientID_1.y + <-.OtherPatientID_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.OtherPatientNamesP)+1,"",<-.patient.OtherPatientNamesP);
              };
              UIlabel EthnicGroupP_1 {
                 y => (<-.OtherPatientNamesP_1.y + <-.OtherPatientNamesP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Ethnic Group :";
              };
              UIlabel EthnicGroupP_2 {
                 x = Offset;
                 y => (<-.OtherPatientNamesP_1.y + <-.OtherPatientNamesP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.EthnicGroupP)+1,"",<-.patient.EthnicGroupP);
              };
              UIlabel PatientCommentsP_1 {
                 y => (<-.EthnicGroupP_1.y + <-.EthnicGroupP_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Comments :";
              };
              UIlabel PatientCommentsP_2 {
                 x = Offset;
                 y => (<-.EthnicGroupP_1.y + <-.EthnicGroupP_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.patient.PatientCommentsP)+1,"",<-.patient.PatientCommentsP);
              };
          };
          macro GeneralStudy<NEx=440.,NEy=319.> {
              UIshell &panel<NEportLevels={2,1}> => <-.GeneralStudyShell;
              group &generalStudy<NEportLevels={2,0}> => <-.Image_Header.generalStudy;
              int Offset = 142;
              UIlabel GeneralStudy {
                 y = 10;
                 width => parent.clientWidth;
                 parent => <-.panel;
                 label = "GENERAL STUDY INFORMATION";
              };
              UIlabel StudyInstanceUIDGS_1 {
                 y => (<-.GeneralStudy.y + <-.GeneralStudy.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Instance UID :";
              };
              UIlabel StudyInstanceUIDGS_2 {
                 x = Offset;
                 y => (<-.GeneralStudy.y + <-.GeneralStudy.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.StudyInstanceUIDGS)+1,"",<-.generalStudy.StudyInstanceUIDGS);
              };
              UIlabel StudyDateGS_1 {
                 y => (<-.StudyInstanceUIDGS_1.y + <-.StudyInstanceUIDGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Date :";
              };
              UIlabel StudyDateGS_2 {
                 x = Offset;
                 y => (<-.StudyInstanceUIDGS_1.y + <-.StudyInstanceUIDGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.StudyDateGS)+1,"",<-.generalStudy.StudyDateGS);
              };
              UIlabel StudyTimeGS_1 {
                 y => (<-.StudyDateGS_1.y + <-.StudyDateGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Time :";
              };
              UIlabel StudyTimeGS_2 {
                 x = Offset;
                 y => (<-.StudyDateGS_1.y + <-.StudyDateGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.StudyTimeGS)+1,"",<-.generalStudy.StudyTimeGS);
              };
              UIlabel ReferringPhysiciansNameGS_1 {
                 y => (<-.StudyTimeGS_1.y + <-.StudyTimeGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Referring Phys Name :";
              };
              UIlabel ReferringPhysiciansNameGS_2 {
                 x = Offset;
                 y => (<-.StudyTimeGS_1.y + <-.StudyTimeGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.ReferringPhysiciansNameGS)+1,"",<-.generalStudy.ReferringPhysiciansNameGS);
              };
              UIlabel StudyIDGS_1 {
                 y => (<-.ReferringPhysiciansNameGS_1.y + <-.ReferringPhysiciansNameGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "ID :";
              };
              UIlabel StudyIDGS_2 {
                 x = Offset;
                 y => (<-.ReferringPhysiciansNameGS_1.y + <-.ReferringPhysiciansNameGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.StudyIDGS)+1,"",<-.generalStudy.StudyIDGS);
              };
              UIlabel AccessionNumberGS_1 {
                 y => (<-.StudyIDGS_1.y + <-.StudyIDGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Accession Number :";
              };
              UIlabel AccessionNumberGS_2 {
                 x = Offset;
                 y => (<-.StudyIDGS_1.y + <-.StudyIDGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.AccessionNumberGS)+1,"",<-.generalStudy.AccessionNumberGS);
              };
              UIlabel StudyDescriptionGS_1 {
                 y => (<-.AccessionNumberGS_1.y + <-.AccessionNumberGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Description :";
              };
              UIlabel StudyDescriptionGS_2 {
                 x = Offset;
                 y => (<-.AccessionNumberGS_1.y + <-.AccessionNumberGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.StudyDescriptionGS)+1,"",<-.generalStudy.StudyDescriptionGS);
              };
              UIlabel NameofPhysiciansReadingStudyGS_1 {
                 y => (<-.StudyDescriptionGS_1.y + <-.StudyDescriptionGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Reading Phys Name :";
              };
              UIlabel NameofPhysiciansReadingStudyGS_2 {
                 x = Offset;
                 y => (<-.StudyDescriptionGS_1.y + <-.StudyDescriptionGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.NameofPhysiciansReadingStudyGS)+1,"",<-.generalStudy.NameofPhysiciansReadingStudyGS);
              };
              UIlabel ReferencedStudySequenceGS_1 {
                 y => (<-.NameofPhysiciansReadingStudyGS_1.y + <-.NameofPhysiciansReadingStudyGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Referenced Seq :";
              };
              UIlabel ReferencedStudySequenceGS_2 {
                 x = Offset;
                 y => (<-.NameofPhysiciansReadingStudyGS_1.y + <-.NameofPhysiciansReadingStudyGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalStudy.ReferencedStudySequenceGS)+1,"",<-.generalStudy.ReferencedStudySequenceGS);
              };
          };
          macro GeneralSeries<NEx=638.,NEy=319.> {
              UIshell &panel<NEportLevels={2,1}> => <-.GeneralSeriesShell;
              group &generalSeries<NEportLevels={2,0}> => <-.Image_Header.generalSeries;
              int Offset = 142;
              UIlabel GeneralSeries {
                 y = 10;
                 width => parent.clientWidth;
                 parent => <-.panel;
                 label = "GENERAL SERIES INFORMATION";
              };
              UIlabel ModalityGS_1 {
                 y => (<-.GeneralSeries.y + <-.GeneralSeries.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Modality :";
              };
              UIlabel ModalityGS_2 {
                 x = Offset;
                 y => (<-.GeneralSeries.y + <-.GeneralSeries.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.ModalityGS)+1,"",<-.generalSeries.ModalityGS);
              };
              UIlabel SeriesInstanceUIDGS_1 {
                 y => (<-.ModalityGS_1.y + <-.ModalityGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Instance UID :";
              };
              UIlabel SeriesInstanceUIDGS_2 {
                 x = Offset;
                 y => (<-.ModalityGS_1.y + <-.ModalityGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SeriesInstanceUIDGS)+1,"",<-.generalSeries.SeriesInstanceUIDGS);
              };
              UIlabel SeriesNumberGS_1 {
                 y => (<-.SeriesInstanceUIDGS_1.y + <-.SeriesInstanceUIDGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Number :";
              };
              UIlabel SeriesNumberGS_2 {
                 x = Offset;
                 y => (<-.SeriesInstanceUIDGS_1.y + <-.SeriesInstanceUIDGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SeriesNumberGS)+1,"",<-.generalSeries.SeriesNumberGS);
              };
              UIlabel Laterality_1 {
                 y => (<-.SeriesNumberGS_1.y + <-.SeriesNumberGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Laterality :";
              };
              UIlabel Laterality_2 {
                 x = Offset;
                 y => (<-.SeriesNumberGS_1.y + <-.SeriesNumberGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.Laterality)+1,"",<-.generalSeries.Laterality);
              };
              UIlabel SeriesDate_1 {
                 y => (<-.Laterality_1.y + <-.Laterality_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Date :";
              };
              UIlabel SeriesDate_2 {
                 x = Offset;
                 y => (<-.Laterality_1.y + <-.Laterality_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SeriesDate)+1,"",<-.generalSeries.SeriesDate);
              };
              UIlabel SeriesTime_1 {
                 y => (<-.SeriesDate_1.y + <-.SeriesDate_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Time :";
              };
              UIlabel SeriesTime_2 {
                 x = Offset;
                 y => (<-.SeriesDate_1.y + <-.SeriesDate_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SeriesTime)+1,"",<-.generalSeries.SeriesTime);
              };
              UIlabel PerformingPhysiciansNameGS_1 {
                 y => (<-.SeriesTime_1.y + <-.SeriesTime_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Perf Phys Name :";
              };
              UIlabel PerformingPhysiciansNameGS_2 {
                 x = Offset;
                 y => (<-.SeriesTime_1.y + <-.SeriesTime_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.PerformingPhysiciansNameGS)+1,"",<-.generalSeries.PerformingPhysiciansNameGS);
              };
              UIlabel ProtocolName_1 {
                 y => (<-.PerformingPhysiciansNameGS_1.y + <-.PerformingPhysiciansNameGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Protocol Name :";
              };
              UIlabel ProtocolName_2 {
                 x = Offset;
                 y => (<-.PerformingPhysiciansNameGS_1.y + <-.PerformingPhysiciansNameGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.ProtocolName)+1,"",<-.generalSeries.ProtocolName);
              };
              UIlabel SeriesDescription_1 {
                 y => (<-.ProtocolName_1.y + <-.ProtocolName_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Description :";
              };
              UIlabel SeriesDescription_2 {
                 x = Offset;
                 y => (<-.ProtocolName_1.y + <-.ProtocolName_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SeriesDescription)+1,"",<-.generalSeries.SeriesDescription);
              };
              UIlabel OperatorsName_1 {
                 y => (<-.SeriesDescription_1.y + <-.SeriesDescription_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Operator's Name :";
              };
              UIlabel OperatorsName_2 {
                 x = Offset;
                 y => (<-.SeriesDescription_1.y + <-.SeriesDescription_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.OperatorsName)+1,"",<-.generalSeries.OperatorsName);
              };
              UIlabel ReferencedStudyComponentSequenceGS_1 {
                 y => (<-.OperatorsName_1.y + <-.OperatorsName_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Ref Study Comp Seq :";
              };
              UIlabel ReferencedStudyComponentSequenceGS_2 {
                 x = Offset;
                 y => (<-.OperatorsName_1.y + <-.OperatorsName_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.ReferencedStudyComponentSequenceGS)+1,"",<-.generalSeries.ReferencedStudyComponentSequenceGS);
              };
              UIlabel BodyPartExaminedGS_1 {
                 y => (<-.ReferencedStudyComponentSequenceGS_1.y + <-.ReferencedStudyComponentSequenceGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Body Part Examined :";
              };
              UIlabel BodyPartExaminedGS_2 {
                 x = Offset;
                 y => (<-.ReferencedStudyComponentSequenceGS_1.y + <-.ReferencedStudyComponentSequenceGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.BodyPartExaminedGS)+1,"",<-.generalSeries.BodyPartExaminedGS);
              };
              UIlabel PatientPosition_1 {
                 y => (<-.BodyPartExaminedGS_1.y + <-.BodyPartExaminedGS_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Patient Position :";
              };
              UIlabel PatientPosition_2 {
                 x = Offset;
                 y => (<-.BodyPartExaminedGS_1.y + <-.BodyPartExaminedGS_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.PatientPosition)+1,"",<-.generalSeries.PatientPosition);
              };
              UIlabel SmallestPixelValueinSeries_1 {
                 y => (<-.PatientPosition_1.y + <-.PatientPosition_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Smallest Pixel Value :";
              };
              UIlabel SmallestPixelValueinSeries_2 {
                 x = Offset;
                 y => (<-.PatientPosition_1.y + <-.PatientPosition_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.SmallestPixelValueinSeries)+1,"",<-.generalSeries.SmallestPixelValueinSeries);
              };
              UIlabel LargestPixelValueinSeries_1 {
                 y => (<-.SmallestPixelValueinSeries_1.y + <-.SmallestPixelValueinSeries_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Largest Pixel Value :";
              };
              UIlabel LargestPixelValueinSeries_2 {
                 x = Offset;
                 y => (<-.SmallestPixelValueinSeries_1.y + <-.SmallestPixelValueinSeries_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalSeries.LargestPixelValueinSeries)+1,"",<-.generalSeries.LargestPixelValueinSeries);
              };
          };
          macro GeneralEquipment<NEx=836.,NEy=319.> {
              UIshell &panel<NEportLevels={2,1}> => <-.GeneralEquipmentShell;
              group &generalEquipment<NEportLevels={2,0}> => <-.Image_Header.generalEquipment;
              int Offset = 142;
              UIlabel GeneralEquipment {
                 y = 10;
                 width => parent.clientWidth;
                 parent => <-.panel;
                 label = "GENERAL EQUIPMENT INFORMATION";
              };
              UIlabel ManufacturerGE_1 {
                 y => (<-.GeneralEquipment.y + <-.GeneralEquipment.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Manufacturer :";
              };
              UIlabel ManufacturerGE_2 {
                 x = Offset;
                 y => (<-.GeneralEquipment.y + <-.GeneralEquipment.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.ManufacturerGE)+1,"",<-.generalEquipment.ManufacturerGE);
              };
              UIlabel InstitutionNameGE_1 {
                 y => (<-.ManufacturerGE_1.y + <-.ManufacturerGE_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Institution Name :";
              };
              UIlabel InstitutionNameGE_2 {
                 x = Offset;
                 y => (<-.ManufacturerGE_1.y + <-.ManufacturerGE_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.InstitutionNameGE)+1,"",<-.generalEquipment.InstitutionNameGE);
              };
              UIlabel InstitutionAddressGE_1 {
                 y => (<-.InstitutionNameGE_1.y + <-.InstitutionNameGE_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Institution Address :";
              };
              UIlabel InstitutionAddressGE_2 {
                 x = Offset;
                 y => (<-.InstitutionNameGE_1.y + <-.InstitutionNameGE_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.InstitutionAddressGE)+1,"",<-.generalEquipment.InstitutionAddressGE);
              };
              UIlabel StationName_1 {
                 y => (<-.InstitutionAddressGE_1.y + <-.InstitutionAddressGE_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Station Name :";
              };
              UIlabel StationName_2 {
                 x = Offset;
                 y => (<-.InstitutionAddressGE_1.y + <-.InstitutionAddressGE_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.StationName)+1,"",<-.generalEquipment.StationName);
              };
              UIlabel InstitutionalDepartmentName_1 {
                 y => (<-.StationName_1.y + <-.StationName_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Institution Dept Name :";
              };
              UIlabel InstitutionalDepartmentName_2 {
                 x = Offset;
                 y => (<-.StationName_1.y + <-.StationName_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.InstitutionalDepartmentName)+1,"",<-.generalEquipment.InstitutionalDepartmentName);
              };
              UIlabel ManufacturersModelName_1 {
                 y => (<-.InstitutionalDepartmentName_1.y + <-.InstitutionalDepartmentName_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Manufacturer's Model :";
              };
              UIlabel ManufacturersModelName_2 {
                 x = Offset;
                 y => (<-.InstitutionalDepartmentName_1.y + <-.InstitutionalDepartmentName_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.ManufacturersModelName)+1,"",<-.generalEquipment.ManufacturersModelName);
              };
              UIlabel PDeviceSerialNumberGE_1 {
                 y => (<-.ManufacturersModelName_1.y + <-.ManufacturersModelName_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Device SN :";
              };
              UIlabel DeviceSerialNumberGE_2 {
                 x = Offset;
                 y => (<-.ManufacturersModelName_1.y + <-.ManufacturersModelName_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.DeviceSerialNumberGE)+1,"",<-.generalEquipment.DeviceSerialNumberGE);
              };
              UIlabel SoftwareVersionsGE_1 {
                 y => (<-.PDeviceSerialNumberGE_1.y + <-.PDeviceSerialNumberGE_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Software Versions :";
              };
              UIlabel SoftwareVersionsGE_2 {
                 x = Offset;
                 y => (<-.PDeviceSerialNumberGE_1.y + <-.PDeviceSerialNumberGE_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.SoftwareVersionsGE)+1,"",<-.generalEquipment.SoftwareVersionsGE);
              };
              UIlabel SpatialResolution_1 {
                 y => (<-.SoftwareVersionsGE_1.y + <-.SoftwareVersionsGE_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Spatial Resolution :";
              };
              UIlabel SpatialResolution_2 {
                 x = Offset;
                 y => (<-.SoftwareVersionsGE_1.y + <-.SoftwareVersionsGE_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.SpatialResolution)+1,"",<-.generalEquipment.SpatialResolution);
              };
              UIlabel DateofLastCalibration_1 {
                 y => (<-.SpatialResolution_1.y + <-.SpatialResolution_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Last Calibration Date :";
              };
              UIlabel DateofLastCalibration_2 {
                 x = Offset;
                 y => (<-.SpatialResolution_1.y + <-.SpatialResolution_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.DateofLastCalibration)+1,"",<-.generalEquipment.DateofLastCalibration);
              };
              UIlabel TimeofLastCalibration_1 {
                 y => (<-.DateofLastCalibration_1.y + <-.DateofLastCalibration_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Last Calibration Time :";
              };
              UIlabel TimeofLastCalibration_2 {
                 x = Offset;
                 y => (<-.DateofLastCalibration_1.y + <-.DateofLastCalibration_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.TimeofLastCalibration)+1,"",<-.generalEquipment.TimeofLastCalibration);
              };
              UIlabel PixelPaddingValue_1 {
                 y => (<-.TimeofLastCalibration_1.y + <-.TimeofLastCalibration_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Pixel Padding Value :";
              };
              UIlabel PixelPaddingValue_2 {
                 x = Offset;
                 y => (<-.TimeofLastCalibration_1.y + <-.TimeofLastCalibration_1.height);
                 width => parent.clientWidth - Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.generalEquipment.PixelPaddingValue)+1,"",<-.generalEquipment.PixelPaddingValue);
              };
          };

          macro ImageSeries {
              UIshell &panel<NEportLevels={2,1}> => <-.ImageSeriesShell;
              group &imageSeries<NEportLevels={2,0}> => <-.Image_Header.imageSeries;
              int Offset = 142;
              UIlabel ImageSeries {
                 y = 10;
                 width => parent.clientWidth;
                 parent => <-.panel;
                 label = "IMAGE SERIES INFORMATION";
              };
              UIlabel Interpretation_1 {
                 y => (<-.ImageSeries.y + <-.ImageSeries.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Interpretation :";
              };
              UIlabel Interpretation_2 {
                 x = Offset;
                 y => Interpretation_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.Interpretation)+1,"", <-.imageSeries.Interpretation);
              };
              UIlabel Rows_1 {
                 y => (<-.Interpretation_1.y + <-.Interpretation_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Rows :";
              };
              UIlabel Rows_2 {
                 x = Offset;
                 y => Rows_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.Rows)+1,"", <-.imageSeries.Rows);
              };
              UIlabel Columns_1 {
                 y => (<-.Rows_1.y + <-.Rows_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Columns :";
              };
              UIlabel Columns_2 {
                 x = Offset;
                 y => Columns_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.Columns)+1,"", <-.imageSeries.Columns);
              };
              UIlabel Slices_1 {
                 y => (<-.Columns_1.y + <-.Columns_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "Slices :";
              };
              UIlabel Slices_2 {
                 x = Offset;
                 y => Slices_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.Slices)+1,"", <-.imageSeries.Slices);
              };
              UIlabel BitsAllocated_1 {
                 y => (<-.Slices_1.y + <-.Slices_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "BitsAllocated :";
              };
              UIlabel BitsAllocated_2 {
                 x = Offset;
                 y => BitsAllocated_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.BitsAllocated)+1,"", <-.imageSeries.BitsAllocated);
              };
              UIlabel SliceSpacing_1 {
                 y => (<-.BitsAllocated_1.y + <-.BitsAllocated_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "SliceSpacing :";
              };
              UIlabel SliceSpacing_2 {
                 x = Offset;
                 y => SliceSpacing_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.SliceSpacing)+1,"", <-.imageSeries.SliceSpacing);
              };
              UIlabel XSpacing_1 {
                 y => (<-.SliceSpacing_1.y + <-.SliceSpacing_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "XSpacing :";
              };
              UIlabel XSpacing_2 {
                 x = Offset;
                 y => XSpacing_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.XSpacing)+1,"", <-.imageSeries.XSpacing);
              };
              UIlabel YSpacing_1 {
                 y => (<-.XSpacing_1.y + <-.XSpacing_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "YSpacing :";
              };
              UIlabel YSpacing_2 {
                 x = Offset;
                 y => YSpacing_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.YSpacing)+1,"", <-.imageSeries.YSpacing);
              };
              UIlabel WindowCentre_1 {
                 y => (<-.YSpacing_1.y + <-.YSpacing_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "WindowCentre :";
              };
              UIlabel WindowCentre_2 {
                 x = Offset;
                 y => WindowCentre_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.WindowCentre)+1,"", <-.imageSeries.WindowCentre);
              };
              UIlabel WindowWidth_1 {
                 y => (<-.WindowCentre_1.y + <-.WindowCentre_1.height);
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label = "WindowWidth :";
              };
              UIlabel WindowWidth_2 {
                 x = Offset;
                 y => WindowWidth_1.y;
                 width => Offset;
                 parent => <-.panel;
                 alignment = "left";
                 label => switch(is_valid(<-.imageSeries.WindowWidth)+1,"", <-.imageSeries.WindowWidth);
              };
          };
   };



   macro ReadPapyrusFunc {
      string+OPort &filename<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadPapyrus.ReadPapyrusMods.ReadPapyrusFile ReadPapyrusFile {
         filename => <-.filename;
      };

      GDM.DataObject DataObject {
         in => <-.ReadPapyrusFile.output_image;
         Obj.name => name_of(<-.<-.<-);
      };

      olink busy       => .ReadPapyrusFile.busy;
      olink out_header => .ReadPapyrusFile.header;
      olink out_fld    => .ReadPapyrusFile.output_image;
      olink out_obj    => .DataObject.obj;
   };



   macro read_papyrus {
      string+OPort filename;

      ReadPapyrusFunc ReadPapyrusFunc {
         filename => <-.filename;
      	DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };

      ReadPapyrusUI ReadPapyrusUI {
         filename => <-.filename;
         Image_Header => <-.ReadPapyrusFunc.out_header;
         panel {
            title => name_of(<-.<-.<-,1);
         };
      };

      olink busy       => .ReadPapyrusFunc.busy;
      olink out_header => .ReadPapyrusFunc.out_header;
      olink out_fld    => .ReadPapyrusFunc.out_fld;
      olink out_obj    => .ReadPapyrusFunc.out_obj;
   };

};


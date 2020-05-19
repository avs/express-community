/********************************************************************************/
/*										*/
/*	Project  : P A P Y R U S  Toolkit				 	*/
/*	File     : PapyEnumMiscModules3.h				 	*/
/*	Function : contains the declarations of the modules that are not	*/
/*		   in the image modules						*/
/*	Authors  : Christian Girard						*/
/*	History  : 08.1994	version 3.0					*/
/*								   		*/
/*	(C) 1994-1996 The University Hospital of Geneva			  	*/
/*		       All Rights Reserved					*/
/*										*/
/********************************************************************************/


#ifndef PapyEnumMiscModulesH 
#define PapyEnumMiscModulesH



/*	Module : Basic Annotation Presentation		*/

enum {
papAnnotationPosition,
papTextString,
papEndBasicAnnotationPresentation
};




/*	Module : Basic Film Box Presentation		*/

enum {
papImageDisplayFormat,
papFilmOrientation,
papFilmSizeID,
papMagnificationTypeBFBP,
papSmoothingTypeBFBP,
papBorderDensity,
papEmptyImageDensity,
papMinDensity,
papMaxDensity,
papTrim,
papConfigurationInformation,
papEndBasicFilmBoxPresentation
};




/*	Module : Basic Film Box Relationship		*/

enum {
papReferencedFilmSessionSequence,
papReferencedImageBoxSequenceBFBR,
papReferencedBasicAnnotationBoxSequence,
papEndBasicFilmBoxRelationship
};




/*	Module : Basic Film Session Presentation	*/

enum {
papNumberofCopies,
papPrintPriorityBFSP,
papMediumType,
papFilmDestination,
papFilmSessionLabel,
papMemoryAllocation,
papEndBasicFilmSessionPresentation
};




/*	Module : File Reference				*/

enum {
papReferencedSOPClassUID,
papReferencedSOPInstanceUID,
papReferencedFileName,
papReferencedFilePath,
papEndFileReference
};



/*	Module : Identifying Image Sequence		*/

enum {
papImageIdentifierSequence,
papEndIdentifyingImageSequence
};




/*	Module : LUT Identification			*/

enum {
papLUTNumber,
papReferencedImageSequenceLI,
papEndLUTIdentification
};




/*	Module : Patient Demographic			*/

enum {
papPatientsAddress,
papRegionofResidence,
papCountryofResidence,
papPatientsTelephoneNumbers,
papPatientsBirthDatePD,
papPatientsBirthTimePD,
papEthnicGroupPD,
papPatientsSexPD,
papPatientsSizePD,
papPatientsWeightPD,
papMilitaryRank,
papBranchofService,
papPatientsInsurancePlanCodeSequence,
papPatientsReligiousPreference,
papPatientCommentsPD,
papEndPatientDemographic
};




/*	Module : Patient Identification			*/

enum {
papPatientsNamePI,
papPatientIDPI,
papIssuerofPatientID,
papOtherPatientIDs,
papOtherPatientNamesPI,
papPatientsBirthName,
papPatientsMothersBirthName,
papMedicalRecordLocator,
papEndPatientIdentification
};




/*	Module : Patient Medical			*/

enum {
papPatientState,
papPregnancyStatus,
papMedicalAlerts,
papContrastAllergies,
papSpecialNeeds,
papLastMenstrualDate,
papSmokingStatus,
papAdditionalPatientHistory,
papEndPatientMedical
};




/*	Module : Patient Relationship			*/

enum {
papReferencedVisitSequencePR,
papReferencedStudySequencePR,
papReferencedPatientAliasSequence,
papEndPatientRelationship
};




/*	Module : Patient Summary			*/

enum {
papPatientsNamePS,
papPatientIDPS,
papEndPatientSummary
};





/*	Module : Printer				*/

enum {
papPrinterStatus,
papPrinterStatusInfo,
papPrinterNameP,
papManufacturerP,
papManufacturerModelName,
papDeviceSerialNumberP,
papSoftwareVersionsP,
papDateOfLastCalibration,
papTimeOfLastCalibration,
papEndPrinter
};




/*	Module : Print Job				*/

enum {
papExecutionStatus,
papExecutionStatusInfo,
papCreationDate,
papCreationTime,
papPrintPriorityPJ,
papPrinterNamePJ,
papOriginator,
papEndPrintJob
};




/*	Module : Result Identification			*/

enum {
papResultsID,
papResultsIDIssuer,
papEndResultIdentification
};




/*	Module : Results Impression			*/

enum {
papImpressions,
papResultsComments,
papEndResultsImpression
};




/*	Module : Result Relationship			*/

enum {
papReferencedStudySequenceRR,
papReferencedInterpretationSequence,
papEndResultRelationship
};




/*	Module : Study Acquisition			*/

enum {
papStudyArrivalDate,
papStudyArrivalTime,
papStudyDateSA,
papStudyTimeSA,
papStudyCompletionDate,
papStudyCompletionTime,
papStudyVerifiedDate,
papStudyVerifiedTime,
papSeriesinStudy,
papAcquisitionsinStudy,
papEndStudyAcquisition
};




/*	Module : Study Classification			*/

enum {
papStudyStatusID,
papStudyPriorityID,
papStudyComments,
papEndStudyClassification
};




/*	Module : Study Component Acquisition		*/

enum {
papModalitySCA,
papStudyDescriptionSCA,
papProcedureCodeSequence,
papPerformingPhysiciansNameSCA,
papStudyComponentStatusID,
papEndStudyComponentAcquisition
};




/*	Module : Study Component			*/

enum {
papStudyIDSC,
papStudyInstanceUIDSC,
papReferencedSeriesSequenceSC,
papEndStudyComponent
};




/*	Module : Study Component Relationship		*/

enum {
papReferencedStudySequenceSCR,
papEndStudyComponentRelationship
};




/*	Module : Study Content				*/

enum {
papStudyIDSCt,
papStudyInstanceUIDSCt,
papReferencedSeriesSequenceSCt,
papEndStudyContent
};




/*	Module : Study Identification			*/

enum {
papStudyIDSI,
papStudyIDIssuer,
papOtherStudyNumbers,
papEndStudyIdentification
};




/*	Module : Study Read				*/

enum {
papNameofPhysiciansReadingStudySR,
papStudyReadDate,
papStudyReadTime,
papEndStudyRead
};




/*	Module : Study Relationship			*/

enum {
papReferencedVisitSequenceSR,
papReferencedPatientSequenceSR,
papReferencedResultsSequenceSR,
papReferencedStudyComponentSequenceSR,
papStudyInstanceUIDSR,
papAccessionNumberSR,
papEndStudyRelationship
};




/*	Module : Study Scheduling			*/

enum {
papScheduledStudyStartDate,
papScheduledStudyStartTime,
papScheduledStudyStopDate,
papScheduledStudyStopTime,
papScheduledStudyLocation,
papScheduledStudyLocationAETitle,
papReasonforStudy,
papRequestingPhysician,
papRequestingService,
papRequestedProcedureDescription,
papRequestedProcedureCodeSequence,
papRequestedContrastAgent,
papEndStudyScheduling
};




/*	Module : US Frame of Reference			*/

enum {
papRegionLocationMinx0,
papRegionLocationMiny0,
papRegionLocationMaxx1,
papRegionLocationMaxy1,
papPhysicalUnitsXDirection,
papPhysicalUnitsYDirection,
papPhysicalDeltaX,
papPhysicalDeltaY,
papReferencePixelx0,
papReferencePixely0,
papRefPixelPhysicalValueX,
papRefPixelPhysicalValueY,
papEndUSFrameofReference
};




/*	Module : Visit Admission			*/

enum {
papAdmittingDate,
papAdmittingTime,
papRouteofAdmissions,
papAdmittingDiagnosisDescription,
papAdmittingDiagnosisCodeSequence,
papReferringPhysiciansNameVA,
papAddress,
papReferringPhysiciansPhoneNumbers,
papEndVisitAdmission
};




/*	Module : Visit Discharge			*/

enum {
papDischargeDate,
papDischargeTime,
papDescription,
papDischargeDiagnosisCodeSequence,
papEndVisitDischarge
};




/*	Module : Visit Identification			*/

enum {
papInstitutionNameVI,
papInstitutionAddressVI,
papInstitutionCodeSequence,
papAdmissionID,
papIssuerofAdmissionID,
papEndVisitIdentification
};




/*	Module : Visit Relationship			*/

enum {
papReferencedStudySequenceVR,
papReferencedPatientSequenceVR,
papEndVisitRelationship
};




/*	Module : Visit Scheduling			*/

enum {
papScheduledAdmissionDate,
papScheduledAdmissionTime,
papScheduledDischargeDate,
papScheduledDischargeTime,
papScheduledPatientInstitutionResidence,
papEndVisitScheduling
};




/*	Module : Visit Status				*/

enum {
papVisitStatusID,
papCurrentPatientLocationVS,
papPatientsInstitutionResidenceVS,
papVisitComments,
papEndVisitStatus
};




/*	Module : Interpretation Approval		*/

enum {
papInterpretationApproverSequence,
papInterpretationDiagnosisDescription,
papInterpretationDiagnosisCodesSequence,
papResultsDistributionListSequence,
papEndInterpretationApproval
};




/*	Module : Interpretation Transcription		*/

enum {
papInterpretationTranscriptionDate,
papInterpretationTranscriptionTime,
papInterpretationTranscriber,
papInterpretationText,
papInterpretationAuthor,
papEndInterpretationTranscription
};




/*	Module : Interpretation State			*/

enum {
papInterpretationTypeID,
papInterpretationStatusID,
papEndInterpretationState
};




/*	Module : Interpretation Recording		*/

enum {
papInterpretationRecordedDate,
papInterpretationRecordedTime,
papInterpretationRecorder,
papReferencetoRecordedSound,
papEndInterpretationRecording
};




/*	Module : Interpretation Identification		*/

enum {
papInterpretationID,
papInterpretationIDIssuer,
papEndInterpretationIdentification
};




/*	Module : Interpretation Relationship		*/

enum {
papReferencedResultsSequenceIR,
papEndInterpretationRelationship
};





/*	Module : Image Overlay Box Relationship		*/

enum {
papReferencedImageBoxSequenceOBR,
papEndImageOverlayBoxRelationship
};




/*	Module : Image Box Relationship			*/

enum {
papReferencedImageSequenceBR,
papReferencedImageOverlayBoxSequence,
papReferencedVOILUTSequence,
papEndImageBoxRelationship
};




/*	Module : Image Box Pixel Presentation		*/

enum {
papImagePosition,
papPolarity,
papMagnificationTypeIBPP,
papSmoothingTypeIBPP,
papRequestedImageSize,
papPreformattedGrayscaleImageSequence,
papPreformattedColorImageSequence,
papEndImageBoxPixelPresentation
};




/*	Module : Image Overlay Box Presentation		*/

enum {
papReferencedOverlayPlaneSequence,
papOverlayMagnificationType,
papOverlaySmoothingType,
papOverlayForegroundDensity,
papOverlayMode,
papThresholdDensity,
papEndImageOverlayBoxPresentation
};




/*	Module : External Visit Reference Sequence	*/

enum {
papReferencedVisitSequenceEVRS,
papEndExternalVisitReferenceSequence
};




/*	Module : External Study File Reference Sequence	*/

enum {
papReferencedStudySequenceESFRS,
papEndExternalStudyFileReferenceSequence
};




/*	Module : External Patient File Reference Sequence*/

enum {
papReferencedPatientSequenceEPFRS,
papEndExternalPatientFileReferenceSequence
};




/*	Module : External Papyrus_File Reference Sequence*/

enum {
papExternalPAPYRUSFileReferenceSequence,
papEndExternalPapyrus_FileReferenceSequence
};




/*	Module : Basic Film Session Relationship	*/

enum {
papReferencedFilmBoxSequence,
papEndBasicFilmSessionRelationship
};


#endif	    /* PapyEnumMiscModulesH */
